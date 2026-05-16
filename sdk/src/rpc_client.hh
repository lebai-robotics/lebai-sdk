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
    const int id = NextId();
    if constexpr (std::is_void_v<Result>) {
      client_->CallMethod<nlohmann::json>(id, method, args);
      return;
    } else {
      return client_->CallMethod<Result>(id, method, args);
    }
  }

  auto CallRaw(const std::string& method, const nlohmann::json& params)
      -> nlohmann::json {
    const int id = NextId();
    if (params.is_array()) {
      return client_->CallMethod<nlohmann::json>(
          id, method, params.get<std::vector<nlohmann::json>>());
    }
    // The public Robot::call API accepts a JSON object payload; controller RPCs
    // expect positional params, so keep the object as one positional argument.
    return client_->CallMethod<nlohmann::json>(id, method, {params});
  }

 private:
  static auto NextId() -> int {
    static std::atomic<int> id(0);
    const int next = ++id;
    // Wrap at INT_MAX to avoid ever producing a negative id for the wire.
    return next > 0 ? next : 1;
  }

  std::shared_ptr<jsonrpccxx::JsonRpcClient> client_;
};

}  // namespace lebai
