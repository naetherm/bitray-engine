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
#include "core/std/typetraits/is_signed.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


using std::distance;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
enum IteratorStatusFlag {
  IsfNone = 0x00,
  IsfValue = 0x01,
  IsfCurrent = 0x02,
  IsfCanDereference = 0x04
};

struct InputIteratorTag {
};
struct OutputIteratorTag {
};
struct ForwardIteratorTag : public InputIteratorTag {
};
struct BidirectionalIteratorTag : public ForwardIteratorTag {
};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {
};
struct ContiguousIteratorTag : public RandomAccessIteratorTag {
};


template<typename TCategory, typename TValue, typename TDistance, typename TPointer, typename TReference>
struct TIterator {
  typedef TCategory IteratorCategory;
  typedef TDistance DifferenceType;
  typedef TValue ValueType;
  typedef TPointer Pointer;
  typedef TReference Reference;
};


template<typename TIterator>
struct IteratorTraits {
  typedef typename TIterator::IteratorCategory IteratorCategory;
  typedef typename TIterator::DifferenceType DifferenceType;
  typedef typename TIterator::ValueType ValueType;
  typedef typename TIterator::Pointer Pointer;
  typedef typename TIterator::Reference Reference;
};

template<typename TType>
struct IteratorTraits<TType *> {
  typedef RandomAccessIteratorTag IteratorCategory;
  typedef TType ValueType;
  typedef std::ptrdiff_t DifferenceType;
  typedef TType *Pointer;
  typedef TType &Reference;
};

template<typename TType>
struct IteratorTraits<const TType *> {
  typedef RandomAccessIteratorTag IteratorCategory;
  typedef TType ValueType;
  typedef std::ptrdiff_t DifferenceType;
  typedef TType *Pointer;
  typedef TType &Reference;
};


/**
 *
 * @tparam TIterator
 */
template<typename TIter>
class ReverseIterator : public TIterator<
  typename IteratorTraits<TIter>::IteratorCategory,
  typename IteratorTraits<TIter>::ValueType,
  typename IteratorTraits<TIter>::DifferenceType,
  typename IteratorTraits<TIter>::Pointer,
  typename IteratorTraits<TIter>::Reference> {
public:
  typedef TIter                                                   IteratorType;
  typedef IteratorType                                              WrappedIteratorType;   // This is not in the C++ Standard; it's used by use to identify it as a wrapping iterator type.
  typedef typename IteratorTraits<TIter>::Pointer         Pointer;
  typedef typename IteratorTraits<TIter>::Reference       Reference;
  typedef typename IteratorTraits<TIter>::DifferenceType DifferenceType;

protected:
  TIter mIterator;

public:
  constexpr ReverseIterator()      // It's important that we construct mIterator, because if Iterator
  : mIterator() { }   // is a pointer, there's a difference between doing it and not.

  constexpr explicit ReverseIterator(IteratorType i)
  : mIterator(i) { }

  constexpr ReverseIterator(const ReverseIterator& ri)
  : mIterator(ri.mIterator) { }

  template <typename U>
  constexpr ReverseIterator(const ReverseIterator<U>& ri)
  : mIterator(ri.base()) { }

  template <typename U>
  constexpr ReverseIterator<TIter>& operator=(const ReverseIterator<U>& ri)
  { mIterator = ri.base(); return *this; }

  constexpr IteratorType base() const
  { return mIterator; }

  constexpr Reference operator*() const
  {
    IteratorType i(mIterator);
    return *--i;
  }

  constexpr Pointer operator->() const
  { return &(operator*()); }

  constexpr ReverseIterator& operator++()
  { --mIterator; return *this; }

  constexpr ReverseIterator operator++(int)
  {
    ReverseIterator ri(*this);
    --mIterator;
    return ri;
  }

  constexpr ReverseIterator& operator--()
  { ++mIterator; return *this; }

  constexpr ReverseIterator operator--(int)
  {
    ReverseIterator ri(*this);
    ++mIterator;
    return ri;
  }

  constexpr ReverseIterator operator+(DifferenceType n) const
  { return ReverseIterator(mIterator - n); }

  constexpr ReverseIterator& operator+=(DifferenceType n)
  { mIterator -= n; return *this; }

  constexpr ReverseIterator operator-(DifferenceType n) const
  { return ReverseIterator(mIterator + n); }

  constexpr ReverseIterator& operator-=(DifferenceType n)
  { mIterator += n; return *this; }

  constexpr Reference operator[](DifferenceType n) const
  { return mIterator[-n - 1]; }
};


/**
 * @brief
 * advance
 *
 * @remarksImplements the advance() function. There are three versions, one for
 * random access iterators (e.g. with vector), one for bidirectional
 * iterators (list) and one for regular input iterators (e.g. with slist).
 */
template <typename InputIterator, typename Distance>
inline void
advance_impl(InputIterator& i, Distance n, InputIteratorTag)
{
  while(n--)
    ++i;
}

template <bool signedDistance>
struct advance_bi_impl
{
  template <typename BidirectionalIterator, typename Distance>
  static void advance_impl(BidirectionalIterator& i, Distance n) // Specialization for unsigned distance type.
  {
    while(n--)
      ++i;
  }
};

template <>
struct advance_bi_impl<true>
{
  template <typename BidirectionalIterator, typename Distance>
  static void advance_impl(BidirectionalIterator& i, Distance n) // Specialization for signed distance type.
  {
    if(n > 0)
    {
      while(n--)
        ++i;
    }
    else
    {
      while(n++)
        --i;
    }
  }
};

template <typename BidirectionalIterator, typename Distance>
inline void
advance_impl(BidirectionalIterator& i, Distance n, BidirectionalIteratorTag)
{
  advance_bi_impl<core::is_signed<Distance>::value>::advance_impl(i, n);
}

template <typename RandomAccessIterator, typename Distance>
inline void
advance_impl(RandomAccessIterator& i, Distance n, RandomAccessIteratorTag)
{
  i += n;
}

// Special version defined so that std C++ iterators can be recognized by
// this function. Unfortunately, this function treats all foreign iterators
// as InputIterators and thus can seriously hamper performance in the case
// of large ranges of bidirectional_iterator_tag iterators.
//template <typename InputIterator, typename Distance>
//inline void
//advance_impl(InputIterator& i, Distance n, ...)
//{
//    while(n--)
//        ++i;
//}

template <typename InputIterator, typename Distance>
inline void advance(InputIterator& i, Distance n)
{
  typedef typename core::IteratorTraits<InputIterator>::IteratorCategory IC;

  core::advance_impl(i, n, IC());
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core