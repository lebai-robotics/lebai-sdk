#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace protos_json::db_proto {

struct LoadRequest {
  std::string name;
  std::string dir;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoadRequest, name, dir)
};

struct LoadListRequest {
  std::string dir;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoadListRequest, dir)
};

struct LoadListResponse {
  std::vector<std::string> names;
};

inline void to_json(nlohmann::json &json, const LoadListResponse &response) {
  json = nlohmann::json{{"names", response.names}};
}

inline void from_json(const nlohmann::json &json, LoadListResponse &response) {
  response.names = json.value("names", std::vector<std::string>{});
}

struct Dir {
  std::string name;
  uint32_t id{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Dir, name, id)
};

struct Dirs {
  std::vector<Dir> dirs;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Dirs, dirs)
};

}  // namespace protos_json::db_proto
