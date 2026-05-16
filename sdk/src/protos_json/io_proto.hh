#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace protos_json::io_proto {

enum class IoDevice : uint32_t {
  ROBOT = 0,
  FLANGE = 1,
  EXTRA = 2,
  ROBOT_BTN = 10,
  SHOULDER = 11,
  FLANGE_BTN = 12,
};

inline void from_json(const nlohmann::json &json, IoDevice &device) {
  const auto value = json.get<std::string>();
  if (value == "FLANGE") {
    device = IoDevice::FLANGE;
  } else if (value == "EXTRA") {
    device = IoDevice::EXTRA;
  } else if (value == "ROBOT_BTN") {
    device = IoDevice::ROBOT_BTN;
  } else if (value == "SHOULDER") {
    device = IoDevice::SHOULDER;
  } else if (value == "FLANGE_BTN") {
    device = IoDevice::FLANGE_BTN;
  } else {
    device = IoDevice::ROBOT;
  }
}

inline void to_json(nlohmann::json &json, const IoDevice &device) {
  switch (device) {
    case IoDevice::FLANGE:
      json = "FLANGE";
      break;
    case IoDevice::EXTRA:
      json = "EXTRA";
      break;
    case IoDevice::ROBOT_BTN:
      json = "ROBOT_BTN";
      break;
    case IoDevice::SHOULDER:
      json = "SHOULDER";
      break;
    case IoDevice::FLANGE_BTN:
      json = "FLANGE_BTN";
      break;
    case IoDevice::ROBOT:
    default:
      json = "ROBOT";
      break;
  }
}

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

struct SetDoPinsRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  std::vector<uint32_t> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetDoPinsRequest, device, pin, values)
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

struct SetAoPinsRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  std::vector<double> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetAoPinsRequest, device, pin, values)
};

struct ButtonIndex {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ButtonIndex, device, pin)
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

struct GetDioModeRequest {
  IoDevice device{IoDevice::ROBOT};
  uint32_t pin{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetDioModeRequest, device, pin)
};

inline bool parse_digital_mode(const nlohmann::json& value) {
  if (value.is_boolean()) {
    return value.get<bool>();
  }
  if (value.is_number_integer()) {
    return value.get<int>() != 0;
  }
  const auto mode = value.get<std::string>();
  return mode == "OUTPUT" || mode == "DO";
}

struct GetDioModeResponse {
  bool mode{};
};

inline void from_json(const nlohmann::json& json, GetDioModeResponse& response) {
  response.mode = parse_digital_mode(json.at("mode"));
}

inline void to_json(nlohmann::json& json, const GetDioModeResponse& response) {
  json = nlohmann::json{{"mode", response.mode ? "OUTPUT" : "INPUT"}};
}

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
    response.modes.clear();
    for (const auto& mode : json.at("modes")) {
      response.modes.push_back(parse_digital_mode(mode));
    }
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
