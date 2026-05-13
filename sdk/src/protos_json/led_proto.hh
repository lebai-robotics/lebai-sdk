#pragma once

#include <nlohmann/json.hpp>

#include <vector>

namespace protos_json::led_proto {

struct LedData {
  int mode{};
  int speed{};
  std::vector<int> colors;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LedData, mode, speed, colors)
};

struct VoiceData {
  int voice{};
  int volume{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(VoiceData, voice, volume)
};

struct FanData {
  int fan{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(FanData, fan)
};

}  // namespace protos_json::led_proto
