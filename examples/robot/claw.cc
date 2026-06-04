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
#include <stdexcept>
#include <string>

#include <lebai/robot.hh>

namespace {

double parse_double(const std::string &value, const std::string &name) {
  std::size_t parsed = 0;
  const double result = std::stod(value, &parsed);
  if (parsed != value.size()) {
    throw std::invalid_argument("Invalid " + name + ": " + value);
  }
  return result;
}

unsigned int parse_uint(const std::string &value, const std::string &name) {
  std::size_t parsed = 0;
  const auto result = std::stoul(value, &parsed);
  if (parsed != value.size()) {
    throw std::invalid_argument("Invalid " + name + ": " + value);
  }
  return static_cast<unsigned int>(result);
}

void print_usage(const char *program) {
  std::cerr << "Usage:\n"
            << "  " << program << " <robot-ip> [sim]\n"
            << "  " << program << " <robot-ip> [sim] --init\n"
            << "  " << program << " <robot-ip> [sim] --force-init\n"
            << "  " << program << " <robot-ip> [sim] --set <force> <amplitude>\n"
            << "  " << program << " <robot-ip> [sim] --ao <address> <value>\n"
            << "  " << program
            << " <robot-ip> [sim] --wait-ai <address> <value> <relation>\n";
}

}  // namespace

int main(int argc, char **argv) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  const std::string ip = argv[1];
  bool sim = false;
  bool should_init_claw = false;
  bool force_init_claw = false;
  bool should_set_claw = false;
  bool should_set_ao = false;
  bool should_wait_ai = false;
  double force = 0.0;
  double amplitude = 0.0;
  unsigned int ao_address = 0;
  double ao_value = 0.0;
  unsigned int wait_ai_address = 0;
  double wait_ai_value = 0.0;
  std::string wait_ai_relation = "GTE";

  try {
    for (int i = 2; i < argc; ++i) {
      const std::string arg = argv[i];
      if (arg == "sim") {
        sim = true;
      } else if (arg == "--init") {
        should_init_claw = true;
      } else if (arg == "--force-init") {
        should_init_claw = true;
        force_init_claw = true;
      } else if (arg == "--set") {
        if (i + 2 >= argc) {
          throw std::invalid_argument("--set requires <force> <amplitude>");
        }
        force = parse_double(argv[++i], "force");
        amplitude = parse_double(argv[++i], "amplitude");
        should_set_claw = true;
      } else if (arg == "--ao") {
        if (i + 2 >= argc) {
          throw std::invalid_argument("--ao requires <address> <value>");
        }
        ao_address = parse_uint(argv[++i], "address");
        ao_value = parse_double(argv[++i], "value");
        should_set_ao = true;
      } else if (arg == "--wait-ai") {
        if (i + 3 >= argc) {
          throw std::invalid_argument(
              "--wait-ai requires <address> <value> <relation>");
        }
        wait_ai_address = parse_uint(argv[++i], "address");
        wait_ai_value = parse_double(argv[++i], "value");
        wait_ai_relation = argv[++i];
        should_wait_ai = true;
      } else {
        throw std::invalid_argument("Unknown argument: " + arg);
      }
    }

    lebai::l_master::Robot robot(ip, sim);
    std::cout << "Lebai SDK version: " << lebai::l_master::version()
              << std::endl;
    std::cout << "Connecting to robot at " << ip << std::endl;
    std::cout << "Simulation mode: " << (sim ? "true" : "false")
              << std::endl;

    if (should_init_claw) {
      robot.init_claw(force_init_claw);
      std::cout << "init_claw: force_initialization="
                << (force_init_claw ? "true" : "false") << std::endl;
    }

    if (should_set_claw) {
      robot.set_claw(force, amplitude);
      std::cout << "set_claw: force=" << force
                << ", amplitude=" << amplitude << std::endl;
    }

    if (should_set_ao) {
      robot.set_claw_ao(ao_address, ao_value);
      std::cout << "set_claw_ao: address=" << ao_address
                << ", value=" << ao_value << std::endl;
    }

    if (should_wait_ai) {
      robot.wait_claw_ai(wait_ai_address, wait_ai_value, wait_ai_relation);
      std::cout << "wait_claw_ai: address=" << wait_ai_address
                << ", value=" << wait_ai_value
                << ", relation=" << wait_ai_relation << std::endl;
    }

    const auto claw = robot.get_claw();
    std::cout << "claw: force=" << claw.force
              << ", amplitude=" << claw.amplitude
              << ", hold_on=" << (claw.hold_on ? "true" : "false")
              << std::endl;

    const auto ai0 = robot.get_claw_ai(0);
    std::cout << "claw ai[0]: " << ai0 << std::endl;
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Claw example failed: " << e.what() << std::endl;
    return 2;
  }
}
