#pragma once

#include <nlohmann/json.hpp>

#include <vector>

namespace protos_json::motor_proto {

struct ServoParam {
  double position_kp{};
  double speed_kp{};
  double speed_it{};
  double torque_cmd_filter{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ServoParam, position_kp, speed_kp, speed_it,
                                 torque_cmd_filter)
};

struct ServoParams {
  std::vector<ServoParam> params;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ServoParams, params)
};

}  // namespace protos_json::motor_proto
