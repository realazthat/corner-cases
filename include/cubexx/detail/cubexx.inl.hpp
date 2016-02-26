/*
    Copyright (c) 2012 Azriel Fasten azriel.fasten@gmail.com

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/


#include <cubexx/cubexx.hpp>

#include <cassert>
#include <vector>

namespace cubexx{

//###################################################################
//#### face_t
//###################################################################

CORNER_CASES_CUBEXX_INLINE
face_t::
face_t(const direction_t& direction)
  : mdirection(direction)
{

}


CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t
face_t::index() const
{
  return mdirection.index();
}


CORNER_CASES_CUBEXX_INLINE
const std::array< face_t, 6 >&
face_t::all()
{
  static const std::array<face_t, 6> faces = 
    {{
      face_t(direction_t::get(0)),
      face_t(direction_t::get(1)),
      face_t(direction_t::get(2)),
      face_t(direction_t::get(3)),
      face_t(direction_t::get(4)),
      face_t(direction_t::get(5))
    }};
  
  return faces;
}


CORNER_CASES_CUBEXX_INLINE
const face_t&
face_t::get(const direction_t& direction)
{
  assert(direction.index() < all().size());
  return all()[direction.index()];
}

CORNER_CASES_CUBEXX_INLINE
const face_t&
face_t::get(const std::uint_fast8_t& idx)
{
  assert(idx < all().size());
  return all()[idx];
}


CORNER_CASES_CUBEXX_INLINE
const direction_t& face_t::direction() const
{
  return mdirection;
}

CORNER_CASES_CUBEXX_INLINE
const face_t& face_t::opposite() const
{
  return face_t::get(mdirection.opposite());
}

CORNER_CASES_CUBEXX_INLINE
std::array<corner_t, 4>
face_t::corners() const
{
  
  /*
  std::array<std::int_fast8_t, 3> direction_components =
    {{
      direction.x(),
      direction.y(),
      direction.z()
    }};
  */
  //bool positive_direction = direction.positive();
  std::bitset<3> direction_bits;
  direction_bits.set(2, mdirection.x() != 0);
  direction_bits.set(1, mdirection.y() != 0);
  direction_bits.set(0, mdirection.z() != 0);
  
  corner_set_t corner_set;
  for(const corner_t& corner : corner_t::all())
  {
    std::bitset<3> corner_bits;
    corner_bits.set(2, corner.x() != 0);
    corner_bits.set(1, corner.y() != 0);
    corner_bits.set(0, corner.z() != 0);
    corner_bits &= direction_bits;
    if (corner_bits.any())
    {
      if (mdirection.positive())
        corner_set |= corner;
      else
        corner_set |= corner.opposite();
    }
  }
  
  assert(corner_set.size() == 4);
  std::vector<corner_t> tmp;
  
  for (const corner_t& corner : corner_t::all())
  {
    if (corner_set.contains(corner))
      tmp.push_back(corner);
  }
  
  ///FIXME: begin() end() broken
  /*
  corner_set_t::const_iterator w = corner_set.begin();
  corner_set_t::const_iterator wend = corner_set.end();
  
  for ( ; w != wend; ++w )
  {
    tmp.push_back(*w);
  }
  */
  
  assert(tmp.size() == 4);
  
  std::array<corner_t, 4> result =
    {{
      tmp[0],
      tmp[1],
      tmp[2],
      tmp[3]
    }};
  
  return result;
}

CORNER_CASES_CUBEXX_INLINE
bool
face_t::operator==(const face_t& other) const
{
  return mdirection == other.mdirection;
}

CORNER_CASES_CUBEXX_INLINE
bool
face_t::operator!=(const face_t& other) const
{
  return mdirection != other.mdirection;
}

CORNER_CASES_CUBEXX_INLINE
bool
face_t::operator<(const face_t& other) const
{
  return mdirection < other.mdirection;
}


//###################################################################



//###################################################################
//#### direction_t
//###################################################################



CORNER_CASES_CUBEXX_INLINE
direction_t::
direction_t()
  : mbits(6)
{
  assert(is_null());
  assert(is_sane());
}


CORNER_CASES_CUBEXX_INLINE
direction_t::direction_t(const std::bitset< 3 >& bits)
  : mbits(bits)
{
  assert(is_sane());
}




CORNER_CASES_CUBEXX_INLINE
bool
direction_t::
is_null() const
{
    return mbits == 6;
}

CORNER_CASES_CUBEXX_INLINE
bool
direction_t::
is_sane() const
{
    return mbits.to_ulong() <= 6;
}
CORNER_CASES_CUBEXX_INLINE
const direction_t&
direction_t::
null_direction()
{
    static const direction_t internal_null = direction_t();
    assert(internal_null.is_null());
    return internal_null;
}



CORNER_CASES_CUBEXX_INLINE
const direction_t&
direction_t::
get(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z)
{
  
  assert(detail::lxor(detail::lxor(x != 0, y != 0), z != 0));
  
  assert(std::abs(x) == 1 || x == 0);
  assert(std::abs(y) == 1 || y == 0);
  assert(std::abs(z) == 1 || z == 0);
  
  
  /**
   * [2 bit number][1 bit indicating "direction is positive"]
   *
   * [1 bit indicating "direction is positive"]: If the direction is positive, this bit is set to 1.
   *
   * [2 bit number]: A number calculated as follows:
   * 
   * direction is in z: 00
   * direction is in y: 01
   * direction is in x: 10
   */
  
  direction_t direction(std::bitset<3>(0));
  
  std::bitset<3>& bits = direction.mbits;
  if ( x != 0 )
  {
    bits.set(2);
  } else if ( y != 0 ) {
    bits.set(1);
  } else if ( z != 0 ) {
    
  }
  
  if (x + y + z > 0)
  {
    bits.set(0);
  }
  
  return direction_t::get( direction );
}

CORNER_CASES_CUBEXX_INLINE
const direction_t&
direction_t::
get(const direction_t& direction)
{
  assert(!direction.is_null());
  assert(direction.is_sane());
  assert(direction.mbits.to_ulong() < SIZE());
  return all()[direction.mbits.to_ulong()];
}

CORNER_CASES_CUBEXX_INLINE
const std::array< direction_t, 6 >&
direction_t::
all()
{
  static const std::array<direction_t, 6> directions =
    {{
      direction_t(std::bitset<3>(0)),
      direction_t(std::bitset<3>(1)),
      direction_t(std::bitset<3>(2)),
      direction_t(std::bitset<3>(3)),
      direction_t(std::bitset<3>(4)),
      direction_t(std::bitset<3>(5))
    }};
  return directions;
}

CORNER_CASES_CUBEXX_INLINE
std::int_fast8_t direction_t::x() const
{
  assert(!is_null());
  assert(is_sane());
  return (mbits[0] ? 1 : -1)  * (mbits[2] && !mbits[1] ? 1 : 0);
}

CORNER_CASES_CUBEXX_INLINE
std::int_fast8_t direction_t::y() const
{
  assert(!is_null());
  assert(is_sane());
  return (mbits[0] ? 1 : -1)  * (!mbits[2] && mbits[1] ? 1 : 0);
}

CORNER_CASES_CUBEXX_INLINE
std::int_fast8_t direction_t::z() const
{
  assert(!is_null());
  assert(is_sane());
  return (mbits[0] ? 1 : -1)  * (!mbits[2] && !mbits[1] ? 1 : 0);
}


CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t direction_t::index() const
{
  assert(!is_null());
  assert(is_sane());
  return mbits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE
const direction_t&
direction_t::get(std::uint_fast8_t idx)
{
  assert(idx < SIZE());
  return all()[idx];
}

CORNER_CASES_CUBEXX_INLINE
bool direction_t::
operator<(const direction_t& other) const
{
  assert(!is_null());
  assert(is_sane());
  assert(!other.is_null());
  assert(other.is_sane());
  return mbits.to_ulong() < other.mbits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE bool direction_t::operator==(const direction_t& other) const
{
  assert(is_sane());
  assert(other.is_sane());
  return mbits == other.mbits;
}

CORNER_CASES_CUBEXX_INLINE bool direction_t::operator!=(const direction_t& other) const
{
  assert(is_sane());
  assert(other.is_sane());
  return mbits != other.mbits;
}








CORNER_CASES_CUBEXX_INLINE
const face_t& direction_t::face() const
{
  return face_t::get(*this);
}

CORNER_CASES_CUBEXX_INLINE
const direction_t& direction_t::opposite() const
{
  std::bitset<3> result_bits = mbits;
  result_bits.flip(0);
  
  assert(result_bits.to_ulong() < SIZE());
  return all()[ result_bits.to_ulong() ];
}

CORNER_CASES_CUBEXX_INLINE
bool direction_t::positive() const
{
  return mbits.test(0);
}

CORNER_CASES_CUBEXX_INLINE
std::array<direction_t, 4>
direction_t::adjacents() const
{
  std::array<direction_t, 4> result = {{ direction_t(), direction_t(), direction_t(), direction_t()}};
  std::size_t ri = 0;
  for (const direction_t& d : direction_t::all())
  {
    if (d != *this && d != opposite())
    {
      assert(ri < result.size());
      result[ri++] = d;
    }
  }
  
  assert(ri == 4);
  
  return result;
}









//###################################################################





//###################################################################
//#### Corners
//###################################################################

CORNER_CASES_CUBEXX_INLINE
corner_t::
corner_t()
    : mbits(0b1000)
{
  
  assert(is_null());
}

CORNER_CASES_CUBEXX_INLINE
corner_t::
corner_t(const std::bitset< 4 >& bits)
  : mbits(bits)
{
#ifndef NDEBUG
   mx = mbits.test(0);
   my = mbits.test(1);
   mz = mbits.test(2);
#endif
}

CORNER_CASES_CUBEXX_INLINE
corner_t::
corner_t(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z)
  : mbits( (x > 0 ? 1 : 0) | (y > 0 ? 2 : 0) | (z > 0 ? 4 : 0))
{
  assert(!is_null());
#ifndef NDEBUG
   mx = mbits.test(0);
   my = mbits.test(1);
   mz = mbits.test(2);
#endif
  //bits.set(0, x);
  //bits.set(1, y);
  //bits.set(2, z);
}


CORNER_CASES_CUBEXX_INLINE
bool
corner_t::
is_null() const
{
    return mbits.test(3);
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
null_corner()
{
    static corner_t internal_null = corner_t();
    assert(internal_null.is_null());
    return internal_null;
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
get(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z)
{
  std::bitset<4> bits;
  bits.set(0,x > 0);
  bits.set(1,y > 0);
  bits.set(2,z > 0);
  return all()[bits.to_ulong()];
}


CORNER_CASES_CUBEXX_INLINE
const std::array< corner_t, 8 >&
corner_t::
all()
{
  static const std::array<corner_t, 8> corners =
    {{
      corner_t(std::bitset<4>(0)),
      corner_t(std::bitset<4>(1)),
      corner_t(std::bitset<4>(2)),
      corner_t(std::bitset<4>(3)),
      corner_t(std::bitset<4>(4)),
      corner_t(std::bitset<4>(5)),
      corner_t(std::bitset<4>(6)),
      corner_t(std::bitset<4>(7))
    }};
  return corners;
}

CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t
corner_t::
index() const
{
  assert(!is_null());
  return mbits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
get(std::uint_fast8_t i)
{
  assert(i < SIZE());
  return all()[i];
}

CORNER_CASES_CUBEXX_INLINE
std::array< corner_t, 3 >
corner_t::
adjacents() const
{
  assert(!is_null());
  std::array<corner_t,3> result =
    {{
      corner_t(-x(), y(), z() ),
      corner_t( x(),-y(), z() ),
      corner_t( x(), y(),-z() )
    }};
  
  return result;
}

CORNER_CASES_CUBEXX_INLINE
corner_set_t
corner_t::adjacents_set() const
{
  assert(!is_null());
  return corner_set_t(adjacents());
}

CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
x() const
{
  assert(!is_null());
  return (mbits[0] ? 1 : -1);
}
CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
y() const
{
  assert(!is_null());
  return (mbits[1] ? 1 : -1);
}

CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
z() const
{
  assert(!is_null());
  return (mbits[2] ? 1 : -1);
}

CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
ux() const
{
  assert(!is_null());
  return mbits[0];
}
CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
uy() const
{
  assert(!is_null());
  return mbits[1];
}

CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
uz() const
{
  assert(!is_null());
  return mbits[2];
}
CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator<(const corner_t& other) const
{
  assert(!is_null());
  assert(!other.is_null());
  return mbits.to_ulong() < other.mbits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE
std::array< face_t, 3 >
corner_t::faces() const
{
  assert(!is_null());
  std::array< face_t, 3 > result = 
    {{
      direction_t::get( x() > 0 ? 1 : -1, 0, 0).face(),
      direction_t::get( 0, y() > 0 ? 1 : -1, 0).face(),
      direction_t::get( 0, 0, z() > 0 ? 1 : -1).face()
    }};
  return result;
}


CORNER_CASES_CUBEXX_INLINE
face_set_t
corner_t::face_set() const
{
  return face_set_t(faces());
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::get(const corner_t& corner)
{
  assert(corner.index() < SIZE());
  return all()[corner.index()];
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::opposite() const
{
  assert(!is_null());
  std::size_t idx =  (std::bitset<4>(mbits) ^ std::bitset<4>(0b0111)).to_ulong();
  assert(idx < SIZE());
  return all()[idx];
}

CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator!=(const corner_t& other) const
{
  return mbits != other.mbits;
}

CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator==(const corner_t& other) const
{
  return mbits == other.mbits;
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
adjacent(const direction_t& direction) const
{
  assert(!is_null());
  return corner_t::get(detail::wrap_around_2(x() + 2*direction.x()),
                       detail::wrap_around_2(y() + 2*direction.y()),
                       detail::wrap_around_2(z() + 2*direction.z()));
}
CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
push(const direction_t& direction) const
{
  assert(!is_null());
  return corner_t::get(detail::clamp(x() + direction.x()*2, -1, +1),
                       detail::clamp(y() + direction.y()*2, -1, +1),
                       detail::clamp(z() + direction.z()*2, -1, +1));
}

CORNER_CASES_CUBEXX_INLINE
bool
corner_t::is_adjacent(const corner_t& other) const
{
  assert(!other.is_null());
  assert(!is_null());
  return detail::has_exactly_one_bit(mbits ^ other.mbits);
}


//###################################################################




//###################################################################
//#### edge_t
//###################################################################


namespace detail{
  CORNER_CASES_CUBEXX_INLINE
  std::array<corner_t, 2>
  make_edge_corners(const corner_t& a, const corner_t& b)
  {
    std::array<corner_t, 2> result =
    {{
      (a.index() < b.index() ? a : b),
      (a.index() < b.index() ? b : a)
    }};
    return result;
  }
} // namespace detail

CORNER_CASES_CUBEXX_INLINE
edge_t::
edge_t()
  : mbits(0b1100)
{
  assert(is_sane());
  assert(is_null());
}
CORNER_CASES_CUBEXX_INLINE
edge_t::
edge_t(const std::bitset<4>& bits)
  : mbits(bits)
{
  assert(is_sane());
}


CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
is_null() const
{
  return mbits == std::bitset<4>(0b1100);
}

CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
is_sane() const
{
  return mbits.to_ulong() <= 0b1100;
}

CORNER_CASES_CUBEXX_INLINE
const edge_t&
edge_t::
null_edge()
{
  static const auto internal_result = edge_t();
  return internal_result;
}


CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t
edge_t::
base_axis() const
{
  auto calc = [](const std::bitset<4>& bits){
    auto base_axis = std::uint_fast8_t((bits >> 2).to_ulong());
    assert(base_axis < 3);
    return std::uint_fast8_t(base_axis);
  };
  
  static const std::uint_fast8_t internal_result[] = {
      calc(edge_t::get(0).mbits)
    , calc(edge_t::get(1).mbits)
    , calc(edge_t::get(2).mbits)
    , calc(edge_t::get(3).mbits)
    , calc(edge_t::get(4).mbits)
    , calc(edge_t::get(5).mbits)
    , calc(edge_t::get(6).mbits)
    , calc(edge_t::get(7).mbits)
    , calc(edge_t::get(8).mbits)
    , calc(edge_t::get(9).mbits)
    , calc(edge_t::get(10).mbits)
    , calc(edge_t::get(11).mbits)
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  
  return internal_result[index()];
}

CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t
edge_t::
secondary_axis() const
{
  auto calc = [](const std::bitset<4>& bits){
    auto base_axis = std::uint_fast8_t((bits >> 2).to_ulong());
    assert(base_axis < 3);
    
#ifdef CUBEXX_EDGE_MODULAR_AXIS_STRATEGY
    /// one strategy
    return std::uint_fast8_t(base_axis + 1 % 3);
#else
    /// another strategy
    return std::uint_fast8_t(base_axis != 0 ? 0 : 1);
#endif
  };
  
  static const std::uint_fast8_t internal_result[] = {
      calc(edge_t::get(0).mbits)
    , calc(edge_t::get(1).mbits)
    , calc(edge_t::get(2).mbits)
    , calc(edge_t::get(3).mbits)
    , calc(edge_t::get(4).mbits)
    , calc(edge_t::get(5).mbits)
    , calc(edge_t::get(6).mbits)
    , calc(edge_t::get(7).mbits)
    , calc(edge_t::get(8).mbits)
    , calc(edge_t::get(9).mbits)
    , calc(edge_t::get(10).mbits)
    , calc(edge_t::get(11).mbits)
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  
  return internal_result[index()];
}


CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t
edge_t::
tertiary_axis() const
{
  auto calc = [](const std::bitset<4>& bits){
    auto base_axis = std::uint_fast8_t((bits >> 2).to_ulong());
    assert(base_axis < 3);
    
#ifdef CUBEXX_EDGE_MODULAR_AXIS_STRATEGY
    /// one strategy
    return std::uint_fast8_t(base_axis + 2 % 3);
#else
    /// another strategy
    return std::uint_fast8_t(base_axis != 2 ? 1 : 0);
#endif
  };
  
  static const std::uint_fast8_t internal_result[] = {
      calc(edge_t::get(0).mbits)
    , calc(edge_t::get(1).mbits)
    , calc(edge_t::get(2).mbits)
    , calc(edge_t::get(3).mbits)
    , calc(edge_t::get(4).mbits)
    , calc(edge_t::get(5).mbits)
    , calc(edge_t::get(6).mbits)
    , calc(edge_t::get(7).mbits)
    , calc(edge_t::get(8).mbits)
    , calc(edge_t::get(9).mbits)
    , calc(edge_t::get(10).mbits)
    , calc(edge_t::get(11).mbits)
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  
  return internal_result[index()];
}


CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
project_secondary() const
{
  assert(!is_null());
  assert(is_sane());
  return mbits.test(0);
}

CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
project_tertiary() const
{
  assert(!is_null());
  assert(is_sane());
  return mbits.test(1);
}



CORNER_CASES_CUBEXX_INLINE
const edge_t&
edge_t::
get(std::uint_fast8_t base_axis, bool project_secondary, bool project_tertiary)
{
  assert(base_axis < 3);
  
  
  std::uint_fast8_t bits = base_axis << 2;
  
  bits |= (project_secondary ? 1 : 0) << 0;
  bits |= (project_tertiary ? 1 : 0) << 1;
  
  assert(bits < 12);
  
  const edge_t& result = edge_t::get(bits);
  
  assert(result.base_axis() == base_axis);
  assert(result.project_secondary() == project_secondary);
  assert(result.project_tertiary() == project_tertiary);
  
  return result;
}

CORNER_CASES_CUBEXX_INLINE
const edge_t&
edge_t::
get(std::uint_fast8_t idx)
{
  assert(idx < 12);
  const auto& result = all()[idx];
  
  assert(!result.is_null());
  assert(result.is_sane());
  
  return result;
}

CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t
edge_t::
index() const
{
  assert(!is_null());
  assert(is_sane());
  
  std::uint_fast8_t result = mbits.to_ulong();
  assert(result < SIZE());
  return result;
}




CORNER_CASES_CUBEXX_INLINE
const corner_t&
edge_t::
calc_corner0() const
{
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  int xyz[] = {-1, -1, -1};
  
  xyz[secondary_axis()] += project_secondary() ? 2 : 0;
  xyz[tertiary_axis()] += project_tertiary() ? 2 : 0;
  return corner_t::get( xyz[0], xyz[1], xyz[2] );
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
edge_t::
calc_corner1() const
{
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  int xyz[] = {-1, -1, -1};
  
  xyz[secondary_axis()] += project_secondary() ? 2 : 0;
  xyz[tertiary_axis()] += project_tertiary() ? 2 : 0;
  xyz[base_axis()] += 2;
  return corner_t::get( xyz[0], xyz[1], xyz[2] );
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
edge_t::
corner0() const
{
  static const corner_t internal_results[] = {
      edge_t::get(0).calc_corner0()
    , edge_t::get(1).calc_corner0()
    , edge_t::get(2).calc_corner0()
    , edge_t::get(3).calc_corner0()
    , edge_t::get(4).calc_corner0()
    , edge_t::get(5).calc_corner0()
    , edge_t::get(6).calc_corner0()
    , edge_t::get(7).calc_corner0()
    , edge_t::get(8).calc_corner0()
    , edge_t::get(9).calc_corner0()
    , edge_t::get(10).calc_corner0()
    , edge_t::get(11).calc_corner0()
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  return internal_results[index()];
}


CORNER_CASES_CUBEXX_INLINE
const corner_t&
edge_t::
corner1() const
{
  static const corner_t internal_results[] = {
      edge_t::get(0).calc_corner1()
    , edge_t::get(1).calc_corner1()
    , edge_t::get(2).calc_corner1()
    , edge_t::get(3).calc_corner1()
    , edge_t::get(4).calc_corner1()
    , edge_t::get(5).calc_corner1()
    , edge_t::get(6).calc_corner1()
    , edge_t::get(7).calc_corner1()
    , edge_t::get(8).calc_corner1()
    , edge_t::get(9).calc_corner1()
    , edge_t::get(10).calc_corner1()
    , edge_t::get(11).calc_corner1()
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  return internal_results[index()];
}


CORNER_CASES_CUBEXX_INLINE
const edge_t&
edge_t::
calc_opposite() const
{
  assert(!is_null());
  assert(is_sane());
  
  return edge_t::get(base_axis(), !project_secondary(), !project_tertiary());
}

CORNER_CASES_CUBEXX_INLINE
const edge_t&
edge_t::
opposite() const
{
  
  static const edge_t internal_results[] = {
      edge_t::get(0).calc_opposite()
    , edge_t::get(1).calc_opposite()
    , edge_t::get(2).calc_opposite()
    , edge_t::get(3).calc_opposite()
    , edge_t::get(4).calc_opposite()
    , edge_t::get(5).calc_opposite()
    , edge_t::get(6).calc_opposite()
    , edge_t::get(7).calc_opposite()
    , edge_t::get(8).calc_opposite()
    , edge_t::get(9).calc_opposite()
    , edge_t::get(10).calc_opposite()
    , edge_t::get(11).calc_opposite()
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  return internal_results[index()];
}

CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
operator==(const edge_t& other) const
{
  assert(is_sane());
  assert(other.is_sane());
  return mbits == other.mbits;
}

CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
operator!=(const edge_t& other) const
{
  assert(is_sane());
  assert(other.is_sane());
  return mbits != other.mbits;
}

CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
operator<(const edge_t& other) const
{
  assert(is_sane());
  assert(!is_null());
  assert(other.is_sane());
  assert(!other.is_null());
  return index() < other.index();
}

CORNER_CASES_CUBEXX_INLINE
const std::array<corner_t, 2>&
edge_t::
corners() const
{
  static const std::array<corner_t, 2> internal_results[] = {
      std::array<corner_t, 2>{edge_t::get(0).calc_corner0(), edge_t::get(0).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(1).calc_corner0(), edge_t::get(1).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(2).calc_corner0(), edge_t::get(2).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(3).calc_corner0(), edge_t::get(3).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(4).calc_corner0(), edge_t::get(4).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(5).calc_corner0(), edge_t::get(5).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(6).calc_corner0(), edge_t::get(6).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(7).calc_corner0(), edge_t::get(7).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(8).calc_corner0(), edge_t::get(8).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(9).calc_corner0(), edge_t::get(9).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(10).calc_corner0(), edge_t::get(10).calc_corner1()}
    , std::array<corner_t, 2>{edge_t::get(11).calc_corner0(), edge_t::get(11).calc_corner1()}
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);
  
  return internal_results[ index() ];
}

CORNER_CASES_CUBEXX_INLINE
const std::array<edge_t, 12>&
edge_t::
all()
{
  
  
  
  
  static const std::array<edge_t, 12> result = 
  {{
    //x-based edge, no projection
    edge_t(std::bitset<4>(0b0000)),
    //x-based edge, y-projected
    edge_t(std::bitset<4>(0b0001)),
    //x-based edge, z-projected
    edge_t(std::bitset<4>(0b0010)),
    //x-based edge, yz-projected
    edge_t(std::bitset<4>(0b0011)),
    
    //y-based edge, no projection
    edge_t(std::bitset<4>(0b0100)),
    //y-based edge, x-projected
    edge_t(std::bitset<4>(0b0101)),
    //y-based edge, z-projected
    edge_t(std::bitset<4>(0b0110)),
    //y-based edge, xz-projected
    edge_t(std::bitset<4>(0b0111)),
    
    
    //z-based edge, no projection
    edge_t(std::bitset<4>(0b1000)),
    //z-based edge, x-projected
    edge_t(std::bitset<4>(0b1001)),
    //z-based edge, y-projected
    edge_t(std::bitset<4>(0b1010)),
    //z-based edge, xy-projected
    edge_t(std::bitset<4>(0b1011)),
    
  }};
  
  return result;
}

//###################################################################




























//###################################################################
//#### set_base_t
//###################################################################

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
set_base_t<derived_t, element_t, N>::
set_base_t()
  : mbits(0)
{

}


template<typename derived_t, typename element_t, std::size_t N>
template<typename Sequence>
CORNER_CASES_CUBEXX_INLINE
set_base_t<derived_t, element_t, N>::
set_base_t(const Sequence& sequence)
  : mbits(0)
{
  self() |= sequence;
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
bool
set_base_t<derived_t, element_t, N>::
contains(const element_t& element) const
{
  assert(element.index() < N);
  return mbits.test(element.index());
}


template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator=(const element_t& element)
{
  assert(element.index() < N);
  
  mbits.reset();
  mbits.set(element.index(), true);
  return self();
}



template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator=(const derived_t& set)
{
  mbits = set.mbits;
  return self();
}



template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator|(const derived_t& set) const
{
  return derived_t(self()) |= set;
}


template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator|(const element_t& element) const
{
  return derived_t(self()) | element;
}

template<typename derived_t, typename element_t, std::size_t N>
template<typename Sequence>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator|(const Sequence& sequence) const
{
  return derived_t(self()) | sequence;
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator|=(const derived_t& set)
{
  mbits |= set.mbits;
  
  return self();
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator|=(const element_t& element)
{
  assert(element.index() < N);
  mbits.set(element.index(), true);
  return self();
}



template<typename derived_t, typename element_t, std::size_t N>
template<typename Sequence>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator|=(const Sequence& sequence)
{
  
  for(const element_t& element : sequence)
  {
    self() |= element;
  }
  
  return self();
}



template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator-=(const derived_t& set)
{
  mbits &= ~set.mbits;
  
  return self();
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator-=(const element_t& element)
{
  assert(element.index() < N);
  mbits.set(element.index(), false);
  return self();
}



template<typename derived_t, typename element_t, std::size_t N>
template<typename Sequence>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator-=(const Sequence& sequence)
{
  
  for(const element_t& element : sequence)
  {
    self() -= element;
  }
  
  return self();
}





template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator-(const derived_t& set) const
{
  return derived_t(self()) -= set;
}


template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator-(const element_t& element) const
{
  return derived_t(self()) -= element;
}

template<typename derived_t, typename element_t, std::size_t N>
template<typename Sequence>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator-(const Sequence& sequence) const
{
  return derived_t(self()) -= sequence;
}




template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
self()
{
  static_assert( std::is_base_of< set_base_t<derived_t, element_t, N>, derived_t >::value
                , "Something's really wrong with our CRTP" );
  return static_cast<derived_t&>(*this);
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
const derived_t&
set_base_t<derived_t, element_t, N>::
self() const
{
  static_assert( std::is_base_of< set_base_t<derived_t, element_t, N>, derived_t >::value
                , "Something's really wrong with our CRTP" );
  return *static_cast<const derived_t*>(this);
}


template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
typename set_base_t<derived_t, element_t, N>::const_iterator 
set_base_t<derived_t, element_t, N>::
begin() const
{
  return const_iterator(&self());
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
typename set_base_t<derived_t, element_t, N>::const_iterator 
set_base_t<derived_t, element_t, N>::
end() const
{
  return const_iterator(&self(), N);
}


template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
std::size_t
set_base_t<derived_t, element_t, N>::
size() const
{
  return mbits.count();
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
void
set_base_t<derived_t, element_t, N>::
clear()
{
  mbits.reset();
}


template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
bool
set_base_t<derived_t, element_t, N>::
operator==(const derived_t& other) const
{
  return mbits == other.mbits;
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
bool
set_base_t<derived_t, element_t, N>::
operator!=(const derived_t& other) const
{
  return mbits != other.mbits;
}





template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
bool
set_base_t<derived_t, element_t, N>::
contains(const std::size_t& idx) const
{
  assert(idx < N);
  return mbits.test(idx);
}



template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
const std::bitset<N>&
set_base_t<derived_t, element_t, N>::
bits() const
{
  return mbits;
}







CORNER_CASES_CUBEXX_INLINE
direction_set_t::direction_set_t()
  : base_t()
{
  
}

template<typename T>
CORNER_CASES_CUBEXX_INLINE
direction_set_t::direction_set_t(const T& v)
  : base_t(v)
{
  
}


CORNER_CASES_CUBEXX_INLINE
face_set_t::face_set_t()
  : base_t()
{

}

template<typename T>
CORNER_CASES_CUBEXX_INLINE
face_set_t::face_set_t(const T& v)
  : base_t(v)
{

}





CORNER_CASES_CUBEXX_INLINE
corner_set_t::corner_set_t()
  : base_t()
{

}

template<typename T>
CORNER_CASES_CUBEXX_INLINE
corner_set_t::corner_set_t(const T& v)
  : base_t(v)
{

}









//###################################################################




} // namespace cubexx

