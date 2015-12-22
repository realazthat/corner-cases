#ifndef CUBELIB_FORMATTERS_HPP
#define CUBELIB_FORMATTERS_HPP 1

#include "cubelib/cubelib.h"

#include <iosfwd>
#include <iostream>
#include <sstream>





std::ostream& operator<<(std::ostream& out, const direction_t& direction);
std::ostream& operator<<(std::ostream& out, const corner_t& corner);


inline
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

inline
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

inline std::string tostr(const corner_t& corner)
{
    std::ostringstream out;
    out << corner;
    return out.str();
}

inline std::string tostr(const direction_t& direction)
{
    std::ostringstream out;
    out << direction;
    return out.str();
}




#endif

