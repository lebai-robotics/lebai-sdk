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
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include "lebai/gripper.hh"

int main(int argc, char* argv[]) {
	// Check command line arguments
	if (argc < 2 || argc > 3) {
		std::cerr << "Usage: " << argv[0] << " <serial_port> [position]" << std::endl;
		std::cerr << "Example (Windows): " << argv[0] << " COM3 50" << std::endl;
		std::cerr << "Example (Linux): " << argv[0] << " /dev/ttyUSB0 50" << std::endl;
		return 1;
	}

	std::string port_name = argv[1];
	unsigned int position = 50;
	if (argc == 3) {
		try {
			size_t parsed = 0;
			unsigned long value = std::stoul(argv[2], &parsed, 10);
			if (argv[2][0] == '-' || parsed != std::strlen(argv[2]) ||
			    value > 100) {
				throw std::invalid_argument("trailing characters");
			}
			position = static_cast<unsigned int>(value);
		}
		catch (const std::exception&) {
			std::cerr << "Invalid position: " << argv[2] << std::endl;
			return 1;
		}
	}

	try {
		std::cout << "Connecting to gripper on port: " << port_name << std::endl;

		// Create gripper instance
		// This will establish Modbus RTU communication over RS485
		// with 115200 baud rate, 8N1, Modbus address 1
		auto gripper = std::make_unique<lebai::l_master::Gripper>(port_name);

		std::cout << "Gripper connected successfully!" << std::endl;
		std::cout << "Setting gripper position to: " << position << std::endl;
		gripper->set_position(position);
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
