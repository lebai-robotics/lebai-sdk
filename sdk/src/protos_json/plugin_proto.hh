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

struct PluginIndex {
  std::string name;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PluginIndex, name)
};

struct CommandStdout {
  bool done{};
  std::string stdout_text;
  std::string stderr_text;
  int code{};
};

inline void from_json(const nlohmann::json &json, CommandStdout &stdout_data) {
  stdout_data.done = json.value("done", false);
  stdout_data.stdout_text = json.value("stdout", std::string{});
  stdout_data.stderr_text = json.value("stderr", std::string{});
  stdout_data.code = json.value("code", 0);
}

inline void to_json(nlohmann::json &json, const CommandStdout &stdout_data) {
  json = nlohmann::json{{"done", stdout_data.done},
                        {"stdout", stdout_data.stdout_text},
                        {"stderr", stdout_data.stderr_text},
                        {"code", stdout_data.code}};
}

}  // namespace protos_json::plugin_proto
