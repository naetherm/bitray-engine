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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/memory/std_allocator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TSpan>
class VectorSpanIterator {
public:

  using TType = typename TSpan::value_type;
  typedef TType* pointer;
  typedef TType& reference;

public:

  VectorSpanIterator();

  VectorSpanIterator(const VectorSpanIterator& rhs);

  VectorSpanIterator(TType* ptr, const TSpan* spanContainer);


  VectorSpanIterator& operator=(const VectorSpanIterator& rhs);

  constexpr bool operator==(const VectorSpanIterator& rhs) const;

  constexpr bool operator!=(const VectorSpanIterator& rhs) const;

  constexpr bool operator<(const VectorSpanIterator& rhs) const;

  constexpr bool operator<=(const VectorSpanIterator& rhs) const;

  constexpr bool operator>(const VectorSpanIterator& rhs) const;

  constexpr bool operator>=(const VectorSpanIterator& rhs) const;

  VectorSpanIterator operator+(core::int64 index) const;

  VectorSpanIterator operator-(core::int64 index) const;

  VectorSpanIterator& operator+=(core::int64 index);

  VectorSpanIterator& operator-=(core::int64 index);


  reference operator*() const;

  pointer operator->() const;

  reference operator[](core::int64 index) const;


  VectorSpanIterator& operator++();

  VectorSpanIterator operator++(int);

  VectorSpanIterator& operator--();

  VectorSpanIterator operator--(int);

private:

  TType* mPtr;
  const TSpan* mSpanContainer;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/vector_span_iterator.inl"