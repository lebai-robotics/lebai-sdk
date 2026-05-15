#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace protos_json::system_proto {

namespace detail {

inline auto parse_u64(const nlohmann::json& value) -> uint64_t {
  if (value.is_string()) {
    return static_cast<uint64_t>(std::stoull(value.get<std::string>()));
  }
  return value.get<uint64_t>();
}

inline auto parse_u32(const nlohmann::json& value) -> uint32_t {
  if (value.is_string()) {
    return static_cast<uint32_t>(std::stoul(value.get<std::string>()));
  }
  return value.get<uint32_t>();
}

inline auto parse_enum_string(const nlohmann::json& value) -> std::string {
  if (!value.is_string()) {
    throw std::runtime_error("enum field must be a string");
  }
  return value.get<std::string>();
}

}  // namespace detail

struct HelloData {
  std::string data;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(HelloData, data)
};

struct SetVirtualIpRequest {
  std::string ifname;
  std::string ip;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetVirtualIpRequest, ifname, ip)
};

enum class RobotState {
  DISCONNECTED = 0,
  ESTOP = 1,
  BOOTING = 2,
  ROBOT_OFF = 3,
  ROBOT_ON = 4,
  IDLE = 5,
  PAUSED = 6,
  MOVING = 7,
  UPDATING = 8,
  STARTING = 9,
  STOPPING = 10,
  TEACHING = 11,
  STOP = 12,
};

inline void from_json(const nlohmann::json& value, RobotState& state) {
  const auto text = detail::parse_enum_string(value);
  if (text == "DISCONNECTED") {
    state = RobotState::DISCONNECTED;
  } else if (text == "ESTOP") {
    state = RobotState::ESTOP;
  } else if (text == "BOOTING") {
    state = RobotState::BOOTING;
  } else if (text == "ROBOT_OFF") {
    state = RobotState::ROBOT_OFF;
  } else if (text == "ROBOT_ON") {
    state = RobotState::ROBOT_ON;
  } else if (text == "IDLE") {
    state = RobotState::IDLE;
  } else if (text == "PAUSED") {
    state = RobotState::PAUSED;
  } else if (text == "MOVING") {
    state = RobotState::MOVING;
  } else if (text == "UPDATING") {
    state = RobotState::UPDATING;
  } else if (text == "STARTING") {
    state = RobotState::STARTING;
  } else if (text == "STOPPING") {
    state = RobotState::STOPPING;
  } else if (text == "TEACHING") {
    state = RobotState::TEACHING;
  } else if (text == "STOP") {
    state = RobotState::STOP;
  } else {
    throw std::runtime_error("unknown robot state: " + text);
  }
}

inline void to_json(nlohmann::json& value, const RobotState& state) {
  switch (state) {
    case RobotState::DISCONNECTED:
      value = "DISCONNECTED";
      return;
    case RobotState::ESTOP:
      value = "ESTOP";
      return;
    case RobotState::BOOTING:
      value = "BOOTING";
      return;
    case RobotState::ROBOT_OFF:
      value = "ROBOT_OFF";
      return;
    case RobotState::ROBOT_ON:
      value = "ROBOT_ON";
      return;
    case RobotState::IDLE:
      value = "IDLE";
      return;
    case RobotState::PAUSED:
      value = "PAUSED";
      return;
    case RobotState::MOVING:
      value = "MOVING";
      return;
    case RobotState::UPDATING:
      value = "UPDATING";
      return;
    case RobotState::STARTING:
      value = "STARTING";
      return;
    case RobotState::STOPPING:
      value = "STOPPING";
      return;
    case RobotState::TEACHING:
      value = "TEACHING";
      return;
    case RobotState::STOP:
      value = "STOP";
      return;
  }
}

enum class NetworkKind {
  UNKNOWN = 0,
  LOOPBACK = 1,
  ETHERNET = 2,
  WIRELESS = 3,
  CELLULAR = 4,
  USB = 5,
  DOCKER = 11,
};

inline void from_json(const nlohmann::json& value, NetworkKind& kind) {
  const auto text = detail::parse_enum_string(value);
  if (text == "LOOPBACK") {
    kind = NetworkKind::LOOPBACK;
  } else if (text == "ETHERNET") {
    kind = NetworkKind::ETHERNET;
  } else if (text == "WIRELESS") {
    kind = NetworkKind::WIRELESS;
  } else if (text == "CELLULAR") {
    kind = NetworkKind::CELLULAR;
  } else if (text == "USB") {
    kind = NetworkKind::USB;
  } else if (text == "DOCKER") {
    kind = NetworkKind::DOCKER;
  } else {
    kind = NetworkKind::UNKNOWN;
  }
}

inline void to_json(nlohmann::json& value, const NetworkKind& kind) {
  switch (kind) {
    case NetworkKind::LOOPBACK:
      value = "LOOPBACK";
      return;
    case NetworkKind::ETHERNET:
      value = "ETHERNET";
      return;
    case NetworkKind::WIRELESS:
      value = "WIRELESS";
      return;
    case NetworkKind::CELLULAR:
      value = "CELLULAR";
      return;
    case NetworkKind::USB:
      value = "USB";
      return;
    case NetworkKind::DOCKER:
      value = "DOCKER";
      return;
    default:
      value = "UNKNOWN";
      return;
  }
}

struct MemoryInfo {
  uint64_t used{};
  uint64_t total{};
  uint64_t used_swap{};
  uint64_t total_swap{};
};

inline void from_json(const nlohmann::json& value, MemoryInfo& info) {
  info.used = detail::parse_u64(value.at("used"));
  info.total = detail::parse_u64(value.at("total"));
  info.used_swap = detail::parse_u64(value.at("used_swap"));
  info.total_swap = detail::parse_u64(value.at("total_swap"));
}

inline void to_json(nlohmann::json& value, const MemoryInfo& info) {
  value = nlohmann::json{
      {"used", info.used},
      {"total", info.total},
      {"used_swap", info.used_swap},
      {"total_swap", info.total_swap},
  };
}

struct DiskInfo {
  std::string name;
  uint64_t used{};
  uint64_t total{};
};

inline void from_json(const nlohmann::json& value, DiskInfo& info) {
  info.name = value.at("name").get<std::string>();
  info.used = detail::parse_u64(value.at("used"));
  info.total = detail::parse_u64(value.at("total"));
}

inline void to_json(nlohmann::json& value, const DiskInfo& info) {
  value = nlohmann::json{
      {"name", info.name},
      {"used", info.used},
      {"total", info.total},
  };
}

struct NetworkInfo {
  std::string ifname;
  NetworkKind kind{};
  std::string mac;
  std::vector<std::string> ips;
  std::vector<std::string> virtual_ips;
  std::string gateway;
  std::string netmask;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(NetworkInfo, ifname, kind, mac, ips,
                                 virtual_ips, gateway, netmask)
};

struct CpuInfo {
  std::string brand;
  uint32_t num{};
  uint64_t frequency{};
  float temperature{};
  float critical{};
};

inline void from_json(const nlohmann::json& value, CpuInfo& info) {
  info.brand = value.at("brand").get<std::string>();
  info.num = detail::parse_u32(value.at("num"));
  info.frequency = detail::parse_u64(value.at("frequency"));
  info.temperature = value.at("temperature").get<float>();
  info.critical = value.at("critical").get<float>();
}

inline void to_json(nlohmann::json& value, const CpuInfo& info) {
  value = nlohmann::json{
      {"brand", info.brand},
      {"num", info.num},
      {"frequency", info.frequency},
      {"temperature", info.temperature},
      {"critical", info.critical},
  };
}

struct SystemInfo {
  std::string name;
  std::string kernel_version;
  std::string os_version;
  std::string host_name;
  MemoryInfo memory;
  std::vector<DiskInfo> disks;
  std::vector<NetworkInfo> networks;
  CpuInfo cpu;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SystemInfo, name, kernel_version, os_version,
                                 host_name, memory, disks, networks, cpu)
};

struct RobotInfo {
  std::string name;
  std::string mac;
  std::string box_model;
  std::string box_sn;
  std::string arm_model;
  std::string arm_sn;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(RobotInfo, name, mac, box_model, box_sn,
                                 arm_model, arm_sn)
};

struct DeviceInfo {
  bool invalid{};
  std::string sn;
  std::string version;
  std::string partition;
  uint32_t di_num{};
  uint32_t do_num{};
  uint32_t dio_num{};
  uint32_t ai_num{};
  uint32_t ao_num{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeviceInfo, invalid, sn, version, partition,
                                 di_num, do_num, dio_num, ai_num, ao_num)
};

struct HardwareInfo {
  DeviceInfo comboard;
  std::vector<DeviceInfo> joints;
  DeviceInfo flange;
  DeviceInfo led;
  DeviceInfo extra_io;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(HardwareInfo, comboard, joints, flange, led,
                                 extra_io)
};

struct SoftwareItemInfo {
  std::string version;
  std::string branch;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SoftwareItemInfo, version, branch)
};

struct SoftwareInfo {
  std::map<std::string, SoftwareItemInfo> software;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SoftwareInfo, software)
};

struct GetBoxDevicesRequest {
  std::string prefix;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetBoxDevicesRequest, prefix)
};

struct GetBoxDevicesResponse {
  std::vector<std::string> devices;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetBoxDevicesResponse, devices)
};

struct GetRobotStateResponse {
  RobotState state{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetRobotStateResponse, state)
};

enum class EstopReason {
  NONE = 0,
  SYSTEM = 2,
  MANUAL = 3,
  HARD_ESTOP = 4,
  COLLISION = 5,
  JOINT_LIMIT = 6,
  EXCEED = 7,
  TRAJECTORY_ERROR = 8,
  COMM_ERROR = 11,
  CAN_ERROR = 12,
  JOINT_ERROR = 13,
};

inline void from_json(const nlohmann::json& value, EstopReason& reason) {
  const auto text = detail::parse_enum_string(value);
  if (text == "NONE") {
    reason = EstopReason::NONE;
  } else if (text == "SYSTEM") {
    reason = EstopReason::SYSTEM;
  } else if (text == "MANUAL") {
    reason = EstopReason::MANUAL;
  } else if (text == "HARD_ESTOP") {
    reason = EstopReason::HARD_ESTOP;
  } else if (text == "COLLISION") {
    reason = EstopReason::COLLISION;
  } else if (text == "JOINT_LIMIT") {
    reason = EstopReason::JOINT_LIMIT;
  } else if (text == "EXCEED") {
    reason = EstopReason::EXCEED;
  } else if (text == "TRAJECTORY_ERROR") {
    reason = EstopReason::TRAJECTORY_ERROR;
  } else if (text == "COMM_ERROR") {
    reason = EstopReason::COMM_ERROR;
  } else if (text == "CAN_ERROR") {
    reason = EstopReason::CAN_ERROR;
  } else if (text == "JOINT_ERROR") {
    reason = EstopReason::JOINT_ERROR;
  } else {
    throw std::runtime_error("unknown estop reason: " + text);
  }
}

inline void to_json(nlohmann::json& value, const EstopReason& reason) {
  switch (reason) {
    case EstopReason::NONE:
      value = "NONE";
      return;
    case EstopReason::SYSTEM:
      value = "SYSTEM";
      return;
    case EstopReason::MANUAL:
      value = "MANUAL";
      return;
    case EstopReason::HARD_ESTOP:
      value = "HARD_ESTOP";
      return;
    case EstopReason::COLLISION:
      value = "COLLISION";
      return;
    case EstopReason::JOINT_LIMIT:
      value = "JOINT_LIMIT";
      return;
    case EstopReason::EXCEED:
      value = "EXCEED";
      return;
    case EstopReason::TRAJECTORY_ERROR:
      value = "TRAJECTORY_ERROR";
      return;
    case EstopReason::COMM_ERROR:
      value = "COMM_ERROR";
      return;
    case EstopReason::CAN_ERROR:
      value = "CAN_ERROR";
      return;
    case EstopReason::JOINT_ERROR:
      value = "JOINT_ERROR";
      return;
  }
}

struct GetEstopReasonResponse {
  EstopReason reason{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetEstopReasonResponse, reason)
};

struct PhyData {
  std::vector<double> joint_temp;
  std::vector<double> joint_voltage;
  double flange_voltage{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PhyData, joint_temp, joint_voltage,
                                 flange_voltage)
};

}  // namespace protos_json::system_proto
