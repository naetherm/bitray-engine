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
#include "core/container/sorted_vector.h"
#include "core/memory/std_allocator.h"
#include "core/std/functional.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TSortPredicate = core::less<TType>>
class Set {
public:

  typedef SortedVector<TType, TSortPredicate> container_type;
  typedef TType value_type;
  typedef typename container_type::iterator iterator;
  typedef typename container_type::const_iterator const_iterator;
  typedef ContainerResult<iterator> container_result;
  typedef TSortPredicate sort_predicate;

public:

  Set();

  explicit Set(core::uint32 capacity);

  Set(const Set& rhs);

  Set(Set&& rhs);

  ~Set();


  Set& operator=(const Set& rhs);

  Set& operator=(Set&& rhs);

  bool operator==(const Set& rhs) const;

  bool operator!=(const Set& rhs) const;


  void swap(Set& rhs);


  core::uint32 size() const;

  core::uint32 data_size() const;

  core::uint32 capacity() const;

  core::uint32 data_capacity() const;

  bool empty() const;

  void clear();

  void reserve(core::uint32 capacity);

  void shrink();


  iterator begin();

  iterator end();

  const_iterator cbegin() const;

  const_iterator cend() const;

  void get_elements(Vector<TType>& outElements) const;

  Vector<TType> get_elements() const;


  container_result insert(const TType& element);

  container_result insert(TType&& element);

  template<typename... TArgs>
  void emplace(TArgs&&... args);

  template<typename TOtherType>
  container_result erase(const TOtherType& element);

  container_result erase(iterator iter);


  template<typename TOtherType>
  iterator find(const TOtherType& element);

  template<typename TOtherType>
  const_iterator find(const TOtherType& element) const;

  template<typename TOtherType>
  bool contains(const TOtherType& element);

private:

  container_type mData;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/set.inl"