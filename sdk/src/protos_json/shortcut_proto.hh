#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace protos_json::shortcut_proto {

struct Shortcut {
  uint32_t id{};
  std::string dir;
  std::string name;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Shortcut, id, dir, name)
};

struct ShortcutList {
  std::vector<Shortcut> list;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ShortcutList, list)
};

}  // namespace protos_json::shortcut_proto
