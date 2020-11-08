cmake_minimum_required(VERSION 3.12)
project(prme)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(inc)
file(GLOB SRC "src/*.cc")
add_executable(prme ${SRC})
