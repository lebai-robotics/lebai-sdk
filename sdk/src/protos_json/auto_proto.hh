#pragma once

#include <nlohmann/json.hpp>

namespace protos_json::auto_proto {

enum class AutoCfg {
  ARM_POWER = 0,
  ENABLE_JOINT = 1,
  INIT_CLAW = 2,
};

struct GetAutoRequest {
  AutoCfg name{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetAutoRequest, name)
};

struct GetAutoResponse {
  bool value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetAutoResponse, value)
};

struct SetAutoRequest {
  AutoCfg name{};
  bool value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetAutoRequest, name, value)
};

struct SetAutoResponse {
  bool value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetAutoResponse, value)
};

}  // namespace protos_json::auto_proto
