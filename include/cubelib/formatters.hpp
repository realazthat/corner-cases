#ifndef CORNER_CASES_CUBELIB_FORMATTERS_HPP
#define CORNER_CASES_CUBELIB_FORMATTERS_HPP 1

#include "cubelib/cubelib.fwd.h"

#include <iosfwd>
#include <string>


::std::ostream& operator<<(std::ostream& out, const cubelib_corner_t& corner);
::std::ostream& operator<<(std::ostream& out, const cubelib_direction_t& direction);
::std::ostream& operator<<(std::ostream& out, const edge_t& edge);

::std::string tostr(const cubelib_corner_t& corner);
::std::string tostr(const cubelib_direction_t& direction);
::std::string tostr(const edge_t& edge);

#ifdef CORNER_CASES_CUBELIB_FORMATTERS_HEADER_ONLY

#include "cubelib/detail/formatters.inl.hpp"

#endif

#endif // CORNER_CASES_CUBELIB_FORMATTERS_HPP

