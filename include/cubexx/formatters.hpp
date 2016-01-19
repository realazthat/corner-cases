#ifndef CORNER_CASES_CUBEXX_FORMATTERS_HPP
#define CORNER_CASES_CUBEXX_FORMATTERS_HPP 1

#include "cubexx/cubexx.fwd.hpp"

#include <iosfwd>
#include <iostream>
#include <sstream>




namespace cubexx{
::std::ostream& operator<<(::std::ostream& out, const corner_t& corner);
::std::ostream& operator<<(::std::ostream& out, const direction_t& direction);
::std::ostream& operator<<(::std::ostream& out, const edge_t& edge);
::std::ostream& operator<<(::std::ostream& out, const face_t& face);

::std::string tostr(const corner_t& corner);
::std::string tostr(const direction_t& direction);
::std::string tostr(const edge_t& edge);
::std::string tostr(const face_t& face);

} // namespace cubexx


#ifdef CORNER_CASES_CUBEXX_FORMATTERS_HEADER_ONLY

#include "cubexx/detail/formatters.inl.hpp"

#endif


#endif // CORNER_CASES_CUBEXX_FORMATTERS_HPP

