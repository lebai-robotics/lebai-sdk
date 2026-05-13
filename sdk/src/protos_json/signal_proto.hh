#pragma once

#include <nlohmann/json.hpp>

namespace protos_json::signal_proto {

struct SetSignalRequest {
  unsigned int key{};
  int value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetSignalRequest, key, value)
};

struct GetSignalRequest {
  unsigned int key{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetSignalRequest, key)
};

struct GetSignalResponse {
  int value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetSignalResponse, value)
};

}  // namespace protos_json::signal_proto
