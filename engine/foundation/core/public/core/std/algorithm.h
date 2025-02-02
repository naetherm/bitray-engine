////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/std/functional.h"
#include "core/std/iterator.h"
#include "core/std/typetraits/conditional.h"
#include "core/std/typetraits/add_reference.h"
#include "core/std/typetraits/is_scalar.h"
#include <algorithm>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


using ::floorf;
using ::ceilf;
using ::roundf;
using ::rintf;


namespace Internal {
// equal helper functions
template<class InputIterator1, class InputIterator2, class BinaryPredicate>
constexpr bool equal_helper(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binaryPredicate)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    if (!binaryPredicate(*first1, *first2))
    {
      return false;
    }
  }

  return true;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
constexpr bool equal_helper(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2,
                            BinaryPredicate binaryPredicate, core::InputIteratorTag, core::InputIteratorTag)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2)
  {
    if (!binaryPredicate(*first1, *first2))
    {
      return false;
    }
  }

  return first1 == last1 && first2 == last2;
}
template<class InputIterator1, class InputIterator2, class BinaryPredicate>
constexpr bool equal_helper(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2,
                            BinaryPredicate binaryPredicate, core::RandomAccessIteratorTag, core::RandomAccessIteratorTag)
{
  if (core::distance(first1, last1) != core::distance(first2, last2))
  {
    return false;
  }

  return equal_helper<InputIterator1, InputIterator2, core::add_lvalue_reference_t<BinaryPredicate>>(first1, last1, first2, binaryPredicate);
}


template <class RandomAccessIterator, class Distance, class T>
constexpr void push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, const T& value)
{
  Distance parent = (holeIndex - 1) / 2;
  while (holeIndex > topIndex && *(first + parent) < value)
  {
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
constexpr void push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, const T& value, Compare comp)
{
  Distance parent = (holeIndex - 1) / 2;
  while (holeIndex > topIndex && comp(*(first + parent), value))
  {
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Distance, class T>
constexpr void adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance length, const T& value)
{
  Distance topIndex = holeIndex;
  Distance secondChild = 2 * holeIndex + 2;
  while (secondChild < length)
  {
    if (*(first + secondChild) < *(first + (secondChild - 1)))
    {
      --secondChild;
    }
    *(first + holeIndex) = *(first + secondChild);
    holeIndex = secondChild;
    secondChild = 2 * (secondChild + 1);
  }
  if (secondChild == length)
  {
    *(first + holeIndex) = *(first + (secondChild - 1));
    holeIndex = secondChild - 1;
  }
  core::Internal::push_heap(first, holeIndex, topIndex, value);
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
constexpr void adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance length, const T& value, Compare comp)
{
  Distance topIndex = holeIndex;
  Distance secondChild = 2 * holeIndex + 2;
  while (secondChild < length)
  {
    if (comp(*(first + secondChild), *(first + (secondChild - 1))))
    {
      --secondChild;
    }
    *(first + holeIndex) = *(first + secondChild);
    holeIndex = secondChild;
    secondChild = 2 * (secondChild + 1);
  }
  if (secondChild == length)
  {
    *(first + holeIndex) = *(first + (secondChild - 1));
    holeIndex = secondChild - 1;
  }
  core::Internal::push_heap(first, holeIndex, topIndex, value, comp);
}


template<bool TIsScalar>
struct FillHelper {
  template<typename TIterator, typename TType>
  static void fill(TIterator begin, TIterator end, const TType& value) {
    for (; begin != end; ++begin) {
      *begin = value;
    }
  }
};
template<>
struct FillHelper<true> {
  template<typename TIterator, typename TType>
  static void fill(TIterator begin, TIterator end, const TType& value) {
    typedef typename core::IteratorTraits<TIterator>::ValueType ValueType;
    for (const TType temp = value; begin != end; ++begin) {
      *begin = static_cast<ValueType>(temp);
    }
  }
};

} // namespace Internal


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<class T>
constexpr T GetMin(const T& l, const T& r) {
  return (l < r) ? l : r;
}
template<class T>
constexpr T min(const T& l, const T& r) {
  return (l < r) ? l : r;
}

template<class T>
constexpr T GetMax(const T& l, const T& r) {
  return (l > r) ? l : r;
}
template<class T>
constexpr T max(const T& l, const T& r) {
  return (l > r) ? l : r;
}


// Find
template<class TInputIterator, class TComparableToIteratorValue>
constexpr TInputIterator find(TInputIterator first, TInputIterator last, const TComparableToIteratorValue& value) {
  for (; first != last; ++first) {
    if (*first == value) {
      break;
    }
  }

  return first;
}
template<class TInputIterator, class TPredicate>
constexpr TInputIterator find_if(TInputIterator first, TInputIterator last, TPredicate pred) {
  for (; first != last; ++first) {
    if (!pred(*first)) {
      break;
    }
  }

  return first;
}


// equal
template<class InputIterator1, class InputIterator2>
constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
  return Internal::equal_helper(first1, last1, first2, core::equal_to<>{});
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binaryPredicate)
{
  return Internal::equal_helper<InputIterator1, InputIterator2, core::add_lvalue_reference_t<BinaryPredicate>>(first1, last1, first2, binaryPredicate);
}

template<class InputIterator1, class InputIterator2>
constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
  return Internal::equal_helper(
    first1, last1, first2, last2, core::equal_to<>{},
    typename core::IteratorTraits<InputIterator1>::IteratorCategory{},
    typename core::IteratorTraits<InputIterator2>::IteratorCategory{});
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate binaryPredicate)
{
  return Internal::equal_helper<InputIterator1, InputIterator2, core::add_lvalue_reference_t<BinaryPredicate>>(
    first1, last1, first2, last2, binaryPredicate,
      typename core::IteratorTraits<InputIterator1>::IteratorCategory{},
      typename core::IteratorTraits<InputIterator2>::IteratorCategory{});
}


using std::equal_range;

// TEMPLATE FUNCTION lower_bound
template<class ForwardIterator, class T>
constexpr ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value)
{
  // find first element not before value, using operator<
  typename IteratorTraits<ForwardIterator>::DifferenceType count = core::distance(first, last);
  typename IteratorTraits<ForwardIterator>::DifferenceType count2{};

  for (; 0 < count; )
  {
    // divide and conquer, find half that contains answer
    count2 = count / 2;
    ForwardIterator mid = first;
    core::advance(mid, count2);

    if (*mid < value)
    {
      first = ++mid;
      count -= count2 + 1;
    }
    else
    {
      count = count2;
    }
  }
  return first;
}

template<class ForwardIterator, class T, typename Compare>
constexpr ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
{
  // find first element not before value, using compare predicate
  typename IteratorTraits<ForwardIterator>::DifferenceType count = core::distance(first, last);
  typename IteratorTraits<ForwardIterator>::DifferenceType count2{};

  for (; 0 < count; )
  {
    // divide and conquer, find half that contains answer
    count2 = count / 2;
    ForwardIterator mid = first;
    core::advance(mid, count2);

    if (comp(*mid, value))
    {
      first = ++mid;
      count -= count2 + 1;
    }
    else
    {
      count = count2;
    }
  }
  return first;
}


// TEMPLATE FUNCTION upper_bound
template<class ForwardIterator, class T>
constexpr ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value)
{
  // find first element that value is before, using operator<
  typename IteratorTraits<ForwardIterator>::DifferenceType count = core::distance(first, last);
  typename IteratorTraits<ForwardIterator>::DifferenceType step{};
  for (; 0 < count; )
  {   // divide and conquer, find half that contains answer
    step = count / 2;
    ForwardIterator mid = first;
    core::advance(mid, step);

    if (!(value < *mid))
    {
      first = ++mid;
      count -= step + 1;
    }
    else
    {
      count = step;
    }
  }
  return first;
}
template<class ForwardIterator, class T, class Compare>
constexpr ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
{
  // find first element not before value, using compare predicate
  typename IteratorTraits<ForwardIterator>::DifferenceType count = core::distance(first, last);
  typename IteratorTraits<ForwardIterator>::DifferenceType step{};
  for (; 0 < count; )
  {
    // divide and conquer, find half that contains answer
    step = count / 2;
    ForwardIterator mid = first;
    core::advance(mid, step);

    if (!comp(value, *mid))
    {
      first = ++mid;
      count -= step + 1;
    }
    else
    {
      count = step;
    }
  }
  return first;
}


// Search
template <class ForwardIterator1, class ForwardIterator2>
constexpr ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
{
  if (first2 == last2)
  {
    return first1;                 // specified in C++11
  }
  while (first1 != last1)
  {
    ForwardIterator1 it1 = first1;
    ForwardIterator2 it2 = first2;
    while (*it1 == *it2)
    {
      ++it1;
      ++it2;
      if (it2 == last2)
      {
        return first1;
      }
      if (it1 == last1)
      {
        return last1;
      }
    }
    ++first1;
  }
  return last1;
}

template <class ForwardIterator1, class ForwardIterator2, class Compare>
constexpr ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Compare comp)
{
  if (first2 == last2)
  {
    return first1;                 // specified in C++11
  }
  while (first1 != last1)
  {
    ForwardIterator1 it1 = first1;
    ForwardIterator2 it2 = first2;
    while (comp(*it1, *it2))
    {
      ++it1;
      ++it2;
      if (it2 == last2)
      {
        return first1;
      }
      if (it1 == last1)
      {
        return last1;
      }
    }
    ++first1;
  }
  return last1;
}



template<class RandomAccessIterator>
constexpr void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
  typename core::IteratorTraits<RandomAccessIterator>::ValueType value = *(last - 1);
  core::Internal::push_heap(first, typename core::IteratorTraits<RandomAccessIterator>::DifferenceType((last - first) - 1), typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(0), value);
}
/// Pushes values to the heap using provided binary predicate Compare. \ref CStd
template<class RandomAccessIterator, class Compare>
constexpr void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
  typename core::IteratorTraits<RandomAccessIterator>::ValueType value = *(last - 1);
  core::Internal::push_heap(first, typename core::IteratorTraits<RandomAccessIterator>::DifferenceType((last - first) - 1), typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(0), value, comp);
}
/// Prepares heap for popping a value using core::less predicate. \ref CStd
template <class RandomAccessIterator>
constexpr void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
  RandomAccessIterator result = last - 1;
  typename core::IteratorTraits<RandomAccessIterator>::ValueType value = *result;
  *result = *first;
  core::Internal::adjust_heap(first, typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(0), typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(result - first), value);
}
/// Prepares heap for popping a value using Compare predicate. \ref CStd
template <class RandomAccessIterator, class Compare>
constexpr void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
  RandomAccessIterator result = last - 1;
  typename core::IteratorTraits<RandomAccessIterator>::ValueType value = *result;
  *result = *first;
  core::Internal::adjust_heap(first, typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(0), typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(result - first), value, comp);
}

/// [Extension] Same as core::pop_heap using core::less predicate, but allows you to provide iterator where to store the result.
template <class RandomAccessIterator>
constexpr void pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result)
{
  typename core::IteratorTraits<RandomAccessIterator>::ValueType value = *result;
  *result = *first;
  core::Internal::adjust_heap(first, typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(0), typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(last - first), value);
}
/// [Extension] Same as core::pop_heap using Compare predicate, but allows you to provide iterator where to store the result.
template <class RandomAccessIterator, class Compare>
constexpr void pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, Compare comp)
{
  typename core::IteratorTraits<RandomAccessIterator>::ValueType value = *result;
  *result = *first;
  core::Internal::adjust_heap(first, typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(0), typename core::IteratorTraits<RandomAccessIterator>::DifferenceType(last - first), value, comp);
}

/// Make a heap from an array of values, using core::less predicate. \ref CStd
template <class RandomAccessIterator>
constexpr void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
  if (last - first < 2)
  {
    return;
  }
  typename core::IteratorTraits<RandomAccessIterator>::DifferenceType length = last - first;
  typename core::IteratorTraits<RandomAccessIterator>::DifferenceType parent = (length - 2) / 2;

  for (;; )
  {
    typename core::IteratorTraits<RandomAccessIterator>::ValueType value = *(first + parent);
    core::Internal::adjust_heap(first, parent, length, value);
    if (parent == 0)
    {
      return;
    }
    --parent;
  }
}

/// Make a heap from an array of values, using Compare predicate. \ref CStd
template <class RandomAccessIterator, class Compare>
constexpr void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
  if (last - first < 2)
  {
    return;
  }
  typename core::IteratorTraits<RandomAccessIterator>::DifferenceType length = last - first;
  typename core::IteratorTraits<RandomAccessIterator>::DifferenceType parent = (length - 2) / 2;

  for (;; )
  {
    typename core::IteratorTraits<RandomAccessIterator>::ValueType value = *(first + parent);
    core::Internal::adjust_heap(first, parent, length, value, comp);
    if (parent == 0)
    {
      return;
    }
    --parent;
  }
}




template<typename TIterator, typename TType>
void Fill(TIterator begin, TIterator end, const TType& value) {
  core::Internal::FillHelper<core::is_scalar<TType>::value>::fill(begin, end, value);
}

//
// FillN
//
template<typename TIter, typename TSize, typename TType>
TIter fill_n(TIter iterator, TSize n, const TType& value) {

  for (; n-- > 0; iterator++) {
    *iterator = value;
  }

  return iterator;
}


//
// MinAlt
//
template<typename T>
inline constexpr typename core::enable_if<is_scalar<T>::value, T>::type
MinAlt(T a, T b) {
  return b < a ? b : a;
}

template<typename T>
inline typename core::enable_if<!is_scalar<T>::value, const T &>::type
MinAlt(const T &a, const T &b) {
  return b < a ? b : a;
}

inline constexpr float MinAlt(float a, float b) {
  return b < a ? b : a;
}

inline constexpr double MinAlt(double a, double b) {
  return b < a ? b : a;
}

inline constexpr long double MinAlt(long double a, long double b) {
  return b < a ? b : a;
}


//
// MinAlt
//
template<typename T>
inline constexpr typename core::enable_if<is_scalar<T>::value, T>::type
MaxAlt(T a, T b) {
  return b > a ? b : a;
}

template<typename T>
inline typename core::enable_if<!is_scalar<T>::value, const T &>::type
MaxAlt(const T &a, const T &b) {
  return b > a ? b : a;
}

inline constexpr float MaxAlt(float a, float b) {
  return b > a ? b : a;
}

inline constexpr double MaxAlt(double a, double b) {
  return b > a ? b : a;
}

inline constexpr long double MaxAlt(long double a, long double b) {
  return b > a ? b : a;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}