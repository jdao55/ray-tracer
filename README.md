# ray-tracer
A simple ray tracer

## Build
- clone directory
  ```
  git clone https://github.com/jdao55/ray-tracer.git
  ```
- update submodules
  ```
  cd ray-tracer
  git submodule update --init --recursive
  git submodule update --recursive --remote
  ```
- build project
  ```
  mkdir build && cd build
  cmake -DCMAKE_BUILD_TYPE=release ../
  make -j8
  ```
