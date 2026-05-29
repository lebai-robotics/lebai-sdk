#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::plugin_proto {

struct PluginStoreInfo {
  std::string name;
  std::string url;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PluginStoreInfo, name, url)
};

struct PluginStore {
  std::vector<PluginStoreInfo> plugins;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PluginStore, plugins)
};

struct PluginInfo {
  std::string name;
  std::vector<std::string> boxs;
  std::vector<std::string> arms;
  std::string description;
  std::string homepage;
  bool auto_restart{};
  bool web{};
  bool daemon{};
  bool cmd{};
  bool enable{};
};

inline void from_json(const nlohmann::json &json, PluginInfo &plugin) {
  plugin.name = json.value("name", std::string{});
  plugin.boxs = json.value("boxs", std::vector<std::string>{});
  plugin.arms = json.value("arms", std::vector<std::string>{});
  plugin.description = json.value("description", std::string{});
  plugin.homepage = json.value("homepage", std::string{});
  plugin.auto_restart = json.value("auto_restart", false);
  plugin.web = json.value("web", false);
  plugin.daemon = json.value("daemon", false);
  plugin.cmd = json.value("cmd", false);
  plugin.enable = json.value("enable", false);
}

inline void to_json(nlohmann::json &json, const PluginInfo &plugin) {
  json = nlohmann::json{{"name", plugin.name},
                        {"boxs", plugin.boxs},
                        {"arms", plugin.arms},
                        {"description", plugin.description},
                        {"homepage", plugin.homepage},
                        {"auto_restart", plugin.auto_restart},
                        {"web", plugin.web},
                        {"daemon", plugin.daemon},
                        {"cmd", plugin.cmd},
                        {"enable", plugin.enable}};
}

struct Plugins {
  std::vector<PluginInfo> plugins;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Plugins, plugins)
};

struct PluginIndex {
  std::string name;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PluginIndex, name)
};

struct PluginCmdRequest {
  std::string name;
  std::vector<std::string> params;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PluginCmdRequest, name, params)
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
