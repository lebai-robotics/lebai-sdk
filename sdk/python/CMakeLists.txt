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
)
if(BUILD_PYTHON)
  target_include_directories(l_master
    PRIVATE
    ${Python3_INCLUDE_DIRS}
  )
elseif(BUILD_PYTHON2)
  target_include_directories(l_master
    PRIVATE
    ${Python2_INCLUDE_DIRS}
  )
endif()

set_property(TARGET l_master PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)
# get_target_property(nd l_master COMPILE_DEFINITIONS)

if(UNIX)
  set_target_properties(l_master PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../${PYTHON_PROJECT}/.libs"
    )
endif()
target_link_libraries(l_master PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)

# Variable PYTHON_LIBRARIES can contains keyword `optimized`
# which won't be interpreted inside a generator expression.
# i.e. we can't use: $<$<PLATFORM_ID:Windows>:${PYTHON_LIBRARIES}>
# see: https://cmake.org/cmake/help/git-stage/command/target_link_libraries.html#command:target_link_libraries
message(STATUS "PYTHON_LIBRARIES: ${Python3_LIBRARIES}")
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
)
if(BUILD_PYTHON)
  target_include_directories(zeroconf
    PRIVATE
    ${Python3_INCLUDE_DIRS}
  )
elseif(BUILD_PYTHON2)
  target_include_directories(zeroconf
    PRIVATE
    ${Python2_INCLUDE_DIRS}
  )
endif()
set_property(TARGET zeroconf PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)

if(UNIX)
  set_target_properties(zeroconf PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../${PYTHON_PROJECT}/.libs"
    )
endif()
target_link_libraries(zeroconf PRIVATE ${PROJECT_NAMESPACE}::lebai-cpp)

# Variable PYTHON_LIBRARIES can contains keyword `optimized`
# which won't be interpreted inside a generator expression.
# i.e. we can't use: $<$<PLATFORM_ID:Windows>:${PYTHON_LIBRARIES}>
# see: https://cmake.org/cmake/help/git-stage/command/target_link_libraries.html#command:target_link_libraries
# if(MSVC)
#   target_link_libraries(zeroconf PRIVATE ${Python2_LIBRARIES} ${Python3_LIBRARIES})
# endif()
if(BUILD_PYTHON)
  if(MSVC)
    target_link_libraries(zeroconf PRIVATE ${Python3_LIBRARIES})
  endif()
elseif(BUILD_PYTHON2)
  if(MSVC)
    target_link_libraries(zeroconf PRIVATE ${Pytho2_LIBRARIES})
  endif()
endif()
