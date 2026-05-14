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
#include "http_jsonrpc_connector.hh"
#include "protos_json/auto_proto.hh"
#include "protos_json/claw_proto.hh"
#include "protos_json/control_proto.hh"
#include "protos_json/db_proto.hh"
#include "protos_json/dynamic_proto.hh"
#include "protos_json/file_proto.hh"
#include "protos_json/hardware_proto.hh"
#include "protos_json/io_proto.hh"
#include "protos_json/kin_factor_proto.hh"
#include "protos_json/kinematic_proto.hh"
#include "protos_json/led_proto.hh"
#include "protos_json/message_proto.hh"
#include "protos_json/modbus_proto.hh"
#include "protos_json/motion_proto.hh"
#include "protos_json/motor_proto.hh"
#include "protos_json/posture_proto.hh"
#include "protos_json/plugin_proto.hh"
#include "protos_json/safety_proto.hh"
#include "protos_json/signal_proto.hh"
#include "protos_json/serial_proto.hh"
#include "protos_json/shortcut_proto.hh"
#include "protos_json/storage_proto.hh"
#include "protos_json/system_proto.hh"
#include "protos_json/trigger_proto.hh"
#include "protos_json/upgrade_proto.hh"
#include "rpc_client.hh"

namespace lebai {
namespace l_master {
class Robot::RobotImpl {
 public:
  RobotImpl(const ::std::string &ip, bool simulator);
  virtual ~RobotImpl();
  std::tuple<int, std::string> call(const std::string &method,
                                    const std::string &params);
  protos_json::system_proto::HelloData hello(
      const protos_json::system_proto::HelloData &req);
  void set_auto(const protos_json::auto_proto::SetAutoRequest &req);
  protos_json::auto_proto::GetAutoResponse get_auto(
      const protos_json::auto_proto::GetAutoRequest &req);
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
  protos_json::motion_proto::MotionIndex move_joint(
      const protos_json::motion_proto::MoveRequest &req);
  protos_json::motion_proto::MotionIndex move_joint(
      const protos_json::motion_proto::CartesianMoveRequest &req);
  protos_json::motion_proto::MotionIndex move_linear(
      const protos_json::motion_proto::MoveRequest &req);
  protos_json::motion_proto::MotionIndex move_linear(
      const protos_json::motion_proto::CartesianMoveRequest &req);
  protos_json::motion_proto::MotionIndex move_circular(
      const protos_json::motion_proto::MoveCircularRequest &req);
  protos_json::motion_proto::MotionIndex move_circular(
      const protos_json::motion_proto::CartesianMoveCircularRequest &req);
  protos_json::motion_proto::MotionIndex toward_joint(
      const protos_json::motion_proto::MoveRequest &req);
  protos_json::motion_proto::MotionIndex speed_joint(
      const protos_json::motion_proto::SpeedJointRequest &req);
  protos_json::motion_proto::MotionIndex speed_linear(
      const protos_json::motion_proto::SpeedLinearRequest &req);
  void move_pvat(const protos_json::motion_proto::MovePvatRequest &req);
  protos_json::db_proto::LoadListResponse load_trajectory_list(
      const protos_json::db_proto::LoadListRequest &req);
  void wait_move(const protos_json::motion_proto::MotionIndex &req);
  protos_json::motion_proto::MotionIndex get_running_motion();
  protos_json::motion_proto::GetMotionStateResponse get_motion_state(
      const protos_json::motion_proto::MotionIndex &req);
  void stop_move();
  void skip_move();
  protos_json::system_proto::RobotState get_robot_state();
  protos_json::system_proto::EstopReason get_estop_reason();
  protos_json::system_proto::SystemInfo get_system_info();
  protos_json::system_proto::RobotInfo get_robot_info();
  protos_json::system_proto::HardwareInfo get_hardware_info();
  protos_json::system_proto::SoftwareInfo get_software_info();
  protos_json::system_proto::GetBoxDevicesResponse get_box_devices(
      const protos_json::system_proto::GetBoxDevicesRequest &req);
  protos_json::db_proto::Dirs get_dirs();
  protos_json::shortcut_proto::ShortcutList get_short_poses();
  protos_json::shortcut_proto::ShortcutList get_short_tasks();
  protos_json::trigger_proto::Triggers get_triggers();
  protos_json::led_proto::LedStyles get_led_styles();
  protos_json::db_proto::LoadListResponse load_led_style_list(
      const protos_json::db_proto::LoadListRequest &req);
  protos_json::motor_proto::ServoParams get_servo_params();
  protos_json::motion_proto::Wrench get_tcp_force();
  protos_json::plugin_proto::Plugins load_plugins();
  protos_json::message_proto::Messages get_messages();
  protos_json::hardware_proto::OtaState get_ota_state();
  protos_json::upgrade_proto::CheckUpgradeResponse check_upgrade();
  protos_json::upgrade_proto::CommandStdout get_upgrade_stdout();
  protos_json::system_proto::PhyData get_phy_data();
  protos_json::kinematic_proto::KinData get_kin_data();
  protos_json::kinematic_proto::DhParams get_dh();
  protos_json::io_proto::GetDioPinResponse get_di(
      const protos_json::io_proto::GetDioPinRequest &req);
  protos_json::io_proto::GetDioPinsResponse get_dis(
      const protos_json::io_proto::GetDioPinsRequest &req);
  protos_json::io_proto::GetDioPinResponse get_do(
      const protos_json::io_proto::GetDioPinRequest &req);
  protos_json::io_proto::GetDioPinsResponse get_dos(
      const protos_json::io_proto::GetDioPinsRequest &req);
  void set_do(const protos_json::io_proto::SetDoPinRequest &req);
  void set_dos(const protos_json::io_proto::SetDoPinsRequest &req);
  protos_json::io_proto::GetAioPinResponse get_ai(
      const protos_json::io_proto::GetAioPinRequest &req);
  protos_json::io_proto::GetAioPinsResponse get_ais(
      const protos_json::io_proto::GetAioPinsRequest &req);
  protos_json::io_proto::GetAioPinResponse get_ao(
      const protos_json::io_proto::GetAioPinRequest &req);
  protos_json::io_proto::GetAioPinsResponse get_aos(
      const protos_json::io_proto::GetAioPinsRequest &req);
  void set_dio_mode(const protos_json::io_proto::SetDioModeRequest &req);
  protos_json::io_proto::GetDioModeResponse get_dio_mode(
      const protos_json::io_proto::GetDioModeRequest &req);
  protos_json::io_proto::GetDiosModeResponse get_dios_mode(
      const protos_json::io_proto::GetDiosModeRequest &req);
  void set_ao(const protos_json::io_proto::SetAoPinRequest &req);
  void set_aos(const protos_json::io_proto::SetAoPinsRequest &req);
  void init_claw(const protos_json::claw_proto::InitClawRequest &req);
  void set_claw(const protos_json::claw_proto::SetClawRequest &req);
  protos_json::claw_proto::Claw get_claw();
  void set_led(const protos_json::led_proto::LedData &req);
  void set_voice(const protos_json::led_proto::VoiceData &req);
  void set_fan(const protos_json::led_proto::FanData &req);
  void set_signal(const protos_json::signal_proto::SetSignalRequest &req);
  void set_signals(const protos_json::signal_proto::SetSignalsRequest &req);
  protos_json::signal_proto::GetSignalResponse get_signal(
      const protos_json::signal_proto::GetSignalRequest &req);
  protos_json::signal_proto::GetSignalsResponse get_signals(
      const protos_json::signal_proto::GetSignalsRequest &req);
  void add_signal(const protos_json::signal_proto::SetSignalRequest &req);
  protos_json::control_proto::TaskIndex start_task(
      const protos_json::control_proto::StartTaskRequest &req);
  protos_json::control_proto::TaskIds load_task_list();
  protos_json::control_proto::TaskStdout wait_task(
      const protos_json::control_proto::TaskIndex &req);
  void pause_task(const protos_json::control_proto::PauseRequest &req);
  void resume_task(const protos_json::control_proto::TaskIndex &req);
  void cancel_task(const protos_json::control_proto::TaskIndex &req);
  protos_json::control_proto::HookResponse exec_hook(
      const protos_json::control_proto::TaskIndex &req);
  protos_json::control_proto::Task load_task(
      const protos_json::control_proto::TaskIndex &req);
  protos_json::control_proto::Task load_task();
  protos_json::kinematic_proto::CartesianPose get_forward_kin(
      const protos_json::kinematic_proto::PoseRequest &req);
  protos_json::kinematic_proto::JointPose get_inverse_kin(
      const protos_json::kinematic_proto::GetInverseKinRequest &req);
  protos_json::kinematic_proto::CartesianPose get_pose_trans(
      const protos_json::kinematic_proto::GetPoseTransRequest &req);
  protos_json::kinematic_proto::CartesianPose get_pose_inverse(
      const protos_json::kinematic_proto::PoseRequest &req);
  void save_file(const protos_json::file_proto::SaveFileRequest &req);
  void rename_file(const protos_json::file_proto::RenameFileRequest &req);
  protos_json::file_proto::File load_file(
      const protos_json::file_proto::FileIndex &req);
  protos_json::file_proto::LoadFileListResponse load_file_list(
      const protos_json::file_proto::LoadFileListRequest &req);
  void zip(const protos_json::file_proto::ZipRequest &req);
  void unzip(const protos_json::file_proto::UnzipRequest &req);
  protos_json::file_proto::LoadZipListResponse load_zip_list(
      const protos_json::file_proto::LoadZipListRequest &req);
  void set_payload(const protos_json::dynamic_proto::SetPayloadRequest &req);
  void set_payload(const protos_json::dynamic_proto::SetCogRequest &req);
  void set_payload(const protos_json::dynamic_proto::SetMassRequest &req);
  protos_json::dynamic_proto::Payload get_payload();
  void set_gravity(const protos_json::posture_proto::Position &req);
  protos_json::posture_proto::Position get_gravity();
  void save_payload(const protos_json::dynamic_proto::SavePayloadRequest &req);
  protos_json::dynamic_proto::Payload load_payload(
      const protos_json::db_proto::LoadRequest &req);
  protos_json::db_proto::LoadListResponse load_payload_list(
      const protos_json::db_proto::LoadListRequest &req);
  void set_tcp(const protos_json::posture_proto::CartesianPose &req);
  protos_json::posture_proto::CartesianPose get_tcp();
  void set_kin_factor(const protos_json::kin_factor_proto::KinFactor &req);
  protos_json::kin_factor_proto::KinFactor get_kin_factor();
  protos_json::posture_proto::CartesianPose load_tcp(
      const protos_json::db_proto::LoadRequest &req);
  protos_json::db_proto::LoadListResponse load_tcp_list(
      const protos_json::db_proto::LoadListRequest &req);
  protos_json::db_proto::LoadListResponse load_pose_list(
      const protos_json::db_proto::LoadListRequest &req);
  protos_json::db_proto::LoadListResponse load_frame_list(
      const protos_json::db_proto::LoadListRequest &req);
  protos_json::db_proto::LoadListResponse load_structure_list(
      const protos_json::db_proto::LoadListRequest &req);
  protos_json::db_proto::LoadListResponse load_modbus_list(
      const protos_json::db_proto::LoadListRequest &req);
  void write_single_coil(const protos_json::modbus_proto::SetCoilRequest &req);
  void write_multiple_coils(
      const protos_json::modbus_proto::SetCoilsRequest &req);
  protos_json::modbus_proto::GetCoilsResponse read_coils(
      const protos_json::modbus_proto::GetCoilsRequest &req);
  protos_json::modbus_proto::GetCoilsResponse read_discrete_inputs(
      const protos_json::modbus_proto::GetCoilsRequest &req);
  void write_single_register(
      const protos_json::modbus_proto::SetRegisterRequest &req);
  void write_multiple_registers(
      const protos_json::modbus_proto::SetRegistersRequest &req);
  protos_json::modbus_proto::GetRegistersResponse read_input_registers(
      const protos_json::modbus_proto::GetRegistersRequest &req);
  protos_json::modbus_proto::GetRegistersResponse read_holding_registers(
      const protos_json::modbus_proto::GetRegistersRequest &req);
  void set_serial_baud_rate(
      const protos_json::serial_proto::SetSerialBaudRateRequest &req);
  void set_serial_parity(
      const protos_json::serial_proto::SetSerialParityRequest &req);
  void set_item(const protos_json::storage_proto::Item &req);
  protos_json::storage_proto::Item get_item(
      const protos_json::storage_proto::ItemIndex &req);
  protos_json::storage_proto::Items get_items(
      const protos_json::storage_proto::GetItemsRequest &req);
  void enable_collision_detector();
  void disable_collision_detector();
  void set_collision_torque_diff(
      const protos_json::safety_proto::CollisionTorqueDiff &req);
  protos_json::safety_proto::CollisionTorqueDiff get_collision_torque_diff();
  void set_collision_detector(
      const protos_json::safety_proto::CollisionDetector &req);
  protos_json::safety_proto::CollisionDetector get_collision_detector();
  void enable_limit();
  void disable_limit();
  void set_joints_limit(const protos_json::safety_proto::JointsLimit &req);
  protos_json::safety_proto::JointsLimit get_joints_limit();
  void set_cart_limit(const protos_json::safety_proto::CartesianLimit &req);
  protos_json::safety_proto::CartesianLimit get_cart_limit();

 protected:
  std::unique_ptr<HttpJsonRpcConnector> http_json_rpc_connector_;
  std::unique_ptr<RpcClient> rpc_client_;
  const uint16_t simulation_port_ = 3030;
  const uint16_t physical_machine_port_ = 3031;
};
}  // namespace l_master
}  // namespace lebai
