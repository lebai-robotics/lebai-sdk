#pragma once

#include <nlohmann/json.hpp>

#include "system_proto.hh"

#include <string>

namespace protos_json::backup_proto {

struct Options {
  bool tmp{};
  bool syslog{};
  bool arm{};
  bool config{};
  bool data{};
  bool file{};
  bool docker{};
  bool ds{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Options, tmp, syslog, arm, config, data, file,
                                 docker, ds)
};

struct BackupRequest {
  std::string file;
  Options option;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(BackupRequest, file, option)
};

struct GetBackupInfoRequest {
  std::string file;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetBackupInfoRequest, file)
};

using RestoreRequest = BackupRequest;

struct BackupInfo {
  protos_json::system_proto::SystemInfo system;
  protos_json::system_proto::RobotInfo robot;
  protos_json::system_proto::HardwareInfo hardware;
  protos_json::system_proto::SoftwareInfo software;
  Options option;
};

inline void from_json(const nlohmann::json &json, BackupInfo &info) {
  if (json.contains("system")) {
    json.at("system").get_to(info.system);
  }
  if (json.contains("robot")) {
    json.at("robot").get_to(info.robot);
  }
  if (json.contains("hardware")) {
    json.at("hardware").get_to(info.hardware);
  }
  if (json.contains("software")) {
    json.at("software").get_to(info.software);
  }
  if (json.contains("option")) {
    json.at("option").get_to(info.option);
  }
}

}  // namespace protos_json::backup_proto
