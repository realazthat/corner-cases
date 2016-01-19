
corner-cases
----

corner-cases/cubexx and corner-cases/cubelib are two libraries for reasoning about a cube's corners, directions/faces, and edges. It is useful for
dealing with data structures based on cubes, such as octrees.

cubexx is a C++ header-only template library with bells and whistles.

cubelib is a C header-only library; it is also made to be compilable and usable in opencl.



[![Build Status](https://travis-ci.org/realazthat/corner-cases.svg?branch=master)](https://travis-ci.org/realazthat/corner-cases)


Motivation
----

Using corner-cases allows one to avoid cryptic bit-twiddling, and instead use logical and
self-documenting functions provided by this library to reason about and manipulate corners of the cube.

Example functionality (cubecxx) pseudocode:

* `auto corner0 = cubecxx::corners_t::get(-1,-1,+1);`
* `auto direction = cubecxx::direction_t::get( 0,-1, 0);`
* `auto corner1 = corner0.adjacent(direction);`
* `auto corner2 = corner1.opposite();`

Example functionality (cubelib) pseudocode:

* `corner0 = get_corner_by_int3(-1,-1,+1)` - get the lower x,y, and upper z corner
* `direction = get_direction_by_int3( 0,+1, 0)` - get the direction of +y
* `corner1 = move_corner(corner0, direction)` - move the corner in the +y direction, return the resulting corner
* `corner2 = opposite_corner(corner1)` - calculate the opposite corner



License
----

Released under the [MIT License](https://opensource.org/licenses/MIT).




Building
----

Use of the library in C does not require anything to be built.

The library can however be built for use in opencl, in which case certain constant-arrays need to be
pre-calculated. Therefore, a binary can be built, which will generate the headers with the constants.

Furthermore, there are unittests that can be built.

The unittests require googletest (gtest) to be installed on the system.

To build (on linux):

```
mkdir build
cd build
cmake ..
#on msys2/mingw-w64 you would run this instead
cmake -G"MSYS Makefiles" ..

#on some systems, you must additionally link pthreads after gtest, so let's store pthread in GTEST_LIB
cmake . "-DGTEST_LIB=gtest;pthread"

#now build
#optionally make the program that generates the opencl consts header, unnecessary
make cubelib-clgen-consts
#optionally make the generated opencl consts header
make cubelib.clgen.h
#make the unittests
make unittests
#or make all
make all
```

The program `cubelib-clgen-consts` will be built, which can generate the required header `cubelib.clgen.h`, which will itself be generated as well
in the build directory.

To run the unittests:

```
#in the build directory
./unittests
```
