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
  std::string address;
  std::string partition;
  std::string step;
  uint32_t progress{};
};

inline void to_json(nlohmann::json &json, const OtaState &state) {
  json = nlohmann::json{{"address", state.address},
                        {"partition", state.partition},
                        {"step", state.step},
                        {"progress", state.progress}};
}

inline void from_json(const nlohmann::json &json, OtaState &state) {
  state.address = json.value("address", std::string{});
  state.partition = json.value("partition", std::string{});
  state.step = json.value("step", std::string{});
  state.progress = json.value("progress", 0U);
}

}  // namespace protos_json::hardware_proto
