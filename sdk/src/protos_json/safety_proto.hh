#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::safety_proto {

struct CartesianLimit {
  double max_a{};
  double max_v{};
  double eq_radius{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartesianLimit, max_a, max_v, eq_radius)
};

struct CollisionDetector {
  std::string action;
  unsigned int pause_time{};
  unsigned int sensitivity{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CollisionDetector, action, pause_time,
                                 sensitivity)
};

struct CollisionTorqueDiff {
  std::vector<double> diffs;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CollisionTorqueDiff, diffs)
};

struct JointLimit {
  double min_position{};
  double max_position{};
  double max_a{};
  double max_v{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(JointLimit, min_position, max_position, max_a,
                                 max_v)
};

struct JointsLimit {
  std::vector<JointLimit> joints;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(JointsLimit, joints)
};

}  // namespace protos_json::safety_proto
