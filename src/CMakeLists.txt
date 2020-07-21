include(CheckIPOSupported)

add_executable(ray-tracer main.cpp ../lodepng/lodepng.cpp)
target_link_libraries(ray-tracer PRIVATE project_options project_warnings CONAN_PKG::docopt.cpp
  CONAN_PKG::fmt CONAN_PKG::spdlog CONAN_PKG::cpp-taskflow)

check_ipo_supported(RESULT lto_support OUTPUT error LANGUAGES CXX)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(ray-tracer PRIVATE -Ofast)
  #add lto to release build
  if(lto_support)
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET ray-tracer PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(STATUS "IPO / LTO not supported: <${error}>")
  endif()
endif ()
