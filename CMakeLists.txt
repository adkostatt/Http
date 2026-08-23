cmake_minimum_required(VERSION 3.20)

project("Http")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(HTTP_ENABLE_TESTS "" ON)
message(STATUS "HTTP_ENABLE_TESTS = ${HTTP_ENABLE_TESTS}")
add_subdirectory(src)
if(HTTP_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
