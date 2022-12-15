if(NOT BUILD_CXX)
  return()
endif()

enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
configure_file("${PROJECT_SOURCE_DIR}/config.hh.in" "${PROJECT_SOURCE_DIR}/sdk/include/lebai/config.hh")

include(GNUInstallDirs)
add_subdirectory(sdk)
# Install
install(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAMESPACE}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  COMPONENT Devel)
include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/${PROJECT_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion)
install(
  FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  COMPONENT Devel)

# add_cpp_example()
# CMake function to generate and build C++ example.
# Parameters:
#  the C++ filename
# e.g.:
# add_cpp_example(foo.cpp)
function(add_cpp_example FILE_NAME)
  message(STATUS "Configuring example ${FILE_NAME}: ...")
  get_filename_component(EXAMPLE_NAME ${FILE_NAME} NAME_WE)
  get_filename_component(COMPONENT_DIR ${FILE_NAME} DIRECTORY)
  get_filename_component(COMPONENT_NAME ${COMPONENT_DIR} NAME)

  if(APPLE)
    set(CMAKE_INSTALL_RPATH
      "@loader_path/../${CMAKE_INSTALL_LIBDIR};@loader_path")
  elseif(UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}:$ORIGIN")
  endif()

  add_executable(${EXAMPLE_NAME} ${FILE_NAME})
  target_include_directories(${EXAMPLE_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/sdk/src)
  target_compile_features(${EXAMPLE_NAME} PRIVATE cxx_std_14)
  target_link_libraries(${EXAMPLE_NAME} PRIVATE
    ${PROJECT_NAMESPACE}::lebai-cpp)
  if(UNIX)
     target_link_libraries(${EXAMPLE_NAME} PRIVATE
        pthread)
  endif()

  # include(GNUInstallDirs)
  # install(TARGETS ${EXAMPLE_NAME})

  # if(BUILD_TESTING)
  #   add_test(NAME cpp_${COMPONENT_NAME}_${EXAMPLE_NAME} COMMAND ${EXAMPLE_NAME})
  # endif()
  message(STATUS "Configuring example ${FILE_NAME}: ...DONE")
endfunction()

option(BUILD_DEB "Create deb package" OFF)
if(UNIX)
  if(BUILD_DEB)
    set(CPACK_PACKAGE_NAME "lebai")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_SOURCE_GENERATOR "DEB")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_CONTACT "liufang_robot@outlook.com")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "liufang <${CPACK_PACKAGE_CONTACT}>")
    include(CPack)
  endif()
endif()
