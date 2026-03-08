# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src")
  file(MAKE_DIRECTORY "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-src")
endif()
file(MAKE_DIRECTORY
  "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-build"
  "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-subbuild/modbuslib-populate-prefix"
  "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-subbuild/modbuslib-populate-prefix/tmp"
  "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-subbuild/modbuslib-populate-prefix/src/modbuslib-populate-stamp"
  "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-subbuild/modbuslib-populate-prefix/src"
  "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-subbuild/modbuslib-populate-prefix/src/modbuslib-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-subbuild/modbuslib-populate-prefix/src/modbuslib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/affairs/lebai/lebai-sdk/build-format/_deps/modbuslib-subbuild/modbuslib-populate-prefix/src/modbuslib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
