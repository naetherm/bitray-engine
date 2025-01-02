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
#include "core/std/iterator.h"
#include "core/std/base.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template <typename T, core::sizeT N = 1>
class Array
{
public:
  typedef Array<T, N>                                   this_type;
  typedef T                                             value_type;
  typedef value_type&                                   reference;
  typedef const value_type&                             const_reference;
  typedef value_type*                                   iterator;
  typedef const value_type*                             const_iterator;
  typedef core::ReverseIterator<iterator>               reverse_iterator;
  typedef core::ReverseIterator<const_iterator>         const_reverse_iterator;
  typedef core::sizeT                                   size_type;
  typedef ptrdiff_t                                     difference_type;

public:
  enum
  {
    count = N
  };

  // Note that the member data is intentionally public.
  // This allows for aggregate initialization of the
  // object (e.g. array<int, 5> a = { 0, 3, 2, 4 }; )
  value_type mValue[N ? N : 1];

public:
  // We intentionally provide no constructor, destructor, or assignment operator.

  void fill(const value_type& value);

  // Unlike the swap function for other containers, array::swap takes linear time,
  // may exit via an exception, and does not cause iterators to become associated with the other container.
  void swap(this_type& x);

  constexpr iterator       begin() noexcept;
  constexpr const_iterator begin() const noexcept;
  constexpr const_iterator cbegin() const noexcept;

  constexpr iterator       end() noexcept;
  constexpr const_iterator end() const noexcept;
  constexpr const_iterator cend() const noexcept;

  constexpr reverse_iterator       rbegin() noexcept;
  constexpr const_reverse_iterator rbegin() const noexcept;
  constexpr const_reverse_iterator crbegin() const noexcept;

  constexpr reverse_iterator       rend() noexcept;
  constexpr const_reverse_iterator rend() const noexcept;
  constexpr const_reverse_iterator crend() const noexcept;

  constexpr bool empty() const noexcept;
  constexpr size_type size() const noexcept;
  constexpr size_type max_size() const noexcept;

  constexpr T*       data() noexcept;
  constexpr const T* data() const noexcept;

  constexpr reference       operator[](size_type i);
  constexpr const_reference operator[](size_type i) const;
  constexpr const_reference at(size_type i) const;
  constexpr reference       at(size_type i);

  constexpr reference       front();
  constexpr const_reference front() const;

  constexpr reference       back();
  constexpr const_reference back() const;

  bool validate() const;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


#include "core/container/array.inl"