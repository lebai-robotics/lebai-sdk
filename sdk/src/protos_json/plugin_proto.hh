#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::plugin_proto {

struct PluginInfo {
  std::string name;
  std::string description;
  std::string homepage;
  bool auto_restart{};
  bool web{};
  bool daemon{};
  bool cmd{};
  bool enable{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PluginInfo, name, description, homepage,
                                 auto_restart, web, daemon, cmd, enable)
};

struct Plugins {
  std::vector<PluginInfo> plugins;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Plugins, plugins)
};

}  // namespace protos_json::plugin_proto
