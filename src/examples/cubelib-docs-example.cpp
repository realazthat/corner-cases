

#include <cubelib/cubelib.h>
#include <cubelib/formatters.hpp>
#include <iostream>
#include <cassert>

int main(){
  //get the far corner of the cube
  cubelib_corner_t corner0 = cubelib_get_corner_by_int3(+1,+1,+1);

  //one can think of the cube as having a length of 2, and lying in a 3D cartesian graph.
  //One can think of the cube's center lying at the origin of the space,
  // with the x-axis going off to the "right", the y-axis going "up", and the z-axis going into the
  // "depth" of space. The particulars of how you imagine this is up to you, as long as you remain consistent.
  
  

  cubelib_corner_t corner1 = cubelib_get_corner_by_int3(-1,-1,-1);
  
  
  //corner1 and corner0 are opposite corners of the cube
  assert(cubelib_is_corner_equal(cubelib_get_opposite_corner(corner1), corner0));
  
  //"push" the corner to the right
  corner1 = cubelib_corner_push(corner1, cubelib_get_direction_by_int3(+1, 0, 0));
  
  //corner1 will now be at (+1,-1,-1)
  
  
  //"push" the corner to the right, again
  corner1 = cubelib_corner_push(corner1, cubelib_get_direction_by_int3(+1, 0, 0));
  
  //nothing will change, there is no where to go in that direction
  //corner1 will now be at (+1,-1,-1)
  
  //get the "adjacent" corner in that direction; this will wrap the corner around the to its original
  // value of (-1,-1,-1)
  corner1 = cubelib_get_adjacent_corner(corner1, cubelib_get_direction_by_int3(+1, 0, 0));
  
  //get a new corner at (+1,-1,-1)
  cubelib_corner_t corner2 = cubelib_get_adjacent_corner(corner1, cubelib_get_direction_by_int3(+1, 0, 0));
  
  //the corners are adjacent
  assert(cubelib_is_corner_adjacent_corner(corner1, corner2));
  
  //now we can get an edge from the two adjacent corners
  cubelib_edge_t edge0 = cubelib_get_edge_by_corners(corner1,corner2);
  
  //a bit-wise set for reasoning about sets of corners
  uint32_t corner_set;
  
  //note that we can use the range-based for loop for cubelib, if using it from C++11
  for (auto face : cubelib_faces_on_edge[cubelib_get_edge_index(edge0)])
  {
    //we can now iterate over the 2 faces that are adjacent to this edge.
    
    //get all the corners that touch these faces
    for (auto corner : cubelib_corners_on_face[cubelib_get_face_index(face)])
    {
        corner_set |= 1 << cubelib_get_corner_index(corner);
    }
  }
  
  //remove the original corners
  corner_set &= ~uint32_t(1 << cubelib_get_corner_index(corner1));
  corner_set &= ~uint32_t(1 << cubelib_get_corner_index(corner2));
  
  //print all the corners on the faces of edge0, aside from the corners of the edge itself
  for (auto corner : cubelib_all_corners)
  {
    if (((corner_set >> cubelib_get_corner_index(corner)) & 1) == 0)
        continue;
    std::cout << corner << std::endl;
  }
  
  return 0;
}

