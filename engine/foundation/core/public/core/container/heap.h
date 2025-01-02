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
#include "core/core/config.h"
#include "core/container/vector.h"
#include "core/memory/std_allocator.h"
#include "core/std/functional.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TSort = core::less<TType>>
class Heap {
public:

  typedef Vector<TType> container_type;
  typedef TType element_type;
  typedef typename container_type::const_iterator iterator;
  typedef TSort sort_predicate;
  typedef typename container_type::container_result container_result;

public:

  Heap();

  Heap(const Heap& rhs);

  Heap(Heap&& rhs);


  Heap& operator=(const Heap& rhs);

  Heap& operator=(Heap&& rhs);

  bool operator==(const Heap& rhs) const;

  bool operator!=(const Heap& rhs) const;

  const TType& operator[](core::uint32 index) const;

  TType& operator[](core::uint32 index);


  void swap(Heap& rhs);


  const TType* data() const;

  core::uint32 size() const;

  core::uint32 data_size() const;

  core::uint32 capacity() const;

  core::uint32 data_capacity() const;

  bool empty() const;


  iterator begin() const;

  iterator end() const;


  const TType& front() const;

  void push_heap(const TType& element);

  void push_heap(TType&& element);

  TType pop_heap();


  container_type erase(iterator iter);


  bool contains(const TType& element);

  void clear();

  void reserve(core::uint32 capacity);

  void shrink();

  void heapify();

private:

  container_type mData;

  sort_predicate mSortPredicate;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/heap.inl"