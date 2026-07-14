set(ROOT "${PROJECT_SOURCE_DIR}")

file(GLOB_RECURSE example_sources
  RELATIVE "${ROOT}/examples"
  "${ROOT}/examples/*.cc"
  "${ROOT}/examples/*.py"
  "${ROOT}/examples/*.cs"
  "${ROOT}/examples/*.java")

if(NOT example_sources)
  message(FATAL_ERROR "Expected examples under examples/<category>/")
endif()

foreach(path IN LISTS example_sources)
  string(REGEX MATCHALL "/" separators "${path}")
  list(LENGTH separators depth)
  if(NOT depth EQUAL 1)
    message(FATAL_ERROR "Example source must live directly under examples/<category>/: ${path}")
  endif()
endforeach()

file(READ "${ROOT}/examples/CMakeLists.txt" examples_cmake)
if(NOT examples_cmake MATCHES "GLOB_RECURSE CPP_SRCS")
  message(FATAL_ERROR "examples/CMakeLists.txt must discover C++ examples recursively")
endif()
