#ifndef CORNER_CASES_CUBELIB_CUBELIB_H
#define CORNER_CASES_CUBELIB_CUBELIB_H 1


#include "cubelib/cubelib.fwd.h"

#ifdef __OPENCL_VERSION__

    #include "cubelib.clgen.h"

    #define GLOBAL_STATIC_CONST __constant const
    #define LOCAL_STATIC_CONST const
    
    typedef uint8_t uint_fast8_t;
    
#else

    #include <assert.h>
    #include <stdint.h>
    #include <stdlib.h>

    #define CUBELIB_GLOBAL_STATIC_CONST static const
    #define CUBELIB_LOCAL_STATIC_CONST static const

#endif

#ifdef __cplusplus
extern "C"{
#endif

    typedef uint_fast8_t corner_value_t;
    typedef uint_fast8_t direction_value_t;
    typedef uint_fast8_t edge_value_t;
    
    typedef struct corner_t{
        corner_value_t value;
    } corner_t;

    typedef struct direction_t{
        direction_value_t value;
    } direction_t;

    
    typedef struct edge_t{
        edge_value_t value;
    } edge_t;

    #define NULL_CORNER (corner_t){8}
    #define NULL_DIRECTION (direction_t){0}
    #define NULL_EGDE (edge_t){12}
    #define NULL_FACE (face_t){0}
    
    #define CORNERS_SIZE 8
    #define DIRECTIONS_SIZE 6
    #define EDGES_SIZE 12
    #define FACES_SIZE 6

    CUBELIB_GLOBAL_STATIC_CONST corner_t null_corner = NULL_CORNER;
    CUBELIB_GLOBAL_STATIC_CONST direction_t null_direction = NULL_DIRECTION;
    CUBELIB_GLOBAL_STATIC_CONST edge_t null_edge = NULL_EGDE;
    CUBELIB_GLOBAL_STATIC_CONST face_t null_face = NULL_FACE;
    
    CUBELIB_GLOBAL_STATIC_CONST size_t corners_size = CORNERS_SIZE;
    CUBELIB_GLOBAL_STATIC_CONST size_t directions_size = DIRECTIONS_SIZE;
    CUBELIB_GLOBAL_STATIC_CONST size_t edges_size = EDGES_SIZE;
    CUBELIB_GLOBAL_STATIC_CONST size_t faces_size = FACES_SIZE;
    

/* -------------------------------------------------------------------------- */
    static inline bool is_corner_valid(corner_t corner);
    static inline bool is_corner_null(corner_t corner);

    static inline int get_corner_x(corner_t corner);
    static inline int get_corner_y(corner_t corner);
    static inline int get_corner_z(corner_t corner);
    static inline int get_corner_i(corner_t corner, uint_fast8_t dim);
    static inline int get_corner_unitx(corner_t corner);
    static inline int get_corner_unity(corner_t corner);
    static inline int get_corner_unitz(corner_t corner);
    /**
     * @brief compute the ith component of the corner's coordinates in unit-space
     * @see get_corner_unitx(), get_corner_unity(), get_corner_unitz()
     */
    static inline int get_corner_uniti(corner_t corner, uint_fast8_t dim);


    static inline
    corner_t get_corner_by_float3(float x, float y, float z);
    static inline
    corner_t get_corner_by_int3(int x, int y, int z);

    static inline
    corner_t get_corner_by_index(uint_fast8_t index);

    /**
     * Return
     *  then this method will wrap around and return the adjacent corner in the specified direction.
     * @see corner_push(), corner_move()
     */
    static inline corner_t get_opposite_corner(corner_t corner);
    
    /**
     * Move a corner in a specified direction. If the new corner cannot be moved in that direction
     *  then this method will wrap around and return the adjacent corner in the specified direction.
     * @see corner_push(), corner_move()
     */
    static inline corner_t get_adjacent_corner(corner_t corner, direction_t direction);
    /**
     * Move a corner in a specified direction. If the new corner cannot be moved in that direction
     *  then this method will return a @c null_corner.
     * @see corner_push(), get_adjacent_corner()
     */
    static inline corner_t corner_move(corner_t corner, direction_t direction);
    /**
     * Move a corner in a specified direction. If the new corner cannot be moved in that direction
     *  then this method will return the same corner.
     * @see corner_move(), get_adjacent_corner()
     */
    static inline corner_t corner_push(corner_t corner, direction_t direction);
    
    
    static inline uint_fast8_t get_corner_index(corner_t corner);
    static inline corner_t calc_cnr_adj_cnr(corner_t corner, uint_fast8_t dim);
    static inline bool is_corner_equal(corner_t left, corner_t right);





/* -------------------------------------------------------------------------- */
    static inline int get_direction_x(direction_t direction);
    static inline int get_direction_y(direction_t direction);
    static inline int get_direction_z(direction_t direction);
    static inline int get_direction_i(direction_t direction, uint_fast8_t dim);

    static inline uint_fast8_t get_direction_index(direction_t direction);
    static inline direction_t get_direction_by_index(uint_fast8_t index);
    static inline direction_t get_direction_by_int3(int x, int y, int z);
    static inline direction_t get_opposite_direction(direction_t direction);
    static inline direction_t get_opposite_face(direction_t direction);

    static inline bool is_corner_adjacent_direction(corner_t corner, direction_t direction);
    
    static inline bool is_direction_valid(direction_t direction);
    static inline bool is_direction_null(direction_t direction);
    static inline bool is_direction_equal(direction_t left, direction_t right);
/* -------------------------------------------------------------------------- */

    static inline edge_t get_edge_by_corner_direction(corner_t corner, direction_t direction);
    static inline edge_t get_edge_by_corners(corner_t corner0, corner_t corner1);
    static inline edge_t get_edge_by_index(uint_fast8_t index);
    static inline uint_fast8_t get_edge_index(edge_t edge);
    static inline edge_t get_opposite_edge(edge_t edge);
    
    
    static inline bool is_corner_adjacent_edge(corner_t corner, edge_t edge);
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
    CUBELIB_GLOBAL_STATIC_CONST corner_t cnr_adj_cnrs[8][3] = CNR_ADJ_CNRS;
#else

    CUBELIB_GLOBAL_STATIC_CONST corner_t cnr_adj_cnrs[8][3] =
        {
            {     calc_cnr_adj_cnr(get_corner_by_index(0), 0)
                , calc_cnr_adj_cnr(get_corner_by_index(0), 1)
                , calc_cnr_adj_cnr(get_corner_by_index(0), 2)}
            , {   calc_cnr_adj_cnr(get_corner_by_index(1), 0)
                , calc_cnr_adj_cnr(get_corner_by_index(1), 1)
                , calc_cnr_adj_cnr(get_corner_by_index(1), 2)}
            , {   calc_cnr_adj_cnr(get_corner_by_index(2), 0)
                , calc_cnr_adj_cnr(get_corner_by_index(2), 1)
                , calc_cnr_adj_cnr(get_corner_by_index(2), 2)}
            , {   calc_cnr_adj_cnr(get_corner_by_index(3), 0)
                , calc_cnr_adj_cnr(get_corner_by_index(3), 1)
                , calc_cnr_adj_cnr(get_corner_by_index(3), 2)}
            , {   calc_cnr_adj_cnr(get_corner_by_index(4), 0)
                , calc_cnr_adj_cnr(get_corner_by_index(4), 1)
                , calc_cnr_adj_cnr(get_corner_by_index(4), 2)}
            , {   calc_cnr_adj_cnr(get_corner_by_index(5), 0)
                , calc_cnr_adj_cnr(get_corner_by_index(5), 1)
                , calc_cnr_adj_cnr(get_corner_by_index(5), 2)}
            , {   calc_cnr_adj_cnr(get_corner_by_index(6), 0)
                , calc_cnr_adj_cnr(get_corner_by_index(6), 1)
                , calc_cnr_adj_cnr(get_corner_by_index(6), 2)}
            , {   calc_cnr_adj_cnr(get_corner_by_index(7), 0)
                , calc_cnr_adj_cnr(get_corner_by_index(7), 1)
                , calc_cnr_adj_cnr(get_corner_by_index(7), 2)}

        };
    
#endif
    
    CUBELIB_GLOBAL_STATIC_CONST corner_t all_corners[8] = { {0}, {1}
                                            , {2}, {3}
                                            , {4}, {5}
                                            , {6}, {7}
                                            };


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

