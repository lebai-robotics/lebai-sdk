set(ROOT "${PROJECT_SOURCE_DIR}")

function(expect_file path)
  if(NOT EXISTS "${ROOT}/${path}")
    message(FATAL_ERROR "Expected example file: ${path}")
  endif()
endfunction()

function(expect_missing path)
  if(EXISTS "${ROOT}/${path}")
    message(FATAL_ERROR "Deprecated example should not exist: ${path}")
  endif()
endfunction()

foreach(path IN ITEMS
  "examples/robot/robot.cc"
  "examples/robot/robot.py"
  "examples/robot/RobotExample.cs"
  "examples/robot/RobotExample.java"
  "examples/gripper/gripper.cc"
  "examples/gripper/gripper.py"
  "examples/gripper/GripperExample.cs"
  "examples/gripper/GripperExample.java"
  "examples/discovery/discovery.cc"
  "examples/discovery/discovery.py"
  "examples/discovery/DiscoveryExample.cs"
  "examples/discovery/DiscoveryExample.java")
  expect_file("${path}")
endforeach()

foreach(path IN ITEMS
  "examples/example.cc"
  "examples/example.py"
  "examples/example.cs"
  "examples/example.java"
  "examples/example_gripper.cc"
  "examples/example_gripper.py"
  "examples/example_gripper.cs"
  "examples/example_gripper.java"
  "examples/example_lua_robot.cc"
  "examples/example_online_move.cc"
  "examples/example_speedl.cc"
  "examples/multi_thread.cc"
  "examples/test_gripper_diagnostics.cc")
  expect_missing("${path}")
endforeach()

file(READ "${ROOT}/examples/CMakeLists.txt" examples_cmake)
if(NOT examples_cmake MATCHES "GLOB_RECURSE CPP_SRCS")
  message(FATAL_ERROR "examples/CMakeLists.txt must discover C++ examples recursively")
endif()
