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
#include "core/std/utils.h"
#include "core/std/typetraits/add_const.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
VectorSpan<TType>::VectorSpan()
: mStart(nullptr)
, mEnd(nullptr) {

}

template<typename TType>
VectorSpan<TType>::VectorSpan(TType* ptr, core::uint32 numElements)
: mStart(ptr)
, mEnd(ptr + numElements) {

}

template<typename TType>
VectorSpan<TType>::VectorSpan(TType* begin, TType* end)
: mStart(begin)
, mEnd(end) {

}

template<typename TType>
VectorSpan<TType>::~VectorSpan() {

}


template<typename TType>
TType& VectorSpan<TType>::operator[](core::uint32 index) const{
  return mStart[index];
}

template<typename TType>
void VectorSpan<TType>::swap(VectorSpan<TType>& rhs){
  core::swap(mStart, rhs.mStart);
  core::swap(mEnd, rhs.mEnd);
}


template<typename TType>
typename VectorSpan<TType>::iterator VectorSpan<TType>::begin() {
  return iterator(mStart, this);
}

template<typename TType>
typename VectorSpan<TType>::iterator VectorSpan<TType>::end() {
  return iterator(mEnd, this);
}

template<typename TType>
typename VectorSpan<TType>::const_iterator VectorSpan<TType>::begin() const {
  return const_iterator(mStart, reinterpret_cast<const VectorSpan<typename core::add_const<TType>::type>*>(this));
}

template<typename TType>
typename VectorSpan<TType>::const_iterator VectorSpan<TType>::end() const {
  return const_iterator(mEnd, reinterpret_cast<const VectorSpan<typename core::add_const<TType>::type>*>(this));
}

template<typename TType>
typename VectorSpan<TType>::const_iterator VectorSpan<TType>::cbegin() const {
  return begin();
}

template<typename TType>
typename VectorSpan<TType>::const_iterator VectorSpan<TType>::cend() const {
  return end();
}


template<typename TType>
template<typename TOtherType>
bool VectorSpan<TType>::contains(const TOtherType& element) const {
  return core::find(begin(), end(), element) != end();
}

template<typename TType>
constexpr bool VectorSpan<TType>::empty() const{
  return (mStart == mEnd);
}

template<typename TType>
constexpr core::uint32 VectorSpan<TType>::size() const{
  return static_cast<core::uint32>(mEnd - mStart);
}

template<typename TType>
constexpr core::uint32 VectorSpan<TType>::data_size() const{
  return (size() * sizeof(TType));
}

template<typename TType>
constexpr TType* VectorSpan<TType>::data() const{
  return mStart;
}

template<typename TType>
TType& VectorSpan<TType>::front() const{
  return *mStart;
}

template<typename TType>
TType& VectorSpan<TType>::back() const{
  return *(mEnd - 1);
}

template<typename TType>
TType& VectorSpan<TType>::at(core::uint32 index) const{
  return mStart[index];
}


template<typename TType>
VectorSpan<TType> VectorSpan<TType>::first(core::uint32 numElements) const{
  return VectorSpan<TType>(mStart, mStart + numElements);
}

template<typename TType>
VectorSpan<TType> VectorSpan<TType>::last(core::uint32 numElements) const{
  return VectorSpan<TType>(mEnd - numElements, mEnd);
}

template<typename TType>
VectorSpan<TType> VectorSpan<TType>::subspan(core::uint32 offset, core::uint32 numElements) const{
  return VectorSpan<TType>(mStart + offset, mStart + offset + numElements);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core