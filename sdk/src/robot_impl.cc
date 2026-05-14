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
#include <exception>
#include <stdexcept>
#include "robot_impl.hh"

namespace lebai {
namespace l_master {

Robot::RobotImpl::RobotImpl(const ::std::string &ip, bool simulator) {
  uint16_t port = simulator ? simulation_port_ : physical_machine_port_;
  http_json_rpc_connector_ = std::make_unique<HttpJsonRpcConnector>(ip, port);
  rpc_client_ = std::make_unique<RpcClient>(*http_json_rpc_connector_);
}
Robot::RobotImpl::~RobotImpl() {}

std::tuple<int, std::string> Robot::RobotImpl::call(
    const std::string &method, const std::string &req_data_str) {
  try {
    const auto params = nlohmann::json::parse(req_data_str);
    const auto response = rpc_client_->CallRaw(method, params);
    return std::make_tuple(0, response.dump());
  } catch (const jsonrpccxx::JsonRpcException &exception) {
    return std::make_tuple(exception.Code(), exception.Message());
  } catch (const nlohmann::json::exception &exception) {
    return std::make_tuple(-32600, exception.what());
  } catch (const std::exception &exception) {
    return std::make_tuple(-1, exception.what());
  }
}

bool Robot::RobotImpl::isNetworkConnected() {
  try {
    protos_json::system_proto::HelloData req;
    req.data = "world";
    const auto resp = hello(req);
    return resp.data == "hello, world";
  } catch (...) {
    return false;
  }
}

protos_json::system_proto::HelloData Robot::RobotImpl::hello(
    const protos_json::system_proto::HelloData &req) {
  return rpc_client_->Call<protos_json::system_proto::HelloData>("hello",
                                                                 {req});
}

void Robot::RobotImpl::set_auto(
    const protos_json::auto_proto::SetAutoRequest &req) {
  rpc_client_->Call<void>("set_auto", {req});
}

protos_json::auto_proto::GetAutoResponse Robot::RobotImpl::get_auto(
    const protos_json::auto_proto::GetAutoRequest &req) {
  return rpc_client_->Call<protos_json::auto_proto::GetAutoResponse>(
      "get_auto", {req});
}

int Robot::RobotImpl::start_sys() {
  rpc_client_->Call<void>("start_sys", {});
  return 0;
}

int Robot::RobotImpl::stop_sys() {
  rpc_client_->Call<void>("stop_sys", {});
  return 0;
}

int Robot::RobotImpl::powerdown() {
  rpc_client_->Call<void>("powerdown", {});
  return 0;
}

int Robot::RobotImpl::stop() {
  rpc_client_->Call<void>("stop", {});
  return 0;
}

int Robot::RobotImpl::estop() {
  rpc_client_->Call<void>("estop", {});
  return 0;
}

int Robot::RobotImpl::start_teach_mode() {
  rpc_client_->Call<void>("start_teach_mode", {});
  return 0;
}

int Robot::RobotImpl::end_teach_mode() {
  rpc_client_->Call<void>("end_teach_mode", {});
  return 0;
}

int Robot::RobotImpl::pause_move() {
  rpc_client_->Call<void>("pause_move", {});
  return 0;
}

int Robot::RobotImpl::resume_move() {
  rpc_client_->Call<void>("resume_move", {});
  return 0;
}

void Robot::RobotImpl::reboot() {
  rpc_client_->Call<void>("reboot", {});
}

protos_json::motion_proto::MotionIndex Robot::RobotImpl::move_joint(
    const protos_json::motion_proto::MoveRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>("move_joint",
                                                                   {req});
}

protos_json::motion_proto::MotionIndex Robot::RobotImpl::move_joint(
    const protos_json::motion_proto::CartesianMoveRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>("move_joint",
                                                                   {req});
}

protos_json::motion_proto::MotionIndex Robot::RobotImpl::move_linear(
    const protos_json::motion_proto::MoveRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "move_linear", {req});
}

protos_json::motion_proto::MotionIndex Robot::RobotImpl::move_linear(
    const protos_json::motion_proto::CartesianMoveRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "move_linear", {req});
}

protos_json::motion_proto::MotionIndex Robot::RobotImpl::move_circular(
    const protos_json::motion_proto::MoveCircularRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "move_circular", {req});
}
protos_json::motion_proto::MotionIndex Robot::RobotImpl::move_circular(
    const protos_json::motion_proto::CartesianMoveCircularRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "move_circular", {req});
}
protos_json::motion_proto::MotionIndex Robot::RobotImpl::toward_joint(
    const protos_json::motion_proto::MoveRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "toward_joint", {req});
}
protos_json::motion_proto::MotionIndex Robot::RobotImpl::speed_joint(
    const protos_json::motion_proto::SpeedJointRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "speed_joint", {req});
}
protos_json::motion_proto::MotionIndex Robot::RobotImpl::speed_linear(
    const protos_json::motion_proto::SpeedLinearRequest &req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "speed_linear", {req});
}
void Robot::RobotImpl::move_pvat(
    const protos_json::motion_proto::MovePvatRequest &req) {
  rpc_client_->Call<void>("move_pvat", {req});
}
void Robot::RobotImpl::wait_move(
    const protos_json::motion_proto::MotionIndex &req) {
  rpc_client_->Call<void>("wait_move", {req});
}
protos_json::motion_proto::MotionIndex Robot::RobotImpl::get_running_motion() {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "get_running_motion", {});
}
protos_json::motion_proto::GetMotionStateResponse
Robot::RobotImpl::get_motion_state(
    const protos_json::motion_proto::MotionIndex &req) {
  return rpc_client_->Call<protos_json::motion_proto::GetMotionStateResponse>(
      "get_motion_state", {req});
}
void Robot::RobotImpl::stop_move() {
  rpc_client_->Call<void>("stop_move", {});
}
void Robot::RobotImpl::skip_move() {
  rpc_client_->Call<void>("skip_move", {});
}
protos_json::system_proto::RobotState Robot::RobotImpl::get_robot_state() {
  const auto response =
      rpc_client_->Call<protos_json::system_proto::GetRobotStateResponse>(
          "get_robot_state", {});
  return response.state;
}

protos_json::system_proto::EstopReason Robot::RobotImpl::get_estop_reason() {
  const auto response =
      rpc_client_->Call<protos_json::system_proto::GetEstopReasonResponse>(
          "get_estop_reason", {});
  return response.reason;
}

protos_json::system_proto::SystemInfo Robot::RobotImpl::get_system_info() {
  return rpc_client_->Call<protos_json::system_proto::SystemInfo>(
      "get_system_info", {});
}

protos_json::system_proto::RobotInfo Robot::RobotImpl::get_robot_info() {
  return rpc_client_->Call<protos_json::system_proto::RobotInfo>(
      "get_robot_info", {});
}

protos_json::system_proto::HardwareInfo Robot::RobotImpl::get_hardware_info() {
  return rpc_client_->Call<protos_json::system_proto::HardwareInfo>(
      "get_hardware_info", {});
}

protos_json::system_proto::SoftwareInfo Robot::RobotImpl::get_software_info() {
  return rpc_client_->Call<protos_json::system_proto::SoftwareInfo>(
      "get_software_info", {});
}

protos_json::system_proto::GetBoxDevicesResponse
Robot::RobotImpl::get_box_devices(
    const protos_json::system_proto::GetBoxDevicesRequest &req) {
  return rpc_client_->Call<protos_json::system_proto::GetBoxDevicesResponse>(
      "get_box_devices", {req});
}

protos_json::message_proto::Messages Robot::RobotImpl::get_messages() {
  return rpc_client_->Call<protos_json::message_proto::Messages>(
      "get_messages", {});
}

protos_json::system_proto::PhyData Robot::RobotImpl::get_phy_data() {
  return rpc_client_->Call<protos_json::system_proto::PhyData>(
      "get_phy_data", {});
}

protos_json::kinematic_proto::KinData Robot::RobotImpl::get_kin_data() {
  return rpc_client_->Call<protos_json::kinematic_proto::KinData>(
      "get_kin_data", {});
}

protos_json::io_proto::GetDioPinResponse Robot::RobotImpl::get_di(
    const protos_json::io_proto::GetDioPinRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetDioPinResponse>("get_di",
                                                                     {req});
}
protos_json::io_proto::GetDioPinsResponse Robot::RobotImpl::get_dis(
    const protos_json::io_proto::GetDioPinsRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetDioPinsResponse>(
      "get_dis", {req});
}
protos_json::io_proto::GetDioPinResponse Robot::RobotImpl::get_do(
    const protos_json::io_proto::GetDioPinRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetDioPinResponse>("get_do",
                                                                     {req});
}
protos_json::io_proto::GetDioPinsResponse Robot::RobotImpl::get_dos(
    const protos_json::io_proto::GetDioPinsRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetDioPinsResponse>(
      "get_dos", {req});
}

void Robot::RobotImpl::set_do(
    const protos_json::io_proto::SetDoPinRequest &req) {
  rpc_client_->Call<void>("set_do", {req});
}

void Robot::RobotImpl::set_dos(
    const protos_json::io_proto::SetDoPinsRequest &req) {
  rpc_client_->Call<void>("set_dos", {req});
}

protos_json::io_proto::GetAioPinResponse Robot::RobotImpl::get_ai(
    const protos_json::io_proto::GetAioPinRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetAioPinResponse>("get_ai",
                                                                     {req});
}
protos_json::io_proto::GetAioPinsResponse Robot::RobotImpl::get_ais(
    const protos_json::io_proto::GetAioPinsRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetAioPinsResponse>(
      "get_ais", {req});
}
protos_json::io_proto::GetAioPinResponse Robot::RobotImpl::get_ao(
    const protos_json::io_proto::GetAioPinRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetAioPinResponse>("get_ao",
                                                                     {req});
}
protos_json::io_proto::GetAioPinsResponse Robot::RobotImpl::get_aos(
    const protos_json::io_proto::GetAioPinsRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetAioPinsResponse>(
      "get_aos", {req});
}
void Robot::RobotImpl::set_dio_mode(
    const protos_json::io_proto::SetDioModeRequest &req) {
  rpc_client_->Call<void>("set_dio_mode", {req});
}
protos_json::io_proto::GetDiosModeResponse Robot::RobotImpl::get_dios_mode(
    const protos_json::io_proto::GetDiosModeRequest &req) {
  return rpc_client_->Call<protos_json::io_proto::GetDiosModeResponse>(
      "get_dios_mode", {req});
}

void Robot::RobotImpl::set_ao(
    const protos_json::io_proto::SetAoPinRequest &req) {
  rpc_client_->Call<void>("set_ao", {req});
}

void Robot::RobotImpl::set_aos(
    const protos_json::io_proto::SetAoPinsRequest &req) {
  rpc_client_->Call<void>("set_aos", {req});
}

void Robot::RobotImpl::init_claw(
    const protos_json::claw_proto::InitClawRequest &req) {
  rpc_client_->Call<void>("init_claw", {req});
}

void Robot::RobotImpl::set_claw(
    const protos_json::claw_proto::SetClawRequest &req) {
  try {
    rpc_client_->Call<void>("set_claw", {req});
  } catch (const jsonrpccxx::JsonRpcException& exception) {
    if (exception.Message().find("ClawUninit") != std::string::npos) {
      return;
    }
    throw;
  }
}
protos_json::claw_proto::Claw Robot::RobotImpl::get_claw() {
  return rpc_client_->Call<protos_json::claw_proto::Claw>("get_claw", {});
}

void Robot::RobotImpl::set_led(const protos_json::led_proto::LedData &req) {
  rpc_client_->Call<void>("set_led", {req});
}

void Robot::RobotImpl::set_voice(const protos_json::led_proto::VoiceData &req) {
  rpc_client_->Call<void>("set_voice", {req});
}

void Robot::RobotImpl::set_fan(const protos_json::led_proto::FanData &req) {
  rpc_client_->Call<void>("set_fan", {req});
}

void Robot::RobotImpl::set_signal(
    const protos_json::signal_proto::SetSignalRequest &req) {
  rpc_client_->Call<void>("set_signal", {req});
}

void Robot::RobotImpl::set_signals(
    const protos_json::signal_proto::SetSignalsRequest &req) {
  rpc_client_->Call<void>("set_signals", {req});
}

protos_json::signal_proto::GetSignalResponse Robot::RobotImpl::get_signal(
    const protos_json::signal_proto::GetSignalRequest &req) {
  return rpc_client_->Call<protos_json::signal_proto::GetSignalResponse>(
      "get_signal", {req});
}

protos_json::signal_proto::GetSignalsResponse Robot::RobotImpl::get_signals(
    const protos_json::signal_proto::GetSignalsRequest &req) {
  return rpc_client_->Call<protos_json::signal_proto::GetSignalsResponse>(
      "get_signals", {req});
}

void Robot::RobotImpl::add_signal(
    const protos_json::signal_proto::SetSignalRequest &req) {
  rpc_client_->Call<void>("add_signal", {req});
}

protos_json::control_proto::TaskIndex Robot::RobotImpl::start_task(
    const protos_json::control_proto::StartTaskRequest &req) {
  return rpc_client_->Call<protos_json::control_proto::TaskIndex>("start_task",
                                                                  {req});
}

protos_json::control_proto::TaskIds Robot::RobotImpl::load_task_list() {
  return rpc_client_->Call<protos_json::control_proto::TaskIds>(
      "load_task_list", {});
}

protos_json::control_proto::TaskStdout Robot::RobotImpl::wait_task(
    const protos_json::control_proto::TaskIndex &req) {
  return rpc_client_->Call<protos_json::control_proto::TaskStdout>("wait_task",
                                                                   {req});
}

void Robot::RobotImpl::pause_task(
    const protos_json::control_proto::PauseRequest &req) {
  rpc_client_->Call<void>("pause_task", {req});
}
void Robot::RobotImpl::resume_task(
    const protos_json::control_proto::TaskIndex &req) {
  rpc_client_->Call<void>("resume_task", {req});
}
void Robot::RobotImpl::cancel_task(
    const protos_json::control_proto::TaskIndex &req) {
  rpc_client_->Call<void>("cancel_task", {req});
}
protos_json::control_proto::HookResponse Robot::RobotImpl::exec_hook(
    const protos_json::control_proto::TaskIndex &req) {
  return rpc_client_->Call<protos_json::control_proto::HookResponse>(
      "exec_hook", {req});
}
protos_json::control_proto::Task Robot::RobotImpl::load_task(
    const protos_json::control_proto::TaskIndex &req) {
  return rpc_client_->Call<protos_json::control_proto::Task>("load_task",
                                                             {req});
}
protos_json::control_proto::Task Robot::RobotImpl::load_task() {
  return rpc_client_->Call<protos_json::control_proto::Task>("load_task", {});
}

protos_json::kinematic_proto::CartesianPose Robot::RobotImpl::get_forward_kin(
    const protos_json::kinematic_proto::PoseRequest &req) {
  return rpc_client_->Call<protos_json::kinematic_proto::CartesianPose>(
      "get_forward_kin", {req});
}

protos_json::kinematic_proto::JointPose Robot::RobotImpl::get_inverse_kin(
    const protos_json::kinematic_proto::GetInverseKinRequest &req) {
  return rpc_client_->Call<protos_json::kinematic_proto::JointPose>(
      "get_inverse_kin", {req});
}

protos_json::kinematic_proto::CartesianPose Robot::RobotImpl::get_pose_trans(
    const protos_json::kinematic_proto::GetPoseTransRequest &req) {
  return rpc_client_->Call<protos_json::kinematic_proto::CartesianPose>(
      "get_pose_trans", {req});
}
protos_json::kinematic_proto::CartesianPose Robot::RobotImpl::get_pose_inverse(
    const protos_json::kinematic_proto::PoseRequest &req) {
  return rpc_client_->Call<protos_json::kinematic_proto::CartesianPose>(
      "get_pose_inverse", {req});
}

void Robot::RobotImpl::save_file(
    const protos_json::file_proto::SaveFileRequest &req) {
  rpc_client_->Call<void>("save_file", {req});
}

void Robot::RobotImpl::rename_file(
    const protos_json::file_proto::RenameFileRequest &req) {
  rpc_client_->Call<void>("rename_file", {req});
}

protos_json::file_proto::File Robot::RobotImpl::load_file(
    const protos_json::file_proto::FileIndex &req) {
  return rpc_client_->Call<protos_json::file_proto::File>("load_file", {req});
}

protos_json::file_proto::LoadFileListResponse Robot::RobotImpl::load_file_list(
    const protos_json::file_proto::LoadFileListRequest &req) {
  return rpc_client_->Call<protos_json::file_proto::LoadFileListResponse>(
      "load_file_list", {req});
}

void Robot::RobotImpl::zip(const protos_json::file_proto::ZipRequest &req) {
  rpc_client_->Call<void>("zip", {req});
}

void Robot::RobotImpl::unzip(const protos_json::file_proto::UnzipRequest &req) {
  rpc_client_->Call<void>("unzip", {req});
}

protos_json::file_proto::LoadZipListResponse Robot::RobotImpl::load_zip_list(
    const protos_json::file_proto::LoadZipListRequest &req) {
  return rpc_client_->Call<protos_json::file_proto::LoadZipListResponse>(
      "load_zip_list", {req});
}

void Robot::RobotImpl::set_payload(
    const protos_json::dynamic_proto::SetPayloadRequest &req) {
  rpc_client_->Call<void>("set_payload", {req});
}
void Robot::RobotImpl::set_payload(
    const protos_json::dynamic_proto::SetCogRequest &req) {
  rpc_client_->Call<void>("set_payload", {req});
}
void Robot::RobotImpl::set_payload(
    const protos_json::dynamic_proto::SetMassRequest &req) {
  rpc_client_->Call<void>("set_payload", {req});
}
protos_json::dynamic_proto::Payload Robot::RobotImpl::get_payload() {
  return rpc_client_->Call<protos_json::dynamic_proto::Payload>("get_payload",
                                                                {});
}
void Robot::RobotImpl::set_gravity(
    const protos_json::posture_proto::Position &req) {
  rpc_client_->Call<void>("set_gravity", {req});
}
protos_json::posture_proto::Position Robot::RobotImpl::get_gravity() {
  return rpc_client_->Call<protos_json::posture_proto::Position>("get_gravity",
                                                                 {});
}
void Robot::RobotImpl::save_payload(
    const protos_json::dynamic_proto::SavePayloadRequest &req) {
  rpc_client_->Call<void>("save_payload", {req});
}
protos_json::dynamic_proto::Payload Robot::RobotImpl::load_payload(
    const protos_json::db_proto::LoadRequest &req) {
  return rpc_client_->Call<protos_json::dynamic_proto::Payload>("load_payload",
                                                                {req});
}
protos_json::db_proto::LoadListResponse Robot::RobotImpl::load_payload_list(
    const protos_json::db_proto::LoadListRequest &req) {
  return rpc_client_->Call<protos_json::db_proto::LoadListResponse>(
      "load_payload_list", {req});
}

void Robot::RobotImpl::set_tcp(
    const protos_json::posture_proto::CartesianPose &req) {
  rpc_client_->Call<void>("set_tcp", {req});
}
protos_json::posture_proto::CartesianPose Robot::RobotImpl::get_tcp() {
  return rpc_client_->Call<protos_json::posture_proto::CartesianPose>(
      "get_tcp", {});
}
void Robot::RobotImpl::set_kin_factor(
    const protos_json::kin_factor_proto::KinFactor &req) {
  rpc_client_->Call<void>("set_kin_factor", {req});
}
protos_json::kin_factor_proto::KinFactor Robot::RobotImpl::get_kin_factor() {
  return rpc_client_->Call<protos_json::kin_factor_proto::KinFactor>(
      "get_kin_factor", {});
}
protos_json::posture_proto::CartesianPose Robot::RobotImpl::load_tcp(
    const protos_json::db_proto::LoadRequest &req) {
  return rpc_client_->Call<protos_json::posture_proto::CartesianPose>(
      "load_tcp", {req});
}
void Robot::RobotImpl::write_single_coil(
    const protos_json::modbus_proto::SetCoilRequest &req) {
  rpc_client_->Call<void>("write_single_coil", {req});
}
void Robot::RobotImpl::write_multiple_coils(
    const protos_json::modbus_proto::SetCoilsRequest &req) {
  rpc_client_->Call<void>("write_multiple_coils", {req});
}
protos_json::modbus_proto::GetCoilsResponse Robot::RobotImpl::read_coils(
    const protos_json::modbus_proto::GetCoilsRequest &req) {
  return rpc_client_->Call<protos_json::modbus_proto::GetCoilsResponse>(
      "read_coils", {req});
}
protos_json::modbus_proto::GetCoilsResponse
Robot::RobotImpl::read_discrete_inputs(
    const protos_json::modbus_proto::GetCoilsRequest &req) {
  return rpc_client_->Call<protos_json::modbus_proto::GetCoilsResponse>(
      "read_discrete_inputs", {req});
}
void Robot::RobotImpl::write_single_register(
    const protos_json::modbus_proto::SetRegisterRequest &req) {
  rpc_client_->Call<void>("write_single_register", {req});
}
void Robot::RobotImpl::write_multiple_registers(
    const protos_json::modbus_proto::SetRegistersRequest &req) {
  rpc_client_->Call<void>("write_multiple_registers", {req});
}
protos_json::modbus_proto::GetRegistersResponse
Robot::RobotImpl::read_input_registers(
    const protos_json::modbus_proto::GetRegistersRequest &req) {
  return rpc_client_->Call<protos_json::modbus_proto::GetRegistersResponse>(
      "read_input_registers", {req});
}
protos_json::modbus_proto::GetRegistersResponse
Robot::RobotImpl::read_holding_registers(
    const protos_json::modbus_proto::GetRegistersRequest &req) {
  return rpc_client_->Call<protos_json::modbus_proto::GetRegistersResponse>(
      "read_holding_registers", {req});
}

void Robot::RobotImpl::set_serial_baud_rate(
    const protos_json::serial_proto::SetSerialBaudRateRequest &req) {
  rpc_client_->Call<void>("set_serial_baud_rate", {req});
}
void Robot::RobotImpl::set_serial_parity(
    const protos_json::serial_proto::SetSerialParityRequest &req) {
  rpc_client_->Call<void>("set_serial_parity", {req});
}
void Robot::RobotImpl::set_item(
    const protos_json::storage_proto::Item &req) {
  rpc_client_->Call<void>("set_item", {req});
}
protos_json::storage_proto::Item Robot::RobotImpl::get_item(
    const protos_json::storage_proto::ItemIndex &req) {
  return rpc_client_->Call<protos_json::storage_proto::Item>("get_item", {req});
}

protos_json::storage_proto::Items Robot::RobotImpl::get_items(
    const protos_json::storage_proto::GetItemsRequest &req) {
  return rpc_client_->Call<protos_json::storage_proto::Items>("get_items",
                                                              {req});
}

void Robot::RobotImpl::enable_collision_detector() {
  rpc_client_->Call<void>("enable_collision_detector", {});
}

void Robot::RobotImpl::disable_collision_detector() {
  rpc_client_->Call<void>("disable_collision_detector", {});
}

void Robot::RobotImpl::set_collision_torque_diff(
    const protos_json::safety_proto::CollisionTorqueDiff &req) {
  rpc_client_->Call<void>("set_collision_torque_diff", {req});
}

protos_json::safety_proto::CollisionTorqueDiff
Robot::RobotImpl::get_collision_torque_diff() {
  return rpc_client_->Call<protos_json::safety_proto::CollisionTorqueDiff>(
      "get_collision_torque_diff", {});
}

void Robot::RobotImpl::set_collision_detector(
    const protos_json::safety_proto::CollisionDetector &req) {
  rpc_client_->Call<void>("set_collision_detector", {req});
}

protos_json::safety_proto::CollisionDetector
Robot::RobotImpl::get_collision_detector() {
  return rpc_client_->Call<protos_json::safety_proto::CollisionDetector>(
      "get_collision_detector", {});
}

void Robot::RobotImpl::enable_limit() {
  rpc_client_->Call<void>("enable_limit", {});
}

void Robot::RobotImpl::disable_limit() {
  rpc_client_->Call<void>("disable_limit", {});
}

void Robot::RobotImpl::set_joints_limit(
    const protos_json::safety_proto::JointsLimit &req) {
  rpc_client_->Call<void>("set_joints_limit", {req});
}

protos_json::safety_proto::JointsLimit Robot::RobotImpl::get_joints_limit() {
  return rpc_client_->Call<protos_json::safety_proto::JointsLimit>(
      "get_joints_limit", {});
}

void Robot::RobotImpl::set_cart_limit(
    const protos_json::safety_proto::CartesianLimit &req) {
  rpc_client_->Call<void>("set_cart_limit", {req});
}

protos_json::safety_proto::CartesianLimit Robot::RobotImpl::get_cart_limit() {
  return rpc_client_->Call<protos_json::safety_proto::CartesianLimit>(
      "get_cart_limit", {});
}

}  // namespace l_master
}  // namespace lebai
