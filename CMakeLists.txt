cmake_minimum_required(VERSION 3.7)
project(logg)

# Pass the log level set on the CMake command line to the compiler.
if(DEFINED LOGG_LOG_LEVEL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOGG_LOG_LEVEL=${LOGG_LOG_LEVEL}")
endif()

include_directories(include)

add_subdirectory(example)
add_subdirectory(src)
