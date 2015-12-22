cubelib
---

cubelib is a C header-only library for reasoning about a cube's corners, directions/faces, and edges. It is useful for
dealing with data structures based on cubes, such as octrees.

![travis continuous integration build status sticker](https://travis-ci.org/realazthat/cubelib.svg?branch=master)

Motivation
----

Using cubelib allows one to avoid cryptic bit-twiddling, and instead use logical and
self-documenting functions provided by this library to reason about and manipulate corners of the cube.

Example functionality:

* `corner0 = get_corner_by_int3(-1,-1,1)` - get the lower x,y, and upper z corner
* `corner0 = get_direction_by_int3(0,1,0)` - get the direction of +y
* `corner1 = move_corner(corner0, direction)` - move the corner in the +y direction, return the resulting corner
* `corner2 = opposite_corner(corner1)` - calculate the opposite corner





License
----

Released under the [MIT License](https://opensource.org/licenses/MIT).



