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
#include "core/container/vector_span_iterator.h"
#include "core/memory/std_allocator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
class VectorSpan {
public:

  typedef TType value_type;
  typedef VectorSpanIterator<VectorSpan<TType>> iterator;
  typedef VectorSpanIterator<VectorSpan<const TType>> const_iterator;

public:

  VectorSpan();

  explicit VectorSpan(TType* ptr, core::uint32 numElements);

  VectorSpan(TType* begin, TType* end);

  ~VectorSpan();


  VectorSpan& operator=(const VectorSpan& rhs) = default;

  VectorSpan& operator=(VectorSpan&& rhs) = default;

  TType& operator[](core::uint32 index) const;

  void swap(VectorSpan& rhs);


  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator cbegin() const;

  const_iterator cend() const;

  template<typename TOtherType>
  bool contains(const TOtherType& element) const;

  constexpr bool empty() const;

  constexpr core::uint32 size() const;

  constexpr core::uint32 data_size() const;

  constexpr TType* data() const;

  TType& front() const;

  TType& back() const;

  TType& at(core::uint32 index) const;


  VectorSpan first(core::uint32 numElements) const;

  VectorSpan last(core::uint32 numElements) const;

  VectorSpan subspan(core::uint32 offset, core::uint32 numElements) const;

private:

  TType* mStart;
  TType* mEnd;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/vector_span.inl"