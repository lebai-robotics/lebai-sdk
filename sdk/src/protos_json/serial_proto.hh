#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace protos_json::serial_proto {

struct SetSerialBaudRateRequest {
  std::string device;
  unsigned int baud_rate{115200};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetSerialBaudRateRequest, device, baud_rate)
};

struct SetSerialParityRequest {
  std::string device;
  unsigned int parity{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetSerialParityRequest, device, parity)
};

}  // namespace protos_json::serial_proto
