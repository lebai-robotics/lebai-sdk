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

struct SetZeroRequest {
  std::vector<double> pose;
  std::vector<bool> valids;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetZeroRequest, pose, valids)
};

struct ExtraServoParam {
  double acc_position_kp{};
  double acc_speed_kp{};
  double acc_speed_it{};
  double uni_position_kp{};
  double uni_speed_kp{};
  double uni_speed_it{};
  double dec_position_kp{};
  double dec_speed_kp{};
  double dec_speed_it{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ExtraServoParam, acc_position_kp,
                                 acc_speed_kp, acc_speed_it, uni_position_kp,
                                 uni_speed_kp, uni_speed_it, dec_position_kp,
                                 dec_speed_kp, dec_speed_it)
};

struct SetExtraServoParamsRequest {
  std::vector<ExtraServoParam> params;
  std::vector<bool> valids;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetExtraServoParamsRequest, params, valids)
};

struct ResetExtraServoParamsRequest {
  std::vector<bool> valids;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResetExtraServoParamsRequest, valids)
};

}  // namespace protos_json::motor_proto
