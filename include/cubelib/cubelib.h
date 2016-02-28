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

    typedef uint_fast8_t corner_value_t;
    typedef uint_fast8_t direction_value_t;
    typedef uint_fast8_t edge_value_t;
    
    
    /** @addtogroup  cubelib-corner-group Corner API
     *  This is the API for cubelib::corner_t.
     *  @{
     */
    ///Struct that represents a corner.
    ///@see cubelib_get_corner_by_int3(), cubelib_get_corner_index(), cubelib_get_corner_by_index()
    ///@see cubelib_is_corner_valid(), cubelib_is_corner_null(), cubelib_is_corner_equal()
    typedef struct cubelib_corner_t{
        corner_value_t value;
    } cubelib_corner_t;
    /** @} */ // end of group cubelib-corner-group

    typedef struct direction_t{
        direction_value_t value;
    } direction_t;

    
    typedef struct edge_t{
        edge_value_t value;
    } edge_t;

    #define NULL_CORNER (cubelib_corner_t){8}
    #define NULL_DIRECTION (direction_t){0}
    #define NULL_EGDE (edge_t){12}
    #define NULL_FACE (face_t){0}
    
    #define CORNERS_SIZE 8
    #define DIRECTIONS_SIZE 6
    #define EDGES_SIZE 12
    #define FACES_SIZE 6

    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t null_corner = NULL_CORNER;
    CUBELIB_GLOBAL_STATIC_CONST direction_t null_direction = NULL_DIRECTION;
    CUBELIB_GLOBAL_STATIC_CONST edge_t null_edge = NULL_EGDE;
    CUBELIB_GLOBAL_STATIC_CONST face_t null_face = NULL_FACE;
    
    CUBELIB_GLOBAL_STATIC_CONST size_t corners_size = CORNERS_SIZE;
    CUBELIB_GLOBAL_STATIC_CONST size_t directions_size = DIRECTIONS_SIZE;
    CUBELIB_GLOBAL_STATIC_CONST size_t edges_size = EDGES_SIZE;
    CUBELIB_GLOBAL_STATIC_CONST size_t faces_size = FACES_SIZE;
    

/* -------------------------------------------------------------------------- */
/** @addtogroup  cubelib-corner-group
 *  @{
 */


 
    ///Checks that a corner is not corrupted somehow and represents a valid corner (including a null corner).
    ///@see cubelib_is_corner_null()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_valid(cubelib_corner_t corner);
    ///Returns true if this corner is a null corner.
    ///@see cubelib_is_corner_valid()
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_null(cubelib_corner_t corner);

    ///get the "origin-centered-cube-representation" x component of the corner.
    ///@see cubelib_get_corner_y(), cubelib_get_corner_z(), cubelib_get_corner_i(), cubelib_get_corner_ui(), cubelib_get_corner_ux()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_x(cubelib_corner_t corner);
    ///get the "origin-centered-cube-representation" y component of the corner.
    ///@see cubelib_get_corner_x(), cubelib_get_corner_z(), cubelib_get_corner_i(), cubelib_get_corner_ui(), cubelib_get_corner_uy()
    ///@see cubelib_get_corner_by_int3()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_y(cubelib_corner_t corner);
    ///get the "origin-centered-cube-representation" z component of the corner.
    ///@see cubelib_get_corner_x(), cubelib_get_corner_y(), cubelib_get_corner_i(), cubelib_get_corner_ui(), cubelib_get_corner_uz()
    ///@see cubelib_get_corner_by_int3()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_z(cubelib_corner_t corner);
    ///get the "origin-centered-cube-representation" i-th component of the corner.
    ///@param dim an index representing the component dimension.
    ///@see cubelib_get_corner_x(), cubelib_get_corner_y(), cubelib_get_corner_z(), cubelib_get_corner_ui()
    ///@see cubelib_get_corner_by_int3()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_i(cubelib_corner_t corner, uint_fast8_t dim);
    
    ///get the "unit-cube-representation" x component of the corner.
    ///@see cubelib_get_corner_uy(), cubelib_get_corner_uz(), cubelib_get_corner_ui(), cubelib_get_corner_i(), cubelib_get_corner_x()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_ux(cubelib_corner_t corner);
    ///get the "unit-cube-representation" y component of the corner.
    ///@see cubelib_get_corner_ux(), cubelib_get_corner_uz(), cubelib_get_corner_ui(), cubelib_get_corner_i(), cubelib_get_corner_y()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_uy(cubelib_corner_t corner);
    ///get the "unit-cube-representation" z component of the corner.
    ///@see cubelib_get_corner_ux(), cubelib_get_corner_uy(), cubelib_get_corner_ui(), cubelib_get_corner_i(), cubelib_get_corner_z()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_uz(cubelib_corner_t corner);
    ///get the "unit-cube-representation" i-th component of the corner.
    ///@param dim an index representing the component dimension.
    ///@see cubelib_get_corner_ux(), cubelib_get_corner_uy(), cubelib_get_corner_uz(), cubelib_get_corner_i()
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_ui(cubelib_corner_t corner, uint_fast8_t dim);

    ///gets a corner using 3 floats, same as cubelib_get_corner_by_int3().
    ///@see cubelib_get_corner_by_int3()
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_corner_by_float3(float x, float y, float z);
    ///Gets a corner using 3 ints.
    ///
    ///Each component represents a dimension in the cube; if the component value
    /// is more than 0, then it is an upper corner (in that dimension); if it is zero
    /// or negative, then it is a lower corner. This accomodates both cube representations,
    /// "unit-cube-representation" (where @f$x,y,z \in \left\{0,1\right\}@f$) or the "origin-centered-cube-representation"
    /// (where @f$x,y,z \in \left\{-1,+1\right\}@f$).
    ///
    ///@see cubelib_get_corner_x(), cubelib_get_corner_y(), cubelib_get_corner_z(), cubelib_get_corner_i()
    ///@see cubelib_get_corner_ux(), cubelib_get_corner_uy(), cubelib_get_corner_uz(), cubelib_get_corner_ui()
    ///@see cubexx::get(std::fast_int8_t,std::fast_int8_t,std::fast_int8_t)
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_corner_by_int3(int x, int y, int z);

    ///Returns a 0-based index that represents this corner; for use in array indexing and similar.
    ///The index returned will be in 3D [Z-order curve](https://en.wikipedia.org/wiki/Z-order_curve)
    /// also known as "Morton order", within the cube, in the range @f$[0,7]@f$.
    ///@see cubelib_get_corner_by_index()
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_corner_index(cubelib_corner_t corner);
    ///Return a corner based on the 0-based index returned by cubelib_get_corner_index().
    ///@see cubelib_get_corner_index()
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_corner_by_index(uint_fast8_t index);

    /**
     * Return the corner on the opposite side of the cube.
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_opposite_corner(cubelib_corner_t corner);
    
    /**
     * Move a corner in a specified direction. If the new corner cannot be moved in that direction
     *  then this method will wrap around and return the adjacent corner in the specified direction.
     * @see cubelib_corner_push(), cubelib_corner_move()
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_adjacent_corner(cubelib_corner_t corner, direction_t direction);

    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_calc_adjacent_corner(cubelib_corner_t corner, direction_t direction);
    
    /**
     * Move a corner in a specified direction. If the new corner cannot be moved in that direction
     *  then this method will return a @c null_corner.
     * @see cubelib_corner_push(), cubelib_get_adjacent_corner()
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_corner_move(cubelib_corner_t corner, direction_t direction);
    /**
     * Move a corner in a specified direction. If the new corner cannot be moved in that direction
     *  then this method will return the same corner.
     * @see cubelib_corner_move(), cubelib_get_adjacent_corner()
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_corner_push(cubelib_corner_t corner, direction_t direction);
    
    
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_calc_is_corner_adjacent_corner(cubelib_corner_t corner0, cubelib_corner_t corner1);
    
    ///Returns true if the two corners are adjacent.
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_adjacent_corner(cubelib_corner_t corner0, cubelib_corner_t corner1);
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_calc_cnr_adj_cnr(cubelib_corner_t corner, uint_fast8_t dim);
    
    ///Comparison.
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_equal(cubelib_corner_t left, cubelib_corner_t right);

/** @} */ // end of group cubelib-corner-group

/* -------------------------------------------------------------------------- */
    static inline int get_direction_x(direction_t direction);
    static inline int get_direction_y(direction_t direction);
    static inline int get_direction_z(direction_t direction);
    static inline int get_direction_i(direction_t direction, uint_fast8_t dim);
    static inline uint_fast8_t get_direction_absx(direction_t direction);
    static inline uint_fast8_t get_direction_absy(direction_t direction);
    static inline uint_fast8_t get_direction_absz(direction_t direction);
    static inline uint_fast8_t get_direction_absi(direction_t direction, uint_fast8_t dim);

    static inline uint_fast8_t get_direction_index(direction_t direction);
    static inline direction_t get_direction_by_index(uint_fast8_t index);
    static inline direction_t get_direction_by_int3(int x, int y, int z);
    static inline direction_t get_opposite_direction(direction_t direction);
    static inline direction_t get_opposite_face(direction_t direction);

    ///returns true if the given corner is on the given face
    static inline bool is_corner_adjacent_face(cubelib_corner_t corner, face_t face);
    
    static inline bool is_direction_valid(direction_t direction);
    static inline bool is_direction_null(direction_t direction);
    static inline bool is_direction_equal(direction_t left, direction_t right);
/* -------------------------------------------------------------------------- */
    
    /**
     * Returns an edge based on a projected-axis representation.
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
     * @see get_edge_base_axis(), get_edge_secondary_axis(), get_edge_tertiary_axis(),
     *          is_edge_projected_secondary(), is_edge_projected_tertiary()
     */
    static inline edge_t get_edge_by_axis(uint_fast8_t base_axis, bool project_secondary, bool project_tertiary);
    
    ///returns the edge adjacent to the given corner, in the given direction 
    static inline edge_t get_edge_by_corner_direction(cubelib_corner_t corner, direction_t direction);
    static inline edge_t calc_edge_by_corner_direction(cubelib_corner_t corner, direction_t direction);
    static inline edge_t get_edge_by_corners(cubelib_corner_t corner0, cubelib_corner_t corner1);
    static inline edge_t get_edge_by_index(uint_fast8_t index);
    static inline uint_fast8_t get_edge_index(edge_t edge);
    static inline edge_t get_opposite_edge(edge_t edge);
    
    static inline cubelib_corner_t get_edge_corner0(edge_t edge);
    static inline cubelib_corner_t get_edge_corner1(edge_t edge);
    
    static inline uint_fast8_t calc_edge_base_axis(edge_t edge);
    static inline uint_fast8_t calc_edge_secondary_axis(edge_t edge);
    static inline uint_fast8_t calc_edge_tertiary_axis(edge_t edge);
    static inline uint_fast8_t get_edge_base_axis(edge_t edge);
    static inline uint_fast8_t get_edge_secondary_axis(edge_t edge);
    static inline uint_fast8_t get_edge_tertiary_axis(edge_t edge);
    static inline bool is_edge_projected_secondary(edge_t edge);
    static inline bool is_edge_projected_tertiary(edge_t edge);

    
    
    static inline bool is_corner_adjacent_edge(cubelib_corner_t corner, edge_t edge);
    static inline bool is_direction_adjacent_edge(direction_t direction, edge_t edge);
    
    
    
    static inline bool is_edge_valid(edge_t edge);
    static inline bool is_edge_null(edge_t edge);
    static inline bool is_edge_equal(edge_t left, edge_t right);
    
    CUBELIB_GLOBAL_STATIC_CONST edge_t all_edges[12] = {  {0}, {1}
                                                        , {2}, {3}
                                                        , {4}, {5}
                                                        , {6}, {7}
                                                        , {8}, {9}
                                                        , {10}, {11}
                                                        };
    
    
/* -------------------------------------------------------------------------- */

    
#ifdef __OPENCL_VERSION__
    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t cubelib_cnr_adj_cnrs[8][3] = CUBELIB_CNR_ADJ_CNRS;
#else
/** @addtogroup  cubelib-corner-group
 *  @{
 */
 
    ///Index through this array with the corner's index, to obtain the 3 adjacent corners.
    ///`for (int i = 0; i < 3; ++i) cubelib_corner_t adj_corneri = cubelib_cnr_adj_cnrs[ cubelib_get_corner_index(corner) ][i];`
    ///Example:
    ///
    ///@see cubelib_get_corner_index()
    CUBELIB_GLOBAL_STATIC_CONST cubelib_corner_t cubelib_cnr_adj_cnrs[8][3] =
        {
            {     cubelib_calc_cnr_adj_cnr(cubelib_get_corner_by_index(0), 0)
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
     * Corner constants
     * ---------------------------------------------------------------------
     */


    /*
     * ---------------------------------------------------------------------
     * Face constants
     * ---------------------------------------------------------------------
     */

#ifdef __OPENCL_VERSION__
    
    CUBELIB_GLOBAL_STATIC_CONST face_t posxface = POSXFACE;
    CUBELIB_GLOBAL_STATIC_CONST face_t posyface = POSYFACE;
    CUBELIB_GLOBAL_STATIC_CONST face_t poszface = POSZFACE;
    CUBELIB_GLOBAL_STATIC_CONST face_t negxface = NEGXFACE;
    CUBELIB_GLOBAL_STATIC_CONST face_t negyface = NEGYFACE;
    CUBELIB_GLOBAL_STATIC_CONST face_t negzface = NEGZFACE;
     
#else

    CUBELIB_GLOBAL_STATIC_CONST face_t posxface = get_direction_by_int3(+1, 0, 0);
    CUBELIB_GLOBAL_STATIC_CONST face_t posyface = get_direction_by_int3( 0,+1, 0);
    CUBELIB_GLOBAL_STATIC_CONST face_t poszface = get_direction_by_int3( 0, 0,+1);
    CUBELIB_GLOBAL_STATIC_CONST face_t negxface = get_direction_by_int3(-1, 0, 0);
    CUBELIB_GLOBAL_STATIC_CONST face_t negyface = get_direction_by_int3( 0,-1, 0);
    CUBELIB_GLOBAL_STATIC_CONST face_t negzface = get_direction_by_int3( 0, 0,-1);
#endif
    /*
     * ---------------------------------------------------------------------
     * direction constants
     * ---------------------------------------------------------------------
     */

    CUBELIB_GLOBAL_STATIC_CONST direction_t all_directions[6] = { {1}, {2}, {3}, {4}, {5}, {6} };



#ifdef __cplusplus
}
#endif


#include "cubelib/detail/cubelib.inl.h"

#endif // CORNER_CASES_CUBELIB_CUBELIB_H

