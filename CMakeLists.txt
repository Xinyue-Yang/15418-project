cmake_minimum_required(VERSION 3.20)
project(parallel_maxflow LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Wall -Wextra -Wpedantic)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

add_executable(main main.cc
    src/network.cc src/utils.cc
    src/sequential/edmonds_karp.cc
    src/parallel1/edmonds_karp.cc
    src/parallel2/edmonds_karp.cc
    src/parallel3/edmonds_karp.cc
    src/parallel4/edmonds_karp.cc
    src/parallel5/edmonds_karp.cc
    src/parallel6/edmonds_karp.cc
    src/sequential/dinics.cc)
target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)

add_executable(generator generator.cc)
target_include_directories(generator PRIVATE)
target_link_libraries(generator PRIVATE)
