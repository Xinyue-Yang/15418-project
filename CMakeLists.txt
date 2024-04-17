cmake_minimum_required(VERSION 3.20)
project(parallel_maxflow LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(main main.cc
    src/network.cc src/utils.cc
    src/sequential/dinics.cc)

target_include_directories(main PRIVATE include)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
