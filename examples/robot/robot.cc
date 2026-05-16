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

#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <lebai/robot.hh>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "You must specify the IP address of the robot" << std::endl;
    std::cerr << "Execute example as follow:" << std::endl;
    std::cerr << argv[0] << " 127.0.0.1 sim" << std::endl;
    std::cerr << argv[0] << " 127.0.0.1 sim --move" << std::endl;
    return 1;
  }

  std::string ip = argv[1];
  bool sim = false;
  bool run_motion = false;
  for (int i = 2; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "sim") {
      sim = true;
    } else if (arg == "--move") {
      run_motion = true;
    } else {
      std::cerr << "Unknown argument: " << arg << std::endl;
      return 1;
    }
  }

  try {
    std::cout << "Lebai SDK version: " << lebai::l_master::version()
              << std::endl;
    std::cout << "Connecting to robot at " << ip << std::endl;
    std::cout << "Simulation mode: " << (sim ? "true" : "false")
              << std::endl;

    lebai::l_master::Robot robot(ip, sim);
    const auto system_info = robot.get_system_info();
    std::cout << "system: " << system_info.name
              << ", kernel=" << system_info.kernel_version
              << ", host=" << system_info.host_name << std::endl;

    const auto hello = robot.hello("example");
    std::cout << "hello: " << hello << std::endl;

    const auto robot_info = robot.get_robot_info();
    std::cout << "robot: " << robot_info.name
              << ", arm=" << robot_info.arm_model
              << ", box=" << robot_info.box_model << std::endl;

    const auto state = robot.get_robot_state();
    std::cout << "state: " << state << std::endl;

    const auto estop_reason = robot.get_estop_reason();
    std::cout << "estop reason: " << estop_reason << std::endl;

    const auto plugins = robot.load_plugins();
    std::cout << "installed plugins: " << plugins.size() << std::endl;

    const auto store = robot.get_plugin_store();
    std::cout << "plugin store entries: " << store.size() << std::endl;

    const auto dh = robot.get_dh();
    std::cout << "dh params: " << dh.size() << std::endl;

    if (run_motion) {
      std::cout << "Starting system before motion..." << std::endl;
      robot.start_sys();
      std::this_thread::sleep_for(std::chrono::seconds(1));

      std::cout << "Running two move_joint commands..." << std::endl;
      robot.move_joint({0.0, 0.0, 0.0, 0.0, 0.0, 0.5}, 1.0, 0.5, 0.0,
                       0.0);
      robot.move_joint({0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 1.0, 0.5, 0.0,
                       0.0);
      robot.wait_move();
      std::cout << "motion finished" << std::endl;
    } else {
      std::cout << "Motion skipped. Pass --move to run the motion demo."
                << std::endl;
    }

    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Example failed: " << e.what() << std::endl;
    return 2;
  }
}
