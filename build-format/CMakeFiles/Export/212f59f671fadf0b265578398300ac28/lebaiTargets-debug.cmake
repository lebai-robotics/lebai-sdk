#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "lebai::lebai-cpp" for configuration "Debug"
set_property(TARGET lebai::lebai-cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(lebai::lebai-cpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/lebai-cpp.lib"
  )

list(APPEND _cmake_import_check_targets lebai::lebai-cpp )
list(APPEND _cmake_import_check_files_for_lebai::lebai-cpp "${_IMPORT_PREFIX}/lib/lebai-cpp.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
