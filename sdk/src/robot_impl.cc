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
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <exception>
#include "robot_impl.hh"
#include "protos/utils.hh"

namespace lebai {
namespace l_master {
namespace {

auto convertToPostureCartesianPose(
    const protos_json::kinematic_proto::CartesianPose &source)
    -> posture::CartesianPose {
  posture::CartesianPose pose;
  pose.mutable_position()->set_x(source.position.x);
  pose.mutable_position()->set_y(source.position.y);
  pose.mutable_position()->set_z(source.position.z);
  pose.mutable_rotation()->mutable_euler_zyx()->set_x(
      source.rotation.euler_zyx.x);
  pose.mutable_rotation()->mutable_euler_zyx()->set_y(
      source.rotation.euler_zyx.y);
  pose.mutable_rotation()->mutable_euler_zyx()->set_z(
      source.rotation.euler_zyx.z);
  return pose;
}

}  // namespace

Robot::RobotImpl::RobotImpl(const ::std::string &ip, bool simulator) {
  uint16_t port = simulator ? simulation_port_ : physical_machine_port_;
  json_rpc_connector_ = std::make_unique<JSONRpcConnector>(ip, port);
  http_json_rpc_connector_ = std::make_unique<HttpJsonRpcConnector>(ip, port);
  rpc_client_ = std::make_unique<RpcClient>(*http_json_rpc_connector_);
  unsigned int i = 0;
  unsigned int count = timeout_ / 0.1;
  while (i < count) {
    if (json_rpc_connector_->GetConnectionStatus() == JSONRpcConnector::kOpen) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    i++;
  }
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
  // return json_rpc_connector_->Ping();
  return json_rpc_connector_->GetConnectionStatus() == JSONRpcConnector::kOpen;
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

motion::MotionIndex Robot::RobotImpl::move_joint(
    const protos_json::motion_proto::MoveRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>("move_joint",
                                                                {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}

motion::MotionIndex Robot::RobotImpl::move_joint(
    const protos_json::motion_proto::CartesianMoveRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>("move_joint",
                                                                {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}

motion::MotionIndex Robot::RobotImpl::move_linear(
    const protos_json::motion_proto::MoveRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>("move_linear",
                                                                {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}

motion::MotionIndex Robot::RobotImpl::move_linear(
    const protos_json::motion_proto::CartesianMoveRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>("move_linear",
                                                                {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}

motion::MotionIndex Robot::RobotImpl::moveJoint(
    const motion::MoveRequest &req) {
  std::string resp;
  json_rpc_connector_->CallRpc("move_joint", req.ToJSONString(), &resp);
  motion::MotionIndex motion_resp;
  motion_resp.FromJSONString(resp);
  return motion_resp;
}
motion::MotionIndex Robot::RobotImpl::moveLinear(
    const motion::MoveRequest &req) {
  std::string resp;
  json_rpc_connector_->CallRpc("move_linear", req.ToJSONString(), &resp);
  motion::MotionIndex motion_resp;
  motion_resp.FromJSONString(resp);
  return motion_resp;
}
motion::MotionIndex Robot::RobotImpl::move_circular(
    const protos_json::motion_proto::MoveCircularRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
          "move_circular", {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}
motion::MotionIndex Robot::RobotImpl::move_circular(
    const protos_json::motion_proto::CartesianMoveCircularRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
          "move_circular", {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}
motion::MotionIndex Robot::RobotImpl::move_circular(
    const motion::MovecRequest &req) {
  std::string resp;
  json_rpc_connector_->CallRpc("move_circular", req.ToJSONString(), &resp);
  motion::MotionIndex motion_resp;
  motion_resp.FromJSONString(resp);
  return motion_resp;
}
motion::MotionIndex Robot::RobotImpl::toward_joint(
    const protos_json::motion_proto::MoveRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
          "toward_joint", {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}
motion::MotionIndex Robot::RobotImpl::speed_joint(
    const protos_json::motion_proto::SpeedJointRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
          "speed_joint", {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}
motion::MotionIndex Robot::RobotImpl::speed_linear(
    const protos_json::motion_proto::SpeedLinearRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
          "speed_linear", {req});
  motion::MotionIndex motion_resp;
  motion_resp.set_id(response.id);
  return motion_resp;
}
void Robot::RobotImpl::move_pvat(
    const protos_json::motion_proto::MovePvatRequest &req) {
  rpc_client_->Call<void>("move_pvat", {req});
}
void Robot::RobotImpl::wait_move(const motion::MotionIndex &req) {
  protos_json::motion_proto::MotionIndex typed_req;
  typed_req.id = req.id();
  rpc_client_->Call<void>("wait_move", {typed_req});
}
motion::MotionIndex Robot::RobotImpl::get_running_motion() {
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
          "get_running_motion", {});
  motion::MotionIndex get_running_motion_resp;
  get_running_motion_resp.set_id(response.id);
  return get_running_motion_resp;
}
motion::GetMotionStateResponse Robot::RobotImpl::get_motion_state(
    const motion::MotionIndex &req) {
  protos_json::motion_proto::MotionIndex typed_req;
  typed_req.id = req.id();
  const auto response =
      rpc_client_->Call<protos_json::motion_proto::GetMotionStateResponse>(
          "get_motion_state", {typed_req});
  motion::GetMotionStateResponse get_motion_state_resp;
  get_motion_state_resp.set_state(
      static_cast<motion::MotionState>(response.state));
  return get_motion_state_resp;
}
void Robot::RobotImpl::stop_move() {
  rpc_client_->Call<void>("stop_move", {});
}
system::RobotState Robot::RobotImpl::get_robot_state() {
  const auto response =
      rpc_client_->Call<protos_json::system_proto::GetRobotStateResponse>(
          "get_robot_state", {});
  return static_cast<system::RobotState>(response.state);
}

system::EstopReason Robot::RobotImpl::get_estop_reason() {
  const auto response =
      rpc_client_->Call<protos_json::system_proto::GetEstopReasonResponse>(
          "get_estop_reason", {});
  return static_cast<system::EstopReason>(response.reason);
}

protos_json::system_proto::SystemInfo Robot::RobotImpl::get_system_info() {
  return rpc_client_->Call<protos_json::system_proto::SystemInfo>(
      "get_system_info", {});
}

system::PhyData Robot::RobotImpl::get_phy_data() {
  const auto response = rpc_client_->Call<protos_json::system_proto::PhyData>(
      "get_phy_data", {});
  system::PhyData phy_data;
  phy_data.set_joint_temp(response.joint_temp);
  phy_data.set_joint_voltage(response.joint_voltage);
  phy_data.set_flange_voltage(response.flange_voltage);
  return phy_data;
}

kinematic::KinData Robot::RobotImpl::get_kin_data() {
  const auto response = rpc_client_->Call<protos_json::kinematic_proto::KinData>(
      "get_kin_data", {});
  kinematic::KinData kin_data;
  kin_data.set_actual_joint_pose(response.actual_joint_pose);
  kin_data.set_actual_joint_speed(response.actual_joint_speed);
  kin_data.set_actual_joint_acc(response.actual_joint_acc);
  kin_data.set_actual_joint_torque(response.actual_joint_torque);
  kin_data.set_target_joint_pose(response.target_joint_pose);
  kin_data.set_target_joint_speed(response.target_joint_speed);
  kin_data.set_target_joint_acc(response.target_joint_acc);
  kin_data.set_target_joint_torque(response.target_joint_torque);
  kin_data.set_actual_tcp_pose(
      convertToPostureCartesianPose(response.actual_tcp_pose));
  kin_data.set_target_tcp_pose(
      convertToPostureCartesianPose(response.target_tcp_pose));
  kin_data.set_actual_flange_pose(
      convertToPostureCartesianPose(response.actual_flange_pose));
  return kin_data;
}

io::GetDioPinResponse Robot::RobotImpl::get_di(
    const protos_json::io_proto::GetDioPinRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetDioPinResponse>("get_di",
                                                                  {req});
  io::GetDioPinResponse get_dio_resp;
  get_dio_resp.set_value(response.value);
  return get_dio_resp;
}
io::GetDioPinsResponse Robot::RobotImpl::get_dis(
    const protos_json::io_proto::GetDioPinsRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetDioPinsResponse>("get_dis",
                                                                   {req});
  io::GetDioPinsResponse resp;
  std::vector<unsigned int> values(response.values.begin(), response.values.end());
  resp.set_values(values);
  return resp;
}
io::GetDioPinResponse Robot::RobotImpl::get_do(
    const protos_json::io_proto::GetDioPinRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetDioPinResponse>("get_do",
                                                                  {req});
  io::GetDioPinResponse get_dio_resp;
  get_dio_resp.set_value(response.value);
  return get_dio_resp;
}
io::GetDioPinsResponse Robot::RobotImpl::get_dos(
    const protos_json::io_proto::GetDioPinsRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetDioPinsResponse>("get_dos",
                                                                   {req});
  io::GetDioPinsResponse resp;
  std::vector<unsigned int> values(response.values.begin(), response.values.end());
  resp.set_values(values);
  return resp;
}

void Robot::RobotImpl::set_do(
    const protos_json::io_proto::SetDoPinRequest &req) {
  rpc_client_->Call<void>("set_do", {req});
}

io::GetAioPinResponse Robot::RobotImpl::get_ai(
    const protos_json::io_proto::GetAioPinRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetAioPinResponse>("get_ai",
                                                                  {req});
  io::GetAioPinResponse get_aio_resp;
  get_aio_resp.set_value(response.value);
  return get_aio_resp;
}
io::GetAioPinsResponse Robot::RobotImpl::get_ais(
    const protos_json::io_proto::GetAioPinsRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetAioPinsResponse>("get_ais",
                                                                   {req});
  io::GetAioPinsResponse get_aio_resp;
  get_aio_resp.set_values(response.values);
  return get_aio_resp;
}
io::GetAioPinResponse Robot::RobotImpl::get_ao(
    const protos_json::io_proto::GetAioPinRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetAioPinResponse>("get_ao",
                                                                  {req});
  io::GetAioPinResponse get_aio_resp;
  get_aio_resp.set_value(response.value);
  return get_aio_resp;
}
io::GetAioPinsResponse Robot::RobotImpl::get_aos(
    const protos_json::io_proto::GetAioPinsRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetAioPinsResponse>("get_aos",
                                                                   {req});
  io::GetAioPinsResponse get_aio_resp;
  get_aio_resp.set_values(response.values);
  return get_aio_resp;
}
void Robot::RobotImpl::set_dio_mode(
    const protos_json::io_proto::SetDioModeRequest &req) {
  rpc_client_->Call<void>("set_dio_mode", {req});
}
io::GetDiosModeResponse Robot::RobotImpl::get_dios_mode(
    const protos_json::io_proto::GetDiosModeRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::io_proto::GetDiosModeResponse>(
          "get_dios_mode", {req});
  io::GetDiosModeResponse resp_;
  resp_.set_values(response.modes);
  return resp_;
}

void Robot::RobotImpl::set_ao(
    const protos_json::io_proto::SetAoPinRequest &req) {
  rpc_client_->Call<void>("set_ao", {req});
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
claw::Claw Robot::RobotImpl::get_claw() {
  const auto response =
      rpc_client_->Call<protos_json::claw_proto::Claw>("get_claw", {});
  claw::Claw get_claw_resp;
  get_claw_resp.set_force(response.force);
  get_claw_resp.set_amplitude(response.amplitude);
  get_claw_resp.set_hold_on(response.hold_on);
  return get_claw_resp;
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
signal::GetSignalResponse Robot::RobotImpl::get_signal(
    const protos_json::signal_proto::GetSignalRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::signal_proto::GetSignalResponse>(
          "get_signal", {req});
  signal::GetSignalResponse set_signal_resp;
  set_signal_resp.set_value(response.value);
  return set_signal_resp;
}
void Robot::RobotImpl::add_signal(
    const protos_json::signal_proto::SetSignalRequest &req) {
  rpc_client_->Call<void>("add_signal", {req});
}

control::TaskIndex Robot::RobotImpl::scene(
    const control::StartTaskRequest &req) {
  std::string resp;
  json_rpc_connector_->CallRpc("start_task", req.ToJSONString(), &resp);
  control::TaskIndex start_task_resp;
  start_task_resp.FromJSONString(resp);
  return start_task_resp;
}

control::TaskIds Robot::RobotImpl::load_task_list() {
  const auto response =
      rpc_client_->Call<protos_json::control_proto::TaskIds>("load_task_list",
                                                             {});
  control::TaskIds list_resp;
  list_resp.set_ids(response.ids);
  return list_resp;
}

control::TaskStdout Robot::RobotImpl::waitTask(const control::TaskIndex &req) {
  std::string resp;
  json_rpc_connector_->CallRpc("wait_task", req.ToJSONString(), &resp);
  control::TaskStdout wait_task_resp;
  wait_task_resp.FromJSONString(resp);
  return wait_task_resp;
}

void Robot::RobotImpl::pauseTask(const control::PauseRequest &req) {
  json_rpc_connector_->CallRpc("pause_task", req.ToJSONString(), nullptr);
}
void Robot::RobotImpl::resumeTask(const control::TaskIndex &req) {
  json_rpc_connector_->CallRpc("resume_task", req.ToJSONString(), nullptr);
}
void Robot::RobotImpl::cancelTask(const control::TaskIndex &req) {
  json_rpc_connector_->CallRpc("cancel_task", req.ToJSONString(), nullptr);
}
control::HookResponse Robot::RobotImpl::execHook(const control::Exec &req) {
  std::string resp;
  json_rpc_connector_->CallRpc("exec_hook", req.ToJSONString(), &resp);
  control::HookResponse hook_resp;
  hook_resp.FromJSONString(resp);
  return hook_resp;
}
control::Task Robot::RobotImpl::load_task(
    const protos_json::control_proto::TaskIndex &req) {
  const auto response = rpc_client_->Call<protos_json::control_proto::Task>(
      "load_task", {req});
  control::Task task_resp;
  task_resp.set_id(response.id);
  task_resp.set_block_id(response.block_id);
  task_resp.set_state(static_cast<control::TaskState>(response.state));
  task_resp.set_loopc(response.loop_count);
  task_resp.set_loopt(response.loop_to);
  task_resp.set_is_parallel(response.is_parallel);
  task_resp.set_is_simu(response.is_simu);
  task_resp.set_out(response.stdout_text);
  task_resp.set_pre_pause(response.pre_pause);
  task_resp.set_kind(static_cast<control::TaskKind>(response.kind));
  task_resp.set_dir(response.dir);
  task_resp.set_name(response.name);
  task_resp.set_params(response.params);
  return task_resp;
}
control::Task Robot::RobotImpl::load_task() {
  const auto response =
      rpc_client_->Call<protos_json::control_proto::Task>("load_task", {});
  control::Task task_resp;
  task_resp.set_id(response.id);
  task_resp.set_block_id(response.block_id);
  task_resp.set_state(static_cast<control::TaskState>(response.state));
  task_resp.set_loopc(response.loop_count);
  task_resp.set_loopt(response.loop_to);
  task_resp.set_is_parallel(response.is_parallel);
  task_resp.set_is_simu(response.is_simu);
  task_resp.set_out(response.stdout_text);
  task_resp.set_pre_pause(response.pre_pause);
  task_resp.set_kind(static_cast<control::TaskKind>(response.kind));
  task_resp.set_dir(response.dir);
  task_resp.set_name(response.name);
  task_resp.set_params(response.params);
  return task_resp;
}

posture::CartesianPose Robot::RobotImpl::get_forward_kin(
    const protos_json::kinematic_proto::PoseRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::kinematic_proto::CartesianPose>(
          "get_forward_kin", {req});
  posture::CartesianPose get_forward_kin_resp;
  get_forward_kin_resp.mutable_position()->set_x(response.position.x);
  get_forward_kin_resp.mutable_position()->set_y(response.position.y);
  get_forward_kin_resp.mutable_position()->set_z(response.position.z);
  get_forward_kin_resp.mutable_rotation()->mutable_euler_zyx()->set_x(
      response.rotation.euler_zyx.x);
  get_forward_kin_resp.mutable_rotation()->mutable_euler_zyx()->set_y(
      response.rotation.euler_zyx.y);
  get_forward_kin_resp.mutable_rotation()->mutable_euler_zyx()->set_z(
      response.rotation.euler_zyx.z);
  return get_forward_kin_resp;
}

posture::JointPose Robot::RobotImpl::get_inverse_kin(
    const protos_json::kinematic_proto::GetInverseKinRequest &req) {
  const auto response = rpc_client_->Call<protos_json::kinematic_proto::JointPose>(
      "get_inverse_kin", {req});
  posture::JointPose get_inverse_kin_resp;
  get_inverse_kin_resp.set_joint(response.joint);
  return get_inverse_kin_resp;
}

posture::CartesianPose Robot::RobotImpl::get_pose_trans(
    const protos_json::kinematic_proto::GetPoseTransRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::kinematic_proto::CartesianPose>(
          "get_pose_trans", {req});
  posture::CartesianPose resp;
  resp.mutable_position()->set_x(response.position.x);
  resp.mutable_position()->set_y(response.position.y);
  resp.mutable_position()->set_z(response.position.z);
  resp.mutable_rotation()->mutable_euler_zyx()->set_x(
      response.rotation.euler_zyx.x);
  resp.mutable_rotation()->mutable_euler_zyx()->set_y(
      response.rotation.euler_zyx.y);
  resp.mutable_rotation()->mutable_euler_zyx()->set_z(
      response.rotation.euler_zyx.z);
  return resp;
}
posture::CartesianPose Robot::RobotImpl::get_pose_inverse(
    const protos_json::kinematic_proto::PoseRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::kinematic_proto::CartesianPose>(
          "get_pose_inverse", {req});
  posture::CartesianPose resp;
  resp.mutable_position()->set_x(response.position.x);
  resp.mutable_position()->set_y(response.position.y);
  resp.mutable_position()->set_z(response.position.z);
  resp.mutable_rotation()->mutable_euler_zyx()->set_x(
      response.rotation.euler_zyx.x);
  resp.mutable_rotation()->mutable_euler_zyx()->set_y(
      response.rotation.euler_zyx.y);
  resp.mutable_rotation()->mutable_euler_zyx()->set_z(
      response.rotation.euler_zyx.z);
  return resp;
}

void Robot::RobotImpl::save_file(
    const protos_json::file_proto::SaveFileRequest &req) {
  rpc_client_->Call<void>("save_file", {req});
}

void Robot::RobotImpl::rename_file(
    const protos_json::file_proto::RenameFileRequest &req) {
  rpc_client_->Call<void>("rename_file", {req});
}

file::File Robot::RobotImpl::load_file(
    const protos_json::file_proto::FileIndex &req) {
  const auto response =
      rpc_client_->Call<protos_json::file_proto::File>("load_file", {req});
  file::File resp;
  resp.set_is_dir(response.is_dir);
  resp.set_data(response.data);
  return resp;
}

file::LoadFileListResponse Robot::RobotImpl::load_file_list(
    const protos_json::file_proto::LoadFileListRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::file_proto::LoadFileListResponse>(
          "load_file_list", {req});
  file::LoadFileListResponse resp;
  for (const auto &file : response.files) {
    file::FileName file_name;
    file_name.set_is_dir(file.is_dir);
    file_name.set_name(file.name);
    resp.mutable_files()->push_back(file_name);
  }
  return resp;
}

void Robot::RobotImpl::zip(const protos_json::file_proto::ZipRequest &req) {
  rpc_client_->Call<void>("zip", {req});
}

void Robot::RobotImpl::unzip(const protos_json::file_proto::UnzipRequest &req) {
  rpc_client_->Call<void>("unzip", {req});
}

file::LoadZipListResponse Robot::RobotImpl::load_zip_list(
    const protos_json::file_proto::LoadZipListRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::file_proto::LoadZipListResponse>(
          "load_zip_list", {req});
  file::LoadZipListResponse resp;
  for (const auto &file : response.files) {
    file::FileName file_name;
    file_name.set_is_dir(file.is_dir);
    file_name.set_name(file.name);
    resp.mutable_files()->push_back(file_name);
  }
  return resp;
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
dynamic::Payload Robot::RobotImpl::get_payload() {
  const auto response =
      rpc_client_->Call<protos_json::dynamic_proto::Payload>("get_payload", {});
  dynamic::Payload resp;
  resp.set_mass(response.mass);
  posture::Position cog;
  cog.set_x(response.cog.x);
  cog.set_y(response.cog.y);
  cog.set_z(response.cog.z);
  resp.set_cog(cog);
  return resp;
}
void Robot::RobotImpl::set_gravity(
    const protos_json::posture_proto::Position &req) {
  rpc_client_->Call<void>("set_gravity", {req});
}
posture::Position Robot::RobotImpl::get_gravity() {
  const auto response =
      rpc_client_->Call<protos_json::posture_proto::Position>("get_gravity",
                                                              {});
  posture::Position resp;
  resp.set_x(response.x);
  resp.set_y(response.y);
  resp.set_z(response.z);
  return resp;
}
void Robot::RobotImpl::save_payload(
    const protos_json::dynamic_proto::SavePayloadRequest &req) {
  rpc_client_->Call<void>("save_payload", {req});
}
dynamic::Payload Robot::RobotImpl::load_payload(
    const protos_json::db_proto::LoadRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::dynamic_proto::Payload>("load_payload",
                                                             {req});
  dynamic::Payload resp;
  resp.set_mass(response.mass);
  posture::Position cog;
  cog.set_x(response.cog.x);
  cog.set_y(response.cog.y);
  cog.set_z(response.cog.z);
  resp.set_cog(cog);
  return resp;
}
db::LoadListResponse Robot::RobotImpl::load_payload_list(
    const protos_json::db_proto::LoadListRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::db_proto::LoadListResponse>(
          "load_payload_list", {req});
  db::LoadListResponse resp;
  resp.set_data(response.names);
  return resp;
}

void Robot::RobotImpl::set_tcp(
    const protos_json::posture_proto::CartesianPose &req) {
  rpc_client_->Call<void>("set_tcp", {req});
}
posture::CartesianPose Robot::RobotImpl::get_tcp() {
  const auto response =
      rpc_client_->Call<protos_json::posture_proto::CartesianPose>("get_tcp",
                                                                   {});
  posture::CartesianPose resp;
  resp.mutable_position()->set_x(response.position.x);
  resp.mutable_position()->set_y(response.position.y);
  resp.mutable_position()->set_z(response.position.z);
  resp.mutable_rotation()->mutable_euler_zyx()->set_x(
      response.rotation.euler_zyx.x);
  resp.mutable_rotation()->mutable_euler_zyx()->set_y(
      response.rotation.euler_zyx.y);
  resp.mutable_rotation()->mutable_euler_zyx()->set_z(
      response.rotation.euler_zyx.z);
  return resp;
}
void Robot::RobotImpl::set_kin_factor(
    const protos_json::kin_factor_proto::KinFactor &req) {
  rpc_client_->Call<void>("set_kin_factor", {req});
}
kinematic::KinFactor Robot::RobotImpl::get_kin_factor() {
  const auto response =
      rpc_client_->Call<protos_json::kin_factor_proto::KinFactor>(
          "get_kin_factor", {});
  kinematic::KinFactor resp;
  resp.set_factor(response.speed_factor);
  return resp;
}
posture::CartesianPose Robot::RobotImpl::load_tcp(
    const protos_json::db_proto::LoadRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::posture_proto::CartesianPose>("load_tcp",
                                                                   {req});
  posture::CartesianPose resp;
  resp.mutable_position()->set_x(response.position.x);
  resp.mutable_position()->set_y(response.position.y);
  resp.mutable_position()->set_z(response.position.z);
  resp.mutable_rotation()->mutable_euler_zyx()->set_x(
      response.rotation.euler_zyx.x);
  resp.mutable_rotation()->mutable_euler_zyx()->set_y(
      response.rotation.euler_zyx.y);
  resp.mutable_rotation()->mutable_euler_zyx()->set_z(
      response.rotation.euler_zyx.z);
  return resp;
}
void Robot::RobotImpl::writeSingleCoil(const modbus::SetCoilRequest &req) {
  json_rpc_connector_->CallRpc("write_single_coil", req.ToJSONString(),
                               nullptr);
}
void Robot::RobotImpl::writeMultipleCoils(const modbus::SetCoilsRequest &req) {
  json_rpc_connector_->CallRpc("write_multiple_coils", req.ToJSONString(),
                               nullptr);
}
modbus::GetCoilsResponse Robot::RobotImpl::readCoils(
    const modbus::GetCoilsRequest &req) {
  std::string resp_str;
  json_rpc_connector_->CallRpc("read_coils", req.ToJSONString(), &resp_str);
  modbus::GetCoilsResponse resp;
  resp.FromJSONString(resp_str);
  return resp;
}
modbus::GetCoilsResponse Robot::RobotImpl::readDiscreteInputs(
    const modbus::GetCoilsRequest &req) {
  std::string resp_str;
  json_rpc_connector_->CallRpc("read_discrete_inputs", req.ToJSONString(),
                               &resp_str);
  modbus::GetCoilsResponse resp;
  resp.FromJSONString(resp_str);
  return resp;
}
void Robot::RobotImpl::writeSingleRegister(
    const modbus::SetRegisterRequest &req) {
  json_rpc_connector_->CallRpc("write_single_register", req.ToJSONString(),
                               nullptr);
}
void Robot::RobotImpl::writeMultipleRegisters(
    const modbus::SetRegistersRequest &req) {
  json_rpc_connector_->CallRpc("write_multiple_registers", req.ToJSONString(),
                               nullptr);
}
modbus::GetRegistersResponse Robot::RobotImpl::readInputRegisters(
    const modbus::GetRegistersRequest &req) {
  std::string resp_str;
  json_rpc_connector_->CallRpc("read_input_registers", req.ToJSONString(),
                               &resp_str);
  modbus::GetRegistersResponse resp;
  resp.FromJSONString(resp_str);
  return resp;
}
modbus::GetRegistersResponse Robot::RobotImpl::readHoldingRegisters(
    const modbus::GetRegistersRequest &req) {
  std::string resp_str;
  json_rpc_connector_->CallRpc("read_holding_registers", req.ToJSONString(),
                               &resp_str);
  modbus::GetRegistersResponse resp;
  resp.FromJSONString(resp_str);
  return resp;
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
storage::Item Robot::RobotImpl::get_item(
    const protos_json::storage_proto::ItemIndex &req) {
  const auto response =
      rpc_client_->Call<protos_json::storage_proto::Item>("get_item", {req});
  storage::Item resp;
  resp.set_key(response.key);
  resp.set_value(response.value);
  return resp;
}

storage::Items Robot::RobotImpl::get_items(
    const protos_json::storage_proto::GetItemsRequest &req) {
  const auto response =
      rpc_client_->Call<protos_json::storage_proto::Items>("get_items", {req});
  storage::Items resp;
  for (const auto &item : response.items) {
    storage::Item storage_item;
    storage_item.set_key(item.key);
    storage_item.set_value(item.value);
    resp.mutable_items()->push_back(storage_item);
  }
  return resp;
}

void Robot::RobotImpl::enableCollisionDetector() {
  json_rpc_connector_->CallRpc("enable_collision_detector", "{}", nullptr);
}

void Robot::RobotImpl::disableCollisionDetector() {
  json_rpc_connector_->CallRpc("disable_collision_detector", "{}", nullptr);
}

void Robot::RobotImpl::setCollisionTorqueDiff(
    const safety::CollisionTorqueDiff &req) {
  json_rpc_connector_->CallRpc("set_collision_torque_diff", req.ToJSONString(),
                               nullptr);
}

safety::CollisionTorqueDiff Robot::RobotImpl::getCollisionTorqueDiff() {
  std::string resp_str;
  json_rpc_connector_->CallRpc("get_collision_torque_diff", "{}", &resp_str);
  safety::CollisionTorqueDiff resp;
  resp.FromJSONString(resp_str);
  return resp;
}

void Robot::RobotImpl::setCollisionDetector(
    const safety::CollisionDetector &req) {
  json_rpc_connector_->CallRpc("set_collision_detector", req.ToJSONString(),
                               nullptr);
}

safety::CollisionDetector Robot::RobotImpl::getCollisionDetector() {
  std::string resp_str;
  json_rpc_connector_->CallRpc("get_collision_detector", "{}", &resp_str);
  safety::CollisionDetector resp;
  resp.FromJSONString(resp_str);
  return resp;
}

void Robot::RobotImpl::enableLimit() {
  json_rpc_connector_->CallRpc("enable_limit", "{}", nullptr);
}

void Robot::RobotImpl::disableLimit() {
  json_rpc_connector_->CallRpc("disable_limit", "{}", nullptr);
}

void Robot::RobotImpl::setJointsLimit(const safety::JointsLimit &req) {
  json_rpc_connector_->CallRpc("set_joints_limit", req.ToJSONString(), nullptr);
}

safety::JointsLimit Robot::RobotImpl::getJointsLimit() {
  std::string resp_str;
  json_rpc_connector_->CallRpc("get_joints_limit", "{}", &resp_str);
  safety::JointsLimit resp;
  resp.FromJSONString(resp_str);
  return resp;
}

void Robot::RobotImpl::setCartLimit(const safety::CartesianLimit &req) {
  json_rpc_connector_->CallRpc("set_cart_limit", req.ToJSONString(), nullptr);
}

safety::CartesianLimit Robot::RobotImpl::getCartLimit() {
  std::string resp_str;
  json_rpc_connector_->CallRpc("get_cart_limit", "{}", &resp_str);
  safety::CartesianLimit resp;
  resp.FromJSONString(resp_str);
  return resp;
}

}  // namespace l_master
}  // namespace lebai
