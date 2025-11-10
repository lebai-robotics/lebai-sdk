cmake_minimum_required(VERSION 3.14)

project(swig-download NONE)

include(ExternalProject)
  ExternalProject_Add(SWIG_project
      URL "https://sourceforge.net/projects/swig/files/swigwin/swigwin-4.2.1/swigwin-4.2.1.zip/download"
      URL_HASH SHA256=c08b4a9c8d7e3ff0b9ea8ee801903822de67e0f0c1e9d039bd6ae1b8b8fe2df8
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
      BUILD_IN_SOURCE 1
      SOURCE_DIR "${CMAKE_BINARY_DIR}/swig"
      UPDATE_COMMAND ""
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      TEST_COMMAND ""
      LOG_DOWNLOAD ON
      LOG_CONFIGURE ON
      LOG_BUILD ON
      )
