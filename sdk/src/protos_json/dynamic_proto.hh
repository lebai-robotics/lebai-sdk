#pragma once

#include <nlohmann/json.hpp>

namespace protos_json::dynamic_proto {

struct Position {
  double x{};
  double y{};
  double z{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y, z)
};

struct SetPayloadRequest {
  double mass{};
  Position cog;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetPayloadRequest, mass, cog)
};

struct SetCogRequest {
  Position cog;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetCogRequest, cog)
};

struct SetMassRequest {
  double mass{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetMassRequest, mass)
};

struct Payload {
  double mass{};
  Position cog;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Payload, mass, cog)
};

}  // namespace protos_json::dynamic_proto
