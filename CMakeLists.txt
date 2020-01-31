cmake_minimum_required(VERSION 3.9)
project(ray-tracer)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")
set (CMAKE_CXX_STANDARD 20)



include_directories(src)
add_executable(ray-tracer src/main.cpp src/util.cpp lodepng/lodepng.cpp)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(ray-tracer PRIVATE OpenMP::OpenMP_CXX)
endif()


add_subdirectory(./googletest)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)
add_executable(runUnitTests ${TEST_SRC_FILES})


target_link_libraries(runUnitTests gtest gtest_main)

add_test(UnitTests runUnitTests)
