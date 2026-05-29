#pragma once

#include <nlohmann/json.hpp>

#include "system_proto.hh"

#include <string>

namespace protos_json::backup_proto {

struct Timestamp {
  int64_t seconds{};
  int32_t nanos{};
};

inline void from_json(const nlohmann::json &json, Timestamp &timestamp) {
  timestamp.seconds = json.value("seconds", int64_t{0});
  timestamp.nanos = json.value("nanos", int32_t{0});
}

inline void to_json(nlohmann::json &json, const Timestamp &timestamp) {
  json = nlohmann::json{{"seconds", timestamp.seconds},
                        {"nanos", timestamp.nanos}};
}

struct Options {
  bool tmp{};
  bool syslog{};
  bool arm{};
  bool config{};
  bool data{};
  bool file{};
  bool docker{};
  bool ds{};
};

inline void from_json(const nlohmann::json &json, Options &options) {
  options.tmp = json.value("tmp", false);
  options.syslog = json.value("syslog", false);
  options.arm = json.value("arm", false);
  options.config = json.value("config", false);
  options.data = json.value("data", false);
  options.file = json.value("file", false);
  options.docker = json.value("docker", false);
  options.ds = json.value("ds", false);
}

inline void to_json(nlohmann::json &json, const Options &options) {
  json = nlohmann::json{{"tmp", options.tmp},
                        {"syslog", options.syslog},
                        {"arm", options.arm},
                        {"config", options.config},
                        {"data", options.data},
                        {"file", options.file},
                        {"docker", options.docker},
                        {"ds", options.ds}};
}

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
  Timestamp timestamp;
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
  if (json.contains("timestamp")) {
    json.at("timestamp").get_to(info.timestamp);
  }
  if (json.contains("option")) {
    json.at("option").get_to(info.option);
  }
}

}  // namespace protos_json::backup_proto
