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

#include <lebai/robot.hh>
#include <websocketpp/base64/base64.hpp>
#include <memory>
#include <stdexcept>
#include "robot_impl.hh"
#include "protos/motion.hh"
#include "protos/serial.hh"
#include "protos/posture.hh"
#include <lebai/config.hh>

namespace lebai {

namespace l_master {

std::string version() { return LEBAI_SDK_VERSION_STR; }

static CartesianPose convertToCartesianPose(
    const posture::CartesianPose &pose) {
  CartesianPose cart_pose;
  cart_pose["x"] = pose.position().x();
  cart_pose["y"] = pose.position().y();
  cart_pose["z"] = pose.position().z();
  if (pose.rotation().euler_zyx()) {
    cart_pose["rz"] = pose.rotation().euler_zyx()->z();
    cart_pose["ry"] = pose.rotation().euler_zyx()->y();
    cart_pose["rx"] = pose.rotation().euler_zyx()->x();
  }
  return cart_pose;
}

static CollisionDetectorConfig convertToCollisionDetectorConfig(
    const safety::CollisionDetector &detector) {
  CollisionDetectorConfig config;
  config.action = static_cast<unsigned int>(detector.action());
  config.pause_time = detector.pause_time();
  config.sensitivity = detector.sensitivity();
  return config;
}

static std::string convertCollisionDetectorActionToWire(unsigned int action) {
  switch (static_cast<safety::CollisionDetectorAction>(action)) {
    case safety::CollisionDetectorAction::ESTOP:
      return "ESTOP";
    case safety::CollisionDetectorAction::PAUSE:
      return "PAUSE";
    case safety::CollisionDetectorAction::STOP_MOVE:
      return "STOP_MOVE";
    case safety::CollisionDetectorAction::NONE:
      return "NONE";
  }
  throw std::runtime_error("unknown collision detector action");
}

static JointLimitConfig convertToJointLimitConfig(
    const safety::JointLimit &joint_limit) {
  JointLimitConfig config;
  config.min_position = joint_limit.min_position();
  config.max_position = joint_limit.max_position();
  config.max_a = joint_limit.max_a();
  config.max_v = joint_limit.max_v();
  return config;
}

static CartesianLimitConfig convertToCartesianLimitConfig(
    const safety::CartesianLimit &cart_limit) {
  CartesianLimitConfig config;
  config.max_a = cart_limit.max_a();
  config.max_v = cart_limit.max_v();
  config.eq_radius = cart_limit.eq_radius();
  return config;
}

Robot::Robot(std::string ip, bool simulator)
// :ip_(ip)
{
  impl_ = std::make_unique<RobotImpl>(ip, simulator);
}
Robot::~Robot() {}

std::tuple<int, std::string> Robot::call(const std::string &method,
                                         const std::string &params) {
  return impl_->call(method, params);
}
bool Robot::is_network_connected() { return impl_->isNetworkConnected(); }

void Robot::start_sys() { impl_->start_sys(); }

void Robot::stop_sys() { impl_->stop_sys(); }

void Robot::powerdown() { impl_->powerdown(); }

void Robot::stop() { impl_->stop(); }

void Robot::estop() { impl_->estop(); }

void Robot::teach_mode() { impl_->start_teach_mode(); }

void Robot::end_teach_mode() { impl_->end_teach_mode(); }

void Robot::pause() { impl_->pause_move(); }

void Robot::resume() { impl_->resume_move(); }

void Robot::reboot() { impl_->reboot(); }

int Robot::movej(const std::vector<double> &joint_positions, double a, double v,
                 double t, double r) {
  protos_json::motion_proto::MoveRequest move_req;
  move_req.pose.kind = 1;
  move_req.pose.joint.joint = joint_positions;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  motion::MotionIndex resp = impl_->move_joint(move_req);
  return resp.id();
}

int Robot::movej(const CartesianPose &cart_pose, double a, double v, double t,
                 double r) {
  protos_json::motion_proto::CartesianMoveRequest move_req;
  move_req.pose.kind = 0;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  if (cart_pose.find("x") != cart_pose.end()) {
    move_req.pose.cart.position.x = cart_pose.at("x");
  } else {
    return -1;
  }
  if (cart_pose.find("y") != cart_pose.end()) {
    move_req.pose.cart.position.y = cart_pose.at("y");
  } else {
    return -1;
  }
  if (cart_pose.find("z") != cart_pose.end()) {
    move_req.pose.cart.position.z = cart_pose.at("z");
  } else {
    return -1;
  }
  if (cart_pose.find("rx") != cart_pose.end()) {
    move_req.pose.cart.rotation.euler_zyx.x = cart_pose.at("rx");
  } else {
    return -1;
  }
  if (cart_pose.find("ry") != cart_pose.end()) {
    move_req.pose.cart.rotation.euler_zyx.y = cart_pose.at("ry");
  } else {
    return -1;
  }
  if (cart_pose.find("rz") != cart_pose.end()) {
    move_req.pose.cart.rotation.euler_zyx.z = cart_pose.at("rz");
  } else {
    return -1;
  }
  motion::MotionIndex resp = impl_->move_joint(move_req);
  return resp.id();
}

int Robot::movel(const std::vector<double> &joint_positions, double a, double v,
                 double t, double r) {
  protos_json::motion_proto::MoveRequest move_req;
  move_req.pose.kind = 1;
  move_req.pose.joint.joint = joint_positions;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  motion::MotionIndex resp = impl_->move_linear(move_req);
  return resp.id();
}

int Robot::movel(const CartesianPose &cart_pose, double a, double v, double t,
                 double r) {
  protos_json::motion_proto::CartesianMoveRequest move_req;
  move_req.pose.kind = 0;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  if (cart_pose.find("x") != cart_pose.end()) {
    move_req.pose.cart.position.x = cart_pose.at("x");
  } else {
    return -1;
  }
  if (cart_pose.find("y") != cart_pose.end()) {
    move_req.pose.cart.position.y = cart_pose.at("y");
  } else {
    return -1;
  }
  if (cart_pose.find("z") != cart_pose.end()) {
    move_req.pose.cart.position.z = cart_pose.at("z");
  } else {
    return -1;
  }
  if (cart_pose.find("rx") != cart_pose.end()) {
    move_req.pose.cart.rotation.euler_zyx.x = cart_pose.at("rx");
  } else {
    return -1;
  }
  if (cart_pose.find("ry") != cart_pose.end()) {
    move_req.pose.cart.rotation.euler_zyx.y = cart_pose.at("ry");
  } else {
    return -1;
  }
  if (cart_pose.find("rz") != cart_pose.end()) {
    move_req.pose.cart.rotation.euler_zyx.z = cart_pose.at("rz");
  } else {
    return -1;
  }
  motion::MotionIndex resp = impl_->move_linear(move_req);
  return resp.id();
}
int Robot::movec(const std::vector<double> &joint_via,
                 const std::vector<double> &joint, double rad, double a,
                 double v, double t, double r) {
  protos_json::motion_proto::MoveCircularRequest move_req;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  move_req.rad = rad;
  move_req.pose_via.kind = 1;
  move_req.pose_via.joint.joint = joint_via;
  move_req.pose.kind = 1;
  move_req.pose.joint.joint = joint;
  motion::MotionIndex resp = impl_->move_circular(move_req);
  return resp.id();
}

int Robot::movec(const CartesianPose &cart_via, const CartesianPose &cart,
                 double rad, double a, double v, double t, double r) {
  protos_json::motion_proto::CartesianMoveCircularRequest move_req;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  move_req.rad = rad;
  move_req.pose_via.kind = 0;
  move_req.pose.kind = 0;
  if (cart_via.find("x") != cart_via.end()) {
    move_req.pose_via.cart.position.x = cart_via.at("x");
  } else {
    return -1;
  }
  if (cart_via.find("y") != cart_via.end()) {
    move_req.pose_via.cart.position.y = cart_via.at("y");
  } else {
    return -1;
  }
  if (cart_via.find("z") != cart_via.end()) {
    move_req.pose_via.cart.position.z = cart_via.at("z");
  } else {
    return -1;
  }
  if (cart_via.find("rx") != cart_via.end()) {
    move_req.pose_via.cart.rotation.euler_zyx.x = cart_via.at("rx");
  } else {
    return -1;
  }
  if (cart_via.find("ry") != cart_via.end()) {
    move_req.pose_via.cart.rotation.euler_zyx.y = cart_via.at("ry");
  } else {
    return -1;
  }
  if (cart_via.find("rz") != cart_via.end()) {
    move_req.pose_via.cart.rotation.euler_zyx.z = cart_via.at("rz");
  } else {
    return -1;
  }
  if (cart.find("x") != cart.end()) {
    move_req.pose.cart.position.x = cart.at("x");
  } else {
    return -1;
  }
  if (cart.find("y") != cart.end()) {
    move_req.pose.cart.position.y = cart.at("y");
  } else {
    return -1;
  }
  if (cart.find("z") != cart.end()) {
    move_req.pose.cart.position.z = cart.at("z");
  } else {
    return -1;
  }
  if (cart.find("rx") != cart.end()) {
    move_req.pose.cart.rotation.euler_zyx.x = cart.at("rx");
  } else {
    return -1;
  }
  if (cart.find("ry") != cart.end()) {
    move_req.pose.cart.rotation.euler_zyx.y = cart.at("ry");
  } else {
    return -1;
  }
  if (cart.find("rz") != cart.end()) {
    move_req.pose.cart.rotation.euler_zyx.z = cart.at("rz");
  } else {
    return -1;
  }
  motion::MotionIndex resp = impl_->move_circular(move_req);
  return resp.id();
}

int Robot::speedj(double a, const std::vector<double> &v, double t) {
  protos_json::motion_proto::SpeedJointRequest req;
  req.param.acc = a;
  req.param.time = t;
  req.speed.joint = v;
  lebai::motion::MotionIndex resp = impl_->speed_joint(req);
  return resp.id();
}

int Robot::speedl(double a, const CartesianPose &v, double t,
                  const CartesianPose &reference) {
  protos_json::motion_proto::SpeedLinearRequest req;
  req.param.acc = a;
  req.param.time = t;
  if (v.find("x") != v.end()) {
    req.speed.position.x = v.at("x");
  } else {
    return -1;
  }
  if (v.find("y") != v.end()) {
    req.speed.position.y = v.at("y");
  } else {
    return -1;
  }
  if (v.find("z") != v.end()) {
    req.speed.position.z = v.at("z");
  } else {
    return -1;
  }
  if (v.find("rx") != v.end()) {
    req.speed.rotation.euler_zyx.x = v.at("rx");
  } else {
    return -1;
  }
  if (v.find("ry") != v.end()) {
    req.speed.rotation.euler_zyx.y = v.at("ry");
  } else {
    return -1;
  }
  if (v.find("rz") != v.end()) {
    req.speed.rotation.euler_zyx.z = v.at("rz");
  } else {
    return -1;
  }
  // if(reference == BASE) {
  //   req.mutable_frame()->set_position_kind(posture::CartesianFrame::Kind::BASE);
  //   req.mutable_frame()->set_rotation_kind(posture::CartesianFrame::Kind::BASE);
  // } else if(reference == FLANGE) {
  //   req.mutable_frame()->set_position_kind(posture::CartesianFrame::Kind::FLANGE);
  //   req.mutable_frame()->set_rotation_kind(posture::CartesianFrame::Kind::FLANGE);
  // } else if(reference == TCP) {
  //   auto tcp = get_target_tcp_pose();
  //   req.mutable_frame()->mutable_position()->set_x(tcp["x"]);
  //   req.mutable_frame()->mutable_position()->set_y(tcp["y"]);
  //   req.mutable_frame()->mutable_position()->set_z(tcp["z"]);
  //   req.mutable_frame()->mutable_rotation()->mutable_euler_zyx()->set_x(tcp["rx"]);
  //   req.mutable_frame()->mutable_rotation()->mutable_euler_zyx()->set_y(tcp["ry"]);
  //   req.mutable_frame()->mutable_rotation()->mutable_euler_zyx()->set_z(tcp["rz"]);
  //   req.mutable_frame()->set_position_kind(posture::CartesianFrame::Kind::CUSTOM);
  //   req.mutable_frame()->set_rotation_kind(posture::CartesianFrame::Kind::CUSTOM);
  // } else {
  //   return -1;
  // }
  req.frame.position_kind = posture::CartesianFrame::Kind::CUSTOM;
  req.frame.rotation_kind = posture::CartesianFrame::Kind::CUSTOM;

  if (reference.find("x") != reference.end()) {
    req.frame.position.x = reference.at("x");
  } else {
    return -1;
  }
  if (reference.find("y") != reference.end()) {
    req.frame.position.y = reference.at("y");
  } else {
    return -1;
  }
  if (reference.find("z") != reference.end()) {
    req.frame.position.z = reference.at("z");
  } else {
    return -1;
  }
  if (reference.find("rx") != reference.end()) {
    req.frame.rotation.euler_zyx.x = reference.at("rx");
  } else {
    return -1;
  }
  if (reference.find("ry") != reference.end()) {
    req.frame.rotation.euler_zyx.y = reference.at("ry");
  } else {
    return -1;
  }
  if (reference.find("rz") != reference.end()) {
    req.frame.rotation.euler_zyx.z = reference.at("rz");
  } else {
    return -1;
  }
  lebai::motion::MotionIndex resp = impl_->speed_linear(req);
  return resp.id();
}

int Robot::towardj(const std::vector<double> &joint_positions, double a,
                   double v, double t, double r) {
  protos_json::motion_proto::MoveRequest move_req;
  move_req.pose.kind = 1;
  move_req.pose.joint.joint = joint_positions;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  motion::MotionIndex resp = impl_->toward_joint(move_req);
  return resp.id();
}

void Robot::move_pvat(std::vector<double> p, std::vector<double> v,
                      std::vector<double> a, double t) {
  protos_json::motion_proto::MovePvatRequest req;
  for (int i = 0; i < p.size(); i++) {
    protos_json::motion_proto::JointMove joint;
    joint.pose = p[i];
    joint.velocity = v[i];
    joint.acc = a[i];
    req.joints.push_back(joint);
  }
  req.duration = t;
  impl_->move_pvat(req);
}

void Robot::wait_move(unsigned int id) {
  motion::MotionIndex req;
  req.set_id(id);
  impl_->wait_move(req);
}
void Robot::wait_move() {
  motion::MotionIndex req;
  req.set_id(0);
  impl_->wait_move(req);
}

unsigned int Robot::get_running_motion() {
  motion::MotionIndex resp = impl_->get_running_motion();
  return resp.id();
}

std::string Robot::get_motion_state(unsigned int id) {
  motion::MotionIndex req;
  req.set_id(id);
  motion::GetMotionStateResponse resp = impl_->get_motion_state(req);
  switch (resp.state()) {
    case motion::WAIT:
      return (std::string) "WAIT";
    case motion::RUNNING:
      return (std::string) "RUNNING";
    case motion::FINISHED:
      return (std::string) "FINISHED";
    default:
      return (std::string) "WAIT";
  }
}

void Robot::stop_move() { impl_->stop_move(); }

int Robot::get_robot_state() { return impl_->get_robot_state(); }

int Robot::get_estop_reason() { return impl_->get_estop_reason(); }

SystemInfoData Robot::get_system_info() {
  const auto info = impl_->get_system_info();
  SystemInfoData system_info;
  system_info.name = info.name;
  system_info.kernel_version = info.kernel_version;
  system_info.os_version = info.os_version;
  system_info.host_name = info.host_name;
  system_info.used_memory = info.memory.used;
  system_info.total_memory = info.memory.total;
  return system_info;
}

PhysicalData Robot::get_phy_data() {
  auto phy_data = impl_->get_phy_data();
  PhysicalData physical_data;
  physical_data.joint_temperature = phy_data.joint_temp();
  physical_data.joint_voltage = phy_data.joint_voltage();
  physical_data.flange_voltage = phy_data.flange_voltage();
  return physical_data;
}

JointMotionData Robot::get_kin_data() {
  auto kin_data = impl_->get_kin_data();
  JointMotionData joint_motion_data;
  joint_motion_data.actual_joint_pose = kin_data.actual_joint_pose();
  joint_motion_data.actual_joint_speed = kin_data.actual_joint_speed();
  joint_motion_data.actual_joint_acc = kin_data.actual_joint_acc();
  joint_motion_data.actual_joint_torque = kin_data.actual_joint_torque();
  joint_motion_data.target_joint_pose = kin_data.target_joint_pose();
  joint_motion_data.target_joint_speed = kin_data.target_joint_speed();
  joint_motion_data.target_joint_acc = kin_data.target_joint_acc();
  joint_motion_data.target_joint_torque = kin_data.target_joint_torque();
  joint_motion_data.actual_tcp_pose =
      convertToCartesianPose(kin_data.actual_tcp_pose());
  joint_motion_data.target_tcp_pose =
      convertToCartesianPose(kin_data.target_tcp_pose());
  joint_motion_data.actual_flange_pose =
      convertToCartesianPose(kin_data.actual_flange_pose());
  return joint_motion_data;
}

bool Robot::is_disconnected() { return impl_->get_robot_state() == 0; }

bool Robot::is_down() { return impl_->get_robot_state() < 4; }

std::vector<double> Robot::get_actual_joint_positions() {
  std::map<std::string, double> ret;
  return *impl_->get_kin_data().mutable_actual_joint_pose();
}

std::vector<double> Robot::get_target_joint_positions() {
  std::map<std::string, double> ret;
  return *impl_->get_kin_data().mutable_target_joint_pose();
}
std::vector<double> Robot::get_actual_joint_speed() {
  return *impl_->get_kin_data().mutable_actual_joint_speed();
}
std::vector<double> Robot::get_target_joint_speed() {
  return *impl_->get_kin_data().mutable_target_joint_speed();
}

CartesianPose Robot::get_actual_tcp_pose() {
  auto pose = impl_->get_kin_data().actual_tcp_pose();
  return convertToCartesianPose(pose);
}
CartesianPose Robot::get_target_tcp_pose() {
  auto pose = impl_->get_kin_data().target_tcp_pose();
  return convertToCartesianPose(pose);
}

double Robot::get_joint_temp(unsigned int joint_index) {
  auto data = impl_->get_phy_data();
  joint_index -= 1;
  if (data.joint_temp().size() > joint_index) {
    return data.joint_temp()[joint_index];
  }
  return 0.0;
}

std::vector<double> Robot::get_actual_joint_torques() {
  return impl_->get_kin_data().actual_joint_torque();
}

std::vector<double> Robot::get_target_joint_torques() {
  return impl_->get_kin_data().target_joint_torque();
}

void Robot::set_do(std::string device, unsigned int pin, unsigned int value) {
  protos_json::io_proto::SetDoPinRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.value = value;
  impl_->set_do(req);
}

unsigned int Robot::get_do(std::string device, unsigned int pin) {
  protos_json::io_proto::GetDioPinRequest req;

  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }

  req.pin = pin;
  io::GetDioPinResponse resp = impl_->get_do(req);
  return resp.value();
}

std::vector<unsigned int> Robot::get_dos(std::string device, unsigned int pin,
                                         unsigned int num) {
  protos_json::io_proto::GetDioPinsRequest req;

  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }

  req.pin = pin;
  req.count = num;
  io::GetDioPinsResponse resp = impl_->get_dos(req);
  return resp.values();
}

unsigned int Robot::get_di(std::string device, unsigned int pin) {
  protos_json::io_proto::GetDioPinRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }

  req.pin = pin;
  io::GetDioPinResponse resp = impl_->get_di(req);
  return resp.value();
}

std::vector<unsigned int> Robot::get_dis(std::string device, unsigned int pin,
                                         unsigned int num) {
  protos_json::io_proto::GetDioPinsRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.count = num;
  io::GetDioPinsResponse resp = impl_->get_dis(req);
  return resp.values();
}

void Robot::set_ao(std::string device, unsigned int pin, double value) {
  protos_json::io_proto::SetAoPinRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.value = value;
  impl_->set_ao(req);
}

double Robot::get_ao(std::string device, unsigned int pin) {
  protos_json::io_proto::GetAioPinRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  io::GetAioPinResponse resp = impl_->get_ao(req);
  return resp.value();
}

std::vector<double> Robot::get_aos(std::string device, unsigned int pin,
                                   unsigned int num) {
  protos_json::io_proto::GetAioPinsRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.count = num;
  io::GetAioPinsResponse resp = impl_->get_aos(req);
  return resp.values();
}

double Robot::get_ai(std::string device, unsigned int pin) {
  protos_json::io_proto::GetAioPinRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  io::GetAioPinResponse resp = impl_->get_ai(req);
  return resp.value();
}

std::vector<double> Robot::get_ais(std::string device, unsigned int pin,
                                   unsigned int num) {
  protos_json::io_proto::GetAioPinsRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.count = num;
  io::GetAioPinsResponse resp = impl_->get_ais(req);
  return resp.values();
}
void Robot::set_dio_mode(std::string device, unsigned int pin, bool value) {
  protos_json::io_proto::SetDioModeRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.value = value;
  impl_->set_dio_mode(req);
}
std::vector<bool> Robot::get_dios_mode(std::string device, unsigned int pin,
                                       unsigned int count) {
  protos_json::io_proto::GetDiosModeRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.count = count;
  io::GetDiosModeResponse resp = impl_->get_dios_mode(req);
  return resp.values();
}

void Robot::init_claw(bool force_initilization) {
  protos_json::claw_proto::InitClawRequest req;
  req.force_initilization = force_initilization;
  impl_->init_claw(req);
}

void Robot::set_claw(double force, double amplitude) {
  protos_json::claw_proto::SetClawRequest req;
  req.force = force;
  req.amplitude = amplitude;
  impl_->set_claw(req);
}

ClawData Robot::get_claw_data() {
  auto resp = impl_->get_claw();
  ClawData claw_data;
  claw_data.force = resp.force();
  claw_data.amplitude = resp.amplitude();
  claw_data.hold_on = resp.hold_on();
  return claw_data;
}

void Robot::set_led(unsigned int mode, unsigned int speed,
                    const std::vector<unsigned int> &color) {
  led::LedData req;
  switch (mode) {
    case 0:
      req.set_mode(led::LedMode::HOLD_LED);
      break;
    case 1:
      req.set_mode(led::LedMode::CLOSE_LED);
      break;
    case 2:
      req.set_mode(led::LedMode::OPEN_LED);
      break;
    case 3:
      req.set_mode(led::LedMode::BREATH);
      break;
    case 4:
      req.set_mode(led::LedMode::FOUR);
      break;
    case 5:
      req.set_mode(led::LedMode::WATER);
      break;
    case 6:
      req.set_mode(led::LedMode::BLINK);
      break;
    default:
      return;
  }
  switch (speed) {
    case 0:
      req.set_speed(led::LedSpeed::HOLD_LED_SPEED);
      break;
    case 1:
      req.set_speed(led::LedSpeed::FAST);
      break;
    case 2:
      req.set_speed(led::LedSpeed::NORMAL);
      break;
    case 3:
      req.set_speed(led::LedSpeed::SLOW);
      break;
    default:
      return;
  }
  std::vector<led::LedColor> _color;
  for (auto _c : color) {
    switch (_c) {
      case 0:
        _color.push_back(led::LedColor::RED);
        break;
      case 1:
        _color.push_back(led::LedColor::GREEN);
        break;
      case 2:
        _color.push_back(led::LedColor::BLUE);
        break;
      case 3:
        _color.push_back(led::LedColor::PINK);
        break;
      case 4:
        _color.push_back(led::LedColor::YELLOW);
        break;
      case 5:
        _color.push_back(led::LedColor::CYAN);
        break;
      case 6:
        _color.push_back(led::LedColor::GRAY);
        break;
      case 7:
        _color.push_back(led::LedColor::BROWN);
        break;
      case 8:
        _color.push_back(led::LedColor::ORANGE);
        break;
      case 9:
        _color.push_back(led::LedColor::GOLD);
        break;
      case 10:
        _color.push_back(led::LedColor::INDIGO);
        break;
      case 11:
        _color.push_back(led::LedColor::LIGHT_SKY_BLUE);
        break;
      case 12:
        _color.push_back(led::LedColor::DARK_VIOLET);
        break;
      case 13:
        _color.push_back(led::LedColor::CHOCOLATE);
        break;
      case 14:
        _color.push_back(led::LedColor::LIGHT_RED);
        break;
      case 15:
        _color.push_back(led::LedColor::WHITE);
        break;
      default:
        return;
    }
  }
  req.set_colors(_color);
  protos_json::led_proto::LedData typed_req;
  typed_req.mode = static_cast<int>(req.mode());
  typed_req.speed = static_cast<int>(req.speed());
  for (const auto color_value : req.colors()) {
    typed_req.colors.push_back(static_cast<int>(color_value));
  }
  impl_->set_led(typed_req);
}

void Robot::set_voice(unsigned int voice, unsigned int volume) {
  led::VoiceData req;
  switch (voice) {
    case 0:
      req.set_voice(led::VoiceKind::OFF);
      break;
    case 1:
      req.set_voice(led::VoiceKind::BOOTING);
      break;
    case 2:
      req.set_voice(led::VoiceKind::STOPING);
      break;
    case 3:
      req.set_voice(led::VoiceKind::COLLISION_DETECTED);
      break;
    case 4:
      req.set_voice(led::VoiceKind::UPGRADE);
      break;
    case 5:
      req.set_voice(led::VoiceKind::TEACH_MODE_ON);
      break;
    case 6:
      req.set_voice(led::VoiceKind::TEACH_MODE_OFF);
      break;
    case 7:
      req.set_voice(led::VoiceKind::FINE_TUNNING_ON);
      break;
    case 8:
      req.set_voice(led::VoiceKind::FINE_TUNNING_OFF);
      break;
    case 9:
      req.set_voice(led::VoiceKind::FINE_TUNNING_CHANGE);
      break;
    case 10:
      req.set_voice(led::VoiceKind::BORING);
      break;
    case 11:
      req.set_voice(led::VoiceKind::CUSTOM1);
      break;
    case 12:
      req.set_voice(led::VoiceKind::CUSTOM2);
      break;
    case 13:
      req.set_voice(led::VoiceKind::CUSTOM3);
      break;
    case 14:
      req.set_voice(led::VoiceKind::CUSTOM4);
      break;
    case 15:
      req.set_voice(led::VoiceKind::CUSTOM5);
      break;
    default:
      return;
  }
  switch (volume) {
    case 0:
      req.set_volume(led::Volume::MUTE);
      break;
    case 1:
      req.set_volume(led::Volume::LOW);
      break;
    case 2:
      req.set_volume(led::Volume::MID);
      break;
    case 3:
      req.set_volume(led::Volume::HIGH);
      break;
    default:
      return;
  }
  protos_json::led_proto::VoiceData typed_req;
  typed_req.voice = static_cast<int>(req.voice());
  typed_req.volume = static_cast<int>(req.volume());
  impl_->set_voice(typed_req);
}

void Robot::set_fan(unsigned int status) {
  led::FanData req;
  switch (status) {
    case 0:
      req.set_fan(led::FanMode::HOLD_FAN);
      break;
    case 1:
      req.set_fan(led::FanMode::CLOSE_FAN);
      break;
    case 2:
      req.set_fan(led::FanMode::OPEN_FAN);
      break;
    default:
      return;
  }
  protos_json::led_proto::FanData typed_req;
  typed_req.fan = static_cast<int>(req.fan());
  impl_->set_fan(typed_req);
}

void Robot::set_signal(unsigned int index, int value) {
  protos_json::signal_proto::SetSignalRequest req;
  req.key = index;
  req.value = value;
  impl_->set_signal(req);
}
int Robot::get_signal(unsigned int index) {
  protos_json::signal_proto::GetSignalRequest req;
  req.key = index;
  signal::GetSignalResponse resp = impl_->get_signal(req);
  return resp.value();
}
void Robot::add_signal(unsigned int index, int value) {
  protos_json::signal_proto::SetSignalRequest req;
  req.key = index;
  req.value = value;
  impl_->add_signal(req);
}

unsigned int Robot::start_task(const std::string &name,
                               const std::vector<std::string> &params,
                               const std::string &dir, bool is_parallel,
                               unsigned int loop_to) {
  protos_json::control_proto::StartTaskRequest req;
  req.name = name;
  req.is_parallel = is_parallel;
  req.loop_to = loop_to;
  req.dir = dir;
  req.kind = static_cast<unsigned int>(control::TaskKind::LUA);
  req.params = params;
  control::TaskIndex resp = impl_->start_task(req);
  return resp.id();
}
unsigned int Robot::start_task(const std::string &name) {
  protos_json::control_proto::StartTaskRequest req;
  req.name = name;
  req.is_parallel = false;
  req.loop_to = 1;
  req.kind = static_cast<unsigned int>(control::TaskKind::LUA);
  control::TaskIndex resp = impl_->start_task(req);
  return resp.id();
}
std::vector<unsigned int> Robot::get_task_list() {
  control::TaskIds resp = impl_->load_task_list();
  return resp.ids();
}
std::string Robot::wait_task(unsigned int id) {
  protos_json::control_proto::TaskIndex task_index;
  task_index.id = id;
  control::TaskStdout resp = impl_->wait_task(task_index);
  return resp.get_stdout();
}
void Robot::pause_task(unsigned int id) {
  protos_json::control_proto::PauseRequest req;
  req.id = id;
  req.time = 0;
  req.wait = false;
  impl_->pause_task(req);
}
void Robot::pause_task(unsigned int id, unsigned long time, bool wait) {
  protos_json::control_proto::PauseRequest req;
  req.id = id;
  req.time = time;
  req.wait = wait;
  impl_->pause_task(req);
}
void Robot::resume_task(unsigned int id) {
  protos_json::control_proto::TaskIndex req;
  req.id = id;
  impl_->resume_task(req);
}
void Robot::cancel_task(unsigned int id) {
  protos_json::control_proto::TaskIndex req;
  req.id = id;
  impl_->cancel_task(req);
}
unsigned int Robot::exec_hook(unsigned int id) {
  protos_json::control_proto::TaskIndex req;
  req.id = id;
  control::HookResponse resp = impl_->exec_hook(req);
  if (!resp.success()) {
    return 0;
  }
  return atoi(resp.error().c_str());
}
std::string Robot::get_task_state(unsigned int id) {
  protos_json::control_proto::TaskIndex req;
  req.id = id;
  control::Task resp = impl_->load_task(req);
  switch (resp.state()) {
    case control::TaskState::NONE:
      return "NONE";
      break;
    case control::TaskState::WAIT:
      return "WAIT";
      break;
    case control::TaskState::RUNNING:
      return "RUNNING";
      break;
    case control::TaskState::PAUSE:
      return "PAUSE";
      break;
    case control::TaskState::SUCCESS:
      return "SUCCESS";
      break;
    case control::TaskState::INTERRUPT:
      return "INTERRUPT";
      break;
    case control::TaskState::FAIL:
      return "FAIL";
      break;
    case control::TaskState::INTERRUPTING:
      return "INTERRUPTING";
      break;
    default:
      return "Undefined State";
  }
}
std::string Robot::get_task_state() {
  control::Task resp = impl_->load_task();
  switch (resp.state()) {
    case control::TaskState::NONE:
      return "NONE";
      break;
    case control::TaskState::WAIT:
      return "WAIT";
      break;
    case control::TaskState::RUNNING:
      return "RUNNING";
      break;
    case control::TaskState::PAUSE:
      return "PAUSE";
      break;
    case control::TaskState::SUCCESS:
      return "SUCCESS";
      break;
    case control::TaskState::INTERRUPT:
      return "INTERRUPT";
      break;
    case control::TaskState::FAIL:
      return "FAIL";
      break;
    case control::TaskState::INTERRUPTING:
      return "INTERRUPTING";
      break;
    default:
      return "Undefined State";
  }
}

KinematicsForwardResp Robot::kinematics_forward(
    const std::vector<double> &joint_positions) {
  protos_json::kinematic_proto::PoseRequest req;
  req.pose.kind = 1;
  req.pose.joint.joint = joint_positions;
  auto resp = impl_->get_forward_kin(req);
  KinematicsForwardResp kf_resp;
  kf_resp.pose["x"] = resp.position().x();
  kf_resp.pose["y"] = resp.position().y();
  kf_resp.pose["z"] = resp.position().z();
  kf_resp.pose["rz"] = resp.rotation().euler_zyx()->z();
  kf_resp.pose["ry"] = resp.rotation().euler_zyx()->y();
  kf_resp.pose["rx"] = resp.rotation().euler_zyx()->x();
  kf_resp.ok = true;
  return kf_resp;
}

KinematicsInverseResp Robot::kinematics_inverse(
    const CartesianPose &pose,
    const std::vector<double> &joint_init_positions) {
  protos_json::kinematic_proto::GetInverseKinRequest req;
  req.pose.kind = 0;
  double x = 0.0;
  if (pose.find("x") != pose.end()) {
    x = pose.at("x");
  }
  double y = 0.0;
  if (pose.find("y") != pose.end()) {
    y = pose.at("y");
  }
  double z = 0.0;
  if (pose.find("z") != pose.end()) {
    z = pose.at("z");
  }
  double rx = 0.0;
  if (pose.find("rx") != pose.end()) {
    rx = pose.at("rx");
  }
  double ry = 0.0;
  if (pose.find("ry") != pose.end()) {
    ry = pose.at("ry");
  }
  double rz = 0.0;
  if (pose.find("rz") != pose.end()) {
    rz = pose.at("rz");
  }

  req.pose.cart.position.x = x;
  req.pose.cart.position.y = y;
  req.pose.cart.position.z = z;
  req.pose.cart.rotation.euler_zyx.z = rz;
  req.pose.cart.rotation.euler_zyx.y = ry;
  req.pose.cart.rotation.euler_zyx.x = rx;

  for (auto &&p : joint_init_positions) {
    req.refer.joint.push_back(p);
  }
  // std::vector<double> joint_positions;
  KinematicsInverseResp ki_resp;
  try {
    auto resp = impl_->get_inverse_kin(req);
    ki_resp.joint_positions = resp.joint();
    ki_resp.ok = true;
  } catch (std::exception &e) {
    ki_resp.ok = false;
    return ki_resp;
  }
  return ki_resp;
}

CartesianPose Robot::pose_times(const CartesianPose &a,
                                const CartesianPose &b) {
  protos_json::kinematic_proto::GetPoseTransRequest req;
  req.from.kind = 0;
  req.from_to.kind = 0;
  double x = 0.0;
  if (a.find("x") != a.end()) {
    x = a.at("x");
  }
  double y = 0.0;
  if (a.find("y") != a.end()) {
    y = a.at("y");
  }
  double z = 0.0;
  if (a.find("z") != a.end()) {
    z = a.at("z");
  }
  double rx = 0.0;
  if (a.find("rx") != a.end()) {
    rx = a.at("rx");
  }
  double ry = 0.0;
  if (a.find("ry") != a.end()) {
    ry = a.at("ry");
  }
  double rz = 0.0;
  if (a.find("rz") != a.end()) {
    rz = a.at("rz");
  }

  req.from.cart.position.x = x;
  req.from.cart.position.y = y;
  req.from.cart.position.z = z;
  req.from.cart.rotation.euler_zyx.z = rz;
  req.from.cart.rotation.euler_zyx.y = ry;
  req.from.cart.rotation.euler_zyx.x = rx;

  x = 0.0;
  if (b.find("x") != b.end()) {
    x = b.at("x");
  }
  y = 0.0;
  if (b.find("y") != b.end()) {
    y = b.at("y");
  }
  z = 0.0;
  if (b.find("z") != b.end()) {
    z = b.at("z");
  }
  rx = 0.0;
  if (b.find("rx") != b.end()) {
    rx = b.at("rx");
  }
  ry = 0.0;
  if (b.find("ry") != b.end()) {
    ry = b.at("ry");
  }
  rz = 0.0;
  if (b.find("rz") != b.end()) {
    rz = b.at("rz");
  }

  req.from_to.cart.position.x = x;
  req.from_to.cart.position.y = y;
  req.from_to.cart.position.z = z;
  req.from_to.cart.rotation.euler_zyx.z = rz;
  req.from_to.cart.rotation.euler_zyx.y = ry;
  req.from_to.cart.rotation.euler_zyx.x = rx;

  auto resp = impl_->get_pose_trans(req);
  CartesianPose pose;
  pose["x"] = resp.position().x();
  pose["y"] = resp.position().y();
  pose["z"] = resp.position().z();
  pose["rz"] = resp.rotation().euler_zyx()->z();
  pose["ry"] = resp.rotation().euler_zyx()->y();
  pose["rx"] = resp.rotation().euler_zyx()->x();
  return pose;
}

CartesianPose Robot::pose_inverse(const CartesianPose &in) {
  protos_json::kinematic_proto::PoseRequest req;
  req.pose.kind = 0;
  double x = 0.0;
  if (in.find("x") != in.end()) {
    x = in.at("x");
  }
  double y = 0.0;
  if (in.find("y") != in.end()) {
    y = in.at("y");
  }
  double z = 0.0;
  if (in.find("z") != in.end()) {
    z = in.at("z");
  }
  double rx = 0.0;
  if (in.find("rx") != in.end()) {
    rx = in.at("rx");
  }
  double ry = 0.0;
  if (in.find("ry") != in.end()) {
    ry = in.at("ry");
  }
  double rz = 0.0;
  if (in.find("rz") != in.end()) {
    rz = in.at("rz");
  }

  req.pose.cart.position.x = x;
  req.pose.cart.position.y = y;
  req.pose.cart.position.z = z;
  req.pose.cart.rotation.euler_zyx.z = rz;
  req.pose.cart.rotation.euler_zyx.y = ry;
  req.pose.cart.rotation.euler_zyx.x = rx;
  auto resp = impl_->get_pose_inverse(req);
  CartesianPose pose;
  pose["x"] = resp.position().x();
  pose["y"] = resp.position().y();
  pose["z"] = resp.position().z();
  pose["rz"] = resp.rotation().euler_zyx()->z();
  pose["ry"] = resp.rotation().euler_zyx()->y();
  pose["rx"] = resp.rotation().euler_zyx()->x();
  return pose;
}

void Robot::save_file(const std::string &dir, const std::string &name,
                      bool is_dir, const std::string &data) {
  protos_json::file_proto::SaveFileRequest req;
  req.dir = dir;
  req.name = name;
  req.file.is_dir = is_dir;
  req.file.data = websocketpp::base64_encode(data);
  impl_->save_file(req);
}
/**
void Robot::save_file(const std::string &dir,const std::string & name,file::File
file)
{
  file::SaveFileRequest req;
  req.set_dir(dir);
  req.set_name(name);
  req.set_file(file);
  impl_->saveFile(req);
}
*/

void Robot::rename_file(const std::string &from_dir,
                        const std::string &from_name, const std::string &to_dir,
                        const std::string &to_name) {
  protos_json::file_proto::RenameFileRequest req;
  req.from.dir = from_dir;
  req.from.name = from_name;
  req.to.dir = to_dir;
  req.to.name = to_name;
  impl_->rename_file(req);
}
/**
void Robot::rename_file(file::FileIndex from,file::FileIndex to)
{
  file::RenameFileRequest req;
  file.set_from(from);
  file.set_to(to);
  impl_->renameFile(req);
}
*/
std::tuple<bool, std::string> Robot::load_file(const std::string &dir,
                                               const std::string &name) {
  protos_json::file_proto::FileIndex req;
  req.dir = dir;
  req.name = name;
  file::File resp = impl_->load_file(req);
  std::tuple<bool, std::string> ret =
      std::make_tuple(resp.is_dir(), websocketpp::base64_decode(resp.data()));
  return ret;
}
std::vector<std::tuple<bool, std::string>> Robot::load_file_list(
    const std::string &dir, const std::string &prefix,
    const std::string &suffix) {
  protos_json::file_proto::LoadFileListRequest req;
  req.dir = dir;
  req.prefix = prefix;
  req.suffix = suffix;
  file::LoadFileListResponse resp = impl_->load_file_list(req);
  std::vector<std::tuple<bool, std::string>> ret;
  for (auto f : resp.files()) {
    std::tuple<bool, std::string> temp = std::make_tuple(f.is_dir(), f.name());
    ret.push_back(temp);
  }
  return ret;
}
void Robot::set_payload(double mass, std::map<std::string, double> cog) {
  protos_json::dynamic_proto::SetPayloadRequest req;
  req.mass = mass;
  req.cog.x = cog.at("x");
  req.cog.y = cog.at("y");
  req.cog.z = cog.at("z");
  impl_->set_payload(req);
}
void Robot::set_payload_mass(double mass) {
  protos_json::dynamic_proto::SetMassRequest req;
  req.mass = mass;
  impl_->set_payload(req);
}
void Robot::set_payload_cog(std::map<std::string, double> cog) {
  protos_json::dynamic_proto::SetCogRequest req;
  req.cog.x = cog.at("x");
  req.cog.y = cog.at("y");
  req.cog.z = cog.at("z");
  impl_->set_payload(req);
}

std::map<std::string, double> Robot::get_payload() {
  dynamic::Payload resp = impl_->get_payload();
  std::map<std::string, double> cog;
  cog["x"] = resp.cog().x();
  cog["y"] = resp.cog().y();
  cog["z"] = resp.cog().z();
  cog["mass"] = resp.mass();
  return cog;
}

void Robot::set_gravity(std::map<std::string, double> gravity) {
  protos_json::posture_proto::Position req;
  req.x = gravity.at("x");
  req.y = gravity.at("y");
  req.z = gravity.at("z");
  impl_->set_gravity(req);
}

std::map<std::string, double> Robot::get_gravity() {
  posture::Position resp = impl_->get_gravity();
  std::map<std::string, double> gravity;
  gravity["x"] = resp.x();
  gravity["y"] = resp.y();
  gravity["z"] = resp.z();
  return gravity;
}

void Robot::enable_collision_detector() { impl_->enable_collision_detector(); }

void Robot::disable_collision_detector() { impl_->disable_collision_detector(); }

void Robot::set_collision_torque_diff(const std::vector<double> &diffs) {
  protos_json::safety_proto::CollisionTorqueDiff req;
  req.diffs = diffs;
  impl_->set_collision_torque_diff(req);
}

std::vector<double> Robot::get_collision_torque_diff() {
  safety::CollisionTorqueDiff resp = impl_->get_collision_torque_diff();
  return resp.diffs();
}

void Robot::set_collision_detector(const CollisionDetectorConfig &config) {
  protos_json::safety_proto::CollisionDetector req;
  req.action = convertCollisionDetectorActionToWire(config.action);
  req.pause_time = config.pause_time;
  req.sensitivity = config.sensitivity;
  impl_->set_collision_detector(req);
}

CollisionDetectorConfig Robot::get_collision_detector() {
  safety::CollisionDetector resp = impl_->get_collision_detector();
  return convertToCollisionDetectorConfig(resp);
}

void Robot::enable_limit() { impl_->enable_limit(); }

void Robot::disable_limit() { impl_->disable_limit(); }

void Robot::set_joints_limit(const std::vector<JointLimitConfig> &joints) {
  protos_json::safety_proto::JointsLimit req;
  for (auto &&joint : joints) {
    protos_json::safety_proto::JointLimit joint_limit;
    joint_limit.min_position = joint.min_position;
    joint_limit.max_position = joint.max_position;
    joint_limit.max_a = joint.max_a;
    joint_limit.max_v = joint.max_v;
    req.joints.push_back(joint_limit);
  }
  impl_->set_joints_limit(req);
}

std::vector<JointLimitConfig> Robot::get_joints_limit() {
  safety::JointsLimit resp = impl_->get_joints_limit();
  std::vector<JointLimitConfig> joints;
  for (auto &&joint : resp.joints()) {
    joints.push_back(convertToJointLimitConfig(joint));
  }
  return joints;
}

void Robot::set_cart_limit(const CartesianLimitConfig &limit) {
  protos_json::safety_proto::CartesianLimit req;
  req.max_a = limit.max_a;
  req.max_v = limit.max_v;
  req.eq_radius = limit.eq_radius;
  impl_->set_cart_limit(req);
}

CartesianLimitConfig Robot::get_cart_limit() {
  safety::CartesianLimit resp = impl_->get_cart_limit();
  return convertToCartesianLimitConfig(resp);
}

void Robot::set_tcp(std::array<double, 6> tcp) {
  protos_json::posture_proto::CartesianPose req;
  req.position.x = tcp[0];
  req.position.y = tcp[1];
  req.position.z = tcp[2];
  req.rotation.euler_zyx.z = tcp[3];
  req.rotation.euler_zyx.y = tcp[4];
  req.rotation.euler_zyx.x = tcp[5];
  impl_->set_tcp(req);
}
std::array<double, 6> Robot::get_tcp() {
  posture::CartesianPose resp = impl_->get_tcp();
  std::array<double, 6> ret;
  ret[0] = resp.position().x();
  ret[1] = resp.position().y();
  ret[2] = resp.position().z();
  ret[3] = resp.rotation().euler_zyx()->z();
  ret[4] = resp.rotation().euler_zyx()->y();
  ret[5] = resp.rotation().euler_zyx()->x();
  return ret;
}

void Robot::set_velocity_factor(int factor) {
  protos_json::kin_factor_proto::KinFactor req;
  req.speed_factor = factor;
  impl_->set_kin_factor(req);
}
int Robot::get_velocity_factor() {
  kinematic::KinFactor resp = impl_->get_kin_factor();
  return resp.factor();
}
CartesianPose Robot::load_tcp(std::string name, std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto &pose = impl_->load_tcp(req);
  return convertToCartesianPose(pose);
}

void Robot::write_single_coil(std::string device, std::string addr,
                              bool value) {
  protos_json::modbus_proto::SetCoilRequest req;
  req.device = device;
  req.pin = addr;
  req.value = value;
  impl_->write_single_coil(req);
}

void Robot::wirte_multiple_coils(std::string device, std::string addr,
                                 std::vector<bool> values) {
  protos_json::modbus_proto::SetCoilsRequest req;
  req.device = device;
  req.pin = addr;
  req.values = values;
  impl_->write_multiple_coils(req);
}
std::vector<bool> Robot::read_coils(std::string device, std::string addr,
                                    unsigned int num) {
  protos_json::modbus_proto::GetCoilsRequest req;
  req.device = device;
  req.pin = addr;
  req.count = num;
  modbus::GetCoilsResponse resp = impl_->read_coils(req);
  return resp.values();
}
std::vector<bool> Robot::read_discrete_inputs(std::string device,
                                              std::string addr,
                                              unsigned int num) {
  protos_json::modbus_proto::GetCoilsRequest req;
  req.device = device;
  req.pin = addr;
  req.count = num;
  modbus::GetCoilsResponse resp = impl_->read_discrete_inputs(req);
  return resp.values();
}
void Robot::write_single_register(std::string device, std::string addr,
                                  unsigned int value) {
  protos_json::modbus_proto::SetRegisterRequest req;
  req.device = device;
  req.pin = addr;
  req.value = value;
  impl_->write_single_register(req);
}
void Robot::write_multiple_registers(std::string device, std::string addr,
                                     std::vector<unsigned int> values) {
  protos_json::modbus_proto::SetRegistersRequest req;
  req.device = device;
  req.pin = addr;
  req.values = values;
  impl_->write_multiple_registers(req);
}

std::vector<unsigned int> Robot::read_holding_registers(std::string device,
                                                        std::string addr,
                                                        unsigned int num) {
  protos_json::modbus_proto::GetRegistersRequest req;
  req.device = device;
  req.pin = addr;
  req.count = num;
  modbus::GetRegistersResponse resp = impl_->read_holding_registers(req);
  return resp.values();
}
std::vector<unsigned int> Robot::read_input_registers(std::string device,
                                                      std::string addr,
                                                      unsigned int num) {
  protos_json::modbus_proto::GetRegistersRequest req;
  req.device = device;
  req.pin = addr;
  req.count = num;
  modbus::GetRegistersResponse resp = impl_->read_input_registers(req);
  return resp.values();
}

void Robot::set_serial_baud_rate(std::string device, unsigned int baud_rate) {
  protos_json::serial_proto::SetSerialBaudRateRequest req;
  req.device = device;
  req.baud_rate = baud_rate;
  impl_->set_serial_baud_rate(req);
}

void Robot::set_serial_parity(std::string device, unsigned int parity) {
  protos_json::serial_proto::SetSerialParityRequest req;
  req.device = device;
  req.parity = parity;
  impl_->set_serial_parity(req);
}

void Robot::set_item(StorageItem item) {
  protos_json::storage_proto::Item req;
  req.key = item.key;
  req.value = item.value;
  impl_->set_item(req);
}

StorageItem Robot::get_item(std::string name) {
  protos_json::storage_proto::ItemIndex req;
  req.key = name;
  storage::Item resp = impl_->get_item(req);
  StorageItem item;
  item.key = resp.key();
  item.value = resp.value();
  return item;
}

std::vector<StorageItem> Robot::get_items(std::string prefix) {
  protos_json::storage_proto::GetItemsRequest req;
  req.prefix = prefix;
  storage::Items resp = impl_->get_items(req);
  std::vector<StorageItem> ret;
  for (const auto &item : resp.items()) {
    StorageItem storage_item;
    storage_item.key = item.key();
    storage_item.value = item.value();
    ret.push_back(storage_item);
  }
  return ret;
}

}  // namespace l_master

}  // namespace lebai
