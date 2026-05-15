/**
 * Copyright 2026 lebai.ltd
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

#include "modbus_rtu_client.hh"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace lebai {
namespace l_master {
namespace {

constexpr uint8_t kReadHoldingRegisters = 0x03;
constexpr uint8_t kWriteSingleRegister = 0x06;
constexpr uint8_t kWriteMultipleRegisters = 0x10;
constexpr uint16_t kCrcInitialValue = 0xFFFF;

uint16_t read_u16_be(const std::vector<uint8_t>& frame, std::size_t offset) {
  if (offset + 1 >= frame.size()) {
    throw std::runtime_error("Modbus RTU frame is too short");
  }
  return static_cast<uint16_t>((static_cast<uint16_t>(frame[offset]) << 8) |
                               frame[offset + 1]);
}

void append_u16_be(std::vector<uint8_t>& frame, uint16_t value) {
  frame.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
  frame.push_back(static_cast<uint8_t>(value & 0xFF));
}

std::string hex_byte(uint8_t value) {
  std::ostringstream oss;
  oss << "0x" << std::uppercase << std::hex << std::setw(2)
      << std::setfill('0') << static_cast<unsigned int>(value);
  return oss.str();
}

std::string exception_message(uint8_t code) {
  switch (code) {
    case 0x01:
      return "illegal function";
    case 0x02:
      return "illegal data address";
    case 0x03:
      return "illegal data value";
    case 0x04:
      return "server device failure";
    default:
      return "exception code " + hex_byte(code);
  }
}

void ensure_slave_and_function(const std::vector<uint8_t>& frame,
                               uint8_t slave_id, uint8_t function_code) {
  if (frame.size() < 4) {
    throw std::runtime_error("Modbus RTU frame is too short");
  }
  if (frame[0] != slave_id) {
    throw std::runtime_error("Unexpected Modbus RTU slave id: " +
                             std::to_string(frame[0]));
  }
  if (frame[1] != function_code) {
    throw std::runtime_error("Unexpected Modbus RTU function code: " +
                             hex_byte(frame[1]));
  }
}

asio::serial_port_base::parity parity_option(ModbusRtuClient::Parity parity) {
  switch (parity) {
    case ModbusRtuClient::Parity::Even:
      return asio::serial_port_base::parity(
          asio::serial_port_base::parity::even);
    case ModbusRtuClient::Parity::Odd:
      return asio::serial_port_base::parity(
          asio::serial_port_base::parity::odd);
    case ModbusRtuClient::Parity::None:
    default:
      return asio::serial_port_base::parity(
          asio::serial_port_base::parity::none);
  }
}

}  // namespace

ModbusRtuClient::ModbusRtuClient(Config config)
    : config_(std::move(config)), serial_(io_), timer_(io_) {
  open_serial_port();
}

ModbusRtuClient::~ModbusRtuClient() {
  std::error_code ignored;
  if (serial_.is_open()) {
    serial_.cancel(ignored);
    serial_.close(ignored);
  }
}

std::vector<uint16_t> ModbusRtuClient::read_holding_registers(
    uint16_t address, uint16_t count) {
  const auto request = build_read_holding_registers_request(
      config_.slave_id, address, count);
  return parse_read_holding_registers_response(transact(request),
                                               config_.slave_id, count);
}

void ModbusRtuClient::write_single_register(uint16_t address, uint16_t value) {
  const auto request = build_write_single_register_request(config_.slave_id,
                                                           address, value);
  validate_write_registers_response(transact(request), config_.slave_id,
                                    kWriteSingleRegister, address, value);
}

void ModbusRtuClient::write_multiple_registers(
    uint16_t address, const std::vector<uint16_t>& values) {
  const auto request =
      build_write_multiple_registers_request(config_.slave_id, address, values);
  validate_write_registers_response(transact(request), config_.slave_id,
                                    kWriteMultipleRegisters, address,
                                    static_cast<uint16_t>(values.size()));
}

uint16_t ModbusRtuClient::crc16(const std::vector<uint8_t>& data) {
  uint16_t crc = kCrcInitialValue;
  for (const auto byte : data) {
    crc ^= byte;
    for (int bit = 0; bit < 8; ++bit) {
      if ((crc & 0x0001) != 0) {
        crc = static_cast<uint16_t>((crc >> 1) ^ 0xA001);
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}

void ModbusRtuClient::append_crc(std::vector<uint8_t>& frame) {
  const uint16_t crc = crc16(frame);
  frame.push_back(static_cast<uint8_t>(crc & 0xFF));
  frame.push_back(static_cast<uint8_t>((crc >> 8) & 0xFF));
}

void ModbusRtuClient::validate_crc(const std::vector<uint8_t>& frame) {
  if (frame.size() < 4) {
    throw std::runtime_error("Modbus RTU frame is too short for CRC");
  }
  const std::vector<uint8_t> body(frame.begin(), frame.end() - 2);
  const uint16_t actual =
      static_cast<uint16_t>(frame[frame.size() - 2] |
                            (static_cast<uint16_t>(frame.back()) << 8));
  const uint16_t expected = crc16(body);
  if (actual != expected) {
    std::ostringstream oss;
    oss << "Invalid Modbus RTU CRC: expected 0x" << std::hex << std::uppercase
        << expected << ", got 0x" << actual;
    throw std::runtime_error(oss.str());
  }
}

void ModbusRtuClient::throw_if_exception_response(
    const std::vector<uint8_t>& frame) {
  if (frame.size() >= 5 && (frame[1] & 0x80) != 0) {
    validate_crc(frame);
    throw std::runtime_error("Modbus RTU exception response: " +
                             exception_message(frame[2]));
  }
}

std::vector<uint8_t> ModbusRtuClient::build_read_holding_registers_request(
    uint8_t slave_id, uint16_t address, uint16_t count) {
  if (count == 0 || count > 125) {
    throw std::invalid_argument(
        "Read holding registers count must be between 1 and 125");
  }
  std::vector<uint8_t> frame = {slave_id, kReadHoldingRegisters};
  append_u16_be(frame, address);
  append_u16_be(frame, count);
  append_crc(frame);
  return frame;
}

std::vector<uint8_t> ModbusRtuClient::build_write_single_register_request(
    uint8_t slave_id, uint16_t address, uint16_t value) {
  std::vector<uint8_t> frame = {slave_id, kWriteSingleRegister};
  append_u16_be(frame, address);
  append_u16_be(frame, value);
  append_crc(frame);
  return frame;
}

std::vector<uint8_t>
ModbusRtuClient::build_write_multiple_registers_request(
    uint8_t slave_id, uint16_t address,
    const std::vector<uint16_t>& values) {
  if (values.empty() || values.size() > 123) {
    throw std::invalid_argument(
        "Write multiple registers count must be between 1 and 123");
  }

  std::vector<uint8_t> frame = {slave_id, kWriteMultipleRegisters};
  append_u16_be(frame, address);
  append_u16_be(frame, static_cast<uint16_t>(values.size()));
  frame.push_back(static_cast<uint8_t>(values.size() * 2));
  for (const auto value : values) {
    append_u16_be(frame, value);
  }
  append_crc(frame);
  return frame;
}

std::vector<uint16_t> ModbusRtuClient::parse_read_holding_registers_response(
    const std::vector<uint8_t>& frame, uint8_t slave_id,
    uint16_t expected_count) {
  validate_crc(frame);
  throw_if_exception_response(frame);
  ensure_slave_and_function(frame, slave_id, kReadHoldingRegisters);

  const std::size_t expected_byte_count =
      static_cast<std::size_t>(expected_count) * 2;
  if (frame.size() != expected_byte_count + 5) {
    throw std::runtime_error("Unexpected Modbus RTU read response length");
  }
  if (frame[2] != expected_byte_count) {
    throw std::runtime_error("Unexpected Modbus RTU read byte count");
  }

  std::vector<uint16_t> values;
  values.reserve(expected_count);
  for (std::size_t offset = 3; offset < 3 + expected_byte_count; offset += 2) {
    values.push_back(read_u16_be(frame, offset));
  }
  return values;
}

void ModbusRtuClient::validate_write_registers_response(
    const std::vector<uint8_t>& frame, uint8_t slave_id,
    uint8_t function_code, uint16_t address, uint16_t count_or_value) {
  validate_crc(frame);
  throw_if_exception_response(frame);
  ensure_slave_and_function(frame, slave_id, function_code);

  if (frame.size() != 8) {
    throw std::runtime_error("Unexpected Modbus RTU write response length");
  }
  if (read_u16_be(frame, 2) != address) {
    throw std::runtime_error("Unexpected Modbus RTU write response address");
  }
  if (read_u16_be(frame, 4) != count_or_value) {
    throw std::runtime_error("Unexpected Modbus RTU write response value");
  }
}

void ModbusRtuClient::open_serial_port() {
  if (config_.port_name.empty()) {
    throw std::invalid_argument("Serial port name is empty");
  }

  std::error_code ec;
  serial_.open(config_.port_name, ec);
  if (ec) {
    throw std::runtime_error("Failed to open serial port " + config_.port_name +
                             ": " + ec.message());
  }

  serial_.set_option(asio::serial_port_base::baud_rate(config_.baud_rate), ec);
  if (ec) {
    throw std::runtime_error("Failed to set serial baud rate: " + ec.message());
  }
  serial_.set_option(asio::serial_port_base::character_size(8), ec);
  if (ec) {
    throw std::runtime_error("Failed to set serial character size: " +
                             ec.message());
  }
  serial_.set_option(parity_option(config_.parity), ec);
  if (ec) {
    throw std::runtime_error("Failed to set serial parity: " + ec.message());
  }
  serial_.set_option(asio::serial_port_base::stop_bits(
                         asio::serial_port_base::stop_bits::one),
                     ec);
  if (ec) {
    throw std::runtime_error("Failed to set serial stop bits: " + ec.message());
  }
  serial_.set_option(asio::serial_port_base::flow_control(
                         asio::serial_port_base::flow_control::none),
                     ec);
  if (ec) {
    throw std::runtime_error("Failed to set serial flow control: " +
                             ec.message());
  }
}

std::vector<uint8_t> ModbusRtuClient::transact(
    const std::vector<uint8_t>& request) {
  std::error_code ec;
  asio::write(serial_, asio::buffer(request), ec);
  if (ec) {
    throw std::runtime_error("Failed to write Modbus RTU request: " +
                             ec.message());
  }

  std::vector<uint8_t> frame = read_exact(2);
  if (frame[0] != config_.slave_id) {
    throw std::runtime_error("Unexpected Modbus RTU slave id: " +
                             std::to_string(frame[0]));
  }

  if ((frame[1] & 0x80) != 0) {
    const auto rest = read_exact(3);
    frame.insert(frame.end(), rest.begin(), rest.end());
    throw_if_exception_response(frame);
    return frame;
  }

  if (request.size() < 2 || frame[1] != request[1]) {
    throw std::runtime_error("Unexpected Modbus RTU function code: " +
                             hex_byte(frame[1]));
  }

  if (frame[1] == kReadHoldingRegisters) {
    const auto byte_count_frame = read_exact(1);
    frame.insert(frame.end(), byte_count_frame.begin(), byte_count_frame.end());
    const auto rest = read_exact(static_cast<std::size_t>(frame[2]) + 2);
    frame.insert(frame.end(), rest.begin(), rest.end());
  } else if (frame[1] == kWriteSingleRegister ||
             frame[1] == kWriteMultipleRegisters) {
    const auto rest = read_exact(6);
    frame.insert(frame.end(), rest.begin(), rest.end());
  } else {
    throw std::runtime_error("Unsupported Modbus RTU function code: " +
                             hex_byte(frame[1]));
  }

  validate_crc(frame);
  return frame;
}

std::vector<uint8_t> ModbusRtuClient::read_exact(std::size_t size) {
  std::vector<uint8_t> buffer(size);
  std::error_code read_error;
  bool done = false;
  bool timed_out = false;

  io_.restart();
  asio::async_read(serial_, asio::buffer(buffer),
                   [&](const std::error_code& ec, std::size_t) {
                     read_error = ec;
                     done = true;
                     std::error_code ignored;
                     timer_.cancel(ignored);
                   });

  timer_.expires_after(config_.timeout);
  timer_.async_wait([&](const std::error_code& ec) {
    if (!ec && !done) {
      timed_out = true;
      std::error_code ignored;
      serial_.cancel(ignored);
    }
  });

  io_.run();

  if (timed_out) {
    throw std::runtime_error("Timed out reading Modbus RTU response");
  }
  if (read_error) {
    throw std::runtime_error("Failed to read Modbus RTU response: " +
                             read_error.message());
  }
  return buffer;
}

}  // namespace l_master
}  // namespace lebai
