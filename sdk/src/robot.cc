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

void Robot::powerdown()
{
  impl_->powerdown();
}

void Robot::stop()
{
  impl_->stop();
}

void Robot::estop()
{
  impl_->estop();
}

void Robot::teach_mode()
{
  impl_->teachMode();
}

void Robot::end_teach_mode()
{
  impl_->endTeachMode();
}

void Robot::pause()
{
  impl_->pause();
}

void Robot::resume()
{
  impl_->resume();
}

int Robot::movej(const std::map<std::string, double> & joint_positions, double a, double v, double t, double r)
{
  MoveRequest move_req;
  move_req.mutable_param().set_acc(a);
  move_req.mutable_param().set_velocity(v);
  move_req.mutable_param().set_time(t);
  move_req.mutable_param().set_radius(r);
  MotionIndex resp;
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
    resp = impl_->moveJoint(move_req);
  }
  else
  {
    return -1;
  }
  return resp.id();
}

int Robot::movej(const CartesianPose & cart_pose, double a, double v, double t, double r)
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
  MotionIndex resp = impl_->moveJoint(move_req);
  return resp.id();
}


int Robot::movel(const std::map<std::string, double> & joint_positions, double a, double v, double t, double r)
{
  MoveRequest move_req;
  move_req.mutable_param().set_acc(a);
  move_req.mutable_param().set_velocity(v);
  move_req.mutable_param().set_time(t);
  move_req.mutable_param().set_radius(r);
  MotionIndex resp;
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
    resp = impl_->moveLinear(move_req);
  }
  else
  {
    return -1;
  }
  return resp.id();
}

int Robot::movel(const CartesianPose & cart_pose, double a, double v, double t, double r)
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
  MotionIndex resp = impl_->moveLinear(move_req);
  return resp.id();
}

void Robot::move_pvat(std::vector<double> p, std::vector<double> v, std::vector<double> a, double t)
{
  MovePvatRequest req;
  std::vector<JointMove> joints;
  for(int i = 0;i < p.size();i++)
  {
    JointMove joint;
    joint.set_pose(p[i]);
    joint.set_velocity(v[i]);
    joint.set_acc(a[i]);
    joints.push_back(joint);
  }
  req.set_duration(t);
  req.set_joints(joints);
  impl_->movePvat(req);
}

void Robot::wait_move(unsigned int id)
{
  MotionIndex req;
  req.set_id(id);
  impl_->waitMove(req);
}
void Robot::wait_move()
{
  MotionIndex req;
  req.set_id(0);
  impl_->waitMove(req);
}

unsigned int Robot::get_running_motion()
{
  MotionIndex resp = impl_->getRunningMotion();
  return resp.id();
}

std::string Robot::get_motion_state(unsigned int id)
{
  MotionIndex req;
  req.set_id(id);
  GetMotionStateResponse resp = impl_->getMotionState(req);
  switch(resp.state())
  {
    case WAIT:return (std::string)"WAIT";
    case RUNNING:return (std::string)"RUNNING";
    case FINISHED:return (std::string)"FINISHED";
    default:return (std::string)"WAIT";
  }
}

void Robot::stop_move()
{
  impl_->stopMove();
}

int Robot::get_robot_mode()
{
  return impl_->getRobotState();
}

std::map<std::string, double> Robot::get_actual_joint_positions()
{
  std::map<std::string, double> ret;
  auto joint_positions = *impl_->getKinData().mutable_actual_joint_pose();
  if(joint_positions.size() == 6)
  {
    ret["j1"] = joint_positions[0];
    ret["j2"] = joint_positions[1];
    ret["j3"] = joint_positions[2];
    ret["j4"] = joint_positions[3];
    ret["j5"] = joint_positions[4];
    ret["j6"] = joint_positions[5];
  }
  return ret;
}

std::map<std::string, double> Robot::get_target_joint_positions()
{
  std::map<std::string, double> ret;
  auto joint_positions = *impl_->getKinData().mutable_target_joint_pose();
  // size = 0
  if(joint_positions.size() == 6)
  {
    ret["j1"] = joint_positions[0];
    ret["j2"] = joint_positions[1];
    ret["j3"] = joint_positions[2];
    ret["j4"] = joint_positions[3];
    ret["j5"] = joint_positions[4];
    ret["j6"] = joint_positions[5];
  }
  return ret;
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
  joint_index -= 1; 
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

void Robot::set_do(unsigned int pin, unsigned int value)
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
void Robot::set_flange_do(unsigned int pin, unsigned int value)
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
void Robot::set_extra_do(unsigned int pin, unsigned int value)
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

void Robot::set_led(unsigned int mode,unsigned int speed,const std::vector<unsigned int> & color)
{
  led::LedData req;
  switch(mode)
  {
    case 0:req.set_mode(led::LedMode::HOLD_LED);break;
    case 1:req.set_mode(led::LedMode::CLOSE_LED);break;
    case 2:req.set_mode(led::LedMode::OPEN_LED);break;
    case 3:req.set_mode(led::LedMode::BREATH);break;
    case 4:req.set_mode(led::LedMode::FOUR);break;
    case 5:req.set_mode(led::LedMode::WATER);break;
    case 6:req.set_mode(led::LedMode::BLINK);break;
    default:return;
  }
  switch(speed)
  {
    case 0:req.set_speed(led::LedSpeed::HOLD_LED_SPEED);break;
    case 1:req.set_speed(led::LedSpeed::FAST);break;
    case 2:req.set_speed(led::LedSpeed::NORMAL);break;
    case 3:req.set_speed(led::LedSpeed::SLOW);break;
    default:return;
  }
  std::vector<led::LedColor> _color;
  for(auto _c:color){
    switch(_c)
    {
      case 0:_color.push_back(led::LedColor::RED);break;
      case 1:_color.push_back(led::LedColor::GREEN);break;
      case 2:_color.push_back(led::LedColor::BLUE);break;
      case 3:_color.push_back(led::LedColor::PINK);break;
      case 4:_color.push_back(led::LedColor::YELLOW);break;
      case 5:_color.push_back(led::LedColor::CYAN);break;
      case 6:_color.push_back(led::LedColor::GRAY);break;
      case 7:_color.push_back(led::LedColor::BROWN);break;
      case 8:_color.push_back(led::LedColor::ORANGE);break;
      case 9:_color.push_back(led::LedColor::GOLD);break;
      case 10:_color.push_back(led::LedColor::INDIGO);break;
      case 11:_color.push_back(led::LedColor::LIGHT_SKY_BLUE);break;
      case 12:_color.push_back(led::LedColor::DARK_VIOLET);break;
      case 13:_color.push_back(led::LedColor::CHOCOLATE);break;
      case 14:_color.push_back(led::LedColor::LIGHT_RED);break;
      case 15:_color.push_back(led::LedColor::WHITE);break;
      default:return;
    }
  }
  req.set_colors(_color);
  impl_->setLed(req);
}

void Robot::set_voice(unsigned int voice,unsigned int volume)
{
  led::VoiceData req;
  switch(voice)
  {
    case 0:req.set_voice(led::VoiceKind::OFF);break;
    case 1:req.set_voice(led::VoiceKind::BOOTING);break;
    case 2:req.set_voice(led::VoiceKind::STOPING);break;
    case 3:req.set_voice(led::VoiceKind::COLLISION_DETECTED);break;
    case 4:req.set_voice(led::VoiceKind::UPGRADE);break;
    case 5:req.set_voice(led::VoiceKind::TEACH_MODE_ON);break;
    case 6:req.set_voice(led::VoiceKind::TEACH_MODE_OFF);break;
    case 7:req.set_voice(led::VoiceKind::FINE_TUNNING_ON);break;
    case 8:req.set_voice(led::VoiceKind::FINE_TUNNING_OFF);break;
    case 9:req.set_voice(led::VoiceKind::FINE_TUNNING_CHANGE);break;
    case 10:req.set_voice(led::VoiceKind::BORING);break;
    case 11:req.set_voice(led::VoiceKind::CUSTOM1);break;
    case 12:req.set_voice(led::VoiceKind::CUSTOM2);break;
    case 13:req.set_voice(led::VoiceKind::CUSTOM3);break;
    case 14:req.set_voice(led::VoiceKind::CUSTOM4);break;
    case 15:req.set_voice(led::VoiceKind::CUSTOM5);break;
    default:return;
  }
  switch(volume)
  {
    case 0:req.set_volume(led::Volume::MUTE);break;
    case 1:req.set_volume(led::Volume::LOW);break;
    case 2:req.set_volume(led::Volume::MID);break;
    case 3:req.set_volume(led::Volume::HIGH);break;
    default:return;
  }
  impl_->setVoice(req);
}

void Robot::set_fan(unsigned int status)
{
  led::FanData req;
  switch(status)
  {
    case 0:req.set_fan(led::FanMode::HOLD_FAN);break;
    case 1:req.set_fan(led::FanMode::CLOSE_FAN);break;
    case 2:req.set_fan(led::FanMode::OPEN_FAN);break;
    default:return;
  }
  impl_->setFan(req);
}

void Robot::set_signal(unsigned int index,int value)
{
  signal::SetSignalRequest req;
  req.set_key(index);
  req.set_value(value);
  impl_->setSignal(req);
}
int Robot::get_signal(unsigned int index)
{
  signal::GetSignalRequest req;
  req.set_key(index);
  signal::GetSignalResponse resp = impl_->getSignal(req);
  return resp.value();
}
void Robot::add_signal(unsigned int index,int value)
{
  signal::SetSignalRequest req;
  req.set_key(index);
  req.set_value(value);
  impl_->addSignal(req);
}

unsigned int Robot::scene(const std::string &name,bool is_parallel,unsigned int loop_to,const std::string & dir,const std::vector<std::string> & params)
{
  control::StartTaskRequest req;
  req.set_name(name);
  req.set_is_parallel(is_parallel);
  req.set_loop_to(loop_to);
  req.set_dir(dir);
  req.set_params(params);
  control::TaskIndex resp = impl_->scene(req);
  return resp.id();
}
unsigned int Robot::scene(const std::string &name,bool is_parallel,unsigned int loop_to,const std::string & dir)
{
  control::StartTaskRequest req;
  req.set_name(name);
  req.set_is_parallel(is_parallel);
  req.set_loop_to(loop_to);
  req.set_dir(dir);
  control::TaskIndex resp = impl_->scene(req);
  return resp.id();
}
unsigned int Robot::scene(const std::string &name)
{
  control::StartTaskRequest req;
  req.set_name(name);
  req.set_is_parallel(false);
  req.set_loop_to(1);
  control::TaskIndex resp = impl_->scene(req);
  return resp.id();
}
std::vector<unsigned int> Robot::load_task_list()
{
  control::TaskIds resp = impl_->loadTaskList();
  return resp.ids();
}
void Robot::pause_task(unsigned int id,unsigned long time,bool wait)
{
  control::PauseRequest req;
  req.set_id(id);
  req.set_time(time);
  req.set_wait(wait);
  impl_->pauseTask(req);
}
void Robot::resume_task(unsigned int id)
{
  control::TaskIndex req;
  req.set_id(id);
  impl_->resumeTask(req);
}
void Robot::cancel_task(unsigned int id)
{
  control::TaskIndex req;
  req.set_id(id);
  impl_->cancelTask(req);
}


KinematicsForwardResp Robot::kinematics_forward(const std::map<std::string, double> & joint_positions)
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
    return KinematicsForwardResp();
  }
  auto resp = impl_->getForwardKin(req);
  KinematicsForwardResp kf_resp;
  kf_resp.pose[0] = resp.position().x();
  kf_resp.pose[1] = resp.position().y();
  kf_resp.pose[2] = resp.position().z();
  kf_resp.pose[3] = resp.rotation().euler_zyx()->z();
  kf_resp.pose[4] = resp.rotation().euler_zyx()->y();
  kf_resp.pose[5] = resp.rotation().euler_zyx()->x();
  kf_resp.ok = true;
  return kf_resp;
}

KinematicsInverseResp Robot::kinematics_inverse(const std::array<double, 6> & pose, const std::map<std::string, double> & joint_init_positions)
{
  posture::GetInverseKinRequest req;
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_x(pose[0]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_y(pose[1]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_position()->set_z(pose[2]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_z(pose[3]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_y(pose[4]);
  req.mutable_pose()->mutable_cart()->mutable_delta()->mutable_rotation()->mutable_euler_zyx()->set_x(pose[5]);
  if(joint_init_positions.size() == 6 && 
  joint_init_positions.find("j1") != joint_init_positions.end() &&
  joint_init_positions.find("j2") != joint_init_positions.end() &&
  joint_init_positions.find("j3") != joint_init_positions.end() &&
  joint_init_positions.find("j4") != joint_init_positions.end() &&
  joint_init_positions.find("j5") != joint_init_positions.end() &&
  joint_init_positions.find("j6") != joint_init_positions.end())
  {
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_init_positions.at("j1"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_init_positions.at("j2"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_init_positions.at("j3"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_init_positions.at("j4"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_init_positions.at("j5"));
    req.mutable_pose()->mutable_joint()->mutable_delta()->mutable_joint()->push_back(joint_init_positions.at("j6"));
  }  
  std::map<std::string, double> joint_positions;
  try
  {
    auto resp = impl_->getInverseKin(req);
    if(resp.joint().size() == 6)
    {
      KinematicsInverseResp ki_resp;
      ki_resp.joint_positions["j1"] = resp.joint()[0];
      ki_resp.joint_positions["j2"] = resp.joint()[1];
      ki_resp.joint_positions["j3"] = resp.joint()[2];
      ki_resp.joint_positions["j4"] = resp.joint()[3];
      ki_resp.joint_positions["j5"] = resp.joint()[4];
      ki_resp.joint_positions["j6"] = resp.joint()[5];
      ki_resp.ok = true;
      return ki_resp;
    }
    else
    {
      return KinematicsInverseResp();
    }    
  }
  catch(std::exception & e)
  {
    return KinematicsInverseResp();
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

void Robot::save_file(const std::string &dir,const std::string &name,bool is_dir,const std::string & data)
{
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
void Robot::save_file(const std::string &dir,const std::string & name,file::File file)
{
  file::SaveFileRequest req;
  req.set_dir(dir);
  req.set_name(name);
  req.set_file(file);
  impl_->saveFile(req);
}
*/


void Robot::rename_file(const std::string &from_dir,const std::string & from_name,const std::string & to_dir,const std::string & to_name)
{
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
std::tuple<bool,std::string> Robot::load_file(const std::string &dir,const std::string & name)
{
  file::FileIndex req;
  req.set_dir(dir);
  req.set_name(name);
  file::File resp = impl_->loadFile(req);
  std::tuple<bool,std::string> ret = std::make_tuple(resp.is_dir(),resp.data());
  return ret;
}
std::vector<std::tuple<bool,std::string>> Robot::load_file_list(const std::string &dir,const std::string & prefix,const std::string & suffix)
{
  file::LoadFileListRequest req;
  req.set_dir(dir);
  req.set_prefix(prefix);
  req.set_suffix(suffix);
  file::LoadFileListResponse resp = impl_->loadFileList(req);
  std::vector<std::tuple<bool,std::string>> ret;
  for(auto f:resp.files())
  {
    std::tuple<bool,std::string> temp = std::make_tuple(f.is_dir(),f.name());
    ret.push_back(temp);
  }
  return ret;
}
void Robot::set_payload(double mass, std::map<std::string, double> cog)
{
  dynamic::SetPayloadRequest req;
  req.set_mass(mass);
  std::vector<double> c;
  c.push_back(cog.at("x"));
  c.push_back(cog.at("y"));
  c.push_back(cog.at("z"));
  req.set_cog(c);
  impl_->setPayload(req);
}

std::tuple<double, std::map<std::string, double>> Robot::get_payload()
{
  dynamic::Payload resp = impl_->getPayload();
  std::map<std::string,double> cog;
  cog["x"] = resp.cog()[0];
  cog["y"] = resp.cog()[1];
  cog["Z"] = resp.cog()[2];
  std::tuple<double, std::map<std::string, double>> ret = std::make_tuple(resp.mass(),cog);
  return ret;
}

void Robot::set_gravity(std::map<std::string,double> gravity)
{
  posture::Position req;
  req.set_x(gravity.at("x"));
  req.set_y(gravity.at("y"));
  req.set_z(gravity.at("z"));
  impl_->setGravity(req);
}

std::map<std::string,double> Robot::get_gravity()
{
  posture::Position resp = impl_->getGravity();
  std::map<std::string,double> gravity;
  gravity["x"] = resp.x();
  gravity["y"] = resp.y();
  gravity["z"] = resp.z();
  return gravity;
}

void Robot::set_tcp(std::array<double, 6> tcp)
{
  posture::CartesianPose req;
  posture::Position pos;
  posture::Rotation rot;
  pos.set_x(tcp[0]);
  pos.set_y(tcp[1]);
  pos.set_z(tcp[2]);
  req.set_position(pos);
  posture::Position p;
  p.set_x(tcp[3]);
  p.set_y(tcp[4]);
  p.set_z(tcp[5]);
  rot.set_euler_zyx(p);
  req.set_rotation(rot);
  impl_->setTcp(req);
}
std::array<double, 6> Robot::get_tcp()
{
  posture::CartesianPose resp = impl_->getTcp();
  std::array<double, 6> ret;
  ret[0] = resp.position().x();
  ret[1] = resp.position().y();
  ret[2] = resp.position().z();
  ret[3] = resp.rotation().euler_zyx()->x();
  ret[4] = resp.rotation().euler_zyx()->y();
  ret[5] = resp.rotation().euler_zyx()->z();
  return ret;
}

void Robot::set_velocity_factor(int factor)
{
  kinematic::KinFactor req;
  req.set_factor(factor);
  impl_->setKinFactor(req);
}
int Robot::get_velocity_factor()
{
  kinematic::KinFactor resp = impl_->getKinFactor();
  return resp.factor();
}

}

}  // namespace l_master_sdk