


#ifdef __cplusplus
extern "C"{
#endif



    /*
     * ---------------------------------------------------------------------
     * Corners
     * ---------------------------------------------------------------------
     */
    static inline bool is_corner_valid(corner_t corner)
    {
        ///0b1000 represents a null corner
        ///0b0*** are all valid corners
        return (corner.value <= 0x8);
    }

    static inline bool is_corner_null(corner_t corner)
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

    static inline int get_corner_i(corner_t corner, uint_fast8_t dim)
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


    static inline int get_corner_uniti(corner_t corner, uint_fast8_t dim)
    {
        assert (dim < 3);

        return ( corner.value & (1 << dim) ? 1 : 0 );
    }


    static inline
    corner_t get_corner_by_float3(float x, float y, float z)
    {
        corner_value_t value = (x > 0 ? 1 : 0) | ((y > 0 ? 1 : 0) << 1) | ((z > 0 ? 1 : 0) << 2);

        corner_t result = {value};
        return result;
    }
    static inline
    corner_t get_corner_by_int3(int x, int y, int z)
    {
        corner_value_t value = (x > 0 ? 1 : 0) | ((y > 0 ? 1 : 0) << 1) | ((z > 0 ? 1 : 0) << 2);

        corner_t result;

        result.value = value;

        return result;
    }

    static inline
    corner_t get_corner_by_index(uint_fast8_t index)
    {
        assert (index < 8);
        corner_t result = {(corner_value_t)index};
        assert(is_corner_valid(result));
        assert(!is_corner_null(result));
        return result;
    }


    /**

     */
    static inline corner_t get_opposite_corner(corner_t corner)
    {
        if (is_corner_null(corner))
            return null_corner;

        corner_t result;
        result.value = (~corner.value) & 0x7;
        return result;
    }

    static inline corner_t corner_move(corner_t corner, direction_t direction)
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

    static inline corner_t corner_push(corner_t corner, direction_t direction)
    {

        int x = get_corner_x(corner) + get_direction_x(direction)*2;
        int y = get_corner_y(corner) + get_direction_y(direction)*2;
        int z = get_corner_z(corner) + get_direction_z(direction)*2;

        return get_corner_by_int3(x,y,z);
    }

    static inline uint_fast8_t get_corner_index(corner_t corner){
        assert (is_corner_valid(corner) && !is_corner_null(corner));
        return corner.value;
    }


    static inline corner_t calc_cnr_adj_cnr(corner_t corner, uint_fast8_t dim)
    {
        assert( dim < 3 );

        ///flip the bit of the specified dimension
        corner_t result = {(corner_value_t)(corner.value ^ (1 << dim))};

        return result;
    }


    static inline bool is_corner_equal(corner_t left, corner_t right)
    {
        return left.value == right.value;
    }


    /*
     * ---------------------------------------------------------------------
     * Directions
     * ---------------------------------------------------------------------
     */
    static inline bool is_direction_valid(direction_t direction){
        ///0b111 is an invalid direction; anything more than 0b111 is also an invalid direction.
        ///0b000 is a null direction.
        return (direction.value < 7);
    }

    static inline bool is_direction_null(direction_t direction)
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
    static inline int get_direction_i(direction_t direction, uint_fast8_t dim)
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

    static inline direction_t get_opposite_direction(direction_t direction)
    {
        assert( !is_direction_null(direction) );
        direction_value_t value = (~direction.value) & 7;
        direction_t result = {value};
        return result;
    }

    static inline direction_t get_opposite_face(direction_t direction)
    {
        return get_opposite_direction(direction);
    }

    static inline
    direction_t get_direction_by_int3(int x, int y, int z)
    {   
        bool pos = x > 0 || y > 0 || z > 0;
        bool neg = x < 0 || y < 0 || z < 0;

        assert(pos != neg && "direction must face a side/face, not a corner or edge of the cube");
        assert(abs(x+y+z) == 1);

        direction_value_t value = (x != 0 ? 1 : 0) | ((y != 0 ? 1 : 0) << 1) | ((z != 0 ? 1 : 0) << 2);
        value = pos ? value : ((~value) & 7);

        direction_t result = {value};
        return result;
    }


    static inline
    direction_t get_direction_by_index(uint_fast8_t index)
    {
        assert(index < 6);
        direction_t result = {(direction_value_t)(index + 1)};
        return result;
    }

    static inline
    uint_fast8_t get_direction_index(direction_t direction)
    {
        assert(is_direction_valid(direction) && !is_direction_null(direction));

        return direction.value - 1;
    }

    static inline
    corner_t calc_dir_adj_cnr(direction_t direction, uint_fast8_t idx)
    {
        assert(idx < 4);

        int x = get_direction_x(direction)
          , y = get_direction_y(direction)
          , z = get_direction_z(direction);


        uint_fast8_t idx0 = 0;
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
     * Edges
     * ---------------------------------------------------------------------
     */
    
    static inline bool is_edge_valid(edge_t edge)
    {
        return edge.value <= 12;
    }


    static inline bool is_edge_null(edge_t edge)
    {
        return edge.value == 12;
    }

    static inline bool is_edge_equal(edge_t left, edge_t right)
    {
        return left.value == right.value;
    }


    /*
     * ---------------------------------------------------------------------
     * Faces
     * ---------------------------------------------------------------------
     */
    static inline bool is_face_null(face_t face)
    {
        return is_direction_null(face);
    }







#ifdef __cplusplus
}
#endif






