#ifndef CORNER_CASES_CUBELIB_CUBELIB_H
#define CORNER_CASES_CUBELIB_CUBELIB_H 1


#include "cubelib/cubelib.fwd.h"

#ifdef __OPENCL_VERSION__

    #include "cubelib.clgen.h"

    #define GLOBAL_STATIC_CONST __constant const
    #define LOCAL_STATIC_CONST const
    #define CUBELIB_FUNCTION_QUALIFIER static inline
    
    typedef uint8_t uint_fast8_t;
    
#else

    #include <assert.h>
    #include <stdint.h>
    #include <stdlib.h>

    #define CUBELIB_GLOBAL_STATIC_CONST static const
    #define CUBELIB_LOCAL_STATIC_CONST static const
    
    #ifndef CUBELIB_FUNCTION_QUALIFIER
      #define CUBELIB_FUNCTION_QUALIFIER static inline
    #endif
    
#endif

#ifdef __cplusplus
extern "C"{
#endif

    typedef uint_fast8_t cubelib_corner_value_t;
    typedef uint_fast8_t cubelib_direction_value_t;
    typedef uint_fast8_t cubelib_edge_value_t;
    typedef uint_fast8_t cubelib_face_value_t;
    
    
    /** @addtogroup cubelib-corner-group Corner API
     *  This is the API for cubelib_corner_t.
     *  @{
     */
    ///@brief Struct that represents a corner.
    ///@see cubelib_get_corner_by_int3(), cubelib_get_corner_index(), cubelib_get_corner_by_index()
    ///@see cubelib_is_corner_valid(), cubelib_is_corner_null(), cubelib_is_corner_equal()
    typedef struct cubelib_corner_t{
        cubelib_corner_value_t value;
    } cubelib_corner_t;
    /** @} */ // end of group cubelib-corner-group

    /** @addtogroup cubelib-direction-group Direction API
     *  This is the API for cubelib_direction_t.
     *  @{
     */
    ///@brief Struct that represents a direction.
    ///@see cubelib_get_direction_by_int3(), cubelib_get_direction_index(), cubelib_get_direction_by_index()
    ///@see cubelib_is_direction_valid(), cubelib_is_direction_null(), cubelib_is_direction_equal()
    typedef struct cubelib_direction_t{
        cubelib_direction_value_t value;
    } cubelib_direction_t;
    /** @} */ // end of group cubelib-direction-group

    
    /** @addtogroup cubelib-edge-group Edge API
     *  This is the API for cubelib_edge_t.
     *  @{
     */
    typedef struct cubelib_edge_t{
        cubelib_edge_value_t value;
    } cubelib_edge_t;
    /** @} */ // end of group cubelib-edge-group

    /** @addtogroup cubelib-face-group Face API
     *  This is the API for cubelib_face_t.
     *  @{
     */
    ///@see cubelib_get_direction_face(), cubelib_get_face_index(), cubelib_get_face_by_index()
    ///@see cubelib_is_face_valid(), cubelib_is_face_null(), cubelib_is_face_equal()
    typedef struct cubelib_face_t{
        cubelib_face_value_t value;
    } cubelib_face_t;
    /** @} */ // end of group cubelib-face-group

    /** @addtogroup cubelib-corner-group
     *  @{
     */
    #define CUBELIB_NULL_CORNER (cubelib_corner_t){8}
    #define CUBELIB_CORNERS_SIZE 8
    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t cubelib_null_corner = CUBELIB_NULL_CORNER;
    CUBELIB_GLOBAL_STATIC_CONST size_t cubelib_corners_size = CUBELIB_CORNERS_SIZE;
    /** @} */ // end of group cubelib-corner-group
    
    /** @addtogroup cubelib-direction-group
     *  @{
     */
    #define CUBELIB_NULL_DIRECTION (cubelib_direction_t){0}
    #define CUBELIB_DIRECTIONS_SIZE 6
    CUBELIB_GLOBAL_STATIC_CONST cubelib_direction_t cubelib_null_direction = CUBELIB_NULL_DIRECTION;
    CUBELIB_GLOBAL_STATIC_CONST size_t cubelib_directions_size = CUBELIB_DIRECTIONS_SIZE;
    /** @} */ // end of group cubelib-direction-group
    /** @addtogroup cubelib-edge-group
     *  @{
     */
    #define CUBELIB_NULL_EGDE (cubelib_edge_t){12}
    #define CUBELIB_EDGES_SIZE 12
    CUBELIB_GLOBAL_STATIC_CONST cubelib_edge_t cubelib_null_edge = CUBELIB_NULL_EGDE;
    CUBELIB_GLOBAL_STATIC_CONST size_t cubelib_edges_size = CUBELIB_EDGES_SIZE;
    /** @} */ // end of group cubelib-edge-group
    /** @addtogroup cubelib-face-group
     *  @{
     */
    #define CUBELIB_NULL_FACE (cubelib_face_t){0}
    #define CUBELIB_FACES_SIZE 6
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t cubelib_null_face = CUBELIB_NULL_FACE;
    CUBELIB_GLOBAL_STATIC_CONST size_t cubelib_faces_size = CUBELIB_FACES_SIZE;
    /** @} */ // end of group cubelib-face-group
    

    
    

/* -------------------------------------------------------------------------- */
/** @addtogroup cubelib-corner-group
 *  @{
 */


 
    ///@brief Checks that a corner is not corrupted somehow and represents a valid corner (including a null corner).
    ///@see cubelib_is_corner_null()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_valid(cubelib_corner_t corner);
    ///@brief Returns true if this corner is a null corner.
    ///@see cubelib_is_corner_valid(), cubelib_null_corner, CUBELIB_NULL_CORNER
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_null(cubelib_corner_t corner);
    ///@brief Comparison.
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_equal(cubelib_corner_t left, cubelib_corner_t right);

    
    ///@brief get the "origin-centered-cube-representation" x component of the corner.
    ///@see cubelib_get_corner_y(), cubelib_get_corner_z(), cubelib_get_corner_i(), cubelib_get_corner_ui(), cubelib_get_corner_ux()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_x(cubelib_corner_t corner);
    ///@brief get the "origin-centered-cube-representation" y component of the corner.
    ///@see cubelib_get_corner_x(), cubelib_get_corner_z(), cubelib_get_corner_i(), cubelib_get_corner_ui(), cubelib_get_corner_uy()
    ///@see cubelib_get_corner_by_int3()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_y(cubelib_corner_t corner);
    ///@brief get the "origin-centered-cube-representation" z component of the corner.
    ///@see cubelib_get_corner_x(), cubelib_get_corner_y(), cubelib_get_corner_i(), cubelib_get_corner_ui(), cubelib_get_corner_uz()
    ///@see cubelib_get_corner_by_int3()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_z(cubelib_corner_t corner);
    ///@brief get the "origin-centered-cube-representation" i-th component of the corner.
    ///@param dim an index representing the component dimension.
    ///@see cubelib_get_corner_x(), cubelib_get_corner_y(), cubelib_get_corner_z(), cubelib_get_corner_ui()
    ///@see cubelib_get_corner_by_int3()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_i(cubelib_corner_t corner, uint_fast8_t dim);
    
    ///@brief get the "unit-cube-representation" x component of the corner.
    ///@see cubelib_get_corner_uy(), cubelib_get_corner_uz(), cubelib_get_corner_ui(), cubelib_get_corner_i(), cubelib_get_corner_x()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_ux(cubelib_corner_t corner);
    ///get the "unit-cube-representation" y component of the corner.
    ///@see cubelib_get_corner_ux(), cubelib_get_corner_uz(), cubelib_get_corner_ui(), cubelib_get_corner_i(), cubelib_get_corner_y()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_uy(cubelib_corner_t corner);
    ///@brief get the "unit-cube-representation" z component of the corner.
    ///@see cubelib_get_corner_ux(), cubelib_get_corner_uy(), cubelib_get_corner_ui(), cubelib_get_corner_i(), cubelib_get_corner_z()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_uz(cubelib_corner_t corner);
    ///@brief get the "unit-cube-representation" i-th component of the corner.
    ///@param dim an index representing the component dimension.
    ///@see cubelib_get_corner_ux(), cubelib_get_corner_uy(), cubelib_get_corner_uz(), cubelib_get_corner_i()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_ui(cubelib_corner_t corner, uint_fast8_t dim);

    ///@brief gets a corner using 3 floats, same as cubelib_get_corner_by_int3().
    ///@see cubelib_get_corner_by_int3()
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_corner_by_float3(float x, float y, float z);
    ///@brief Gets a corner using 3 component integer vector representation.
    ///
    ///Each component represents a dimension in the cube; if the component value
    /// is more than 0, then it is an upper corner (in that dimension); if it is zero
    /// or negative, then it is a lower corner. This accomodates both cube representations,
    /// "unit-cube-representation" (where @f$x,y,z \in \left\{0,1\right\}@f$) or the "origin-centered-cube-representation"
    /// (where @f$x,y,z \in \left\{-1,+1\right\}@f$).
    ///
    ///@see cubelib_get_corner_x(), cubelib_get_corner_y(), cubelib_get_corner_z(), cubelib_get_corner_i()
    ///@see cubelib_get_corner_ux(), cubelib_get_corner_uy(), cubelib_get_corner_uz(), cubelib_get_corner_ui()
    ///@see cubexx::corner_t::get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t)
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_corner_by_int3(int x, int y, int z);

    ///@brief Returns a 0-based index that represents this corner; for use in array indexing and similar.
    ///
    ///The index returned will be in 3D [Z-order curve](https://en.wikipedia.org/wiki/Z-order_curve)
    /// also known as "Morton order", within the cube, in the range @f$[0,7]@f$.
    ///@see cubelib_get_corner_by_index()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_corner_index(cubelib_corner_t corner);
    ///@brief Return a corner based on the 0-based index returned by cubelib_get_corner_index().
    ///
    ///@see cubelib_get_corner_index()
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_corner_by_index(uint_fast8_t index);

    /**
     * @brief Return the corner on the opposite side of the cube.
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_opposite_corner(cubelib_corner_t corner);
    
    /**
     * @brief Retrieve an adjacent a corner in a specified dimension.
     *
     * If the new corner cannot be moved in the specified direction
     *  then this method will wrap around and return the adjacent corner in the specified direction.
     * @see cubelib_corner_push(), cubelib_corner_move()
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_adjacent_corner(cubelib_corner_t corner, cubelib_direction_t direction);

    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_calc_adjacent_corner(cubelib_corner_t corner, cubelib_direction_t direction);
    
    /**
     * @brief Move a corner in a specified direction.
     * 
     & If the new corner cannot be moved in that direction
     *  then this method will return a @c null_corner.
     * @see cubelib_corner_push(), cubelib_get_adjacent_corner()
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_corner_move(cubelib_corner_t corner, cubelib_direction_t direction);
    /**
     * @brief Push a corner in a specified direction.
     *
     * If the new corner cannot be moved in that direction
     *  then this method will return the same corner.
     * @see cubelib_corner_move(), cubelib_get_adjacent_corner()
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_corner_push(cubelib_corner_t corner, cubelib_direction_t direction);
    
    
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_calc_is_corner_adjacent_corner(cubelib_corner_t corner0, cubelib_corner_t corner1);
    
    ///@brief Returns true if the two corners are adjacent.
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_adjacent_corner(cubelib_corner_t corner0, cubelib_corner_t corner1);
    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_calc_cnr_adj_cnr(cubelib_corner_t corner, uint_fast8_t dim);
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_calc_cnr_adj_edge(cubelib_corner_t corner, uint_fast8_t idx);
    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_calc_cnr_adj_face(cubelib_corner_t corner, uint_fast8_t idx);
/** @} */ // end of group cubelib-corner-group

/* -------------------------------------------------------------------------- */
/** @addtogroup cubelib-direction-group
 *  @{
 */

    ///@brief Checks that a direction is not corrupted somehow and represents a valid direction (including a null direction).
    ///@see cubelib_is_direction_null(), cubelib_null_direction, CUBELIB_NULL_DIRECTION
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_direction_valid(cubelib_direction_t direction);
    ///@brief Returns true if this corner is a null corner.
    ///@see cubelib_is_corner_valid()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_direction_null(cubelib_direction_t direction);
    ///@brief Comparison.
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_direction_equal(cubelib_direction_t left, cubelib_direction_t right);
    
    ///@brief Returns the x-component of the vector representation of this direction.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_y(), cubelib_get_direction_z(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absy(), cubelib_get_direction_absz(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_x(cubelib_direction_t direction);
    ///@brief Returns the y-component of the vector representation of this direction.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_x(), cubelib_get_direction_z(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absy(), cubelib_get_direction_absz(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_y(cubelib_direction_t direction);
    ///@brief Returns the z-component of the vector representation of this direction.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_x(), cubelib_get_direction_y(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absy(), cubelib_get_direction_absz(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_z(cubelib_direction_t direction);
    ///@brief Returns the specified ith-component of the vector representation of this direction.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_x(), cubelib_get_direction_y(), cubelib_get_direction_z()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absy(), cubelib_get_direction_absz(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_i(cubelib_direction_t direction, uint_fast8_t dim);
    ///@brief Returns the absolute value of the x-component of the vector representation of this direction.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_x(), cubelib_get_direction_y(), cubelib_get_direction_z(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absy(), cubelib_get_direction_absz(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_absx(cubelib_direction_t direction);
    ///@brief Returns the absolute value of the y-component of the vector representation of this direction.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_x(), cubelib_get_direction_y(), cubelib_get_direction_z(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absz(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_absy(cubelib_direction_t direction);
    ///@brief Returns the absolute value of the z-component of the vector representation of this direction.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_x(), cubelib_get_direction_y(), cubelib_get_direction_z(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absy(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_absz(cubelib_direction_t direction);
    ///@brief Returns the absolute value of the ith-component of the vector representation of this direction.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_x(), cubelib_get_direction_y(), cubelib_get_direction_z(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absy(), cubelib_get_direction_absz()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_absi(cubelib_direction_t direction, uint_fast8_t dim);
    
    
    
    ///@brief Returns the dimension of the vector-representation of a direction that has a non-zero value.
    ///
    ///So for example, :math:`(1,0,0)` will result in `0`; :math:`(0,-1,0)` will be `1`; :math:`(0,0,-1)` will be `2`.
    ///
    ///@see cubelib_get_direction_by_int3()
    ///@see cubelib_get_direction_x(), cubelib_get_direction_y(), cubelib_get_direction_z(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absy(), cubelib_get_direction_absz(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_sigdim(cubelib_direction_t direction);
    
    
    
    
    ///@brief Returns a 0-based index that represents this direction; for use in array indexing and similar.
    ///
    ///@see cubelib_get_direction_by_index()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_index(cubelib_direction_t direction);
    ///@brief Return a direction based on the 0-based index returned by cubelib_get_direction_index().
    ///
    ///@see cubelib_get_direction_index()
    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t cubelib_get_direction_by_index(uint_fast8_t index);
    
    ///@brief Returns a direction represented by a given 3 component vector.
    ///
    ///@note two of the components must be zero, and one component must be non-zero.
    ///
    ///Examples:
    ///* `cubelib_get_direction_by_int3(0,+1,0)` Returns a direction pointing in the direction of the
    ///       positive Y axis from the the center of the cube
    ///* `cubelib_get_direction_by_int3(0,0,-1)` Returns a direction pointing in the direction of the
    ///       negative Z axis from the the center of the cube
    ///
    ///@see cubelib_get_direction_x(), cubelib_get_direction_y(), cubelib_get_direction_z(), cubelib_get_direction_i()
    ///@see cubelib_get_direction_absx(), cubelib_get_direction_absy(), cubelib_get_direction_absz(), cubelib_get_direction_absi()
    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t cubelib_get_direction_by_int3(int x, int y, int z);
    
    /**
     * @brief Returns the opposite direction.
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t cubelib_get_opposite_direction(cubelib_direction_t direction);
    
    ///@brief convert a direction to a face
    ///@see cubelib_face_t, cubelib_get_face_direction()
    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_get_direction_face(cubelib_direction_t direction);
    
    
/** @} */ // end of group cubelib-direction-group
/* -------------------------------------------------------------------------- */

    
    
    
    ///@brief Checks that a edge is not corrupted somehow and represents a valid edge (including a null edge).
    ///@see cubelib_cubelib_is_edge_null(), cubelib_null_edge, CUBELIB_NULL_EDGE
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_valid(cubelib_edge_t edge);
    ///@brief Returns true if this edge is a null edge.
    ///@see cubelib_is_edge_valid()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_null(cubelib_edge_t edge);
    ///@brief Comparison.
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_equal(cubelib_edge_t left, cubelib_edge_t right);
    
    /**
     * @brief Returns an edge based on a projected-axis representation.
     * 
     * The representation works as follows:
     *
     * Every edge has a base-axis, which is one of the x,y, or z axes, which are 0, 1, or 2 respectively.
     *
     * After choosing the base-axis, the "secondary" and "tertiary" axis are incremented from base axis (modulo 3).
     * So if the base-axis is 0, then the secondary is 1, and the tertiary is 2.
     * If the base-axis is 1, then the secondary is 2, and the tertiary is 0.
     * If the base-axis is 2, then the secondary is 0, and the tertiary is 1.
     *
     * Then, overlay the axis with an edge, starting at the origin, and ending at a distance of 1 along the axis.
     *
     * Then, we optionally project the edge across the cube along the secondary axis.
     * Then, we optionally project the edge across the cube along the tertiary axis.
     *
     * Thus any edge can be represented by a base-axis, and two booleans determining if the edge is projected
     * across the secondary and tertiary axes.
     *
     * @see cubelib_get_edge_base_axis(), cubelib_get_edge_secondary_axis(), cubelib_get_edge_tertiary_axis(),
     *          cubelib_is_edge_projected_secondary(), cubelib_is_edge_projected_tertiary()
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_by_axis(uint_fast8_t base_axis, bool project_secondary, bool project_tertiary);
    
    ///@brief returns the edge adjacent to the given corner, in the given direction 
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_by_corner_direction(cubelib_corner_t corner, cubelib_direction_t direction);
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_calc_edge_by_corner_direction(cubelib_corner_t corner, cubelib_direction_t direction);
    ///@brief given two corners, this returns the edge between them. the given corners must be adjacent; the same edge will
    /// be returned regardless of the order of the given corners.
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_by_corners(cubelib_corner_t corner0, cubelib_corner_t corner1);
    ///@brief Return a edge based on the 0-based index returned by cubelib_get_edge_index().
    ///
    ///@see cubelib_get_edge_index()
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_by_index(uint_fast8_t index);
    ///@brief Returns a 0-based index that represents this edge; for use in array indexing and similar.
    ///
    ///@see cubelib_get_edge_by_index()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_edge_index(cubelib_edge_t edge);
    ///@brief Returns the edge on the opposite side of the cube.
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_opposite_edge(cubelib_edge_t edge);
    
    ///@brief Return the lower corner of the edge (where lower means closer to the lower corner of the cube).
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_edge_corner0(cubelib_edge_t edge);
    ///@brief Return the upper corner of the edge (where lower means closer to the lower corner of the cube
    /// and upper is the opposite corner of the lower corner).
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_edge_corner1(cubelib_edge_t edge);
    
    ///@brief Returns a direction that is parallel to the edge; it will always be a positively
    /// pointing direction.
    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t cubelib_get_edge_direction(cubelib_edge_t edge);
    
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_calc_edge_base_axis(cubelib_edge_t edge);
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_calc_edge_secondary_axis(cubelib_edge_t edge);
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_calc_edge_tertiary_axis(cubelib_edge_t edge);
    ///@brief Returns the "base-axis" of this edge.
    ///@see cubelib_get_edge_by_axis(), cubelib_get_edge_secondary_axis(), cubelib_get_edge_tertiary_axis()
    ///@see cubelib_is_edge_projected_secondary(), cubelib_is_edge_projected_tertiary()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_edge_base_axis(cubelib_edge_t edge);
    ///@brief Returns the "secondary-axis" of this edge.
    ///@see cubelib_get_edge_by_axis(), cubelib_get_edge_base_axis(), cubelib_get_edge_tertiary_axis()
    ///@see cubelib_is_edge_projected_secondary(), cubelib_is_edge_projected_tertiary()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_edge_secondary_axis(cubelib_edge_t edge);
    ///@brief Returns the "tertiary-axis" of this edge.
    ///@see cubelib_get_edge_by_axis(), cubelib_get_edge_base_axis(), cubelib_get_edge_secondary_axis()
    ///@see cubelib_is_edge_projected_secondary(), cubelib_is_edge_projected_tertiary()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_edge_tertiary_axis(cubelib_edge_t edge);
    ///@brief Returns true if this edge is projected across the "secondary-axis".
    ///@see cubelib_get_edge_by_axis(), cubelib_get_edge_base_axis(), cubelib_get_edge_secondary_axis(), cubelib_get_edge_tertiary_axis()
    ///@see cubelib_is_edge_projected_tertiary()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_projected_secondary(cubelib_edge_t edge);
    ///@brief Returns true if this edge is projected across the "tertiary-axis".
    ///@see cubelib_get_edge_by_axis(), cubelib_get_edge_base_axis(), cubelib_get_edge_secondary_axis(), cubelib_get_edge_tertiary_axis()
    ///@see cubelib_is_edge_projected_secondary()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_projected_tertiary(cubelib_edge_t edge);

    ///@brief Returns an edge movedacross a face in the given direction; if there is no edge in the specified direction,
    /// just returns a copy.
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_edge_push(cubelib_edge_t edge, cubelib_direction_t direction);
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_calc_edge_push(cubelib_edge_t edge, cubelib_direction_t direction);
    
    //@brief Returns true if a corner is on an edge
    // CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_adjacent_edge(cubelib_corner_t corner, cubelib_edge_t edge);
    // CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_direction_adjacent_edge(cubelib_direction_t direction, cubelib_edge_t edge);
    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_calc_face_on_edge(cubelib_edge_t edge, uint_fast8_t idx);
    ///@brief Retrieve the faces on an edge.
    ///@param edge the edge.
    ///@param idx a number in the range `[0,1]` that represents which corner on the face to retrieve.
    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_get_face_on_edge(cubelib_edge_t edge, uint_fast8_t idx);
    
    
    ///@brief Returns true if the given face is on the given edge
    ///@see cubelib_is_edge_on_face()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_face_on_edge(cubelib_face_t face, cubelib_edge_t edge);
    
    
    
/* -------------------------------------------------------------------------- */

    
    
    
    ///@brief Checks that a face is not corrupted somehow and represents a valid face (including a null face).
    ///@see cubelib_is_face_null(), cubelib_null_face CUBELIB_NULL_FACE
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_face_valid(cubelib_face_t face);
    ///@brief Returns true if this face is a null face.
    ///@see cubelib_is_face_valid()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_face_null(cubelib_face_t face);
    ///@brief Comparison.
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_face_equal(cubelib_face_t left, cubelib_face_t right);
    
    ///@brief Return a face based on the 0-based index returned by cubelib_get_face_index().
    ///
    ///@see cubelib_get_facedex()
    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_get_face_by_index(uint_fast8_t index);
    ///@brief Returns a 0-based index that represents this face; for use in array indexing and similar.
    ///
    ///@see cubelib_get_face_by_index()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_face_index(cubelib_face_t face);
    
    ///@brief convert a face to a direction
    ///@see cubelib_direction_t, cubelib_get_direction_face()
    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t cubelib_get_face_direction(cubelib_face_t face);
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_calc_edge_on_face(cubelib_face_t face, uint_fast8_t idx);
    ///@brief Retrieve the edges on a face.
    ///@param face the face.
    ///@param idx a number in the range `[0,3]` that represents which edge on the face to retrieve.
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_on_face(cubelib_face_t face, uint_fast8_t idx);
    
    ///@brief Returns true if the given edge is on the given face
    ///@see cubelib_is_face_on_edge()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_on_face(cubelib_edge_t edge, cubelib_face_t face);
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_calc_corner_on_face(cubelib_face_t face, uint_fast8_t idx);
    ///@brief Retrieve the corners on a face.
    ///@param face the face.
    ///@param idx a number in the range `[0,3]` that represents which corner on the face to retrieve.
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_corner_on_face(cubelib_face_t face, uint_fast8_t idx);
    
    
    ///@brief Returns true if the given corner is on the given face
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_on_face(cubelib_corner_t corner, cubelib_face_t face);
    
    
/* -------------------------------------------------------------------------- */
    
    
    /*
     * ---------------------------------------------------------------------
     * Corner constants
     * ---------------------------------------------------------------------
     */
#ifdef __OPENCL_VERSION__
    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t cubelib_cnr_adj_cnrs[8][3] = CUBELIB_CNR_ADJ_CNRS;
#else
/** @addtogroup cubelib-corner-group
 *  @{
 */
    
    ///@brief Index through this array with the corner's index, to obtain the 3 adjacent corners.
    ///
    ///Example:
    ///
    ///\code{.c}
    ///for (int i = 0; i < 3; ++i)
    ///    cubelib_corner_t adj_corner_i = cubelib_cnr_adj_cnrs[ cubelib_get_corner_index(corner) ][i];
    ///\endcode
    ///
    ///@see cubelib_get_corner_index()
    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t cubelib_cnr_adj_cnrs[8][3] =
        {
              {   cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(0), 0)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(0), 1)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(0), 2)}
            , {   cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(1), 0)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(1), 1)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(1), 2)}
            , {   cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(2), 0)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(2), 1)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(2), 2)}
            , {   cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(3), 0)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(3), 1)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(3), 2)}
            , {   cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(4), 0)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(4), 1)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(4), 2)}
            , {   cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(5), 0)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(5), 1)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(5), 2)}
            , {   cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(6), 0)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(6), 1)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(6), 2)}
            , {   cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(7), 0)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(7), 1)
                , cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(7), 2)}

        };
    
#endif
    
    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t cubelib_all_corners[8] = { {0}, {1}
                                            , {2}, {3}
                                            , {4}, {5}
                                            , {6}, {7}
                                            };
/** @} */ // end of group cubelib-corner-group

    /*
     * ---------------------------------------------------------------------
     * Direction constants
     * ---------------------------------------------------------------------
     */

    CUBELIB_GLOBAL_STATIC_CONST cubelib_direction_t cubelib_all_directions[6] = { {1}, {2}, {3}, {4}, {5}, {6} };

    /*
     * ---------------------------------------------------------------------
     * edge constants
     * ---------------------------------------------------------------------
     */

#ifdef __OPENCL_VERSION__
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t cubelib_faces_on_edge[6][4] = CUBELIB_FACES_ON_EDGE;
#else
/** @addtogroup cubelib-edge-group
 *  @{
 */
 
    ///@brief Index through this array with the edges's index, to obtain the 2 adjacent faces.
    ///
    ///Example:
    ///
    ///\code{.c}
    ///for (int i = 0; i < 2; ++i)
    ///    cubelib_face_t face_on_edge = cubelib_faces_on_edge[ cubelib_get_edge_index(edge) ][i];
    ///\endcode
    ///
    ///@see cubelib_get_edge_index()
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t cubelib_faces_on_edge[12][2] =
        {
              {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(0), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(0), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(1), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(1), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(2), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(2), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(3), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(3), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(4), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(4), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(5), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(5), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(6), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(6), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(7), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(7), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(8), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(8), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(9), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(9), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(10), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(10), 1)}
            , {   cubelib_calc_face_on_edge(cubelib_get_edge_by_index(11), 0)
                , cubelib_calc_face_on_edge(cubelib_get_edge_by_index(11), 1)}
        };
    
    
    
    
#endif

     
    CUBELIB_GLOBAL_STATIC_CONST cubelib_edge_t cubelib_all_edges[12] = {  {0}, {1}
                                                        , {2}, {3}
                                                        , {4}, {5}
                                                        , {6}, {7}
                                                        , {8}, {9}
                                                        , {10}, {11}
                                                        };

    /*
     * ---------------------------------------------------------------------
     * Face constants
     * ---------------------------------------------------------------------
     */

#ifdef __OPENCL_VERSION__
    CUBELIB_GLOBAL_STATIC_CONST cubelib_edge_t cubelib_edges_on_face[6][4] = CUBELIB_EDGES_ON_FACE;
#else
/** @addtogroup cubelib-face-group
 *  @{
 */
 
    ///@brief Index through this array with the face's index, to obtain the 4 adjacent corners.
    ///
    ///Example:
    ///
    ///\code{.c}
    ///for (int i = 0; i < 4; ++i)
    ///    cubelib_edge_t edge_on_face = cubelib_edges_on_face[ cubelib_get_face_index(face) ][i];
    ///\endcode
    ///
    ///@see cubelib_get_face_index()
    CUBELIB_GLOBAL_STATIC_CONST cubelib_edge_t cubelib_edges_on_face[6][4] =
        {
              {   cubelib_calc_edge_on_face(cubelib_get_face_by_index(0), 0)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(0), 1)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(0), 2)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(0), 3)}
            , {   cubelib_calc_edge_on_face(cubelib_get_face_by_index(1), 0)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(1), 1)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(1), 2)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(1), 3)}
            , {   cubelib_calc_edge_on_face(cubelib_get_face_by_index(2), 0)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(2), 1)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(2), 2)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(2), 3)}
            , {   cubelib_calc_edge_on_face(cubelib_get_face_by_index(3), 0)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(3), 1)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(3), 2)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(3), 3)}
            , {   cubelib_calc_edge_on_face(cubelib_get_face_by_index(4), 0)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(4), 1)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(4), 2)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(4), 3)}
            , {   cubelib_calc_edge_on_face(cubelib_get_face_by_index(5), 0)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(5), 1)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(5), 2)
                , cubelib_calc_edge_on_face(cubelib_get_face_by_index(5), 3)}
        };
    
    
    
    
#endif

#ifdef __OPENCL_VERSION__
    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t cubelib_corners_on_face[6][4] = CUBELIB_CORNERS_ON_FACE;
#else

    
    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t cubelib_corners_on_face[6][4] = {
        {     cubelib_calc_corner_on_face(cubelib_get_face_by_index(0), 0)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(0), 1)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(0), 2)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(0), 3)}
        , {   cubelib_calc_corner_on_face(cubelib_get_face_by_index(1), 0)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(1), 1)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(1), 2)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(1), 3)}
        , {   cubelib_calc_corner_on_face(cubelib_get_face_by_index(2), 0)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(2), 1)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(2), 2)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(2), 3)}
        , {   cubelib_calc_corner_on_face(cubelib_get_face_by_index(3), 0)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(3), 1)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(3), 2)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(3), 3)}
        , {   cubelib_calc_corner_on_face(cubelib_get_face_by_index(4), 0)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(4), 1)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(4), 2)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(4), 3)}
        , {   cubelib_calc_corner_on_face(cubelib_get_face_by_index(5), 0)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(5), 1)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(5), 2)
            , cubelib_calc_corner_on_face(cubelib_get_face_by_index(5), 3)}
    };
    
#endif
    
#ifdef __OPENCL_VERSION__
    
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t posxface = POSXFACE;
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t posyface = POSYFACE;
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t poszface = POSZFACE;
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t negxface = NEGXFACE;
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t negyface = NEGYFACE;
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t negzface = NEGZFACE;
    
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t all_faces[6] = cubelib_all_directions;
#else

    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t posxface = cubelib_get_direction_face(cubelib_get_direction_by_int3(+1, 0, 0));
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t posyface = cubelib_get_direction_face(cubelib_get_direction_by_int3( 0,+1, 0));
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t poszface = cubelib_get_direction_face(cubelib_get_direction_by_int3( 0, 0,+1));
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t negxface = cubelib_get_direction_face(cubelib_get_direction_by_int3(-1, 0, 0));
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t negyface = cubelib_get_direction_face(cubelib_get_direction_by_int3( 0,-1, 0));
    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t negzface = cubelib_get_direction_face(cubelib_get_direction_by_int3( 0, 0,-1));
#endif

    CUBELIB_GLOBAL_STATIC_CONST cubelib_face_t cubelib_all_faces[6] = { {1}, {2}, {3}, {4}, {5}, {6} };

#ifdef __cplusplus
}
#endif


#include "cubelib/detail/cubelib.inl.h"

#endif // CORNER_CASES_CUBELIB_CUBELIB_H

