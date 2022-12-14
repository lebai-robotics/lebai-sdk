set_property(SOURCE l_master.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE l_master.i PROPERTY SWIG_MODULE_NAME l_master)
swig_add_library(l_master
  TYPE MODULE
  LANGUAGE python
  OUTPUT_DIR ${PYTHON_PROJECT_DIR}
  SOURCES l_master.i)
add_library(${PROJECT_NAMESPACE}::l_master ALIAS l_master)

target_include_directories(l_master
  PRIVATE
  ${PROJECT_SOURCE_DIR}/sdk/include
  ${PROJECT_SOURCE_DIR}/sdk/src
  ${Python3_INCLUDE_DIRS}
  )
set_property(TARGET l_master PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)

# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_target_properties(l_master PROPERTIES
    SUFFIX ".so"
    INSTALL_RPATH "@loader_path;@loader_path/../../${PYTHON_PROJECT}/.libs"
    )
  set_property(TARGET l_master APPEND PROPERTY
    LINK_FLAGS "-flat_namespace -undefined suppress"
    )
elseif(UNIX)
  set_target_properties(l_master PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../${PYTHON_PROJECT}/.libs"
    )
endif()
target_link_libraries(l_master PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)

# Variable PYTHON_LIBRARIES can contains keyword `optimized`
# which won't be interpreted inside a generator expression.
# i.e. we can't use: $<$<PLATFORM_ID:Windows>:${PYTHON_LIBRARIES}>
# see: https://cmake.org/cmake/help/git-stage/command/target_link_libraries.html#command:target_link_libraries
if(MSVC)
  target_link_libraries(l_master PRIVATE ${Python3_LIBRARIES})
endif()


set_property(SOURCE zeroconf.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE zeroconf.i PROPERTY SWIG_MODULE_NAME zeroconf)
swig_add_library(zeroconf
  TYPE MODULE
  LANGUAGE python
  OUTPUT_DIR ${PYTHON_PROJECT_DIR}
  SOURCES zeroconf.i)
add_library(${PROJECT_NAMESPACE}::zeroconf ALIAS zeroconf)

target_include_directories(zeroconf
  PRIVATE
  ${PROJECT_SOURCE_DIR}/sdk/include
  ${PROJECT_SOURCE_DIR}/sdk/include/zeroconf
  ${PROJECT_SOURCE_DIR}/sdk/src
  ${Python3_INCLUDE_DIRS}
  )
set_property(TARGET zeroconf PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)

# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_target_properties(zeroconf PROPERTIES
    SUFFIX ".so"
    INSTALL_RPATH "@loader_path;@loader_path/../../${PYTHON_PROJECT}/.libs"
    )
  set_property(TARGET zeroconf APPEND PROPERTY
    LINK_FLAGS "-flat_namespace -undefined suppress"
    )
elseif(UNIX)
  set_target_properties(zeroconf PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../${PYTHON_PROJECT}/.libs"
    )
endif()
target_link_libraries(zeroconf PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)

# Variable PYTHON_LIBRARIES can contains keyword `optimized`
# which won't be interpreted inside a generator expression.
# i.e. we can't use: $<$<PLATFORM_ID:Windows>:${PYTHON_LIBRARIES}>
# see: https://cmake.org/cmake/help/git-stage/command/target_link_libraries.html#command:target_link_libraries
if(MSVC)
  target_link_libraries(zeroconf PRIVATE ${Python3_LIBRARIES})
endif()
