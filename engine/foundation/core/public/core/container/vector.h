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
#include "core/container/container_result.h"
#include "core/container/container_policies.h"
#include "core/container/vector_span.h"
#include "core/memory/std_allocator.h"
#include "core/std/iterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TAllocator = DEFAULT_ALLOCATOR>
class Vector {
public:

  typedef Vector<TType>                                   this_type;
  typedef TType                                           value_type;
  typedef value_type&                                     reference;
  typedef const value_type&                               const_reference;
  typedef value_type*                                     iterator;
  typedef const value_type*                               const_iterator;
  typedef core::ReverseIterator<iterator>               reverse_iterator;
  typedef core::ReverseIterator<const_iterator>         const_reverse_iterator;
  typedef core::sizeT                                   size_type;
  typedef std::ptrdiff_t                                       difference_type;
  typedef ContainerResult<iterator>                       container_result;
  typedef TAllocator                                      allocator;
  typedef typename ConstructorExecutorPolicy<TType>::type constructor_policy;
  typedef typename DestructorExecutorPolicy<TType>::type  destructor_policy;
  typedef typename CopyConstructorExecutorPolicy<TType>::type  copy_constructor_policy;
  typedef typename MoveConstructorExecutorPolicy<TType>::type  move_constructor_policy;
  typedef typename MoveAssignExecutorPolicy<TType>::type  move_assign_policy;

public:

  inline Vector();

  Vector(const Vector& rhs);

  Vector(Vector&& rhs);

  explicit Vector(typename Vector<TType, TAllocator>::size_type nSize);

  ~Vector();


  Vector& operator=(const Vector& rhs);

  Vector& operator=(Vector&& rhs);

  bool operator==(const Vector& rhs) const;

  bool operator!=(const Vector& rhs) const;

  TType& operator[](typename Vector<TType, TAllocator>::size_type index);

  const TType& operator[](typename Vector<TType, TAllocator>::size_type index) const;


  void swap(Vector& rhs);


  TType* data();

  const TType* data() const;

  core::uint32 capacity() const;

  core::uint32 size() const;

  core::uint32 max_size() const;

  core::uint32 data_size() const;

  core::uint32 data_capacity() const;

  bool empty() const;


  TType& front();

  const TType& front() const;

  TType& back();

  const TType& back() const;

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator cbegin() const;

  const_iterator cend() const;

  reverse_iterator rbegin();

  reverse_iterator rend();

  const_reverse_iterator rbegin() const;

  const_reverse_iterator rend() const;

  const_reverse_iterator crbegin() const;

  const_reverse_iterator crend() const;


  reference at(size_type pos);

  const_reference at(size_type pos) const;


  void push_back(const TType& rhs);

  void push_back(TType&& rhs);

  void push_back(const Vector& rhs);

  TType pop_back();


  container_result insert(const_iterator iter, const TType& element);

  container_result insert(const_iterator iter, TType&& element);

  container_result insert_at(const size_type index, const TType& element);

  container_result insert_at(const size_type index, TType&& element);

  template<typename... TArgs>
  container_result emplace(const_iterator iter, TArgs&&... args);

  template<typename... TArgs>
  container_result emplace_at(const size_type index, TArgs&&... args);

  template<typename... TArgs>
  TType& emplace_back(TArgs&&... args);

  container_result erase(iterator iter);

  container_result erase(const_iterator iter);

  container_result erase(iterator first, iterator last);

  container_result erase(const_iterator first, const_iterator last);

  container_result erase(const TType& element);

  container_result erase_at(const size_type index);

  container_result erase_at(const size_type first, const size_type last);

  size_type get_index(const TType& rhs) const;

  size_type find(const TType& element) const;

  bool contains(const TType& element) const;

  void clear();

  void resize(core::sizeT size);

  void resize(core::sizeT size, const TType& element);

  void grow(core::uint32 numElementsToAdd);

  void reserve(core::uint32 capacity);

  void shrink();

protected:

  void grow_internal(core::uint32 numElementsToAdd);

  void resize_buffer_internal(core::uint32 newCapacity);

  void ensure_buffer_size(core::uint32 newSize);

private:

private:

  allocator mAllocator;

  TType* mBuffer;

  core::uint32 mSize;
  core::uint32 mCapacity;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/vector.inl"