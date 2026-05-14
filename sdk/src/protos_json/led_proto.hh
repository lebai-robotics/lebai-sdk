#pragma once

#include <nlohmann/json.hpp>

#include <map>
#include <string>
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

struct LedStyleLedData {
  std::string mode;
  std::string speed;
  std::vector<std::string> colors;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LedStyleLedData, mode, speed, colors)
};

struct LedStyle {
  LedStyleLedData led;
  std::string voice;
  std::string volume;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LedStyle, led, voice, volume)
};

struct LedStyles {
  std::map<std::string, LedStyle> styles;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LedStyles, styles)
};

}  // namespace protos_json::led_proto
