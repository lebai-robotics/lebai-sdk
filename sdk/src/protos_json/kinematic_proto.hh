#pragma once

#include <nlohmann/json.hpp>

#include <vector>

namespace protos_json::kinematic_proto {

struct Position {
  double x{};
  double y{};
  double z{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y, z)
};

struct DhParam {
  double a{};
  double alpha{};
  double d{};
  double theta{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(DhParam, a, alpha, d, theta)
};

struct DhParams {
  std::vector<DhParam> params;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(DhParams, params)
};

struct JointPose {
  std::vector<double> joint;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(JointPose, joint)
};

struct EulerZyx {
  double x{};
  double y{};
  double z{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EulerZyx, x, y, z)
};

struct Rotation {
  EulerZyx euler_zyx;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rotation, euler_zyx)
};

struct CartesianPose {
  Position position;
  Rotation rotation;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartesianPose, position, rotation)
};

struct Pose {
  int kind{};
  JointPose joint;
  CartesianPose cart;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Pose, kind, joint, cart)
};

struct PoseRequest {
  Pose pose;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PoseRequest, pose)
};

struct GetInverseKinRequest {
  Pose pose;
  JointPose refer;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetInverseKinRequest, pose, refer)
};

struct GetPoseTransRequest {
  Pose from;
  Pose from_to;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetPoseTransRequest, from, from_to)
};

struct GetPoseAddRequest {
  Pose pose;
  Pose delta;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetPoseAddRequest, pose, delta)
};

struct CalcFrameRequest {
  CartesianPose o;
  CartesianPose x;
  CartesianPose xy;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CalcFrameRequest, o, x, xy)
};

struct KinData {
  std::vector<double> actual_joint_pose;
  std::vector<double> actual_joint_speed;
  std::vector<double> actual_joint_acc;
  std::vector<double> actual_joint_torque;
  std::vector<double> target_joint_pose;
  std::vector<double> target_joint_speed;
  std::vector<double> target_joint_acc;
  std::vector<double> target_joint_torque;
  CartesianPose actual_tcp_pose;
  CartesianPose target_tcp_pose;
  CartesianPose actual_flange_pose;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(
      KinData, actual_joint_pose, actual_joint_speed, actual_joint_acc,
      actual_joint_torque, target_joint_pose, target_joint_speed,
      target_joint_acc, target_joint_torque, actual_tcp_pose, target_tcp_pose,
      actual_flange_pose)
};

}  // namespace protos_json::kinematic_proto
