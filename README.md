# ray-tracer
A simple ray tracer implementation WIP

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
## Running
### Unit Test
google test can be run by the executable
``` ./runUnitTests```
### Ray tracer
Currently the tracer creates a static image and can be run with no arguments
```./ray-tracer```
