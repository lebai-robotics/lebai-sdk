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

#pragma once

#include <asio.hpp>

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace lebai {
namespace l_master {

class ModbusRtuClient {
 public:
  enum class Parity { None, Even, Odd };

  struct Config {
    std::string port_name;
    unsigned int baud_rate = 115200;
    uint8_t slave_id = 1;
    std::chrono::milliseconds timeout = std::chrono::milliseconds(5000);
    Parity parity = Parity::None;
  };

  explicit ModbusRtuClient(Config config);
  ~ModbusRtuClient();

  ModbusRtuClient(const ModbusRtuClient&) = delete;
  ModbusRtuClient& operator=(const ModbusRtuClient&) = delete;

  std::vector<uint16_t> read_holding_registers(uint16_t address,
                                               uint16_t count);
  void write_single_register(uint16_t address, uint16_t value);
  void write_multiple_registers(uint16_t address,
                                const std::vector<uint16_t>& values);

  static uint16_t crc16(const std::vector<uint8_t>& data);
  static void append_crc(std::vector<uint8_t>& frame);
  static void validate_crc(const std::vector<uint8_t>& frame);
  static void throw_if_exception_response(const std::vector<uint8_t>& frame);

  static std::vector<uint8_t> build_read_holding_registers_request(
      uint8_t slave_id, uint16_t address, uint16_t count);
  static std::vector<uint8_t> build_write_single_register_request(
      uint8_t slave_id, uint16_t address, uint16_t value);
  static std::vector<uint8_t> build_write_multiple_registers_request(
      uint8_t slave_id, uint16_t address, const std::vector<uint16_t>& values);
  static std::vector<uint16_t> parse_read_holding_registers_response(
      const std::vector<uint8_t>& frame, uint8_t slave_id,
      uint16_t expected_count);
  static void validate_write_registers_response(
      const std::vector<uint8_t>& frame, uint8_t slave_id,
      uint8_t function_code, uint16_t address, uint16_t count_or_value);

 private:
  void open_serial_port();
  std::vector<uint8_t> transact(const std::vector<uint8_t>& request);
  std::vector<uint8_t> read_exact(std::size_t size);

  Config config_;
  asio::io_context io_;
  asio::serial_port serial_;
  asio::steady_timer timer_;
};

}  // namespace l_master
}  // namespace lebai
