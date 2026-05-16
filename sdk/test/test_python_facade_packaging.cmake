set(_root "${PROJECT_SOURCE_DIR}")

foreach(_path IN ITEMS
  "python/pylebai/__init__.py"
  "python/pylebai/robot.py")
  if(NOT EXISTS "${_root}/${_path}")
    message(FATAL_ERROR "Missing Python facade file: ${_path}")
  endif()
endforeach()

file(READ "${_root}/python/pylebai/__init__.py" _python_init)
if(NOT _python_init MATCHES "from \\.robot import Robot")
  message(FATAL_ERROR "pylebai.__init__ must export the Python Robot facade")
endif()

file(READ "${_root}/cmake/python.cmake" _python_cmake)
if(NOT _python_cmake MATCHES "python/\\$\\{PYTHON_PROJECT\\}/robot\\.py")
  message(FATAL_ERROR "Python package target must depend on the Robot facade source")
endif()

file(READ "${_root}/examples/example.py" _example)
if(NOT _example MATCHES "from pylebai import Robot, zeroconf")
  message(FATAL_ERROR "Python example must use the public Robot facade")
endif()

file(READ "${_root}/docs/python.md" _docs)
if(NOT _docs MATCHES "from pylebai import Robot")
  message(FATAL_ERROR "Python docs must show the public Robot facade")
endif()
