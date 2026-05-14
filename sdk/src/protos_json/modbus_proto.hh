#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::modbus_proto {

struct Modbus {
  std::string kind;
  std::string address;
  unsigned int port{};
  unsigned int slave_id{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Modbus, kind, address, port, slave_id)
};

struct ModbusRegister {
  std::string kind;
  unsigned int address{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ModbusRegister, kind, address)
};

struct SetCoilRequest {
  std::string device;
  std::string pin;
  bool value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetCoilRequest, device, pin, value)
};

struct SetCoilsRequest {
  std::string device;
  std::string pin;
  std::vector<bool> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetCoilsRequest, device, pin, values)
};

struct GetCoilsRequest {
  std::string device;
  std::string pin;
  unsigned int count{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetCoilsRequest, device, pin, count)
};

struct GetCoilsResponse {
  std::vector<bool> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetCoilsResponse, values)
};

struct SetRegisterRequest {
  std::string device;
  std::string pin;
  unsigned int value{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetRegisterRequest, device, pin, value)
};

struct SetRegistersRequest {
  std::string device;
  std::string pin;
  std::vector<unsigned int> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetRegistersRequest, device, pin, values)
};

struct GetRegistersRequest {
  std::string device;
  std::string pin;
  unsigned int count{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetRegistersRequest, device, pin, count)
};

struct GetRegistersResponse {
  std::vector<unsigned int> values;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetRegistersResponse, values)
};

struct LoadModbusRegisterListRequest {
  std::string device;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoadModbusRegisterListRequest, device)
};

struct LoadModbusRegisterRequest {
  std::string device;
  std::string name;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoadModbusRegisterRequest, device, name)
};

}  // namespace protos_json::modbus_proto
