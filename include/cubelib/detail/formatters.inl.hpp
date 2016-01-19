
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

    if (is_null_direction(direction))
        out << "null";
    else
        out << "(" << get_direction_x(direction)
                 << "," << get_direction_y(direction)
                 << "," << get_direction_z(direction)
                 << ")";
    return out;
}

CORNER_CASES_CUBELIB_FORMATTERS_INLINE
std::ostream& operator<<(std::ostream& out, const corner_t& corner)
{
    if (is_null_corner(corner))
        out << "null";
    else
    out << "(" << get_corner_x(corner)
             << "," << get_corner_y(corner)
             << "," << get_corner_z(corner)
             << ")";
    return out;
}

CORNER_CASES_CUBELIB_FORMATTERS_INLINE
std::string tostr(const corner_t& corner)
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




