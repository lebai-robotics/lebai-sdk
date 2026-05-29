#pragma once

#include <nlohmann/json.hpp>

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace protos_json::motion_proto {

struct JointPose {
  std::vector<double> joint;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(JointPose, joint)
};

struct Pose {
  int kind{1};
  JointPose joint;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Pose, kind, joint)
};

struct MoveParam {
  double velocity{};
  double acc{};
  double time{};
  double radius{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(MoveParam, velocity, acc, time, radius)
};

struct MoveRequest {
  Pose pose;
  MoveParam param;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(MoveRequest, pose, param)
};

struct MoveCircularRequest {
  Pose pose_via;
  Pose pose;
  double rad{};
  MoveParam param;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(MoveCircularRequest, pose_via, pose, rad,
                                 param)
};

struct MotionIndex {
  uint32_t id{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(MotionIndex, id)
};

struct Position {
  double x{};
  double y{};
  double z{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y, z)
};

struct Wrench {
  Position force;
  Position torque;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Wrench, force, torque)
};

struct SetForceModeSensorRequest {
  std::string sensor;
  uint32_t address{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetForceModeSensorRequest, sensor, address)
};

struct SetForceModeParamRequest {
  std::optional<double> damping;
  std::optional<double> mass;
  std::optional<double> force_threshold;
  std::optional<double> torque_threshold;
};

inline void to_json(nlohmann::json &json,
                    const SetForceModeParamRequest &request) {
  json = nlohmann::json::object();
  if (request.damping.has_value()) {
    json["damping"] = *request.damping;
  }
  if (request.mass.has_value()) {
    json["mass"] = *request.mass;
  }
  if (request.force_threshold.has_value()) {
    json["force_threshold"] = *request.force_threshold;
  }
  if (request.torque_threshold.has_value()) {
    json["torque_threshold"] = *request.torque_threshold;
  }
}

inline void from_json(const nlohmann::json &json,
                      SetForceModeParamRequest &request) {
  request.damping.reset();
  request.mass.reset();
  request.force_threshold.reset();
  request.torque_threshold.reset();
  if (json.contains("damping") && !json.at("damping").is_null()) {
    request.damping = json.at("damping").get<double>();
  }
  if (json.contains("mass") && !json.at("mass").is_null()) {
    request.mass = json.at("mass").get<double>();
  }
  if (json.contains("force_threshold") &&
      !json.at("force_threshold").is_null()) {
    request.force_threshold = json.at("force_threshold").get<double>();
  }
  if (json.contains("torque_threshold") &&
      !json.at("torque_threshold").is_null()) {
    request.torque_threshold = json.at("torque_threshold").get<double>();
  }
}

struct Rotation {
  Position euler_zyx;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rotation, euler_zyx)
};

struct CartesianPose {
  Position position;
  Rotation rotation;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartesianPose, position, rotation)
};

struct StartForceModeRequest {
  CartesianPose limit;
  Wrench wrench;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(StartForceModeRequest, limit, wrench)
};

struct CartesianMovePose {
  int kind{};
  CartesianPose cart;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartesianMovePose, kind, cart)
};

struct CartesianMoveRequest {
  CartesianMovePose pose;
  MoveParam param;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartesianMoveRequest, pose, param)
};

struct CartesianMoveCircularRequest {
  CartesianMovePose pose_via;
  CartesianMovePose pose;
  double rad{};
  MoveParam param;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartesianMoveCircularRequest, pose_via, pose,
                                 rad, param)
};

struct CartesianFrame {
  int position_kind{};
  Position position;
  int rotation_kind{};
  Rotation rotation;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartesianFrame, position_kind, position,
                                 rotation_kind, rotation)
};

struct SpeedParam {
  double acc{};
  double time{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SpeedParam, acc, time)
};

struct SpeedJointRequest {
  JointPose speed;
  SpeedParam param;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SpeedJointRequest, speed, param)
};

struct SpeedLinearRequest {
  CartesianPose speed;
  SpeedParam param;
  CartesianFrame frame;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SpeedLinearRequest, speed, param, frame)
};

struct JointMove {
  double pose{};
  double velocity{};
  double acc{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(JointMove, pose, velocity, acc)
};

struct MovePvatRequest {
  double duration{};
  std::vector<JointMove> joints;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(MovePvatRequest, duration, joints)
};

struct Trajectory {
  std::string kind;
  std::vector<MovePvatRequest> data;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Trajectory, kind, data)
};

struct SaveTrajectoryRequest {
  std::string name;
  Trajectory data;
  std::string dir;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveTrajectoryRequest, name, data, dir)
};

struct StartRecordTrajectoryRequest {
  std::string kind;
  double duration{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(StartRecordTrajectoryRequest, kind, duration)
};

struct EndRecordTrajectoryRequest {
  std::string name;
  std::string dir;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EndRecordTrajectoryRequest, name, dir)
};

enum class MotionState {
  WAIT = 0,
  RUNNING = 1,
  FINISHED = 2,
};

struct GetMotionStateResponse {
  MotionState state{MotionState::WAIT};
};

inline void from_json(const nlohmann::json &json, MotionState &state) {
  const auto value = json.get<std::string>();
  if (value == "WAIT") {
    state = MotionState::WAIT;
  } else if (value == "RUNNING") {
    state = MotionState::RUNNING;
  } else if (value == "FINISHED") {
    state = MotionState::FINISHED;
  } else {
    throw nlohmann::json::type_error::create(302, "invalid motion state",
                                             &json);
  }
}

inline void to_json(nlohmann::json &json, const MotionState &state) {
  switch (state) {
    case MotionState::WAIT:
      json = "WAIT";
      return;
    case MotionState::RUNNING:
      json = "RUNNING";
      return;
    case MotionState::FINISHED:
      json = "FINISHED";
      return;
  }
  throw std::runtime_error("invalid motion state");
}

inline void from_json(const nlohmann::json &json,
                      GetMotionStateResponse &response) {
  json.at("state").get_to(response.state);
}

inline void to_json(nlohmann::json &json,
                    const GetMotionStateResponse &response) {
  json = nlohmann::json{{"state", response.state}};
}

}  // namespace protos_json::motion_proto
