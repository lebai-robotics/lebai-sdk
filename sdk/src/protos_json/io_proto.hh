#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <vector>

namespace protos_json::io_proto {

enum class IoDevice : uint32_t {
  ROBOT = 0,
  FLANGE = 1,
  EXTRA = 2,
  SHOULDER = 3,
  FLANGE_BTN = 4,
};

struct GetDioPinRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetDioPinRequest, device, pin)
};

struct GetDioPinsRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  uint32_t count{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetDioPinsRequest, device, pin, count)
};

struct SetDoPinRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  uint32_t value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetDoPinRequest, device, pin, value)
};

struct GetDioPinResponse {
  uint32_t value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetDioPinResponse, value)
};

struct GetDioPinsResponse {
  std::vector<uint32_t> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetDioPinsResponse, values)
};

struct GetAioPinRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetAioPinRequest, device, pin)
};

struct GetAioPinsRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  uint32_t count{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetAioPinsRequest, device, pin, count)
};

struct SetAoPinRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  double value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetAoPinRequest, device, pin, value)
};

struct GetAioPinResponse {
  double value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetAioPinResponse, value)
};

struct GetAioPinsResponse {
  std::vector<double> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetAioPinsResponse, values)
};

struct SetDioModeRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  bool value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetDioModeRequest, device, pin, value)
};

struct GetDiosModeRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  uint32_t count{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetDiosModeRequest, device, pin, count)
};

struct GetDiosModeResponse {
  std::vector<bool> modes;
};

inline void from_json(const nlohmann::json& json, GetDiosModeResponse& response) {
  if (json.contains("modes")) {
    json.at("modes").get_to(response.modes);
  } else if (json.contains("values")) {
    json.at("values").get_to(response.modes);
  } else {
    response.modes.clear();
  }
}

inline void to_json(nlohmann::json& json, const GetDiosModeResponse& response) {
  json = nlohmann::json{{"modes", response.modes}};
}

}  // namespace protos_json::io_proto
