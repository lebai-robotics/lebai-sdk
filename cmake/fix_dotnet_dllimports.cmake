if(NOT DEFINED DOTNET_PROJECT_DIR)
  message(FATAL_ERROR "DOTNET_PROJECT_DIR is required")
endif()
if(NOT DEFINED DOTNET_NATIVE_LIBRARY)
  message(FATAL_ERROR "DOTNET_NATIVE_LIBRARY is required")
endif()

file(GLOB pinvoke_files "${DOTNET_PROJECT_DIR}/*PINVOKE.cs")
foreach(path IN LISTS pinvoke_files)
  file(READ "${path}" content)
  set(updated "${content}")
  string(REGEX REPLACE
    "DllImport\\(\"(l_master|zeroconf|gripper|gripper_module)\""
    "DllImport(\"${DOTNET_NATIVE_LIBRARY}\""
    updated
    "${updated}")
  if(NOT updated STREQUAL content)
    file(WRITE "${path}" "${updated}")
  endif()
endforeach()
