#ifndef CUBELIB_H
#define CUBELIB_H 1

#ifdef __OPENCL_VERSION__

    #include "cubelib.clgen.h"

    #define GLOBAL_STATIC_CONST __constant const
    #define LOCAL_STATIC_CONST const
    
#else

    #include <assert.h>
    #include <stdint.h>

    #define CUBELIB_GLOBAL_STATIC_CONST static const
    #define CUBELIB_LOCAL_STATIC_CONST static const

#endif

#ifdef __cplusplus
extern "C"{
#endif

    typedef unsigned int word_t;
    typedef struct corner_t{
        word_t value;
    } corner_t;

    typedef struct direction_t{
        word_t value;
    } direction_t;

    typedef direction_t face_t;

    #define NULL_CORNER (corner_t){8}
    #define NULL_DIRECTION (direction_t){0}
    #define NULL_FACE (face_t){0}

    CUBELIB_GLOBAL_STATIC_CONST face_t null_face = (face_t){0};
    CUBELIB_GLOBAL_STATIC_CONST direction_t null_direction = (direction_t){0};
    CUBELIB_GLOBAL_STATIC_CONST corner_t null_corner = (corner_t){8};


    static inline bool is_valid_corner(corner_t corner);
    static inline bool is_null_corner(corner_t corner);
    //static inline bool is_a_corner(corner_t corner);
    static inline int get_corner_x(corner_t corner);
    static inline int get_corner_y(corner_t corner);
    static inline int get_corner_z(corner_t corner);
    static inline int get_corner_i(corner_t corner, uint32_t dim);
    static inline int get_corner_unitx(corner_t corner);
    static inline int get_corner_unity(corner_t corner);
    static inline int get_corner_unitz(corner_t corner);
    static inline int get_corner_uniti(corner_t corner, uint32_t dim);


    static inline
    corner_t get_corner_by_float3(float x, float y, float z);
    static inline
    corner_t get_corner_by_int3(int x, int y, int z);

    static inline
    corner_t get_corner_by_index(uint32_t index);

    static inline corner_t opposite_corner(corner_t corner);
    static inline corner_t move_corner(corner_t corner, direction_t direction);

    static inline corner_t bounded_move_corner(corner_t corner, direction_t direction);
    static inline uint32_t get_corner_index(corner_t corner);
    static inline corner_t calc_cnr_adj_cnr(corner_t corner, uint32_t dim);
    static inline bool is_corner_equal(corner_t left, corner_t right);

    static inline int get_direction_x(direction_t direction);
    static inline int get_direction_y(direction_t direction);
    static inline int get_direction_z(direction_t direction);
    static inline int get_direction_i(direction_t direction, uint32_t dim);

    static inline uint32_t get_direction_index(direction_t direction);
    static inline direction_t get_direction_by_index(uint32_t index);
    static inline direction_t get_direction_by_int3(int x, int y, int z);
    static inline direction_t opposite_direction(direction_t direction);
    static inline direction_t opposite_face(direction_t direction);

    static inline bool is_valid_direction(direction_t direction);
    static inline bool is_null_direction(direction_t direction);
    static inline bool is_direction_equal(direction_t left, direction_t right);

    
#ifdef __OPENCL_VERSION__
    CUBELIB_GLOBAL_STATIC_CONST corner_t cnr_adj_cnrs[8][3] = CNR_ADJ_CNRS;
#else

    CUBELIB_GLOBAL_STATIC_CONST corner_t cnr_adj_cnrs[8][3]
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
     * Directions
     * ---------------------------------------------------------------------
     */
    static inline bool is_valid_direction(direction_t direction){
        ///0b111 is an invalid direction; anything more than 0b111 is also an invalid direction.
        ///0b000 is a null direction.
        return (direction.value < 7);
    }

    static inline bool is_null_direction(direction_t direction)
    {
        return direction.value == 0;
    }

    static inline int get_direction_x(direction_t direction)
    {
        const char xs[] = { 0, 1, 0, 0, 0, 0, -1, 0};
        return direction.value < 8 ? xs[direction.value] : 0;
    }
    static inline int get_direction_y(direction_t direction)
    {
        const char ys[] = { 0, 0, 1, 0, 0, -1, 0, 0};
        return direction.value < 8 ? ys[direction.value] : 0;
    }
    static inline int get_direction_z(direction_t direction)
    {
        const char zs[] = { 0, 0, 0, -1, 1, 0, 0, 0};
        return direction.value < 8 ? zs[direction.value] : 0;
    }
    static inline int get_direction_i(direction_t direction, uint32_t dim)
    {
        assert(dim < 3);
        if (dim == 0)
            return get_direction_x(direction);
        if (dim == 1)
            return get_direction_y(direction);
        if (dim == 2)
            return get_direction_z(direction);
        return 0;
    }

    static inline bool is_direction_equal(direction_t left, direction_t right)
    {
        return left.value == right.value;
    }

    /*
    static inline int get_direction_unitx(direction_t direction)
    {
        static const char xs[] = { 0, 1, 0, 0, 0, 0, -0, 0};
        return direction.value < 8 ? xs[direction.value] : 0;
    }
    static inline int get_direction_unity(direction_t direction)
    {
        static const char ys[] = { 0, 0, 1, 0, 0, -0, 0, 0};
        return direction.value < 8 ? ys[direction.value] : 0;
    }
    static inline int get_direction_unitz(direction_t direction)
    {
        static const char zs[] = { 0, 0, 0, -0, 1, 0, 0, 0};
        return direction.value < 8 ? zs[direction.value] : 0;
    }
    */

    static inline direction_t opposite_direction(direction_t direction)
    {
        assert( !is_null_direction(direction) );
        word_t value = (~direction.value) & 7;
        direction_t result = {value};
        return result;
    }

    static inline direction_t opposite_face(direction_t direction)
    {
        return opposite_direction(direction);
    }

    static inline
    direction_t get_direction_by_int3(int x, int y, int z)
    {   
        bool pos = x > 0 || y > 0 || z > 0;
        bool neg = x < 0 || y < 0 || z < 0;

        assert(pos != neg);
        assert(abs(x+y+z) == 1);

        word_t value = (x != 0 ? 1 : 0) | ((y != 0 ? 1 : 0) << 1) | ((z != 0 ? 1 : 0) << 2);
        value = pos ? value : ((~value) & 7);

        direction_t result = {value};
        return result;
    }


    static inline
    direction_t get_direction_by_index(uint32_t index)
    {
        assert(index < 6);
        direction_t result = {index + 1};
        return result;
    }

    static inline
    uint32_t get_direction_index(direction_t direction)
    {
        assert(is_valid_direction(direction) && !is_null_direction(direction));

        return direction.value - 1;
    }

    static inline
    corner_t calc_dir_adj_cnr(direction_t direction, uint32_t idx)
    {
        assert(idx < 4);

        int x = get_direction_x(direction)
          , y = get_direction_y(direction)
          , z = get_direction_z(direction);


        uint32_t idx0 = 0;
        for (size_t i = 0; i < 8; ++i)
        {
            corner_t corner = all_corners[i];
            
            int cx = get_corner_x(corner)
              , cy = get_corner_y(corner)
              , cz = get_corner_z(corner);

            if (!(cx == x || cy == y || cz == z))
                continue;

            assert(cx == x || cy == y || cz == z);
            
            if (idx0 == idx)
                return corner;
            idx0++;
        }

        assert(false && "should be unreachable");
        return null_corner;
    }


#ifdef __OPENCL_VERSION__
    CUBELIB_GLOBAL_STATIC_CONST corner_t dir_adj_cnrs[6][4] = DIR_ADJ_CNRS;
#else

    CUBELIB_GLOBAL_STATIC_CONST corner_t dir_adj_cnrs[6][4] = {
        {     calc_dir_adj_cnr(get_direction_by_index(0), 0)
            , calc_dir_adj_cnr(get_direction_by_index(0), 1)
            , calc_dir_adj_cnr(get_direction_by_index(0), 2)
            , calc_dir_adj_cnr(get_direction_by_index(0), 3)}
        , {   calc_dir_adj_cnr(get_direction_by_index(1), 0)
            , calc_dir_adj_cnr(get_direction_by_index(1), 1)
            , calc_dir_adj_cnr(get_direction_by_index(1), 2)
            , calc_dir_adj_cnr(get_direction_by_index(1), 3)}
        , {   calc_dir_adj_cnr(get_direction_by_index(2), 0)
            , calc_dir_adj_cnr(get_direction_by_index(2), 1)
            , calc_dir_adj_cnr(get_direction_by_index(2), 2)
            , calc_dir_adj_cnr(get_direction_by_index(2), 3)}
        , {   calc_dir_adj_cnr(get_direction_by_index(3), 0)
            , calc_dir_adj_cnr(get_direction_by_index(3), 1)
            , calc_dir_adj_cnr(get_direction_by_index(3), 2)
            , calc_dir_adj_cnr(get_direction_by_index(3), 3)}
        , {   calc_dir_adj_cnr(get_direction_by_index(4), 0)
            , calc_dir_adj_cnr(get_direction_by_index(4), 1)
            , calc_dir_adj_cnr(get_direction_by_index(4), 2)
            , calc_dir_adj_cnr(get_direction_by_index(4), 3)}
        , {   calc_dir_adj_cnr(get_direction_by_index(5), 0)
            , calc_dir_adj_cnr(get_direction_by_index(5), 1)
            , calc_dir_adj_cnr(get_direction_by_index(5), 2)
            , calc_dir_adj_cnr(get_direction_by_index(5), 3)}
    };
    
#endif
    
    /*
     * ---------------------------------------------------------------------
     * Faces
     * ---------------------------------------------------------------------
     */
    static inline bool is_null_face(face_t face)
    {
        return is_null_direction(face);
    }



    /*
     * ---------------------------------------------------------------------
     * Corners
     * ---------------------------------------------------------------------
     */
    static inline bool is_valid_corner(corner_t corner)
    {
        ///0b1000 represents a null corner
        ///0b0*** are all valid corners
        return (corner.value <= 0x8);
    }

    static inline bool is_null_corner(corner_t corner)
    {
        ///0b1000 represents a null corner
        ///0b0*** are all valid corners
        //return valid_corner(corner) && (corner.value & 0x8);
        return corner.value == 0x8;
    }


    static inline int get_corner_x(corner_t corner)
    {
        return (corner.value & 1 ? 1 : -1);
    }
    static inline int get_corner_y(corner_t corner)
    {
        return (corner.value & 2 ? 1 : -1);
    }
    static inline int get_corner_z(corner_t corner)
    {
        return (corner.value & 4 ? 1 : -1);
    }

    static inline int get_corner_i(corner_t corner, uint32_t dim)
    {
        assert (dim < 3);

        return ( corner.value & (1 << dim) ? 1 : -1 );
    }




    static inline int get_corner_unitx(corner_t corner)
    {
        return (corner.value & 1 ? 1 : 0);
    }
    static inline int get_corner_unity(corner_t corner)
    {
        return (corner.value & 2 ? 1 : 0);
    }
    static inline int get_corner_unitz(corner_t corner)
    {
        return (corner.value & 4 ? 1 : 0);
    }


    static inline int get_corner_uniti(corner_t corner, uint32_t dim)
    {
        assert (dim < 3);

        return ( corner.value & (1 << dim) ? 1 : 0 );
    }


    static inline
    corner_t get_corner_by_float3(float x, float y, float z)
    {
        word_t value = (x > 0 ? 1 : 0) | ((y > 0 ? 1 : 0) << 1) | ((z > 0 ? 1 : 0) << 2);

        corner_t result = {value};
        return result;
    }
    static inline
    corner_t get_corner_by_int3(int x, int y, int z)
    {
        word_t value = (x > 0 ? 1 : 0) | ((y > 0 ? 1 : 0) << 1) | ((z > 0 ? 1 : 0) << 2);

        corner_t result;

        result.value = value;

        return result;
    }

    static inline
    corner_t get_corner_by_index(uint32_t index)
    {
        assert (index < 8);
        corner_t result = {(word_t)index};
        return result;
    }


    /**

     */
    static inline corner_t opposite_corner(corner_t corner)
    {
        if (is_null_corner(corner))
            return null_corner;

        corner_t result;
        result.value = (~corner.value) & 0x7;
        return result;
    }

    /**
     * Move a corner in a specified direction. If the new corner cannot be moved in that direction
     *  then this method will return a @c null_corner.
     */
    static inline corner_t move_corner(corner_t corner, direction_t direction)
    {

        int x = get_corner_x(corner) + get_direction_x(direction)*2;
        int y = get_corner_y(corner) + get_direction_y(direction)*2;
        int z = get_corner_z(corner) + get_direction_z(direction)*2;

        //std::cout << "corner: " << corner << ", direction: " << direction << std::endl;
        //std::cout << "x0,y0,z0: " << get_corner_x(corner) << ", " << get_corner_y(corner) << ", " << get_corner_z(corner) << std::endl;
        //std::cout << "x,y,z: " << x << ", " << y << ", " << z << std::endl;

        if (x > 1 || x < -1 || y > 1 || y < -1 || z > 1 || z < -1)
            return null_corner;

        return get_corner_by_int3(x,y,z);
    }

    static inline corner_t bounded_move_corner(corner_t corner, direction_t direction)
    {

        int x = get_corner_x(corner) + get_direction_x(direction);
        int y = get_corner_y(corner) + get_direction_y(direction);
        int z = get_corner_z(corner) + get_direction_z(direction);

        return get_corner_by_int3(x,y,z);
    }

    static inline uint32_t get_corner_index(corner_t corner){
        assert (is_valid_corner(corner) && !is_null_corner(corner));
        return corner.value;
    }


    static inline corner_t calc_cnr_adj_cnr(corner_t corner, uint32_t dim)
    {
        assert( dim < 3 );

        ///flip the bit of the specified dimension
        corner_t result = {corner.value ^ (1 << dim)};

        return result;
    }


    static inline bool is_corner_equal(corner_t left, corner_t right)
    {
        return left.value == right.value;
    }


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

#endif
