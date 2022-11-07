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
    unsigned int i = 0;
    unsigned int count = timeout_ / 0.1;
    while(i < count)
    {
      if(json_rpc_connector_->GetConnectionStatus() == JSONRpcConnector::kOpen)
      {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      i++;
    }
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
    return json_rpc_connector_->CallRpc("start_sys", "{}", nullptr);
  }

  int Robot::RobotImpl::stopSys()
  {
    return json_rpc_connector_->CallRpc( "stop_sys","{}", nullptr);
  }

  int Robot::RobotImpl::powerdown()
  {
    return json_rpc_connector_->CallRpc( "powerdown","{}", nullptr);
  }

  int Robot::RobotImpl::stop()
  {
    return json_rpc_connector_->CallRpc( "stop","{}",nullptr);
  }

  int Robot::RobotImpl::estop()
  {
    return json_rpc_connector_->CallRpc( "estop","{}",nullptr);
  }

  int Robot::RobotImpl::teachMode()
  {
    return json_rpc_connector_->CallRpc( "start_teach_mode","{}",nullptr);
  }

  int Robot::RobotImpl::endTeachMode()
  {
    return json_rpc_connector_->CallRpc( "end_teach_mode","{}",nullptr);
  }

  int Robot::RobotImpl::pause()
  {
    return json_rpc_connector_->CallRpc( "pause","{}",nullptr);
  }

  int Robot::RobotImpl::resume()
  {
    return json_rpc_connector_->CallRpc( "resume","{}",nullptr);
  }

  MotionIndex Robot::RobotImpl::moveJoint(const MoveRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("move_joint", req.ToJSONString(), &resp);
    MotionIndex motion_resp;
    motion_resp.FromJSONString(resp);
    return motion_resp;
  }
  MotionIndex Robot::RobotImpl::moveLinear(const MoveRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("move_linear", req.ToJSONString(), &resp);
    MotionIndex motion_resp;
    motion_resp.FromJSONString(resp);
    return motion_resp;
  }
  void Robot::RobotImpl::waitMove(const MotionIndex & req)
  {
    json_rpc_connector_->CallRpc("wait_move",req.ToJSONString(),nullptr);
  }
  MotionIndex Robot::RobotImpl::getRunningMotion()
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_running_motion","{}",&resp);
    MotionIndex get_running_motion_resp;
    get_running_motion_resp.FromJSONString(resp);
    return get_running_motion_resp;
  }
  GetMotionStateResponse Robot::RobotImpl::getMotionState(const MotionIndex & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("get_motion_state",req.ToJSONString(),&resp);
    GetMotionStateResponse get_motion_state_resp;
    get_motion_state_resp.FromJSONString(resp);
    return get_motion_state_resp;
  }
  void Robot::RobotImpl::stopMove()
  {
    json_rpc_connector_->CallRpc("stop_move","{}",nullptr);
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
    json_rpc_connector_->CallRpc("start_task",req.ToJSONString(),&resp);
    control::TaskIndex start_task_resp;
    start_task_resp.FromJSONString(resp);
    return start_task_resp;
  }

  control::TaskIds Robot::RobotImpl::loadTaskList()
  {
    std::string resp;
    json_rpc_connector_->CallRpc("load_task_list","{}",&resp);
    control::TaskIds list_resp;
    list_resp.FromJSONString(resp);
    return list_resp;
  }

  void Robot::RobotImpl::pauseTask(const control::PauseRequest & req)
  {
    json_rpc_connector_->CallRpc("pause_task",req.ToJSONString(),nullptr);
  }
  void Robot::RobotImpl::resumeTask(const control::TaskIndex & req)
  {
    json_rpc_connector_->CallRpc("resume_task",req.ToJSONString(),nullptr);
  }
  void Robot::RobotImpl::cancelTask(const control::TaskIndex & req)
  {
    json_rpc_connector_->CallRpc("cancel_task",req.ToJSONString(),nullptr);
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

  void Robot::RobotImpl::saveFile(const file::SaveFileRequest & req)
  {
    json_rpc_connector_->CallRpc("save_file",req.ToJSONString(),nullptr);
  }

  void Robot::RobotImpl::renameFile(const file::RenameFileRequest & req)
  {
    json_rpc_connector_->CallRpc("rename_file",req.ToJSONString(),nullptr);
  }

  file::File Robot::RobotImpl::loadFile(const file::FileIndex & req)
  {
    std::string resp_str;
    json_rpc_connector_->CallRpc("load_file",req.ToJSONString(),&resp_str);
    file::File resp;
    resp.FromJSONString(resp_str);
    return resp;
  }

  file::LoadFileListResponse Robot::RobotImpl::loadFileList(const file::LoadFileListRequest &req)
  {
    std::string resp_str;
    json_rpc_connector_->CallRpc("load_file_list", req.ToJSONString(), &resp_str);
    file::LoadFileListResponse resp;
    resp.FromJSONString(resp_str);
    return resp;
  }

  void Robot::RobotImpl::zip(const file::ZipRequest &req)
  {
    json_rpc_connector_->CallRpc("zip",req.ToJSONString(),nullptr);
  }

  void Robot::RobotImpl::unzip(const file::UnzipRequest &req)
  {
    json_rpc_connector_->CallRpc("unzip",req.ToJSONString(),nullptr);
  }

  file::LoadZipListResponse Robot::RobotImpl::loadZipList(const file::LoadZipListRequest &req)
  {
    std::string resp_str;
    json_rpc_connector_->CallRpc("load_zip_list",req.ToJSONString(),&resp_str);
    file::LoadZipListResponse resp;
    resp.FromJSONString(resp_str);
    return resp;
  }
  }
}