
corner-cases/cubexx Reference Documentation
=================================================

.. role:: cppcode(code)
   :language: cpp


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

* Obtain a corner via 3 unit cube coordinates, i.e coordinates in :math:`\left\{0,1\right\}^3`, such as :math:`(0,0,1)` and :math:`(1,0,1)`,
    * Retrieve :cpp:func:`x <cubexx::corner_t::ux()>`, :cpp:func:`y <cubexx::corner_t::uy()>`, :cpp:func:`z <cubexx::corner_t::uz()>` unit-cube coordinates,
* Obrain a corner via 3 origin-centered cube coordinates, i.e coordinates in :math:`\left\{-1,+1\right\}^3`, such as :math:`(-1,-1,+1)` and :math:`(+1,-1,+1)`,
    * Retrieve :cpp:func:`x <cubexx::corner_t::x()>`, :cpp:func:`y <cubexx::corner_t::y()>`, :cpp:func:`z <cubexx::corner_t::z()>` origin centered coordinates,
* Obtain all (3) adjacent corners,
* Push a corner in a :ref:`direction <cubexx-direction-overview>`, to a neighboring corner (no wrap around),
* Move a corner in a :ref:`direction <cubexx-direction-overview>`, to a neighboring corner (with wrap around),
* Obtain adjacent (3) :ref:`faces <cubexx-face-overview>`,
* Obtain or (3) adjacent :ref:`edges <cubexx-edge-overview>`,
* Obtain the opposite corner,
* Retrieve an index suitable for array storage,
    * The index is naturally in 3D `morton-order <https://en.wikipedia.org/wiki/Z-order_curve>`_ for the cube-space,
    * And the inverse, retrieve a corner via its index
* Reason about sets of corners (using :cpp:class:`cubexx::corner_set_t <cubexx::corner_set_t>`).

.. _cubexx-direction-overview:

cubexx::direction_t Overview
------------------------------------------------

The :cpp:class:`cubexx::direction_t <cubexx::direction_t>` represents a direction in the cube.

* Directions and :ref:`faces <cubexx-face-overview>` are related `1:1`.
* There are 6 directions for 6 faces.
* Directions and faces and are convertible to each-other. They are separated because they intuitively separate certain functionality.

Once again, you can do things like:

* Obtain a direction via 3 vector coordinates, representing the direction as a relative vector from origin
    , i.e coordinates in :math:`{0,-1,+1}^3`, such as :math:`(+0,0,+1)` and :math:`(0,-1,0)`,
    
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

The :cpp:class:`cubexx::edge_t <cubexx::edge_t>` represents the edge of a cube.

* There are 12 edges in a cube.

You can use it to:

* Obtain the lower and upper :ref:`corners <cubexx-corner-overview>` of the edge,
* Obtain all (4) adjacent edges,
* Obtain all (2) adjacent :ref:`faces <cubexx-face-overview>`,
* Obtain the opposite edge,
* Retrieve an index suitable for array storage,
    * And the inverse, retrieve a edge via its index
* Reason about sets of edges (using :cpp:class:`cubexx::edge_set_t <cubexx::edge_set_t>`).





.. _cubexx-face-overview:

cubexx::face_t Overview
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




.. _cubexx-set-overview:

cubexx::set_base_t Overview
------------------------------------------------

cubexx comes with set types, which can be used to store and reason about combinations of
{corners,directions,edges,faces}. These set types inherit from the type :cpp:class:`cubexx::set_base_t <cubexx::set_base_t>`.
The there is one set type for each {corners,directions,edges,faces}.

* :cpp:class:`cubexx::corner_set_t <cubexx::corner_set_t>`
* :cpp:class:`cubexx::direction_set_t <cubexx::direction_set_t>`
* :cpp:class:`cubexx::edge_set_t <cubexx::edge_set_t>`
* :cpp:class:`cubexx::face_set_t <cubexx::face_set_t>`

The set supports:

* insertion
* union
* difference
* erasure
* intersection
* membership testing
* iteration







Usage
------------------------


.. literalinclude:: /../../src/examples/cubexx-docs-example.cpp
   :language: c++
   :linenos:


Reference pages:
------------------------

.. toctree::
   :maxdepth: 2
   
   cubexx-corner
   cubexx-direction
   cubexx-edge
   cubexx-face
   cubexx-sets





