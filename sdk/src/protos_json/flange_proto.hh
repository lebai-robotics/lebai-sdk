#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>

namespace protos_json::flange_proto {

struct SetFlangeBaudRateRequest {
  uint32_t baud_rate{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetFlangeBaudRateRequest, baud_rate)
};

}  // namespace protos_json::flange_proto
