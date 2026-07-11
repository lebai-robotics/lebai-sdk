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

#include "lua_robot_impl.hh"

#include <stdexcept>
#include <string>
#include <utility>

#include "timed_operation.hh"

namespace lebai {
namespace l_master {

namespace {

LuaRobot::LuaRobotImpl::Config validate_config(
    LuaRobot::LuaRobotImpl::Config config) {
  if (config.timeout <= std::chrono::milliseconds::zero()) {
    throw std::invalid_argument("Lua robot timeout must be positive");
  }
  if (config.max_response_bytes < 2) {
    throw std::invalid_argument(
        "Lua robot maximum response size must be at least 2 bytes");
  }
  return config;
}

std::runtime_error transport_error(const char* operation,
                                   const std::error_code& error) {
  return std::runtime_error(std::string("Lua robot ") + operation +
                            " failed: " + error.message());
}

}  // namespace

LuaRobot::LuaRobotImpl::LuaRobotImpl(const std::string& ip)
    : LuaRobotImpl(ip, Config{}) {}

LuaRobot::LuaRobotImpl::LuaRobotImpl(const std::string& ip, Config config)
    : config_(validate_config(std::move(config))),
      resolver_(io_context_),
      socket_(io_context_),
      response_buffer_(config_.max_response_bytes) {
  connect(ip);
}

LuaRobot::LuaRobotImpl::~LuaRobotImpl() noexcept {
  try {
    resolver_.cancel();
  } catch (...) {
  }
  std::error_code ignored;
  socket_.close(ignored);
}

void LuaRobot::LuaRobotImpl::connect(const std::string& ip) {
  asio::ip::tcp::resolver::results_type endpoints;
  const auto service = std::to_string(config_.port);
  const auto resolve_result = run_timed_operation(
      io_context_, config_.timeout,
      [this, &ip, &service, &endpoints](auto complete) {
        resolver_.async_resolve(
            ip, service,
            [&endpoints, complete](
                const std::error_code& error,
                asio::ip::tcp::resolver::results_type result) {
              if (!error) {
                endpoints = std::move(result);
              }
              complete(error);
            });
      },
      [this] { resolver_.cancel(); });

  if (resolve_result.timed_out) {
    throw std::runtime_error("Lua robot resolve timed out");
  }
  if (resolve_result.error) {
    throw transport_error("resolve", resolve_result.error);
  }

  const auto connect_result = run_timed_operation(
      io_context_, config_.timeout,
      [this, &endpoints](auto complete) {
        asio::async_connect(
            socket_, endpoints,
            [complete](const std::error_code& error,
                       const asio::ip::tcp::endpoint&) { complete(error); });
      },
      [this] {
        std::error_code ignored;
        socket_.close(ignored);
      });

  if (connect_result.timed_out) {
    throw std::runtime_error("Lua robot connect timed out");
  }
  if (connect_result.error) {
    throw transport_error("connect", connect_result.error);
  }
}

void LuaRobot::LuaRobotImpl::send(const std::string& lua_code) {
  std::error_code error;
  asio::write(socket_, asio::buffer(lua_code), error);
  if (error) {
    throw transport_error("send", error);
  }
}

std::string LuaRobot::LuaRobotImpl::call(const std::string& lua_code) {
  send("print(" + lua_code + ")");

  std::size_t bytes_transferred = 0;
  TimedOperationResult read_result;
  try {
    read_result = run_timed_operation(
        io_context_, config_.timeout,
        [this, &bytes_transferred](auto complete) {
          asio::async_read_until(
              socket_, response_buffer_, "\r\n",
              [&bytes_transferred, complete](const std::error_code& error,
                                             std::size_t bytes) {
                bytes_transferred = bytes;
                complete(error);
              });
        },
        [this] {
          std::error_code ignored;
          socket_.close(ignored);
        });
  } catch (const std::length_error&) {
    throw std::runtime_error("Lua robot maximum response size exceeded");
  }

  if (read_result.timed_out) {
    throw std::runtime_error("Lua robot response timed out");
  }
  if (read_result.error == asio::error::not_found) {
    throw std::runtime_error("Lua robot maximum response size exceeded");
  }
  if (read_result.error == asio::error::eof) {
    throw std::runtime_error("Lua robot incomplete response before EOF");
  }
  if (read_result.error) {
    throw transport_error("response read", read_result.error);
  }

  std::string response(bytes_transferred, '\0');
  if (!response.empty()) {
    asio::buffer_copy(asio::buffer(response.data(), response.size()),
                      response_buffer_.data(), response.size());
  }
  response_buffer_.consume(bytes_transferred);

  if (response.size() < 2 ||
      response.compare(response.size() - 2, 2, "\r\n") != 0) {
    throw std::runtime_error("Lua robot response has invalid framing");
  }
  response.resize(response.size() - 2);
  return response;
}

}  // namespace l_master
}  // namespace lebai
