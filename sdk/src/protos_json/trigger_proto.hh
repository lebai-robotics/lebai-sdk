#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace protos_json::trigger_proto {

struct ButtonIndex {
  std::string device;
  uint32_t pin{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ButtonIndex, device, pin)
};

struct ButtonStatus {
  std::string state;
  uint32_t time{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ButtonStatus, state, time)
};

struct Condition {
  std::vector<ButtonIndex> pressed;
  ButtonIndex button;
  ButtonStatus status;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Condition, pressed, button, status)
};

struct Trigger {
  Condition condition;
  std::string function;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Trigger, condition, function)
};

struct Triggers {
  std::vector<Trigger> triggers;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Triggers, triggers)
};

}  // namespace protos_json::trigger_proto
