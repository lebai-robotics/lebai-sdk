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

#include "robot_impl.hh"
#include <exception>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "protos/utils.hh"

namespace lebai
{
  namespace l_master {
  Robot::RobotImpl::RobotImpl(const ::std::string &ip, bool simulator)
  {
    uint16_t port = simulator ? 3030 : 3031;
    json_rpc_connector_ = std::make_unique<JSONRpcConnector>(ip, port);
  }
  Robot::RobotImpl::~RobotImpl() {}

  std::tuple<int, std::string> Robot::RobotImpl::call(const std::string & method, const std::string & req_data_str)
  {
    // std::string jsonrpc_req = ToJSONRpcReqString(++jsonrpc_id_, method, params);
    std::string resp_data_str;
    auto ret = json_rpc_connector_->CallRpc(method, req_data_str, &resp_data_str);
    // std::string resp_data;
    // if(ExtractJSONRpcRespString(jsonrpc_resp, resp_id, resp_data) < 0)
    // {
    //   return std::make_tuple(-1, "");
    // }
    return std::make_tuple(ret, resp_data_str);
  }

  int Robot::RobotImpl::startSys()
  {
    json_rpc_connector_->CallRpc("start_sys", "{}", nullptr);
  }

  int Robot::RobotImpl::stopSys()
  {
    json_rpc_connector_->CallRpc( "stop_sys","{}", nullptr);
  }

  int Robot::RobotImpl::powerdown()
  {
    json_rpc_connector_->CallRpc( "powerdown","{}", nullptr);
  }

  int Robot::RobotImpl::stop()
  {
    json_rpc_connector_->CallRpc( "stop","{}",nullptr);
  }

  int Robot::RobotImpl::estop()
  {
    json_rpc_connector_->CallRpc( "estop","{}",nullptr);
  }

  int Robot::RobotImpl::teachMode()
  {
    json_rpc_connector_->CallRpc( "teach_mode","{}",nullptr);
  }

  int Robot::RobotImpl::endTeachMode()
  {
    json_rpc_connector_->CallRpc( "end_teach_mode","{}",nullptr);
  }

  int Robot::RobotImpl::pause()
  {
    json_rpc_connector_->CallRpc( "pause","{}",nullptr);
  }

  int Robot::RobotImpl::resume()
  {
    json_rpc_connector_->CallRpc( "resume","{}",nullptr);
  }

  void Robot::RobotImpl::moveJoint(const MoveRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("move_joint", req.ToJSONString(), &resp);
  }
  void Robot::RobotImpl::moveLinear(const MoveRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("move_linear", req.ToJSONString(), &resp);
  }
  system::RobotState Robot::RobotImpl::getRobotState()
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_robot_state", "{}", &resp);
    system::GetRobotStateResponse get_robot_state_resp;
    get_robot_state_resp.FromJSONString(resp);
    return get_robot_state_resp.state();
  }

  system::PhyData Robot::RobotImpl::getPhyData()
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_phy_data", "{}", &resp);
    // std::cout<<"resp "<<resp<<"\n";
    system::PhyData phy_data;
    phy_data.FromJSONString(resp);
    return phy_data;    
  }

  kinematic::KinData Robot::RobotImpl::getKinData()
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_kin_data", "{}", &resp);    
    // std::cout<<"resp "<<resp<<"\n";
    kinematic::KinData kin_data;
    kin_data.FromJSONString(resp);
    return kin_data;
  }

  io::GetDioPinResponse Robot::RobotImpl::getDI(const io::GetDioPinRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_di", req.ToJSONString(), &resp);
    io::GetDioPinResponse get_dio_resp;
    get_dio_resp.FromJSONString(resp);
    return get_dio_resp;    
  }
  io::GetDioPinResponse Robot::RobotImpl::getDO(const io::GetDioPinRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_do", req.ToJSONString(), &resp);
    io::GetDioPinResponse get_dio_resp;
    get_dio_resp.FromJSONString(resp);
    return get_dio_resp;    
  }

  void Robot::RobotImpl::setDO(const io::SetDoPinRequest & req)
  {
    json_rpc_connector_->CallRpc("set_do", req.ToJSONString(), nullptr);
    return;
  }

  io::GetAioPinResponse Robot::RobotImpl::getAI(const io::GetAioPinRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_ai", req.ToJSONString(), &resp);
    io::GetAioPinResponse get_aio_resp;
    get_aio_resp.FromJSONString(resp);
    return get_aio_resp;
  }

  void Robot::RobotImpl::setAO(const io::SetAoPinRequest & req)
  {
    json_rpc_connector_->CallRpc("set_ao", req.ToJSONString(), nullptr);
    return;
  }

  void Robot::RobotImpl::setClaw(const claw::SetClawRequest & req)
  {
    json_rpc_connector_->CallRpc("set_claw", req.ToJSONString(), nullptr);
    return;
  }
  claw::Claw Robot::RobotImpl::getClaw()
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_claw", "{}", &resp);
    claw::Claw get_claw_resp;
    get_claw_resp.FromJSONString(resp);
    return get_claw_resp;
  }

  void Robot::RobotImpl::setLed(const led::LedData & req)
  {
    json_rpc_connector_->CallRpc("set_led",req.ToJSONString(),nullptr);
  }

  void Robot::RobotImpl::setVoice(const led::VoiceData & req)
  {
    json_rpc_connector_->CallRpc("set_voice",req.ToJSONString(),nullptr);
  }

  void Robot::RobotImpl::setFan(const led::FanData & req)
  {
    json_rpc_connector_->CallRpc("set_fan",req.ToJSONString(),nullptr);
  }

  void Robot::RobotImpl::setSignal(const signal::SetSignalRequest & req)
  {
    json_rpc_connector_->CallRpc("set_signal",req.ToJSONString(),nullptr);
  }
  signal::GetSignalResponse Robot::RobotImpl::getSignal(const signal::GetSignalRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_signal",req.ToJSONString(),&resp);
    signal::GetSignalResponse set_signal_resp;
    set_signal_resp.FromJSONString(resp);
    return set_signal_resp;
  }
  void Robot::RobotImpl::addSignal(const signal::SetSignalRequest & req)
  {
    json_rpc_connector_->CallRpc("add_signal",req.ToJSONString(),nullptr);
  }

  control::TaskIndex Robot::RobotImpl::scene(const control::StartTaskRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("scene",req.ToJSONString(),&resp);
    control::TaskIndex start_task_resp;
    start_task_resp.FromJSONString(resp);
    return start_task_resp;
  }

  posture::CartesianPose Robot::RobotImpl::getForwardKin(const posture::PoseRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_forward_kin", req.ToJSONString(), &resp);
    posture::CartesianPose get_forward_kin_resp;
    get_forward_kin_resp.FromJSONString(resp);
    return get_forward_kin_resp;
  }

  posture::JointPose Robot::RobotImpl::getInverseKin(
    const posture::GetInverseKinRequest & req)
  {
    std::string resp;
    if(json_rpc_connector_->CallRpc("get_inverse_kin", req.ToJSONString(), &resp) < 0)
    {
      throw std::runtime_error("get_inverse_kin failed");
    }
    posture::JointPose get_inverse_kin_resp;
    get_inverse_kin_resp.FromJSONString(resp);
    return get_inverse_kin_resp;
  }

  posture::CartesianPose Robot::RobotImpl::getPoseMultiply(const posture::GetPoseMultiplyRequest & req)
  {
    std::string resp_str;
    json_rpc_connector_->CallRpc("get_pose_multiply", req.ToJSONString(), &resp_str);
    posture::CartesianPose resp;    
    resp.FromJSONString(resp_str);
    return resp;
  }
  posture::CartesianPose Robot::RobotImpl::getPoseInverse(const posture::PoseRequest & req)
  {
    std::string resp_str;
    json_rpc_connector_->CallRpc("get_pose_inverse", req.ToJSONString(), &resp_str);
    posture::CartesianPose resp;    
    resp.FromJSONString(resp_str);
    return resp;
  }

  }

}