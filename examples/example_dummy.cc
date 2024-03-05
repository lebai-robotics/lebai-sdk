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

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "You must specify the IP address of the robot" << std::endl;
    std::cerr << "Execute example as follow:" << std::endl;
    std::cerr << "./example 192.168.1.200 sim" << std::endl;
    return 0;
  }
  std::string ip = argv[1];
  bool sim = false;
  if (argc > 2)
  {
    std::string sim_str = argv[2];
    if (sim_str == "sim")
    {
      sim = true;
    }
  }
  std::cout << "Connecting to robot at " << ip << std::endl;
  std::cout << "Connecting for simulation mode is " << sim << std::endl;
  // Create robot instance
  lebai::l_master::Robot robot(ip, sim);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  robot.movej({73.6359 / 180.0 * M_PI, -68.4064 / 180.0 * M_PI, -52.2675 / 180.0 * M_PI, 60.5182 / 180.0 * M_PI, -79.6674 / 180.0 * M_PI, 92.9829 / 180.0 * M_PI}, 1.0, 0.5, 0.0, 0.0);
  robot.wait_move();

  std::vector<double> jp = {73.6359 / 180.0 * M_PI, -68.4064 / 180.0 * M_PI, -52.2675 / 180.0 * M_PI, 60.5182 / 180.0 * M_PI, -79.6674 / 180.0 * M_PI, 92.9829 / 180.0 * M_PI};
  std::cout << "jp " << jp[0] << ", " << jp[1] << ", " << jp[2] << ", " << jp[3] << ", " << jp[4] << ", " << jp[5] << std::endl;
  lebai::l_master::CartesianPose pose;
  pose["x"] = 0.088501997;
  pose["y"] = -0.29743993;
  pose["z"] = 0.70731318;
  pose["rx"] = 16.6127 / 180.0 * M_PI;
  pose["ry"] = 86.6987 / 180.0 * M_PI;
  pose["rz"] = -66.8482 / 180.0 * M_PI;

  auto ik_resp = robot.kinematics_inverse(pose, jp);
  if (ik_resp.ok)
  {
    std::cout << "kinematics_inverse: " << ik_resp.joint_positions[0] << ", " << ik_resp.joint_positions[1] << ", " << ik_resp.joint_positions[2] << ", " << ik_resp.joint_positions[3] << ", " << ik_resp.joint_positions[4] << ", " << ik_resp.joint_positions[5] << std::endl;
  }
  else
  {
    std::cout << "ik not ok!\n";
  }
  // jp = {-28/ 180.0 * M_PI, -59.0/ 180.0 * M_PI, 96.0/ 180.0 * M_PI, -2.0/ 180.0 * M_PI, -92.0/ 180.0 * M_PI, 16.0/ 180.0 * M_PI};
  // std::cout<<"jp "<<jp[0]<<", "<<jp[1]<<", "<<jp[2]<<", "<<jp[3]<<", "<<jp[4]<<", "<<jp[5]<<std::endl;
  // fk_resp = robot.kinematics_forward(jp);
  // std::cout<<"kinematics_forward: "<<fk_resp.pose["x"]<<", "<<fk_resp.pose["y"]<<", "<<fk_resp.pose["z"]<<", "<<fk_resp.pose["rx"]<<", "<<fk_resp.pose["ry"]<<", "<<fk_resp.pose["rz"]<<std::endl;
  return 0;
}
