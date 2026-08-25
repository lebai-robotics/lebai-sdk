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

#include <exception>
#include <iostream>
#include <string>

#include <lebai/robot.hh>

int main(int argc, char** argv) {
  if (argc < 3 || argc > 4) {
    std::cerr << "Usage: " << argv[0] << " <robot_ip> <task_name> [sim]"
              << std::endl;
    return 1;
  }

  bool simulator = false;
  if (argc == 4) {
    if (std::string(argv[3]) != "sim") {
      std::cerr << "The optional third argument must be 'sim'" << std::endl;
      return 1;
    }
    simulator = true;
  }

  try {
    lebai::l_master::Robot robot(argv[1], simulator);
    robot.start_sys();

    const auto task_id = robot.start_task(argv[2]);
    std::cout << "Started task " << task_id << std::endl;

    const auto print_result = robot.wait_task(task_id);
    std::cout << "Task print result:" << std::endl << print_result;
    if (print_result.empty() || print_result.back() != '\n') {
      std::cout << std::endl;
    }
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Example failed: " << e.what() << std::endl;
    return 2;
  }
}
