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
#include <lebai/robot.hh>


int main(int argc, char ** argv)
{
  if(argc < 2)
  {
    std::cerr<<"You must specify the IP address of the robot"<<std::endl;
    std::cerr<<"Execute example as follow:"<<std::endl;
    std::cerr<<"./example 192.168.1.200 sim"<<std::endl;
    return 0;
  }
  std::string ip = argv[1];
  bool sim = false;
  if(argc > 2)
  {
    std::string sim_str = argv[2];
    if(sim_str == "sim")
    {
      sim = true;
    }
  }
  std::cout<<"Connecting to robot at "<<ip<<std::endl;
  std::cout<<"Connecting for simulation mode is "<<sim<<std::endl;
  // Create robot instance
  lebai::l_master::Robot robot(ip, sim);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  robot.movej({0.0/ 180.0 * M_PI, -45.0/ 180.0 * M_PI, 90.0/ 180.0 * M_PI, -45/ 180.0 * M_PI, 90.0/ 180.0 * M_PI, 0.0/ 180.0 * M_PI}, 1.0, 0.5, 0.0, 0.0);
  robot.wait_move();   
  lebai::l_master::CartesianPose dir = {{"x", 0.05}, {"y", 0.0}, {"z", 0.0}, {"rx", 0.0}, {"ry", 0.0}, {"rz", 0.0}};
  auto tcp = robot.get_target_tcp_pose(); 
  robot.speedl(1.0, dir, 2.0, tcp);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  robot.stop_move();
  return 0;
}
