/**
 * Copyright 2022 lebai.ltd
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

#pragma once

#include <lebai/robot.hh>
#include "protos/motion.hh"
#include "protos/system.hh"
#include "protos/io.hh"
#include "protos/claw.hh"
#include "protos/led.hh"
#include "protos/kinematic.hh"
#include "jsonrpc_connector.hh"
#include "protos/signal.hh"
#include "protos/control.hh"
#include "protos/file.hh"
#include "protos/dynamic.hh"
#include "protos/db.hh"
#include "protos/modbus.hh"
#include "protos/serial.hh"
#include "protos/storage.hh"
#include "protos/safety.hh"
#include "http_jsonrpc_connector.hh"
#include "protos_json/claw_proto.hh"
#include "protos_json/control_proto.hh"
#include "protos_json/db_proto.hh"
#include "protos_json/dynamic_proto.hh"
#include "protos_json/file_proto.hh"
#include "protos_json/io_proto.hh"
#include "protos_json/kin_factor_proto.hh"
#include "protos_json/kinematic_proto.hh"
#include "protos_json/led_proto.hh"
#include "protos_json/motion_proto.hh"
#include "protos_json/posture_proto.hh"
#include "protos_json/signal_proto.hh"
#include "protos_json/storage_proto.hh"
#include "protos_json/system_proto.hh"
#include "rpc_client.hh"

namespace lebai {
namespace l_master {
class Robot::RobotImpl {
 public:
  RobotImpl(const ::std::string &ip, bool simulator);
  virtual ~RobotImpl();
  std::tuple<int, std::string> call(const std::string &method,
                                    const std::string &params);
  bool isNetworkConnected();
  int start_sys();
  int stop_sys();
  int powerdown();
  int stop();
  int estop();
  int start_teach_mode();
  int end_teach_mode();
  int pause_move();
  int resume_move();
  void reboot();
  // // int movej(const std::vector<double> & p, double v, double a, double t,
  // double r, bool relative);
  motion::MotionIndex move_joint(
      const protos_json::motion_proto::MoveRequest &req);
  motion::MotionIndex move_joint(
      const protos_json::motion_proto::CartesianMoveRequest &req);
  motion::MotionIndex move_linear(
      const protos_json::motion_proto::MoveRequest &req);
  motion::MotionIndex move_linear(
      const protos_json::motion_proto::CartesianMoveRequest &req);
  motion::MotionIndex moveJoint(const motion::MoveRequest &req);
  motion::MotionIndex moveLinear(const motion::MoveRequest &req);
  motion::MotionIndex move_circular(
      const protos_json::motion_proto::MoveCircularRequest &req);
  motion::MotionIndex move_circular(
      const protos_json::motion_proto::CartesianMoveCircularRequest &req);
  motion::MotionIndex move_circular(const motion::MovecRequest &req);
  motion::MotionIndex toward_joint(
      const protos_json::motion_proto::MoveRequest &req);
  motion::MotionIndex speed_joint(
      const protos_json::motion_proto::SpeedJointRequest &req);
  motion::MotionIndex speed_linear(
      const protos_json::motion_proto::SpeedLinearRequest &req);
  void move_pvat(const protos_json::motion_proto::MovePvatRequest &req);
  void wait_move(const motion::MotionIndex &req);
  motion::MotionIndex get_running_motion();
  motion::GetMotionStateResponse get_motion_state(
      const motion::MotionIndex &req);
  void stop_move();
  system::RobotState get_robot_state();
  system::EstopReason get_estop_reason();
  protos_json::system_proto::SystemInfo get_system_info();
  system::PhyData get_phy_data();
  kinematic::KinData get_kin_data();
  io::GetDioPinResponse get_di(
      const protos_json::io_proto::GetDioPinRequest &req);
  io::GetDioPinsResponse get_dis(
      const protos_json::io_proto::GetDioPinsRequest &req);
  io::GetDioPinResponse get_do(
      const protos_json::io_proto::GetDioPinRequest &req);
  io::GetDioPinsResponse get_dos(
      const protos_json::io_proto::GetDioPinsRequest &req);
  void set_do(const protos_json::io_proto::SetDoPinRequest &req);
  io::GetAioPinResponse get_ai(
      const protos_json::io_proto::GetAioPinRequest &req);
  io::GetAioPinsResponse get_ais(
      const protos_json::io_proto::GetAioPinsRequest &req);
  io::GetAioPinResponse get_ao(
      const protos_json::io_proto::GetAioPinRequest &req);
  io::GetAioPinsResponse get_aos(
      const protos_json::io_proto::GetAioPinsRequest &req);
  void set_dio_mode(const protos_json::io_proto::SetDioModeRequest &req);
  io::GetDiosModeResponse get_dios_mode(
      const protos_json::io_proto::GetDiosModeRequest &req);
  void set_ao(const protos_json::io_proto::SetAoPinRequest &req);
  void init_claw(const protos_json::claw_proto::InitClawRequest &req);
  void set_claw(const protos_json::claw_proto::SetClawRequest &req);
  claw::Claw get_claw();
  void set_led(const protos_json::led_proto::LedData &req);
  void set_voice(const protos_json::led_proto::VoiceData &req);
  void set_fan(const protos_json::led_proto::FanData &req);
  void set_signal(const protos_json::signal_proto::SetSignalRequest &req);
  signal::GetSignalResponse get_signal(
      const protos_json::signal_proto::GetSignalRequest &req);
  void add_signal(const protos_json::signal_proto::SetSignalRequest &req);
  control::TaskIndex scene(const control::StartTaskRequest &req);
  control::TaskIds load_task_list();
  control::TaskStdout waitTask(const control::TaskIndex &req);
  void pauseTask(const control::PauseRequest &req);
  void resumeTask(const control::TaskIndex &req);
  void cancelTask(const control::TaskIndex &req);
  control::HookResponse execHook(const control::Exec &req);
  control::Task load_task(const protos_json::control_proto::TaskIndex &req);
  control::Task load_task();
  posture::CartesianPose get_forward_kin(
      const protos_json::kinematic_proto::PoseRequest &req);
  posture::JointPose get_inverse_kin(
      const protos_json::kinematic_proto::GetInverseKinRequest &req);
  posture::CartesianPose get_pose_trans(
      const protos_json::kinematic_proto::GetPoseTransRequest &req);
  posture::CartesianPose get_pose_inverse(
      const protos_json::kinematic_proto::PoseRequest &req);
  void save_file(const protos_json::file_proto::SaveFileRequest &req);
  void rename_file(const protos_json::file_proto::RenameFileRequest &req);
  file::File load_file(const protos_json::file_proto::FileIndex &req);
  file::LoadFileListResponse load_file_list(
      const protos_json::file_proto::LoadFileListRequest &req);
  void zip(const file::ZipRequest &req);
  void unzip(const file::UnzipRequest &req);
  file::LoadZipListResponse loadZipList(const file::LoadZipListRequest &req);
  void set_payload(const protos_json::dynamic_proto::SetPayloadRequest &req);
  void set_payload(const protos_json::dynamic_proto::SetCogRequest &req);
  void set_payload(const protos_json::dynamic_proto::SetMassRequest &req);
  dynamic::Payload get_payload();
  void set_gravity(const protos_json::posture_proto::Position &req);
  posture::Position get_gravity();
  void save_payload(const protos_json::dynamic_proto::SavePayloadRequest &req);
  dynamic::Payload load_payload(const protos_json::db_proto::LoadRequest &req);
  db::LoadListResponse load_payload_list(
      const protos_json::db_proto::LoadListRequest &req);
  void set_tcp(const protos_json::posture_proto::CartesianPose &req);
  posture::CartesianPose get_tcp();
  void set_kin_factor(const protos_json::kin_factor_proto::KinFactor &req);
  kinematic::KinFactor get_kin_factor();
  posture::CartesianPose load_tcp(
      const protos_json::db_proto::LoadRequest &req);
  void writeSingleCoil(const modbus::SetCoilRequest &req);
  void writeMultipleCoils(const modbus::SetCoilsRequest &req);
  modbus::GetCoilsResponse readCoils(const modbus::GetCoilsRequest &req);
  modbus::GetCoilsResponse readDiscreteInputs(
      const modbus::GetCoilsRequest &req);
  void writeSingleRegister(const modbus::SetRegisterRequest &req);
  void writeMultipleRegisters(const modbus::SetRegistersRequest &req);
  modbus::GetRegistersResponse readInputRegisters(
      const modbus::GetRegistersRequest &req);
  modbus::GetRegistersResponse readHoldingRegisters(
      const modbus::GetRegistersRequest &req);
  void setSerialBaudRateRequest(const serial::SetSerialBaudRateRequest &req);
  void setSerialParityRequest(const serial::SetSerialParityRequest &req);
  void set_item(const protos_json::storage_proto::Item &req);
  storage::Item get_item(const protos_json::storage_proto::ItemIndex &req);
  storage::Items get_items(
      const protos_json::storage_proto::GetItemsRequest &req);
  void enableCollisionDetector();
  void disableCollisionDetector();
  void setCollisionTorqueDiff(const safety::CollisionTorqueDiff &req);
  safety::CollisionTorqueDiff getCollisionTorqueDiff();
  void setCollisionDetector(const safety::CollisionDetector &req);
  safety::CollisionDetector getCollisionDetector();
  void enableLimit();
  void disableLimit();
  void setJointsLimit(const safety::JointsLimit &req);
  safety::JointsLimit getJointsLimit();
  void setCartLimit(const safety::CartesianLimit &req);
  safety::CartesianLimit getCartLimit();

 protected:
  std::unique_ptr<JSONRpcConnector> json_rpc_connector_;
  std::unique_ptr<HttpJsonRpcConnector> http_json_rpc_connector_;
  std::unique_ptr<RpcClient> rpc_client_;
  double timeout_ = 1.0;
  const uint16_t simulation_port_ = 3030;
  const uint16_t physical_machine_port_ = 3031;
  // int jsonrpc_id_ = 0;
  // WebSocketEndPoint endpoint_;
};
}  // namespace l_master
}  // namespace lebai
