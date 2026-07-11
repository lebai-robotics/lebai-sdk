#pragma once

#include <httplib.h>
#include <jsonrpccxx/common.hpp>
#include <jsonrpccxx/iclientconnector.hpp>
#include <nlohmann/json.hpp>

#include <chrono>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace lebai {

struct HttpJsonRpcTimeouts {
  std::chrono::microseconds connect{std::chrono::seconds(5)};
  std::chrono::microseconds read{std::chrono::seconds(5)};
  std::chrono::microseconds write{std::chrono::seconds(5)};
  std::chrono::microseconds wait_read{std::chrono::hours(24)};
};

class HttpJsonRpcConnector : public jsonrpccxx::IClientConnector {
 public:
  explicit HttpJsonRpcConnector(
      const std::string& host, int port,
      HttpJsonRpcTimeouts timeouts = HttpJsonRpcTimeouts{})
      : host_(host), port_(port), timeouts_(timeouts) {
    ValidateTimeouts(timeouts_);
  }

  auto Send(const std::string& request) -> std::string override {
    httplib::Client http_client(host_, port_);
    const auto connect_timeout = TimeoutParts(timeouts_.connect);
    const auto read_timeout = TimeoutParts(
        IsWaitRequest(request) ? timeouts_.wait_read : timeouts_.read);
    const auto write_timeout = TimeoutParts(timeouts_.write);
    http_client.set_connection_timeout(connect_timeout.first,
                                       connect_timeout.second);
    http_client.set_read_timeout(read_timeout.first, read_timeout.second);
    http_client.set_write_timeout(write_timeout.first, write_timeout.second);

    const auto response =
        http_client.Post("/jsonrpc", request, "application/json");
    if (!response || response->status != 200) {
      std::ostringstream message;
      message << "client connector error";
      if (response) {
        message << ", status=" << response->status;
        if (!response->body.empty()) {
          message << ", body=" << response->body;
        }
      } else {
        message << ", no response, error="
                << httplib::to_string(response.error());
      }
      throw jsonrpccxx::JsonRpcException(-32003, message.str());
    }
    return response->body;
  }

 private:
  static void ValidateTimeout(const char* name,
                              std::chrono::microseconds timeout) {
    if (timeout < std::chrono::microseconds::zero()) {
      throw std::invalid_argument(std::string("HTTP JSON-RPC ") + name +
                                  " timeout must be non-negative");
    }
  }

  static void ValidateTimeouts(const HttpJsonRpcTimeouts& timeouts) {
    ValidateTimeout("connect", timeouts.connect);
    ValidateTimeout("read", timeouts.read);
    ValidateTimeout("write", timeouts.write);
    ValidateTimeout("wait_read", timeouts.wait_read);
  }

  static auto IsWaitRequest(const std::string& request) -> bool {
    const auto json = nlohmann::json::parse(request, nullptr, false);
    if (!json.is_object()) {
      return false;
    }

    const auto method = json.find("method");
    if (method == json.end() || !method->is_string()) {
      return false;
    }

    const auto& method_name = method->get_ref<const std::string&>();
    return method_name.compare(0, 5, "wait_") == 0;
  }

  static auto TimeoutParts(std::chrono::microseconds timeout)
      -> std::pair<time_t, time_t> {
    const auto seconds =
        std::chrono::duration_cast<std::chrono::seconds>(timeout);
    const auto microseconds = timeout - seconds;
    return {static_cast<time_t>(seconds.count()),
            static_cast<time_t>(microseconds.count())};
  }

  std::string host_;
  int port_;
  HttpJsonRpcTimeouts timeouts_;
};

}  // namespace lebai
