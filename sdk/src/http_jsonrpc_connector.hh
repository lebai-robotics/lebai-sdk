#pragma once

#include <httplib.h>
#include <jsonrpccxx/common.hpp>
#include <jsonrpccxx/iclientconnector.hpp>

#include <mutex>
#include <sstream>
#include <string>

namespace lebai {

class HttpJsonRpcConnector : public jsonrpccxx::IClientConnector {
 public:
  explicit HttpJsonRpcConnector(const std::string& host, int port)
      : http_client_(host, port) {
    http_client_.set_connection_timeout(5, 0);
    http_client_.set_read_timeout(5, 0);
    http_client_.set_write_timeout(5, 0);
  }

  auto Send(const std::string& request) -> std::string override {
    std::lock_guard<std::mutex> lock(mutex_);
    const auto response =
        http_client_.Post("/jsonrpc", request, "application/json");
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
      throw jsonrpccxx::JsonRpcException(
          -32003, message.str());
    }
    return response->body;
  }

 private:
  std::mutex mutex_;
  httplib::Client http_client_;
};

}  // namespace lebai
