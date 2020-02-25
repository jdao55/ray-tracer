cmake_minimum_required(VERSION 3.9)
project(ray-tracer)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -Ofast")
set (CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

include_directories(src)
include_directories("cpp-taskflow/taskflow")

file(GLOB RT_SOURCE
  src/util.cpp src/geometry/geometry_util.cpp lodepng/lodepng.cpp
  )
add_executable(ray-tracer  src/main.cpp ${RT_SOURCE})
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(ray-tracer PRIVATE OpenMP::OpenMP_CXX)
endif()


add_subdirectory(./googletest)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)
add_executable(runUnitTests ${TEST_SRC_FILES} ${RT_SOURCE})


target_link_libraries(runUnitTests gtest gtest_main)

add_test(UnitTests runUnitTests)
