

#include "cubexx/cubexx.hpp"
#include "cubexx/formatters.hpp"

#include <iostream>
#include <sstream>


#ifdef CORNER_CASES_CUBEXX_FORMATTERS_HEADER_ONLY
#define CORNER_CASES_CUBEXX_FORMATTERS_INLINE inline
#else
#define CORNER_CASES_CUBEXX_FORMATTERS_INLINE
#endif

namespace cubexx{

CORNER_CASES_CUBEXX_FORMATTERS_INLINE
::std::ostream& operator<<(::std::ostream& out, const face_t& face)
{

    return out << face.direction();
}
CORNER_CASES_CUBEXX_FORMATTERS_INLINE
::std::ostream& operator<<(::std::ostream& out, const direction_t& direction)
{
    if (direction.is_null())
        out << "null";
    else
        out << "(" << (direction.x() > 0 ? "+" : "") << (int)direction.x()
                 << "," << (direction.y() > 0 ? "+" : "") << (int)direction.y()
                 << "," << (direction.z() > 0 ? "+" : "") << (int)direction.z()
                 << ")";
    return out;
}

CORNER_CASES_CUBEXX_FORMATTERS_INLINE
::std::ostream& operator<<(::std::ostream& out, const corner_set_t& corner_set)
{
    std::size_t i = 0;
    
    out << "[";
    for (auto corner : corner_set)
    {
        out << (i++ != 0 ? ", " : "") << corner;
    }
    out << "]";
    
    return out;
}

CORNER_CASES_CUBEXX_FORMATTERS_INLINE
::std::ostream& operator<<(::std::ostream& out, const corner_t& corner)
{
    if (corner.is_null())
        std::cout << "null";
    else
        out << "(" << (corner.x() > 0 ? "+" : "") << (int)corner.x()
                 << "," << (corner.y() > 0 ? "+" : "") << (int)corner.y()
                 << "," << (corner.z() > 0 ? "+" : "") << (int)corner.z()
                 << ")";
    return out;
}

CORNER_CASES_CUBEXX_FORMATTERS_INLINE
::std::ostream& operator<<(::std::ostream& out, const edge_t& edge)
{
    if (edge.is_null())
        return std::cout << "null";
    
    
    auto corners = edge.corners();
    cubexx::corner_t corner0 = corners[0], corner1 = corners[1];
    
    out << "(" << corner0 << " <=> " << corner1 << ")";
    return out;
}

CORNER_CASES_CUBEXX_FORMATTERS_INLINE
std::string tostr(const corner_set_t& corner_set)
{
    std::ostringstream out;
    out << corner_set;
    return out.str();
}

CORNER_CASES_CUBEXX_FORMATTERS_INLINE
std::string tostr(const corner_t& corner)
{
    std::ostringstream out;
    out << corner;
    return out.str();
}

CORNER_CASES_CUBEXX_FORMATTERS_INLINE
std::string tostr(const direction_t& direction)
{
    std::ostringstream out;
    out << direction;
    return out.str();
}


CORNER_CASES_CUBEXX_FORMATTERS_INLINE
std::string tostr(const edge_t& edge)
{
    std::ostringstream out;
    out << edge;
    return out.str();
}




} // namespace cubexx


