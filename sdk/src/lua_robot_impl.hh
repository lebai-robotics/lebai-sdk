/**
 * Copyright 2022-2023 lebai.ltd
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
#include <lebai/lua_robot.hh>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>

namespace lebai {
namespace l_master {
class LuaRobot::LuaRobotImpl {
 public:
  struct Config {
    uint16_t port{5180};
    std::chrono::milliseconds timeout{1000};
    std::size_t max_response_bytes{1000};
  };

  explicit LuaRobotImpl(const std::string& ip);
  LuaRobotImpl(const std::string& ip, Config config);
  virtual ~LuaRobotImpl() noexcept;

  void send(const std::string& lua_code);
  std::string call(const std::string& lua_code);

 private:
  void connect(const std::string& ip);

  Config config_;
  asio::io_context io_context_;
  asio::ip::tcp::resolver resolver_;
  asio::ip::tcp::socket socket_;
  asio::streambuf response_buffer_;
};
}  // namespace l_master
}  // namespace lebai
