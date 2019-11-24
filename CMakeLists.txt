cmake_minimum_required(VERSION 3.9)
project(ray-tracer)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")
set (CMAKE_CXX_STANDARD 20)



include_directories(src)
add_executable(ray-tracer src/main.cpp)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(ray-tracer PRIVATE OpenMP::OpenMP_CXX)
endif()
