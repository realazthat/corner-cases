
corner-cases/cubexx Reference Documentation
===================================



**CDEF:**

* **C**\ orners
* **D**\ irections
* **E**\ dges
* **F**\ aces

.. _cubexx-corner-overview:

cubexx::corner_t Overview
------------------------------------------------

The :cpp:class:`cubexx::corner_t <cubexx::corner_t>` represents the corner of a cube.

* There are 8 corners in a cube.

You can use it to:

* Obtain a corner via 3 unit cube coordinates, i.e coordinates in {0,1}^3, such as ``(0,0,1)`` and ``(1,0,1)``,
    * Retrieve :cpp:func:`x <cubexx::corner_t::ux()>`, :cpp:func:`y <cubexx::corner_t::uy()>`, :cpp:func:`z <cubexx::corner_t::uz()>` unit-cube coordinates,
* Obrain a corner via 3 origin-centered cube coordinates, i.e coordinates in {-1,+1}^3, such as ``(-1,-1,+1)` and ``(+1,-1,+1)``,
    * Retrieve :cpp:func:`x <cubexx::corner_t::x()>`, :cpp:func:`y <cubexx::corner_t::y()>`, :cpp:func:`z <cubexx::corner_t::z()>` origin centered coordinates,
* Obtain all (3) adjacent corners,
* Push a corner in a :ref:`direction <cubexx-direction-overview>`, to a neighboring corner (no wrap around),
* Move a corner in a :ref:`direction <cubexx-direction-overview>`, to a neighboring corner (with wrap around),
* Obtain adjacent (3) faces,
* Obtain or (3) adjacent edges,
* Obtain the opposite corner,
* Retrieve an index suitable for array storage,
    * The index is naturally in 3D `morton-order <https://en.wikipedia.org/wiki/Z-order_curve>`_ for the cube-space,
    * And the inverse, retrieve a corner via its index
* Reason about sets of corners (using :cpp:class:`cubexx::corner_set_t <cubexx::corner_set_t>`).

.. _cubexx-direction-overview:

cubexx::direction_t Overview
------------------------------------------------

The :cpp:class:`cubexx::direction_t <cubexx::direction_t>` represents a direction in the cube.

* Directions and :ref:`faces <cubexx-face-overview>` are related ``1:1``.
* There are 6 directions for 6 faces.
* Directions and faces and are convertible to each-other. They are separated because they intuitively separate certain functionality.

Once again, you can do things like:

* Obtain a direction via 3 vector coordinates, representing the direction as a relative vector from origin
    , i.e coordinates in {-1,+1}^3, such as ``(+0,0,+1)`` and ``(0,-1,0)``,
    
    * Retrieve :cpp:func:`x <cubexx::direction_t::x()>`, :cpp:func:`y <cubexx::direction_t::y()>`, :cpp:func:`z <cubexx::direction_t::z()>` vector coordinates,
* Check if a direction is positive, i.e if it's non-zero coordinate is positive,
* Obtain all (4) "adjacent" directions,
* Obtain the opposite direction,
* 
* Retrieve an index suitable for array storage,
    * And the inverse, retrieve a direction via its index
* Reason about sets of directions (using :cpp:class:`cubexx::direction_set_t <cubexx::direction_set_t>`),
* Convert it to a :ref:`face <cubexx-face-overview>`.



.. _cubexx-edge-overview:

cubexx::edge_t Overview
------------------------------------------------


.. _cubexx-face-overview:

cubexx::face_t Overview
------------------------------------------------






Usage
------------------------

.. code-block:: c++

  #include <cubexx/cubexx.hpp>
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
    corner_set |= faces.corners();
  }
  
  //remove the original corners
  corner_set -= corner1;
  corner_set -= corner2;
  
  //print all the corners on the faces of edge0, aside from the corners of the edge itself
  for (auto corner : corner_set)
  {
    std::cout << corner << std::endl;
  }
  



Reference pages:
------------------------

.. toctree::
   :maxdepth: 2
   
   cubexx-corner
   cubexx-direction
   cubexx-edge
   cubexx-face





