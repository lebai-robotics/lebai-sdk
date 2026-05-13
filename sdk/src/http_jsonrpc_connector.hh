#pragma once

#include <httplib.h>
#include <jsonrpccxx/common.hpp>
#include <jsonrpccxx/iclientconnector.hpp>

#include <sstream>
#include <string>

namespace lebai {

class HttpJsonRpcConnector : public jsonrpccxx::IClientConnector {
 public:
  explicit HttpJsonRpcConnector(const std::string& host, int port)
      : http_client_(host, port) {}

  auto Send(const std::string& request) -> std::string override {
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
        message << ", no response";
      }
      throw jsonrpccxx::JsonRpcException(
          -32003, message.str());
    }
    return response->body;
  }

 private:
  httplib::Client http_client_;
};

}  // namespace lebai
