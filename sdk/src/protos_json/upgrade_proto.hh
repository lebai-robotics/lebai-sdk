#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace protos_json::upgrade_proto {

struct CheckUpgradeResponse {
  bool need_upgrade{};
  std::string introduction;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CheckUpgradeResponse, need_upgrade,
                                 introduction)
};

struct CommandStdout {
  bool done{};
  std::string stdout_text;
  std::string stderr_text;
  int code{};
};

inline void from_json(const nlohmann::json &json, CommandStdout &stdout_data) {
  json.at("done").get_to(stdout_data.done);
  json.at("stdout").get_to(stdout_data.stdout_text);
  json.at("stderr").get_to(stdout_data.stderr_text);
  json.at("code").get_to(stdout_data.code);
}

inline void to_json(nlohmann::json &json, const CommandStdout &stdout_data) {
  json = nlohmann::json{{"done", stdout_data.done},
                        {"stdout", stdout_data.stdout_text},
                        {"stderr", stdout_data.stderr_text},
                        {"code", stdout_data.code}};
}

}  // namespace protos_json::upgrade_proto
