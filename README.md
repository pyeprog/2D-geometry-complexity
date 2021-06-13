# 2D-geometry-complexity
An implementation of paper [ESTIMATING COMPLEXITY OF 2D SHAPES](https://ieeexplore.ieee.org/abstract/document/4014089)

## Compile
1. `pip install conan` to install conan
2. create conan profile
3. `mkdir build && cd build`
4. `conan install ..`, if failed, probably `conan install .. --build=geos,gtest` will work
5. `cmake ..`
6. `make`


## Usage
```c++
gcpx --wkt "the wkt" -j 4
```
