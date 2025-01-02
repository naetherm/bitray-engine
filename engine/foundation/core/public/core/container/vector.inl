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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/memory/memory.h"
#include "core/std/algorithm.h"
#include "core/std/functional.h"
#include <cmath>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TAllocator>
Vector<TType, TAllocator>::Vector()
: mBuffer(nullptr)
, mSize(0)
, mCapacity(0) {

}

template<typename TType, typename TAllocator>
Vector<TType, TAllocator>::Vector(const Vector<TType, TAllocator>& rhs)
: mBuffer(nullptr)
, mSize(rhs.mSize)
, mCapacity(0)  {
  resize_buffer_internal(rhs.mCapacity);
  core::Memory::copy(mBuffer, rhs.mBuffer, rhs.data_capacity());
  //for (auto element: rhs) {
  //  push_back(element);
  //}
}

template<typename TType, typename TAllocator>
Vector<TType, TAllocator>::Vector(Vector<TType, TAllocator>&& rhs)
: mBuffer(nullptr)
, mSize(0)
, mCapacity(0)  {

}

template<typename TType, typename TAllocator>
Vector<TType, TAllocator>::Vector(typename Vector<TType, TAllocator>::size_type nSize)
: mBuffer(nullptr)
, mSize(0)
, mCapacity(0)  {
  resize(nSize);
}

template<typename TType, typename TAllocator>
Vector<TType, TAllocator>::~Vector() {
  clear();
  if (mBuffer)
    mAllocator.deallocate(mBuffer, 1);
}


template<typename TType, typename TAllocator>
Vector<TType, TAllocator>& Vector<TType, TAllocator>::operator=(const Vector<TType, TAllocator>& rhs) {
  if (&rhs != this) {
    // Clear own buffer
    clear();
    shrink();

    resize(rhs.size());

    // Copy buffer
    core::Memory::copy(data(), rhs.data(), rhs.data_size());
  }

  return *this;
}

template<typename TType, typename TAllocator>
Vector<TType, TAllocator>& Vector<TType, TAllocator>::operator=(Vector<TType, TAllocator>&& rhs) {
  if (&rhs != this) {
    // Clear own buffer
    clear();
    shrink();

    resize(rhs.size());

    // Copy buffer
    copy_constructor_policy::exec(data(), rhs.begin(), rhs.size());
    //core::Memory::copy(data(), rhs.data(), rhs.data_size());
    //for (size_t i = 0; i < rhs.size(); ++i) {
    //  mBuffer[i] = rhs.mBuffer[i];
    //}
  }

  return *this;
}

template<typename TType, typename TAllocator>
bool Vector<TType, TAllocator>::operator==(const Vector<TType, TAllocator>& rhs) const {
  return ((mSize == rhs.mSize) && (mCapacity == rhs.mCapacity) &&
          core::equal(begin(), end(), rhs.begin()));
}

template<typename TType, typename TAllocator>
bool Vector<TType, TAllocator>::operator!=(const Vector<TType, TAllocator>& rhs) const {
  return !operator==(rhs);
}

template<typename TType, typename TAllocator>
TType& Vector<TType, TAllocator>::operator[](typename Vector<TType, TAllocator>::size_type index) {
  return mBuffer[index];
}

template<typename TType, typename TAllocator>
const TType& Vector<TType, TAllocator>::operator[](typename Vector<TType, TAllocator>::size_type index) const {
  return mBuffer[index];
}

template<typename TType, typename TAllocator>
TType& Vector<TType, TAllocator>::at(typename Vector<TType, TAllocator>::size_type index) {
  return mBuffer[index];
}

template<typename TType, typename TAllocator>
const TType& Vector<TType, TAllocator>::at(typename Vector<TType, TAllocator>::size_type index) const {
  return mBuffer[index];
}


template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::swap(Vector<TType, TAllocator>& rhs) {}


template<typename TType, typename TAllocator>
TType* Vector<TType, TAllocator>::data() {
  return mBuffer;
}

template<typename TType, typename TAllocator>
const TType* Vector<TType, TAllocator>::data() const {
  return mBuffer;
}

template<typename TType, typename TAllocator>
core::uint32 Vector<TType, TAllocator>::capacity() const {
  return mCapacity;
}

template<typename TType, typename TAllocator>
core::uint32 Vector<TType, TAllocator>::size() const {
  return mSize;
}

template<typename TType, typename TAllocator>
core::uint32 Vector<TType, TAllocator>::max_size() const {
  return std::numeric_limits<core::uint32>::max() / sizeof(TType);
}

template<typename TType, typename TAllocator>
core::uint32 Vector<TType, TAllocator>::data_size() const {
  return mSize * sizeof(TType);
}

template<typename TType, typename TAllocator>
core::uint32 Vector<TType, TAllocator>::data_capacity() const {
  return mCapacity * sizeof(TType);
}

template<typename TType, typename TAllocator>
bool Vector<TType, TAllocator>::empty() const {
  return mSize == 0;
}


template<typename TType, typename TAllocator>
TType& Vector<TType, TAllocator>::front() {
  return mBuffer[0];
}

template<typename TType, typename TAllocator>
const TType& Vector<TType, TAllocator>::front() const {
  return mBuffer[0];

}

template<typename TType, typename TAllocator>
TType& Vector<TType, TAllocator>::back() {
  return mBuffer[mSize - 1];
}

template<typename TType, typename TAllocator>
const TType& Vector<TType, TAllocator>::back() const {
  return mBuffer[mSize - 1];
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::iterator Vector<TType, TAllocator>::begin() {
  return Vector<TType, TAllocator>::iterator(mBuffer);
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::iterator Vector<TType, TAllocator>::end() {
  return Vector<TType, TAllocator>::iterator(mBuffer + mSize);
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::const_iterator Vector<TType, TAllocator>::begin() const {
  return Vector<TType, TAllocator>::const_iterator(mBuffer);
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::const_iterator Vector<TType, TAllocator>::end() const {
  return Vector<TType, TAllocator>::const_iterator(mBuffer + mSize);
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::const_iterator Vector<TType, TAllocator>::cbegin() const {
  return Vector<TType, TAllocator>::const_iterator(mBuffer);
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::const_iterator Vector<TType, TAllocator>::cend() const {
  return Vector<TType, TAllocator>::const_iterator(mBuffer + mSize);
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::reverse_iterator Vector<TType, TAllocator>::rbegin() {
  return Vector<TType, TAllocator>::reverse_iterator(end());
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::reverse_iterator Vector<TType, TAllocator>::rend() {
  return Vector<TType, TAllocator>::reverse_iterator(begin());
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::const_reverse_iterator Vector<TType, TAllocator>::rbegin() const {
  return Vector<TType, TAllocator>::const_reverse_iterator(end());
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::const_reverse_iterator Vector<TType, TAllocator>::rend() const {
  return Vector<TType, TAllocator>::const_reverse_iterator(begin());
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::const_reverse_iterator Vector<TType, TAllocator>::crbegin() const {
  return Vector<TType, TAllocator>::const_reverse_iterator(end());
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::const_reverse_iterator Vector<TType, TAllocator>::crend() const {
  return Vector<TType, TAllocator>::const_reverse_iterator(begin());
}


template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::push_back(const TType& rhs) {
  grow_internal(1);
  copy_constructor_policy::exec(data() + mSize - 1, &rhs);
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::push_back(TType&& rhs) {
  grow_internal(1);
  move_constructor_policy::exec(data() + mSize - 1, &rhs);
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::push_back(const Vector& rhs) {
  const core::uint32 size = rhs.size();
  grow_internal(size);
  copy_constructor_policy::exec(data() + mSize - size, rhs.data(), size);
}

template<typename TType, typename TAllocator>
TType Vector<TType, TAllocator>::pop_back() {
  --mSize;
  const TType* lastElement = data() + mSize;
  TType result = core::move(*lastElement);
  return result;
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::insert(const_iterator iter, const TType& element) {
  return insert_at(static_cast<core::uint32>(iter - begin()), element);
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::insert(const_iterator iter, TType&& element) {
  return insert_at(static_cast<core::uint32>(iter - begin()), core::forward<TType>(element));
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::insert_at(const typename Vector<TType, TAllocator>::size_type index, const TType& element) {
  if (index == mSize) {
    // Just add the element at the end
    push_back(element);

    return container_result::success(iterator(data() + index));
  } else {
    core::uint32 tempSize = size();
    grow_internal(1);
    TType* temp_data = data();
    const core::uint32 temp_size = size();

    // Move last element to a newly created position
    move_constructor_policy::exec(temp_data + temp_size - 1, temp_data + tempSize - 1);
    move_assign_policy::exec(data() + index + 1, data() + index, tempSize - index - 1);
    destructor_policy::exec(data() + index, 1);

    copy_constructor_policy::exec(data() + index, &element);

    return container_result::success(iterator(data() + index));
  }
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::insert_at(const typename Vector<TType, TAllocator>::size_type index, TType&& element) {
  if (index == mSize) {
    // Just add the element at the end
    push_back(element);

    return container_result::success(iterator(data() + index));
  } else {
    core::uint32 tempSize = size();
    grow_internal(1);
    TType* temp_data = data();
    const core::uint32 temp_size = size();

    // Move last element to a newly created position
    move_constructor_policy::exec(temp_data + temp_size - 1, temp_data + tempSize - 1);
    move_assign_policy::exec(data() + index + 1, data() + index, tempSize - index - 1);
    destructor_policy::exec(data() + index, 1);

    move_constructor_policy::exec(&mBuffer[index], &element);

    return container_result::success(iterator(data() + index));
  }
}

template<typename TType, typename TAllocator>
template<typename... TArgs>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::emplace(const_iterator iter, TArgs&&... args) {
  return emplace_at(static_cast<core::uint32>(iter - begin()), core::forward<TArgs>(args)...);
}

template<typename TType, typename TAllocator>
template<typename... TArgs>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::emplace_at(const typename Vector<TType, TAllocator>::size_type index, TArgs&&... args) {
  // Check element is added at the end
  if (mSize == index) {
    emplace_back(args...);

    return container_result::success(data() + mSize - 1);
  } else {
    // TODO(naetherm): Move all elements one step forward starting at index
    grow_internal(1);
    for (core::uint32 i = mSize; mSize > index; i--) {
      core::Memory::move(&mBuffer[i], &mBuffer[i-1], sizeof(TType));
    }
    // Get element at position index
    TType* element = data() + index;
    ::new(element) TType(core::forward<TArgs>(args)...);

    return Vector<TType, TAllocator>::container_result();
  }

}

template<typename TType, typename TAllocator>
template<typename... TArgs>
TType& Vector<TType, TAllocator>::emplace_back(TArgs&&... args) {
  grow_internal(1);
  return *(::new(data() + mSize - 1) TType(core::forward<TArgs>(args)...));
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::erase(const_iterator iter) {
  return erase_at(static_cast<core::uint32>(iter - begin()));
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::erase(iterator iter) {
  return erase_at(static_cast<core::uint32>(iter - begin()));
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::erase(const_iterator first, const_iterator last) {
  return erase_at(static_cast<core::uint32>(first - begin()), static_cast<core::uint32>(last - begin()));
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::erase(iterator first, iterator last) {
  return erase_at(static_cast<core::uint32>(first - begin()), static_cast<core::uint32>(last - begin()));
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::erase(const TType& element) {
  // Loop through all elements and check if we can find `element`
  const TType* typedData = data();
  for (core::uint32 i = 0; i < mSize; ++i) {
    if (typedData[i] == element) {
      return erase_at(i);
    }
  }
  return container_result::failure();
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::erase_at(const typename Vector<TType, TAllocator>::size_type index) {
  TType* element = data() + index;
  // Move elements back
  move_assign_policy::exec(element, element+1, static_cast<core::uint32>(mSize - index - 1));

  destructor_policy::exec(element + static_cast<core::uint32>(mSize - index - 1), 1);

  --mSize;
  return container_result::success(element);
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::container_result Vector<TType, TAllocator>::erase_at(const typename Vector<TType, TAllocator>::size_type first, const typename Vector<TType, TAllocator>::size_type last) {
  if ( first < last )
  {
    const core::uint32 num = last - first;
    if ( num == mSize )
    {
      clear();
      return container_result::success( begin() );
    }
    else
    {
      TType* data = data() + first;
      move_assign_policy::exec(data, data+num, static_cast< core::uint32 >( mSize - first - num ));

      destructor_policy::exec(data + static_cast< core::uint32 >( mSize - first - num ), num);
      mSize -= num;
      return container_result::success( iterator( data ) );
    }
  }
  return container_result::failure();
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::size_type Vector<TType, TAllocator>::get_index(const TType& rhs) const {
  const_iterator iter = core::find(begin(), end(), rhs);
  if (iter != end()) {
    return static_cast<core::int32>(iter - begin());
  }
  return INVALID_HANDLE;
}

template<typename TType, typename TAllocator>
typename Vector<TType, TAllocator>::size_type Vector<TType, TAllocator>::find(const TType& element) const {
  for (core::uint32 i = 0; i < size(); ++i) {
    if (at(i) == element) {
      return i;
    }
  }

  return INVALID_HANDLE;
}

template<typename TType, typename TAllocator>
bool Vector<TType, TAllocator>::contains(const TType& element) const {
  return core::find(begin(), end(), element) != end();
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::clear() {
  if (mCapacity) {
    // Clear all elements.
    destructor_policy::exec(data(), mSize);
    mSize = 0;
  }
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::resize(typename Vector<TType, TAllocator>::size_type size) {
  if (mSize > size) {
    // We need to shrink
    destructor_policy::exec(data() + size, mSize - size);
    mSize = size;
  } else if (mSize < size) {
    // We need to grow
    if (size > capacity()) {
      // We even need to increase the capacity
      resize_buffer_internal(size);
    }

    constructor_policy::exec(data() + mSize, size - mSize);
    mSize = size;
  }
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::resize(typename Vector<TType, TAllocator>::size_type size, const TType& element) {
  if (mSize > size) {
    // We need to shrink
    destructor_policy::exec(data() + size, mSize - size);
    mSize = size;
  } else if (mSize < size) {
    // We need to grow
    if (size > capacity()) {
      // We even need to increase the capacity
      resize_buffer_internal(size);
    }

    copy_constructor_policy::exec(data() + mSize, element, size - mSize);
    mSize = size;
  }
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::grow(core::uint32 numElementsToAdd) {
  const core::uint32 newSize = mSize + numElementsToAdd;
  ensure_buffer_size(newSize);
  constructor_policy::exec(data() + mSize, numElementsToAdd);
  mSize = newSize;
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::reserve(core::uint32 capacity) {
  if (this->capacity() < capacity) {
    resize_buffer_internal(capacity);
  }
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::shrink() {
  if (capacity() > mSize) {
    resize_buffer_internal(mSize);
  }
}


template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::grow_internal(core::uint32 numElementsToAdd) {
  const core::uint32 newSize = mSize + numElementsToAdd;
  ensure_buffer_size(newSize);
  mSize = newSize;
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::resize_buffer_internal(core::uint32 newCapacity) {
  if (mCapacity != newCapacity) {

    TType* newBuffer = reinterpret_cast<TType*>(mAllocator.allocate(newCapacity * sizeof(TType), alignof(TType)));

    if (mBuffer != nullptr) {
      // Check if the new buffer is smaller
      if (mSize > newCapacity) {
        core::Memory::move(newBuffer, mBuffer, newCapacity * sizeof(TType));
      } else {
        core::Memory::move(newBuffer, mBuffer, data_size());
      }
      mAllocator.deallocate(mBuffer, data_size());
    }

    mBuffer = newBuffer;

    mCapacity = newCapacity;
  }
}

template<typename TType, typename TAllocator>
void Vector<TType, TAllocator>::ensure_buffer_size(core::uint32 newSize) {
  if (newSize > capacity()) {
    // Also take current capacity in consideration for calculating new capacity
    core::uint32 newCapacity = capacity() != 0 ? capacity() : 1;
    while (newSize > newCapacity) {
      newCapacity += std::ceil(mCapacity * 0.5f);
    }
    resize_buffer_internal(newCapacity);
  }
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core