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
#include "core/std/algorithm.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TSort>
Heap<TType, TSort>::Heap() {}

template<typename TType, typename TSort>
Heap<TType, TSort>::Heap(const Heap& rhs)
: mData(rhs.mData) {

}

template<typename TType, typename TSort>
Heap<TType, TSort>::Heap(Heap&& rhs)
: mData(rhs.mData) {

}


template<typename TType, typename TSort>
Heap<TType, TSort>& Heap<TType, TSort>::operator=(const Heap& rhs) {
  mData = rhs.mData;
  return *this;
}

template<typename TType, typename TSort>
Heap<TType, TSort>& Heap<TType, TSort>::operator=(Heap&& rhs) {
  mData = rhs.mData;
  return *this;
}

template<typename TType, typename TSort>
bool Heap<TType, TSort>::operator==(const Heap& rhs) const {
  return mData == rhs.mData;
}

template<typename TType, typename TSort>
bool Heap<TType, TSort>::operator!=(const Heap& rhs) const {
  return !operator==(rhs);
}

template<typename TType, typename TSort>
const TType& Heap<TType, TSort>::operator[](core::uint32 index) const {
  return mData[index];
}

template<typename TType, typename TSort>
TType& Heap<TType, TSort>::operator[](core::uint32 index) {
  return mData[index];
}


template<typename TType, typename TSort>
void Heap<TType, TSort>::swap(Heap& rhs) {
  mData.swap(rhs.mData);
}


template<typename TType, typename TSort>
const TType* Heap<TType, TSort>::data() const {
  return mData.data();
}

template<typename TType, typename TSort>
core::uint32 Heap<TType, TSort>::size() const {
  return mData.size();
}

template<typename TType, typename TSort>
core::uint32 Heap<TType, TSort>::data_size() const {
  return mData.data_size();
}

template<typename TType, typename TSort>
core::uint32 Heap<TType, TSort>::capacity() const {
  return mData.capacity();
}

template<typename TType, typename TSort>
core::uint32 Heap<TType, TSort>::data_capacity() const {
  return mData.data_capacity();
}

template<typename TType, typename TSort>
bool Heap<TType, TSort>::empty() const {
  return mData.empty();
}


template<typename TType, typename TSort>
typename Heap<TType, TSort>::iterator Heap<TType, TSort>::begin() const {
  return mData.begin();
}

template<typename TType, typename TSort>
typename Heap<TType, TSort>::iterator Heap<TType, TSort>::end() const {
  return mData.end();
}


template<typename TType, typename TSort>
const TType& Heap<TType, TSort>::front() const {
  return mData.front();
}

template<typename TType, typename TSort>
void Heap<TType, TSort>::push_heap(const TType& element) {
  mData.push_back(element);
  core::push_heap(begin(), end(), mSortPredicate);
}

template<typename TType, typename TSort>
void Heap<TType, TSort>::push_heap(TType&& element) {
  mData.push_back(element);
  core::push_heap(begin(), end(), mSortPredicate);
}

template<typename TType, typename TSort>
TType Heap<TType, TSort>::pop_heap() {
  core::pop_heap(begin(), end(), mSortPredicate);
  return mData.pop_back();
}


template<typename TType, typename TSort>
typename Heap<TType, TSort>::container_type Heap<TType, TSort>::erase(iterator iter) {
  container_result result = mData.erase(iter);

  heapify();

  return result;
}


template<typename TType, typename TSort>
bool Heap<TType, TSort>::contains(const TType& element) {
  return mData.contains(element);
}

template<typename TType, typename TSort>
void Heap<TType, TSort>::clear() {
  return mData.clear();
}

template<typename TType, typename TSort>
void Heap<TType, TSort>::reserve(core::uint32 capacity) {
  return mData.reserve(capacity);
}

template<typename TType, typename TSort>
void Heap<TType, TSort>::shrink() {
  return mData.shrink();
}

template<typename TType, typename TSort>
void Heap<TType, TSort>::heapify() {
  core::make_heap(begin(), end(), mSortPredicate);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core