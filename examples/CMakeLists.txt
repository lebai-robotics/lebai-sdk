if(NOT BUILD_EXAMPLES)
  return()
endif()


file(GLOB CPP_SRCS "*.cc")
foreach(FILE_NAME IN LISTS CPP_SRCS)
  add_cpp_example(${FILE_NAME})
endforeach()

if(BUILD_DOTNET)
  file(GLOB DOTNET_SRCS "*.cs")
  foreach(FILE_NAME IN LISTS DOTNET_SRCS)
    add_dotnet_example(${FILE_NAME})
  endforeach()
endif()

if(BUILD_JAVA)
  file(GLOB JAVA_SRCS "*.java")
  foreach(FILE_NAME IN LISTS JAVA_SRCS)
    add_java_example(${FILE_NAME})
  endforeach()
endif()