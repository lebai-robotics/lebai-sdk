if(NOT DEFINED BUILD_DIR OR BUILD_DIR STREQUAL "")
  message(FATAL_ERROR "BUILD_DIR is required")
endif()
if(NOT DEFINED INSTALL_PREFIX OR INSTALL_PREFIX STREQUAL "")
  message(FATAL_ERROR "INSTALL_PREFIX is required")
endif()

file(REMOVE_RECURSE "${INSTALL_PREFIX}")

set(install_command
  "${CMAKE_COMMAND}" --install "${BUILD_DIR}" --prefix "${INSTALL_PREFIX}")
if(DEFINED BUILD_CONFIG AND NOT BUILD_CONFIG STREQUAL "")
  list(APPEND install_command --config "${BUILD_CONFIG}")
endif()

execute_process(
  COMMAND ${install_command}
  RESULT_VARIABLE install_result
  OUTPUT_VARIABLE install_output
  ERROR_VARIABLE install_error)
if(NOT install_result EQUAL 0)
  message(FATAL_ERROR
    "Staged install failed (${install_result})\n${install_output}\n${install_error}")
endif()

foreach(header IN ITEMS
    config.hh
    discovery.hh
    gripper.hh
    lebai.hh
    lua_robot.hh
    robot.hh)
  set(installed_header "${INSTALL_PREFIX}/include/lebai/${header}")
  if(NOT EXISTS "${installed_header}")
    message(FATAL_ERROR "Missing installed public header: ${installed_header}")
  endif()
endforeach()
