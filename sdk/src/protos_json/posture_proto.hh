#pragma once

#include <nlohmann/json.hpp>

namespace protos_json::posture_proto {

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

}  // namespace protos_json::posture_proto
