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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TSpan>
VectorSpanIterator<TSpan>::VectorSpanIterator()
: mPtr(nullptr)
, mSpanContainer(nullptr) {

}

template<typename TSpan>
VectorSpanIterator<TSpan>::VectorSpanIterator(const VectorSpanIterator<TSpan> &rhs)
: mPtr(rhs.mPtr)
, mSpanContainer(rhs.mSpanContainer) {

}

template<typename TSpan>
VectorSpanIterator<TSpan>::VectorSpanIterator(TType *ptr, const TSpan *spanContainer)
: mPtr(ptr)
, mSpanContainer(spanContainer) {

}

template<typename TSpan>
VectorSpanIterator<TSpan> &VectorSpanIterator<TSpan>::operator=(const VectorSpanIterator<TSpan> &rhs) {
  mPtr = rhs.mPtr;
  mSpanContainer = rhs.mSpanContainer;

  return *this;
}

template<typename TSpan>
constexpr bool VectorSpanIterator<TSpan>::operator==(const VectorSpanIterator<TSpan> &rhs) const {
  return ((mPtr == rhs.mPtr) && (mSpanContainer == rhs.mSpanContainer));
}

template<typename TSpan>
constexpr bool VectorSpanIterator<TSpan>::operator!=(const VectorSpanIterator<TSpan> &rhs) const {
  return !operator==(rhs);
}

template<typename TSpan>
constexpr bool VectorSpanIterator<TSpan>::operator<(const VectorSpanIterator<TSpan> &rhs) const {
  return (mPtr < rhs.mPtr);
}

template<typename TSpan>
constexpr bool VectorSpanIterator<TSpan>::operator<=(const VectorSpanIterator<TSpan> &rhs) const {
  return (mPtr <= rhs.mPtr);
}

template<typename TSpan>
constexpr bool VectorSpanIterator<TSpan>::operator>(const VectorSpanIterator<TSpan> &rhs) const {
  return (mPtr > rhs.mPtr);
}

template<typename TSpan>
constexpr bool VectorSpanIterator<TSpan>::operator>=(const VectorSpanIterator<TSpan> &rhs) const {
  return (mPtr >= rhs.mPtr);
}

template<typename TSpan>
VectorSpanIterator<TSpan> VectorSpanIterator<TSpan>::operator+(core::int64 index) const {
  return VectorSpanIterator(mPtr + index, mSpanContainer);
}

template<typename TSpan>
VectorSpanIterator<TSpan> VectorSpanIterator<TSpan>::operator-(core::int64 index) const {
  return VectorSpanIterator(mPtr - index, mSpanContainer);
}

template<typename TSpan>
VectorSpanIterator<TSpan> &VectorSpanIterator<TSpan>::operator+=(core::int64 index) {
  mPtr += index;

  return *this;
}

template<typename TSpan>
VectorSpanIterator<TSpan> &VectorSpanIterator<TSpan>::operator-=(core::int64 index) {
  mPtr -= index;

  return *this;
}

template<typename TSpan>
typename VectorSpanIterator<TSpan>::reference VectorSpanIterator<TSpan>::operator*() const {
  return *mPtr;
}

template<typename TSpan>
typename VectorSpanIterator<TSpan>::pointer VectorSpanIterator<TSpan>::operator->() const {
  return mPtr;
}

template<typename TSpan>
typename VectorSpanIterator<TSpan>::reference VectorSpanIterator<TSpan>::operator[](core::int64 index) const {
  return *(mPtr + index);
}

template<typename TSpan>
VectorSpanIterator<TSpan> &VectorSpanIterator<TSpan>::operator++() {
  ++mPtr;

  return *this;
}

template<typename TSpan>
VectorSpanIterator<TSpan> VectorSpanIterator<TSpan>::operator++(int) {
  VectorSpanIterator result(*this);
  ++mPtr;

  return result;
}

template<typename TSpan>
VectorSpanIterator<TSpan> &VectorSpanIterator<TSpan>::operator--() {
  --mPtr;

  return *this;
}

template<typename TSpan>
VectorSpanIterator<TSpan> VectorSpanIterator<TSpan>::operator--(int) {
  VectorSpanIterator result(*this);
  --mPtr;

  return result;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core