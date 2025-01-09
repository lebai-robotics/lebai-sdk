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
#include "protos/motion.hh"
#include "protos/system.hh"
#include "protos/io.hh"
#include "protos/claw.hh"
#include "protos/led.hh"
#include "protos/kinematic.hh"
#include "jsonrpc_connector.hh"
#include "protos/signal.hh"
#include "protos/control.hh"
#include "protos/file.hh"
#include "protos/dynamic.hh"
#include "protos/db.hh"
#include "protos/modbus.hh"
#include "protos/serial.hh"

namespace lebai {
namespace l_master {
class Robot::RobotImpl {
 public:
  RobotImpl(const ::std::string &ip, bool simulator);
  virtual ~RobotImpl();
  std::tuple<int, std::string> call(const std::string &method,
                                    const std::string &params);
  bool isNetworkConnected();
  int startSys();
  int stopSys();
  int powerdown();
  int stop();
  int estop();
  int teachMode();
  int endTeachMode();
  int pause();
  int resume();
  void reboot();
  // // int movej(const std::vector<double> & p, double v, double a, double t,
  // double r, bool relative);
  motion::MotionIndex moveJoint(const motion::MoveRequest &req);
  motion::MotionIndex moveLinear(const motion::MoveRequest &req);
  motion::MotionIndex moveCircular(const motion::MovecRequest &req);
  motion::MotionIndex towardJoint(const motion::MoveRequest &req);
  motion::MotionIndex speedJoint(const motion::SpeedJRequest &req);
  motion::MotionIndex speedLinear(const motion::SpeedLRequest &req);
  void movePvat(const motion::MovePvatRequest &req);
  void waitMove(const motion::MotionIndex &req);
  motion::MotionIndex getRunningMotion();
  motion::GetMotionStateResponse getMotionState(const motion::MotionIndex &req);
  void stopMove();
  system::RobotState getRobotState();
  system::EstopReason getEstopReason();
  system::PhyData getPhyData();
  kinematic::KinData getKinData();
  io::GetDioPinResponse getDI(const io::GetDioPinRequest &req);
  io::GetDioPinsResponse getDIS(const io::GetDioPinsRequest &req);
  io::GetDioPinResponse getDO(const io::GetDioPinRequest &req);
  io::GetDioPinsResponse getDOS(const io::GetDioPinsRequest &req);
  void setDO(const io::SetDoPinRequest &req);
  io::GetAioPinResponse getAI(const io::GetAioPinRequest &req);
  io::GetAioPinsResponse getAIS(const io::GetAioPinsRequest &req);
  io::GetAioPinResponse getAO(const io::GetAioPinRequest &req);
  io::GetAioPinsResponse getAOS(const io::GetAioPinsRequest &req);
  void setDioMode(const io::SetDioModeRequest &req);
  io::GetDiosModeResponse getDiosMode(const io::GetDiosModeRequest &req);
  void setAO(const io::SetAoPinRequest &req);
  void setClaw(const claw::SetClawRequest &req);
  void setLed(const led::LedData &req);
  void setVoice(const led::VoiceData &req);
  void setFan(const led::FanData &req);
  void setSignal(const signal::SetSignalRequest &req);
  signal::GetSignalResponse getSignal(const signal::GetSignalRequest &req);
  void addSignal(const signal::SetSignalRequest &req);
  control::TaskIndex scene(const control::StartTaskRequest &req);
  control::TaskIds loadTaskList();
  control::TaskStdout waitTask(const control::TaskIndex &req);
  void pauseTask(const control::PauseRequest &req);
  void resumeTask(const control::TaskIndex &req);
  void cancelTask(const control::TaskIndex &req);
  control::HookResponse execHook(const control::Exec &req);
  control::Task loadTask(const control::TaskIndex &req);
  control::Task loadTask();
  claw::Claw getClaw();
  posture::CartesianPose getForwardKin(const posture::PoseRequest &req);
  posture::JointPose getInverseKin(const posture::GetInverseKinRequest &req);
  posture::CartesianPose getPoseTrans(const posture::GetPoseTransRequest &req);
  posture::CartesianPose getPoseInverse(const posture::PoseRequest &req);
  void saveFile(const file::SaveFileRequest &req);
  void renameFile(const file::RenameFileRequest &req);
  file::File loadFile(const file::FileIndex &req);
  file::LoadFileListResponse loadFileList(const file::LoadFileListRequest &req);
  void zip(const file::ZipRequest &req);
  void unzip(const file::UnzipRequest &req);
  file::LoadZipListResponse loadZipList(const file::LoadZipListRequest &req);
  void setPayload(const dynamic::SetPayloadRequest &req);
  void setPayload(const dynamic::SetCogRequest &req);
  void setPayload(const dynamic::SetMassRequest &req);
  dynamic::Payload getPayload();
  void setGravity(const posture::Position &req);
  posture::Position getGravity();
  void savePayload(const dynamic::SavePayloadRequest &req);
  dynamic::Payload loadPayload(const db::LoadRequest &req);
  db::LoadListResponse loadPayloadList(const db::LoadListRequest &req);
  void setTcp(const posture::CartesianPose &req);
  posture::CartesianPose getTcp();
  void setKinFactor(const kinematic::KinFactor &req);
  kinematic::KinFactor getKinFactor();
  posture::CartesianPose loadTcp(const db::LoadRequest &req);
  void writeSingleCoil(const modbus::SetCoilRequest &req);
  void writeMultipleCoils(const modbus::SetCoilsRequest &req);
  modbus::GetCoilsResponse readCoils(const modbus::GetCoilsRequest &req);
  modbus::GetCoilsResponse readDiscreteInputs(
      const modbus::GetCoilsRequest &req);
  void writeSingleRegister(const modbus::SetRegisterRequest &req);
  void writeMultipleRegisters(const modbus::SetRegistersRequest &req);
  modbus::GetRegistersResponse readInputRegisters(
      const modbus::GetRegistersRequest &req);
  modbus::GetRegistersResponse readHoldingRegisters(
      const modbus::GetRegistersRequest &req);
  void setSerialBaudRateRequest(const serial::SetSerialBaudRateRequest &req);
  void setSerialParityRequest(const serial::SetSerialParityRequest &req);

 protected:
  std::unique_ptr<JSONRpcConnector> json_rpc_connector_;
  double timeout_ = 1.0;
  const uint16_t simulation_port_ = 3030;
  const uint16_t physical_machine_port_ = 3031;
  // int jsonrpc_id_ = 0;
  // WebSocketEndPoint endpoint_;
};
}  // namespace l_master
}  // namespace lebai