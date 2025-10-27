cmake_minimum_required(VERSION 3.15)
if(POLICY CMP0122)
  cmake_policy(SET CMP0122 NEW)
endif()
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()
# SWIG policies - use modern SWIG target naming
if(POLICY CMP0078)
  cmake_policy(SET CMP0078 NEW)
endif()
if(POLICY CMP0086)
  cmake_policy(SET CMP0086 NEW)
endif()

project(
  lebai
  VERSION 1.3.6
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAMESPACE lebai)

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(isMultiConfig)
  if(NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES
        "Release;Debug"
        CACHE
          STRING
          "Choose the type of builds, options are: Debug Release RelWithDebInfo MinSizeRel. (default: Release;Debug)"
          FORCE)
  endif()
  message(STATUS "Configuration types: ${CMAKE_CONFIGURATION_TYPES}")
else()
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE
          STRING
          "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel. (default: Release)"
          FORCE)
  endif()
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

# Layout build dir like install dir
include(GNUInstallDirs)
if(UNIX)
  option(BUILD_SHARED_LIBS "Build shared libraries (.so or .dyld)." ON)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
  # for multi-config build system (e.g. Xcode, Ninja Multi-Config)
  foreach(OutputConfig IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${OutputConfig} OUTPUTCONFIG)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/${OutputConfig}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/${OutputConfig}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/${OutputConfig}/${CMAKE_INSTALL_BINDIR})
  endforeach()
else()
  # Currently Only support static build for windows
  option(BUILD_SHARED_LIBS "Build shared libraries (.dll)." OFF)
  add_compile_definitions(_USE_MATH_DEFINES)
  add_compile_definitions(_WEBSOCKETPP_CPP11_RANDOM_DEVICE_)
  add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)  # Suppress inet_addr deprecation from websocketpp
  # add_compile_definitions(NOMINMAX) add_compile_definitions(_DEBUG)
  if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
  endif()
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
  # for multi-config builds (e.g. msvc)
  foreach(OutputConfig IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${OutputConfig} OUTPUTCONFIG)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/${OutputConfig}/${CMAKE_INSTALL_BINDIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/${OutputConfig}/${CMAKE_INSTALL_BINDIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/${OutputConfig}/${CMAKE_INSTALL_BINDIR})
  endforeach()
endif()

option(ENABLE_TSAN "Enable thread sanitier" OFF)
if(UNIX)
  if(ENABLE_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=thread")
  endif()
endif()

# By default only the C++ library is built.
option(BUILD_CXX "Build C++ library" ON)
message(STATUS "Build C++ library: ${BUILD_CXX}")

option(BUILD_PYTHON "Build Python Library" OFF)
message(STATUS "Build Python: ${BUILD_PYTHON}")

option(BUILD_DOTNET "Build .NET Library" OFF)
message(STATUS "Build .Net: ${BUILD_DOTNET}")

option(BUILD_JAVA "Build Java Library" OFF)
message(STATUS "Build Java: ${BUILD_JAVA}")
# if(UNIX)
if(BUILD_PYTHON)
  option(FETCH_PYTHON_DEPS "Install python required modules if not available"
         ON)
  message(STATUS "Python fetch dependencies: ${FETCH_PYTHON_DEPS}")
  if(DEFINED PYTHONPATH)
    message(STATUS "Python path: ${PYTHONPATH}")
    set(Python3_ROOT_DIR ${PYTHONPATH})
  endif()
  include(python)
endif()
# Disable CTest targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)
include(cpp)

# Fetch ModbusLib
message(STATUS "Fetching ModbusLib...")
include(FetchContent)
set(BUILD_SHARED_LIBS_SAVE ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF) # Force ModbusLib to build as static
set(MB_CLIENT_DISABLE OFF CACHE BOOL "Enable Modbus client" FORCE)
set(MB_SERVER_DISABLE ON CACHE BOOL "Disable Modbus server" FORCE)
set(MB_EXAMPLES_ENABLED OFF CACHE BOOL "Disable ModbusLib examples" FORCE)
set(MB_TESTS_ENABLED OFF CACHE BOOL "Disable ModbusLib tests" FORCE)
FetchContent_Declare(
  ModbusLib
  GIT_REPOSITORY https://github.com/serhmarch/ModbusLib.git
  GIT_TAG        main  # or specify a specific tag/commit

)
FetchContent_MakeAvailable(ModbusLib)
# Set modbus as a static library that won't be installed
set_target_properties(modbus PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  EXCLUDE_FROM_ALL TRUE
)
set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_SAVE})

if(WIN32
   AND (BUILD_DOTNET
        OR BUILD_JAVA
        OR BUILD_PYTHON))
  message(STATUS "Getting SWIG: ...")
  # add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
  include(swig)
  message(STATUS "Getting SWIG: ...DONE")
endif()

if(BUILD_PYTHON)
  option(FETCH_PYTHON_DEPS "Install python required modules if not available"
         ON)
  message(STATUS "Python fetch dependencies: ${FETCH_PYTHON_DEPS}")
endif()

if(BUILD_DOTNET)
  option(USE_DOTNET_8 "Use .Net 8.0 LTS support" ON)
  message(STATUS ".Net: Use .Net 8.0 LTS support: ${USE_DOTNET_8}")
endif()
include(dotnet)

if(BUILD_JAVA)
  option(SKIP_GPG "Skip gpg sign" ON)
  message(STATUS "Maven skip GPG sign: ${SKIP_GPG}")
endif()
include(java)

# add_subdirectory(tests)

option(BUILD_EXAMPLES "Build examples" ON)
message(STATUS "Build examples: ${BUILD_EXAMPLES}")
add_subdirectory(examples)

set(SDK_DIR "${CMAKE_SOURCE_DIR}/sdk")
# Get all .cc, .c, .cpp, .h, .hh, .hpp files in the SDK directory
file(
  GLOB_RECURSE
  SDK_CXX_FILES
  "${SDK_DIR}/*.cc"
  "${SDK_DIR}/*.c"
  "${SDK_DIR}/*.cpp"
  "${SDK_DIR}/*.h"
  "${SDK_DIR}/*.hh"
  "${SDK_DIR}/*.hpp")
option(CLANG_FORMAT_CHECK "Run clang-format in check mode" OFF)
message(STATUS "Clang format check: ${CLANG_FORMAT_CHECK}")

list(FILTER SDK_CXX_FILES EXCLUDE REGEX "${SDK_DIR}/include/lebai/config.hh")

# Create the clang-format target
if(CLANG_FORMAT_CHECK)
  message(STATUS "Add custom target clang-format")
  add_custom_target(clang-format ALL COMMAND clang-format -style=file --Werror
                                             --dry-run ${SDK_CXX_FILES})
else()
  add_custom_target(clang-format COMMAND clang-format -style=file -i
                                         ${SDK_CXX_FILES})
endif()

find_package(Doxygen)
option(BUILD_DOCUMENTATION
       "Create and install the HTML based API documentation (requires Doxygen)"
       ON)

if(BUILD_DOCUMENTATION AND DOXYGEN_FOUND)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(
    doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
          DESTINATION share/doc/${PROJECT_NAME})
endif()
