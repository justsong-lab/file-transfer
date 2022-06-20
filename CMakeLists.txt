cmake_minimum_required(VERSION 3.0)

project(server CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

add_subdirectory(./lib/muduo)
include_directories("./lib/muduo")

add_executable(server src/main.cpp src/server.cpp)
target_link_libraries(server muduo_net)
target_link_libraries(server muduo_base)
target_link_libraries(server pthread rt)