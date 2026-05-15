/**
 * Diagnostic tool to test different Modbus RTU configurations
 */

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <vector>

#include "modbus_rtu_client.hh"

using lebai::l_master::ModbusRtuClient;

struct TestConfig {
  int baudRate;
  ModbusRtuClient::Parity parity;
  const char* parityName;
};

bool testConfiguration(const std::string& portName, int baudRate,
                       ModbusRtuClient::Parity parity, const char* parityName,
                       uint8_t modbusAddr) {
  std::cout << "\nTesting: " << baudRate << " baud, " << parityName
            << ", address " << (int)modbusAddr << std::endl;

  try {
    ModbusRtuClient::Config config;
    config.port_name = portName;
    config.baud_rate = static_cast<unsigned int>(baudRate);
    config.slave_id = modbusAddr;
    config.timeout = std::chrono::milliseconds(1000);
    config.parity = parity;

    ModbusRtuClient client(config);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // Try to read a register to test communication
    const auto values = client.read_holding_registers(0, 1);
    std::cout << "  SUCCESS! Read value: " << values.front() << std::endl;
    return true;

  } catch (const std::exception& e) {
    std::cout << "  Exception: " << e.what() << std::endl;
    return false;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <serial_port>" << std::endl;
    std::cerr << "Example: " << argv[0] << " COM6" << std::endl;
    return 1;
  }

  std::string portName = argv[1];
  std::cout << "Modbus RTU Diagnostic Tool" << std::endl;
  std::cout << "Testing port: " << portName << std::endl;
  std::cout << "================================" << std::endl;

  // Common configurations to test
  std::vector<TestConfig> configs = {
      {9600, ModbusRtuClient::Parity::None, "8N1"},
      {9600, ModbusRtuClient::Parity::Even, "8E1"},
      {19200, ModbusRtuClient::Parity::None, "8N1"},
      {19200, ModbusRtuClient::Parity::Even, "8E1"},
      {38400, ModbusRtuClient::Parity::None, "8N1"},
      {57600, ModbusRtuClient::Parity::None, "8N1"},
      {115200, ModbusRtuClient::Parity::None, "8N1"},
      {115200, ModbusRtuClient::Parity::Even, "8E1"},
  };

  // Common Modbus addresses to test
  std::vector<uint8_t> addresses = {0, 1};

  bool found = false;

  for (const auto& config : configs) {
    for (uint8_t addr : addresses) {
      if (testConfiguration(portName, config.baudRate, config.parity,
                            config.parityName, addr)) {
        std::cout << "\n*** WORKING CONFIGURATION FOUND ***" << std::endl;
        std::cout << "Baud Rate: " << config.baudRate << std::endl;
        std::cout << "Settings: " << config.parityName << std::endl;
        std::cout << "Modbus Address: " << (int)addr << std::endl;
        found = true;
        goto done;  // Exit both loops
      }
    }
  }

done:
  if (!found) {
    std::cout << "\nNo working configuration found." << std::endl;
    std::cout << "Please check:" << std::endl;
    std::cout << "1. Device is powered on and connected" << std::endl;
    std::cout << "2. RS485/RS232 converter is working" << std::endl;
    std::cout << "3. Device documentation for correct parameters" << std::endl;
  }

  return found ? 0 : 1;
}
