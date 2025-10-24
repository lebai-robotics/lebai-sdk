if(NOT BUILD_EXAMPLES)
  return()
endif()


file(GLOB CPP_SRCS "*.cc")
foreach(FILE_NAME IN LISTS CPP_SRCS)
  get_filename_component(EXAMPLE_NAME ${FILE_NAME} NAME_WE)

  # Special handling for test_gripper_diagnostics
  if(EXAMPLE_NAME STREQUAL "test_gripper_diagnostics")
    add_executable(${EXAMPLE_NAME} ${FILE_NAME})
    target_include_directories(${EXAMPLE_NAME}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/sdk/src
        ${CMAKE_BINARY_DIR}/_deps/modbuslib-src/src
    )
    target_compile_features(${EXAMPLE_NAME} PRIVATE cxx_std_14)
    target_link_libraries(${EXAMPLE_NAME} PRIVATE modbus)
    if(WIN32)
      target_link_libraries(${EXAMPLE_NAME} PRIVATE ws2_32)
    endif()
  else()
    add_cpp_example(${FILE_NAME})
  endif()
endforeach()

if(BUILD_DOTNET)
  file(GLOB DOTNET_SRCS "*.cs")
  foreach(FILE_NAME IN LISTS DOTNET_SRCS)
    add_dotnet_example(${FILE_NAME})
  endforeach()
endif()

if(BUILD_JAVA)
  file(GLOB JAVA_SRCS "*.java")
  foreach(FILE_NAME IN LISTS JAVA_SRCS)
    add_java_example(${FILE_NAME})
  endforeach()
endif()