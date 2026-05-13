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
#include "protos_json/kinematic_proto.hh"
#include "protos_json/motion_proto.hh"
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

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
