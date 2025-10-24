/**
 * Diagnostic tool to test different Modbus RTU configurations
 */

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <vector>
#include "Modbus.h"
#include "ModbusClient.h"
#include "ModbusClientPort.h"
#include "ModbusPort.h"

struct TestConfig {
  int baudRate;
  Modbus::Parity parity;
  const char* parityName;
};

bool testConfiguration(const std::string& portName, int baudRate,
                       Modbus::Parity parity, const char* parityName,
                       uint8_t modbusAddr) {
  std::cout << "\nTesting: " << baudRate << " baud, " << parityName
            << ", address " << (int)modbusAddr << std::endl;

  try {
    Modbus::SerialSettings settings;
    settings.portName = portName.c_str();
    settings.baudRate = baudRate;
    settings.dataBits = 8;
    settings.parity = parity;
    settings.stopBits = Modbus::OneStop;
    settings.flowControl = Modbus::NoFlowControl;
    settings.timeoutFirstByte = 1000;  // 1 second
    settings.timeoutInterByte = 100;   // 100 ms

    ModbusClientPort* port =
        Modbus::createClientPort(Modbus::RTU, &settings, true);
    if (!port) {
      std::cout << "  Failed to create port" << std::endl;
      return false;
    }

    ModbusPort* underlying = port->port();
    if (underlying) {
      Modbus::StatusCode status = underlying->open();
      if (status != Modbus::Status_Good) {
        std::cout << "  Failed to open port" << std::endl;
        delete port;
        return false;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    ModbusClient client(modbusAddr, port);

    // Try to read a register to test communication
    uint16_t value = 0;
    Modbus::StatusCode status = client.readHoldingRegisters(0, 1, &value);

    int retries = 0;
    while (status == Modbus::Status_Processing && retries < 10) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      status = client.readHoldingRegisters(0, 1, &value);
      retries++;
    }

    if (underlying) {
      underlying->close();
    }
    delete port;

    if (Modbus::StatusIsGood(status)) {
      std::cout << "  SUCCESS! Read value: " << value << std::endl;
      return true;
    } else {
      std::cout << "  Failed with status: " << (int)status << std::endl;
      return false;
    }

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
      {9600, Modbus::NoParity, "8N1"},   {9600, Modbus::EvenParity, "8E1"},
      {19200, Modbus::NoParity, "8N1"},  {19200, Modbus::EvenParity, "8E1"},
      {38400, Modbus::NoParity, "8N1"},  {57600, Modbus::NoParity, "8N1"},
      {115200, Modbus::NoParity, "8N1"}, {115200, Modbus::EvenParity, "8E1"},
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