#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>

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

struct SetClawAoRequest {
  uint32_t address{};
  double value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetClawAoRequest, address, value)
};

struct Claw {
  double force{};
  double amplitude{};
  double weight{};
  bool hold_on{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Claw, force, amplitude, weight, hold_on)
};

struct GetClawAiRequest {
  uint32_t address{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetClawAiRequest, address)
};

struct GetClawAiResponse {
  double value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetClawAiResponse, value)
};

}  // namespace protos_json::claw_proto
