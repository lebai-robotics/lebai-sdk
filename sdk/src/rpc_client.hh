#pragma once

#include <jsonrpccxx/client.hpp>
#include <jsonrpccxx/common.hpp>
#include <nlohmann/json.hpp>

#include <atomic>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "http_jsonrpc_connector.hh"

namespace lebai {

class RpcClient {
 public:
  explicit RpcClient(HttpJsonRpcConnector& connector)
      : client_(std::make_shared<jsonrpccxx::JsonRpcClient>(
            connector, jsonrpccxx::version::v2)) {}

  template <class Result = nlohmann::json>
  auto Call(const std::string& method,
            const std::vector<nlohmann::json>& args) -> Result {
    const int id = static_cast<int>(NextId());
    if constexpr (std::is_void_v<Result>) {
      client_->CallMethod<nlohmann::json>(id, method, args);
      return;
    } else {
      return client_->CallMethod<Result>(id, method, args);
    }
  }

  auto CallRaw(const std::string& method, const nlohmann::json& params)
      -> nlohmann::json {
    const int id = static_cast<int>(NextId());
    if (params.is_array()) {
      return client_->CallMethod<nlohmann::json>(
          id, method, params.get<std::vector<nlohmann::json>>());
    }
    return client_->CallMethod<nlohmann::json>(id, method, {params});
  }

 private:
  static auto NextId() -> uint32_t {
    static std::atomic<uint32_t> id(0);
    return ++id;
  }

  std::shared_ptr<jsonrpccxx::JsonRpcClient> client_;
};

}  // namespace lebai
