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
#include "robot_impl.hh"
#include "protos/motion.hh"
#include "protos/serial.hh"
#include <lebai/config.hh>

namespace lebai {

namespace l_master {

std::string version() { return LEBAI_SDK_VERSION_STR; }

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

void Robot::start_sys() { impl_->startSys(); }

void Robot::stop_sys() { impl_->stopSys(); }

void Robot::powerdown() { impl_->powerdown(); }

void Robot::stop() { impl_->stop(); }

void Robot::estop() { impl_->estop(); }

void Robot::teach_mode() { impl_->teachMode(); }

void Robot::end_teach_mode() { impl_->endTeachMode(); }

void Robot::pause() { impl_->pause(); }

void Robot::resume() { impl_->resume(); }

void Robot::reboot() { impl_->reboot(); }

int Robot::movej(const std::vector<double> &joint_positions, double a, double v,
                 double t, double r) {
  motion::MoveRequest move_req;
  move_req.mutable_param()->set_acc(a);
  move_req.mutable_param()->set_velocity(v);
  move_req.mutable_param()->set_time(t);
  move_req.mutable_param()->set_radius(r);
  motion::MotionIndex resp;
  for (auto &&p : joint_positions) {
    move_req.mutable_pose()->mutable_joint()->mutable_joint()->push_back(p);
  }
  resp = impl_->moveJoint(move_req);
  return resp.id();
}

int Robot::movej(const CartesianPose &cart_pose, double a, double v, double t,
                 double r) {
  motion::MoveRequest move_req;
  move_req.mutable_param()->set_acc(a);
  move_req.mutable_param()->set_velocity(v);
  move_req.mutable_param()->set_time(t);
  move_req.mutable_param()->set_radius(r);
  if (cart_pose.find("x") != cart_pose.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_x(
        cart_pose.at("x"));
  } else {
    return -1;
  }
  if (cart_pose.find("y") != cart_pose.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_y(
        cart_pose.at("y"));
  } else {
    return -1;
  }
  if (cart_pose.find("z") != cart_pose.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_z(
        cart_pose.at("z"));
  } else {
    return -1;
  }
  if (cart_pose.find("rx") != cart_pose.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_x(cart_pose.at("rx"));
  } else {
    return -1;
  }
  if (cart_pose.find("ry") != cart_pose.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_y(cart_pose.at("ry"));
  } else {
    return -1;
  }
  if (cart_pose.find("rz") != cart_pose.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_z(cart_pose.at("rz"));
  } else {
    return -1;
  }
  motion::MotionIndex resp = impl_->moveJoint(move_req);
  return resp.id();
}

int Robot::movel(const std::vector<double> &joint_positions, double a, double v,
                 double t, double r) {
  motion::MoveRequest move_req;
  move_req.mutable_param()->set_acc(a);
  move_req.mutable_param()->set_velocity(v);
  move_req.mutable_param()->set_time(t);
  move_req.mutable_param()->set_radius(r);
  for (auto &&p : joint_positions) {
    move_req.mutable_pose()->mutable_joint()->mutable_joint()->push_back(p);
  }
  motion::MotionIndex resp = impl_->moveLinear(move_req);
  return resp.id();
}

int Robot::movel(const CartesianPose &cart_pose, double a, double v, double t,
                 double r) {
  motion::MoveRequest move_req;
  move_req.mutable_param()->set_acc(a);
  move_req.mutable_param()->set_velocity(v);
  move_req.mutable_param()->set_time(t);
  move_req.mutable_param()->set_radius(r);
  if (cart_pose.find("x") != cart_pose.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_x(
        cart_pose.at("x"));
  } else {
    return -1;
  }
  if (cart_pose.find("y") != cart_pose.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_y(
        cart_pose.at("y"));
  } else {
    return -1;
  }
  if (cart_pose.find("z") != cart_pose.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_z(
        cart_pose.at("z"));
  } else {
    return -1;
  }
  if (cart_pose.find("rx") != cart_pose.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_x(cart_pose.at("rx"));
  } else {
    return -1;
  }
  if (cart_pose.find("ry") != cart_pose.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_y(cart_pose.at("ry"));
  } else {
    return -1;
  }
  if (cart_pose.find("rz") != cart_pose.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_z(cart_pose.at("rz"));
  } else {
    return -1;
  }
  motion::MotionIndex resp = impl_->moveLinear(move_req);
  return resp.id();
}
int Robot::movec(const std::vector<double> &joint_via,
                 const std::vector<double> &joint, double rad, double a,
                 double v, double t, double r) {
  motion::MovecRequest move_req;
  move_req.mutable_param()->set_acc(a);
  move_req.mutable_param()->set_velocity(v);
  move_req.mutable_param()->set_time(t);
  move_req.mutable_param()->set_radius(r);
  move_req.set_rad(rad);
  for (auto &&p : joint_via) {
    move_req.mutable_pose_via()->mutable_joint()->mutable_joint()->push_back(p);
  }
  for (auto &&p : joint) {
    move_req.mutable_pose()->mutable_joint()->mutable_joint()->push_back(p);
  }
  motion::MotionIndex resp = impl_->moveCircular(move_req);
  return resp.id();
}

int Robot::movec(const CartesianPose &cart_via, const CartesianPose &cart,
                 double rad, double a, double v, double t, double r) {
  motion::MovecRequest move_req;
  move_req.mutable_param()->set_acc(a);
  move_req.mutable_param()->set_velocity(v);
  move_req.mutable_param()->set_time(t);
  move_req.mutable_param()->set_radius(r);
  move_req.set_rad(rad);
  if (cart_via.find("x") != cart_via.end()) {
    move_req.mutable_pose_via()->mutable_cart()->mutable_position()->set_x(
        cart_via.at("x"));
  } else {
    return -1;
  }
  if (cart_via.find("y") != cart_via.end()) {
    move_req.mutable_pose_via()->mutable_cart()->mutable_position()->set_y(
        cart_via.at("y"));
  } else {
    return -1;
  }
  if (cart_via.find("z") != cart_via.end()) {
    move_req.mutable_pose_via()->mutable_cart()->mutable_position()->set_z(
        cart_via.at("z"));
  } else {
    return -1;
  }
  if (cart_via.find("rx") != cart_via.end()) {
    move_req.mutable_pose_via()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_x(cart_via.at("rx"));
  } else {
    return -1;
  }
  if (cart_via.find("ry") != cart_via.end()) {
    move_req.mutable_pose_via()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_y(cart_via.at("ry"));
  } else {
    return -1;
  }
  if (cart_via.find("rz") != cart_via.end()) {
    move_req.mutable_pose_via()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_z(cart_via.at("rz"));
  } else {
    return -1;
  }
  if (cart.find("x") != cart.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_x(
        cart.at("x"));
  } else {
    return -1;
  }
  if (cart.find("y") != cart.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_y(
        cart.at("y"));
  } else {
    return -1;
  }
  if (cart.find("z") != cart.end()) {
    move_req.mutable_pose()->mutable_cart()->mutable_position()->set_z(
        cart.at("z"));
  } else {
    return -1;
  }
  if (cart.find("rx") != cart.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_x(cart.at("rx"));
  } else {
    return -1;
  }
  if (cart.find("ry") != cart.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_y(cart.at("ry"));
  } else {
    return -1;
  }
  if (cart.find("rz") != cart.end()) {
    move_req.mutable_pose()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_z(cart.at("rz"));
  } else {
    return -1;
  }
  motion::MotionIndex resp = impl_->moveCircular(move_req);
  return resp.id();
}

int Robot::speedj(double a, const std::vector<double> &v, double t) {
  motion::SpeedJRequest req;
  req.mutable_param()->set_acc(a);
  req.mutable_param()->set_time(a);
  for (auto &&p : v) {
    req.mutable_speed()->mutable_joint()->push_back(p);
  }
  lebai::motion::MotionIndex resp = impl_->speedJoint(req);
  return resp.id();
}

int Robot::speedl(double a, const CartesianPose &v, double t,
                  const CartesianPose &reference) {
  motion::SpeedLRequest req;
  req.mutable_param()->set_acc(a);
  req.mutable_param()->set_time(a);
  if (v.find("x") != v.end()) {
    req.mutable_speed()->mutable_position()->set_x(v.at("x"));
  } else {
    return -1;
  }
  if (v.find("y") != v.end()) {
    req.mutable_speed()->mutable_position()->set_y(v.at("y"));
  } else {
    return -1;
  }
  if (v.find("z") != v.end()) {
    req.mutable_speed()->mutable_position()->set_z(v.at("z"));
  } else {
    return -1;
  }
  if (v.find("rx") != v.end()) {
    req.mutable_speed()->mutable_rotation()->mutable_euler_zyx()->set_x(
        v.at("rx"));
  } else {
    return -1;
  }
  if (v.find("ry") != v.end()) {
    req.mutable_speed()->mutable_rotation()->mutable_euler_zyx()->set_y(
        v.at("ry"));
  } else {
    return -1;
  }
  if (v.find("rz") != v.end()) {
    req.mutable_speed()->mutable_rotation()->mutable_euler_zyx()->set_z(
        v.at("rz"));
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
  req.mutable_frame()->set_position_kind(posture::CartesianFrame::Kind::CUSTOM);
  req.mutable_frame()->set_rotation_kind(posture::CartesianFrame::Kind::CUSTOM);

  if (reference.find("x") != reference.end()) {
    req.mutable_frame()->mutable_position()->set_x(reference.at("x"));
  } else {
    return -1;
  }
  if (reference.find("y") != reference.end()) {
    req.mutable_frame()->mutable_position()->set_y(reference.at("y"));
  } else {
    return -1;
  }
  if (reference.find("z") != reference.end()) {
    req.mutable_frame()->mutable_position()->set_z(reference.at("z"));
  } else {
    return -1;
  }
  if (reference.find("rx") != reference.end()) {
    req.mutable_frame()->mutable_rotation()->mutable_euler_zyx()->set_x(
        reference.at("rx"));
  } else {
    return -1;
  }
  if (reference.find("ry") != reference.end()) {
    req.mutable_frame()->mutable_rotation()->mutable_euler_zyx()->set_y(
        reference.at("ry"));
  } else {
    return -1;
  }
  if (reference.find("rz") != reference.end()) {
    req.mutable_frame()->mutable_rotation()->mutable_euler_zyx()->set_z(
        reference.at("rz"));
  } else {
    return -1;
  }
  lebai::motion::MotionIndex resp = impl_->speedLinear(req);
  return resp.id();
}

int Robot::towardj(const std::vector<double> &joint_positions, double a,
                   double v, double t, double r) {
  motion::MoveRequest move_req;
  move_req.mutable_param()->set_acc(a);
  move_req.mutable_param()->set_velocity(v);
  move_req.mutable_param()->set_time(t);
  move_req.mutable_param()->set_radius(r);
  for (auto &&p : joint_positions) {
    move_req.mutable_pose()->mutable_joint()->mutable_joint()->push_back(p);
  }
  motion::MotionIndex resp = impl_->towardJoint(move_req);
  return resp.id();
}

void Robot::move_pvat(std::vector<double> p, std::vector<double> v,
                      std::vector<double> a, double t) {
  motion::MovePvatRequest req;
  std::vector<motion::JointMove> joints;
  for (int i = 0; i < p.size(); i++) {
    motion::JointMove joint;
    joint.set_pose(p[i]);
    joint.set_velocity(v[i]);
    joint.set_acc(a[i]);
    joints.push_back(joint);
  }
  req.set_duration(t);
  req.set_joints(joints);
  impl_->movePvat(req);
}

void Robot::wait_move(unsigned int id) {
  motion::MotionIndex req;
  req.set_id(id);
  impl_->waitMove(req);
}
void Robot::wait_move() {
  motion::MotionIndex req;
  req.set_id(0);
  impl_->waitMove(req);
}

unsigned int Robot::get_running_motion() {
  motion::MotionIndex resp = impl_->getRunningMotion();
  return resp.id();
}

std::string Robot::get_motion_state(unsigned int id) {
  motion::MotionIndex req;
  req.set_id(id);
  motion::GetMotionStateResponse resp = impl_->getMotionState(req);
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

void Robot::stop_move() { impl_->stopMove(); }

int Robot::get_robot_mode() { return impl_->getRobotState(); }

int Robot::get_estop_reason() { return impl_->getEstopReason(); }

bool Robot::is_disconnected() { return impl_->getRobotState() == 0; }

bool Robot::is_down() { return impl_->getRobotState() < 4; }

std::vector<double> Robot::get_actual_joint_positions() {
  std::map<std::string, double> ret;
  return *impl_->getKinData().mutable_actual_joint_pose();
}

std::vector<double> Robot::get_target_joint_positions() {
  std::map<std::string, double> ret;
  return *impl_->getKinData().mutable_target_joint_pose();
}
std::vector<double> Robot::get_actual_joint_speed() {
  return *impl_->getKinData().mutable_actual_joint_speed();
}
std::vector<double> Robot::get_target_joint_speed() {
  return *impl_->getKinData().mutable_target_joint_speed();
}

CartesianPose Robot::get_actual_tcp_pose() {
  auto pose = impl_->getKinData().actual_tcp_pose();
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
CartesianPose Robot::get_target_tcp_pose() {
  auto pose = impl_->getKinData().target_tcp_pose();
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

double Robot::get_joint_temp(unsigned int joint_index) {
  auto data = impl_->getPhyData();
  joint_index -= 1;
  if (data.joint_temp().size() > joint_index) {
    return data.joint_temp()[joint_index];
  }
  return 0.0;
}

std::vector<double> Robot::get_actual_joint_torques() {
  return impl_->getKinData().actual_joint_torque();
}

std::vector<double> Robot::get_target_joint_torques() {
  return impl_->getKinData().target_joint_torque();
}

void Robot::set_do(std::string device, unsigned int pin, unsigned int value) {
  io::SetDoPinRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  req.set_value(value);
  impl_->setDO(req);
}

unsigned int Robot::get_do(std::string device, unsigned int pin) {
  io::GetDioPinRequest req;

  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }

  req.set_pin(pin);
  io::GetDioPinResponse resp = impl_->getDO(req);
  return resp.value();
}

std::vector<unsigned int> Robot::get_dos(std::string device, unsigned int pin,
                                         unsigned int num) {
  io::GetDioPinsRequest req;

  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }

  req.set_pin(pin);
  req.set_count(num);
  io::GetDioPinsResponse resp = impl_->getDOS(req);
  return resp.values();
}

unsigned int Robot::get_di(std::string device, unsigned int pin) {
  io::GetDioPinRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }

  req.set_pin(pin);
  io::GetDioPinResponse resp = impl_->getDI(req);
  return resp.value();
}

std::vector<unsigned int> Robot::get_dis(std::string device, unsigned int pin,
                                         unsigned int num) {
  io::GetDioPinsRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  req.set_count(num);
  io::GetDioPinsResponse resp = impl_->getDIS(req);
  return resp.values();
}

void Robot::set_ao(std::string device, unsigned int pin, double value) {
  io::SetAoPinRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  req.set_value(value);
  impl_->setAO(req);
}

double Robot::get_ao(std::string device, unsigned int pin) {
  io::GetAioPinRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  io::GetAioPinResponse resp = impl_->getAO(req);
  return resp.value();
}

std::vector<double> Robot::get_aos(std::string device, unsigned int pin,
                                   unsigned int num) {
  io::GetAioPinsRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  req.set_count(num);
  io::GetAioPinsResponse resp = impl_->getAOS(req);
  return resp.values();
}

double Robot::get_ai(std::string device, unsigned int pin) {
  io::GetAioPinRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  io::GetAioPinResponse resp = impl_->getAI(req);
  return resp.value();
}

std::vector<double> Robot::get_ais(std::string device, unsigned int pin,
                                   unsigned int num) {
  io::GetAioPinsRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  req.set_count(num);
  io::GetAioPinsResponse resp = impl_->getAIS(req);
  return resp.values();
}
void Robot::set_dio_mode(std::string device, unsigned int pin, bool value) {
  io::SetDioModeRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  req.set_value(value);
  impl_->setDioMode(req);
}
std::vector<bool> Robot::get_dios_mode(std::string device, unsigned int pin,
                                       unsigned int count) {
  io::GetDiosModeRequest req;
  if (device == "ROBOT") {
    req.set_device(io::IoDevice::ROBOT);
  } else if (device == "FLANGE") {
    req.set_device(io::IoDevice::FLANGE);
  } else if (device == "EXTRA") {
    req.set_device(io::IoDevice::EXTRA);
  }
  req.set_pin(pin);
  req.set_count(count);
  io::GetDiosModeResponse resp = impl_->getDiosMode(req);
  return resp.values();
}

void Robot::set_claw(double force, double amplitude) {
  claw::SetClawRequest req;
  req.set_force(force);
  req.set_amplitude(amplitude);
  impl_->setClaw(req);
}

std::tuple<double, double, bool> Robot::get_claw() {
  auto resp = impl_->getClaw();
  return std::make_tuple(resp.force(), resp.amplitude(), resp.hold_on());
}

ClawData Robot::get_claw_data() {
  auto resp = impl_->getClaw();
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
  impl_->setLed(req);
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
  impl_->setVoice(req);
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
  impl_->setFan(req);
}

void Robot::set_signal(unsigned int index, int value) {
  signal::SetSignalRequest req;
  req.set_key(index);
  req.set_value(value);
  impl_->setSignal(req);
}
int Robot::get_signal(unsigned int index) {
  signal::GetSignalRequest req;
  req.set_key(index);
  signal::GetSignalResponse resp = impl_->getSignal(req);
  return resp.value();
}
void Robot::add_signal(unsigned int index, int value) {
  signal::SetSignalRequest req;
  req.set_key(index);
  req.set_value(value);
  impl_->addSignal(req);
}

unsigned int Robot::start_task(const std::string &name,
                               const std::vector<std::string> &params,
                               const std::string &dir, bool is_parallel,
                               unsigned int loop_to) {
  control::StartTaskRequest req;
  req.set_name(name);
  req.set_is_parallel(is_parallel);
  req.set_loop_to(loop_to);
  req.set_dir(dir);
  req.set_params(params);
  control::TaskIndex resp = impl_->scene(req);
  return resp.id();
}
unsigned int Robot::start_task(const std::string &name) {
  control::StartTaskRequest req;
  req.set_name(name);
  req.set_is_parallel(false);
  req.set_loop_to(1);
  control::TaskIndex resp = impl_->scene(req);
  return resp.id();
}
std::vector<unsigned int> Robot::load_task_list() {
  control::TaskIds resp = impl_->loadTaskList();
  return resp.ids();
}
bool Robot::wait_task(unsigned int id) {
  control::TaskIndex task_index;
  task_index.set_id(id);
  control::TaskStdout resp = impl_->waitTask(task_index);
  return resp.done();
}
void Robot::pause_task(unsigned int id) {
  control::PauseRequest req;
  req.set_id(id);
  req.set_time(0);
  req.set_wait(false);
  impl_->pauseTask(req);
}
void Robot::pause_task(unsigned int id, unsigned long time, bool wait) {
  control::PauseRequest req;
  req.set_id(id);
  req.set_time(time);
  req.set_wait(wait);
  impl_->pauseTask(req);
}
void Robot::resume_task(unsigned int id) {
  control::TaskIndex req;
  req.set_id(id);
  impl_->resumeTask(req);
}
void Robot::cancel_task(unsigned int id) {
  control::TaskIndex req;
  req.set_id(id);
  impl_->cancelTask(req);
}
unsigned int Robot::exec_hook(unsigned int id) {
  control::Exec req;
  req.set_id(id);
  control::HookResponse resp = impl_->execHook(req);
  if (!resp.success()) {
    return 0;
  }
  return atoi(resp.error().c_str());
}
std::string Robot::get_task_state(unsigned int id) {
  control::TaskIndex req;
  req.set_id(id);
  control::Task resp = impl_->loadTask(req);
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
  control::Task resp = impl_->loadTask();
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
  posture::PoseRequest req;
  for (auto &&p : joint_positions) {
    req.mutable_pose()->mutable_joint()->mutable_joint()->push_back(p);
  }
  auto resp = impl_->getForwardKin(req);
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
  posture::GetInverseKinRequest req;
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

  req.mutable_pose()->mutable_cart()->mutable_position()->set_x(x);
  req.mutable_pose()->mutable_cart()->mutable_position()->set_y(y);
  req.mutable_pose()->mutable_cart()->mutable_position()->set_z(z);
  req.mutable_pose()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_z(rz);
  req.mutable_pose()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_y(ry);
  req.mutable_pose()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_x(rx);

  for (auto &&p : joint_init_positions) {
    req.mutable_refer()->mutable_joint()->push_back(p);
  }
  // std::vector<double> joint_positions;
  KinematicsInverseResp ki_resp;
  try {
    auto resp = impl_->getInverseKin(req);
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
  posture::GetPoseTransRequest req;
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

  req.mutable_from()->mutable_cart()->mutable_position()->set_x(x);
  req.mutable_from()->mutable_cart()->mutable_position()->set_y(y);
  req.mutable_from()->mutable_cart()->mutable_position()->set_z(z);
  req.mutable_from()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_z(rz);
  req.mutable_from()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_y(ry);
  req.mutable_from()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_x(rx);

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

  req.mutable_from_to()->mutable_cart()->mutable_position()->set_x(x);
  req.mutable_from_to()->mutable_cart()->mutable_position()->set_y(y);
  req.mutable_from_to()->mutable_cart()->mutable_position()->set_z(z);
  req.mutable_from_to()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_z(rz);
  req.mutable_from_to()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_y(ry);
  req.mutable_from_to()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_x(rx);

  auto resp = impl_->getPoseTrans(req);
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
  posture::PoseRequest req;
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

  req.mutable_pose()->mutable_cart()->mutable_position()->set_x(x);
  req.mutable_pose()->mutable_cart()->mutable_position()->set_y(y);
  req.mutable_pose()->mutable_cart()->mutable_position()->set_z(z);
  req.mutable_pose()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_z(rz);
  req.mutable_pose()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_y(ry);
  req.mutable_pose()
      ->mutable_cart()
      ->mutable_rotation()
      ->mutable_euler_zyx()
      ->set_x(rx);
  auto resp = impl_->getPoseInverse(req);
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
  file::SaveFileRequest req;
  req.set_dir(dir);
  req.set_name(name);
  file::File file;
  file.set_is_dir(is_dir);
  file.set_data(data);
  req.set_file(file);
  impl_->saveFile(req);
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
  file::RenameFileRequest req;
  file::FileIndex from;
  file::FileIndex to;
  from.set_dir(from_dir);
  from.set_name(from_name);
  to.set_dir(to_dir);
  to.set_name(to_name);
  req.set_from(from);
  req.set_to(to);
  impl_->renameFile(req);
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
  file::FileIndex req;
  req.set_dir(dir);
  req.set_name(name);
  file::File resp = impl_->loadFile(req);
  std::tuple<bool, std::string> ret =
      std::make_tuple(resp.is_dir(), resp.data());
  return ret;
}
std::vector<std::tuple<bool, std::string>> Robot::load_file_list(
    const std::string &dir, const std::string &prefix,
    const std::string &suffix) {
  file::LoadFileListRequest req;
  req.set_dir(dir);
  req.set_prefix(prefix);
  req.set_suffix(suffix);
  file::LoadFileListResponse resp = impl_->loadFileList(req);
  std::vector<std::tuple<bool, std::string>> ret;
  for (auto f : resp.files()) {
    std::tuple<bool, std::string> temp = std::make_tuple(f.is_dir(), f.name());
    ret.push_back(temp);
  }
  return ret;
}
void Robot::set_payload(double mass, std::map<std::string, double> cog) {
  dynamic::SetPayloadRequest req;
  req.set_mass(mass);
  posture::Position c;
  c.set_x(cog.at("x"));
  c.set_y(cog.at("y"));
  c.set_z(cog.at("z"));
  req.set_cog(c);
  impl_->setPayload(req);
}
void Robot::set_payload_mass(double mass) {
  dynamic::SetMassRequest req;
  req.set_mass(mass);
  impl_->setPayload(req);
}
void Robot::set_payload_cog(std::map<std::string, double> cog) {
  dynamic::SetCogRequest req;
  posture::Position c;
  c.set_x(cog.at("x"));
  c.set_y(cog.at("y"));
  c.set_z(cog.at("z"));
  req.set_cog(c);
  impl_->setPayload(req);
}

std::map<std::string, double> Robot::get_payload() {
  dynamic::Payload resp = impl_->getPayload();
  std::map<std::string, double> cog;
  cog["x"] = resp.cog().x();
  cog["y"] = resp.cog().y();
  cog["z"] = resp.cog().z();
  cog["mass"] = resp.mass();
  return cog;
}

void Robot::set_gravity(std::map<std::string, double> gravity) {
  posture::Position req;
  req.set_x(gravity.at("x"));
  req.set_y(gravity.at("y"));
  req.set_z(gravity.at("z"));
  impl_->setGravity(req);
}

std::map<std::string, double> Robot::get_gravity() {
  posture::Position resp = impl_->getGravity();
  std::map<std::string, double> gravity;
  gravity["x"] = resp.x();
  gravity["y"] = resp.y();
  gravity["z"] = resp.z();
  return gravity;
}

void Robot::set_tcp(std::array<double, 6> tcp) {
  posture::CartesianPose req;
  posture::Position pos;
  posture::Rotation rot;
  pos.set_x(tcp[0]);
  pos.set_y(tcp[1]);
  pos.set_z(tcp[2]);
  req.set_position(pos);
  posture::Position p;
  p.set_z(tcp[3]);
  p.set_y(tcp[4]);
  p.set_x(tcp[5]);
  rot.set_euler_zyx(p);
  req.set_rotation(rot);
  impl_->setTcp(req);
}
std::array<double, 6> Robot::get_tcp() {
  posture::CartesianPose resp = impl_->getTcp();
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
  kinematic::KinFactor req;
  req.set_factor(factor);
  impl_->setKinFactor(req);
}
int Robot::get_velocity_factor() {
  kinematic::KinFactor resp = impl_->getKinFactor();
  return resp.factor();
}
CartesianPose Robot::load_tcp(std::string name, std::string dir) {
  db::LoadRequest req;
  req.set_name(name);
  req.set_dir(dir);
  const auto &pose = impl_->loadTcp(req);
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

void Robot::write_single_coil(std::string device, std::string addr,
                              bool value) {
  modbus::SetCoilRequest req;
  req.set_device(device);
  req.set_pin(addr);
  req.set_value(value);
  impl_->writeSingleCoil(req);
}

void Robot::wirte_multiple_coils(std::string device, std::string addr,
                                 std::vector<bool> values) {
  modbus::SetCoilsRequest req;
  req.set_device(device);
  req.set_pin(addr);
  req.set_values(values);
  impl_->writeMultipleCoils(req);
}
std::vector<bool> Robot::read_coils(std::string device, std::string addr,
                                    unsigned int num) {
  modbus::GetCoilsRequest req;
  req.set_device(device);
  req.set_pin(addr);
  req.set_count(num);
  modbus::GetCoilsResponse resp = impl_->readCoils(req);
  return resp.values();
}
std::vector<bool> Robot::read_discrete_inputs(std::string device,
                                              std::string addr,
                                              unsigned int num) {
  modbus::GetCoilsRequest req;
  req.set_device(device);
  req.set_pin(addr);
  req.set_count(num);
  modbus::GetCoilsResponse resp = impl_->readDiscreteInputs(req);
  return resp.values();
}
void Robot::write_single_register(std::string device, std::string addr,
                                  unsigned int value) {
  modbus::SetRegisterRequest req;
  req.set_device(device);
  req.set_pin(addr);
  req.set_value(value);
  impl_->writeSingleRegister(req);
}
void Robot::write_multiple_registers(std::string device, std::string addr,
                                     std::vector<unsigned int> values) {
  modbus::SetRegistersRequest req;
  req.set_device(device);
  req.set_pin(addr);
  req.set_values(values);
  impl_->writeMultipleRegisters(req);
}

std::vector<unsigned int> Robot::read_holding_registers(std::string device,
                                                        std::string addr,
                                                        unsigned int num) {
  modbus::GetRegistersRequest req;
  req.set_device(device);
  req.set_pin(addr);
  req.set_count(num);
  modbus::GetRegistersResponse resp = impl_->readHoldingRegisters(req);
  return resp.values();
}
std::vector<unsigned int> Robot::read_input_registers(std::string device,
                                                      std::string addr,
                                                      unsigned int num) {
  modbus::GetRegistersRequest req;
  req.set_device(device);
  req.set_pin(addr);
  req.set_count(num);
  modbus::GetRegistersResponse resp = impl_->readInputRegisters(req);
  return resp.values();
}

void Robot::set_serial_baud_rate(std::string device, unsigned int baud_rate) {
  serial::SetSerialBaudRateRequest req;
  req.set_device(device);
  req.set_baud_rate(baud_rate);
  impl_->setSerialBaudRateRequest(req);
}

void Robot::set_serial_parity(std::string device, unsigned int parity) {
  serial::SetSerialParityRequest req;
  req.set_device(device);
  req.set_parity(static_cast<serial::Parity>(parity));
  impl_->setSerialParityRequest(req);
}

}  // namespace l_master

}  // namespace lebai
