#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::signal_proto {

struct SetSignalRequest {
  unsigned int key{};
  int value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetSignalRequest, key, value)
};

struct SetSignalsRequest {
  unsigned int key{};
  std::vector<int> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetSignalsRequest, key, values)
};

struct GetSignalRequest {
  unsigned int key{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetSignalRequest, key)
};

struct GetSignalsRequest {
  unsigned int key{};
  unsigned int len{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetSignalsRequest, key, len)
};

struct WaitSignalRequest {
  unsigned int key{};
  int value{};
  std::string relation;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(WaitSignalRequest, key, value, relation)
};

struct GetSignalResponse {
  int value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetSignalResponse, value)
};

struct GetSignalsResponse {
  std::vector<int> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetSignalsResponse, values)
};

}  // namespace protos_json::signal_proto
