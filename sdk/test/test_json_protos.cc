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
#include "protos_json/claw_proto.hh"
#include "protos_json/control_proto.hh"
#include "protos_json/db_proto.hh"
#include "protos_json/dynamic_proto.hh"
#include "protos_json/file_proto.hh"
#include "protos_json/hardware_proto.hh"
#include "protos_json/io_proto.hh"
#include "protos_json/kinematic_proto.hh"
#include "protos_json/led_proto.hh"
#include "protos_json/message_proto.hh"
#include "protos_json/motion_proto.hh"
#include "protos_json/modbus_proto.hh"
#include "protos_json/motor_proto.hh"
#include "protos_json/multi_devices_proto.hh"
#include "protos_json/plugin_proto.hh"
#include "protos_json/posture_proto.hh"
#include "protos_json/safety_proto.hh"
#include "protos_json/serial_proto.hh"
#include "protos_json/shortcut_proto.hh"
#include "protos_json/signal_proto.hh"
#include "protos_json/storage_proto.hh"
#include "protos_json/structure_proto.hh"
#include "protos_json/system_proto.hh"
#include "protos_json/trigger_proto.hh"
#include "protos_json/upgrade_proto.hh"
#include "base64.hh"

TEST(Base64Test, EncodesAndDecodesFilePayloads) {
  const std::string input = "hello";
  const auto encoded = lebai::base64::encode(input);

  EXPECT_EQ(encoded, "aGVsbG8=");
  EXPECT_EQ(lebai::base64::decode(encoded), input);
}

TEST(JsonAutoProtoTest, SetAutoRequestSerializesToExpectedFields) {
  protos_json::auto_proto::SetAutoRequest req;
  req.name = protos_json::auto_proto::AutoCfg::ARM_POWER;
  req.value = true;

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("value"), true);
  EXPECT_TRUE(json.contains("name"));
}

TEST(JsonAutoProtoTest, GetAutoResponseDefaultsMissingValueToFalse) {
  const auto parsed =
      nlohmann::json::object().get<protos_json::auto_proto::GetAutoResponse>();

  EXPECT_FALSE(parsed.value);
}

TEST(JsonMultiDevicesProtoTest, DiscoverRobotsResponseParsesDevices) {
  const auto parsed = nlohmann::json{
      {"devices", nlohmann::json::array({{{"name", "lebai"},
                                          {"mac", "aa"},
                                          {"ip", "1.2.3.4"},
                                          {"online", true}}})}}
                          .get<protos_json::multi_devices_proto::
                                   DiscoverRobotsResponse>();

  ASSERT_EQ(parsed.devices.size(), 1U);
  EXPECT_EQ(parsed.devices.front().name, "lebai");
  EXPECT_EQ(parsed.devices.front().mac, "aa");
  EXPECT_EQ(parsed.devices.front().ip, "1.2.3.4");
  EXPECT_TRUE(parsed.devices.front().online);
}

TEST(JsonModbusProtoTest, LoadModbusRegisterListRequestSerializesDevice) {
  protos_json::modbus_proto::LoadModbusRegisterListRequest req;
  req.device = "modbus0";

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("device"), "modbus0");
}

TEST(JsonClawProtoTest, ClawAiRequestAndResponseUseAddressAndValue) {
  protos_json::claw_proto::SetClawAoRequest set_ao;
  set_ao.address = 1;
  set_ao.value = 2.5;
  const nlohmann::json set_ao_json = set_ao;
  EXPECT_EQ(set_ao_json.at("address"), 1);
  EXPECT_DOUBLE_EQ(set_ao_json.at("value"), 2.5);

  protos_json::claw_proto::GetClawAiRequest req;
  req.address = 2;

  const nlohmann::json json = req;
  protos_json::claw_proto::WaitClawAiRequest wait_req;
  wait_req.address = 2;
  wait_req.value = 0.5;
  wait_req.relation = "GTE";
  const nlohmann::json wait_json = wait_req;
  const auto response =
      nlohmann::json{{"value", 1.5}}
          .get<protos_json::claw_proto::GetClawAiResponse>();

  EXPECT_EQ(json.at("address"), 2);
  EXPECT_EQ(wait_json.at("address"), 2);
  EXPECT_DOUBLE_EQ(wait_json.at("value"), 0.5);
  EXPECT_EQ(wait_json.at("relation"), "GTE");
  EXPECT_DOUBLE_EQ(response.value, 1.5);
}

TEST(JsonPostureProtoTest, ManipulationRequestAndResponseRoundTrip) {
  protos_json::posture_proto::JointPose req;
  req.joint = {0.0, 1.0, 2.0};

  const nlohmann::json json = req;
  const auto response =
      nlohmann::json{{"manipulation", 0.25}}
          .get<protos_json::posture_proto::Manipulation>();

  EXPECT_EQ(json.at("joint").size(), 3U);
  EXPECT_DOUBLE_EQ(response.manipulation, 0.25);
}

TEST(JsonPostureProtoTest, PoseAndFrameParseControllerDefaults) {
  const auto pose =
      nlohmann::json{{"kind", "UNKNOWN"}}
          .get<protos_json::posture_proto::Pose>();
  const auto frame =
      nlohmann::json{{"position_kind", "BASE"}, {"rotation_kind", "BASE"}}
          .get<protos_json::posture_proto::CartesianFrame>();

  EXPECT_EQ(pose.kind, "UNKNOWN");
  EXPECT_TRUE(pose.joint.joint.empty());
  EXPECT_EQ(frame.position_kind, "BASE");
  EXPECT_EQ(frame.rotation_kind, "BASE");
}

TEST(JsonStructureProtoTest, StructureParsesControllerPayload) {
  const auto parsed =
      nlohmann::json{{"active", false},
                     {"name", ""},
                     {"kind", "J6M1"},
                     {"dof", 0},
                     {"dh", ""},
                     {"dyn", ""},
                     {"servo", ""}}
          .get<protos_json::structure_proto::Structure>();

  EXPECT_FALSE(parsed.active);
  EXPECT_EQ(parsed.kind, "J6M1");
  EXPECT_EQ(parsed.dof, 0U);
}

TEST(JsonIoProtoTest, SetDosRequestSerializesValues) {
  protos_json::io_proto::SetDoPinsRequest req;
  req.device = protos_json::io_proto::IoDevice::ROBOT;
  req.pin = 1;
  req.values = {1, 0};

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("pin"), 1);
  EXPECT_EQ(json.at("values").size(), 2U);
}

TEST(JsonIoProtoTest, SetAosRequestSerializesValues) {
  protos_json::io_proto::SetAoPinsRequest req;
  req.device = protos_json::io_proto::IoDevice::ROBOT;
  req.pin = 1;
  req.values = {0.2, 0.0};

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("pin"), 1);
  EXPECT_EQ(json.at("values").size(), 2U);
}

TEST(JsonIoProtoTest, ButtonIndexSerializesDeviceAndPin) {
  protos_json::io_proto::ButtonIndex req;
  req.device = protos_json::io_proto::IoDevice::FLANGE_BTN;
  req.pin = 0;

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("device"), "FLANGE_BTN");
  EXPECT_EQ(json.at("pin"), 0);
}

TEST(JsonIoProtoTest, DioModeResponsesParseModeLabels) {
  const auto single_json = nlohmann::json::parse(R"({"mode":"OUTPUT"})");
  const auto single =
      single_json.get<protos_json::io_proto::GetDioModeResponse>();

  EXPECT_TRUE(single.mode);

  const auto batch_json =
      nlohmann::json::parse(R"({"modes":["INPUT","OUTPUT"]})");
  const auto batch =
      batch_json.get<protos_json::io_proto::GetDiosModeResponse>();

  ASSERT_EQ(batch.modes.size(), 2U);
  EXPECT_FALSE(batch.modes.front());
  EXPECT_TRUE(batch.modes.back());
}

TEST(JsonHardwareProtoTest, OtaStateParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({"step":"NONE","progress":0})");
  const auto parsed = json.get<protos_json::hardware_proto::OtaState>();

  EXPECT_EQ(parsed.step, "NONE");
  EXPECT_EQ(parsed.progress, 0U);
}

TEST(JsonSignalProtoTest, SetSignalsRequestSerializesValues) {
  protos_json::signal_proto::SetSignalsRequest req;
  req.key = 10;
  req.values = {1, 2};

  const nlohmann::json json = req;

  EXPECT_EQ(json.at("key"), 10);
  EXPECT_EQ(json.at("values").size(), 2U);

  protos_json::signal_proto::WaitSignalRequest wait_req;
  wait_req.key = 20;
  wait_req.value = 2;
  wait_req.relation = "EQ";
  const nlohmann::json wait_json = wait_req;

  EXPECT_EQ(wait_json.at("key"), 20);
  EXPECT_EQ(wait_json.at("value"), 2);
  EXPECT_EQ(wait_json.at("relation"), "EQ");
}

TEST(JsonSignalProtoTest, GetSignalsResponseParsesValues) {
  const auto parsed =
      nlohmann::json{{"values", {1, 2}}}
          .get<protos_json::signal_proto::GetSignalsResponse>();

  EXPECT_EQ(parsed.values.size(), 2U);
  EXPECT_EQ(parsed.values.at(0), 1);
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

TEST(JsonSystemProtoTest, RobotHardwareSoftwareInfoParseControllerPayloads) {
  const auto robot_json = nlohmann::json::parse(R"({
    "name":"lebai-123456",
    "mac":"00:11:22:33:44:55",
    "box_model":"LM3",
    "box_sn":"C123",
    "arm_model":"J6M1",
    "arm_sn":"R123"
  })");
  const auto robot_info =
      robot_json.get<protos_json::system_proto::RobotInfo>();

  EXPECT_EQ(robot_info.name, "lebai-123456");
  EXPECT_EQ(robot_info.box_model, "LM3");
  EXPECT_EQ(robot_info.arm_model, "J6M1");

  const auto hardware_json = nlohmann::json::parse(R"({
    "comboard":{"invalid":false,"sn":"C123","version":"1.0.0","partition":"A","di_num":4,"do_num":4,"dio_num":0,"ai_num":2,"ao_num":2},
    "joints":[{"invalid":false,"sn":"J1","version":"1.0.0","partition":"B","di_num":0,"do_num":0,"dio_num":0,"ai_num":0,"ao_num":0}],
    "flange":{"invalid":false,"sn":"F1","version":"1.0.0","partition":"A","di_num":2,"do_num":2,"dio_num":0,"ai_num":0,"ao_num":0},
    "led":{"invalid":false,"sn":"L1","version":"1.0.0","partition":"A","di_num":0,"do_num":0,"dio_num":0,"ai_num":0,"ao_num":0},
    "extra_io":{"invalid":true,"sn":"","version":"","partition":"UNKNOWN","di_num":12,"do_num":12,"dio_num":0,"ai_num":2,"ao_num":2}
  })");
  const auto hardware_info =
      hardware_json.get<protos_json::system_proto::HardwareInfo>();

  EXPECT_EQ(hardware_info.comboard.di_num, 4U);
  ASSERT_EQ(hardware_info.joints.size(), 1U);
  EXPECT_EQ(hardware_info.joints.front().partition, "B");
  EXPECT_TRUE(hardware_info.extra_io.invalid);

  const auto software_json = nlohmann::json::parse(R"({
    "software":{"rc":{"version":"0.0.0","branch":"first"}}
  })");
  const auto software_info =
      software_json.get<protos_json::system_proto::SoftwareInfo>();

  ASSERT_EQ(software_info.software.count("rc"), 1U);
  EXPECT_EQ(software_info.software.at("rc").branch, "first");
}

TEST(JsonSystemProtoTest, BoxDevicesRequestAndResponseRoundTrip) {
  protos_json::system_proto::GetBoxDevicesRequest request;
  request.prefix = "tty";

  const nlohmann::json request_json = request;
  EXPECT_EQ(request_json.at("prefix"), "tty");

  const auto response_json =
      nlohmann::json::parse(R"({"devices":["ttyUSB0","ttyS0"]})");
  const auto response =
      response_json.get<protos_json::system_proto::GetBoxDevicesResponse>();

  ASSERT_EQ(response.devices.size(), 2U);
  EXPECT_EQ(response.devices.front(), "ttyUSB0");
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

TEST(JsonControlProtoTest, TasksParseControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "tasks":[{
      "id":7,
      "block_id":"block",
      "state":"RUNNING",
      "loop_count":1,
      "loop_to":2,
      "is_parallel":false,
      "is_simu":true,
      "stdout":"ok",
      "kind":"LUA",
      "dir":"tasks",
      "name":"main",
      "params":[]
    }]
  })");
  const auto parsed = json.get<protos_json::control_proto::Tasks>();

  ASSERT_EQ(parsed.tasks.size(), 1U);
  EXPECT_EQ(parsed.tasks.front().id, 7U);
  EXPECT_EQ(parsed.tasks.front().stdout_text, "ok");
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

TEST(JsonKinematicProtoTest, DhParamsParseControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "params":[{"a":0.0,"alpha":1.5708,"d":0.21583,"theta":0.0}]
  })");
  const auto parsed = json.get<protos_json::kinematic_proto::DhParams>();

  ASSERT_EQ(parsed.params.size(), 1U);
  EXPECT_DOUBLE_EQ(parsed.params.front().alpha, 1.5708);
  EXPECT_DOUBLE_EQ(parsed.params.front().d, 0.21583);
}

TEST(JsonMessageProtoTest, MessagesParseControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "messages":[
      {
        "level":"ERROR",
        "kind":"TRAJECTOR_ERROR",
        "detail":"trajector error code: 6",
        "time":"2026-04-14T05:01:28.573084023+00:00"
      }
    ]
  })");

  const auto parsed = json.get<protos_json::message_proto::Messages>();

  ASSERT_EQ(parsed.messages.size(), 1U);
  EXPECT_EQ(parsed.messages.front().level, "ERROR");
  EXPECT_EQ(parsed.messages.front().kind, "TRAJECTOR_ERROR");
  EXPECT_EQ(parsed.messages.front().time,
            "2026-04-14T05:01:28.573084023+00:00");
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

TEST(JsonMotionProtoTest, WrenchParsesControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "force":{"x":1.0,"y":2.0,"z":3.0},
    "torque":{"x":4.0,"y":5.0,"z":6.0}
  })");
  const auto parsed = json.get<protos_json::motion_proto::Wrench>();

  EXPECT_DOUBLE_EQ(parsed.force.x, 1.0);
  EXPECT_DOUBLE_EQ(parsed.torque.z, 6.0);
}

TEST(JsonMotionProtoTest, TrajectoryParsesControllerPayload) {
  protos_json::motion_proto::StartRecordTrajectoryRequest start_req;
  start_req.kind = "PVAT";
  start_req.duration = 0.01;
  const nlohmann::json start_json = start_req;
  EXPECT_EQ(start_json.at("kind"), "PVAT");
  EXPECT_DOUBLE_EQ(start_json.at("duration"), 0.01);

  protos_json::motion_proto::EndRecordTrajectoryRequest end_req;
  end_req.name = "recorded";
  end_req.dir = "";
  const nlohmann::json end_json = end_req;
  EXPECT_EQ(end_json.at("name"), "recorded");

  const auto parsed =
      nlohmann::json{{"kind", "PT"}, {"data", nlohmann::json::array()}}
          .get<protos_json::motion_proto::Trajectory>();

  EXPECT_EQ(parsed.kind, "PT");
  EXPECT_TRUE(parsed.data.empty());
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

TEST(JsonDbProtoTest, DirRequestsSerializeControllerFields) {
  protos_json::db_proto::Dir create_req;
  create_req.name = "codex_dir_smoke";
  create_req.id = 0;
  const nlohmann::json create_json = create_req;

  protos_json::db_proto::UpdateDirRequest update_req;
  update_req.from = "codex_dir_smoke";
  update_req.to = "codex_dir_smoke_renamed";
  const nlohmann::json update_json = update_req;

  EXPECT_EQ(create_json.at("name"), "codex_dir_smoke");
  EXPECT_EQ(create_json.at("id"), 0);
  EXPECT_EQ(update_json.at("from"), "codex_dir_smoke");
  EXPECT_EQ(update_json.at("to"), "codex_dir_smoke_renamed");
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

TEST(JsonDbProtoTest, DirsParsesControllerPayload) {
  const auto json =
      nlohmann::json::parse(R"({"dirs":[{"name":"default","id":1}]})");
  const auto parsed = json.get<protos_json::db_proto::Dirs>();

  ASSERT_EQ(parsed.dirs.size(), 1U);
  EXPECT_EQ(parsed.dirs.front().name, "default");
  EXPECT_EQ(parsed.dirs.front().id, 1U);
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

  protos_json::serial_proto::SetSerialTimeoutRequest timeout;
  timeout.device = "ttyS0";
  timeout.timeout = 800;
  EXPECT_EQ(nlohmann::json(timeout).at("timeout"), 800);

  protos_json::serial_proto::WriteSerialRequest write;
  write.device = "ttyS0";
  write.data = {1, 2, 3};
  EXPECT_EQ(nlohmann::json(write).at("data").size(), 3U);

  const auto read = nlohmann::json::parse(R"({"data":[1,2,3]})")
                        .get<protos_json::serial_proto::ReadSerialResponse>();
  EXPECT_EQ(read.data, std::vector<unsigned int>({1, 2, 3}));
}

TEST(JsonShortcutProtoTest, ShortcutListParsesControllerPayload) {
  const auto json = nlohmann::json::parse(
      R"({"list":[{"id":1,"dir":"default","name":"home"}]})");
  const auto parsed = json.get<protos_json::shortcut_proto::ShortcutList>();

  ASSERT_EQ(parsed.list.size(), 1U);
  EXPECT_EQ(parsed.list.front().id, 1U);
  EXPECT_EQ(parsed.list.front().dir, "default");
  EXPECT_EQ(parsed.list.front().name, "home");
}

TEST(JsonTriggerProtoTest, TriggersParseControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "triggers":[
      {
        "condition":{
          "pressed":[{"device":"SHOULDER","pin":0}],
          "button":{"device":"FLANGE_BTN","pin":1},
          "status":{"state":"CLICK","time":5}
        },
        "function":"SET_ZERO"
      }
    ]
  })");
  const auto parsed = json.get<protos_json::trigger_proto::Triggers>();

  ASSERT_EQ(parsed.triggers.size(), 1U);
  EXPECT_EQ(parsed.triggers.front().function, "SET_ZERO");
  ASSERT_EQ(parsed.triggers.front().condition.pressed.size(), 1U);
  EXPECT_EQ(parsed.triggers.front().condition.pressed.front().device,
            "SHOULDER");
  EXPECT_EQ(parsed.triggers.front().condition.status.state, "CLICK");
}

TEST(JsonLedProtoTest, LedStylesParseControllerPayload) {
  protos_json::led_proto::LedStyleItem item;
  item.state = "ESTOP";
  item.style.led.mode = "BLINK";
  item.style.led.speed = "FAST";
  item.style.led.colors = {"RED"};
  item.style.voice = "OFF";
  item.style.volume = "MID";
  const nlohmann::json item_json = item;
  EXPECT_EQ(item_json.at("state"), "ESTOP");
  EXPECT_EQ(item_json.at("style").at("voice"), "OFF");

  const auto json = nlohmann::json::parse(R"({
    "styles":{
      "5":{
        "led":{"mode":"OPEN_LED","speed":"NORMAL","colors":["GREEN"]},
        "voice":"OFF",
        "volume":"MID"
      }
    }
  })");
  const auto parsed = json.get<protos_json::led_proto::LedStyles>();

  ASSERT_EQ(parsed.styles.count("5"), 1U);
  EXPECT_EQ(parsed.styles.at("5").led.mode, "OPEN_LED");
  EXPECT_EQ(parsed.styles.at("5").led.colors.front(), "GREEN");
  EXPECT_EQ(parsed.styles.at("5").volume, "MID");
}

TEST(JsonLedProtoTest, LedStyleParsesControllerDefaults) {
  const auto parsed =
      nlohmann::json{{"voice", "OFF"}, {"volume", "MUTE"}}
          .get<protos_json::led_proto::LedStyle>();

  EXPECT_EQ(parsed.voice, "OFF");
  EXPECT_EQ(parsed.volume, "MUTE");
  EXPECT_TRUE(parsed.led.mode.empty());
  EXPECT_TRUE(parsed.led.colors.empty());
}

TEST(JsonMotorProtoTest, ServoParamsParseControllerPayload) {
  const auto json = nlohmann::json::parse(R"({
    "params":[
      {"position_kp":10.0,"speed_kp":18.0,"speed_it":300.0,"torque_cmd_filter":1.0}
    ]
  })");
  const auto parsed = json.get<protos_json::motor_proto::ServoParams>();

  ASSERT_EQ(parsed.params.size(), 1U);
  EXPECT_DOUBLE_EQ(parsed.params.front().position_kp, 10.0);
  EXPECT_DOUBLE_EQ(parsed.params.front().speed_it, 300.0);
}

TEST(JsonPluginProtoTest, PluginsParseControllerPayload) {
  const auto store_json = nlohmann::json::parse(R"({
    "plugins":[{"name":"camera","url":"https://example.invalid/camera.zip"}]
  })");
  const auto store = store_json.get<protos_json::plugin_proto::PluginStore>();

  ASSERT_EQ(store.plugins.size(), 1U);
  EXPECT_EQ(store.plugins.front().name, "camera");
  EXPECT_EQ(store.plugins.front().url, "https://example.invalid/camera.zip");

  const auto json = nlohmann::json::parse(R"({
    "plugins":[
      {
        "name":"demo",
        "description":"Demo plugin",
        "homepage":"https://example.invalid",
        "auto_restart":true,
        "web":false,
        "daemon":true,
        "cmd":false,
        "enable":true
      }
    ]
  })");
  const auto parsed = json.get<protos_json::plugin_proto::Plugins>();

  ASSERT_EQ(parsed.plugins.size(), 1U);
  EXPECT_EQ(parsed.plugins.front().name, "demo");
  EXPECT_TRUE(parsed.plugins.front().auto_restart);
  EXPECT_TRUE(parsed.plugins.front().daemon);
}

TEST(JsonPluginProtoTest, PluginIndexAndCommandStdoutRoundTrip) {
  protos_json::plugin_proto::PluginIndex index;
  index.name = "demo";

  const nlohmann::json index_json = index;
  const auto stdout_data =
      nlohmann::json{{"done", true},
                     {"stdout", "out"},
                     {"stderr", ""},
                     {"code", 0}}
          .get<protos_json::plugin_proto::CommandStdout>();

  EXPECT_EQ(index_json.at("name"), "demo");
  EXPECT_TRUE(stdout_data.done);
  EXPECT_EQ(stdout_data.stdout_text, "out");
  EXPECT_EQ(stdout_data.code, 0);
}

TEST(JsonUpgradeProtoTest, UpgradeResponsesParseControllerPayloads) {
  const auto check_json =
      nlohmann::json::parse(R"({"need_upgrade":false,"introduction":""})");
  const auto check =
      check_json.get<protos_json::upgrade_proto::CheckUpgradeResponse>();

  EXPECT_FALSE(check.need_upgrade);
  EXPECT_EQ(check.introduction, "");

  const auto stdout_json =
      nlohmann::json::parse(R"({"done":true,"stdout":"","stderr":"","code":0})");
  const auto stdout_data =
      stdout_json.get<protos_json::upgrade_proto::CommandStdout>();

  EXPECT_TRUE(stdout_data.done);
  EXPECT_EQ(stdout_data.code, 0);
}

TEST(JsonModbusProtoTest, RequestsAndResponsesRoundTrip) {
  protos_json::modbus_proto::SetModbusTimeoutRequest timeout_req;
  timeout_req.device = "modbus0";
  timeout_req.timeout = 600;
  const nlohmann::json timeout_json = timeout_req;
  EXPECT_EQ(timeout_json.at("device"), "modbus0");
  EXPECT_EQ(timeout_json.at("timeout"), 600);

  protos_json::modbus_proto::SetModbusRetryRequest retry_req;
  retry_req.device = "modbus0";
  retry_req.retry = 2;
  const nlohmann::json retry_json = retry_req;
  EXPECT_EQ(retry_json.at("device"), "modbus0");
  EXPECT_EQ(retry_json.at("retry"), 2);

  protos_json::modbus_proto::DisconnectModbusRequest disconnect_req;
  disconnect_req.device = "modbus0";
  const nlohmann::json disconnect_json = disconnect_req;
  EXPECT_EQ(disconnect_json.at("device"), "modbus0");

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

TEST(JsonModbusProtoTest, ModbusResourcesParseControllerPayloads) {
  const auto modbus =
      nlohmann::json{{"kind", "ROBOT"},
                     {"address", ""},
                     {"port", 0},
                     {"slave_id", 0}}
          .get<protos_json::modbus_proto::Modbus>();
  const auto reg =
      nlohmann::json{{"kind", "DISCRETE_INPUT"}, {"address", 0}}
          .get<protos_json::modbus_proto::ModbusRegister>();

  EXPECT_EQ(modbus.kind, "ROBOT");
  EXPECT_EQ(modbus.slave_id, 0U);
  EXPECT_EQ(reg.kind, "DISCRETE_INPUT");
  EXPECT_EQ(reg.address, 0U);
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
