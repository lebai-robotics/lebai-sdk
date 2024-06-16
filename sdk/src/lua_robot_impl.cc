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

#include "lua_robot_impl.hh"
#include <exception>
#include <iostream>

namespace lebai {
namespace l_master {
LuaRobot::LuaRobotImpl::LuaRobotImpl(const std::string &ip) {
  io_service_ = std::make_unique<asio::io_service>();
  connect(ip);
}
LuaRobot::LuaRobotImpl::~LuaRobotImpl() {
  if (socket_) {
    socket_->close();
  }
}
int LuaRobot::LuaRobotImpl::connect(const std::string &ip) {
  asio::ip::tcp::resolver resolver(*io_service_);
  auto endpoint_iterator = resolver.resolve({ip, "5180"});
  socket_ = std::make_unique<asio::ip::tcp::socket>(*io_service_);
  doConnect(endpoint_iterator);
  return 0;
}
void LuaRobot::LuaRobotImpl::send(const std::string &lua_code) {
  asio::write(*socket_, asio::buffer(lua_code.c_str(), lua_code.size()));
  return;
}

std::string LuaRobot::LuaRobotImpl::call(const std::string &lua_code) {
  std::string print_lua_code = "print(" + lua_code + ")";
  asio::write(*socket_,
              asio::buffer(print_lua_code.c_str(), print_lua_code.size()));
  std::string ret;
  // Max 1000 return string size.
  ret.resize(1000);
  size_t len = socket_->read_some(asio::buffer(ret));
  ret = ret.substr(0, len - 2);
  return ret;
}

}  // namespace l_master
}  // namespace lebai