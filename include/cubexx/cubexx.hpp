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
    : mset(NULL), mindex(value_type::SIZE)
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
    
    while (mindex < value_type::SIZE)
    {
      if (mset->contains(mindex))
      {
        break;
      }
      ++mindex;
    }
    
    assert(valid());
    assert(dereferencable() || mindex == value_type::SIZE);
    
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
      assert(mindex != value_type::SIZE);
      assert(mset->contains(mindex));
    assert(dereferencable());
    
    ++mindex;
      
    while (mindex < value_type::SIZE && !dereferencable())
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
      mindex <= value_type::SIZE &&
      ///If not an end iterator, mset should be set, and mindex should be contained in it
      (mindex == value_type::SIZE || mset->contains(mindex));
  }
  
  /**
   * @brief tests if an iterator is dereferencable; an iterator is dereferencable if
   *        it is valid (see valid()) and does *NOT* represent end().
   * @return 
   * @see valid()
   */
  CORNER_CASES_CUBEXX_INLINE
  bool dereferencable() const{
    return valid() && mindex != value_type::SIZE && mset->contains(mindex);
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













struct direction_t
{
  
  const face_t& face() const;
  const direction_t& opposite() const;
  std::array<direction_t, 4> adjacents() const;
  
  static const direction_t& get(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z);
  static const direction_t& get(const direction_t& direction);
  static const std::array<direction_t, 6>& all();
  
  std::int_fast8_t x() const;
  std::int_fast8_t y() const;
  std::int_fast8_t z() const;
  bool positive() const;
  
  std::uint_fast8_t index() const;
  static const direction_t& index(std::uint_fast8_t idx);
  bool operator<(const direction_t& other) const;
  bool operator==(const direction_t& other) const;
  bool operator!=(const direction_t& other) const;
  
  ///Default constructs a null direction_t
  direction_t();
  
  bool is_null() const;
  bool is_sane() const;
  static const direction_t& null_direction();
  static const std::size_t SIZE = 6;
private:
  std::bitset<3> mbits;
  
  
  direction_t(const std::bitset<3>& bits);
};

struct face_t{
  
  const direction_t& direction() const;
  const face_t& opposite() const;
  std::array<face_t, 4> adjacents() const;
  std::array<corner_t, 4> corners() const;
  corner_set_t corner_set() const;
  std::array<edge_t, 4> edges() const;
  
  static const std::array<face_t, 6>& all();
  static const face_t& get(const direction_t& direction);
  static const face_t& get(const std::uint_fast8_t& idx);
  
  
  std::uint_fast8_t index() const;
  static const direction_t& index(std::uint_fast8_t idx);
  bool operator<(const face_t& other) const;
  bool operator==(const face_t& other) const;
  bool operator!=(const face_t& other) const;
  static const std::size_t SIZE = 6;
  
protected:
  direction_t mdirection;
private:
  face_t();
  face_t(const direction_t& direction);
  
};

struct corner_t{
  typedef direction_t direction_type;
  typedef edge_t edge_type;
  typedef face_t face_type;
  
  
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
  std::array<corner_t, 3> adjacents() const;
  corner_set_t adjacents_set() const;
  bool is_adjacent(const corner_t& other) const;
  
  std::array<face_t, 3> faces() const;
  face_set_t face_set() const;
  
  std::array<edge_t, 3> edges() const;
  edge_t edge(const direction_t& direction);
  
  
  const corner_t& opposite() const;
  
  static const corner_t& get(std::uint_fast8_t i);
  static const corner_t& get(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z);
  static const corner_t& get(const corner_t& corner);
  static const corner_t& index(std::uint_fast8_t idx);
  
  std::uint_fast8_t index() const;
  bool operator<(const corner_t& other) const;
  bool operator==(const corner_t& other) const;
  bool operator!=(const corner_t& other) const;
  
  
  static const std::array<corner_t, 8>& all();
  
  ///return {-1,1} depending if the corner is in the near side or the far side of the x, respectively
  std::int_fast8_t x() const;
  ///return {-1,1} depending if the corner is in the near side or the far side of the y, respectively
  std::int_fast8_t y() const;
  ///return {-1,1} depending if the corner is in the near side or the far side of the z, respectively
  std::int_fast8_t z() const;
  
  ///return {0,1} depending if the corner is in the near side or the far side of the x, respectively
  std::uint_fast8_t ux() const;
  ///return {0,1} depending if the corner is in the near side or the far side of the y, respectively
  std::uint_fast8_t uy() const;
  ///return {0,1} depending if the corner is in the near side or the far side of the z, respectively
  std::uint_fast8_t uz() const;
  
  
  ///Number of corners
  static const std::size_t SIZE = 8;
  corner_t();
  
  bool is_null() const;
  static const corner_t& null_corner();
protected:
#ifndef NDEBUG
  bool mx, my, mz;
#endif
private:
  corner_t(std::int_fast8_t x, std::int_fast8_t y, std::int_fast8_t z);
  corner_t(const std::bitset<4>& bits);
  
  std::bitset<4> mbits;
};


struct edge_t{
  const std::array<corner_t, 2>& corners() const;
  std::array<edge_t, 4> adjacents() const;
  std::array<face_t, 2> faces() const;
  
  corner_set_t corner_set() const;
  
  static const std::array<edge_t, 12>& all();
  
  bool operator<(const edge_t& other) const;
  
private:
  edge_t(const corner_t& a, const corner_t& b);
  edge_t();
  
  std::array<corner_t, 2> mcorners;
};

struct cube_t{
  std::array<face_t, 6> faces() const;
  const std::array<corner_t, 8>& corners() const;
  std::array<edge_t, 12> edges() const;
  std::array<direction_t, 6> directions() const;
  
  static const cube_t& identity();
private:
  cube_t();
};


} // namespace cubexx

#include <cubexx/detail/cubexx.inl.hpp>


#endif // CORNER_CASES_CUBEXX_CUBEXX_HPP


