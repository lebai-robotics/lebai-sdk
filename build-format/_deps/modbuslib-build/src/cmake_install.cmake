# Install script for directory: D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/affairs/lebai/lebai-sdk/build-format/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib" TYPE STATIC_LIBRARY FILES "D:/affairs/lebai/lebai-sdk/build-format/Debug/bin/modbus.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib" TYPE STATIC_LIBRARY FILES "D:/affairs/lebai/lebai-sdk/build-format/Release/bin/modbus.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib" TYPE STATIC_LIBRARY FILES "D:/affairs/lebai/lebai-sdk/build-format/MinSizeRel/bin/modbus.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib" TYPE STATIC_LIBRARY FILES "D:/affairs/lebai/lebai-sdk/build-format/RelWithDebInfo/bin/modbus.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./include/modbus" TYPE FILE FILES
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/Modbus_config.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusPlatform.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusGlobal.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/Modbus.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusObject.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusPort.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusSerialPort.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusRtuPort.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusAscPort.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusTcpPort.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusClient.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/ModbusClientPort.h"
    "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src/src/cModbus.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    include("D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-build/src/CMakeFiles/modbus.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    include("D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-build/src/CMakeFiles/modbus.dir/install-cxx-module-bmi-Release.cmake" OPTIONAL)
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    include("D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-build/src/CMakeFiles/modbus.dir/install-cxx-module-bmi-MinSizeRel.cmake" OPTIONAL)
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    include("D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-build/src/CMakeFiles/modbus.dir/install-cxx-module-bmi-RelWithDebInfo.cmake" OPTIONAL)
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-build/src/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
