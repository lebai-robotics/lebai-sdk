set(ROOT "${PROJECT_SOURCE_DIR}")

function(expect_file path)
  if(NOT EXISTS "${ROOT}/${path}")
    message(FATAL_ERROR "Expected workflow file: ${path}")
  endif()
endfunction()

function(expect_missing path)
  if(EXISTS "${ROOT}/${path}")
    message(FATAL_ERROR "Deprecated workflow should not exist: ${path}")
  endif()
endfunction()

function(read_project_file var path)
  expect_file("${path}")
  file(READ "${ROOT}/${path}" content)
  set("${var}" "${content}" PARENT_SCOPE)
endfunction()

function(expect_contains content needle description)
  string(FIND "${content}" "${needle}" found)
  if(found EQUAL -1)
    message(FATAL_ERROR "Expected ${description}: '${needle}'")
  endif()
endfunction()

function(expect_not_contains content needle description)
  string(FIND "${content}" "${needle}" found)
  if(NOT found EQUAL -1)
    message(FATAL_ERROR "Did not expect ${description}: '${needle}'")
  endif()
endfunction()

foreach(path IN ITEMS
  ".github/workflows/cpp_build.yml"
  ".github/workflows/cpp_release.yml"
  ".github/workflows/python_build.yml"
  ".github/workflows/python_release.yml"
  ".github/workflows/dotnet_build.yml"
  ".github/workflows/dotnet_release.yml"
  ".github/workflows/java_build.yml"
  ".github/workflows/java_release.yml"
  ".github/workflows/release.yml")
  expect_file("${path}")
endforeach()

foreach(path IN ITEMS
  ".github/workflows/linux_cpp_build.yml"
  ".github/workflows/windows_cpp_build.yml"
  ".github/workflows/linux_cpp_release.yml"
  ".github/workflows/windows_cpp_release.yml"
  ".github/workflows/linux_cpp_arm64_release.yml"
  ".github/workflows/linux_python_build.yml"
  ".github/workflows/windows_python_build.yml"
  ".github/workflows/linux_python_release.yml"
  ".github/workflows/windows_python_release.yml"
  ".github/workflows/linux_python_aarch64_build.yml"
  ".github/workflows/linux_python_aarch64_release.yml"
  ".github/workflows/linux_dotnet_build.yml"
  ".github/workflows/windows_dotnet_build.yml"
  ".github/workflows/linux_dotnet_release.yml"
  ".github/workflows/windows_dotnet_release.yml"
  ".github/workflows/linux_java_build.yml")
  expect_missing("${path}")
endforeach()

read_project_file(cpp_build ".github/workflows/cpp_build.yml")
read_project_file(cpp_release ".github/workflows/cpp_release.yml")
read_project_file(python_build ".github/workflows/python_build.yml")
read_project_file(python_release ".github/workflows/python_release.yml")
read_project_file(dotnet_build ".github/workflows/dotnet_build.yml")
read_project_file(dotnet_release ".github/workflows/dotnet_release.yml")
read_project_file(java_build ".github/workflows/java_build.yml")
read_project_file(java_release ".github/workflows/java_release.yml")
read_project_file(release_workflow ".github/workflows/release.yml")

foreach(content_name IN ITEMS
  cpp_build
  python_build
  dotnet_build
  java_build)
  expect_contains("${${content_name}}" "pull_request:" "${content_name} PR trigger")
  expect_contains("${${content_name}}" "ubuntu-24.04-arm" "${content_name} Linux arm64 build")
  expect_contains("${${content_name}}" "windows-2022" "${content_name} Windows x64 build")
  expect_not_contains("${${content_name}}" "Win32" "${content_name} legacy Windows 32-bit build")
endforeach()

foreach(content_name IN ITEMS
  cpp_release
  python_release
  dotnet_release
  java_release)
  expect_contains("${${content_name}}" "workflow_call:" "${content_name} release orchestration entry")
  expect_contains("${${content_name}}" "ubuntu-24.04-arm" "${content_name} Linux arm64 release")
  expect_contains("${${content_name}}" "windows-2022" "${content_name} Windows x64 release")
  expect_not_contains("${${content_name}}" "Win32" "${content_name} legacy Windows 32-bit release")
endforeach()

expect_contains("${release_workflow}" "uses: ./.github/workflows/cpp_release.yml" "top-level C++ release workflow")
expect_contains("${release_workflow}" "uses: ./.github/workflows/python_release.yml" "top-level Python release workflow")
expect_contains("${release_workflow}" "uses: ./.github/workflows/dotnet_release.yml" "top-level .NET release workflow")
expect_contains("${release_workflow}" "uses: ./.github/workflows/java_release.yml" "top-level Java release workflow")
expect_not_contains("${release_workflow}" "Win32" "legacy Windows 32-bit release artifact")
