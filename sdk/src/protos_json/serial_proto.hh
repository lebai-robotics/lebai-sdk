#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

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

struct SetSerialTimeoutRequest {
  std::string device;
  unsigned int timeout{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetSerialTimeoutRequest, device, timeout)
};

struct WriteSerialRequest {
  std::string device;
  std::vector<unsigned int> data;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(WriteSerialRequest, device, data)
};

struct ReadSerialRequest {
  std::string device;
  unsigned int len{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ReadSerialRequest, device, len)
};

struct ReadSerialResponse {
  std::vector<unsigned int> data;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ReadSerialResponse, data)
};

struct ClearSerialRequest {
  std::string device;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ClearSerialRequest, device)
};

}  // namespace protos_json::serial_proto
