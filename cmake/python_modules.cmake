function(_probe_python_module module_name no_version result_var version_var)
  if(no_version)
    execute_process(
      COMMAND "${Python3_EXECUTABLE}" -c
        "import importlib; importlib.import_module('${module_name}')"
      RESULT_VARIABLE probe_result
      ERROR_QUIET)
    set(module_version "unknown")
  else()
    execute_process(
      COMMAND "${Python3_EXECUTABLE}" -c
        "import importlib; module = importlib.import_module('${module_name}'); print(module.__version__)"
      RESULT_VARIABLE probe_result
      OUTPUT_VARIABLE module_version
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif()

  set(${result_var} "${probe_result}" PARENT_SCOPE)
  set(${version_var} "${module_version}" PARENT_SCOPE)
endfunction()

# Find a Python module and optionally enforce a minimum version. When
# FETCH_PYTHON_DEPS is enabled, install or upgrade the package in the Python
# user site and verify the module again before returning.
function(search_python_module)
  set(options NO_VERSION)
  set(one_value_args NAME PACKAGE MIN_VERSION)
  cmake_parse_arguments(MODULE
    "${options}"
    "${one_value_args}"
    ""
    ${ARGN})

  if(NOT MODULE_NAME)
    message(FATAL_ERROR "search_python_module requires NAME")
  endif()
  if(NOT MODULE_PACKAGE)
    set(MODULE_PACKAGE "${MODULE_NAME}")
  endif()
  if(MODULE_NO_VERSION AND MODULE_MIN_VERSION)
    message(FATAL_ERROR
      "search_python_module cannot combine NO_VERSION and MIN_VERSION")
  endif()
  if(NOT Python3_EXECUTABLE)
    message(FATAL_ERROR "Python3_EXECUTABLE is required")
  endif()

  message(STATUS "Searching python module: \"${MODULE_NAME}\"")
  _probe_python_module(
    "${MODULE_NAME}" "${MODULE_NO_VERSION}" probe_result module_version)

  set(module_available FALSE)
  set(module_too_old FALSE)
  if("${probe_result}" STREQUAL "0")
    message(STATUS
      "Found python module: \"${MODULE_NAME}\" (found version \"${module_version}\")")
    if(MODULE_MIN_VERSION AND
       "${module_version}" VERSION_LESS "${MODULE_MIN_VERSION}")
      set(module_too_old TRUE)
    else()
      set(module_available TRUE)
    endif()
  endif()

  if(module_available)
    return()
  endif()

  if(NOT FETCH_PYTHON_DEPS)
    if(module_too_old)
      message(FATAL_ERROR
        "Python module \"${MODULE_NAME}\" found version \"${module_version}\", required version \"${MODULE_MIN_VERSION}\" or newer")
    endif()
    message(FATAL_ERROR
      "Can't find python module: \"${MODULE_NAME}\"; install package \"${MODULE_PACKAGE}\" using your system package manager")
  endif()

  set(package_requirement "${MODULE_PACKAGE}")
  if(MODULE_MIN_VERSION)
    string(APPEND package_requirement ">=${MODULE_MIN_VERSION}")
  endif()

  message(WARNING
    "Python module \"${MODULE_NAME}\" is missing or too old; upgrading \"${package_requirement}\" with pip")
  execute_process(
    COMMAND "${Python3_EXECUTABLE}" -m pip install --user --upgrade
      "${package_requirement}"
    RESULT_VARIABLE pip_result
    OUTPUT_VARIABLE pip_output
    ERROR_VARIABLE pip_error
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE)
  if(NOT "${pip_result}" STREQUAL "0")
    message(FATAL_ERROR
      "Failed to install Python module \"${MODULE_NAME}\" with pip (exit ${pip_result})\n${pip_output}\n${pip_error}")
  endif()

  _probe_python_module(
    "${MODULE_NAME}" "${MODULE_NO_VERSION}" probe_result module_version)
  if(NOT "${probe_result}" STREQUAL "0")
    message(FATAL_ERROR
      "Python module \"${MODULE_NAME}\" is still unavailable after pip install")
  endif()
  if(MODULE_MIN_VERSION AND
     "${module_version}" VERSION_LESS "${MODULE_MIN_VERSION}")
    message(FATAL_ERROR
      "Python module \"${MODULE_NAME}\" still found version \"${module_version}\" after pip install; required version \"${MODULE_MIN_VERSION}\" or newer")
  endif()

  message(STATUS
    "Found python module: \"${MODULE_NAME}\" (found version \"${module_version}\")")
endfunction()
