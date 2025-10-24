set_property(SOURCE l_master.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE l_master.i PROPERTY COMPILE_OPTIONS
  -package ${JAVA_PACKAGE})
swig_add_library(jniLMaster
  TYPE OBJECT
  LANGUAGE java
  OUTPUT_DIR ${JAVA_PROJECT_DIR}/${JAVA_SRC_PATH}/
  SOURCES l_master.i)

set_target_properties(jniLMaster PROPERTIES
  SWIG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/java
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  POSITION_INDEPENDENT_CODE ON)
target_include_directories(jniLMaster PRIVATE 
  ${PROJECT_SOURCE_DIR}/sdk/include
  ${PROJECT_SOURCE_DIR}/sdk/src
  ${JNI_INCLUDE_DIRS}
)
target_link_libraries(jniLMaster PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)


set_property(SOURCE zeroconf.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE zeroconf.i PROPERTY COMPILE_OPTIONS
  -package ${JAVA_PACKAGE})
swig_add_library(jniZeroconf
  TYPE OBJECT
  LANGUAGE java
  OUTPUT_DIR ${JAVA_PROJECT_DIR}/${JAVA_SRC_PATH}/
  SOURCES zeroconf.i)

set_target_properties(jniZeroconf PROPERTIES
  SWIG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/java
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  POSITION_INDEPENDENT_CODE ON)
target_include_directories(jniZeroconf PRIVATE 
  ${PROJECT_SOURCE_DIR}/sdk/include
  ${PROJECT_SOURCE_DIR}/sdk/src
  ${JNI_INCLUDE_DIRS}
)
target_link_libraries(jniZeroconf PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)


# Gripper module
set_property(SOURCE gripper.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE gripper.i PROPERTY COMPILE_OPTIONS
  -package ${JAVA_PACKAGE})
swig_add_library(jniGripper
  TYPE OBJECT
  LANGUAGE java
  OUTPUT_DIR ${JAVA_PROJECT_DIR}/${JAVA_SRC_PATH}/
  SOURCES gripper.i)

set_target_properties(jniGripper PROPERTIES
  SWIG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/java
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  POSITION_INDEPENDENT_CODE ON)
target_include_directories(jniGripper PRIVATE
  ${PROJECT_SOURCE_DIR}/sdk/include
  ${PROJECT_SOURCE_DIR}/sdk/src
  ${JNI_INCLUDE_DIRS}
)
target_link_libraries(jniGripper PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)
