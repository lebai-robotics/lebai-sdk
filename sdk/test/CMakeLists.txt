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

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# find_package(GTest CONFIG REQUIRED)
# find_package(Threads)


add_executable(test_protos test_protos.cc)
target_include_directories(test_protos PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/sdk/src
)
target_link_directories(test_protos PRIVATE  
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
)
target_link_libraries(test_protos PRIVATE
  ${PROJECT_NAMESPACE}::lebai-cpp
  gtest)


set_target_properties(test_protos PROPERTIES
  INSTALL_RPATH 
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})  
add_test(NAME TestProtos COMMAND test_protos WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sdk/test)




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