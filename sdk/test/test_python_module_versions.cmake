foreach(required IN ITEMS PROJECT_SOURCE_DIR TEST_ROOT PYTHON_EXECUTABLE)
  if(NOT DEFINED ${required} OR "${${required}}" STREQUAL "")
    message(FATAL_ERROR "${required} is required")
  endif()
endforeach()

function(expect_contains text expected context)
  string(FIND "${text}" "${expected}" position)
  if(position EQUAL -1)
    message(FATAL_ERROR
      "${context}: expected output to contain '${expected}'\n${text}")
  endif()
endfunction()

set(fixture "${TEST_ROOT}/fixture")
set(driver "${TEST_ROOT}/driver.cmake")
file(REMOVE_RECURSE "${TEST_ROOT}")
file(MAKE_DIRECTORY "${fixture}/pip")

file(WRITE "${fixture}/pip/__init__.py" "")
file(WRITE "${fixture}/pip/__main__.py" [=[
import pathlib
import sys

root = pathlib.Path(__file__).resolve().parents[1]
arguments = sys.argv[1:]
(root / "pip-args.txt").write_text("\n".join(arguments), encoding="utf-8")

expected = ["install", "--user", "--upgrade", "setuptools>=61.0"]
if arguments != expected:
    print(f"unexpected pip arguments: {arguments!r}", file=sys.stderr)
    raise SystemExit(41)

mode = (root / "pip-mode.txt").read_text(encoding="utf-8").strip()
if mode == "fail":
    print("fake pip failure", file=sys.stderr)
    raise SystemExit(42)
if mode == "upgrade":
    (root / "setuptools.py").write_text(
        '__version__ = "61.0"\n', encoding="utf-8"
    )
elif mode != "noop":
    print(f"unknown fake pip mode: {mode}", file=sys.stderr)
    raise SystemExit(43)
]=])

file(WRITE "${driver}" [=[
if(NOT DEFINED PROJECT_SOURCE_DIR OR NOT DEFINED Python3_EXECUTABLE)
  message(FATAL_ERROR "Driver inputs are required")
endif()
include("${PROJECT_SOURCE_DIR}/cmake/python_modules.cmake")
search_python_module(
  NAME setuptools
  PACKAGE setuptools
  MIN_VERSION 61.0)
]=])

function(reset_fixture mode)
  file(WRITE "${fixture}/setuptools.py" "__version__ = \"59.6\"\n")
  file(WRITE "${fixture}/pip-mode.txt" "${mode}\n")
  file(REMOVE "${fixture}/pip-args.txt")
endfunction()

function(run_driver fetch result_var output_var)
  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E env
      "PYTHONNOUSERSITE=1"
      "PYTHONDONTWRITEBYTECODE=1"
      "PYTHONPATH=${fixture}"
      "${CMAKE_COMMAND}"
      "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
      "-DPython3_EXECUTABLE=${PYTHON_EXECUTABLE}"
      "-DFETCH_PYTHON_DEPS=${fetch}"
      -P "${driver}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr)
  set(${result_var} "${result}" PARENT_SCOPE)
  set(${output_var} "${stdout}\n${stderr}" PARENT_SCOPE)
endfunction()

set(expected_pip_arguments
  "install\n--user\n--upgrade\nsetuptools>=61.0")

reset_fixture(noop)
run_driver(OFF result output)
if(result EQUAL 0)
  message(FATAL_ERROR "Fetch-OFF scenario unexpectedly succeeded")
endif()
expect_contains("${output}" "found version \"59.6\"" "Fetch-OFF scenario")
expect_contains("${output}" "required version \"61.0\"" "Fetch-OFF scenario")
if(EXISTS "${fixture}/pip-args.txt")
  message(FATAL_ERROR "Fetch-OFF scenario invoked pip")
endif()

reset_fixture(upgrade)
run_driver(ON result output)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "Upgrade scenario failed\n${output}")
endif()
file(READ "${fixture}/setuptools.py" upgraded_module)
expect_contains("${upgraded_module}" "61.0" "Upgrade scenario")
file(READ "${fixture}/pip-args.txt" pip_arguments)
if(NOT pip_arguments STREQUAL expected_pip_arguments)
  message(FATAL_ERROR
    "Upgrade scenario used unexpected pip arguments: '${pip_arguments}'")
endif()

reset_fixture(fail)
run_driver(ON result output)
if(result EQUAL 0)
  message(FATAL_ERROR "Pip-failure scenario unexpectedly succeeded")
endif()
expect_contains("${output}" "Failed to install Python module \"setuptools\""
  "Pip-failure scenario")
expect_contains("${output}" "42" "Pip-failure scenario")

reset_fixture(noop)
run_driver(ON result output)
if(result EQUAL 0)
  message(FATAL_ERROR "No-op pip scenario unexpectedly succeeded")
endif()
expect_contains("${output}" "still found version \"59.6\""
  "No-op pip scenario")
expect_contains("${output}" "required version \"61.0\""
  "No-op pip scenario")
