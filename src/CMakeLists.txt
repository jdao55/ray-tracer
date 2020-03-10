
add_executable(ray-tracer main.cpp ../lodepng/lodepng.cpp)
target_link_libraries(ray-tracer PRIVATE project_options project_warnings CONAN_PKG::docopt.cpp
                CONAN_PKG::fmt CONAN_PKG::spdlog CONAN_PKG::cpp-taskflow)
