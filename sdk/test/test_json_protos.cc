/**
 * Copyright 2026 lebai.ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "protos_json/auto_proto.hh"
#include "protos_json/control_proto.hh"
#include "protos_json/db_proto.hh"
#include "protos_json/dynamic_proto.hh"
#include "protos_json/file_proto.hh"
#include "protos_json/kinematic_proto.hh"
#include "protos_json/motion_proto.hh"
#include "protos_json/modbus_proto.hh"
#include "protos_json/safety_proto.hh"
#include "protos_json/serial_proto.hh"
#include "protos_json/storage_proto.hh"
#include "protos_json/system_proto.hh"

TEST(JsonAutoProtoTest, SetAutoRequestSerializesToExpectedFields) {
  protos_json::auto_proto::SetAutoRequest req;
  req.name = protos_json::auto_proto::AutoCfg::ARM_POWER;
  req.value = true;

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("value"), true);
  EXPECT_TRUE(json.contains("name"));
}

TEST(JsonSystemProtoTest, HelloDataRoundTripsThroughJson) {
  protos_json::system_proto::HelloData hello;
  hello.data = "world";

  const nlohmann::json json = hello;
  const auto parsed = json.get<protos_json::system_proto::HelloData>();

  EXPECT_EQ(json.at("data"), "world");
  EXPECT_EQ(parsed.data, "world");
}

TEST(JsonSystemProtoTest, SystemInfoParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "name":"Ubuntu",
    "kernel_version":"6.6.87.2-microsoft-standard-WSL2",
    "os_version":"16.04",
    "host_name":"3c92351aa22a",
    "memory":{"used":"8926142464","total":"16465965056","used_swap":"0","total_swap":"4294967296"},
    "disks":[{"name":"/","used":"162601762816","total":"1081101176832"}],
    "networks":[{"ifname":"eth0","kind":"ETHERNET","mac":"da:ae:c4:7d:3a:0c","ips":["172.17.0.3"],"virtual_ips":[],"gateway":"172.17.0.1","netmask":"255.255.0.0"}],
    "cpu":{"brand":"Intel(R) Core(TM) Ultra 7 255HX","num":20,"frequency":"2879","temperature":0.0,"critical":100.0}
  })");

  const auto parsed = json.get<protos_json::system_proto::SystemInfo>();

  EXPECT_EQ(parsed.name, "Ubuntu");
  EXPECT_EQ(parsed.host_name, "3c92351aa22a");
  EXPECT_EQ(parsed.memory.total, 16465965056ULL);
  ASSERT_EQ(parsed.networks.size(), 1U);
  EXPECT_EQ(parsed.networks.front().kind,
            protos_json::system_proto::NetworkKind::ETHERNET);
  EXPECT_EQ(parsed.cpu.frequency, 2879ULL);
}

TEST(JsonSystemProtoTest, RobotStateParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({"state":"ROBOT_OFF"})");
  const auto parsed =
      json.get<protos_json::system_proto::GetRobotStateResponse>();

  EXPECT_EQ(parsed.state, protos_json::system_proto::RobotState::ROBOT_OFF);
}

TEST(JsonSystemProtoTest, EstopReasonParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({"reason":"NONE"})");
  const auto parsed =
      json.get<protos_json::system_proto::GetEstopReasonResponse>();

  EXPECT_EQ(parsed.reason, protos_json::system_proto::EstopReason::NONE);
}

TEST(JsonControlProtoTest, StartTaskRequestSerializesExpectedFields) {
  protos_json::control_proto::StartTaskRequest req;
  req.name = "demo";
  req.is_parallel = false;
  req.loop_to = 1;
  req.dir = "";
  req.kind = 0;
  req.params = {"a", "b"};

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("name"), "demo");
  EXPECT_EQ(json.at("kind"), 0);
  ASSERT_EQ(json.at("params").size(), 2U);
}

TEST(JsonControlProtoTest, TaskStdoutParsesControllerPayload) {
  const auto json =
      nlohmann::json::parse(R"({"id":1,"done":true,"stdout":"ok"})");
  const auto parsed = json.get<protos_json::control_proto::TaskStdout>();

  EXPECT_EQ(parsed.id, 1U);
  EXPECT_TRUE(parsed.done);
  EXPECT_EQ(parsed.stdout_text, "ok");
}

TEST(JsonSystemProtoTest, PhyDataParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "joint_temp":[0.0,0.0,0.0,0.0,0.0,0.0],
    "joint_voltage":[0.0,0.0,0.0,0.0,0.0,0.0],
    "flange_voltage":0.0
  })");
  const auto parsed = json.get<protos_json::system_proto::PhyData>();

  ASSERT_EQ(parsed.joint_temp.size(), 6U);
  ASSERT_EQ(parsed.joint_voltage.size(), 6U);
  EXPECT_DOUBLE_EQ(parsed.flange_voltage, 0.0);
}

TEST(JsonKinematicProtoTest, KinDataParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "actual_joint_pose":[0.0,0.0,0.0,0.0,0.0,0.0],
    "actual_joint_speed":[0.0,0.0,0.0,0.0,0.0,0.0],
    "actual_joint_acc":[0.0,0.0,0.0,0.0,0.0,0.0],
    "actual_joint_torque":[0.0,-22.18,-8.09,0.0,0.0,0.0],
    "target_joint_pose":[0.0,0.0,0.0,0.0,0.0,0.0],
    "target_joint_speed":[0.0,0.0,0.0,0.0,0.0,0.0],
    "target_joint_acc":[0.0,0.0,0.0,0.0,0.0,0.0],
    "target_joint_torque":[0.0,-22.18,-8.09,0.0,0.0,0.0],
    "actual_tcp_pose":{"position":{"x":-0.54,"y":-0.20,"z":0.11},"rotation":{"euler_zyx":{"x":1.5708,"y":0.0,"z":0.0}}},
    "target_tcp_pose":{"position":{"x":-0.54,"y":-0.20,"z":0.11},"rotation":{"euler_zyx":{"x":1.5708,"y":0.0,"z":0.0}}},
    "actual_flange_pose":{"position":{"x":-0.54,"y":-0.20,"z":0.11},"rotation":{"euler_zyx":{"x":1.5708,"y":0.0,"z":0.0}}}
  })");
  const auto parsed = json.get<protos_json::kinematic_proto::KinData>();

  ASSERT_EQ(parsed.actual_joint_pose.size(), 6U);
  EXPECT_DOUBLE_EQ(parsed.actual_tcp_pose.position.x, -0.54);
  EXPECT_DOUBLE_EQ(parsed.actual_tcp_pose.rotation.euler_zyx.x, 1.5708);
}

TEST(JsonMotionProtoTest, MoveRequestSerializesJointPoseInProtoShape) {
  protos_json::motion_proto::MoveRequest req;
  req.pose.kind = 1;
  req.pose.joint.joint = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  req.param.acc = 1.0;
  req.param.velocity = 0.5;
  req.param.time = 0.0;
  req.param.radius = 0.0;

  const auto json = nlohmann::json(req);

  EXPECT_EQ(json.at("pose").at("kind"), 1);
  ASSERT_TRUE(json.at("pose").at("joint").at("joint").is_array());
  EXPECT_EQ(json.at("pose").at("joint").at("joint").size(), 6U);
}

TEST(JsonDbProtoTest, LoadRequestSerializesNameAndDir) {
  protos_json::db_proto::LoadRequest req;
  req.name = "tool0";
  req.dir = "default";

  const nlohmann::json json = req;
  const auto parsed = json.get<protos_json::db_proto::LoadRequest>();

  EXPECT_EQ(json.at("name"), "tool0");
  EXPECT_EQ(json.at("dir"), "default");
  EXPECT_EQ(parsed.name, "tool0");
  EXPECT_EQ(parsed.dir, "default");
}

TEST(JsonDynamicProtoTest, SavePayloadRequestSerializesNestedPayload) {
  protos_json::dynamic_proto::SavePayloadRequest req;
  req.name = "payload0";
  req.dir = "default";
  req.data.mass = 1.5;
  req.data.cog.x = 0.1;
  req.data.cog.y = 0.2;
  req.data.cog.z = 0.3;

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("name"), "payload0");
  EXPECT_DOUBLE_EQ(json.at("data").at("mass"), 1.5);
  EXPECT_DOUBLE_EQ(json.at("data").at("cog").at("z"), 0.3);
}

TEST(JsonDbProtoTest, LoadListResponseParsesControllerNamesField) {
  const auto json = nlohmann::json::parse(R"({"names":["payload0"]})");
  const auto parsed = json.get<protos_json::db_proto::LoadListResponse>();

  ASSERT_EQ(parsed.names.size(), 1U);
  EXPECT_EQ(parsed.names.front(), "payload0");
}

TEST(JsonDynamicProtoTest, PayloadParsesPartialControllerPayload) {
  const auto json = nlohmann::json::parse(R"({"mass":0.0})");
  const auto parsed = json.get<protos_json::dynamic_proto::Payload>();

  EXPECT_DOUBLE_EQ(parsed.mass, 0.0);
  EXPECT_DOUBLE_EQ(parsed.cog.x, 0.0);
  EXPECT_DOUBLE_EQ(parsed.cog.y, 0.0);
  EXPECT_DOUBLE_EQ(parsed.cog.z, 0.0);
}

TEST(JsonStorageProtoTest, ItemsParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "items":[{"key":"codex_storage_smoke","value":"ok"}]
  })");
  const auto parsed = json.get<protos_json::storage_proto::Items>();

  ASSERT_EQ(parsed.items.size(), 1U);
  EXPECT_EQ(parsed.items.front().key, "codex_storage_smoke");
  EXPECT_EQ(parsed.items.front().value, "ok");
}

TEST(JsonFileProtoTest, LoadFileListParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "files":[{"name":"codex_file_smoke.txt","is_dir":false}]
  })");
  const auto parsed = json.get<protos_json::file_proto::LoadFileListResponse>();

  ASSERT_EQ(parsed.files.size(), 1U);
  EXPECT_EQ(parsed.files.front().name, "codex_file_smoke.txt");
  EXPECT_FALSE(parsed.files.front().is_dir);
}

TEST(JsonFileProtoTest, ZipRequestSerializesNestedZipIndex) {
  protos_json::file_proto::ZipRequest req;
  req.zip.dir = "";
  req.zip.name = "archive.zip";
  req.files = {"codex_file_smoke.txt"};
  req.dir = "";

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("zip").at("name"), "archive.zip");
  ASSERT_EQ(json.at("files").size(), 1U);
  EXPECT_EQ(json.at("files").front(), "codex_file_smoke.txt");
}

TEST(JsonSerialProtoTest, SerialRequestsSerializeExpectedFields) {
  protos_json::serial_proto::SetSerialBaudRateRequest baud;
  baud.device = "ttyS0";
  baud.baud_rate = 115200;
  EXPECT_EQ(nlohmann::json(baud).at("device"), "ttyS0");
  EXPECT_EQ(nlohmann::json(baud).at("baud_rate"), 115200);

  protos_json::serial_proto::SetSerialParityRequest parity;
  parity.device = "ttyS0";
  parity.parity = 2;
  EXPECT_EQ(nlohmann::json(parity).at("parity"), 2);
}

TEST(JsonModbusProtoTest, RequestsAndResponsesRoundTrip) {
  protos_json::modbus_proto::SetRegisterRequest set_req;
  set_req.device = "modbus0";
  set_req.pin = "40001";
  set_req.value = 7;
  const nlohmann::json set_json = set_req;
  EXPECT_EQ(set_json.at("device"), "modbus0");
  EXPECT_EQ(set_json.at("value"), 7);

  const auto response_json = nlohmann::json::parse(R"({"values":[1,2,3]})");
  const auto response =
      response_json.get<protos_json::modbus_proto::GetRegistersResponse>();
  ASSERT_EQ(response.values.size(), 3U);
  EXPECT_EQ(response.values.back(), 3U);
}

TEST(JsonSafetyProtoTest, CollisionDetectorParsesControllerPayload) {
  const auto json = nlohmann::json::parse(
      R"({"action":"ESTOP","pause_time":0,"sensitivity":50})");
  const auto parsed =
      json.get<protos_json::safety_proto::CollisionDetector>();

  EXPECT_EQ(parsed.action, "ESTOP");
  EXPECT_EQ(parsed.pause_time, 0U);
  EXPECT_EQ(parsed.sensitivity, 50U);
}

TEST(JsonSafetyProtoTest, SafetyLimitsParseControllerPayloads) {
  const auto joints_json = nlohmann::json::parse(R"({
    "joints":[{"min_position":-6.28,"max_position":6.28,"max_a":6.28,"max_v":3.14}]
  })");
  const auto joints =
      joints_json.get<protos_json::safety_proto::JointsLimit>();

  ASSERT_EQ(joints.joints.size(), 1U);
  EXPECT_DOUBLE_EQ(joints.joints.front().min_position, -6.28);
  EXPECT_DOUBLE_EQ(joints.joints.front().max_v, 3.14);

  const auto cart_json =
      nlohmann::json::parse(R"({"max_a":1.5,"max_v":1.0,"eq_radius":1.0})");
  const auto cart =
      cart_json.get<protos_json::safety_proto::CartesianLimit>();

  EXPECT_DOUBLE_EQ(cart.max_a, 1.5);
  EXPECT_DOUBLE_EQ(cart.max_v, 1.0);
  EXPECT_DOUBLE_EQ(cart.eq_radius, 1.0);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
