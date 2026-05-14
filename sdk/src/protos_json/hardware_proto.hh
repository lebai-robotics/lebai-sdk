#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>

namespace protos_json::hardware_proto {

struct OtaState {
  std::string step;
  uint32_t progress{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(OtaState, step, progress)
};

}  // namespace protos_json::hardware_proto
