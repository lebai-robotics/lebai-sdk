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
#include <memory>
#include <stdexcept>
#include "base64.hh"
#include "robot_impl.hh"
#include <lebai/config.hh>

namespace lebai {

namespace l_master {

std::string version() { return LEBAI_SDK_VERSION_STR; }

constexpr int kCartesianFrameCustom = 99;

static CartesianPose convertToCartesianPose(
    const protos_json::kinematic_proto::CartesianPose &pose) {
  CartesianPose cart_pose;
  cart_pose["x"] = pose.position.x;
  cart_pose["y"] = pose.position.y;
  cart_pose["z"] = pose.position.z;
  cart_pose["rz"] = pose.rotation.euler_zyx.z;
  cart_pose["ry"] = pose.rotation.euler_zyx.y;
  cart_pose["rx"] = pose.rotation.euler_zyx.x;
  return cart_pose;
}

static protos_json::kinematic_proto::CartesianPose convertToKinematicPose(
    const CartesianPose &pose) {
  protos_json::kinematic_proto::CartesianPose typed_pose;
  typed_pose.position.x = pose.count("x") ? pose.at("x") : 0.0;
  typed_pose.position.y = pose.count("y") ? pose.at("y") : 0.0;
  typed_pose.position.z = pose.count("z") ? pose.at("z") : 0.0;
  typed_pose.rotation.euler_zyx.x = pose.count("rx") ? pose.at("rx") : 0.0;
  typed_pose.rotation.euler_zyx.y = pose.count("ry") ? pose.at("ry") : 0.0;
  typed_pose.rotation.euler_zyx.z = pose.count("rz") ? pose.at("rz") : 0.0;
  return typed_pose;
}

static CartesianPose convertToCartesianPose(
    const protos_json::posture_proto::CartesianPose &pose) {
  CartesianPose cart_pose;
  cart_pose["x"] = pose.position.x;
  cart_pose["y"] = pose.position.y;
  cart_pose["z"] = pose.position.z;
  cart_pose["rz"] = pose.rotation.euler_zyx.z;
  cart_pose["ry"] = pose.rotation.euler_zyx.y;
  cart_pose["rx"] = pose.rotation.euler_zyx.x;
  return cart_pose;
}

static protos_json::posture_proto::CartesianPose convertToPosturePose(
    const CartesianPose &pose) {
  protos_json::posture_proto::CartesianPose typed_pose;
  typed_pose.position.x = pose.count("x") ? pose.at("x") : 0.0;
  typed_pose.position.y = pose.count("y") ? pose.at("y") : 0.0;
  typed_pose.position.z = pose.count("z") ? pose.at("z") : 0.0;
  typed_pose.rotation.euler_zyx.x = pose.count("rx") ? pose.at("rx") : 0.0;
  typed_pose.rotation.euler_zyx.y = pose.count("ry") ? pose.at("ry") : 0.0;
  typed_pose.rotation.euler_zyx.z = pose.count("rz") ? pose.at("rz") : 0.0;
  return typed_pose;
}

static CartesianPose convertToPositionPose(
    const protos_json::posture_proto::Position &position) {
  CartesianPose pose;
  pose["x"] = position.x;
  pose["y"] = position.y;
  pose["z"] = position.z;
  return pose;
}

static CartesianPose convertToRotationPose(
    const protos_json::posture_proto::Rotation &rotation) {
  CartesianPose pose;
  pose["rx"] = rotation.euler_zyx.x;
  pose["ry"] = rotation.euler_zyx.y;
  pose["rz"] = rotation.euler_zyx.z;
  return pose;
}

static DeviceInfoData convertToDeviceInfoData(
    const protos_json::system_proto::DeviceInfo &info) {
  DeviceInfoData data;
  data.invalid = info.invalid;
  data.sn = info.sn;
  data.version = info.version;
  data.partition = info.partition;
  data.di_num = info.di_num;
  data.do_num = info.do_num;
  data.dio_num = info.dio_num;
  data.ai_num = info.ai_num;
  data.ao_num = info.ao_num;
  return data;
}

static CollisionDetectorConfig convertToCollisionDetectorConfig(
    const protos_json::safety_proto::CollisionDetector &detector) {
  CollisionDetectorConfig config;
  if (detector.action == "ESTOP") {
    config.action = 0;
  } else if (detector.action == "PAUSE") {
    config.action = 1;
  } else if (detector.action == "STOP_MOVE") {
    config.action = 2;
  } else if (detector.action == "NONE") {
    config.action = 99;
  } else {
    throw std::runtime_error("unknown collision detector action: " +
                             detector.action);
  }
  config.pause_time = detector.pause_time;
  config.sensitivity = detector.sensitivity;
  return config;
}

static std::string convertCollisionDetectorActionToWire(unsigned int action) {
  switch (action) {
    case 0:
      return "ESTOP";
    case 1:
      return "PAUSE";
    case 2:
      return "STOP_MOVE";
    case 99:
      return "NONE";
  }
  throw std::runtime_error("unknown collision detector action");
}

static std::string convertTaskStateToString(
    protos_json::control_proto::TaskState state) {
  using TaskState = protos_json::control_proto::TaskState;
  switch (state) {
    case TaskState::NONE:
      return "NONE";
    case TaskState::WAIT:
      return "WAIT";
    case TaskState::RUNNING:
      return "RUNNING";
    case TaskState::PAUSE:
      return "PAUSE";
    case TaskState::SUCCESS:
      return "SUCCESS";
    case TaskState::INTERRUPT:
      return "INTERRUPT";
    case TaskState::FAIL:
      return "FAIL";
    case TaskState::INTERRUPTING:
      return "INTERRUPTING";
  }
  return "Undefined State";
}

static std::string convertTaskKindToString(
    protos_json::control_proto::TaskKind kind) {
  switch (kind) {
    case protos_json::control_proto::TaskKind::LUA:
      return "LUA";
    case protos_json::control_proto::TaskKind::APP:
      return "APP";
  }
  return "Undefined Kind";
}

static JointLimitConfig convertToJointLimitConfig(
    const protos_json::safety_proto::JointLimit &joint_limit) {
  JointLimitConfig config;
  config.min_position = joint_limit.min_position;
  config.max_position = joint_limit.max_position;
  config.max_a = joint_limit.max_a;
  config.max_v = joint_limit.max_v;
  return config;
}

static CartesianLimitConfig convertToCartesianLimitConfig(
    const protos_json::safety_proto::CartesianLimit &cart_limit) {
  CartesianLimitConfig config;
  config.max_a = cart_limit.max_a;
  config.max_v = cart_limit.max_v;
  config.eq_radius = cart_limit.eq_radius;
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

std::string Robot::hello(const std::string &data) {
  protos_json::system_proto::HelloData req;
  req.data = data;
  return impl_->hello(req).data;
}

void Robot::set_auto(int name, bool value) {
  if (name < 1 || name > 3) {
    throw std::invalid_argument("auto config name must be 1, 2, or 3");
  }
  protos_json::auto_proto::SetAutoRequest req;
  req.name = static_cast<protos_json::auto_proto::AutoCfg>(name - 1);
  req.value = value;
  impl_->set_auto(req);
}

bool Robot::get_auto(int name) {
  if (name < 1 || name > 3) {
    throw std::invalid_argument("auto config name must be 1, 2, or 3");
  }
  protos_json::auto_proto::GetAutoRequest req;
  req.name = static_cast<protos_json::auto_proto::AutoCfg>(name - 1);
  return impl_->get_auto(req).value;
}

bool Robot::is_network_connected() { return impl_->isNetworkConnected(); }

void Robot::start_sys() { impl_->start_sys(); }

void Robot::stop_sys() { impl_->stop_sys(); }

void Robot::powerdown() { impl_->powerdown(); }

void Robot::stop() { impl_->stop(); }

void Robot::estop() { impl_->estop(); }

void Robot::start_teach_mode() { impl_->start_teach_mode(); }

void Robot::teach_mode() { start_teach_mode(); }

void Robot::end_teach_mode() { impl_->end_teach_mode(); }

void Robot::pause_move() { impl_->pause_move(); }

void Robot::pause() { pause_move(); }

void Robot::resume_move() { impl_->resume_move(); }

void Robot::resume() { resume_move(); }

void Robot::reboot() { impl_->reboot(); }

int Robot::move_joint(const std::vector<double> &joint_positions, double a,
                      double v, double t, double r) {
  protos_json::motion_proto::MoveRequest move_req;
  move_req.pose.kind = 1;
  move_req.pose.joint.joint = joint_positions;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  const auto resp = impl_->move_joint(move_req);
  return resp.id;
}

int Robot::movej(const std::vector<double> &joint_positions, double a, double v,
                 double t, double r) {
  return move_joint(joint_positions, a, v, t, r);
}

int Robot::move_joint(const CartesianPose &cart_pose, double a, double v,
                      double t, double r) {
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
  const auto resp = impl_->move_joint(move_req);
  return resp.id;
}

int Robot::movej(const CartesianPose &cart_pose, double a, double v, double t,
                 double r) {
  return move_joint(cart_pose, a, v, t, r);
}

int Robot::move_linear(const std::vector<double> &joint_positions, double a,
                       double v, double t, double r) {
  protos_json::motion_proto::MoveRequest move_req;
  move_req.pose.kind = 1;
  move_req.pose.joint.joint = joint_positions;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  const auto resp = impl_->move_linear(move_req);
  return resp.id;
}

int Robot::movel(const std::vector<double> &joint_positions, double a, double v,
                 double t, double r) {
  return move_linear(joint_positions, a, v, t, r);
}

int Robot::move_linear(const CartesianPose &cart_pose, double a, double v,
                       double t, double r) {
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
  const auto resp = impl_->move_linear(move_req);
  return resp.id;
}

int Robot::movel(const CartesianPose &cart_pose, double a, double v, double t,
                 double r) {
  return move_linear(cart_pose, a, v, t, r);
}

int Robot::move_circular(const std::vector<double> &joint_via,
                         const std::vector<double> &joint, double rad,
                         double a, double v, double t, double r) {
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
  const auto resp = impl_->move_circular(move_req);
  return resp.id;
}

int Robot::movec(const std::vector<double> &joint_via,
                 const std::vector<double> &joint, double rad, double a,
                 double v, double t, double r) {
  return move_circular(joint_via, joint, rad, a, v, t, r);
}

int Robot::move_circular(const CartesianPose &cart_via,
                         const CartesianPose &cart, double rad, double a,
                         double v, double t, double r) {
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
  const auto resp = impl_->move_circular(move_req);
  return resp.id;
}

int Robot::movec(const CartesianPose &cart_via, const CartesianPose &cart,
                 double rad, double a, double v, double t, double r) {
  return move_circular(cart_via, cart, rad, a, v, t, r);
}

int Robot::speed_joint(double a, const std::vector<double> &v, double t) {
  protos_json::motion_proto::SpeedJointRequest req;
  req.param.acc = a;
  req.param.time = t;
  req.speed.joint = v;
  const auto resp = impl_->speed_joint(req);
  return resp.id;
}

int Robot::speedj(double a, const std::vector<double> &v, double t) {
  return speed_joint(a, v, t);
}

int Robot::speed_linear(double a, const CartesianPose &v, double t,
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
  req.frame.position_kind = kCartesianFrameCustom;
  req.frame.rotation_kind = kCartesianFrameCustom;

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
  const auto resp = impl_->speed_linear(req);
  return resp.id;
}

int Robot::speedl(double a, const CartesianPose &v, double t,
                  const CartesianPose &reference) {
  return speed_linear(a, v, t, reference);
}

int Robot::toward_joint(const std::vector<double> &joint_positions, double a,
                        double v, double t, double r) {
  protos_json::motion_proto::MoveRequest move_req;
  move_req.pose.kind = 1;
  move_req.pose.joint.joint = joint_positions;
  move_req.param.acc = a;
  move_req.param.velocity = v;
  move_req.param.time = t;
  move_req.param.radius = r;
  const auto resp = impl_->toward_joint(move_req);
  return resp.id;
}

int Robot::towardj(const std::vector<double> &joint_positions, double a,
                   double v, double t, double r) {
  return toward_joint(joint_positions, a, v, t, r);
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

static JointMoveData convertToJointMoveData(
    const protos_json::motion_proto::JointMove &joint) {
  JointMoveData data;
  data.pose = joint.pose;
  data.velocity = joint.velocity;
  data.acc = joint.acc;
  return data;
}

static protos_json::motion_proto::Trajectory convertToMotionTrajectory(
    const TrajectoryData &trajectory) {
  protos_json::motion_proto::Trajectory data;
  data.kind = trajectory.kind;
  for (const auto &point : trajectory.data) {
    protos_json::motion_proto::MovePvatRequest point_data;
    point_data.duration = point.duration;
    for (const auto &joint : point.joints) {
      protos_json::motion_proto::JointMove joint_data;
      joint_data.pose = joint.pose;
      joint_data.velocity = joint.velocity;
      joint_data.acc = joint.acc;
      point_data.joints.push_back(joint_data);
    }
    data.data.push_back(point_data);
  }
  return data;
}

void Robot::wait_move(unsigned int id) {
  protos_json::motion_proto::MotionIndex req;
  req.id = id;
  impl_->wait_move(req);
}
void Robot::wait_move() {
  protos_json::motion_proto::MotionIndex req;
  req.id = 0;
  impl_->wait_move(req);
}

unsigned int Robot::get_running_motion() {
  const auto resp = impl_->get_running_motion();
  return resp.id;
}

std::string Robot::get_motion_state(unsigned int id) {
  protos_json::motion_proto::MotionIndex req;
  req.id = id;
  const auto resp = impl_->get_motion_state(req);
  switch (resp.state) {
    case protos_json::motion_proto::MotionState::WAIT:
      return (std::string) "WAIT";
    case protos_json::motion_proto::MotionState::RUNNING:
      return (std::string) "RUNNING";
    case protos_json::motion_proto::MotionState::FINISHED:
      return (std::string) "FINISHED";
    default:
      return (std::string) "WAIT";
  }
}

void Robot::stop_move() { impl_->stop_move(); }

void Robot::skip_move() { impl_->skip_move(); }

int Robot::get_robot_state() {
  return static_cast<int>(impl_->get_robot_state());
}

int Robot::get_estop_reason() {
  return static_cast<int>(impl_->get_estop_reason());
}

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

RobotInfoData Robot::get_robot_info() {
  const auto info = impl_->get_robot_info();
  RobotInfoData robot_info;
  robot_info.name = info.name;
  robot_info.mac = info.mac;
  robot_info.box_model = info.box_model;
  robot_info.box_sn = info.box_sn;
  robot_info.arm_model = info.arm_model;
  robot_info.arm_sn = info.arm_sn;
  return robot_info;
}

HardwareInfoData Robot::get_hardware_info() {
  const auto info = impl_->get_hardware_info();
  HardwareInfoData hardware_info;
  hardware_info.comboard = convertToDeviceInfoData(info.comboard);
  hardware_info.flange = convertToDeviceInfoData(info.flange);
  hardware_info.led = convertToDeviceInfoData(info.led);
  hardware_info.extra_io = convertToDeviceInfoData(info.extra_io);
  for (const auto &joint : info.joints) {
    hardware_info.joints.push_back(convertToDeviceInfoData(joint));
  }
  return hardware_info;
}

SoftwareInfoData Robot::get_software_info() {
  const auto info = impl_->get_software_info();
  SoftwareInfoData software_info;
  for (const auto &item : info.software) {
    SoftwareItemInfoData data;
    data.version = item.second.version;
    data.branch = item.second.branch;
    software_info.software[item.first] = data;
  }
  return software_info;
}

std::vector<std::string> Robot::get_box_devices(const std::string &prefix) {
  protos_json::system_proto::GetBoxDevicesRequest req;
  req.prefix = prefix;
  return impl_->get_box_devices(req).devices;
}

std::vector<DirData> Robot::get_dirs() {
  const auto response = impl_->get_dirs();
  std::vector<DirData> dirs;
  for (const auto &dir : response.dirs) {
    DirData data;
    data.name = dir.name;
    data.id = dir.id;
    dirs.push_back(data);
  }
  return dirs;
}

static std::vector<ShortcutData> convertShortcuts(
    const protos_json::shortcut_proto::ShortcutList &response) {
  std::vector<ShortcutData> shortcuts;
  for (const auto &shortcut : response.list) {
    ShortcutData data;
    data.id = shortcut.id;
    data.dir = shortcut.dir;
    data.name = shortcut.name;
    shortcuts.push_back(data);
  }
  return shortcuts;
}

static ShortcutData convertShortcut(
    const protos_json::shortcut_proto::Shortcut &shortcut) {
  ShortcutData data;
  data.id = shortcut.id;
  data.dir = shortcut.dir;
  data.name = shortcut.name;
  return data;
}

static protos_json::shortcut_proto::Shortcut convertToShortcut(
    const ShortcutData &shortcut_data) {
  protos_json::shortcut_proto::Shortcut shortcut;
  shortcut.id = shortcut_data.id;
  shortcut.dir = shortcut_data.dir;
  shortcut.name = shortcut_data.name;
  return shortcut;
}

std::vector<ShortcutData> Robot::get_short_poses() {
  return convertShortcuts(impl_->get_short_poses());
}

void Robot::set_short_pose(ShortcutData shortcut_data) {
  impl_->set_short_pose(convertToShortcut(shortcut_data));
}

ShortcutData Robot::get_short_pose(unsigned int id) {
  protos_json::shortcut_proto::ShortcutIndex req;
  req.id = id;
  return convertShortcut(impl_->get_short_pose(req));
}

std::vector<ShortcutData> Robot::get_short_tasks() {
  return convertShortcuts(impl_->get_short_tasks());
}

void Robot::set_short_task(ShortcutData shortcut_data) {
  impl_->set_short_task(convertToShortcut(shortcut_data));
}

ShortcutData Robot::get_short_task(unsigned int id) {
  protos_json::shortcut_proto::ShortcutIndex req;
  req.id = id;
  return convertShortcut(impl_->get_short_task(req));
}

static ButtonIndexData convertButtonIndex(
    const protos_json::trigger_proto::ButtonIndex &button) {
  ButtonIndexData data;
  data.device = button.device;
  data.pin = button.pin;
  return data;
}

static protos_json::trigger_proto::ButtonIndex convertToButtonIndex(
    const ButtonIndexData &button) {
  protos_json::trigger_proto::ButtonIndex req;
  req.device = button.device;
  req.pin = button.pin;
  return req;
}

static protos_json::trigger_proto::ButtonStatus convertToButtonStatus(
    const ButtonStatusData &status) {
  protos_json::trigger_proto::ButtonStatus req;
  req.state = status.state;
  req.time = status.time;
  return req;
}

static protos_json::trigger_proto::Condition convertToTriggerCondition(
    const TriggerConditionData &condition) {
  protos_json::trigger_proto::Condition req;
  for (const auto &button : condition.pressed) {
    req.pressed.push_back(convertToButtonIndex(button));
  }
  req.button = convertToButtonIndex(condition.button);
  req.status = convertToButtonStatus(condition.status);
  return req;
}

static protos_json::trigger_proto::Trigger convertToTrigger(
    const TriggerData &trigger) {
  protos_json::trigger_proto::Trigger req;
  req.condition = convertToTriggerCondition(trigger.condition);
  req.function = trigger.function;
  return req;
}

std::vector<TriggerData> Robot::get_triggers() {
  const auto response = impl_->get_triggers();
  std::vector<TriggerData> triggers;
  for (const auto &trigger : response.triggers) {
    TriggerData data;
    for (const auto &button : trigger.condition.pressed) {
      data.condition.pressed.push_back(convertButtonIndex(button));
    }
    data.condition.button = convertButtonIndex(trigger.condition.button);
    data.condition.status.state = trigger.condition.status.state;
    data.condition.status.time = trigger.condition.status.time;
    data.function = trigger.function;
    triggers.push_back(data);
  }
  return triggers;
}

void Robot::set_trigger(TriggerData trigger) {
  impl_->set_trigger(convertToTrigger(trigger));
}

static LedStyleData convertLedStyle(
    const protos_json::led_proto::LedStyle &style) {
  LedStyleData data;
  data.mode = style.led.mode;
  data.speed = style.led.speed;
  data.colors = style.led.colors;
  data.voice = style.voice;
  data.volume = style.volume;
  return data;
}

static protos_json::led_proto::LedStyle convertToLedStyle(
    const LedStyleData &style) {
  protos_json::led_proto::LedStyle req;
  req.led.mode = style.mode;
  req.led.speed = style.speed;
  req.led.colors = style.colors;
  req.voice = style.voice;
  req.volume = style.volume;
  return req;
}

std::map<std::string, LedStyleData> Robot::get_led_styles() {
  const auto response = impl_->get_led_styles();
  std::map<std::string, LedStyleData> styles;
  for (const auto &style : response.styles) {
    styles[style.first] = convertLedStyle(style.second);
  }
  return styles;
}

void Robot::set_led_styles(std::map<std::string, LedStyleData> styles) {
  protos_json::led_proto::LedStyles req;
  for (const auto &style : styles) {
    req.styles[style.first] = convertToLedStyle(style.second);
  }
  impl_->set_led_styles(req);
}

LedStyleData Robot::load_led_style(std::string name, std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto style = impl_->load_led_style(req);
  return convertLedStyle(style);
}

void Robot::save_led_style(std::string name, LedStyleData style,
                           std::string dir) {
  protos_json::led_proto::SaveLedStyleRequest req;
  req.name = name;
  req.dir = dir;
  req.data = convertToLedStyle(style);
  impl_->save_led_style(req);
}

void Robot::set_led_style(std::string state, LedStyleData style) {
  protos_json::led_proto::LedStyleItem req;
  req.state = state;
  req.style = convertToLedStyle(style);
  impl_->set_led_style(req);
}

std::vector<std::string> Robot::load_led_style_list(std::string dir) {
  protos_json::db_proto::LoadListRequest req;
  req.dir = dir;
  return impl_->load_led_style_list(req).names;
}

std::vector<ServoParamData> Robot::get_servo_params() {
  const auto response = impl_->get_servo_params();
  std::vector<ServoParamData> params;
  for (const auto &param : response.params) {
    ServoParamData data;
    data.position_kp = param.position_kp;
    data.speed_kp = param.speed_kp;
    data.speed_it = param.speed_it;
    data.torque_cmd_filter = param.torque_cmd_filter;
    params.push_back(data);
  }
  return params;
}

WrenchData Robot::get_tcp_force() {
  const auto response = impl_->get_tcp_force();
  WrenchData data;
  data.force = {response.force.x, response.force.y, response.force.z};
  data.torque = {response.torque.x, response.torque.y, response.torque.z};
  return data;
}

void Robot::set_tcp_force(const WrenchData &wrench) {
  if (wrench.force.size() != 3 || wrench.torque.size() != 3) {
    throw std::invalid_argument("tcp force and torque must both have 3 values");
  }

  protos_json::motion_proto::Wrench req;
  req.force.x = wrench.force.at(0);
  req.force.y = wrench.force.at(1);
  req.force.z = wrench.force.at(2);
  req.torque.x = wrench.torque.at(0);
  req.torque.y = wrench.torque.at(1);
  req.torque.z = wrench.torque.at(2);
  impl_->set_tcp_force(req);
}

void Robot::set_force_mode_sensor(const std::string &sensor,
                                  unsigned int address) {
  protos_json::motion_proto::SetForceModeSensorRequest req;
  req.sensor = sensor;
  req.address = address;
  impl_->set_force_mode_sensor(req);
}

void Robot::set_force_mode_param(double damping, double gain,
                                 const std::vector<double> &max_vel) {
  if (max_vel.size() != 6) {
    throw std::invalid_argument("force mode max_vel must have 6 values");
  }

  protos_json::motion_proto::SetForceModeParamRequest req;
  req.damping = damping;
  req.gain = gain;
  req.max_vel = max_vel;
  impl_->set_force_mode_param(req);
}

void Robot::end_force_mode() {
  impl_->end_force_mode();
}

static PluginInfoData convertPluginInfo(
    const protos_json::plugin_proto::PluginInfo &plugin) {
  PluginInfoData data;
  data.name = plugin.name;
  data.description = plugin.description;
  data.homepage = plugin.homepage;
  data.auto_restart = plugin.auto_restart;
  data.web = plugin.web;
  data.daemon = plugin.daemon;
  data.cmd = plugin.cmd;
  data.enable = plugin.enable;
  return data;
}

std::vector<PluginInfoData> Robot::load_plugins() {
  const auto response = impl_->load_plugins();
  std::vector<PluginInfoData> plugins;
  for (const auto &plugin : response.plugins) {
    plugins.push_back(convertPluginInfo(plugin));
  }
  return plugins;
}

PluginInfoData Robot::load_plugin(const std::string &name) {
  protos_json::plugin_proto::PluginIndex req;
  req.name = name;
  return convertPluginInfo(impl_->load_plugin(req));
}

std::vector<DiscoveredRobotData> Robot::discover_robots() {
  const auto response = impl_->discover_robots();
  std::vector<DiscoveredRobotData> devices;
  for (const auto &device : response.devices) {
    DiscoveredRobotData data;
    data.name = device.name;
    data.mac = device.mac;
    data.ip = device.ip;
    data.online = device.online;
    devices.push_back(data);
  }
  return devices;
}

CommandStdoutData Robot::get_plugin_daemon_stdout(const std::string &name) {
  protos_json::plugin_proto::PluginIndex req;
  req.name = name;
  const auto response = impl_->get_plugin_daemon_stdout(req);
  CommandStdoutData data;
  data.done = response.done;
  data.stdout_text = response.stdout_text;
  data.stderr_text = response.stderr_text;
  data.code = response.code;
  return data;
}

std::vector<MessageData> Robot::get_messages() {
  const auto response = impl_->get_messages();
  std::vector<MessageData> messages;
  for (const auto &message : response.messages) {
    MessageData data;
    data.level = message.level;
    data.kind = message.kind;
    data.detail = message.detail;
    data.time = message.time;
    messages.push_back(data);
  }
  return messages;
}

OtaStateData Robot::get_ota_state() {
  const auto state = impl_->get_ota_state();
  OtaStateData data;
  data.step = state.step;
  data.progress = state.progress;
  return data;
}

CheckUpgradeData Robot::check_upgrade() {
  const auto response = impl_->check_upgrade();
  CheckUpgradeData data;
  data.need_upgrade = response.need_upgrade;
  data.introduction = response.introduction;
  return data;
}

CommandStdoutData Robot::get_upgrade_stdout() {
  const auto response = impl_->get_upgrade_stdout();
  CommandStdoutData data;
  data.done = response.done;
  data.stdout_text = response.stdout_text;
  data.stderr_text = response.stderr_text;
  data.code = response.code;
  return data;
}

PhysicalData Robot::get_phy_data() {
  auto phy_data = impl_->get_phy_data();
  PhysicalData physical_data;
  physical_data.joint_temperature = phy_data.joint_temp;
  physical_data.joint_voltage = phy_data.joint_voltage;
  physical_data.flange_voltage = phy_data.flange_voltage;
  return physical_data;
}

JointMotionData Robot::get_kin_data() {
  auto kin_data = impl_->get_kin_data();
  JointMotionData joint_motion_data;
  joint_motion_data.actual_joint_pose = kin_data.actual_joint_pose;
  joint_motion_data.actual_joint_speed = kin_data.actual_joint_speed;
  joint_motion_data.actual_joint_acc = kin_data.actual_joint_acc;
  joint_motion_data.actual_joint_torque = kin_data.actual_joint_torque;
  joint_motion_data.target_joint_pose = kin_data.target_joint_pose;
  joint_motion_data.target_joint_speed = kin_data.target_joint_speed;
  joint_motion_data.target_joint_acc = kin_data.target_joint_acc;
  joint_motion_data.target_joint_torque = kin_data.target_joint_torque;
  joint_motion_data.actual_tcp_pose =
      convertToCartesianPose(kin_data.actual_tcp_pose);
  joint_motion_data.target_tcp_pose =
      convertToCartesianPose(kin_data.target_tcp_pose);
  joint_motion_data.actual_flange_pose =
      convertToCartesianPose(kin_data.actual_flange_pose);
  return joint_motion_data;
}

bool Robot::is_disconnected() { return get_robot_state() == 0; }

bool Robot::is_down() { return get_robot_state() < 4; }

std::vector<double> Robot::get_actual_joint_positions() {
  return impl_->get_kin_data().actual_joint_pose;
}

std::vector<double> Robot::get_target_joint_positions() {
  return impl_->get_kin_data().target_joint_pose;
}
std::vector<double> Robot::get_actual_joint_speed() {
  return impl_->get_kin_data().actual_joint_speed;
}
std::vector<double> Robot::get_target_joint_speed() {
  return impl_->get_kin_data().target_joint_speed;
}

CartesianPose Robot::get_actual_tcp_pose() {
  auto pose = impl_->get_kin_data().actual_tcp_pose;
  return convertToCartesianPose(pose);
}
CartesianPose Robot::get_target_tcp_pose() {
  auto pose = impl_->get_kin_data().target_tcp_pose;
  return convertToCartesianPose(pose);
}

double Robot::get_joint_temp(unsigned int joint_index) {
  auto data = impl_->get_phy_data();
  joint_index -= 1;
  if (data.joint_temp.size() > joint_index) {
    return data.joint_temp[joint_index];
  }
  return 0.0;
}

std::vector<double> Robot::get_actual_joint_torques() {
  return impl_->get_kin_data().actual_joint_torque;
}

std::vector<double> Robot::get_target_joint_torques() {
  return impl_->get_kin_data().target_joint_torque;
}

static protos_json::io_proto::IoDevice convertIoDevice(
    const std::string &device) {
  if (device == "FLANGE") {
    return protos_json::io_proto::IoDevice::FLANGE;
  }
  if (device == "EXTRA") {
    return protos_json::io_proto::IoDevice::EXTRA;
  }
  if (device == "ROBOT_BTN") {
    return protos_json::io_proto::IoDevice::ROBOT_BTN;
  }
  if (device == "SHOULDER") {
    return protos_json::io_proto::IoDevice::SHOULDER;
  }
  if (device == "FLANGE_BTN") {
    return protos_json::io_proto::IoDevice::FLANGE_BTN;
  }
  return protos_json::io_proto::IoDevice::ROBOT;
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

void Robot::set_dos(std::string device, unsigned int pin,
                    std::vector<unsigned int> values) {
  protos_json::io_proto::SetDoPinsRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.values = {values.begin(), values.end()};
  impl_->set_dos(req);
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
  const auto resp = impl_->get_do(req);
  return resp.value;
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
  const auto resp = impl_->get_dos(req);
  return {resp.values.begin(), resp.values.end()};
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
  const auto resp = impl_->get_di(req);
  return resp.value;
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
  const auto resp = impl_->get_dis(req);
  return {resp.values.begin(), resp.values.end()};
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

void Robot::set_aos(std::string device, unsigned int pin,
                    std::vector<double> values) {
  protos_json::io_proto::SetAoPinsRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  req.values = values;
  impl_->set_aos(req);
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
  const auto resp = impl_->get_ao(req);
  return resp.value;
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
  const auto resp = impl_->get_aos(req);
  return resp.values;
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
  const auto resp = impl_->get_ai(req);
  return resp.value;
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
  const auto resp = impl_->get_ais(req);
  return resp.values;
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
bool Robot::get_dio_mode(std::string device, unsigned int pin) {
  protos_json::io_proto::GetDioModeRequest req;
  if (device == "ROBOT") {
    req.device = protos_json::io_proto::IoDevice::ROBOT;
  } else if (device == "FLANGE") {
    req.device = protos_json::io_proto::IoDevice::FLANGE;
  } else if (device == "EXTRA") {
    req.device = protos_json::io_proto::IoDevice::EXTRA;
  }
  req.pin = pin;
  return impl_->get_dio_mode(req).mode;
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
  const auto resp = impl_->get_dios_mode(req);
  return resp.modes;
}

void Robot::enable_button(std::string device, unsigned int pin) {
  protos_json::io_proto::ButtonIndex req;
  req.device = convertIoDevice(device);
  req.pin = pin;
  impl_->enable_button(req);
}

void Robot::disable_button(std::string device, unsigned int pin) {
  protos_json::io_proto::ButtonIndex req;
  req.device = convertIoDevice(device);
  req.pin = pin;
  impl_->disable_button(req);
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

void Robot::set_claw_ao(unsigned int address, double value) {
  protos_json::claw_proto::SetClawAoRequest req;
  req.address = address;
  req.value = value;
  impl_->set_claw_ao(req);
}

ClawData Robot::get_claw() {
  auto resp = impl_->get_claw();
  ClawData claw_data;
  claw_data.force = resp.force;
  claw_data.amplitude = resp.amplitude;
  claw_data.hold_on = resp.hold_on;
  return claw_data;
}

ClawData Robot::get_claw_data() { return get_claw(); }

double Robot::get_claw_ai(unsigned int address) {
  protos_json::claw_proto::GetClawAiRequest req;
  req.address = address;
  return impl_->get_claw_ai(req).value;
}

void Robot::set_led(unsigned int mode, unsigned int speed,
                    const std::vector<unsigned int> &color) {
  protos_json::led_proto::LedData typed_req;
  if (mode > 6 || speed > 3) {
    return;
  }
  typed_req.mode = static_cast<int>(mode);
  typed_req.speed = static_cast<int>(speed);
  for (const auto color_value : color) {
    if (color_value > 15) {
      return;
    }
    typed_req.colors.push_back(static_cast<int>(color_value));
  }
  impl_->set_led(typed_req);
}

void Robot::set_voice(unsigned int voice, unsigned int volume) {
  if (voice > 15 || volume > 3) {
    return;
  }
  protos_json::led_proto::VoiceData typed_req;
  typed_req.voice = static_cast<int>(voice);
  typed_req.volume = static_cast<int>(volume);
  impl_->set_voice(typed_req);
}

void Robot::set_fan(unsigned int status) {
  if (status > 2) {
    return;
  }
  protos_json::led_proto::FanData typed_req;
  typed_req.fan = static_cast<int>(status);
  impl_->set_fan(typed_req);
}

void Robot::set_signal(unsigned int index, int value) {
  protos_json::signal_proto::SetSignalRequest req;
  req.key = index;
  req.value = value;
  impl_->set_signal(req);
}

void Robot::set_signals(unsigned int index, std::vector<int> values) {
  protos_json::signal_proto::SetSignalsRequest req;
  req.key = index;
  req.values = values;
  impl_->set_signals(req);
}

int Robot::get_signal(unsigned int index) {
  protos_json::signal_proto::GetSignalRequest req;
  req.key = index;
  const auto resp = impl_->get_signal(req);
  return resp.value;
}

std::vector<int> Robot::get_signals(unsigned int index, unsigned int len) {
  protos_json::signal_proto::GetSignalsRequest req;
  req.key = index;
  req.len = len;
  return impl_->get_signals(req).values;
}

void Robot::wait_signal(unsigned int index, int value, std::string relation) {
  protos_json::signal_proto::WaitSignalRequest req;
  req.key = index;
  req.value = value;
  req.relation = relation;
  impl_->wait_signal(req);
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
  req.kind =
      static_cast<unsigned int>(protos_json::control_proto::TaskKind::LUA);
  req.params = params;
  const auto resp = impl_->start_task(req);
  return resp.id;
}
unsigned int Robot::start_task(const std::string &name) {
  protos_json::control_proto::StartTaskRequest req;
  req.name = name;
  req.is_parallel = false;
  req.loop_to = 1;
  req.kind =
      static_cast<unsigned int>(protos_json::control_proto::TaskKind::LUA);
  const auto resp = impl_->start_task(req);
  return resp.id;
}
std::vector<unsigned int> Robot::load_task_list() {
  const auto resp = impl_->load_task_list();
  return resp.ids;
}

std::vector<unsigned int> Robot::get_task_list() { return load_task_list(); }

std::vector<TaskData> Robot::load_running_tasks() {
  const auto resp = impl_->load_running_tasks();
  std::vector<TaskData> tasks;
  for (const auto &task : resp.tasks) {
    TaskData data;
    data.id = task.id;
    data.state = convertTaskStateToString(task.state);
    data.loop_count = task.loop_count;
    data.loop_to = task.loop_to;
    data.is_parallel = task.is_parallel;
    data.is_simu = task.is_simu;
    data.stdout_text = task.stdout_text;
    data.kind = convertTaskKindToString(task.kind);
    data.dir = task.dir;
    data.name = task.name;
    data.params = task.params;
    tasks.push_back(data);
  }
  return tasks;
}

TaskStdoutData Robot::get_task_stdout(unsigned int id) {
  protos_json::control_proto::TaskIndex task_index;
  task_index.id = id;
  const auto resp = impl_->get_task_stdout(task_index);
  TaskStdoutData data;
  data.id = resp.id;
  data.done = resp.done;
  data.stdout_text = resp.stdout_text;
  return data;
}

std::string Robot::wait_task(unsigned int id) {
  protos_json::control_proto::TaskIndex task_index;
  task_index.id = id;
  const auto resp = impl_->wait_task(task_index);
  return resp.stdout_text;
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
  const auto resp = impl_->exec_hook(req);
  if (!resp.success) {
    return 0;
  }
  return atoi(resp.error.c_str());
}
std::string Robot::load_task(unsigned int id) {
  protos_json::control_proto::TaskIndex req;
  req.id = id;
  const auto resp = impl_->load_task(req);
  return convertTaskStateToString(resp.state);
}

std::string Robot::get_task_state(unsigned int id) { return load_task(id); }

std::string Robot::load_task() {
  const auto resp = impl_->load_task();
  return convertTaskStateToString(resp.state);
}

std::string Robot::get_task_state() { return load_task(); }

KinematicsForwardResp Robot::get_forward_kin(
    const std::vector<double> &joint_positions) {
  protos_json::kinematic_proto::PoseRequest req;
  req.pose.kind = 1;
  req.pose.joint.joint = joint_positions;
  auto resp = impl_->get_forward_kin(req);
  KinematicsForwardResp kf_resp;
  kf_resp.pose = convertToCartesianPose(resp);
  kf_resp.ok = true;
  return kf_resp;
}

KinematicsForwardResp Robot::kinematics_forward(
    const std::vector<double> &joint_positions) {
  return get_forward_kin(joint_positions);
}

KinematicsInverseResp Robot::get_inverse_kin(
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
    ki_resp.joint_positions = resp.joint;
    ki_resp.ok = true;
  } catch (std::exception &e) {
    ki_resp.ok = false;
    return ki_resp;
  }
  return ki_resp;
}

KinematicsInverseResp Robot::kinematics_inverse(
    const CartesianPose &pose,
    const std::vector<double> &joint_init_positions) {
  return get_inverse_kin(pose, joint_init_positions);
}

double Robot::measure_manipulation(
    const std::vector<double> &joint_positions) {
  protos_json::posture_proto::JointPose req;
  req.joint = joint_positions;
  return impl_->measure_manipulation(req).manipulation;
}

CartesianPose Robot::get_pose_trans(const CartesianPose &a,
                                    const CartesianPose &b) {
  protos_json::kinematic_proto::GetPoseTransRequest req;
  req.from.kind = 0;
  req.from.cart = convertToKinematicPose(a);
  req.from_to.kind = 0;
  req.from_to.cart = convertToKinematicPose(b);

  auto resp = impl_->get_pose_trans(req);
  return convertToCartesianPose(resp);
}

CartesianPose Robot::pose_times(const CartesianPose &a,
                                const CartesianPose &b) {
  return get_pose_trans(a, b);
}

CartesianPose Robot::get_pose_add(const CartesianPose &pose,
                                  const CartesianPose &delta) {
  protos_json::kinematic_proto::GetPoseAddRequest req;
  req.pose.kind = 0;
  req.pose.cart = convertToKinematicPose(pose);
  req.delta.kind = 0;
  req.delta.cart = convertToKinematicPose(delta);
  auto resp = impl_->get_pose_add(req);
  return convertToCartesianPose(resp);
}

CartesianPose Robot::calc_frame(const CartesianPose &o, const CartesianPose &x,
                                const CartesianPose &xy) {
  protos_json::kinematic_proto::CalcFrameRequest req;
  req.o = convertToKinematicPose(o);
  req.x = convertToKinematicPose(x);
  req.xy = convertToKinematicPose(xy);
  auto resp = impl_->calc_frame(req);
  return convertToCartesianPose(resp);
}

CartesianPose Robot::get_pose_inverse(const CartesianPose &in) {
  protos_json::kinematic_proto::PoseRequest req;
  req.pose.kind = 0;
  req.pose.cart = convertToKinematicPose(in);
  auto resp = impl_->get_pose_inverse(req);
  return convertToCartesianPose(resp);
}

CartesianPose Robot::pose_inverse(const CartesianPose &in) {
  return get_pose_inverse(in);
}

void Robot::save_file(const std::string &dir, const std::string &name,
                      bool is_dir, const std::string &data) {
  protos_json::file_proto::SaveFileRequest req;
  req.dir = dir;
  req.name = name;
  req.file.is_dir = is_dir;
  req.file.data = base64::encode(data);
  impl_->save_file(req);
}
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

std::tuple<bool, std::string> Robot::load_file(const std::string &dir,
                                               const std::string &name) {
  protos_json::file_proto::FileIndex req;
  req.dir = dir;
  req.name = name;
  const auto resp = impl_->load_file(req);
  std::tuple<bool, std::string> ret =
      std::make_tuple(resp.is_dir, base64::decode(resp.data));
  return ret;
}
std::vector<std::tuple<bool, std::string>> Robot::load_file_list(
    const std::string &dir, const std::string &prefix,
    const std::string &suffix) {
  protos_json::file_proto::LoadFileListRequest req;
  req.dir = dir;
  req.prefix = prefix;
  req.suffix = suffix;
  const auto resp = impl_->load_file_list(req);
  std::vector<std::tuple<bool, std::string>> ret;
  for (auto f : resp.files) {
    std::tuple<bool, std::string> temp = std::make_tuple(f.is_dir, f.name);
    ret.push_back(temp);
  }
  return ret;
}

void Robot::zip(const std::string &from_dir, std::vector<std::string> files,
                const std::string &to_dir, const std::string &name) {
  protos_json::file_proto::ZipRequest req;
  req.zip.dir = to_dir;
  req.zip.name = name;
  req.files = files;
  req.dir = from_dir;
  impl_->zip(req);
}

void Robot::unzip(const std::string &from_dir, const std::string &name,
                  std::vector<std::string> files, const std::string &to_dir) {
  protos_json::file_proto::UnzipRequest req;
  req.zip.dir = from_dir;
  req.zip.name = name;
  req.files = files;
  req.dir = to_dir;
  impl_->unzip(req);
}

std::vector<std::tuple<bool, std::string>> Robot::load_zip_list(
    const std::string &zip, const std::string &dir, const std::string &prefix,
    const std::string &suffix) {
  protos_json::file_proto::LoadZipListRequest req;
  req.zip.name = zip;
  req.dir = dir;
  req.prefix = prefix;
  req.suffix = suffix;
  const auto resp = impl_->load_zip_list(req);
  std::vector<std::tuple<bool, std::string>> ret;
  for (auto f : resp.files) {
    ret.push_back(std::make_tuple(f.is_dir, f.name));
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

void Robot::save_payload(std::string name, std::map<std::string, double> payload,
                         std::string dir) {
  protos_json::dynamic_proto::SavePayloadRequest req;
  req.name = name;
  req.dir = dir;
  req.data.mass = payload.at("mass");
  req.data.cog.x = payload.at("x");
  req.data.cog.y = payload.at("y");
  req.data.cog.z = payload.at("z");
  impl_->save_payload(req);
}

std::map<std::string, double> Robot::get_payload() {
  const auto resp = impl_->get_payload();
  std::map<std::string, double> cog;
  cog["x"] = resp.cog.x;
  cog["y"] = resp.cog.y;
  cog["z"] = resp.cog.z;
  cog["mass"] = resp.mass;
  return cog;
}

std::map<std::string, double> Robot::load_payload(std::string name,
                                                  std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto resp = impl_->load_payload(req);
  std::map<std::string, double> payload;
  payload["x"] = resp.cog.x;
  payload["y"] = resp.cog.y;
  payload["z"] = resp.cog.z;
  payload["mass"] = resp.mass;
  return payload;
}

std::vector<std::string> Robot::load_payload_list(std::string dir) {
  protos_json::db_proto::LoadListRequest req;
  req.dir = dir;
  return impl_->load_payload_list(req).names;
}

void Robot::set_gravity(std::map<std::string, double> gravity) {
  protos_json::posture_proto::Position req;
  req.x = gravity.at("x");
  req.y = gravity.at("y");
  req.z = gravity.at("z");
  impl_->set_gravity(req);
}

std::map<std::string, double> Robot::get_gravity() {
  const auto resp = impl_->get_gravity();
  std::map<std::string, double> gravity;
  gravity["x"] = resp.x;
  gravity["y"] = resp.y;
  gravity["z"] = resp.z;
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
  const auto resp = impl_->get_collision_torque_diff();
  return resp.diffs;
}

void Robot::set_collision_detector(const CollisionDetectorConfig &config) {
  protos_json::safety_proto::CollisionDetector req;
  req.action = convertCollisionDetectorActionToWire(config.action);
  req.pause_time = config.pause_time;
  req.sensitivity = config.sensitivity;
  impl_->set_collision_detector(req);
}

CollisionDetectorConfig Robot::get_collision_detector() {
  const auto resp = impl_->get_collision_detector();
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
  const auto resp = impl_->get_joints_limit();
  std::vector<JointLimitConfig> joints;
  for (auto &&joint : resp.joints) {
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
  const auto resp = impl_->get_cart_limit();
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
  const auto resp = impl_->get_tcp();
  std::array<double, 6> ret;
  ret[0] = resp.position.x;
  ret[1] = resp.position.y;
  ret[2] = resp.position.z;
  ret[3] = resp.rotation.euler_zyx.z;
  ret[4] = resp.rotation.euler_zyx.y;
  ret[5] = resp.rotation.euler_zyx.x;
  return ret;
}

std::vector<DhParamData> Robot::get_dh() {
  const auto response = impl_->get_dh();
  std::vector<DhParamData> params;
  for (const auto &param : response.params) {
    DhParamData data;
    data.a = param.a;
    data.alpha = param.alpha;
    data.d = param.d;
    data.theta = param.theta;
    params.push_back(data);
  }
  return params;
}

void Robot::set_kin_factor(int factor) {
  protos_json::kin_factor_proto::KinFactor req;
  req.speed_factor = factor;
  impl_->set_kin_factor(req);
}

void Robot::set_velocity_factor(int factor) { set_kin_factor(factor); }

int Robot::get_kin_factor() {
  const auto resp = impl_->get_kin_factor();
  return resp.speed_factor;
}

int Robot::get_velocity_factor() { return get_kin_factor(); }
void Robot::save_tcp(std::string name, CartesianPose tcp, std::string dir) {
  protos_json::kinematic_proto::SaveTcpRequest req;
  req.name = name;
  req.data = convertToPosturePose(tcp);
  req.dir = dir;
  impl_->save_tcp(req);
}

CartesianPose Robot::load_tcp(std::string name, std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto &pose = impl_->load_tcp(req);
  return convertToCartesianPose(pose);
}

std::vector<std::string> Robot::load_tcp_list(std::string dir) {
  protos_json::db_proto::LoadListRequest req;
  req.dir = dir;
  return impl_->load_tcp_list(req).names;
}

std::vector<std::string> Robot::load_trajectory_list(std::string dir) {
  protos_json::db_proto::LoadListRequest req;
  req.dir = dir;
  return impl_->load_trajectory_list(req).names;
}

TrajectoryData Robot::load_trajectory(std::string name, std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto resp = impl_->load_trajectory(req);
  TrajectoryData trajectory;
  trajectory.kind = resp.kind;
  for (const auto &point : resp.data) {
    PvatPointData point_data;
    point_data.duration = point.duration;
    for (const auto &joint : point.joints) {
      point_data.joints.push_back(convertToJointMoveData(joint));
    }
    trajectory.data.push_back(point_data);
  }
  return trajectory;
}

void Robot::save_trajectory(std::string name, TrajectoryData trajectory,
                            std::string dir) {
  protos_json::motion_proto::SaveTrajectoryRequest req;
  req.name = name;
  req.data = convertToMotionTrajectory(trajectory);
  req.dir = dir;
  impl_->save_trajectory(req);
}

void Robot::start_record_trajectory(std::string kind, double duration) {
  protos_json::motion_proto::StartRecordTrajectoryRequest req;
  req.kind = kind;
  req.duration = duration;
  impl_->start_record_trajectory(req);
}

void Robot::end_record_trajectory(std::string name, std::string dir) {
  protos_json::motion_proto::EndRecordTrajectoryRequest req;
  req.name = name;
  req.dir = dir;
  impl_->end_record_trajectory(req);
}

std::vector<std::string> Robot::load_pose_list(std::string dir) {
  protos_json::db_proto::LoadListRequest req;
  req.dir = dir;
  return impl_->load_pose_list(req).names;
}

PoseData Robot::load_pose(std::string name, std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto resp = impl_->load_pose(req);
  PoseData data;
  data.kind = resp.kind;
  data.cart = convertToCartesianPose(resp.cart);
  data.joint = resp.joint.joint;
  return data;
}

void Robot::save_pose(std::string name, PoseData pose, std::string dir) {
  protos_json::posture_proto::SavePoseRequest req;
  req.name = name;
  req.dir = dir;
  req.data.kind = pose.kind;
  req.data.cart = convertToPosturePose(pose.cart);
  req.data.joint.joint = pose.joint;
  impl_->save_pose(req);
}

std::vector<std::string> Robot::load_frame_list(std::string dir) {
  protos_json::db_proto::LoadListRequest req;
  req.dir = dir;
  return impl_->load_frame_list(req).names;
}

FrameData Robot::load_frame(std::string name, std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto resp = impl_->load_frame(req);
  FrameData data;
  data.position_kind = resp.position_kind;
  data.position = convertToPositionPose(resp.position);
  data.rotation_kind = resp.rotation_kind;
  data.rotation = convertToRotationPose(resp.rotation);
  return data;
}

void Robot::save_frame(std::string name, FrameData frame, std::string dir) {
  protos_json::posture_proto::SaveFrameRequest req;
  req.name = name;
  req.dir = dir;
  req.data.position_kind = frame.position_kind;
  req.data.position.x =
      frame.position.count("x") ? frame.position.at("x") : 0.0;
  req.data.position.y =
      frame.position.count("y") ? frame.position.at("y") : 0.0;
  req.data.position.z =
      frame.position.count("z") ? frame.position.at("z") : 0.0;
  req.data.rotation_kind = frame.rotation_kind;
  req.data.rotation.euler_zyx.x =
      frame.rotation.count("rx") ? frame.rotation.at("rx") : 0.0;
  req.data.rotation.euler_zyx.y =
      frame.rotation.count("ry") ? frame.rotation.at("ry") : 0.0;
  req.data.rotation.euler_zyx.z =
      frame.rotation.count("rz") ? frame.rotation.at("rz") : 0.0;
  impl_->save_frame(req);
}

StructureData Robot::load_structure(std::string name, std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto resp = impl_->load_structure(req);
  StructureData data;
  data.active = resp.active;
  data.name = resp.name;
  data.kind = resp.kind;
  data.dof = resp.dof;
  data.dh = resp.dh;
  data.dyn = resp.dyn;
  data.servo = resp.servo;
  return data;
}

void Robot::save_structure(std::string name, StructureData structure,
                           std::string dir) {
  protos_json::structure_proto::SaveStructureRequest req;
  req.name = name;
  req.dir = dir;
  req.data.active = structure.active;
  req.data.name = structure.name;
  req.data.kind = structure.kind;
  req.data.dof = structure.dof;
  req.data.dh = structure.dh;
  req.data.dyn = structure.dyn;
  req.data.servo = structure.servo;
  impl_->save_structure(req);
}

std::vector<std::string> Robot::load_structure_list(std::string dir) {
  protos_json::db_proto::LoadListRequest req;
  req.dir = dir;
  return impl_->load_structure_list(req).names;
}

std::vector<std::string> Robot::load_modbus_list(std::string dir) {
  protos_json::db_proto::LoadListRequest req;
  req.dir = dir;
  return impl_->load_modbus_list(req).names;
}

ModbusData Robot::load_modbus(std::string name, std::string dir) {
  protos_json::db_proto::LoadRequest req;
  req.name = name;
  req.dir = dir;
  const auto resp = impl_->load_modbus(req);
  ModbusData data;
  data.kind = resp.kind;
  data.address = resp.address;
  data.port = resp.port;
  data.slave_id = resp.slave_id;
  return data;
}

void Robot::save_modbus(std::string name, ModbusData modbus) {
  protos_json::modbus_proto::SaveModbusRequest req;
  req.name = name;
  req.data.kind = modbus.kind;
  req.data.address = modbus.address;
  req.data.port = modbus.port;
  req.data.slave_id = modbus.slave_id;
  impl_->save_modbus(req);
}

void Robot::set_modbus_timeout(std::string device, unsigned int timeout) {
  protos_json::modbus_proto::SetModbusTimeoutRequest req;
  req.device = device;
  req.timeout = timeout;
  impl_->set_modbus_timeout(req);
}

void Robot::set_modbus_retry(std::string device, unsigned int retry) {
  protos_json::modbus_proto::SetModbusRetryRequest req;
  req.device = device;
  req.retry = retry;
  impl_->set_modbus_retry(req);
}

std::vector<std::string> Robot::load_modbus_register_list(std::string device) {
  protos_json::modbus_proto::LoadModbusRegisterListRequest req;
  req.device = device;
  return impl_->load_modbus_register_list(req).names;
}

ModbusRegisterData Robot::load_modbus_register(std::string device,
                                               std::string name) {
  protos_json::modbus_proto::LoadModbusRegisterRequest req;
  req.device = device;
  req.name = name;
  const auto resp = impl_->load_modbus_register(req);
  ModbusRegisterData data;
  data.kind = resp.kind;
  data.address = resp.address;
  return data;
}

void Robot::save_modbus_register(std::string device, std::string name,
                                 ModbusRegisterData reg) {
  protos_json::modbus_proto::SaveModbusRegisterRequest req;
  req.device = device;
  req.name = name;
  req.data.kind = reg.kind;
  req.data.address = reg.address;
  impl_->save_modbus_register(req);
}

void Robot::write_single_coil(std::string device, std::string addr,
                              bool value) {
  protos_json::modbus_proto::SetCoilRequest req;
  req.device = device;
  req.pin = addr;
  req.value = value;
  impl_->write_single_coil(req);
}

void Robot::write_multiple_coils(std::string device, std::string addr,
                                  std::vector<bool> values) {
  protos_json::modbus_proto::SetCoilsRequest req;
  req.device = device;
  req.pin = addr;
  req.values = values;
  impl_->write_multiple_coils(req);
}

void Robot::wirte_multiple_coils(std::string device, std::string addr,
                                 std::vector<bool> values) {
  write_multiple_coils(device, addr, values);
}
std::vector<bool> Robot::read_coils(std::string device, std::string addr,
                                    unsigned int num) {
  protos_json::modbus_proto::GetCoilsRequest req;
  req.device = device;
  req.pin = addr;
  req.count = num;
  const auto resp = impl_->read_coils(req);
  return resp.values;
}
std::vector<bool> Robot::read_discrete_inputs(std::string device,
                                              std::string addr,
                                              unsigned int num) {
  protos_json::modbus_proto::GetCoilsRequest req;
  req.device = device;
  req.pin = addr;
  req.count = num;
  const auto resp = impl_->read_discrete_inputs(req);
  return resp.values;
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
  const auto resp = impl_->read_holding_registers(req);
  return resp.values;
}
std::vector<unsigned int> Robot::read_input_registers(std::string device,
                                                      std::string addr,
                                                      unsigned int num) {
  protos_json::modbus_proto::GetRegistersRequest req;
  req.device = device;
  req.pin = addr;
  req.count = num;
  const auto resp = impl_->read_input_registers(req);
  return resp.values;
}

void Robot::set_serial_baud_rate(std::string device, unsigned int baud_rate) {
  protos_json::serial_proto::SetSerialBaudRateRequest req;
  req.device = device;
  req.baud_rate = baud_rate;
  impl_->set_serial_baud_rate(req);
}

void Robot::set_serial_timeout(std::string device, unsigned int timeout) {
  protos_json::serial_proto::SetSerialTimeoutRequest req;
  req.device = device;
  req.timeout = timeout;
  impl_->set_serial_timeout(req);
}

void Robot::set_serial_parity(std::string device, unsigned int parity) {
  protos_json::serial_proto::SetSerialParityRequest req;
  req.device = device;
  req.parity = parity;
  impl_->set_serial_parity(req);
}

void Robot::write_serial(std::string device, std::vector<unsigned int> data) {
  protos_json::serial_proto::WriteSerialRequest req;
  req.device = device;
  req.data = data;
  impl_->write_serial(req);
}

std::vector<unsigned int> Robot::read_serial(std::string device,
                                             unsigned int len) {
  protos_json::serial_proto::ReadSerialRequest req;
  req.device = device;
  req.len = len;
  return impl_->read_serial(req).data;
}

void Robot::clear_serial(std::string device) {
  protos_json::serial_proto::ClearSerialRequest req;
  req.device = device;
  impl_->clear_serial(req);
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
  const auto resp = impl_->get_item(req);
  StorageItem item;
  item.key = resp.key;
  item.value = resp.value;
  return item;
}

std::vector<StorageItem> Robot::get_items(std::string prefix) {
  protos_json::storage_proto::GetItemsRequest req;
  req.prefix = prefix;
  const auto resp = impl_->get_items(req);
  std::vector<StorageItem> ret;
  for (const auto &item : resp.items) {
    StorageItem storage_item;
    storage_item.key = item.key;
    storage_item.value = item.value;
    ret.push_back(storage_item);
  }
  return ret;
}

}  // namespace l_master

}  // namespace lebai
