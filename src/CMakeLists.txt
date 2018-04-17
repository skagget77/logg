cmake_minimum_required(VERSION 3.7)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_library(logg STATIC win32_header.cpp)
else()
  add_library(logg STATIC posix_header.cpp)
endif()
