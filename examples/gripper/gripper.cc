/**
 * Copyright 2022-2025 lebai.ltd
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
#include <memory>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "lebai/gripper.hh"

int main(int argc, char* argv[]) {
	// Check command line arguments
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <serial_port>" << std::endl;
		std::cerr << "Example (Windows): " << argv[0] << " COM3" << std::endl;
		std::cerr << "Example (Linux): " << argv[0] << " /dev/ttyUSB0" << std::endl;
		return 1;
	}

	std::string port_name = argv[1];

	try {
		std::cout << "Connecting to gripper on port: " << port_name << std::endl;

		// Create gripper instance
		// This will establish Modbus RTU communication over RS485
		// with 115200 baud rate, 8N1, Modbus address 1
		auto gripper = std::make_unique<lebai::l_master::Gripper>(port_name);

		std::cout << "Gripper connected successfully!" << std::endl;
		gripper->set_position(30);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		gripper->set_position(70);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		gripper->do_calibration();
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "Unknown error occurred" << std::endl;
		return 1;
	}

	return 0;
}