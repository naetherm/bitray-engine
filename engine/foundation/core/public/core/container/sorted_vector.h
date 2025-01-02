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
#include "core/container/vector.h"
#include "core/core/config.h"
#include "core/memory/std_allocator.h"
#include "core/utility/functional.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TSortPredicate = core::Less<>>
class SortedVector : public Vector<TType> {
public:

  typedef Vector<TType> base_type;
  typedef TType element_type;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::reverse_iterator reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;
  typedef typename base_type::container_result container_result;
  typedef TSortPredicate sort_predicate;

public:

  SortedVector();

  SortedVector(const SortedVector& rhs);

  SortedVector(SortedVector&& rhs);

  explicit SortedVector(core::uint32 size);


  SortedVector& operator=(const SortedVector& rhs);

  SortedVector& operator=(SortedVector&& rh);

  bool operator==(const SortedVector& rhs) const;

  bool operator!=(const SortedVector& rhs) const;

  TType& operator[](core::uint32 index);

  const TType& operator[](core::uint32 index) const;


  void swap(SortedVector& rhs);


  const TType* data() const;

  TType* data();

  void clear();

  void resize(core::uint32 size);

  void resize(core::uint32 size, const TType& element);

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator cbegin() const;

  const_iterator cend() const;

  reverse_iterator rbegin();

  reverse_iterator rend();

  const_reverse_iterator crbegin() const;

  const_reverse_iterator crend() const;

  const TType& front() const;

  TType& front();

  const TType& back() const;

  TType& back();


  void push_back(const TType& element);

  void push_back(TType& element);

  void push_back(const Vector<TType>& vec);

  TType pop_back();

  container_result insert(const TType& element);

  container_result insert(TType&& element);

  container_result insert(const_iterator iter, const TType& element);

  container_result insert(const_iterator iter, TType&& element);

  container_result insert_at(core::uint32 index, const TType& element);

  container_result insert_at(core::uint32 index, TType& element);

  template<typename... TArgs>
  container_result emplace(const_iterator iter, TArgs&&... args);

  template<typename... TArgs>
  TType& emplace_back(TArgs&&... args);

  template<typename... TArgs>
  container_result emplace_at(const core::uint32 index, TArgs&&... args);

  container_result erase(const TType& element);

  container_result erase_at(const core::uint32 index);

  container_result erase_at(const core::uint32 first, core::uint32 last);

  using base_type::erase;


  core::int32 get_index(const TType& element) const;

  bool contains(const TType& element);

  iterator find(const TType& element);

  const_iterator find(const TType& element) const;


  void grow(core::uint32 numOfElements);

  void sort();

  void insertion_sort();

  bool is_sorted() const;

  void make_clean();

  void make_unique();

protected:

  void set_dirty();

  bool is_dirty() const;

private:

  bool mIsDirty;

  sort_predicate mPredicate;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/sorted_vector.inl"