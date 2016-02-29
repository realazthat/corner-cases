
corner-cases/cubelib Reference Documentation
===================================================

.. role:: ccode(code)
   :language: cpp

..  role:: strikethrough

make text :strikethrough:`strikethrough` in the sphinx

**CDEF:**

* **C**\ orners
* **D**\ irections
* **E**\ dges
* **F**\ aces

.. _cubelib-corner-overview:

cubelib_corner_t Overview
------------------------------------------------

The :c:type:`cubelib_corner_t <cubelib_corner_t>` represents the corner of a cube.

* There are 8 corners in a cube.

You can use it to:

* Obtain a corner via 3 unit cube coordinates, i.e coordinates in :math:`\left\{0,1\right\}^3`, such as :math:`(0,0,1)` and :math:`(1,0,1)`,
    * Retrieve :c:func:`x <cubelib_get_corner_ux()>`, :c:func:`y <cubelib_get_corner_uy()>`, :c:func:`z <cubelib_get_corner_uz()>` unit-cube coordinates,
* Obrain a corner via 3 origin-centered cube coordinates, i.e coordinates in :math:`\left\{-1,+1\right\}^3`,, such as :math:`(-1,-1,+1)` and :math:`(+1,-1,+1)`,
    * Retrieve :c:func:`x <cubelib_get_corner_x()>`, :c:func:`y <cubelib_get_corner_y()>`, :c:func:`z <cubelib_get_corner_z()>` origin centered coordinates,
* Obtain all (3) adjacent corners,
* Push a corner in a :ref:`direction <cubelib-direction-overview>`, to a neighboring corner (no wrap around),
* Move a corner in a :ref:`direction <cubelib-direction-overview>`, to a neighboring corner (with wrap around),
* Obtain adjacent (3) :ref:`faces <cubelib-face-overview>`,
* :strikethrough:`Obtain or (3) adjacent edges`,
* Obtain the opposite corner,
* Retrieve an index suitable for array storage,
    * The index is naturally in 3D `morton-order <https://en.wikipedia.org/wiki/Z-order_curve>`_ for the cube-space,
    * And the inverse, retrieve a corner via its index

.. _cubelib-direction-overview:

cubelib_direction_t Overview
------------------------------------------------

The :c:type:`cubelib_direction_t <cubelib_direction_t>` represents a direction in the cube.

* Directions and :ref:`faces <cubelib-face-overview>` are related `1:1`.
* There are 6 directions for 6 faces.
* Directions and faces and are convertible to each-other. They are separated because they intuitively separate certain functionality.

Once again, you can do things like:

* Obtain a direction via 3 vector coordinates, representing the direction as a relative vector from origin
    , i.e coordinates in {0,-1,+1}^3, such as :math:`(+0,0,+1)` and :math:`(0,-1,0)`,
    
    * Retrieve :c:func:`x <cubelib_get_direction_x()>`, :c:func:`y <cubelib_get_direction_y()>`, :c:func:`z <cubelib_get_direction_z()>` vector coordinates,
* Check if a direction is positive, i.e if it's non-zero coordinate is positive,
* Obtain the opposite direction,
* 
* Retrieve an index suitable for array storage,
    * And the inverse, retrieve a direction via its index
* Convert it to a :ref:`face <cubelib-face-overview>`.



.. _cubexx-edge-overview:

cubelib_edge_t Overview
------------------------------------------------

The :c:class:`cubelib_edge_t <cubelib_edge_t>` represents the edge of a cube.

* There are 12 edges in a cube.

You can use it to:

* Obtain the lower and upper :ref:`corners <cubexx-corner-overview>` of the edge,
* Obtain all (4) adjacent edges,
* Obtain all (2) adjacent :ref:`faces <cubexx-face-overview>`,
* Obtain the opposite edge,
* Retrieve an index suitable for array storage,
    * And the inverse, retrieve a edge via its index





.. _cubexx-face-overview:

cubelib_face_t Overview
------------------------------------------------

The :cpp:class:`cubexx::face_t <cubexx::face_t>` represents the face of a cube.

* There are 6 faces in a cube.

You can use it to:

* Obtain the opposite face,
* Obtain all (4) adjacent faces,
* Obtain all (4) :ref:`corners <cubexx-corner-overview>` lying on the face,
* Obtain all (4) :ref:`edges <cubexx-edge-overview>`. lying on the face,
* Retrieve an index suitable for array storage,
    * And the inverse, retrieve a face via its index
* Reason about sets of faces (using :cpp:class:`cubexx::face_set_t <cubexx::face_set_t>`),
* Convert it to a :ref:`direction <cubexx-direction-overview>`.










Usage
------------------------

.. code-block:: c

  #include <cubelib/cubelib.h>
  
  //get the far corner of the cube
  corner_t corner0 = cubelib_get_corner(+1,+1,+1);

  //one can think of the cube as having a length of 2, and lying in a 3D cartesian graph.
  //One can think of the cube's center lying at the origin of the space,
  // with the x-axis going off to the "right", the y-axis going "up", and the z-axis going into the
  // "depth" of space. The particulars of how you imagine this is up to you, as long as you remain consistent.
  
  

  corner_t corner1 = corner_get_corner(-1,-1,-1);
  
  
  //corner1 and corner0 are opposite corners of the cube
  assert(cubelib_is_corner_equal(cubelib_get_opposite_corner(corner1), corner0));
  
  //"push" the corner to the right
  corner1 = corner1.push(direction_t::get(+1, 0, 0));
  corner1 = cubelib_corner_push(corner1,cubelib_get_direction(+1, 0, 0));
  
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
   
   cubelib-corner
   cubelib-direction
   cubelib-edge
   cubelib-face
   cubelib-sets





