#ifndef CORNER_CASES_CUBELIB_CUBELIB_INL_H
#define CORNER_CASES_CUBELIB_CUBELIB_INL_H

#ifdef __cplusplus
extern "C"{
#endif



    /*
     * ---------------------------------------------------------------------
     * Corners
     * ---------------------------------------------------------------------
     */
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_valid(cubelib_corner_t corner)
    {
        ///0b1000 represents a null corner
        ///0b0*** are all valid corners
        return (corner.value <= 0x8);
    }

    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_null(cubelib_corner_t corner)
    {
        ///0b1000 represents a null corner
        ///0b0*** are all valid corners
        //return valid_corner(corner) && (corner.value & 0x8);
        return corner.value == 0x8;
    }


    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_x(cubelib_corner_t corner)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        
        return (corner.value & 1 ? 1 : -1);
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_y(cubelib_corner_t corner)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        
        return (corner.value & 2 ? 1 : -1);
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_z(cubelib_corner_t corner)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        
        return (corner.value & 4 ? 1 : -1);
    }

    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_i(cubelib_corner_t corner, uint_fast8_t dim)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        assert (dim < 3);

        return ( corner.value & (1 << dim) ? 1 : -1 );
    }




    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_ux(cubelib_corner_t corner)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        
        return (corner.value & 1 ? 1 : 0);
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_uy(cubelib_corner_t corner)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        
        return (corner.value & 2 ? 1 : 0);
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_uz(cubelib_corner_t corner)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        
        return (corner.value & 4 ? 1 : 0);
    }


    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_corner_ui(cubelib_corner_t corner, uint_fast8_t dim)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        
        assert (dim < 3);

        return ( corner.value & (1 << dim) ? 1 : 0 );
    }


    CUBELIB_FUNCTION_QUALIFIER
    cubelib_corner_t cubelib_get_corner_by_float3(float x, float y, float z)
    {
        cubelib_corner_value_t value = (x > 0 ? 1 : 0) | ((y > 0 ? 1 : 0) << 1) | ((z > 0 ? 1 : 0) << 2);

        cubelib_corner_t result = {value};
        
        
        assert(cubelib_is_corner_valid(result));
        assert(!cubelib_is_corner_null(result));
        
        return result;
    }
    CUBELIB_FUNCTION_QUALIFIER
    cubelib_corner_t cubelib_get_corner_by_int3(int x, int y, int z)
    {
        cubelib_corner_value_t value = (x > 0 ? 1 : 0) | ((y > 0 ? 1 : 0) << 1) | ((z > 0 ? 1 : 0) << 2);

        cubelib_corner_t result;

        result.value = value;

        assert(cubelib_is_corner_valid(result));
        assert(!cubelib_is_corner_null(result));
        return result;
    }

    CUBELIB_FUNCTION_QUALIFIER
    cubelib_corner_t cubelib_get_corner_by_index(uint_fast8_t index)
    {
        assert (index < 8);
        cubelib_corner_t result = {(cubelib_corner_value_t)index};
        assert(cubelib_is_corner_valid(result));
        assert(!cubelib_is_corner_null(result));
        return result;
    }


    /**

     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_opposite_corner(cubelib_corner_t corner)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));

        cubelib_corner_t result;
        result.value = (~corner.value) & 0x7;
        return result;
    }

    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_corner_move(cubelib_corner_t corner, cubelib_direction_t direction)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        int x = cubelib_get_corner_x(corner) + cubelib_get_direction_x(direction)*2;
        int y = cubelib_get_corner_y(corner) + cubelib_get_direction_y(direction)*2;
        int z = cubelib_get_corner_z(corner) + cubelib_get_direction_z(direction)*2;

        //std::cout << "corner: " << corner << ", direction: " << direction << std::endl;
        //std::cout << "x0,y0,z0: " << cubelib_get_corner_x(corner) << ", " << cubelib_get_corner_y(corner) << ", " << cubelib_get_corner_z(corner) << std::endl;
        //std::cout << "x,y,z: " << x << ", " << y << ", " << z << std::endl;

        if (x > 1 || x < -1 || y > 1 || y < -1 || z > 1 || z < -1)
            return cubelib_null_corner;

        return cubelib_get_corner_by_int3(x,y,z);
    }

    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_corner_push(cubelib_corner_t corner, cubelib_direction_t direction)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        
        int x = cubelib_get_corner_x(corner) + cubelib_get_direction_x(direction)*2;
        int y = cubelib_get_corner_y(corner) + cubelib_get_direction_y(direction)*2;
        int z = cubelib_get_corner_z(corner) + cubelib_get_direction_z(direction)*2;

        return cubelib_get_corner_by_int3(x,y,z);
    }
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_calc_adjacent_corner(cubelib_corner_t corner, cubelib_direction_t direction)
    {
        assert(!cubelib_is_corner_null(corner));
        assert(cubelib_is_corner_valid(corner));
        assert(cubelib_is_direction_valid(direction));
        
        
        uint_fast8_t x = (cubelib_get_corner_ux(corner) + cubelib_get_direction_absx(direction)) % 2;
        uint_fast8_t y = (cubelib_get_corner_uy(corner) + cubelib_get_direction_absy(direction)) % 2;
        uint_fast8_t z = (cubelib_get_corner_uz(corner) + cubelib_get_direction_absz(direction)) % 2;

        return cubelib_get_corner_by_int3(x,y,z);
        
    }
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_adjacent_corner(cubelib_corner_t corner, cubelib_direction_t direction){
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        return cubelib_calc_adjacent_corner(corner, direction);
    }

    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_corner_index(cubelib_corner_t corner){
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        return corner.value;
    }

    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_calc_cnr_adj_cnr(cubelib_corner_t corner, uint_fast8_t dim)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        assert( dim < 3 );

        ///flip the bit of the specified dimension
        cubelib_corner_t result = {(cubelib_corner_value_t)(corner.value ^ (1 << dim))};

        assert(cubelib_is_corner_valid(result));
        assert(!cubelib_is_corner_null(result));
        return result;
    }
    
    
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_calc_is_corner_adjacent_corner(cubelib_corner_t corner0, cubelib_corner_t corner1)
    {
        assert(cubelib_is_corner_valid(corner0));
        assert(cubelib_is_corner_valid(corner1));
        assert(!cubelib_is_corner_null(corner0));
        assert(!cubelib_is_corner_null(corner1));
        
        uint_fast8_t different_axis_bits = (corner0.value ^ corner1.value);
        
        return different_axis_bits == 1 || different_axis_bits == 2 || different_axis_bits == 4;
    }


    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_adjacent_corner(cubelib_corner_t corner0, cubelib_corner_t corner1)
    {
        assert(cubelib_is_corner_valid(corner0));
        assert(cubelib_is_corner_valid(corner1));
        assert(!cubelib_is_corner_null(corner0));
        assert(!cubelib_is_corner_null(corner1));
        return cubelib_calc_is_corner_adjacent_corner(corner0, corner1);
    }

    

    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_equal(cubelib_corner_t left, cubelib_corner_t right)
    {
        assert(cubelib_is_corner_valid(left));
        assert(cubelib_is_corner_valid(right));
        return left.value == right.value;
    }

    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_corner_on_face(cubelib_corner_t corner, cubelib_face_t face)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        cubelib_direction_t direction = cubelib_get_face_direction(face);
        uint_fast8_t sigdim = cubelib_get_direction_sigdim(direction);
        
        return cubelib_get_corner_i(corner, sigdim) == cubelib_get_direction_i(direction,sigdim);
    }


    /*
     * ---------------------------------------------------------------------
     * Directions
     * ---------------------------------------------------------------------
     */
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_direction_valid(cubelib_direction_t direction){
        ///0b111 is an invalid direction; anything more than 0b111 is also an invalid direction.
        ///0b000 is a null direction.
        return (direction.value < 7);
    }

    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_direction_null(cubelib_direction_t direction)
    {
        return direction.value == 0;
    }

    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_x(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        ////direction indices:
        ///[null, (+1,0,0), (0,+1,0), (0,0,+1), (0,0,-1), (0,-1,0), (-1,0,0)]
        static const int_fast8_t xs[] = { 0, 1, 0, 0, 0, 0, (int_fast8_t)-1, 0};
        return xs[direction.value];
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_y(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        ////direction indices:
        ///[null, (+1,0,0), (0,+1,0), (0,0,+1), (0,0,-1), (0,-1,0), (-1,0,0)]
        static const int_fast8_t ys[] = { 0, 0, 1, 0, 0, (int_fast8_t)-1, 0, 0};
        return ys[direction.value];
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_z(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        ////direction indices:
        ///[null, (+1,0,0), (0,+1,0), (0,0,+1), (0,0,-1), (0,-1,0), (-1,0,0)]
        static const int_fast8_t zs[] = { 0, 0, 0, (int_fast8_t)-1, 1, 0, 0, 0};
        return zs[direction.value];
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_i(cubelib_direction_t direction, uint_fast8_t dim)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        assert(dim < 3);
        
        ///TODO: move this to a table
        if (dim == 0)
            return cubelib_get_direction_x(direction);
        if (dim == 1)
            return cubelib_get_direction_y(direction);
        if (dim == 2)
            return cubelib_get_direction_z(direction);
        return 0;
    }

    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_absx(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        ////direction indices:
        ///[null, (+1,0,0), (0,+1,0), (0,0,+1), (0,0,-1), (0,-1,0), (-1,0,0)]
        const char xs[] = { 0, 1, 0, 0, 0, 0, +1, 0};
        return xs[direction.value];
    }
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_absy(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        ////direction indices:
        ///[null, (+1,0,0), (0,+1,0), (0,0,+1), (0,0,-1), (0,-1,0), (-1,0,0)]
        const char ys[] = { 0, 0, 1, 0, 0, +1, 0, 0};
        return ys[direction.value];
    }
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_absz(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        ////direction indices:
        ///[null, (+1,0,0), (0,+1,0), (0,0,+1), (0,0,-1), (0,-1,0), (-1,0,0)]
        const char zs[] = { 0, 0, 0, +1, 1, 0, 0, 0};
        return zs[direction.value];
    }
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_absi(cubelib_direction_t direction, uint_fast8_t dim)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        assert(dim < 3);
        
        if (dim == 0)
            return cubelib_get_direction_absx(direction);
        if (dim == 1)
            return cubelib_get_direction_absy(direction);
        if (dim == 2)
            return cubelib_get_direction_absz(direction);
        return 0;
    }
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_direction_equal(cubelib_direction_t left, cubelib_direction_t right)
    {
        assert(cubelib_is_direction_valid(left));
        assert(cubelib_is_direction_valid(right));
        
        return left.value == right.value;
    }

    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_unitx(cubelib_direction_t direction)
    {
        static const char xs[] = { 0, 1, 0, 0, 0, 0, -0, 0};
        return direction.value < 8 ? xs[direction.value] : 0;
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_unity(cubelib_direction_t direction)
    {
        static const char ys[] = { 0, 0, 1, 0, 0, -0, 0, 0};
        return direction.value < 8 ? ys[direction.value] : 0;
    }
    CUBELIB_FUNCTION_QUALIFIER int cubelib_get_direction_unitz(cubelib_direction_t direction)
    {
        static const char zs[] = { 0, 0, 0, -0, 1, 0, 0, 0};
        return direction.value < 8 ? zs[direction.value] : 0;
    }
    

    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t cubelib_get_opposite_direction(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        cubelib_direction_value_t value = (~direction.value) & 7;
        cubelib_direction_t result = {value};
        assert(cubelib_is_direction_valid(result));
        assert(!cubelib_is_direction_null(result));
        return result;
    }

    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t get_opposite_face(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        return cubelib_get_opposite_direction(direction);
    }

    CUBELIB_FUNCTION_QUALIFIER
    cubelib_direction_t cubelib_get_direction_by_int3(int x, int y, int z)
    {   
        bool pos = x > 0 || y > 0 || z > 0;
        bool neg = x < 0 || y < 0 || z < 0;

        assert(pos != neg && "direction must face a side/face, not a corner or edge of the cube");
        assert(abs(x+y+z) == 1);

        cubelib_direction_value_t value = (x != 0 ? 1 : 0) | ((y != 0 ? 1 : 0) << 1) | ((z != 0 ? 1 : 0) << 2);
        value = pos ? value : ((~value) & 7);

        cubelib_direction_t result = {value};
        assert(cubelib_is_direction_valid(result));
        assert(!cubelib_is_direction_null(result));
        return result;
    }


    CUBELIB_FUNCTION_QUALIFIER
    cubelib_direction_t cubelib_get_direction_by_index(uint_fast8_t index)
    {
        assert(index < 6);
        cubelib_direction_t result = {(cubelib_direction_value_t)(index + 1)};
        assert(cubelib_is_direction_valid(result));
        assert(!cubelib_is_direction_null(result));
        return result;
    }

    CUBELIB_FUNCTION_QUALIFIER
    uint_fast8_t cubelib_get_direction_index(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));

        return direction.value - 1;
    }

    
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_direction_sigdim(cubelib_direction_t direction)
    {
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        
        ////direction indices:
        ///[null, (+1,0,0), (0,+1,0), (0,0,+1), (0,0,-1), (0,-1,0), (-1,0,0)]
        static const uint_fast8_t sigdims[] = { (uint_fast8_t)-1/*null => invalid*/
                                              , 0/*(+1,0,0) => x*/, 1/*(0,+1,0) => y*/, 2/*(0,0,+1) => z*/
                                              , 2/*(0,0,-1) => z*/, 1/*(0,-1,0) => y*/, 0/*(-1,0,0) => x*/};
        return sigdims[direction.value];
    }

    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_get_direction_face(cubelib_direction_t direction)
    {
        return (cubelib_face_t){direction.value};
    }

    /*
     * ---------------------------------------------------------------------
     * Edges
     * ---------------------------------------------------------------------
     */
    
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_valid(cubelib_edge_t edge)
    {
        return edge.value <= 12;
    }


    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_null(cubelib_edge_t edge)
    {
        return edge.value == 12;
    }

    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_equal(cubelib_edge_t left, cubelib_edge_t right)
    {
        assert(cubelib_is_edge_valid(left));
        assert(cubelib_is_edge_valid(right));
        return left.value == right.value;
    }
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_by_axis(uint_fast8_t base_axis, bool project_secondary, bool project_tertiary)
    {
        assert(base_axis < 3);
        
        ///the value will be in the form of 0bBBTS; where BB is the base axis bits, and S,T are the "is projected" bits
        /// for the Secondary and Tertiary axes.
        cubelib_edge_value_t result_value = (base_axis << 2) | (project_tertiary ? 0x2 : 0x0) | (project_secondary ? 0x1 : 0);
        cubelib_edge_t result = {result_value};
        
        assert(cubelib_is_edge_valid(result));
        assert(!cubelib_is_edge_null(result));
        assert(cubelib_get_edge_base_axis(result) == base_axis);
        assert(cubelib_is_edge_projected_secondary(result) == project_secondary);
        assert(cubelib_is_edge_projected_tertiary(result) == project_tertiary);
        
        return result;
    }

    CUBELIB_FUNCTION_QUALIFIER
    uint_fast8_t cubelib_get_edge_index(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));

        return edge.value;
    }

    CUBELIB_FUNCTION_QUALIFIER
    cubelib_edge_t cubelib_get_edge_by_index(uint_fast8_t index)
    {
        assert(index < 12);

        cubelib_edge_t result = {(cubelib_edge_value_t)index};
        assert(cubelib_is_edge_valid(result));
        assert(!cubelib_is_edge_null(result));
        return result;
    }
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_calc_edge_by_corner_direction(cubelib_corner_t corner, cubelib_direction_t direction)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        cubelib_corner_t next_corner = cubelib_corner_move(corner, direction);
        assert(!cubelib_is_corner_null(next_corner) && "there is no adjacent corner in that direction");
        
        assert(cubelib_is_corner_adjacent_corner(corner,next_corner));
        
        cubelib_edge_t result = cubelib_get_edge_by_corners(corner,next_corner);
        
        assert(cubelib_is_edge_valid(result));
        assert(!cubelib_is_edge_null(result));
        
        ///corner == corner0(edge) || corner == corner1(edge)
        assert(cubelib_is_corner_equal(cubelib_get_edge_corner0(result), corner) || cubelib_is_corner_equal(cubelib_get_edge_corner1(result), corner));
        
        ///next_corner == corner0(edge) || next_corner == corner1(edge)
        assert(cubelib_is_corner_equal(cubelib_get_edge_corner0(result), next_corner) || cubelib_is_corner_equal(cubelib_get_edge_corner1(result), next_corner));
        
        return result;
    }
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_by_corner_direction(cubelib_corner_t corner, cubelib_direction_t direction)
    {
        assert(cubelib_is_corner_valid(corner));
        assert(!cubelib_is_corner_null(corner));
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        return cubelib_calc_edge_by_corner_direction(corner,direction);
    }
    
    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_opposite_edge(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        
        
        ///the value will be in the form of 0bBBTS; where BB is the base axis bits, and S,T are the "is projected" bits
        /// for the Secondary and Tertiary axes.

        
        cubelib_edge_value_t base_axis_bits = edge.value >> 2;
        cubelib_edge_value_t projected_secondary = (edge.value >> 0) & 1;
        cubelib_edge_value_t projected_tertiary = (edge.value >> 1) & 1;
        
        projected_secondary = (~projected_secondary) & 1;
        projected_tertiary = (~projected_tertiary) & 1;
        
        
        cubelib_edge_value_t result_value = (base_axis_bits << 2) | (projected_tertiary << 1) | projected_secondary;
        
        cubelib_edge_t result = {result_value};
        assert(cubelib_is_edge_valid(result));
        assert(!cubelib_is_edge_null(result));
        return result;
    }
    
    
    
    
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_calc_edge_base_axis(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        uint_fast8_t result = edge.value >> 2;
        assert(result < 3);
        return result;
    }
    
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_calc_edge_secondary_axis(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        return (cubelib_calc_edge_base_axis(edge) + 1) % 3;
    }
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_calc_edge_tertiary_axis(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        return (cubelib_calc_edge_base_axis(edge) + 2) % 3;
    }
    
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_edge_base_axis(cubelib_edge_t edge) {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        return cubelib_calc_edge_base_axis(edge);
    }
    
    
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_edge_secondary_axis(cubelib_edge_t edge) {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        return cubelib_calc_edge_secondary_axis(edge);
    }
    
    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_edge_tertiary_axis(cubelib_edge_t edge) {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        return cubelib_calc_edge_tertiary_axis(edge);
    }
    
    
    
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_projected_secondary(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        return edge.value & 1;
    }
    
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_projected_tertiary(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));

        return (edge.value >> 1) & 1;
    }
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t calc_edge_corner0(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        
        int xyz[] = {-1, -1, -1};
        
        xyz[cubelib_get_edge_secondary_axis(edge)] += cubelib_is_edge_projected_secondary(edge) ? 2 : 0;
        xyz[cubelib_get_edge_tertiary_axis(edge)] += cubelib_is_edge_projected_tertiary(edge) ? 2 : 0;
        return cubelib_get_corner_by_int3( xyz[0], xyz[1], xyz[2] );
    }
    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t calc_edge_corner1(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        
        int xyz[] = {-1, -1, -1};
        
        xyz[cubelib_get_edge_secondary_axis(edge)] += cubelib_is_edge_projected_secondary(edge) ? 2 : 0;
        xyz[cubelib_get_edge_tertiary_axis(edge)] += cubelib_is_edge_projected_tertiary(edge) ? 2 : 0;
        
        xyz[cubelib_get_edge_base_axis(edge)] += 2;
        return cubelib_get_corner_by_int3( xyz[0], xyz[1], xyz[2] );
    }
    
    
    
    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_edge_corner0(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        
        return calc_edge_corner0(edge);
    }
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_corner_t cubelib_get_edge_corner1(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        
        return calc_edge_corner1(edge);
    }
    
    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t calc_edge_by_corners(cubelib_corner_t corner0, cubelib_corner_t corner1)
    {
        assert(cubelib_is_corner_valid(corner0));
        assert(cubelib_is_corner_valid(corner1));
        assert(!cubelib_is_corner_null(corner0));
        assert(!cubelib_is_corner_null(corner1));
        assert(!cubelib_is_corner_equal(corner0, corner1));
        assert(cubelib_is_corner_adjacent_corner(corner0, corner1));
        
        ///the value will be in the form of 0bBBTS; where BB is the base axis bits, and S,T are the "is projected" bits
        /// for the Secondary and Tertiary axes.
        cubelib_edge_value_t value = 0;
        
        ///the bit that is different will be the base axis
        cubelib_corner_value_t axis_bit = corner0.value ^ corner1.value;
        assert(axis_bit == 1 || axis_bit == 2 || axis_bit == 4);
        
        ///the bits that are the same will be the projected axis
        cubelib_corner_value_t projection_bits = corner0.value & corner1.value;
        
        
        
        ///table to convert a 3 bit word with one bit set into an index of the position
        ///in other words, given an "axis-bit" of 0b001, 0b010 or 0b100, representing x,y,z axes
        /// respectively, this table will turn them into 0-based indices of 0,1,2 respectively.
        static const uint_fast8_t axis_bit_2_axis[] = { /*invalid*/(uint_fast8_t)-1,/*0b001*/ 0, /*0b010*/1, /*invalid*/(uint_fast8_t)-1, /*0b100*/2 };
        
        uint_fast8_t base_axis = axis_bit_2_axis[axis_bit];
        assert(base_axis < 3);
        
        
        value |= base_axis << 2;
        
        uint_fast8_t secondary_axis = (base_axis + 1) % 3;
        uint_fast8_t tertiary_axis = (base_axis + 2) % 3;
        uint_fast8_t is_projected_secondary_bit = (projection_bits >> secondary_axis) & 1;
        uint_fast8_t is_projected_tertiary_bit = (projection_bits >> tertiary_axis) & 1;
        
        value |= is_projected_secondary_bit;
        value |= is_projected_tertiary_bit << 1;
        
        cubelib_edge_t result = {value};
        
        assert(cubelib_is_edge_valid(result));
        assert(!cubelib_is_edge_null(result));
        assert(cubelib_get_edge_base_axis(result) == base_axis);
        assert(cubelib_get_edge_secondary_axis(result) == secondary_axis);
        assert(cubelib_get_edge_tertiary_axis(result) == tertiary_axis);
        assert(cubelib_is_edge_projected_secondary(result) == is_projected_secondary_bit);
        assert(cubelib_is_edge_projected_tertiary(result) == is_projected_tertiary_bit);
        //assert(cubelib_is_corner_adjacent_edge(corner0, result));
        //assert(cubelib_is_corner_adjacent_edge(corner1, result));
        assert(cubelib_is_corner_equal(cubelib_get_edge_corner0(result), corner0) || cubelib_is_corner_equal(cubelib_get_edge_corner0(result), corner1));
        assert(cubelib_is_corner_equal(cubelib_get_edge_corner1(result), corner0) || cubelib_is_corner_equal(cubelib_get_edge_corner1(result), corner1));
        return result;
    }
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_by_corners(cubelib_corner_t corner0, cubelib_corner_t corner1)
    {
        assert(cubelib_is_corner_valid(corner0));
        assert(cubelib_is_corner_valid(corner1));
        assert(!cubelib_is_corner_null(corner0));
        assert(!cubelib_is_corner_null(corner1));
        assert(cubelib_is_corner_adjacent_corner(corner0, corner1));

        return calc_edge_by_corners(corner0, corner1);
    }
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_edge_push(cubelib_edge_t edge, cubelib_direction_t direction)
    {
        return cubelib_calc_edge_push(edge, direction);
    }
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_calc_edge_push(cubelib_edge_t edge, cubelib_direction_t direction)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        assert(cubelib_is_direction_valid(direction));
        assert(!cubelib_is_direction_null(direction));
        
        
        cubelib_corner_t corner0 = cubelib_get_edge_corner0(edge);
        cubelib_direction_t edge_dir = cubelib_get_edge_direction(edge);
        
        ///move the base of the edge
        corner0 = cubelib_corner_push(corner0, direction);
        
        ///now recalculate the other corner of the edge
        cubelib_corner_t corner1 = cubelib_get_adjacent_corner(corner0, edge_dir);
        
        ///recreate the edge
        cubelib_edge_t result = cubelib_get_edge_by_corners(corner0, corner1);
        
        assert(cubelib_get_edge_base_axis(edge) == cubelib_get_edge_base_axis(result));
         
        
        return result;
      
    }
    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t cubelib_get_edge_direction(cubelib_edge_t edge)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        
        uint_fast8_t base_axis = cubelib_get_edge_base_axis(edge);
        
        int xyz[] = {0,0,0};
        xyz[base_axis] = +1;
        
        cubelib_direction_t result = cubelib_get_direction_by_int3(xyz[0],xyz[1],xyz[2]);
        
        assert(cubelib_is_direction_valid(result));
        assert(!cubelib_is_direction_null(result));
        return result;
    }
    
    
    
    /*
     * ---------------------------------------------------------------------
     * Faces
     * ---------------------------------------------------------------------
     */
    CUBELIB_FUNCTION_QUALIFIER cubelib_direction_t cubelib_get_face_direction(cubelib_face_t face)
    {
        cubelib_direction_t direction = {face.value};
        return direction;
    }

    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_face_valid(cubelib_face_t face)
    {
        return cubelib_is_direction_valid(cubelib_get_face_direction(face));
    }

    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_face_null(cubelib_face_t face)
    {
        return cubelib_is_direction_null(cubelib_get_face_direction(face));
    }

    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_face_equal(cubelib_face_t left, cubelib_face_t right)
    {
        return cubelib_is_direction_equal(cubelib_get_face_direction(left), cubelib_get_face_direction(right));
    }

    CUBELIB_FUNCTION_QUALIFIER uint_fast8_t cubelib_get_face_index(cubelib_face_t face)
    {
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        return cubelib_get_direction_index(cubelib_get_face_direction(face));
    }

    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_get_face_by_index(uint_fast8_t index)
    {
        assert(index < 6);
        return cubelib_get_direction_face(cubelib_get_direction_by_index(index));
    }



    
    
    
    CUBELIB_FUNCTION_QUALIFIER
    cubelib_corner_t cubelib_calc_corner_on_face(cubelib_face_t face, uint_fast8_t idx)
    {
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        assert(idx < 4);
        
        cubelib_direction_t direction = cubelib_get_face_direction(face);

        int x = cubelib_get_direction_x(direction)
          , y = cubelib_get_direction_y(direction)
          , z = cubelib_get_direction_z(direction);


        uint_fast8_t idx0 = 0;
        for (size_t i = 0; i < 8; ++i)
        {
            cubelib_corner_t corner = cubelib_all_corners[i];
            
            int cx = cubelib_get_corner_x(corner)
              , cy = cubelib_get_corner_y(corner)
              , cz = cubelib_get_corner_z(corner);

            if (!(cx == x || cy == y || cz == z))
                continue;

            assert(cx == x || cy == y || cz == z);
            
            if (idx0 == idx)
                return corner;
            idx0++;
        }

        assert(false && "should be unreachable");
        return cubelib_null_corner;
    }


    CUBELIB_FUNCTION_QUALIFIER
    cubelib_corner_t cubelib_get_corner_on_face(cubelib_face_t face, uint_fast8_t idx)
    {
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        assert(idx < 4);
        return cubelib_corners_on_face[cubelib_get_face_index(face)][idx];
    }

    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_calc_edge_on_face(cubelib_face_t face, uint_fast8_t idx)
    {
        assert(idx < 4);
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        
        
        cubelib_direction_t direction = cubelib_get_face_direction(face);
        
        uint_fast8_t sigdim = cubelib_get_direction_sigdim(direction);


        cubelib_edge_t face_edges[4] = {cubelib_null_edge, cubelib_null_edge, cubelib_null_edge, cubelib_null_edge};
        
        ///two edges will have a "base-axis" of sigdim + 1 (modulo 3),
        /// and two will have a "base-axis" of sigdim + 2 (modulo 3)
        
        uint_fast8_t next_dim = (sigdim + 1) % 3;
        uint_fast8_t next_next_dim = (sigdim + 2) % 3;
        uint_fast8_t face_base_axes[2] = {next_dim, next_next_dim};
        
        for (size_t edge_pair_i = 0; edge_pair_i < 2; ++edge_pair_i)
        {
            uint_fast8_t base_axis = face_base_axes[edge_pair_i];
            ///this will be the axis/direction across the face, from the pair's face-base-edge
            uint_fast8_t other_axis = face_base_axes[(edge_pair_i + 1) % 2];
            
            int other_axis_direction_int3[] = {0,0,0};
            other_axis_direction_int3[other_axis] = +1;
            cubelib_direction_t other_axis_direction = cubelib_get_direction_by_int3( other_axis_direction_int3[0]
                                                                                    , other_axis_direction_int3[1]
                                                                                    , other_axis_direction_int3[2]);
            
            ///this gets a base edge aligned with the face, but not necessarily in the face
            cubelib_edge_t pair_base_edge = cubelib_get_edge_by_axis(base_axis, false, false);
            
            ///push it into the face; now this will be in this face
            cubelib_edge_t pair_face_base_edge = cubelib_edge_push(pair_base_edge, direction);
            
            
            ///push pair_face_base_edge across the face
            cubelib_edge_t pair_face_2nd_edge = cubelib_edge_push(pair_face_base_edge, other_axis_direction);
            
            
            face_edges[edge_pair_i*2 + 0] = pair_face_base_edge;
            face_edges[edge_pair_i*2 + 1] = pair_face_2nd_edge;
        }
        
        
        return face_edges[idx];
    }
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_edge_t cubelib_get_edge_on_face(cubelib_face_t face, uint_fast8_t idx)
    {
        assert(idx < 4);
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        
        return cubelib_edges_on_face[cubelib_get_face_index(face)][idx];
    }
    
    
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_edge_on_face(cubelib_edge_t edge, cubelib_face_t face)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        
        
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        
        cubelib_direction_t direction = cubelib_get_face_direction(face);
        int xyz[] = {cubelib_get_direction_x(direction), cubelib_get_direction_y(direction), cubelib_get_direction_z(direction) };
        
        
        uint_fast8_t base_axis = cubelib_get_edge_base_axis(edge);
        uint_fast8_t secondary_axis = cubelib_get_edge_secondary_axis(edge);
        uint_fast8_t tertiary_axis = cubelib_get_edge_tertiary_axis(edge);
        
        int secondary_magnitude = cubelib_is_edge_projected_secondary(edge) ? +1 : -1;
        int tertiary_magnitude = cubelib_is_edge_projected_tertiary(edge) ? +1 : -1;
        
        if (xyz[base_axis] != 0)
            return false;
        
        if (xyz[secondary_axis] != 0 && xyz[secondary_axis] != secondary_magnitude)
            return false;
        
        if (xyz[tertiary_axis] != 0 && xyz[tertiary_axis] != tertiary_magnitude)
            return false;
        
        return true;
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_calc_face_on_edge(cubelib_edge_t edge, uint_fast8_t idx)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        assert(idx < 2);
        
        
        int face_xyz[2][3] = {{0,0,0},{0,0,0}};
        
        face_xyz[0][cubelib_get_edge_secondary_axis(edge)] = cubelib_is_edge_projected_secondary(edge) ? +1 : -1;
        face_xyz[1][cubelib_get_edge_tertiary_axis(edge)] = cubelib_is_edge_projected_tertiary(edge) ? +1 : -1;
        
        cubelib_face_t result = cubelib_get_direction_face( cubelib_get_direction_by_int3(face_xyz[idx][0], face_xyz[idx][1], face_xyz[idx][2]) );
        
        assert(cubelib_is_edge_on_face(edge, result));
        assert(cubelib_is_face_on_edge(result, edge));
        return result;
    }
    CUBELIB_FUNCTION_QUALIFIER cubelib_face_t cubelib_get_face_on_edge(cubelib_edge_t edge, uint_fast8_t idx)
    {
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        assert(idx < 2);
        cubelib_face_t result = cubelib_faces_on_edge[cubelib_get_edge_index(edge)][idx];
        assert(cubelib_is_edge_on_face(edge, result));
        assert(cubelib_is_face_on_edge(result, edge));
        return result;
    }
    
    CUBELIB_FUNCTION_QUALIFIER bool cubelib_is_face_on_edge(cubelib_face_t face, cubelib_edge_t edge)
    {
        assert(cubelib_is_face_valid(face));
        assert(!cubelib_is_face_null(face));
        assert(cubelib_is_edge_valid(edge));
        assert(!cubelib_is_edge_null(edge));
        
        cubelib_direction_t direction = cubelib_get_face_direction(face);
        int xyz[] = {cubelib_get_direction_x(direction), cubelib_get_direction_y(direction), cubelib_get_direction_z(direction) };
        
        
        uint_fast8_t base_axis = cubelib_get_edge_base_axis(edge);
        uint_fast8_t secondary_axis = cubelib_get_edge_secondary_axis(edge);
        uint_fast8_t tertiary_axis = cubelib_get_edge_tertiary_axis(edge);
        
        int secondary_magnitude = cubelib_is_edge_projected_secondary(edge) ? +1 : -1;
        int tertiary_magnitude = cubelib_is_edge_projected_tertiary(edge) ? +1 : -1;
        
        if (xyz[base_axis] != 0)
            return false;
        
        if (xyz[secondary_axis] != 0 && xyz[secondary_axis] != secondary_magnitude)
            return false;
        
        if (xyz[tertiary_axis] != 0 && xyz[tertiary_axis] != tertiary_magnitude)
            return false;
        
        return true;
    }

#ifdef __cplusplus
}
#endif





#endif
