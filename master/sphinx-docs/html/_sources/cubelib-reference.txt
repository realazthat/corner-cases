
corner-cases/cubelib Reference Documentation
===================================================

.. role:: ccode(code)
   :language: c

..  role:: strikethrough


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



.. _cubelib-edge-overview:

cubelib_edge_t Overview
------------------------------------------------

The :c:type:`cubelib_edge_t <cubelib_edge_t>` represents the edge of a cube.

* There are 12 edges in a cube.

You can use it to:

* Obtain the lower and upper :ref:`corners <cubelib-corner-overview>` of the edge,
* Obtain all (4) adjacent edges,
* Obtain all (2) adjacent :ref:`faces <cubelib-face-overview>`,
* Obtain the opposite edge,
* Retrieve an index suitable for array storage,
    * And the inverse, retrieve a edge via its index





.. _cubelib-face-overview:

cubelib_face_t Overview
------------------------------------------------

The :c:type:`cubelib_face_t <cubelib_face_t>` represents the face of a cube.

* There are 6 faces in a cube.

You can use it to:

* Obtain the opposite face,
* Obtain all (4) adjacent faces,
* Obtain all (4) :ref:`corners <cubelib-corner-overview>` lying on the face,
* Obtain all (4) :ref:`edges <cubelib-edge-overview>`. lying on the face,
* Retrieve an index suitable for array storage,
    * And the inverse, retrieve a face via its index
* Reason about sets of faces (using :c:type:`cubelib_face_set_t <cubelib_face_set_t>`),
* Convert it to a :ref:`direction <cubelib-direction-overview>`.










Usage
------------------------


.. literalinclude:: /../../src/examples/cubelib-docs-example.cpp
   :language: c++
   :linenos:




Reference pages:
------------------------

.. toctree::
   :maxdepth: 2
   
   cubelib-corner
   cubelib-direction
   cubelib-edge
   cubelib-face





