# ray-tracer
A simple header only ray tracer implementation (WIP)
![image created](/img.png?raw=true "Optional Title")
## Dependencies and 3rd party libaries 
- c++11 or higher compiler
- google test
- cmake
- [conan](https://conan.io/)
- [cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow)
## Build
- make sure conan and cmake is installed
- clone directory and submodules
  ```
  git clone --recurse-submodules https://github.com/jdao55/ray-tracer.git
  ```
- build project
  ```
  mkdir build && cd build
  cmake -DCMAKE_BUILD_TYPE=Release ../
  make
  ```
## Running
### Unit Test
google test can be run by the executable  
``` ./runUnitTests```
### Ray tracer
The current example program generates a static image. 
```./ray-tracer``` 
