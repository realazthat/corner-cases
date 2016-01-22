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
      face_t(direction_t::index(0)),
      face_t(direction_t::index(1)),
      face_t(direction_t::index(2)),
      face_t(direction_t::index(3)),
      face_t(direction_t::index(4)),
      face_t(direction_t::index(5))
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
  : bits(0b111)
{

#ifndef NDEBUG
  mx = x();
  my = y();
  mz = z();
  mindex = index();
#endif
}


CORNER_CASES_CUBEXX_INLINE
direction_t::direction_t(const std::bitset< 3 >& bits)
  : bits(bits)
{
#ifndef NDEBUG
  mx = x();
  my = y();
  mz = z();
  mindex = index();
#endif
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
  
  std::bitset<3>& bits = direction.bits;
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
  assert(direction.bits.to_ulong() < SIZE);
  return all()[direction.bits.to_ulong()];
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
  return (bits[0] ? 1 : -1)  * (bits[2] && !bits[1] ? 1 : 0);
}

CORNER_CASES_CUBEXX_INLINE
std::int_fast8_t direction_t::y() const
{
  return (bits[0] ? 1 : -1)  * (!bits[2] && bits[1] ? 1 : 0);
}

CORNER_CASES_CUBEXX_INLINE
std::int_fast8_t direction_t::z() const
{
  return (bits[0] ? 1 : -1)  * (!bits[2] && !bits[1] ? 1 : 0);
}


CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t direction_t::index() const
{
  return bits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE
const direction_t&
direction_t::index(std::uint_fast8_t idx)
{
  assert(idx < SIZE);
  return all()[idx];
}

CORNER_CASES_CUBEXX_INLINE
bool direction_t::
operator<(const direction_t& other) const
{
  return bits.to_ulong() < other.bits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE bool direction_t::operator==(const direction_t& other) const
{
  return bits == other.bits;
}

CORNER_CASES_CUBEXX_INLINE bool direction_t::operator!=(const direction_t& other) const
{
  return bits != other.bits;
}




CORNER_CASES_CUBEXX_INLINE
const face_t& direction_t::face() const
{
  return face_t::get(*this);
}

CORNER_CASES_CUBEXX_INLINE
const direction_t& direction_t::opposite() const
{
  std::bitset<3> result_bits = bits;
  result_bits.flip(0);
  
  assert(result_bits.to_ulong() < SIZE);
  return all()[ result_bits.to_ulong() ];
}

CORNER_CASES_CUBEXX_INLINE
bool direction_t::positive() const
{
  return bits.test(0);
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
{
  
}

CORNER_CASES_CUBEXX_INLINE
corner_t::
corner_t(const std::bitset< 3 >& bits)
  : bits(bits)
{
#ifndef NDEBUG
   mx = bits.test(0);
   my = bits.test(1);
   mz = bits.test(2);
#endif
}

CORNER_CASES_CUBEXX_INLINE
corner_t::
corner_t(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z)
  : bits( (x > 0 ? 1 : 0) | (y > 0 ? 2 : 0) | (z > 0 ? 4 : 0))
{
  
#ifndef NDEBUG
   mx = bits.test(0);
   my = bits.test(1);
   mz = bits.test(2);
#endif
  //bits.set(0, x);
  //bits.set(1, y);
  //bits.set(2, z);
}


CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
get(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z)
{
  std::bitset<3> bits;
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
      corner_t(std::bitset<3>(0)),
      corner_t(std::bitset<3>(1)),
      corner_t(std::bitset<3>(2)),
      corner_t(std::bitset<3>(3)),
      corner_t(std::bitset<3>(4)),
      corner_t(std::bitset<3>(5)),
      corner_t(std::bitset<3>(6)),
      corner_t(std::bitset<3>(7))
    }};
  return corners;
}

CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t
corner_t::
index() const
{
  return bits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
get(std::uint_fast8_t i)
{
  assert(i < SIZE);
  return all()[i];
}

CORNER_CASES_CUBEXX_INLINE
std::array< corner_t, 3 >
corner_t::
adjacents() const
{
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
  return corner_set_t(adjacents());
}

CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
x() const
{
  return (bits[0] ? 1 : -1);
}
CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
y() const
{
  return (bits[1] ? 1 : -1);
}

CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
z() const
{
  return (bits[2] ? 1 : -1);
}

CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
ux() const
{
  return bits[0];
}
CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
uy() const
{
  return bits[1];
}

CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
uz() const
{
  return bits[2];
}
CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator<(const corner_t& other) const
{
  return bits.to_ulong() < other.bits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE
std::array< face_t, 3 >
corner_t::faces() const
{
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
  assert(corner.index() < SIZE);
  return all()[corner.index()];
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::opposite() const
{
  std::size_t idx =  std::bitset<3>(bits).flip().to_ulong();
  assert(idx < SIZE);
  return all()[idx];
}

CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator!=(const corner_t& other) const
{
  return bits != other.bits;
}

CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator==(const corner_t& other) const
{
  return bits == other.bits;
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::index(std::uint_fast8_t idx)
{
  return corner_t::get(idx);
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
adjacent(const direction_t& direction) const
{
  
  return corner_t::get(detail::wrap_around_2(x() + 2*direction.x()),
                       detail::wrap_around_2(y() + 2*direction.y()),
                       detail::wrap_around_2(z() + 2*direction.z()));
}
CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
push(const direction_t& direction) const
{
  return corner_t::get(detail::clamp(x() + direction.x()*2, -1, +1),
                       detail::clamp(y() + direction.y()*2, -1, +1),
                       detail::clamp(z() + direction.z()*2, -1, +1));
}

CORNER_CASES_CUBEXX_INLINE
bool
corner_t::is_adjacent(const corner_t& other) const
{
  return detail::has_exactly_one_bit(bits ^ other.bits);
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
edge_t(const corner_t& a, const corner_t& b)
  : mcorners(detail::make_edge_corners(a,b))
{
  assert(mcorners.front().index() < mcorners.back().index());
  assert(mcorners.front() != mcorners.back());
  assert(mcorners.front().adjacents_set().contains(mcorners.back()));
  assert(mcorners.back().adjacents_set().contains(mcorners.front()));
}

CORNER_CASES_CUBEXX_INLINE
const std::array<corner_t, 2>&
edge_t::
corners() const
{
  return mcorners;
}

CORNER_CASES_CUBEXX_INLINE
const std::array<edge_t, 12>&
edge_t::
all()
{
  
  ///right top far
  static const corner_t& rtf = corner_t::get( true, true, true);
  
  ///right bottom near
  static const corner_t& rbn = corner_t::get( true,false,false);
  
  ///left bottom, far
  static const corner_t& lbf = corner_t::get(false,false, true);
  
  ///left top near
  static const corner_t& lbn = corner_t::get(false, true,false);
  
  
  
  
  static const std::array<edge_t, 12> result = 
  {{
    edge_t(rtf, rtf.adjacents()[0]),
    edge_t(rtf, rtf.adjacents()[1]),
    edge_t(rtf, rtf.adjacents()[2]),
    
    edge_t(rbn, rbn.adjacents()[0]),
    edge_t(rbn, rbn.adjacents()[1]),
    edge_t(rbn, rbn.adjacents()[2]),
    
    edge_t(lbf, lbf.adjacents()[0]),
    edge_t(lbf, lbf.adjacents()[1]),
    edge_t(lbf, lbf.adjacents()[2]),
    
    edge_t(lbn, lbn.adjacents()[0]),
    edge_t(lbn, lbn.adjacents()[1]),
    edge_t(lbn, lbn.adjacents()[2])
  }};
  
  return result;
}

//###################################################################




CORNER_CASES_CUBEXX_INLINE
cube_t::
cube_t()
{

}

CORNER_CASES_CUBEXX_INLINE
const cube_t&
cube_t::
identity()
{
  static const cube_t cube;
  return cube;
}


CORNER_CASES_CUBEXX_INLINE
const std::array< corner_t, 8 >&
cube_t::corners() const
{
  return corner_t::all();
}






















































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
  *this |= sequence;
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
  return derived_t(*this) |= set;
}


template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator|(const element_t& element) const
{
  return derived_t(*this) | element;
}

template<typename derived_t, typename element_t, std::size_t N>
template<typename Sequence>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator|(const Sequence& sequence) const
{
  return derived_t(*this) | sequence;
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
    *this |= element;
  }
  
  return self();
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

