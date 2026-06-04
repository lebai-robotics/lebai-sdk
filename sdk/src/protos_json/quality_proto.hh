#pragma once

#include <nlohmann/json.hpp>

#include "system_proto.hh"

#include <string>

namespace protos_json::quality_proto {

struct Auth {
  std::string time;
  std::string auth;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Auth, time, auth)
};

struct EmptyRequest {
  Auth auth;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EmptyRequest, auth)
};

struct BoxTestResponse {
  int status{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxTestResponse, status)
};

struct InitRobotRequest {
  Auth auth;
  protos_json::system_proto::RobotInfo info;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitRobotRequest, auth, info)
};

struct InitRobotResponse {
  std::string cup;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitRobotResponse, cup)
};

}  // namespace protos_json::quality_proto
