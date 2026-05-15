add_library(lebai-cpp)
target_sources(lebai-cpp
  PRIVATE
    src/robot.cc
    src/robot_impl.cc
    src/discovery.cc
    src/discovery_impl.cc
    src/lua_robot.cc
    src/lua_robot_impl.cc
    src/gripper.cc
    src/gripper_impl.cc
    src/modbus_rtu_client.cc
    )
target_include_directories(lebai-cpp
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third>
    $<INSTALL_INTERFACE:include>)
target_compile_features(lebai-cpp PUBLIC cxx_std_17)

if(TARGET nlohmann_json::nlohmann_json)
  target_link_libraries(lebai-cpp PRIVATE nlohmann_json::nlohmann_json)
elseif(TARGET nlohmann_json)
  target_link_libraries(lebai-cpp PRIVATE nlohmann_json)
endif()

if(TARGET httplib::httplib)
  target_link_libraries(lebai-cpp PRIVATE httplib::httplib)
endif()

if(TARGET unofficial::json-rpc-cxx::json-rpc-cxx)
  target_link_libraries(lebai-cpp PRIVATE unofficial::json-rpc-cxx::json-rpc-cxx)
elseif(TARGET json-rpc-cxx)
  target_link_libraries(lebai-cpp PRIVATE json-rpc-cxx)
endif()

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
