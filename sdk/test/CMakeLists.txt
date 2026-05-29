#see
#https://github.com/google/googletest/blob/master/googletest/README.md#incorporating-into-an-existing-cmake-project
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# FetchContent_Declare(googletest
#  GIT_REPOSITORY https://github.com/google/googletest.git
#  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0)
# )
option(INSTALL_GMOCK "Install Googletest's GMock?" OFF)
option(INSTALL_GTEST "Install Googletest's GTest?" OFF)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# find_package(GTest CONFIG REQUIRED)
# find_package(Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/tests")
foreach(OutputConfig IN LISTS CMAKE_CONFIGURATION_TYPES)
  string(TOUPPER ${OutputConfig} OUTPUTCONFIG)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      "${CMAKE_BINARY_DIR}/${OutputConfig}/${CMAKE_INSTALL_BINDIR}/tests")
endforeach()

add_executable(test_json_protos test_json_protos.cc)
target_include_directories(test_json_protos PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/sdk/src
)
target_link_directories(test_json_protos PRIVATE
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
)
target_link_libraries(test_json_protos PRIVATE
  ${PROJECT_NAMESPACE}::lebai-cpp
  gtest)
if(LEBAI_NLOHMANN_JSON_TARGET)
  target_link_libraries(test_json_protos PRIVATE ${LEBAI_NLOHMANN_JSON_TARGET})
endif()
set_target_properties(test_json_protos PROPERTIES
  INSTALL_RPATH
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
add_test(NAME TestJsonProtos COMMAND test_json_protos WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/test)

add_executable(test_modbus_rtu_client test_modbus_rtu_client.cc)
target_include_directories(test_modbus_rtu_client PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/sdk/src
)
target_link_libraries(test_modbus_rtu_client PRIVATE
  ${PROJECT_NAMESPACE}::lebai-cpp
  gtest)
set_target_properties(test_modbus_rtu_client PROPERTIES
  INSTALL_RPATH
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
add_test(NAME TestModbusRtuClient COMMAND test_modbus_rtu_client WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/test)

add_test(
  NAME DotnetPackagingTemplates
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_dotnet_packaging_templates.cmake)

add_test(
  NAME DotnetCiAggregation
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_dotnet_ci_aggregation.cmake)

add_test(
  NAME PythonCiAggregation
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_python_ci_aggregation.cmake)

add_test(
  NAME PythonFacadePackaging
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_python_facade_packaging.cmake)

add_test(
  NAME JavaCiRelease
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_java_ci_release.cmake)

add_test(
  NAME JavaFacadePackaging
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_java_facade_packaging.cmake)

add_test(
  NAME ExamplesLayout
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_examples_layout.cmake)

add_test(
  NAME WorkflowLayout
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_workflow_layout.cmake)

add_test(
  NAME CMakeDependencyBoundaries
  COMMAND ${CMAKE_COMMAND}
    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/test_cmake_dependency_boundaries.cmake)

add_executable(test_rpc_client test_rpc_client.cc)
target_include_directories(test_rpc_client PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/sdk/src
)
target_link_libraries(test_rpc_client PRIVATE
  gtest)
if(LEBAI_NLOHMANN_JSON_TARGET)
  target_link_libraries(test_rpc_client PRIVATE ${LEBAI_NLOHMANN_JSON_TARGET})
endif()
if(LEBAI_HTTPLIB_TARGET)
  target_link_libraries(test_rpc_client PRIVATE ${LEBAI_HTTPLIB_TARGET})
endif()
if(LEBAI_JSON_RPC_CXX_TARGET)
  target_link_libraries(test_rpc_client PRIVATE ${LEBAI_JSON_RPC_CXX_TARGET})
endif()
set_target_properties(test_rpc_client PROPERTIES
  INSTALL_RPATH
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
add_test(NAME TestRpcClient COMMAND test_rpc_client WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/test)

add_executable(test_robot test_robot.cc)
if(DEFINED TEST_ROBOT_IP)
  target_compile_definitions(test_robot PRIVATE TEST_L_MASTER_IP="${TEST_ROBOT_IP}")
else()
  target_compile_definitions(test_robot PRIVATE TEST_L_MASTER_IP="127.0.0.1")
endif()


target_include_directories(test_robot PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/sdk/src
)
target_link_directories(test_robot PRIVATE  
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
)
target_link_libraries(test_robot PRIVATE
  ${PROJECT_NAMESPACE}::lebai-cpp
  gtest)
set_target_properties(test_robot PROPERTIES
  INSTALL_RPATH 
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})  
add_test(NAME TestRobot COMMAND test_robot WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/test)



add_executable(test_lua_robot test_lua_robot.cc)
if(DEFINED TEST_ROBOT_IP)
  target_compile_definitions(test_lua_robot PRIVATE TEST_L_MASTER_IP="${TEST_ROBOT_IP}")
else()
  target_compile_definitions(test_lua_robot PRIVATE TEST_L_MASTER_IP="127.0.0.1")
endif()


target_include_directories(test_lua_robot PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/sdk/src
)
target_link_directories(test_lua_robot PRIVATE  
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
)
target_link_libraries(test_lua_robot PRIVATE
  ${PROJECT_NAMESPACE}::lebai-cpp
  gtest)
set_target_properties(test_lua_robot PROPERTIES
  INSTALL_RPATH 
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})  
add_test(NAME TestLuaRobot COMMAND test_lua_robot WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/test)




add_executable(test_discovery test_discovery.cc)
target_include_directories(test_discovery PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/sdk/src
)
target_link_directories(test_discovery PRIVATE  
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
)
target_link_libraries(test_discovery PRIVATE
  ${PROJECT_NAMESPACE}::lebai-cpp
  gtest)


set_target_properties(test_discovery PROPERTIES
  INSTALL_RPATH 
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})  
add_test(NAME TestDiscovery COMMAND test_discovery WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/test)
# add_executable(test_discovery test_discovery.cc)
# target_include_directories(test_discovery
#   PUBLIC
#     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
#     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sdk/third>
#     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sdk/third/zeroconf>
#     $<INSTALL_INTERFACE:include>)
