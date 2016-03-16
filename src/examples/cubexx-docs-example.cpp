
#include <cubexx/cubexx.hpp>
#include <cubexx/formatters.hpp>
#include <iostream>
#include <cassert>

int main(){
  using cubexx::corner_t;
  using cubexx::direction_t;
  using cubexx::edge_t;
  using cubexx::corner_set_t;
      
  //get the far corner of the cube
  corner_t corner0 = corner_t::get(+1,+1,+1);

  //one can think of the cube as having a length of 2, and lying in a 3D cartesian graph.
  //One can think of the cube's center lying at the origin of the space,
  // with the x-axis going off to the "right", the y-axis going "up", and the z-axis going into the
  // "depth" of space. The particulars of how you imagine this is up to you, as long as you remain consistent.
  
  

  corner_t corner1 = corner_t::get(-1,-1,-1);
  
  
  //corner1 and corner0 are opposite corners of the cube
  assert(corner1.opposite() == corner0);
  
  //"push" the corner to the right
  corner1 = corner1.push(direction_t::get(+1, 0, 0));
  
  //corner1 will now be at (+1,-1,-1)
  
  
  //"push" the corner to the right, again
  corner1 = corner1.push(direction_t::get(+1, 0, 0));
  
  //nothing will change, there is no where to go in that direction
  //corner1 will now be at (+1,-1,-1)
  
  //get the "adjacent" corner in that direction; this will wrap the corner around the to its original
  // value of (-1,-1,-1)
  corner1 = corner1.adjacent(direction_t::get(+1, 0, 0));
  
  //get a new corner at (+1,-1,-1)
  corner_t corner2 = corner1.adjacent(direction_t::get(+1, 0, 0));
  
  //the corners are adjacent
  assert(corner1.is_adjacent(corner2));
  
  //now we can get an edge from the two adjacent corners
  edge_t edge0 = edge_t::get(corner1,corner2);
  
  //a special bit-wise set for reasoning about sets of corners
  corner_set_t corner_set;
  for (auto face : edge0.faces())
  {
    //we can now iterate over the 2 faces that are adjacent to this edge.
    
    //get all the corners that touch these faces
    corner_set |= face.corners();
  }
  
  //remove the original corners
  corner_set -= corner1;
  corner_set -= corner2;
  
  //print all the corners on the faces of edge0, aside from the corners of the edge itself
  for (auto corner : corner_set)
  {
    std::cout << corner << std::endl;
  }
  
  return 0;
}