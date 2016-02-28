
#include "cubelib/cubelib.h"

#include <iosfwd>
#include <iostream>
#include <sstream>


#ifdef CORNER_CASES_CUBELIB_FORMATTERS_HEADER_ONLY
#define CORNER_CASES_CUBELIB_FORMATTERS_INLINE inline
#else
#define CORNER_CASES_CUBELIB_FORMATTERS_INLINE
#endif






CORNER_CASES_CUBELIB_FORMATTERS_INLINE
std::ostream& operator<<(std::ostream& out, const direction_t& direction)
{

    if (is_direction_null(direction))
        out << "null";
    else
        out << "(" << get_direction_x(direction)
                 << "," << get_direction_y(direction)
                 << "," << get_direction_z(direction)
                 << ")";
    return out;
}

CORNER_CASES_CUBELIB_FORMATTERS_INLINE
std::ostream& operator<<(std::ostream& out, const cubelib_corner_t& corner)
{
    if (cubelib_is_corner_null(corner))
        out << "null";
    else
    out << "(" << cubelib_get_corner_x(corner)
             << "," << cubelib_get_corner_y(corner)
             << "," << cubelib_get_corner_z(corner)
             << ")";
    return out;
}

CORNER_CASES_CUBELIB_FORMATTERS_INLINE
std::ostream& operator<<(std::ostream& out, const edge_t& edge)
{
    if (is_edge_null(edge))
        out << "null";
    else
        out << "(" << get_edge_corner0(edge)
                 << " <=> "
                 << get_edge_corner1(edge)
                 << ")";
    return out;
}

CORNER_CASES_CUBELIB_FORMATTERS_INLINE
std::string tostr(const cubelib_corner_t& corner)
{
    std::ostringstream out;
    out << corner;
    return out.str();
}

CORNER_CASES_CUBELIB_FORMATTERS_INLINE
std::string tostr(const direction_t& direction)
{
    std::ostringstream out;
    out << direction;
    return out.str();
}




