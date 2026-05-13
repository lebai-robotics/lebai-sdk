#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace protos_json::dynamic_proto {

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
};

inline void to_json(nlohmann::json &json, const Payload &payload) {
  json = nlohmann::json{{"mass", payload.mass}, {"cog", payload.cog}};
}

inline void from_json(const nlohmann::json &json, Payload &payload) {
  payload.mass = json.value("mass", 0.0);
  if (json.contains("cog") && !json.at("cog").is_null()) {
    payload.cog = json.at("cog").get<Position>();
  } else {
    payload.cog = Position{};
  }
}

struct SavePayloadRequest {
  std::string name;
  Payload data;
  std::string dir;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SavePayloadRequest, name, data, dir)
};

}  // namespace protos_json::dynamic_proto
