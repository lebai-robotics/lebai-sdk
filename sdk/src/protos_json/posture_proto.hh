#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::posture_proto {

struct JointPose {
  std::vector<double> joint;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(JointPose, joint)
};

struct Position {
  double x{};
  double y{};
  double z{};
};

inline void to_json(nlohmann::json &json, const Position &position) {
  json = nlohmann::json{
      {"x", position.x}, {"y", position.y}, {"z", position.z}};
}

inline void from_json(const nlohmann::json &json, Position &position) {
  position.x = json.value("x", 0.0);
  position.y = json.value("y", 0.0);
  position.z = json.value("z", 0.0);
}

struct Rotation {
  Position euler_zyx;
};

inline void to_json(nlohmann::json &json, const Rotation &rotation) {
  json = nlohmann::json{{"euler_zyx", rotation.euler_zyx}};
}

inline void from_json(const nlohmann::json &json, Rotation &rotation) {
  if (json.contains("euler_zyx") && !json.at("euler_zyx").is_null()) {
    rotation.euler_zyx = json.at("euler_zyx").get<Position>();
  } else {
    rotation.euler_zyx = Position{};
  }
}

struct CartesianPose {
  Position position;
  Rotation rotation;
};

inline void to_json(nlohmann::json &json, const CartesianPose &pose) {
  json = nlohmann::json{{"position", pose.position}, {"rotation", pose.rotation}};
}

inline void from_json(const nlohmann::json &json, CartesianPose &pose) {
  if (json.contains("position") && !json.at("position").is_null()) {
    pose.position = json.at("position").get<Position>();
  } else {
    pose.position = Position{};
  }
  if (json.contains("rotation") && !json.at("rotation").is_null()) {
    pose.rotation = json.at("rotation").get<Rotation>();
  } else {
    pose.rotation = Rotation{};
  }
}

struct CartesianFrame {
  std::string position_kind;
  Position position;
  std::string rotation_kind;
  Rotation rotation;
};

inline void to_json(nlohmann::json &json, const CartesianFrame &frame) {
  json = nlohmann::json{{"position_kind", frame.position_kind},
                        {"position", frame.position},
                        {"rotation_kind", frame.rotation_kind},
                        {"rotation", frame.rotation}};
}

inline void from_json(const nlohmann::json &json, CartesianFrame &frame) {
  frame.position_kind = json.value("position_kind", std::string{});
  if (json.contains("position") && !json.at("position").is_null()) {
    frame.position = json.at("position").get<Position>();
  } else {
    frame.position = Position{};
  }
  frame.rotation_kind = json.value("rotation_kind", std::string{});
  if (json.contains("rotation") && !json.at("rotation").is_null()) {
    frame.rotation = json.at("rotation").get<Rotation>();
  } else {
    frame.rotation = Rotation{};
  }
}

struct Pose {
  std::string kind;
  CartesianPose cart;
  JointPose joint;
};

inline void to_json(nlohmann::json &json, const Pose &pose) {
  json = nlohmann::json{{"kind", pose.kind},
                        {"cart", pose.cart},
                        {"joint", pose.joint}};
}

inline void from_json(const nlohmann::json &json, Pose &pose) {
  pose.kind = json.value("kind", std::string{});
  if (json.contains("cart") && !json.at("cart").is_null()) {
    pose.cart = json.at("cart").get<CartesianPose>();
  } else {
    pose.cart = CartesianPose{};
  }
  if (json.contains("joint") && !json.at("joint").is_null()) {
    pose.joint = json.at("joint").get<JointPose>();
  } else {
    pose.joint = JointPose{};
  }
}

struct SavePoseRequest {
  std::string name;
  Pose data;
  std::string dir;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SavePoseRequest, name, data, dir)
};

struct SaveFrameRequest {
  std::string name;
  CartesianFrame data;
  std::string dir;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveFrameRequest, name, data, dir)
};

struct Manipulation {
  double manipulation{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Manipulation, manipulation)
};

}  // namespace protos_json::posture_proto
