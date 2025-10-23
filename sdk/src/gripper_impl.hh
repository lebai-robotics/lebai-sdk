
/**
 * Copyright 2022-2025 lebai.ltd
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

#include "lebai/gripper.hh"
#include <memory>
#include <string>

// Forward declarations for ModbusLib
class ModbusClient;
class ModbusClientPort;

namespace lebai {
namespace l_master {

class Gripper::GripperImpl {
 public:
  GripperImpl(const std::string& port_name);
  virtual ~GripperImpl();

  void SetPosition(unsigned int amplitude);
  unsigned int GetCurrentPosition() const;
  void SetForce(unsigned int force);
  unsigned int GetCurrentForce() const;
  void SetVelocity(unsigned int velocity, bool persistent);
  unsigned int GetCurrentVelocity(bool persistent) const;
  void DoCalibration();
  bool IsCalibrated() const;
  void TurnOnAutoCalibration();
  void TurnOffAutoCalibration();

 private:
  //  通讯接口 ：RS485     波特率：115200      8N1
  //  Modbus RTU 协议
  //  Modbus 地址 ：1

  std::string port_name_;
  unsigned int baud_rate_ = 115200;
  uint8_t modbus_address_ = 1;

  ModbusClientPort* port_;
  std::unique_ptr<ModbusClient> client_;
};

}  // namespace l_master
}  // namespace lebai