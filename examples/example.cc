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

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "You must specify the IP address of the robot" << std::endl;
    std::cerr << "Execute example as follow:" << std::endl;
    std::cerr << "./example 192.168.1.200 sim" << std::endl;
    return 0;
  }
  std::string ip = argv[1];
  bool sim = false;
  if (argc > 2) {
    std::string sim_str = argv[2];
    if (sim_str == "sim") {
      sim = true;
    }
  }
  std::cout << "Connecting to robot at " << ip << std::endl;
  std::cout << "Connecting for simulation mode is " << sim << std::endl;
  // Create robot instance
  lebai::l_master::Robot robot(ip, sim);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  robot.movej({0.0, 0.0, 0.0, 0.0, 0.0, 0.5}, 1.0, 0.5, 0.0, 0.0);
  robot.movej({0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 1.0, 0.5, 0.0, 0.0);

  // robot.movej({0.0 / 180.0 * M_PI, -52.0 / 180.0 * M_PI, 86.0 / 180.0 * M_PI,
  //              8.0 / 180.0 * M_PI, -59.0 / 180.0 * M_PI, -11.0 / 180.0 *
  //              M_PI},
  //             1.0, 0.5, 0.0, 0.0);
  robot.wait_move();
  // movej({j1=0,j2=0,j3=0,j4=0,j5=0,j6=0}, 0.1, 0.1, 0, 0)
  // move_pvat({j1=0.1,j2=0,j3=0,j4=0,j5=0,j6=0}, 0.1, 0.1, 2)
  // move_pvat({j1=0.2,j2=0,j3=0,j4=0,j5=0,j6=0}, 0.1, 0.1, 2)
  // move_pvat({j1=0.3,j2=0,j3=0,j4=0,j5=0,j6=0}, 0.1, 0.1, 2)
  // move_pvat({j1=0.4,j2=0,j3=0,j4=0,j5=0,j6=0}, 0.1, 0.1, 2)
  robot.move_pvat({0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1}, 2.0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  robot.move_pvat({0.2, 0.0, 0.0, 0.0, 0.0, 0.0},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1}, 2.0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  robot.move_pvat({0.3, 0.0, 0.0, 0.0, 0.0, 0.0},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1}, 2.0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  robot.move_pvat({0.4, 0.0, 0.0, 0.0, 0.0, 0.0},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1}, 2.0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  robot.move_pvat({0.5, 0.0, 0.0, 0.0, 0.0, 0.0},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1},
                  {0.1, 0.1, 0.1, 0.1, 0.1, 0.1}, 2.0);
  robot.wait_move();
  // for (size_t i = 0; i < 30; ++i)
  // {
  //   std::this_thread::sleep_for(std::chrono::seconds(1));
  //   try {
  //     robot.get_robot_mode();
  //   }
  //   catch(std::runtime_error & ex)
  //   {
  //     std::cout<<"get_robot_mode error: "<<ex.what()<<std::endl;
  //   }

  // }

  // std::cout << "after sleep" << std::endl;
  // bool is_network_connected = robot.is_network_connected();
  // std::cout << "is connected "
  //           << "\n";
  // if (!is_network_connected)
  // {
  //   std::cout << "exit....\n";
  //   return 0;
  // }

  // robot.wait_move();
  // robot.movec({{"x", -0.282541}, {"y", -0.168246}, {"z", 0.265824},
  // {"rz", 1.27256}, {"ry", -0.206353}, {"rx", 0.937445}},
  // {{"x", -0.255832}, {"y", 0.00270435}, {"z", 0.266642}, {"rz", 1.27293},
  // {"ry", -0.20805}, {"rx", 0.94485}},
  //   0.0, 1.0, 0.5, 0.0, 0.0);
  // kinematics_forward: -0.282541, -0.168246, 0.265824, 1.27256, -0.206353,
  // 0.937445 kinematics_forward: -0.255832, 0.00270435, 0.266642, 1.27293,
  // -0.20805, 0.94485

  // auto fk_resp = robot.kinematics_forward({3.0/ 180.0 * M_PI, -48.0/ 180.0 *
  // M_PI, 78.0/ 180.0 * M_PI, 9.0/ 180.0 * M_PI, -67.0/ 180.0 * M_PI, -3.0/
  // 180.0 * M_PI}); std::vector<double> jp = {3.0/ 180.0 * M_PI, -48.0/ 180.0 *
  // M_PI, 78.0/ 180.0 * M_PI, 9.0/ 180.0 * M_PI, -67.0/ 180.0 * M_PI, -3.0/
  // 180.0 * M_PI}; std::cout<<"jp "<<jp[0]<<", "<<jp[1]<<", "<<jp[2]<<",
  // "<<jp[3]<<", "<<jp[4]<<", "<<jp[5]<<std::endl;
  // std::cout<<"kinematics_forward: "<<fk_resp.pose["x"]<<",
  // "<<fk_resp.pose["y"]<<", "<<fk_resp.pose["z"]<<", "<<fk_resp.pose["rx"]<<",
  // "<<fk_resp.pose["ry"]<<", "<<fk_resp.pose["rz"]<<std::endl; auto ik_resp =
  // robot.kinematics_inverse(fk_resp.pose); if(ik_resp.ok)
  //{
  //   std::cout<<"kinematics_inverse: "<<ik_resp.joint_positions[0]<<",
  //   "<<ik_resp.joint_positions[1]<<", "<<ik_resp.joint_positions[2]<<",
  //   "<<ik_resp.joint_positions[3]<<", "<<ik_resp.joint_positions[4]<<",
  //   "<<ik_resp.joint_positions[5]<<std::endl;
  // }
  // jp = {-28/ 180.0 * M_PI, -59.0/ 180.0 * M_PI, 96.0/ 180.0 * M_PI, -2.0/
  // 180.0 * M_PI, -92.0/ 180.0 * M_PI, 16.0/ 180.0 * M_PI}; std::cout<<"jp
  // "<<jp[0]<<", "<<jp[1]<<", "<<jp[2]<<", "<<jp[3]<<", "<<jp[4]<<",
  // "<<jp[5]<<std::endl; fk_resp = robot.kinematics_forward(jp);
  // std::cout<<"kinematics_forward: "<<fk_resp.pose["x"]<<",
  // "<<fk_resp.pose["y"]<<", "<<fk_resp.pose["z"]<<", "<<fk_resp.pose["rx"]<<",
  // "<<fk_resp.pose["ry"]<<", "<<fk_resp.pose["rz"]<<std::endl;
  return 0;
}
