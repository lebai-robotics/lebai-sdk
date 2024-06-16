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

#include <memory>
#include <lebai/robot.hh>
#include "discovery_impl.hh"

namespace lebai {

namespace zeroconf {

std::string ControllerInfo::str() {
  std::string str;
  str += "{'hostname':" + hostname + ",";
  str += "'ip_address':" + ip_address + ",";
  str += "'mac_address':" + mac_address + ",";
  str += "'model':" + model + ",";
  str += "'ds_version':" + ds_version + ",";
  str += "'rc_version':" + rc_version + ",";
  str += "'id':" + id + "}";
  return str;
}
Discovery::Discovery() { impl_ = std::make_unique<DiscoveryImpl>(); }
Discovery::~Discovery() {}

std::vector<ControllerInfo> Discovery::resolve() { return impl_->resolve(); }

}  // namespace zeroconf

}  // namespace lebai