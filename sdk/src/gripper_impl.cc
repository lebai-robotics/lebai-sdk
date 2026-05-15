/**
 * Copyright 2025 lebai.ltd
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

#include "gripper_impl.hh"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

namespace lebai {
namespace l_master {
namespace {

constexpr uint16_t kPositionCommandRegister = 0x9C40;
constexpr uint16_t kForceCommandRegister = 0x9C41;
constexpr uint16_t kCurrentForceRegister = 0x9C42;
constexpr uint16_t kCurrentPositionRegister = 0x9C45;
constexpr uint16_t kCalibrationCommandRegister = 0x9C48;
constexpr uint16_t kCalibrationStateRegister = 0x9C49;
constexpr uint16_t kVolatileVelocityRegister = 0x9C4A;
constexpr uint16_t kPersistentVelocityRegister = 0x9C4B;
constexpr uint16_t kAutoCalibrationRegister = 40090;

void validate_percent(unsigned int value, const char* name) {
  if (value > 100) {
    throw std::invalid_argument(std::string(name) +
                                " must be between 0 and 100");
  }
}

unsigned int clamp_percent(uint16_t value) {
  return value > 100 ? 100 : static_cast<unsigned int>(value);
}

uint16_t read_single(ModbusRtuClient& client, uint16_t reg,
                     const char* action) {
  try {
    const auto values = client.read_holding_registers(reg, 1);
    if (values.size() != 1) {
      throw std::runtime_error("unexpected register count");
    }
    return values.front();
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to ") + action + ": " +
                             e.what());
  }
}

void write_single_as_multiple(ModbusRtuClient& client, uint16_t reg,
                              uint16_t value, const char* action) {
  try {
    client.write_multiple_registers(reg, {value});
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to ") + action + ": " +
                             e.what());
  }
}

}  // namespace

Gripper::GripperImpl::GripperImpl(const std::string& port_name)
    : port_name_(port_name) {
  try {
    ModbusRtuClient::Config config;
    config.port_name = port_name_;
    config.baud_rate = baud_rate_;
    config.slave_id = modbus_address_;
    config.timeout = std::chrono::milliseconds(5000);

    client_ = std::make_unique<ModbusRtuClient>(config);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Gripper initialized on " << port_name_
              << " (Unit: " << static_cast<int>(modbus_address_)
              << ", Baud: " << baud_rate_ << ")" << std::endl;
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to initialize gripper: " +
                             std::string(e.what()));
  }
}

Gripper::GripperImpl::~GripperImpl() = default;

void Gripper::GripperImpl::SetPosition(unsigned int position) {
  validate_percent(position, "Position");
  write_single_as_multiple(*client_, kPositionCommandRegister,
                           static_cast<uint16_t>(position), "set position");
}

unsigned int Gripper::GripperImpl::GetCurrentPosition() const {
  return clamp_percent(
      read_single(*client_, kCurrentPositionRegister, "get current position"));
}

void Gripper::GripperImpl::SetForce(unsigned int force) {
  validate_percent(force, "Force");
  write_single_as_multiple(*client_, kForceCommandRegister,
                           static_cast<uint16_t>(force), "set force");
}

unsigned int Gripper::GripperImpl::GetCurrentForce() const {
  return clamp_percent(
      read_single(*client_, kCurrentForceRegister, "get current force"));
}

void Gripper::GripperImpl::SetVelocity(unsigned int velocity, bool persistent) {
  validate_percent(velocity, "Velocity");
  const uint16_t reg =
      persistent ? kPersistentVelocityRegister : kVolatileVelocityRegister;
  write_single_as_multiple(*client_, reg, static_cast<uint16_t>(velocity),
                           "set velocity");
}

unsigned int Gripper::GripperImpl::GetCurrentVelocity(bool persistent) const {
  const uint16_t reg =
      persistent ? kPersistentVelocityRegister : kVolatileVelocityRegister;
  return clamp_percent(read_single(*client_, reg, "get current velocity"));
}

void Gripper::GripperImpl::DoCalibration() {
  write_single_as_multiple(*client_, kCalibrationCommandRegister, 1,
                           "do calibration");
}

bool Gripper::GripperImpl::IsCalibrated() const {
  const uint16_t flag =
      static_cast<uint16_t>(read_single(*client_, kCalibrationStateRegister,
                                        "get calibration state") &
                            0x0001);
  return flag == 0;
}

void Gripper::GripperImpl::TurnOnAutoCalibration() {
  write_single_as_multiple(*client_, kAutoCalibrationRegister, 3,
                           "turn on auto calibration");
}

void Gripper::GripperImpl::TurnOffAutoCalibration() {
  try {
    client_->write_single_register(kAutoCalibrationRegister, 0x03);
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to turn off auto calibration: " +
                             std::string(e.what()));
  }
}

}  // namespace l_master
}  // namespace lebai
