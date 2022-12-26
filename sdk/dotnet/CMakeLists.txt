set_property(SOURCE l_master.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE l_master.i PROPERTY COMPILE_OPTIONS
  -namespace ${DOTNET_PROJECT}.l_master
  -dllimport lebai-native)
swig_add_library(dotnet_l_master
  TYPE OBJECT
  LANGUAGE csharp
  OUTPUT_DIR ${DOTNET_PROJECT_DIR}
  SOURCES l_master.i)

set_target_properties(dotnet_l_master PROPERTIES
  SWIG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/dotnet
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  POSITION_INDEPENDENT_CODE ON)
target_include_directories(dotnet_l_master
  PRIVATE
  ${PROJECT_SOURCE_DIR}/sdk/include
  ${PROJECT_SOURCE_DIR}/sdk/src
  )
target_link_libraries(dotnet_l_master PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)

set_property(SOURCE zeroconf.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE zeroconf.i PROPERTY COMPILE_OPTIONS
  -namespace ${DOTNET_PROJECT}.zeroconf
  -dllimport lebai-native)
swig_add_library(dotnet_zeroconf
  TYPE OBJECT
  LANGUAGE csharp
  OUTPUT_DIR ${DOTNET_PROJECT_DIR}
  SOURCES zeroconf.i)

set_target_properties(dotnet_zeroconf PROPERTIES
  SWIG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/dotnet
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  POSITION_INDEPENDENT_CODE ON)
target_include_directories(dotnet_zeroconf
  PRIVATE
  ${PROJECT_SOURCE_DIR}/sdk/include
  ${PROJECT_SOURCE_DIR}/sdk/src
  )
target_link_libraries(dotnet_zeroconf PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)
