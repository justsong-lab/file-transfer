cmake_minimum_required(VERSION 3.0)

project(file_transfer CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

add_subdirectory(./lib/muduo)
include_directories("./lib/muduo")

add_executable(file_transfer src/main.cpp)
target_link_libraries(file_transfer muduo_net)
target_link_libraries(file_transfer muduo_base)
target_link_libraries(file_transfer pthread rt)