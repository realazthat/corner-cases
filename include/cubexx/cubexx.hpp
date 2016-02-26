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
#ifndef CORNER_CASES_CUBEXX_CUBEXX_HPP
#define CORNER_CASES_CUBEXX_CUBEXX_HPP

#include "cubexx/cubexx.fwd.hpp"

#include <bitset>
#include <cstdint>
#include <array>
#include <cassert>

#include <type_traits>
#include <cstddef>
#include <iterator>
#include <memory>


namespace cubexx{

#ifndef CORNER_CASES_CUBEXX_INLINE
#define CORNER_CASES_CUBEXX_INLINE inline
#endif

struct face_t;
struct face_set_t;
struct direction_t;
struct direction_set_t;
struct corner_t;
struct corner_set_t;
struct edge_t;
struct edge_set_t;
struct cube_t;


namespace detail{
CORNER_CASES_CUBEXX_INLINE
bool lxor(const bool& a, const bool& b)
{
  return !a != !b;
}

CORNER_CASES_CUBEXX_INLINE
bool liff(const bool& a, const bool& b)
{
  return ( (a && b) || (!a && !b) );
}


CORNER_CASES_CUBEXX_INLINE
bool lif(const bool& a, const bool& b)
{
  return (!a || b);
}

CORNER_CASES_CUBEXX_INLINE
bool lnand(const bool& a, const bool& b)
{
  return !(a && b);
}


template<std::size_t N>
CORNER_CASES_CUBEXX_INLINE
bool has_exactly_one_bit(const std::bitset<N>& v)
{
  return v.count() == 1;
}


template<typename T>
CORNER_CASES_CUBEXX_INLINE
T clamp(const T& v, const T& a, const T& b)
{
    return std::max(std::min(v, b), a);
}

template<typename T>
CORNER_CASES_CUBEXX_INLINE
T safemod(const T& v, const T& modulo)
{
    assert(false);
}


template<typename T>
CORNER_CASES_CUBEXX_INLINE
T wrap_around_2(const T& v)
{
    return v > 1 ? -1 : (v < -1 ? +1 : v);
}


} // namespace detail


/**
 * @class const_element_set_iterator_t
 * @brief An iterator for the set_base_t set; as an example this would be what is
 *          returned from corner_set_t::begin().
 * 
 * @param value_type
 *          The type of the value that the iterator returns when dereferences; as an example
 *          for the corner_set_t, this would return a const cubexx::corner_t.
 * @param set_type
 *          The type of the set that this is an iterator for; as an example, for the iterator
 *          returned by corner_set_t::begin(), this would be of type corner_set_t.
 * 
 * Note that changing the underlying set can invalidate the iterator.
 *
 * Note that there are few states an iterator can be in.
 * * Default constructed
 *   * Not associated with any container.
 *   * Only the assignment operation is valid.
 * * Valid
 *   * Associated with a container
 *   * In this state, an iterator is either also "dereferencable" or is the end() iterator
 * * Invalid
 *   * Associated with a container
 *   * Does not represent any item in the container, and cannot be used, it must be discarded. Using such
 *      an iterator (other than assignment operations) leads to undefined behaviour.
 *   * In this state, the iterator is certainly not "dereferencable".
 *   * Such an iterator can result from changing the underlying set after obtaining the iterator.
 * * Dereferencable
 *   * Associated with a container
 *   * Valid
 *   * The iterator is *not* equivalent to end().
 *   * In this state the iterator represents an item in the container, and the item may be retrieved.
 */
template<typename value_type, typename set_type>
struct const_element_set_iterator_t
  : public std::iterator<std::forward_iterator_tag,
                                  value_type, std::ptrdiff_t, const value_type*,
                                  const value_type&>
{
private:

  template<typename V, typename S>
  friend struct const_element_set_iterator_t;
  
  ///we will use this later for enable_if (google SFINAE), see below
  struct enabler {};  // a private type avoids misuse
    public:

  /**
   * @brief Default constructor; it is not a "valid" iterator, because it is not associated
   *        with a particular set.
   */
  CORNER_CASES_CUBEXX_INLINE
  const_element_set_iterator_t()
    : mset(NULL), mindex(value_type::SIZE())
  {
    
  }
  
  
  /**
   * @brief Constructor, associates the iterator with a set; it finds the first
   *            valid item in the set, and represents that item; otherwise it
   *            represents set_type_t::end().
   * @param set the set this iterator is to be associated with.
   * @return A valid iterator, though not necessarily dereferencable if the
   *        set is empty, as it will be equal to set_type_t::end().
   */
  CORNER_CASES_CUBEXX_INLINE
  const_element_set_iterator_t(set_type* set)
    : mset(set), mindex(0)
  {
    
    while (mindex < value_type::SIZE())
    {
      if (mset->contains(mindex))
      {
        break;
      }
      ++mindex;
    }
    
    assert(valid());
    assert(dereferencable() || mindex == value_type::SIZE());
    
  }

  /**
   * @brief Constructor, with explicit index; this allows the caller to
   *        set the iterator to point to a specific bit in the underlying bitset representation
   *        of the set. Alternatively, it can point to `SIZE` of the bitset, which indicates a
   *        non-dereferencable iterator (end()). Note that if index != SIZE, this bit *must* be set;
   *        in other words, the set *must* contain this item.
   * @param set the set this iterator is to be associated with.
   * @param index the specific bit in the underlying bitset representation that this iterator will represent,
   *        or be equal to `SIZE`, the size of the underlying bitset, to represent and end() iterator.
   * @return An iterator pointing at the item represented by index.
   */
  CORNER_CASES_CUBEXX_INLINE
  const_element_set_iterator_t(set_type* set, std::size_t index)
    : mset(set), mindex(index)
  {
    assert(valid());
  }
  
  /**
   * @brief Copy constructor, or conversion from a non-const version of this iterator to a const one.
   * @param other the other iterator
   * @return A copy of the iterator.
   */
  template <class OtherValue, class OtherSet>
  CORNER_CASES_CUBEXX_INLINE
  const_element_set_iterator_t(
      const const_element_set_iterator_t<OtherValue, OtherSet>& other
    )
    : mset(other.mset), mindex(other.mindex)
  {
    
    static_assert( std::is_convertible< OtherValue*,value_type >::value
                , "Converting iterator with non-convertible value type;"
                  " prolly trying to convert a const iterator with a const value type to a non-const iterator" );
    static_assert( std::is_convertible< OtherSet*,set_type >::value
                , "Converting iterator with non-convertible container type;"
                  " prolly trying to convert a const iterator with a const value type to a non-const iterator" );
    
  }
  /*
  template <class OtherValue, class OtherSet>
  CORNER_CASES_CUBEXX_INLINE
  const_element_set_iterator_t(
      const const_element_set_iterator_t<OtherValue, OtherSet>& other
    , typename std::enable_if< std::is_convertible<OtherValue*,value_type*>::value, enabler>::type = enabler()
    )
    : mset(other.mset), mindex(other.mindex)
  {
    
    
  }
   */

  /**
   * @brief Assignment.
   * @param other the other iterator
   * 
   * After being assigned, this iterator will point to the same set, and the same item in the set as
   * @p other.
   */
  template<typename T>
  CORNER_CASES_CUBEXX_INLINE
  const_element_set_iterator_t& operator=(const T& other)
  {
    assign(other);
    return *this;
  }

  /**
   * @brief Iterator dereference. Note, calling this on end() or an invalid iterator is an error (undefined behavior).
   * @return The value represented.
   */
  CORNER_CASES_CUBEXX_INLINE
  const value_type& operator*() const {
    assert(valid());
    assert(dereferencable());
    return value_type::get(mindex);
  }
  
  /**
   * @brief Iterator dereference. Note, calling this on end() or an invalid iterator is an error (undefined behavior).
   * @return The value represented.
   */
  CORNER_CASES_CUBEXX_INLINE
  const value_type* operator->() const {
    assert(valid());
    assert(dereferencable());
    return &value_type::get(mindex);
  }
  
  
  
  /**
   * @brief Equality test; tests that the other iterator points to the same set, and the same item in the set
   *            (or if they are both end() of the same set). Note that invalid iterators are never equal.
   * @param other compare this to other.
   * @return true if they are equal.
   */
  template <class OtherValue, class OtherSet>
  CORNER_CASES_CUBEXX_INLINE
  bool operator==(const const_element_set_iterator_t<OtherValue, OtherSet>& other) const
  {
    return
      ///Invalid iterators are never equal.
      this->mset != nullptr
      ///The two iterators are referring to the same set
      && (this->mset == other.mset
        ///Or one of them is a default constructed iterator
        || ( this->mset == NULL || other.mset == NULL ))
      ///And the indices are equal
      && this->mindex == other.mindex;
  }
  
  /**
   * @brief Inequality test.
   * @param other compare this to other.
   * @return true if they are not equal
   * @see operator==()
   */
  template <class OtherValue, class OtherSet>
  CORNER_CASES_CUBEXX_INLINE
  bool operator!=(const const_element_set_iterator_t<OtherValue, OtherSet>& other) const
  {
    return !(*this == other);
  }
  
  /**
   * @brief Regular iterator pre-increment.
   */
  const_element_set_iterator_t& operator++() {
    increment();
    return *this;
  }
  
  /**
   * @brief Regular iterator post-increment.
   */
  const_element_set_iterator_t operator++(int) {
    const_element_set_iterator_t result(*this);
    increment();
    return result;
  }
  
private:
  /**
   * @brief internal self increment.
   */
  CORNER_CASES_CUBEXX_INLINE
  void increment() {
    assert(valid());
      assert(mset);
      assert(mindex != value_type::SIZE());
      assert(mset->contains(mindex));
    assert(dereferencable());
    
    ++mindex;
      
    while (mindex < value_type::SIZE() && !dereferencable())
      ++mindex;
    
    assert(valid());
  }
  
private:
  /**
   * @brief tests if an iterator is valid; an iterator is valid if it is associated with a set,
   *        and {represents an item in the set, or represents the end() iterator}.
   * @see dereferencable()
   */
  CORNER_CASES_CUBEXX_INLINE
  bool valid() const{
    return
      mset != nullptr &&
      mindex <= value_type::SIZE() &&
      ///If not an end iterator, mset should be set, and mindex should be contained in it
      (mindex == value_type::SIZE() || mset->contains(mindex));
  }
  
  /**
   * @brief tests if an iterator is dereferencable; an iterator is dereferencable if
   *        it is valid (see valid()) and does *NOT* represent end().
   * @return 
   * @see valid()
   */
  CORNER_CASES_CUBEXX_INLINE
  bool dereferencable() const{
    return valid() && mindex != value_type::SIZE() && mset->contains(mindex);
  }
  
  /**
   * @brief Internal assignment.
   * @param other the iterator to assign from.
   */
  template <class OtherValue, class OtherSet>
  CORNER_CASES_CUBEXX_INLINE
  void
  assign(const_element_set_iterator_t<OtherValue, OtherSet> const& other,
            typename std::enable_if< std::is_convertible<OtherValue*,value_type*>::value, enabler >::type = enabler() )
  {
    mset = other.mset;
    mindex = other.mindex;
  }
  
  
  /**
   * Pointer to the representative set or nullptr if defaultconstructed.
   */
  set_type* mset;
  /**
   * Position of the iterator within the bitset/container.
   */
  std::size_t mindex;
  
};





template<typename derived_t, typename element_t, std::size_t N>
struct set_base_t
{
  typedef element_t value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  
  typedef const_element_set_iterator_t<value_type const, derived_t const> const_iterator;
  typedef const_iterator iterator;
  
  const_iterator begin() const;
  const_iterator end() const;
  
  
  template<typename Sequence>
  set_base_t(const Sequence& sequence);
  set_base_t(const derived_t& set);
  set_base_t();
  
  template<typename Sequence>
  derived_t& operator=(const Sequence& sequence);
  derived_t& operator=(const element_t& element);
  derived_t& operator=(const derived_t& set);
  
  template<typename Sequence>
  derived_t& operator|=(const Sequence& sequence);
  derived_t& operator|=(const derived_t& set);
  derived_t& operator|=(const element_t& element);
  
  template<typename Sequence>
  derived_t operator|(const Sequence& sequence) const;
  derived_t operator|(const derived_t& set) const;
  derived_t operator|(const element_t& element) const;
  
  template<typename Sequence>
  derived_t& operator-=(const Sequence& sequence);
  derived_t& operator-=(const derived_t& set);
  derived_t& operator-=(const element_t& element);
  
  template<typename Sequence>
  derived_t operator-(const Sequence& sequence) const;
  derived_t operator-(const derived_t& set) const;
  derived_t operator-(const element_t& element) const;
  
  bool contains(const element_t& element) const;
  bool contains(const std::size_t& idx) const;
  
  std::size_t size() const;
  
  void clear();
  
  bool operator==(const derived_t& other) const;
  bool operator!=(const derived_t& other) const;
  
  const std::bitset<N>& bits() const;
private:
  
  
  derived_t& self();
  const derived_t& self() const;
  
  std::bitset<N> mbits;
};


struct corner_set_t : public set_base_t<corner_set_t, corner_t, 8>
{
  typedef corner_set_t self_t;
  typedef set_base_t<corner_set_t, corner_t, 8> base_t;
  
  template<typename T>
  corner_set_t(const T& v);
  corner_set_t();
  
};

struct face_set_t : public set_base_t<face_set_t, face_t, 6>
{
  typedef face_set_t self_t;
  typedef set_base_t<face_set_t, face_t, 6> base_t;
  
  template<typename T>
  face_set_t(const T& v);
  face_set_t();
  
};


struct direction_set_t : public set_base_t<direction_set_t, direction_t, 6>
{
  typedef direction_set_t self_t;
  typedef set_base_t<direction_set_t, direction_t, 6> base_t;
  
  template<typename T>
  direction_set_t(const T& v);
  direction_set_t();
  
};









/**
 * Represents an direction of a cube. There are 6 directions in a cube.
 * A direction_t is 1:1 equivalent to a face_t; they can be converted from one to the other
 * via direction_t::face() and face_t::direction().
 *
 * @see face_t
 */
struct direction_t
{
  ///Default constructs a "null" direction_t.
  direction_t();
  
  ///Returns the equivalent face
  const face_t& face() const;
  ///Returns the opposite direction
  const direction_t& opposite() const;
  ///Returns the four adjacent directions
  std::array<direction_t, 4> adjacents() const;
  
  ///Returns a direction from a vector of 3 integer components; two of them must be 0, the third must be positive or negative
  /// in the dimension of the direction. For example, (0,0,+1) means a direction on the z-axis pointing along
  /// the positive length of the axis. While (-1,0,0) means a direction on the (negative) x-axis pointing toward
  /// the negative asymptote of the x-axis.
  static const direction_t& get(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z);
  ///Returns a direction given a direction.
  static const direction_t& get(const direction_t& direction);
  ///Return the direction represented by an index.
  ///@see index()
  static const direction_t& get(std::uint_fast8_t idx);
  ///Return a list of all the directions on the cube.
  static const std::array<direction_t, 6>& all();
  
  ///Return the x component of the direction vector. See
  /// get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t) for more information.
  ///@see get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t)
  std::int_fast8_t x() const;
  ///Return the y component of the direction vector. See
  /// get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t) for more information.
  ///@see get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t)
  std::int_fast8_t y() const;
  ///Return the z component of the direction vector. See
  /// get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t) for more information.
  ///@see get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t)
  std::int_fast8_t z() const;
  ///Returns true if the direction vector is positive; false if it is negative. See
  /// get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t) for more information.
  ///@see get(std::int_fast8_t,std::int_fast8_t,std::int_fast8_t)
  bool positive() const;
  
  
  ///Retrieve a numeric 0-based index for the direction, for use in indexing when storing in an array, or for comparison
  /// when storing in a container requiring comparison. The index will be less than SIZE(). The "null" direction
  /// has no index, and it is illegal to call index() on it.
  ///@see get(std::uint_fast8_t idx)
  std::uint_fast8_t index() const;
  ///Comparison
  bool operator<(const direction_t& other) const;
  ///Comparison
  bool operator==(const direction_t& other) const;
  ///Comparison
  bool operator!=(const direction_t& other) const;
  
  
  ///Returns true if this direction is a null direction_t.
  bool is_null() const;
  
  bool is_sane() const;
  
  ///Returns a null direction.
  static const direction_t& null_direction();
  
  ///Number of directions.
  CORNER_CASES_CUBEXX_INLINE static std::size_t SIZE(){ return 6; }
private:
  std::bitset<3> mbits;
  
  
  direction_t(const std::bitset<3>& bits);
};

/**
 * Represents an face of a cube. There are 6 faces in a cube.
 * A face_t is 1:1 equivalent to a direction_t; they can be converted from one to the other
 * via face_t::direction() and direction_t::face().
 *
 * @see direction_t
 */
struct face_t{
  ///Default construct a "null" face.
  face_t();
  
  ///Return the direction that is equivalent to this face.
  const direction_t& direction() const;
  ///Return the opposite face.
  const face_t& opposite() const;
  ///Return the 4 adjacent faces
  std::array<face_t, 4> adjacents() const;
  ///Return the 4 corners on this face.
  std::array<corner_t, 4> corners() const;
  ///Return a corner_set_t containing the 4 corners on this face.
  corner_set_t corner_set() const;
  ///Return the 4 edges on this face
  std::array<edge_t, 4> edges() const;
  
  ///Return a list of all the faces on the cube.
  static const std::array<face_t, 6>& all();
  ///Return the face equivalent to a specified direction
  static const face_t& get(const direction_t& direction);
  ///Return the face represented by an index.
  ///@see index()
  static const face_t& get(const std::uint_fast8_t& idx);
  
  ///Retrieve a numeric 0-based index for the face, for use in indexing when storing in an array, or for comparison
  /// when storing in a container requiring comparison. The index will be less than SIZE(). The "null" face
  /// has no index, and it is illegal to call index() on it.
  ///@see get(std::uint_fast8_t idx)
  std::uint_fast8_t index() const;
  
  ///Comparison
  bool operator<(const face_t& other) const;
  ///Comparison
  bool operator==(const face_t& other) const;
  ///Comparison
  bool operator!=(const face_t& other) const;
  
  ///Returns true if this face is a null face_t.
  bool is_null() const;
  
  bool is_sane() const;
  
  ///Returns a null direction.
  static const face_t& null_face();
  
  ///Number of faces.
  CORNER_CASES_CUBEXX_INLINE static std::size_t SIZE(){ return 6; }
  
protected:
  direction_t mdirection;
private:
  face_t(const direction_t& direction);
};

/**
 * Represents an corner of a cube. There are 8 corners in a cube.
 */
struct corner_t{
  ///Default construct a "null" corner.
  corner_t();
  
  /**
   * Returns a corner that is adjacent in the axis of the direction.
   * Specifically, it will wrap around the cube if there is no corner
   * in the specified direction
   *
   * @see corner_t::push()
   */
  const corner_t& adjacent(const direction_t& direction) const;
  /**
   * Returns a corner that is the next corner in the specified direction.
   * Specifically, it will **NOT** wrap around the cube if there is no corner
   * in the specified direction; but instead return the same corner.
   *
   * @see corner_t::adjacent()
   */
  const corner_t& push(const direction_t& direction) const;
  ///Returns a list of 3 adjacent corners to this corner.
  std::array<corner_t, 3> adjacents() const;
  ///Returns a set of 3 adjacent corners to this corner.
  corner_set_t adjacents_set() const;
  ///Returns true if @param other is a corner adjacent to this corner; otherwise returns false.
  bool is_adjacent(const corner_t& other) const;
  
  ///Returns the 3 adjacent faces to this corner.
  std::array<face_t, 3> faces() const;
  ///Returns a set of the 3 adjacent faces to this corner.
  face_set_t face_set() const;
  
  ///Returns a set of the 3 adjacent edges to this corner.
  std::array<edge_t, 3> edges() const;
  ///Returns the edge between this corner and an adjacent corner, specified via the @param direction parameter.
  edge_t edge(const direction_t& direction);
  
  ///Returns the opposite corner across the cube.
  const corner_t& opposite() const;
  
  ///Returns a corner that is represented by the specified index, @param idx.
  static const corner_t& get(std::uint_fast8_t idx);
  /**
   * Returns a corner that is represented by a vector of 3 components.
   *
   * There are two representations that fit in well with the same function.
   * 
   * The first, which we be called "origin-centered-cube-representation" is as follows:
   *
   * Cube coordinates are in the range `[-1,+1]`, and specfically can each take one of the two values in `{-1,+1}`.
   * The lower corner is represented as `(-1,-1,-1)`. The other corners are offset away from the
   * corner by `+2`, so  they look like `(+1,-1,-1)` or `(+1,+1,+1)` (for the far corner), and so on.
   * The coordinates of this representation can be retrieved via the corner_t::x(), corner_t::y(),
   * and corner_t::z() functions.
   *
   * The alternative representation, which will called "unit-cube-representation", (or "unsigned") is as follows:
   *
   * Cube coordinates are in the range `[0,1]` and specifically can each take one of the two values in `{0,1}`
   * The lower corner is represented as `(0,0,0)`. The other corners are offset away from the lower corner by `+1`,
   * so they look like `(1,0,0)` or `(1,1,1)` (for the far corner), and so on.
   * The coordinates of this representation can be retrieved via the corner_t::ux(), corner_t::uy(),
   * and corner_t::uz() functions.
   *
   * This function can construct the corner_t with either reprentation. The rule is as follows:
   * if a coordinate is positive, it will result in being +1; if it is zero or negative, it will be lower coordinate; `-1` in the
   * "origin-centered-cube-representation" or `0` in "unit-cube-representation".
   *
   * @see corner_t::x(), corner_t::y(), and corner_t::z(), corner_t::ux(), corner_t::uy(), and corner_t::uz()
   */
  static const corner_t& get(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z);
  ///Returns a corner, given a corner.
  static const corner_t& get(const corner_t& corner);
  
  ///Retrieve a numeric 0-based index for the corner, for use in indexing when storing in an array, or for comparison
  /// when storing in a container requiring comparison. The index will be less than SIZE(). The "null" corner
  /// has no index, and it is illegal to call index() on it.
  ///@see get(std::uint_fast8_t idx)
  std::uint_fast8_t index() const;
  ///Comparison
  bool operator<(const corner_t& other) const;
  ///Comparison
  bool operator==(const corner_t& other) const;
  ///Comparison
  bool operator!=(const corner_t& other) const;
  
  
  ///Return a list of all the faces on the cube.
  static const std::array<corner_t, 8>& all();
  
  ///Return an integer in {-1,1} depending if the corner is in the near side or the far side of the x, respectively
  std::int_fast8_t x() const;
  ///Return an integer in {-1,1} depending if the corner is in the near side or the far side of the y, respectively
  std::int_fast8_t y() const;
  ///Return an integer in {-1,1} depending if the corner is in the near side or the far side of the z, respectively
  std::int_fast8_t z() const;
  
  ///Return an unsigned integer in {0,1} depending if the corner is in the near side or the far side of the x, respectively
  std::uint_fast8_t ux() const;
  ///Return an unsigned integer in {0,1} depending if the corner is in the near side or the far side of the y, respectively
  std::uint_fast8_t uy() const;
  ///Return an unsigned integer in {0,1} depending if the corner is in the near side or the far side of the z, respectively
  std::uint_fast8_t uz() const;
  
  
  ///Number of corners.
  CORNER_CASES_CUBEXX_INLINE static std::size_t SIZE(){ return 8; }
  
  ///Returns true if this corner is null; false otherwise.
  bool is_null() const;
  bool is_sane() const;
  
  ///Retrieve the "null" corner.
  static const corner_t& null_corner();
private:
  corner_t(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z);
  corner_t(const std::bitset<4>& bits);
  
  std::bitset<4> mbits;
};


/**
 * Represents an edge of a cube. There are 12 edges in a cube.
 *
 */
struct edge_t{
  ///Default construct a "null" edge.
  edge_t();
  
  /**
   * Retrieve a list of (2) corners associated with this edge.
   */
  const std::array<corner_t, 2>& corners() const;
  
  ///Get the lower corner of this edge. Note that "lower" here
  /// means the one closer to the origin.
  ///@see corner1()
  const corner_t& corner0() const;
  ///Get the upper corner of this edge. Note that "upper" here
  /// means the one farther from the origin.
  ///@see corner0()
  const corner_t& corner1() const;
  
  /**
   * Retrieve the set of corners associated with this edge.
   */
  const corner_set_t& corner_set() const;
  
  /**
   * Retrieve a list of (4) edges that are adjacent with this edge.
   *
   */
  const std::array<edge_t, 4>& adjacents() const;
  
  ///Retrieve a list of (2) faces that are adjacent with this edge.
  const std::array<face_t, 2>& faces() const;
  
  
  /**
   * Retrieve a list of (2) adjacent edges associated with a particular corner of the edge.
   */
  const std::array<edge_t, 2>& adjacents(const corner_t& corner) const;
  
  ///Retrieve a list of all (12) edges on the cube.
  static const std::array<edge_t, 12>& all();
  
  
  
  ///Retrieve the edge on the opposite side of the cube.
  const edge_t& opposite() const;
  
  
  
  ///Test if an edge is a "null" edge.
  bool is_null() const;
  
  bool is_sane() const;
  
  ///Retrieve the "null" edge.
  static const edge_t& null_edge();
  
  /**
   * Retrieve an edge by its index.
   * 
   * @see index()
   */
  static const edge_t& get(std::uint_fast8_t idx);
  
  /**
   * Retrieves an edge by the following descriptive logic:
   *
   * First, choose a base-axis, the X axis, the Y axis or the Z axis (0,1, or 2 respectively).
   *
   * Second, let us define the "secondary-axis" as the first non-base-axis in (X,Y,Z), and the "tertiary-axis"
   * as the remaining non-base-axis.
   *
   * Finally, you can "project" the axis across the cube in the direction of the secondary or tertiary axes.
   *
   * There are four combinations of projection: project none, project the secondary only, project the tertiary only,
   * and project it across both, which would give an edge on the far side of the cube.
   *
   * @see base_axis(), secondary_axis(), tertiary_axis(), project_secondary(), project_tertiary()
   */
  static const edge_t& get(std::uint_fast8_t axis, bool project_secondary, bool project_tertiary);
  /**
   * Retrieves an edge via the two corners. Note, the corners specified here may be in any order
   * so long as they are adjacent; however the edge canonicalizes the order so that calls to 
   * corner0() and corner1() may not match the two corners used to obtain the edge here.
   *
   * @see corner0(), corner1()
   */
  static const edge_t& get(const corner_t& corner0, const corner_t& corner1);
  
  ///Returns the "base-axis" (in `{0,1,2}` for the `x,y,z` axis repsectively) that this edge is aligned with this edge.
  ///@see secondary_axis(), tertiary_axis(), get(std::uint_fast8_t,bool,bool)
  std::uint_fast8_t base_axis() const;
  ///Returns the "secondary-axis" (in `{0,1,2}` for the `x,y,z` axis repsectively); the "secondary-axis" is the
  ///next axis (modular increment) from the "base-axis".
  ///@see base_axis(), tertiary_axis(), get(std::uint_fast8_t,bool,bool)
  std::uint_fast8_t secondary_axis() const;
  ///Returns the "tertiary-axis" (in `{0,1,2}` for the `x,y,z` axis repsectively); the "tertiary-axis" is the
  ///next axis (modular increment) from the "secondary-axis".
  ///@see base_axis(), secondary_axis(), get(std::uint_fast8_t,bool,bool)
  std::uint_fast8_t tertiary_axis() const;
  
  ///Returns true if this edge is closer to or farther from the lower (-1,-1,-1) corner in the cube, specifically
  /// compared with the other parallel edge along the "secondary-axis".
  ///@see base_axis(), secondary_axis(), get(std::uint_fast8_t,bool,bool)
  bool project_secondary() const;
  ///Returns true if this edge is closer to or farther from the lower (-1,-1,-1) corner in the cube, specifically
  /// compared with the other parallel edge along the "tertiary-axis".
  ///@see base_axis(), tertiary_axis(), get(std::uint_fast8_t,bool,bool)
  bool project_tertiary() const;
  
  ///Retrieve a numeric 0-based index for the edge, for use in indexing when storing in an array, or for comparison
  /// when storing in a container requiring comparison. The index will be less than SIZE(). The "null" edge
  /// has no index, and it is illegal to call index() on it.
  ///@see get(std::uint_fast8_t idx)
  std::uint_fast8_t index() const;
  ///A comparison operator for using the edge in an std::set-like container.
  bool operator<(const edge_t& other) const;
  
  ///Equality operator, can be used between null edge and non-null-edges etc. Two null edges are equal.
  bool operator==(const edge_t& other) const;
  ///Inequality operator, can be used between null edge and non-null-edges etc. Two null edges are equal.
  bool operator!=(const edge_t& other) const;
  
  ///Number of edges.
  CORNER_CASES_CUBEXX_INLINE static std::size_t SIZE() { return 12; }
private:
  ///internal ctor
  edge_t(const std::bitset<4>& bits);
  
  /**
   * Representation:
   *
   * `edge description possibilities = {base-axis-x, base-axis-y, base-axis-z, null}*{4 projections}`
   *
   * `projections possibilities = {no projection, project axis a, project axis b, project both axis a and axis b}
   *
   * `axis a,b` are the remaining 2 axis for the `base-axis` of the edge.
   *
   * bit packing as follows:
   * 
   * ```
   * 
   * mbits := 0bBBTS
   * 
   * BB    := the "base-axis"; 00 is x, 01 is y, 10 is z, and 11 is used for null.
   * 
   * T     := bit to indicate if "*t*ertiary-axis" is projected.
   * 
   * P     := bit to indicate if "*s*econdary-axis" is projected.
   * ```
   * Some examples: 0b0010, means the edge on the x-axis, projected along the z-axis (which is tertiary when the "base-axis" is x).
   
   */
  std::bitset<4> mbits;
  
  
  const corner_t& calc_corner0() const;
  const corner_t& calc_corner1() const;
  const edge_t& calc_opposite() const;
};



} // namespace cubexx

#include <cubexx/detail/cubexx.inl.hpp>


#endif // CORNER_CASES_CUBEXX_CUBEXX_HPP


