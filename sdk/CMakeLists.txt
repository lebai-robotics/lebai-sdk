add_library(lebai-cpp)
file(GLOB PROTOS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/protos/*.cc")
target_sources(lebai-cpp
  PRIVATE
    src/robot.cc
    src/robot_impl.cc
    src/jsonrpc_connector.cc
    src/discovery.cc
    src/discovery_impl.cc
    src/lua_robot.cc
    src/lua_robot_impl.cc
    src/gripper.cc
    src/gripper_impl.cc
    ${PROTOS_SRC}
    )
target_include_directories(lebai-cpp
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_BINARY_DIR}/_deps/modbuslib-src/src)
target_compile_features(lebai-cpp PUBLIC cxx_std_14)

# Link modbus as a static library - use $<BUILD_INTERFACE:> to exclude from export
target_link_libraries(lebai-cpp PRIVATE $<BUILD_INTERFACE:modbus>)

if(WIN32)
  target_link_libraries(lebai-cpp INTERFACE iphlpapi ws2_32)
endif()

set_target_properties(lebai-cpp PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER "include/lebai/robot.hh;include/lebai/lebai.hh;include/lebai/discovery.hh;include/lebai/config.hh;include/lebai/gripper.hh"
  )
add_library(${PROJECT_NAMESPACE}::lebai-cpp ALIAS lebai-cpp)

option(BUILD_TESTING "Build test" ON)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()


# Install
install(TARGETS lebai-cpp
  EXPORT ${PROJECT_NAME}Targets
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)