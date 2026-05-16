#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::multi_devices_proto {

struct DeviceInfo {
  std::string name;
  std::string mac;
  std::string ip;
  bool online = false;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeviceInfo, name, mac, ip, online)
};

struct DiscoverRobotsResponse {
  std::vector<DeviceInfo> devices;
};

inline void to_json(nlohmann::json &json,
                    const DiscoverRobotsResponse &response) {
  json = nlohmann::json{{"devices", response.devices}};
}

inline void from_json(const nlohmann::json &json,
                      DiscoverRobotsResponse &response) {
  response.devices = json.value("devices", std::vector<DeviceInfo>{});
}

}  // namespace protos_json::multi_devices_proto
