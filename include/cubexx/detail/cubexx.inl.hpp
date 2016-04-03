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
  assert(is_sane());
}

CORNER_CASES_CUBEXX_INLINE
face_t::
face_t()
  : mdirection(direction_t::null_direction())
{
  assert(is_sane());
  assert(is_null());
}

CORNER_CASES_CUBEXX_INLINE
bool
face_t::is_sane() const
{
  return mdirection.is_sane();
}
CORNER_CASES_CUBEXX_INLINE
bool
face_t::is_null() const
{
  return mdirection.is_null();
}


CORNER_CASES_CUBEXX_INLINE
const face_t&
face_t::null_face()
{
  static const face_t internal = face_t();
  return internal;
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
  const face_t& result = all()[idx];
  assert(result.is_sane());
  assert(!result.is_null());
  return result;
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
face_t::calc_corners() const
{
  assert(is_sane());
  assert(!is_null());

  std::array<corner_t, 4> result;

  auto dir = this->direction();

  int xyz[] = {dir.x(), dir.y(), dir.z()};

  auto axis = dir.axis();
  auto axis2 = (axis + 1) % 3;
  auto axis3 = (axis + 2) % 3;

  assert(xyz[axis] != 0);
  assert(xyz[axis2] == 0);
  assert(xyz[axis3] == 0);

  int i = 0;
  for (auto u : {-1,+1})
  for (auto v : {-1,+1})
  {
    xyz[axis2] = u;
    xyz[axis3] = v;

    corner_t corner = corner_t::get(xyz[0], xyz[1], xyz[2]);
    result[i] = corner;

    ++i;
  }

  return result;
}


CORNER_CASES_CUBEXX_INLINE
const std::array<corner_t, 4>&
face_t::corners() const
{

  static const std::array<corner_t, 4> internal_results[] = {
      (face_t::get(0).calc_corners())
    , (face_t::get(1).calc_corners())
    , (face_t::get(2).calc_corners())
    , (face_t::get(3).calc_corners())
    , (face_t::get(4).calc_corners())
    , (face_t::get(5).calc_corners())
  };



  assert(!is_null());
  assert(is_sane());
  assert(index() < 6);

  const std::array<corner_t, 4>& internal_result = internal_results[index()];


  return internal_result;

}

CORNER_CASES_CUBEXX_INLINE
const corner_set_t&
face_t::corner_set() const
{

  static const corner_set_t internal_results[] = {
      corner_set_t(face_t::get(0).corners())
    , corner_set_t(face_t::get(1).corners())
    , corner_set_t(face_t::get(2).corners())
    , corner_set_t(face_t::get(3).corners())
    , corner_set_t(face_t::get(4).corners())
    , corner_set_t(face_t::get(5).corners())
  };



  assert(!is_null());
  assert(is_sane());
  assert(index() < 6);

  const corner_set_t& internal_result = internal_results[index()];


  return internal_result;

}

CORNER_CASES_CUBEXX_INLINE
const std::array<edge_t, 4>&
face_t::edges() const
{
  typedef std::array<edge_t, 4> result_type;
  auto precompute = []()
  {
    std::array<result_type, 6> internal_results;

    for (auto face : face_t::all())
    {
      auto direction = face.direction();

      int xyz[] = {0,0,0};

      xyz[direction.axis()] = direction.positive() ? +1 : -1;


      corner_t corners[] = {corner_t::null_corner(),corner_t::null_corner(),corner_t::null_corner(),corner_t::null_corner()};


      xyz[(direction.axis() + 1) % 3] = -1;
      xyz[(direction.axis() + 2) % 3] = -1;
      corners[0] = corner_t::get(xyz[0], xyz[1], xyz[2]);
      xyz[(direction.axis() + 1) % 3] = +1;
      xyz[(direction.axis() + 2) % 3] = -1;
      corners[1] = corner_t::get(xyz[0], xyz[1], xyz[2]);
      xyz[(direction.axis() + 1) % 3] = +1;
      xyz[(direction.axis() + 2) % 3] = +1;
      corners[2] = corner_t::get(xyz[0], xyz[1], xyz[2]);
      xyz[(direction.axis() + 1) % 3] = -1;
      xyz[(direction.axis() + 2) % 3] = +1;
      corners[3] = corner_t::get(xyz[0], xyz[1], xyz[2]);


      internal_results[face.index()][0] = edge_t::get(corners[0], corners[1]);
      internal_results[face.index()][1] = edge_t::get(corners[1], corners[2]);
      internal_results[face.index()][2] = edge_t::get(corners[2], corners[3]);
      internal_results[face.index()][3] = edge_t::get(corners[3], corners[0]);
    }

    return internal_results;
  };

  static const auto internal_results = precompute();

  assert(is_sane());
  assert(!is_null());
  assert(index() < 6);

  const auto& result = internal_results[index()];

  return result;
}
CORNER_CASES_CUBEXX_INLINE
const edge_set_t&
face_t::edge_set() const
{
  auto precompute = []()
  {
    std::array<edge_set_t, 6> internal_results;

    for (auto face : face_t::all())
    {
      internal_results[face.index()] = edge_set_t(face.edges());
    }

    return internal_results;
  };

  static const auto internal_results = precompute();

  assert(is_sane());
  assert(!is_null());
  assert(index() < 6);

  const auto& result = internal_results[index()];

  return result;
}



CORNER_CASES_CUBEXX_INLINE
bool
face_t::is_adjacent(const face_t& other) const
{
  assert(is_sane());
  assert(!is_null());
  assert(other.is_sane());
  assert(!other.is_null());
  return (other.corner_set() & this->corner_set()).size() == 2;
}

CORNER_CASES_CUBEXX_INLINE
bool
face_t::is_adjacent(const edge_t& edge) const
{

  assert(is_sane());
  assert(!is_null());
  assert(edge.is_sane());
  assert(!edge.is_null());
  
  
  const auto& corner0 = edge.corner0();
  
  int corner0_xyz[] = {corner0.x(), corner0.y(), corner0.z()};
  int direction_xyz[] = {direction().x(), direction().y(), direction().z()};
  
  ///the edge and face/direction are not parallel and
  return edge.base_axis() != direction().axis()
          ///the significant dimension component of any edge's corner is the same as the direction
          && corner0_xyz[direction().axis()] == direction_xyz[direction().axis()];
  
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

  assert(detail::lxor(detail::lxor(x != 0, y != 0), z != 0) && "direction must have one non-zero component");



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
  assert(is_sane());
  assert(!is_null());
  return (mbits[0] ? 1 : -1)  * (mbits[2] && !mbits[1] ? 1 : 0);
}

CORNER_CASES_CUBEXX_INLINE
std::int_fast8_t direction_t::y() const
{
  assert(is_sane());
  assert(!is_null());
  return (mbits[0] ? 1 : -1)  * (!mbits[2] && mbits[1] ? 1 : 0);
}

CORNER_CASES_CUBEXX_INLINE
std::int_fast8_t direction_t::z() const
{
  assert(is_sane());
  assert(!is_null());
  return (mbits[0] ? 1 : -1)  * (!mbits[2] && !mbits[1] ? 1 : 0);
}


CORNER_CASES_CUBEXX_INLINE
std::uint_fast8_t direction_t::index() const
{
  assert(is_sane());
  assert(!is_null());
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
  return mbits.to_ulong() < other.mbits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE bool direction_t::operator==(const direction_t& other) const
{
  return mbits == other.mbits;
}

CORNER_CASES_CUBEXX_INLINE bool direction_t::operator!=(const direction_t& other) const
{
  return mbits != other.mbits;
}








CORNER_CASES_CUBEXX_INLINE
const face_t& direction_t::face() const
{
  assert(is_sane());
  assert(!is_null());

  return face_t::get(*this);
}

CORNER_CASES_CUBEXX_INLINE
const direction_t& direction_t::opposite() const
{
  assert(is_sane());
  assert(!is_null());

  std::bitset<3> result_bits = mbits;
  result_bits.flip(0);

  assert(result_bits.to_ulong() < SIZE());
  return all()[ result_bits.to_ulong() ];
}


CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
direction_t::axis() const
{
  auto precompute = [](){
    std::array<uint8_t, 6> internal_results;
    for (auto direction : direction_t::all())
    {
      uint8_t axis = direction.x() != 0 ? 0
                   : (direction.y() != 0 ? 1
                      : 2);

      int xyz[] = {direction.x(), direction.y(), direction.z()};
      assert(xyz[axis] != 0);
      internal_results[direction.index()] = axis;
    }

    return internal_results;
  };

  static const auto& internal_results = precompute();


  assert(is_sane());
  assert(!is_null());


  assert(index() < 6);
  auto result = internal_results[index()];

  int xyz[] = {x(), y(), z()};
  assert(xyz[result] != 0);

  return result;
}

CORNER_CASES_CUBEXX_INLINE
bool direction_t::positive() const
{
  assert(is_sane());
  assert(!is_null());
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

  assert(is_sane());
  assert(!is_null());
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

  assert(is_sane());
  assert(is_null());
}

CORNER_CASES_CUBEXX_INLINE
corner_t::
corner_t(const std::bitset< 4 >& bits)
  : mbits(bits)
{
  assert(is_sane());
}

CORNER_CASES_CUBEXX_INLINE
corner_t::
corner_t(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z)
  : mbits( (x > 0 ? 1 : 0) | (y > 0 ? 2 : 0) | (z > 0 ? 4 : 0))
{
  assert(is_sane());
  assert(!is_null());
}


CORNER_CASES_CUBEXX_INLINE
bool
corner_t::
is_sane() const
{
  return mbits.to_ulong() <= 8;
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
  assert(internal_null.is_sane());
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
  const corner_t& result = all()[bits.to_ulong()];

  assert(result.is_sane());
  assert(!result.is_null());
  return result;
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
  assert(is_sane());
  assert(!is_null());
  return mbits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
get(std::uint_fast8_t i)
{
  assert(i < SIZE());
  const corner_t& result = all()[i];

  assert(result.is_sane());
  assert(!result.is_null());
  return result;
}

CORNER_CASES_CUBEXX_INLINE
const std::array< corner_t, 3 >&
corner_t::
adjacents() const
{

  auto precompute = [](){
    typedef std::array<corner_t,3> result_type;
    std::array<result_type, 12> internal_results;

    for (auto corner: all())
    {
      internal_results[corner.index()] = {{
        corner_t(-corner.x(), corner.y(), corner.z() ),
        corner_t( corner.x(),-corner.y(), corner.z() ),
        corner_t( corner.x(), corner.y(),-corner.z() )
      }};
    }
    return internal_results;
  };

  static const auto internal_results = precompute();

  assert(is_sane());
  assert(!is_null());
  assert(index() < internal_results.size());

  const auto& result = internal_results[index()];

  assert(!corner_set_t(result).contains(*this));
  for (auto other : result)
  {
    assert(other.is_adjacent(*this));
    assert(is_adjacent(other));
  }

  return result;
}

CORNER_CASES_CUBEXX_INLINE
const corner_set_t&
corner_t::adjacent_set() const
{
  auto precompute = [](){
    std::array<corner_set_t, 12> internal_results;

    for (auto corner: all())
    {
      internal_results[corner.index()] = corner_set_t(corner.adjacents());
    }
    return internal_results;
  };

  static const auto internal_results = precompute();

  assert(is_sane());
  assert(!is_null());
  assert(index() < internal_results.size());

  const auto& result = internal_results[index()];

  assert(result.contains(*this));
  for (auto other : result)
  {
    assert(other.is_adjacent(*this));
    assert(is_adjacent(other));
  }

  return result;
}

CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
x() const
{
  assert(is_sane());
  assert(!is_null());
  return (mbits[0] ? 1 : -1);
}
CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
y() const
{
  assert(is_sane());
  assert(!is_null());
  return (mbits[1] ? 1 : -1);
}

CORNER_CASES_CUBEXX_INLINE
int_fast8_t
corner_t::
z() const
{
  assert(is_sane());
  assert(!is_null());
  return (mbits[2] ? 1 : -1);
}

CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
ux() const
{
  assert(is_sane());
  assert(!is_null());
  return mbits[0];
}
CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
uy() const
{
  assert(is_sane());
  assert(!is_null());
  return mbits[1];
}

CORNER_CASES_CUBEXX_INLINE
uint_fast8_t
corner_t::
uz() const
{
  assert(is_sane());
  assert(!is_null());
  return mbits[2];
}



CORNER_CASES_CUBEXX_INLINE
const std::array<int_fast8_t,3>&
corner_t::
xyz() const
{
  typedef std::array<int_fast8_t,3> result_type;
  auto precompute = [](){
    std::array<result_type, 8> internal_results;
    
    
    for (auto corner : cubexx::corner_t::all())
    {
      internal_results[corner.index()] = result_type{corner.x(), corner.y(),corner.z()};
    }
    return internal_results;
  };
  
  static const auto internal_results = precompute();
  
  assert(is_sane());
  assert(!is_null());
  
  const auto& result = internal_results[index()];
  
  assert(result[0] == x());
  assert(result[1] == y());
  assert(result[2] == z());
  
  return result;
  
}

CORNER_CASES_CUBEXX_INLINE
const std::array<uint_fast8_t,3>&
corner_t::
uxyz() const
{
  typedef std::array<uint_fast8_t,3> result_type;
  auto precompute = [](){
    std::array<result_type, 8> internal_results;
    
    
    for (auto corner : cubexx::corner_t::all())
    {
      internal_results[corner.index()] = result_type{corner.ux(), corner.uy(),corner.uz()};
    }
    return internal_results;
  };
  
  static const auto internal_results = precompute();
  
  assert(is_sane());
  assert(!is_null());
  
  const auto& result = internal_results[index()];
  
  assert(result[0] == ux());
  assert(result[1] == uy());
  assert(result[2] == uz());
  
  return result;
  
}



CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator<(const corner_t& other) const
{
  assert(is_sane());
  assert(!is_null());
  assert(other.is_sane());
  assert(!other.is_null());
  return mbits.to_ulong() < other.mbits.to_ulong();
}

CORNER_CASES_CUBEXX_INLINE
const std::array< face_t, 3 >&
corner_t::faces() const
{
  typedef std::array< face_t, 3 > result_type;
  auto precompute = [](){
    std::array<result_type, 12> internal_results;
    for (auto corner : all())
    {
      internal_results[corner.index()] = {{
        direction_t::get( corner.x() > 0 ? 1 : -1, 0, 0).face(),
        direction_t::get( 0, corner.y() > 0 ? 1 : -1, 0).face(),
        direction_t::get( 0, 0, corner.z() > 0 ? 1 : -1).face()
      }};
    }
    return internal_results;
  };

  static const auto internal_results = precompute();

  assert(is_sane());
  assert(!is_null());
  assert(index() < internal_results.size());

  const auto& result = internal_results[index()];


  return result;
}


CORNER_CASES_CUBEXX_INLINE
const face_set_t&
corner_t::face_set() const
{
  auto precompute = [](){
    std::array<face_set_t, 12> internal_results;
    for (auto corner : all())
    {
      internal_results[corner.index()] = face_set_t(corner.faces());
    }
    return internal_results;
  };

  static const auto internal_results = precompute();

  assert(is_sane());
  assert(!is_null());
  assert(index() < internal_results.size());

  const auto& result = internal_results[index()];


  return result;
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::get(const corner_t& corner)
{
  assert(corner.is_sane());
  assert(!corner.is_null());
  assert(corner.index() < SIZE());
  const corner_t& result = all()[corner.index()];
  assert(result.is_sane());
  assert(!result.is_null());
  return result;
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::opposite() const
{
  assert(is_sane());
  assert(!is_null());
  std::size_t idx =  (std::bitset<4>(mbits) ^ std::bitset<4>(0b0111)).to_ulong();
  assert(idx < SIZE());
  const corner_t& result = all()[idx];
  assert(result.is_sane());
  assert(!result.is_null());
  return result;
}

CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator!=(const corner_t& other) const
{
  assert(is_sane());
  return mbits != other.mbits;
}

CORNER_CASES_CUBEXX_INLINE
bool corner_t::operator==(const corner_t& other) const
{
  assert(is_sane());
  return mbits == other.mbits;
}

CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
adjacent(const direction_t& direction) const
{
  assert(is_sane());
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
  assert(is_sane());
  assert(!is_null());
  return corner_t::get(detail::clamp(x() + direction.x()*2, -1, +1),
                       detail::clamp(y() + direction.y()*2, -1, +1),
                       detail::clamp(z() + direction.z()*2, -1, +1));
}


CORNER_CASES_CUBEXX_INLINE
const corner_t&
corner_t::
move(const direction_t& direction) const
{
  assert(is_sane());
  assert(!is_null());


  const auto& result = push(direction);
  if (*this == result)
    return null_corner();
  return result;
}


CORNER_CASES_CUBEXX_INLINE
const edge_t&
corner_t::
edge(const direction_t& direction) const
{
  auto precompute = []()
  {
    std::array< std::array<edge_t, 12>, 3> internal_results;
    for (auto corner : corner_t::all())
    {
      for (auto direction : direction_t::all())
      {
        uint_fast8_t base_axis = direction.axis();
        uint_fast8_t secondary_axis = (base_axis + 1) % 3;
        uint_fast8_t tertiary_axis = (base_axis + 2) % 3;

        int xyz[] = {corner.ux(), corner.uy(), corner.uz()};

        bool projected_secondary = xyz[secondary_axis] > 0;
        bool projected_tertiary = xyz[tertiary_axis] > 0;

        edge_t result = edge_t::get(base_axis, projected_secondary, projected_tertiary);

        assert(result.corner_set().contains(corner));
        assert(result.base_axis() == direction.axis());

        internal_results[base_axis][corner.index()] = result;
      }
    }
    return internal_results;
  };

  static const auto internal_results = precompute();

  assert(is_sane());
  assert(!is_null());
  assert(direction.is_sane());
  assert(!direction.is_null());


  uint_fast8_t base_axis = direction.axis();

  assert(base_axis < 3);
  assert(index() < 12);

  const edge_t& result = internal_results[base_axis][index()];

  assert(result.corner_set().contains(*this));

  return result;
}


CORNER_CASES_CUBEXX_INLINE
const edge_t&
corner_t::
edge(const corner_t& other) const
{
  assert(is_sane());
  assert(!is_null());
  assert(other.is_sane());
  assert(!other.is_null());

  assert(is_adjacent(other));
  assert(other.is_adjacent(*this));

  return this->edge(this->get_adjacent_direction(other));
}


CORNER_CASES_CUBEXX_INLINE
const std::array<edge_t,3 >&
corner_t::
edges() const
{
  typedef std::array<edge_t,3 > result_type;
  auto precompute = [](){
    std::array<result_type, 8> internal_results;
    
    
    for (auto corner : corner_t::all())
    {
      for (auto axis : {0U, 1U, 2U})
      {
        auto corner_xyz = corner.xyz();
        
        ///flip one component
        corner_xyz[axis] = -corner_xyz[axis];
        
        corner_t other_corner = corner_t::get(corner_xyz[0],corner_xyz[1],corner_xyz[2]);
        
        assert(corner.is_adjacent(other_corner));
        assert(other_corner.is_adjacent(corner));
        internal_results[corner.index()][axis] = corner.edge(other_corner);
      }
    }
    
    return internal_results;
  };
  
  
  static const auto internal_results = precompute();
  
  assert(is_sane());
  assert(!is_null());
  
  const result_type& result = internal_results[index()];
  
  for (const edge_t& edge : result)
  {
    assert(edge.is_sane());
    assert(!edge.is_null());
    assert(edge.corner_set().contains(*this));
    assert(edge.is_adjacent(*this));
    assert(is_adjacent(edge));
  }
  
  return result;
}



CORNER_CASES_CUBEXX_INLINE
const edge_set_t&
corner_t::
edge_set() const
{
  
  auto precompute = [](){
    std::array<edge_set_t, 8> internal_results;
    
    
    for (auto corner : corner_t::all())
    {
      internal_results[corner.index()] = edge_set_t(corner.edges());
    }
    
    return internal_results;
  };
  
  
  static const auto internal_results = precompute();
  
  assert(is_sane());
  assert(!is_null());
  
  const edge_set_t& result = internal_results[index()];
  
  for (const edge_t& edge : result)
  {
    assert(edge.is_sane());
    assert(!edge.is_null());
    assert(edge.corner_set().contains(*this));
    assert(edge.is_adjacent(*this));
    assert(is_adjacent(edge));
  }
  
  return result;
}

CORNER_CASES_CUBEXX_INLINE
const direction_t&
corner_t::get_adjacent_direction(const corner_t& other) const
{
  assert(is_sane());
  assert(!is_null());
  assert(other.is_sane());
  assert(!other.is_null());
  assert(is_adjacent(other));
  assert(other.is_adjacent(*this));

  int xyz[] = {other.x() - x(), other.y() - y(), other.z() - z()};

  return direction_t::get(xyz[0], xyz[1], xyz[2]);
}

CORNER_CASES_CUBEXX_INLINE
bool
corner_t::is_adjacent(const corner_t& other) const
{
  assert(is_sane());
  assert(!is_null());
  assert(other.is_sane());
  assert(!other.is_null());
  return detail::has_exactly_one_bit(mbits ^ other.mbits);
}

CORNER_CASES_CUBEXX_INLINE
bool
corner_t::is_adjacent(const edge_t& edge) const
{
  assert(is_sane());
  assert(!is_null());
  assert(edge.is_sane());
  assert(!edge.is_null());
  
  
  return edge.corner0() == *this || edge.corner1() == *this;
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

    return std::uint_fast8_t((base_axis + 1) % 3);
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
  auto precompute = [](const std::bitset<4>& bits){
    auto base_axis = std::uint_fast8_t((bits >> 2).to_ulong());
    assert(base_axis < 3);

    return std::uint_fast8_t((base_axis + 2) % 3);
  };

  static const std::uint_fast8_t internal_result[] = {
      precompute(edge_t::get(0).mbits)
    , precompute(edge_t::get(1).mbits)
    , precompute(edge_t::get(2).mbits)
    , precompute(edge_t::get(3).mbits)
    , precompute(edge_t::get(4).mbits)
    , precompute(edge_t::get(5).mbits)
    , precompute(edge_t::get(6).mbits)
    , precompute(edge_t::get(7).mbits)
    , precompute(edge_t::get(8).mbits)
    , precompute(edge_t::get(9).mbits)
    , precompute(edge_t::get(10).mbits)
    , precompute(edge_t::get(11).mbits)
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
bool
edge_t::
is_adjacent(const edge_t& other) const
{
  assert(is_sane());
  assert(!is_null());
  assert(other.is_sane());
  assert(!other.is_null());

  return (other.corner_set() & this->corner_set()).size() > 0;
}

CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
is_adjacent(const face_t& face) const
{
  assert(is_sane());
  assert(!is_null());
  assert(face.is_sane());
  assert(!face.is_null());

  return (face.corner_set() & this->corner_set()).size() == 2;
}

CORNER_CASES_CUBEXX_INLINE
bool
edge_t::
is_adjacent(const corner_t& corner) const
{
  assert(is_sane());
  assert(!is_null());
  assert(corner.is_sane());
  assert(!corner.is_null());

  return this->corner_set().contains(corner);
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
get(const corner_t& lhs, const corner_t& rhs)
{
    auto precompute = [](){
        std::array<edge_t, 144> internal_results;

        for (auto lhs : corner_t::all())
        {
            for (auto direction : direction_t::all())
            {
                auto rhs = lhs.adjacent(direction);

                size_t idx = lhs.index() + rhs.index()*corner_t::SIZE();

                assert(idx < 144);

                internal_results[idx] = lhs.edge(rhs);
            }
        }

        return internal_results;
    };

    assert(lhs.is_sane());
    assert(!lhs.is_null());
    assert(rhs.is_sane());
    assert(!rhs.is_null());
    assert(lhs != rhs);
    assert(lhs.is_adjacent(rhs));

    static const std::array<edge_t, 144> internal_results = precompute();

    size_t idx = lhs.index() + rhs.index()*corner_t::SIZE();

    assert(idx < 144);

    const edge_t& internal_result = internal_results[idx];
    assert(internal_result.is_sane());
    assert(!internal_result.is_null());
    assert(internal_result.corner_set().contains(lhs));
    assert(internal_result.corner_set().contains(rhs));

    return internal_result;
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
const edge_t&
edge_t::
opposite(const face_t& face) const
{
    auto precompute = [](){
        std::array< std::array<edge_t, 6>, 12> internal_results;

        for (auto edge : edge_t::all())
        {
            corner_t lhs = edge.corner0();
            corner_t rhs = edge.corner1();

            for (auto face : edge.faces())
            {
                auto other_corners = face.corner_set() - lhs - rhs;

                std::vector<corner_t> opposite_corners(other_corners.begin(),other_corners.end());

                auto opposite_edge = edge_t::get(opposite_corners[0], opposite_corners[1]);
                internal_results[edge.index()][face.index()] = opposite_edge;
            }
        }

        return internal_results;
    };

    static const auto internal_results = precompute();

    assert(is_sane());
    assert(!is_null());
    assert(is_adjacent(face));
    assert(face.is_adjacent(*this));

    const auto& result = internal_results[index()][face.index()];

    assert(result.is_sane());
    assert(!result.is_null());

    return result;
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
const corner_set_t&
edge_t::
corner_set() const
{
  static const corner_set_t internal_results[] = {
      corner_set_t(edge_t::get(0).corners())
    , corner_set_t(edge_t::get(1).corners())
    , corner_set_t(edge_t::get(2).corners())
    , corner_set_t(edge_t::get(3).corners())
    , corner_set_t(edge_t::get(4).corners())
    , corner_set_t(edge_t::get(5).corners())
    , corner_set_t(edge_t::get(6).corners())
    , corner_set_t(edge_t::get(7).corners())
    , corner_set_t(edge_t::get(8).corners())
    , corner_set_t(edge_t::get(9).corners())
    , corner_set_t(edge_t::get(10).corners())
    , corner_set_t(edge_t::get(11).corners())
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);

  return internal_results[ index() ];
}


CORNER_CASES_CUBEXX_INLINE
const std::array<edge_t,4>&
edge_t::
adjacent_edges() const
{
  
  typedef std::array<edge_t,4> result_type;
  auto precompute = [](){
    std::array< result_type, 12> internal_results;
    
    for (auto edge : edge_t::all())
    {
      std::size_t i = 0;
      for (auto corner : edge.corners())
      {
        assert(edge.is_adjacent(corner));
        assert(corner.is_adjacent(edge));
        
        for (auto direction : direction_t::all())
        {
          if (direction.axis() == edge.base_axis() || !direction.positive())
            continue;
          
          auto adjacent_edge = corner.edge(direction);
          
          assert(i < 4);
          internal_results[edge.index()][i++] = adjacent_edge;
        }
        
      }
      assert(i == 4);
    }
    
    return internal_results;
  };
  
  static const auto internal_results = precompute();
  
  assert(is_sane());
  assert(!is_null());
  
  
  
  
  const auto& result = internal_results[index()];
  for (auto edge : result)
  {
    assert(edge.is_sane());
    assert(!edge.is_null());
    assert(edge.is_adjacent(*this));
    assert((*this).is_adjacent(edge));
  }
  return result;
}


CORNER_CASES_CUBEXX_INLINE
const std::array<edge_t,2>&
edge_t::
adjacent_edges(const corner_t& corner) const
{
  typedef std::array<edge_t,2> result_type;
  auto precompute = [](){
    std::array< std::array<result_type, 8>, 12> internal_results;
    
    for (auto edge : edge_t::all())
    {
      for (auto corner : edge.corners())
      {
        assert(edge.is_adjacent(corner));
        assert(corner.is_adjacent(edge));
        
        std::size_t i = 0;
        for (auto direction : direction_t::all())
        {
          if (direction.axis() == edge.base_axis() || !direction.positive())
            continue;
          
          auto adjacent_edge = corner.edge(direction);
          
          assert(i < 2);
          internal_results[edge.index()][corner.index()][i++] = adjacent_edge;
        }
        
        assert(i == 2);
      }
    }
    
    return internal_results;
  };
  
  static const auto internal_results = precompute();
  
  assert(is_sane());
  assert(!is_null());
  assert(corner.is_sane());
  assert(!corner.is_null());
  
  
  assert(is_adjacent(corner));
  assert(corner.is_adjacent(*this));
  
  
  const auto& result = internal_results[index()][corner.index()];
  for (auto edge : result)
  {
    assert(edge.is_sane());
    assert(!edge.is_null());
    assert(edge.is_adjacent(*this));
    assert((*this).is_adjacent(edge));
    assert(edge.is_adjacent(corner));
    assert(corner.is_adjacent(edge));
  }
  return result;
}


CORNER_CASES_CUBEXX_INLINE
const edge_set_t&
edge_t::
adjacent_edge_set() const
{
  auto precompute = [](){
    std::array<edge_set_t, 12> internal_results;
    for (auto edge : edge_t::all())
    {
      internal_results[edge.index()] = edge_set_t(edge.adjacent_edges());
    }
    return internal_results;
  };
  static const auto internal_results = precompute();
  
  assert(is_sane());
  assert(!is_null());
  
  
  const auto& result = internal_results[index()];
  for (auto edge : result)
  {
    assert(edge.is_sane());
    assert(!edge.is_null());
    assert(edge.is_adjacent(*this));
    assert((*this).is_adjacent(edge));
  }
  return result;
}


CORNER_CASES_CUBEXX_INLINE
const edge_set_t&
edge_t::
adjacent_edge_set(const corner_t& corner) const
{
  auto precompute = [](){
    std::array<std::array<edge_set_t, 8>, 12> internal_results;
    for (auto edge : edge_t::all())
    {
      for (auto corner : edge.corners())
      {
        assert(corner.is_adjacent(edge));
        assert(edge.is_adjacent(corner));
        internal_results[edge.index()][corner.index()] = edge_set_t(edge.adjacent_edges(corner));
      }
    }
    return internal_results;
  };
  static const auto internal_results = precompute();
  
  assert(is_sane());
  assert(!is_null());
  assert(corner.is_sane());
  assert(!corner.is_null());
  
  assert(corner.is_adjacent(*this));
  assert((*this).is_adjacent(corner));

  const auto& result = internal_results[index()][corner.index()];
  for (auto edge : result)
  {
    assert(edge.is_sane());
    assert(!edge.is_null());
    assert(edge.is_adjacent(*this));
    assert((*this).is_adjacent(edge));
  }
  return result;
}


CORNER_CASES_CUBEXX_INLINE
const face_t&
edge_t::
calc_face(uint_fast8_t idx) const
{
    assert(idx < 2);


    int xyz[2][3] = {{0,0,0}, {0,0,0}};

    xyz[0][secondary_axis()] = project_secondary() ? +1 : -1;
    xyz[1][tertiary_axis()] = project_tertiary() ? +1 : -1;

    const face_t& result = face_t::get(direction_t::get(xyz[idx][0],xyz[idx][1],xyz[idx][2]));

    assert(result.is_sane());
    assert(!result.is_null());
    return result;
}

CORNER_CASES_CUBEXX_INLINE
const std::array<face_t, 2>&
edge_t::
faces() const
{
  static const std::array<face_t, 2> internal_results[] = {
      std::array<face_t, 2>{edge_t::get(0).calc_face(0), edge_t::get(0).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(1).calc_face(0), edge_t::get(1).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(2).calc_face(0), edge_t::get(2).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(3).calc_face(0), edge_t::get(3).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(4).calc_face(0), edge_t::get(4).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(5).calc_face(0), edge_t::get(5).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(6).calc_face(0), edge_t::get(6).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(7).calc_face(0), edge_t::get(7).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(8).calc_face(0), edge_t::get(8).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(9).calc_face(0), edge_t::get(9).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(10).calc_face(0), edge_t::get(10).calc_face(1)}
    , std::array<face_t, 2>{edge_t::get(11).calc_face(0), edge_t::get(11).calc_face(1)}
  };
  assert(!is_null());
  assert(is_sane());
  assert(index() < 12);

  return internal_results[ index() ];
}
CORNER_CASES_CUBEXX_INLINE
const face_set_t&
edge_t::
face_set() const
{
  auto precompute = []()
  {
    typedef face_set_t result_type;

    std::array<result_type, 12> internal_results;

    for (auto edge : edge_t::all())
    {
      internal_results[edge.index()] = face_set_t(edge.faces());
    }

    return internal_results;
  };

  static const auto internal_results = precompute();

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
operator&=(const derived_t& set)
{
  mbits &= set.mbits;

  return self();
}

template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator&=(const element_t& element)
{
  assert(element.index() < N);
  self() &= derived_t(element);
  return self();
}



template<typename derived_t, typename element_t, std::size_t N>
template<typename Sequence>
CORNER_CASES_CUBEXX_INLINE
derived_t&
set_base_t<derived_t, element_t, N>::
operator&=(const Sequence& sequence)
{


  self() &= derived_t(sequence);

  return self();
}





template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator&(const derived_t& set) const
{
  return derived_t(self()) &= set;
}


template<typename derived_t, typename element_t, std::size_t N>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator&(const element_t& element) const
{
  return derived_t(self()) &= element;
}

template<typename derived_t, typename element_t, std::size_t N>
template<typename Sequence>
CORNER_CASES_CUBEXX_INLINE
derived_t
set_base_t<derived_t, element_t, N>::
operator&(const Sequence& sequence) const
{
  return derived_t(self()) &= sequence;
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


CORNER_CASES_CUBEXX_INLINE
edge_set_t::edge_set_t()
  : base_t()
{

}

template<typename T>
CORNER_CASES_CUBEXX_INLINE
edge_set_t::edge_set_t(const T& v)
  : base_t(v)
{

}









//###################################################################




} // namespace cubexx
