#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>

namespace protos_json::subscribe_proto {

struct SubscribeRequest {
  uint64_t interval_min{};
  uint64_t interval_max{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SubscribeRequest, interval_min, interval_max)
};

}  // namespace protos_json::subscribe_proto
