cmake_minimum_required(VERSION 3.25)
project(stratum VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan QUIET)

add_subdirectory(src/core)
add_subdirectory(src/renderer)
add_subdirectory(src/integration)
add_subdirectory(src/calibration)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
