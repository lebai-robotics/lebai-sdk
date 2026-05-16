#pragma once

#include <nlohmann/json.hpp>

namespace protos_json::kin_factor_proto {

struct KinFactor {
  int speed_factor{};
};

inline void from_json(const nlohmann::json& json, KinFactor& factor) {
  if (json.contains("speed_factor")) {
    json.at("speed_factor").get_to(factor.speed_factor);
  } else {
    json.at("factor").get_to(factor.speed_factor);
  }
}

inline void to_json(nlohmann::json& json, const KinFactor& factor) {
  json = nlohmann::json{{"speed_factor", factor.speed_factor}};
}

}  // namespace protos_json::kin_factor_proto
