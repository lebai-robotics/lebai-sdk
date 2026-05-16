set(ROOT "${PROJECT_SOURCE_DIR}")

function(read_project_file var path)
  if(NOT EXISTS "${ROOT}/${path}")
    message(FATAL_ERROR "Expected file to exist: ${path}")
  endif()
  file(READ "${ROOT}/${path}" content)
  set("${var}" "${content}" PARENT_SCOPE)
endfunction()

function(expect_contains content needle description)
  string(FIND "${content}" "${needle}" found)
  if(found EQUAL -1)
    message(FATAL_ERROR "Expected ${description}: '${needle}'")
  endif()
endfunction()

read_project_file(root_cmake "CMakeLists.txt")
read_project_file(sdk_cmake "sdk/CMakeLists.txt")
read_project_file(test_cmake "sdk/test/CMakeLists.txt")
read_project_file(cpp_build ".github/workflows/cpp_build.yml")
read_project_file(cpp_release ".github/workflows/cpp_release.yml")

expect_contains("${root_cmake}" "CMAKE_POLICY_VERSION_MINIMUM" "CMake 4 policy compatibility for fetched dependencies")
expect_contains("${root_cmake}" "HTTPLIB_USE_OPENSSL_IF_AVAILABLE" "deterministic cpp-httplib OpenSSL option")
expect_contains("${root_cmake}" "HTTPLIB_USE_ZLIB_IF_AVAILABLE" "deterministic cpp-httplib zlib option")
expect_contains("${root_cmake}" "HTTPLIB_USE_BROTLI_IF_AVAILABLE" "deterministic cpp-httplib brotli option")
expect_contains("${root_cmake}" "HTTPLIB_USE_ZSTD_IF_AVAILABLE" "deterministic cpp-httplib zstd option")
expect_contains("${root_cmake}" "HTTPLIB_INSTALL" "disable fetched cpp-httplib install rules")

expect_contains("${root_cmake}" "LEBAI_NLOHMANN_JSON_TARGET" "nlohmann target variable")
expect_contains("${root_cmake}" "LEBAI_HTTPLIB_TARGET" "httplib target variable")
expect_contains("${root_cmake}" "LEBAI_JSON_RPC_CXX_TARGET" "json-rpc-cxx target variable")

expect_contains("${sdk_cmake}" [=[$<BUILD_INTERFACE:${LEBAI_NLOHMANN_JSON_TARGET}>]=] "nlohmann build-only link dependency")
expect_contains("${sdk_cmake}" [=[$<BUILD_INTERFACE:${LEBAI_HTTPLIB_TARGET}>]=] "httplib build-only link dependency")
expect_contains("${sdk_cmake}" [=[$<BUILD_INTERFACE:${LEBAI_JSON_RPC_CXX_TARGET}>]=] "json-rpc-cxx build-only link dependency")

expect_contains("${test_cmake}" "LEBAI_NLOHMANN_JSON_TARGET" "test_json_protos direct nlohmann include dependency")

expect_contains("${cpp_build}" "-DBUILD_TESTING=OFF" "C++ package jobs skip test targets")
expect_contains("${cpp_release}" "-DBUILD_TESTING=OFF" "C++ release package jobs skip test targets")
