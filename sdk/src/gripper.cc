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

#include "lebai/gripper.hh"
#include "gripper_impl.hh"

namespace lebai {
namespace l_master {

Gripper::Gripper(const std::string &port_name)
    : impl_(std::make_unique<GripperImpl>(port_name)) {}

Gripper::~Gripper() = default;

void Gripper::set_position(unsigned int position) {
  impl_->SetPosition(position);
}

unsigned int Gripper::get_current_position() const {
  return impl_->GetCurrentPosition();
}

void Gripper::set_force(unsigned int force) { return impl_->SetForce(force); }
unsigned int Gripper::get_current_force() const {
  return impl_->GetCurrentForce();
}

void Gripper::set_velocity(unsigned int velocity, bool persistent) {
  return impl_->SetVelocity(velocity, persistent);
}

unsigned int Gripper::get_current_velocity(bool persistent) const {
  return impl_->GetCurrentVelocity(persistent);
}

void Gripper::do_calibration() { impl_->DoCalibration(); }

bool Gripper::is_calibrated() const { return impl_->IsCalibrated(); }

void Gripper::turn_on_auto_calibration() { impl_->TurnOnAutoCalibration(); }

void Gripper::turn_off_auto_calibration() { impl_->TurnOffAutoCalibration(); }

}  // namespace l_master
}  // namespace lebai