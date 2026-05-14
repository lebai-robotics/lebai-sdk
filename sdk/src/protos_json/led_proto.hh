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
};

inline void to_json(nlohmann::json &json, const LedStyle &style) {
  json = nlohmann::json{{"led", style.led},
                        {"voice", style.voice},
                        {"volume", style.volume}};
}

inline void from_json(const nlohmann::json &json, LedStyle &style) {
  style.led = json.value("led", LedStyleLedData{});
  style.voice = json.value("voice", std::string{});
  style.volume = json.value("volume", std::string{});
}

struct SaveLedStyleRequest {
  std::string name;
  LedStyle data;
  std::string dir;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveLedStyleRequest, name, data, dir)
};

struct LedStyles {
  std::map<std::string, LedStyle> styles;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LedStyles, styles)
};

}  // namespace protos_json::led_proto
