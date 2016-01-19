
corner-cases
----

**corner-cases/cubexx** and **corner-cases/cubelib** are two libraries for reasoning about a cube's corners, directions/faces, and edges. It is useful for
dealing with data structures based on cubes, such as octrees.

**corner-cases/cubexx** is a C++11 header-only template library with bells and whistles.

**corner-cases/cubelib** is a C header-only library; it is also made to be compilable and usable in opencl.



Branch  | Status
---     | ---
master  | [![Build Status](https://travis-ci.org/realazthat/corner-cases.svg?branch=master)](https://travis-ci.org/realazthat/corner-cases)
develop | [![Build Status](https://travis-ci.org/realazthat/corner-cases.svg?branch=develop)](https://travis-ci.org/realazthat/corner-cases)


Motivation
----

Using corner-cases allows one to avoid cryptic bit-twiddling, and instead use logical and
self-documenting functions provided by this library to reason about and manipulate corners of the cube.

**CDEF**
-----

* Corners
* Directions
* Edges
* Faces


Example functionality (cubexx) pseudocode:

* `auto corner0 = cubexx::corners_t::get(-1,-1,+1);`
* `auto direction = cubexx::direction_t::get( 0,-1, 0);`
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


* Requires CMake >= 3.1.3


Targets:

* cubelib (not build-able)
    * Header-only C library, nothing to build
    * Requires C99
* cubelib-clgen-consts
    * Program that generates a header containing constants for cubelib, for use with opencl
* cubelib.clgen.h
    * Target that generates the constants header, for use with opencl
* cubelib-formatters
    * Optionally link the iostream pretty printing formatters; alternatively can define the macro
        `CORNER_CASES_CUBELIB_FORMATTERS_HEADER_ONLY` to include the functionality inline (header-only)
    * Requires C++98
* cubelib-unittests
    * Unit tests for cubelib
    * Requires C++11
* cubexx (not build-able)
    * Header-only C++11 library, nothing to build
    * Requires C++11
* cubexx-formatters
    * Optionally link the iostream pretty printing formatters; alternatively can define the macro
        `CORNER_CASES_CUBEXX_FORMATTERS_HEADER_ONLY` to include the functionality inline (header-only)
    * Requires C++11
* cubexx-unittests
    * Unit tests for cubexx
    * Requires C++11, gtest

**corner-cases/cubexx**
-----

You can use **corner-cases/cubexx** by simply including the file `cubexx/cubexx.hpp` (there is also a
forward declaration header you can use at `cubexx/cubexx.fwd.hpp`.

If you want to use the `iostream` formatters (for pretty printing), you should include the file
`cubexx/formatters.hpp` and define the macro `CORNER_CASES_CUBEXX_FORMATTERS_HEADER_ONLY` in your project;
alternatively you can link to the `cubexx-formatters` target, which you can build, instructions to follow.

There are also **corner-cases/cubexx** unit tests that can be built.

The cubexx-unittests require:

* googletest (gtest) to be installed on the system.
* C++11

To build (on a linux-like environment, or msys2):

```

#install dependencies, there are some bash scripts provided in the ./scripts/ directory
# that will download and build many of the dependencies and put them in the ./libs directory
# the scripts are meant for the continuous integration system, but you can run them yourself
# or read them for assistance.

#optionally install or download and build googletest (only matters if you are going to run the unit tests)
#note, choose the appropriate generator
#see the bash scripts for for more details
CMAKE_GENERATOR="MSYS Makefiles" CMAKE_BUILD_TYPE="Debug" bash ./scripts/download-and-build-googletest.sh
#... etc. install dependencies

#make a build directory
mkdir build && cd build
#note, choose the appropriate generator
cmake -G"MSYS Makefiles" .. -DCMAKE_BUILD_TYPE="Debug"



cmake -L # list all the user-definable variables
#on some systems, you must additionally link pthreads after gtest, so let's store pthread in GTEST_LIB
cmake . "-DGTEST_LIB=gtest;pthread"

#now build
#optionally make the cubexx formatter lib for iostream pretty printing
cmake --build . --target cubexx-formatters
#optionally make the unit tests
cmake --build . --target cubelib-unittests
#or make all
cmake --build .
```

To run the unit tests:

```
#in the build directory
./cubexx-unittests
```


**corner-cases/cubelib**
-----

Use of the library in C does not require anything to be built.

You can use **corner-cases/cubelib** in C99 by simply including the file `cubelib/cubelib.h` (there is also a
forward declaration header you can use at `cubelib/cubelib.fwd.h`.


If you want to use the `iostream` formatters (for pretty printing), you should include the file
`cubelib/formatters.hpp` and define the macro `CORNER_CASES_CUBELIB_FORMATTERS_HEADER_ONLY` in your project;
alternatively you can link to the `cubelib-formatters` target, which you can build, instructions to follow.



The library can however be built for use in opencl, in which case certain constant-arrays need to be
pre-calculated. To do this, **corner-cases/cubelib** includes a target called `cubelib-clgen-consts`, which can be built
and which will generate the headers with the constants. There is also a direct target called `cubelib.clgen.h`
which will build everything necessary and dump the header into the build directory.

Furthermore, there are also **corner-cases/cubelib** unit tests that can be built.

The cubelib-unittests require:

* googletest (gtest) to be installed on the system.
* C++11



To build (on a linux-like environment, or msys2):

```

#install dependencies, there are some bash scripts provided in the ./scripts/ directory
# that will download and build many of the dependencies and put them in the ./libs directory
# the scripts are meant for the continuous integration system, but you can run them yourself
# or read them for assistance.

#optionally install or download and build googletest (only matters if you are going to run the unittests)
#note, choose the appropriate generator
#see the bash scripts for for more details
CMAKE_GENERATOR="MSYS Makefiles" CMAKE_BUILD_TYPE="Debug" bash ./scripts/download-and-build-googletest.sh
#... etc. install dependencies

#make a build directory
mkdir build && cd build
#note, choose the appropriate generator
cmake -G"MSYS Makefiles" .. -DCMAKE_BUILD_TYPE="Debug"



cmake -L # list all the user-definable variables
#on some systems, you must additionally link pthreads after gtest, so let's store pthread in GTEST_LIB
cmake . "-DGTEST_LIB=gtest;pthread"

#now build
#optionally make the program that generates the opencl consts header, unnecessary
cmake --build . --target cubelib-clgen-consts
#optionally make the generated opencl consts header
cmake --build . --target cubelib.clgen.h
#optionally make the cubelib formatter lib for iostream pretty printing
cmake --build . --target cubelib-formatters
#optionally make the unittests
cmake --build . --target cubelib-unittests
#or make all
cmake --build .
```

The program `cubelib-clgen-consts` will be built, which can generate the required header `cubelib.clgen.h`, which will itself be generated as well
in the build directory.

To run the unittests:

```
#in the build directory
./cubelib-unittests
```