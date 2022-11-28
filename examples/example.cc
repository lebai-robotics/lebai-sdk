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
  // try
  // {
  //   robot.start_sys();
  //   auto jp = robot.get_actual_joint_positions();
  //   for (auto&& j : jp)
  //   {
  //     std::cout << j.first<<" "<<j.second << std::endl;
  //   }
  // }
  // catch(std::exception & e)
  // {
  //   std::cout<<"Exception: "<<e.what()<<std::endl;
  // }

  

  
  // {
  //   std::array<double, 6> a = {1, 2, 0, 0, 0, 0};
  //   std::array<double, 6> b = {0, 0, 3, 0, 0, 0};
  //   auto c = robot.pose_inverse(a);
  //   std::cout<<"pose_inverse: "<<c[0]<<", "<<c[1]<<", "<<c[2]<<", "<<c[3]<<", "<<c[4]<<", "<<c[5]<<std::endl;
  //   a = {0, 0, 0, 1, 0, 0};
  //   b = {0, 0, 0, 0, 0, 1};
  //   c = robot.pose_times(a,b);
  //   std::cout<<"pose_times: "<<c[0]<<", "<<c[1]<<", "<<c[2]<<", "<<c[3]<<", "<<c[4]<<", "<<c[5]<<std::endl;
  //   auto d = robot.pose_inverse(a);
  //   std::cout<<"pose_inverse: "<<d[0]<<", "<<d[1]<<", "<<d[2]<<", "<<d[3]<<", "<<d[4]<<", "<<d[5]<<std::endl;
  //   return 0;
  // }

  robot.start_sys();
  std::tuple<int,std::string> resp;
  // Call json string api
  // {    
  //   std::string movej_req = "{\"param\":{\"velocity\":0.5},\"pose\":{\"joint\":{\"delta\":"
  //   "{\"joint\":[-1.0,0.0,0.0,0.0,0.0,0.0]}}}}";
  //   resp = robot.call("move_joint", movej_req);
  // }
  // movej with joint positions.
  {    
    std::map<std::string, double> joint_positions;
    joint_positions["j1"] = 0.0;
    joint_positions["j2"] = -60.0 / 180.0 * M_PI;
    joint_positions["j3"] = 80.0 / 180.0 * M_PI;
    joint_positions["j4"] = -10.0 / 180.0 * M_PI;
    joint_positions["j5"] = -60.0 / 180.0 * M_PI;
    joint_positions["j6"] = 0.0;
    robot.movej(joint_positions, 1.0, 0.5, 0.0, 0.0);
  }
  // movej with cartesian pose.
  {
    robot.movej({-0.296,-0.295,0.285,60.0 / 180.0 * M_PI,-5.0 / 180.0 * M_PI,81.0 / 180.0 * M_PI}, 3.0, 1.0, 0.0, 0.0);    
  }
  // movel with joint positions.
  {
    std::map<std::string, double> joint_positions;
    joint_positions["j1"] = 0.0;
    joint_positions["j2"] = -60.0 / 180.0 * M_PI;
    joint_positions["j3"] = 80.0 / 180.0 * M_PI;
    joint_positions["j4"] = -10.0 / 180.0 * M_PI;
    joint_positions["j5"] = -60.0 / 180.0 * M_PI;
    joint_positions["j6"] = 0.0;
    robot.movel(joint_positions, 0.3, 1.0, 0.0, 0.0);
  }
  // movel with cartesian pose.
  {
    robot.movel({-0.296,-0.295,0.285,60.0 / 180.0 * M_PI,-5.0 / 180.0 * M_PI,81.0 / 180.0 * M_PI}, 1.0, 0.5, 0.0, 0.0);    
  }

  return 0;
}
