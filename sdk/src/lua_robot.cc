/**
 * Copyright 2022-2023 lebai.ltd
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

#include <lebai/lua_robot.hh>
#include "lua_robot_impl.hh"
#include <memory>



namespace lebai {

namespace l_master
{

LuaRobot::LuaRobot(std::string ip)
{
  impl_ = std::make_unique<LuaRobotImpl>(ip);
}
LuaRobot::~LuaRobot() {}


void LuaRobot::send(const std::string & lua_code)
{
    impl_->send(lua_code);
}


void LuaRobot::send(const std::vector<std::string> & lua_codes)
{
  std::string codes;
  codes = "program_begin(0)\n";
  for(auto &&code: lua_codes)
  {
    codes+=code+"\n";
  }
  codes += "program_end(0)\n";
  impl_->send(codes);
}

std::string LuaRobot::call(const std::string & lua_code)
{
    return impl_->call(lua_code);
}

}

}  // namespace l_master_sdk
