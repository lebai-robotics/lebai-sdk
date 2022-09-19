add_library(lebai-sdk)
target_sources(lebai-sdk
  PRIVATE
    include/lebai-sdk/robot.hh
    src/robot.cc
    src/robot_impl.cc
    src/jsonrpc_connector.cc
    src/posture.cc
    src/motion.cc
    src/jsonbase.cc
    )
target_include_directories(lebai-sdk
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third>
    $<INSTALL_INTERFACE:include>)
target_compile_features(lebai-sdk PUBLIC cxx_std_14)
set_target_properties(lebai-sdk PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER include/lebai-sdk/robot.hh)
#target_link_libraries(Foo PUBLIC ...)
add_library(${PROJECT_NAMESPACE}::lebai-sdk ALIAS lebai-sdk)

# add_subdirectory(tests)

# Install
install(TARGETS lebai-sdk
  EXPORT ${PROJECT_NAME}Targets
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )