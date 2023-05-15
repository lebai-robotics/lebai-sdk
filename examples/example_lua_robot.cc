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

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <lebai/lua_robot.hh>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "You must specify the IP address of the robot" << std::endl;
    std::cerr << "Execute example as follow:" << std::endl;
    std::cerr << "./example 192.168.1.200" << std::endl;
    return 0;
  }
  std::string ip = argv[1];
  bool sim = false;
  std::cout << "Connecting to robot at " << ip << std::endl;

  lebai::l_master::LuaRobot lua_robot(ip);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // lua_robot.call("print(start_sys())");
  // lua_robot.send("movej({j1 = 0.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.2, 0.2, 0, 0)")
  // lua_robot.send("movej({j1 = 1.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.2, 0.2, 0, 0)");
  // lua_robot.send("get_robot_mode()");
  // auto ret = lua_robot.call("movej({j1 = 0.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.5, 0.8, 0, 0)");
  // std::cerr<<"Ret is: '"<<ret<<"'.\n";
  // ret = lua_robot.call("movej({j1 = 1.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.5, 0.8, 0, 0)");
  // std::cerr<<"Ret is: '"<<ret<<"'.\n";
  // ret = lua_robot.call("get_robot_mode()");
  // std::cerr<<"Ret is: '"<<ret<<"'.\n";
  // auto ret = lua_robot.call("program_begin(0)\n \
  // print(movej({j1 = 1.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.2, 0.2, 0, 0))\n \
  // print(movej({j1 = 0.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.5, 0.8, 0, 0))\n \
  // program_end(0)\n");
  // std::cerr<<"Ret is: '"<<ret<<"'.\n";
  std::vector<std::string> codes;
  codes.push_back("start_sys()");
  codes.push_back("movej({j1 = 0.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.2, 0.2, 0, 0)");
  codes.push_back("movej({j1 = 1.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.2, 0.2, 0, 0)");
  codes.push_back("sync()");
  codes.push_back("stop_sys()");
  lua_robot.send(codes);
  auto jps = lua_robot.call("get_target_joint_positions()");
  
  return 0;
}
