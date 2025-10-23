/**
 * Copyright 2025 lebai.ltd
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

#include "gripper_impl.hh"
#include "Modbus.h"
#include "ModbusClient.h"
#include "ModbusClientPort.h"
#include "ModbusPort.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace lebai {
	namespace l_master {

		Gripper::GripperImpl::GripperImpl(const std::string& port_name)
			: port_name_(port_name), port_(nullptr) {
			try {
				// Configure serial port parameters for Modbus RTU
				Modbus::SerialSettings settings;
				settings.portName = port_name_.c_str();
				settings.baudRate = baud_rate_;
				settings.dataBits = 8;
				settings.parity = Modbus::NoParity;
				settings.stopBits = Modbus::OneStop;
				settings.flowControl = Modbus::NoFlowControl;
				settings.timeoutFirstByte =
					5000;  // 5 seconds (increased for slower devices)
				settings.timeoutInterByte =
					100;  // 100 ms (increased for RTU frame completion)

				// Create Modbus RTU client port with blocking mode
				const bool blocking = true;
				port_ = Modbus::createClientPort(Modbus::RTU, &settings, blocking);

				if (!port_) {
					throw std::runtime_error("Failed to create Modbus RTU client port");
				}

				// Get the underlying port and open it explicitly
				ModbusPort* underlying_port = port_->port();
				if (underlying_port) {
					Modbus::StatusCode status = underlying_port->open();
					if (status != Modbus::Status_Good) {
						throw std::runtime_error(
							"Failed to open serial port: " + port_name_ +
							", Status: " + std::to_string(static_cast<int>(status)));
					}

					// Give the device time to initialize after port opening
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}

				// Create Modbus client with the specified unit address
				client_ = std::make_unique<ModbusClient>(modbus_address_, port_);

				std::cout << "Gripper initialized on " << port_name_
					<< " (Unit: " << static_cast<int>(modbus_address_)
					<< ", Baud: " << baud_rate_ << ")" << std::endl;

			}
			catch (const std::exception& e) {
				// Clean up if initialization fails
				if (port_) {
					delete port_;
					port_ = nullptr;
				}
				throw std::runtime_error("Failed to initialize gripper: " +
					std::string(e.what()));
			}
		}

		Gripper::GripperImpl::~GripperImpl() {
			// Clean up the client first
			client_.reset();

			// Clean up the port
			if (port_) {
				ModbusPort* underlying_port = port_->port();
				if (underlying_port && underlying_port->isOpen()) {
					underlying_port->close();
				}
				delete port_;
				port_ = nullptr;
			}
		}

		void Gripper::GripperImpl::SetPosition(unsigned int position) {
			if (position > 100) {
				throw std::invalid_argument("Position must be between 0 and 100");
			}

			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			// Check if port is open
			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}
			try {
				// Write to holding register 40001 (address 0 in Modbus)
				// In Modbus, address 40001 corresponds to holding register address 0
				uint16_t value = static_cast<uint16_t>(position);

				// Write to register at address 0x9C40 using Write Multiple Registers (FC 0x10)
				// This will generate frame: 01 10 9C 40 00 01 02 [value_bytes] [CRC]
				uint16_t values[1] = { value };
				Modbus::StatusCode status = client_->writeMultipleRegisters(0x9C40, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to write amplitude to gripper. Status: " +
						std::to_string(static_cast<int>(status));

					// Add more detailed error information
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to set position: " +
					std::string(e.what()));
			}
		}



		unsigned int Gripper::GripperImpl::GetCurrentPosition() const {
			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			// Check if port is open
			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}

			// 40005（0x9C45） 夹爪当前位置(可读) 0 - 100
			try {
				uint16_t values[1] = { 0 };
				Modbus::StatusCode status = client_->readHoldingRegisters(0x9C45, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to read current position from gripper. Status: " +
						std::to_string(static_cast<int>(status));
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}

				unsigned int pos = static_cast<unsigned int>(values[0]);
				if (pos > 100)
				{
					pos = 100;  // 设备声明范围为 0-100，超出则钳制
				}
				return pos;
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to get current position: " + std::string(e.what()));
			}
		}


		void Gripper::GripperImpl::SetForce(unsigned int force) {
			if (force > 100) {
				throw std::invalid_argument("Force must be between 0 and 100");
			}

			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			// Check if port is open
			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}
			try {
				// Write to holding register 40001 (address 0 in Modbus)
				// In Modbus, address 40001 corresponds to holding register address 0
				uint16_t value = static_cast<uint16_t>(force);

				uint16_t values[1] = { value };
				Modbus::StatusCode status = client_->writeMultipleRegisters(0x9C41, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to write force to gripper. Status: " +
						std::to_string(static_cast<int>(status));

					// Add more detailed error information
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to set force: " +
					std::string(e.what()));
			}
		}

		unsigned int Gripper::GripperImpl::GetCurrentForce() const
		{
			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}
			// Check if port is open
			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}
			// 40002（0x9C42） 夹爪当前力道(可读) 0 - 100
			try {
				uint16_t values[1] = { 0 };
				Modbus::StatusCode status = client_->readHoldingRegisters(0x9C42, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to read current force from gripper. Status: " +
						std::to_string(static_cast<int>(status));
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}
				unsigned int force = static_cast<unsigned int>(values[0]);
				if (force > 100)
				{
					force = 100;  // 设备声明范围为 0-100，超出则钳制
				}
				return force;
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to get current force: " + std::string(e.what()));
			}
		}

		void Gripper::GripperImpl::SetVelocity(unsigned int velocity, bool persistent)
		{
			//40010 (0x9C4A) 设置夹爪开合速度(读写) 0 - 100
			//40011（0x9C4B） 保存夹爪开合速度，断电保留（读写） 0 - 100
			if (velocity > 100) {
				throw std::invalid_argument("Velocity must be between 0 and 100");
			}

			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}

			try {
				const uint16_t reg = persistent ? 0x9C4B : 0x9C4A;
				uint16_t values[1] = { static_cast<uint16_t>(velocity) };
				Modbus::StatusCode status = client_->writeMultipleRegisters(reg, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to write velocity to gripper. Status: " +
						std::to_string(static_cast<int>(status));
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to set velocity: " + std::string(e.what()));
			}
		}

		unsigned int Gripper::GripperImpl::GetCurrentVelocity(bool persistent) const
		{
			//40010 (0x9C4A) 设置夹爪开合速度(读写) 0 - 100
			//40011（0x9C4B） 保存夹爪开合速度，断电保留（读写） 0 - 100
			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}

			try {
				const uint16_t reg = persistent ? 0x9C4B : 0x9C4A;
				uint16_t values[1] = { 0 };
				Modbus::StatusCode status = client_->readHoldingRegisters(reg, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to read current velocity from gripper. Status: " +
						std::to_string(static_cast<int>(status));
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}

				unsigned int velocity = static_cast<unsigned int>(values[0]);
				if (velocity > 100) {
					velocity = 100; // 设备声明范围为 0-100，超出则钳制
				}
				return velocity;
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to get current velocity: " + std::string(e.what()));
			}
		}

		void Gripper::GripperImpl::DoCalibration()
		{
			//40008（0x9C48） 找行程指令（可写） 1
			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}

			try {
				uint16_t values[1] = { 1 };
				Modbus::StatusCode status = client_->writeMultipleRegisters(0x9C48, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to write calibration command to gripper. Status: " +
						std::to_string(static_cast<int>(status));
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to do calibration: " + std::string(e.what()));
			}
		}

		bool Gripper::GripperImpl::IsCalibrated() const
		{
			// 40009 (0x9C49) 夹爪未找行程（可读） 1 未找行程 0 找行程完成
			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}

			try {
				uint16_t values[1] = { 0 };
				Modbus::StatusCode status = client_->readHoldingRegisters(0x9C49, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to read calibration state from gripper. Status: " +
						std::to_string(static_cast<int>(status));
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}

				// 按协议：1=未找行程，0=找行程完成
				const uint16_t flag = static_cast<uint16_t>(values[0] & 0x0001);
				return (flag == 0);
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to get calibration state: " + std::string(e.what()));
			}
		}

		void Gripper::GripperImpl::TurnOnAutoCalibration()
		{
			//40090 (0x9C9A) 关闭自动找行程指令(可写) 2 关闭自动找行程且断电保存 3 恢复自动找行程且断电保存
			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}

			try {
				const uint16_t reg = 40090;
				uint16_t values[1] = { 3 };
				Modbus::StatusCode status = client_->writeMultipleRegisters(reg, 1, values);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg = "Failed to enable auto calibration setting on gripper. Status: " +
						std::to_string(static_cast<int>(status));
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to turn on auto calibration: " + std::string(e.what()));
			}
		}
		void Gripper::GripperImpl::TurnOffAutoCalibration()
		{
			//40090 (0x9C9A) 关闭自动找行程指令(可写) 2 关闭自动找行程且断电保存 3 恢复自动找行程且断电保存
			if (!client_ || !port_) {
				throw std::runtime_error("Modbus client is not initialized");
			}

			ModbusPort* underlying_port = port_->port();
			if (!underlying_port || !underlying_port->isOpen()) {
				throw std::runtime_error("Modbus port is not open");
			}

			try {
				const uint16_t reg = 40090;
				uint16_t value = 0x03;
				Modbus::StatusCode status = client_->writeSingleRegister(reg, value);
				if (!Modbus::StatusIsGood(status)) {
					std::string errorMsg;
					std::ostringstream oss;
					oss << "Failed to disable auto calibration setting on gripper. Status: 0x"
						<< std::setfill('0') << std::setw(2)  // optional: pad to 2 digits
						<< std::hex << std::uppercase
						<< static_cast<int>(status);
					errorMsg = oss.str();
					if (status == (Modbus::Status_Bad | 0x102)) {
						errorMsg += " (RTU frame error - check baud rate and device settings)";
					}
					errorMsg += ", Error: ";
					errorMsg += port_->lastErrorText();
					throw std::runtime_error(errorMsg);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Failed to turn off auto calibration: " + std::string(e.what()));
			}
		}



	}  // namespace l_master
}  // namespace lebai