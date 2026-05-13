#pragma once

#include <nlohmann/json.hpp>

namespace protos_json::claw_proto {

struct InitClawRequest {
  bool force_initilization{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitClawRequest, force_initilization)
};

struct SetClawRequest {
  double force{};
  double amplitude{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetClawRequest, force, amplitude)
};

struct Claw {
  double force{};
  double amplitude{};
  double weight{};
  bool hold_on{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Claw, force, amplitude, weight, hold_on)
};

}  // namespace protos_json::claw_proto
