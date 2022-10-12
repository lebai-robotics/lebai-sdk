/**
 * Copyright 2002 lebai.ltd
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


namespace lebai {

  namespace l_master
  {

  

Robot::Robot(std::string ip, bool simulator)
// :ip_(ip)
{
  impl_ = std::make_unique<RobotImpl>(ip, simulator);
}
Robot::~Robot() {}

std::tuple<int, std::string> Robot::call(const std::string & method, const std::string & params)
{
  return impl_->call(method, params);
}

void Robot::start_sys()
{
  impl_->startSys();
}

void Robot::stop_sys()
{
  impl_->stopSys();
}

bool Robot::movej(const std::map<std::string, double> & joint_positions, double a, double v, double t, double r)
{
  MoveRequest move_req;
  move_req.mutable_param().set_acc(a);
  move_req.mutable_param().set_velocity(v);
  move_req.mutable_param().set_time(t);
  move_req.mutable_param().set_radius(r);
  if(joint_positions.find("j1") != joint_positions.end() && 
  joint_positions.find("j2") != joint_positions.end() &&
  joint_positions.find("j3") != joint_positions.end() &&
  joint_positions.find("j4") != joint_positions.end() &&
  joint_positions.find("j5") != joint_positions.end() &&
  joint_positions.find("j6") != joint_positions.end())
  {
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j1"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j2"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j3"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j4"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j5"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j6"));
    impl_->moveJoint(move_req);
  }
  else
  {
    return false;
  }
  return true;
}

bool Robot::movej(const CartesianPose & cart_pose, double a, double v, double t, double r)
{
  MoveRequest move_req;
  move_req.mutable_param().set_acc(a);
  move_req.mutable_param().set_velocity(v);
  move_req.mutable_param().set_time(t);
  move_req.mutable_param().set_radius(r);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_position()->set_x(cart_pose[0]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_position()->set_y(cart_pose[1]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_position()->set_z(cart_pose[2]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_z(cart_pose[3]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_y(cart_pose[4]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_x(cart_pose[5]);
  impl_->moveJoint(move_req);
  return true;
}


bool Robot::movel(const std::map<std::string, double> & joint_positions, double a, double v, double t, double r)
{
  MoveRequest move_req;
  move_req.mutable_param().set_acc(a);
  move_req.mutable_param().set_velocity(v);
  move_req.mutable_param().set_time(t);
  move_req.mutable_param().set_radius(r);
  if(joint_positions.find("j1") != joint_positions.end() && 
  joint_positions.find("j2") != joint_positions.end() &&
  joint_positions.find("j3") != joint_positions.end() &&
  joint_positions.find("j4") != joint_positions.end() &&
  joint_positions.find("j5") != joint_positions.end() &&
  joint_positions.find("j6") != joint_positions.end())
  {
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j1"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j2"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j3"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j4"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j5"));
    move_req.mutable_pose().mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j6"));
    impl_->moveLinear(move_req);
  }
  else
  {
    return false;
  }
  return true;
}

bool Robot::movel(const CartesianPose & cart_pose, double a, double v, double t, double r)
{
  MoveRequest move_req;
  move_req.mutable_param().set_acc(a);
  move_req.mutable_param().set_velocity(v);
  move_req.mutable_param().set_time(t);
  move_req.mutable_param().set_radius(r);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_position()->set_x(cart_pose[0]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_position()->set_y(cart_pose[1]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_position()->set_z(cart_pose[2]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_z(cart_pose[3]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_y(cart_pose[4]);
  move_req.mutable_pose().mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_x(cart_pose[5]);
  impl_->moveLinear(move_req);
  return true;
}
int Robot::get_robot_mode()
{
  return impl_->getRobotState();
}

std::vector<double> Robot::get_actual_joint_positions()
{
  return *impl_->getKinData().mutable_actual_joint_pose();
}

std::vector<double> Robot::get_target_joint_positions()
{
  return *impl_->getKinData().mutable_target_joint_pose();
}
std::vector<double> Robot::get_actual_joint_speed()
{
  return *impl_->getKinData().mutable_actual_joint_speed();
}
std::vector<double> Robot::get_target_joint_speed()
{
    return *impl_->getKinData().mutable_target_joint_speed();
}

Robot::CartesianPose Robot::get_actual_tcp_pose()
{
  const auto & pose = impl_->getKinData().actual_tcp_pose();
  // pose.position()
  CartesianPose cart_pose;
  cart_pose[0] = pose.position().x();
  cart_pose[1] = pose.position().y();
  cart_pose[2] = pose.position().z();
  if(pose.rotation().euler_zyx())
  {
    cart_pose[3] = pose.rotation().euler_zyx()->z();
    cart_pose[4] = pose.rotation().euler_zyx()->y();
    cart_pose[5] = pose.rotation().euler_zyx()->x();
  }
  return cart_pose;
}
Robot::CartesianPose Robot::get_target_tcp_pose()
{
  const auto & pose = impl_->getKinData().target_tcp_pose();
  // pose.position()
  CartesianPose cart_pose;
  cart_pose[0] = pose.position().x();
  cart_pose[1] = pose.position().y();
  cart_pose[2] = pose.position().z();
  if(pose.rotation().euler_zyx())
  {
    cart_pose[3] = pose.rotation().euler_zyx()->z();
    cart_pose[4] = pose.rotation().euler_zyx()->y();
    cart_pose[5] = pose.rotation().euler_zyx()->x();
  }
  return cart_pose;
}

double Robot::get_joint_temp(unsigned int joint_index)
{
  auto data = impl_->getPhyData();  
  if(data.joint_temp().size() > joint_index)
  {
    return data.joint_temp()[joint_index];
  }
  return 0.0;
}

std::vector<double> Robot::get_actual_joint_torques()
{
  return impl_->getKinData().actual_joint_torque();  
}

std::vector<double> Robot::get_target_joint_torques()
{
  return impl_->getKinData().target_joint_torque();  
}

bool Robot::get_di(unsigned int pin)
{
  io::GetDioPinRequest req;
  req.set_pin(pin);
  req.set_device(io::IoDevice::ROBOT);
  auto resp = impl_->getDI(req);
  return resp.value() ? true : false;
}

void Robot::set_do(unsigned int pin, bool value)
{
  io::SetDoPinRequest req;
  req.set_pin(pin);
  req.set_value(value);
  req.set_device(io::IoDevice::ROBOT);
  impl_->setDO(req);
}

double Robot::get_ai(unsigned int pin)
{
  io::GetAioPinRequest req;
  req.set_pin(pin);
  req.set_device(io::IoDevice::ROBOT);
  auto resp = impl_->getAI(req);
  return resp.value();
}

void Robot::set_ao(unsigned int pin, double value)
{
  io::SetAoPinRequest req;
  req.set_pin(pin);
  req.set_value(value);
  req.set_device(io::IoDevice::ROBOT);
  impl_->setAO(req);
}


bool Robot::get_flange_di(unsigned int pin)
{
  io::GetDioPinRequest req;
  req.set_pin(pin);
  req.set_device(io::IoDevice::FLANGE);
  auto resp = impl_->getDI(req);
  return resp.value() ? true : false;
}

void Robot::set_flange_do(unsigned int pin, bool value)
{
  io::SetDoPinRequest req;
  req.set_pin(pin);
  req.set_value(value);
  req.set_device(io::IoDevice::FLANGE);
  impl_->setDO(req);
}

bool Robot::get_extra_di(unsigned int pin)
{
  io::GetDioPinRequest req;
  req.set_pin(pin);
  req.set_device(io::IoDevice::EXTRA);
  auto resp = impl_->getDI(req);
  return resp.value() ? true : false;
}

void Robot::set_extra_do(unsigned int pin, bool value)
{
  io::SetDoPinRequest req;
  req.set_pin(pin);
  req.set_value(value);
  req.set_device(io::IoDevice::EXTRA);
  impl_->setDO(req);
}

double Robot::get_extra_ai(unsigned int pin)
{
  io::GetAioPinRequest req;
  req.set_pin(pin);
  req.set_device(io::IoDevice::EXTRA);
  auto resp = impl_->getAI(req);
  return resp.value();
}

void Robot::set_extra_ao(unsigned int pin, double value)
{
  io::SetAoPinRequest req;
  req.set_pin(pin);
  req.set_value(value);
  req.set_device(io::IoDevice::EXTRA);
  impl_->setAO(req);
}

void Robot::set_claw(double force, double amplitude)
{
  claw::SetClawRequest req;
  req.set_force(force);
  req.set_amplitude(amplitude);
  impl_->setClaw(req);  
}

std::tuple<double, double ,bool> Robot::get_claw()
{
  auto resp = impl_->getClaw();
  return std::make_tuple(resp.force(), resp.amplitude(), resp.hold_on());
}

std::tuple<std::array<double, 6>, bool> Robot::kinematics_forward(const std::map<std::string, double> & joint_positions)
{
  posture::PoseRequest req;
  if(joint_positions.find("j1") != joint_positions.end() && 
  joint_positions.find("j2") != joint_positions.end() &&
  joint_positions.find("j3") != joint_positions.end() &&
  joint_positions.find("j4") != joint_positions.end() &&
  joint_positions.find("j5") != joint_positions.end() &&
  joint_positions.find("j6") != joint_positions.end())
  {
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j1"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j2"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j3"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j4"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j5"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_positions.at("j6"));
  }
  else
  {
    return std::tuple<std::array<double, 6>, bool>({0.0,0.0,0.0,0.0,0.0,0.0}, false);
  }
  auto resp = impl_->getForwardKin(req);
  std::array<double, 6> pose;
  pose[0] = resp.position().x();
  pose[1] = resp.position().y();
  pose[2] = resp.position().z();
  pose[3] = resp.rotation().euler_zyx()->z();
  pose[4] = resp.rotation().euler_zyx()->y();
  pose[5] = resp.rotation().euler_zyx()->x();
  return std::tuple<std::array<double, 6>, bool>(pose, true);
}

std::tuple<std::map<std::string, double>, bool> Robot::kinematics_inverse(const std::array<double, 6> & pose, const std::vector<double> & joint_init_positions)
{
  posture::GetInverseKinRequest req;
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_x(pose[0]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_y(pose[1]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_z(pose[2]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_z(pose[3]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_y(pose[4]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_x(pose[5]);
  if(joint_init_positions.size() == 6)
  {
    req.mutable_refer()->mutable_joint()->push_back(joint_init_positions[0]);
    req.mutable_refer()->mutable_joint()->push_back(joint_init_positions[1]);
    req.mutable_refer()->mutable_joint()->push_back(joint_init_positions[2]);
    req.mutable_refer()->mutable_joint()->push_back(joint_init_positions[3]);
    req.mutable_refer()->mutable_joint()->push_back(joint_init_positions[4]);
    req.mutable_refer()->mutable_joint()->push_back(joint_init_positions[5]);
  }
  std::map<std::string, double> joint_positions;
  try
  {
    auto resp = impl_->getInverseKin(req);
    if(resp.joint().size() == 6)
    {
      joint_positions["j1"] = resp.joint()[0];
      joint_positions["j2"] = resp.joint()[1];
      joint_positions["j3"] = resp.joint()[2];
      joint_positions["j4"] = resp.joint()[3];
      joint_positions["j5"] = resp.joint()[4];
      joint_positions["j6"] = resp.joint()[5];
      return std::tuple<std::map<std::string, double>, bool>(joint_positions, true);
    }
    else
    {
      return std::tuple<std::map<std::string, double>, bool>(joint_positions, false);
    }    
  }
  catch(std::exception & e)
  {
    return std::tuple<std::map<std::string, double>, bool>(joint_positions, false);
  }

}

std::array<double, 6> Robot::pose_times(const std::array<double, 6> & a, const std::array<double, 6> & b)
{
  posture::GetPoseMultiplyRequest req;
  req.mutable_base()->mutable_cart()->mutable_delta()->mutable_position()->set_x(a[0]);
  req.mutable_base()->mutable_cart()->mutable_delta()->mutable_position()->set_y(a[1]);
  req.mutable_base()->mutable_cart()->mutable_delta()->mutable_position()->set_z(a[2]);
  req.mutable_base()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_z(a[3]);
  req.mutable_base()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_y(a[4]);
  req.mutable_base()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_x(a[5]);

  req.mutable_target()->mutable_cart()->mutable_delta()->mutable_position()->set_x(b[0]);
  req.mutable_target()->mutable_cart()->mutable_delta()->mutable_position()->set_y(b[1]);
  req.mutable_target()->mutable_cart()->mutable_delta()->mutable_position()->set_z(b[2]);
  req.mutable_target()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_z(b[3]);
  req.mutable_target()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_y(b[4]);
  req.mutable_target()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_x(b[5]);

  auto resp = impl_->getPoseMultiply(req);
  std::array<double, 6> pose;
  pose[0] = resp.position().x();
  pose[1] = resp.position().y();
  pose[2] = resp.position().z();
  pose[3] = resp.rotation().euler_zyx()->z();
  pose[4] = resp.rotation().euler_zyx()->y();
  pose[5] = resp.rotation().euler_zyx()->x();
  return pose;
}

std::array<double, 6> Robot::pose_inverse(const std::array<double, 6> & in)
{
  posture::PoseRequest req;
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_x(in[0]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_y(in[1]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_z(in[2]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_z(in[3]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_y(in[4]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_x(in[5]);
  auto resp = impl_->getPoseInverse(req);
  std::array<double, 6> pose;
  pose[0] = resp.position().x();
  pose[1] = resp.position().y();
  pose[2] = resp.position().z();
  pose[3] = resp.rotation().euler_zyx()->z();
  pose[4] = resp.rotation().euler_zyx()->y();
  pose[5] = resp.rotation().euler_zyx()->x();
  return pose;
}

}

}  // namespace l_master_sdk