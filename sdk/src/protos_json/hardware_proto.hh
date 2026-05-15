#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>

namespace protos_json::hardware_proto {

struct StartOtaRequest {
  std::string address;
  std::string partition;
  std::string file;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(StartOtaRequest, address, partition, file)
};

struct SwitchPartitionRequest {
  std::string address;
  std::string partition;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SwitchPartitionRequest, address, partition)
};

struct OtaState {
  std::string step;
  uint32_t progress{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(OtaState, step, progress)
};

}  // namespace protos_json::hardware_proto
