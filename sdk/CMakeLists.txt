add_library(lebai-cpp)
file(GLOB PROTOS_SRC RELATIVE ${CMAKE_SOURCE_DIR}/sdk "src/protos/*.cc")
target_sources(lebai-cpp
  PRIVATE
    include/lebai/robot.hh
    src/robot.cc
    src/robot_impl.cc
    src/jsonrpc_connector.cc
    src/discovery.cc
    src/discovery_impl.cc
    ${PROTOS_SRC}
    )
target_include_directories(lebai-cpp
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third/zeroconf>
    $<INSTALL_INTERFACE:include>)
target_compile_features(lebai-cpp PUBLIC cxx_std_14)
set_target_properties(lebai-cpp PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER include/lebai/robot.hh
  PUBLIC_HEADER include/lebai/lebai.hh
  PUBLIC_HEADER include/lebai/discovery.hh
  # PUBLIC_HEADER include/lebai/jsonbase.hh
  # PUBLIC_HEADER include/lebai/motion.hh
  # PUBLIC_HEADER include/lebai/posture.hh
  )
add_library(${PROJECT_NAMESPACE}::lebai-cpp ALIAS lebai-cpp)




if(BUILD_TESTING)
  add_subdirectory(test)
endif()

# Install
install(TARGETS lebai-cpp
  EXPORT ${PROJECT_NAME}Targets
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )