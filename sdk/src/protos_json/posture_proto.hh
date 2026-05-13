#pragma once

#include <nlohmann/json.hpp>

namespace protos_json::posture_proto {

struct Position {
  double x{};
  double y{};
  double z{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y, z)
};

struct Rotation {
  Position euler_zyx;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rotation, euler_zyx)
};

struct CartesianPose {
  Position position;
  Rotation rotation;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartesianPose, position, rotation)
};

}  // namespace protos_json::posture_proto
