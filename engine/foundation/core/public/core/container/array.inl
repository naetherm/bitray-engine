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
template<typename T, core::sizeT N>
inline void Array<T, N>::fill(const Array<T, N>::value_type &value) {
  core::fill_n(&mValue[0], N, value);
}


template<typename T, core::sizeT N>
inline void Array<T, N>::swap(this_type &x) {
  for (core::sizeT i = 0; i < N; ++i) {
    T temp = x.mValue[i];
    x.mValue[i] = mValue[i];
    mValue[i] = temp;
  }
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::iterator
Array<T, N>::begin() noexcept {
  return &mValue[0];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_iterator
Array<T, N>::begin() const noexcept {
  return &mValue[0];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_iterator
Array<T, N>::cbegin() const noexcept {
  return &mValue[0];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::iterator
Array<T, N>::end() noexcept {
  return &mValue[N];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_iterator
Array<T, N>::end() const noexcept {
  return &mValue[N];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_iterator
Array<T, N>::cend() const noexcept {
  return &mValue[N];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::reverse_iterator
Array<T, N>::rbegin() noexcept {
  return reverse_iterator(&mValue[N]);
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_reverse_iterator
Array<T, N>::rbegin() const noexcept {
  return const_reverse_iterator(&mValue[N]);
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_reverse_iterator
Array<T, N>::crbegin() const noexcept {
  return const_reverse_iterator(&mValue[N]);
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::reverse_iterator
Array<T, N>::rend() noexcept {
  return reverse_iterator(&mValue[0]);
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_reverse_iterator
Array<T, N>::rend() const noexcept {
  return const_reverse_iterator(static_cast<const_iterator>(&mValue[0]));
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_reverse_iterator
Array<T, N>::crend() const noexcept {
  return const_reverse_iterator(static_cast<const_iterator>(&mValue[0]));
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::size_type
Array<T, N>::size() const noexcept {
  return (size_type) N;
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::size_type
Array<T, N>::max_size() const noexcept {
  return (size_type) N;
}


template<typename T, core::sizeT N>
constexpr inline bool Array<T, N>::empty() const noexcept {
  return (N == 0);
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::reference
Array<T, N>::operator[](size_type i) {
  return mValue[i];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_reference
Array<T, N>::operator[](size_type i) const {
  return mValue[i];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::reference
Array<T, N>::front() {
  return mValue[0];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_reference
Array<T, N>::front() const {
  return mValue[0];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::reference
Array<T, N>::back() {
  return mValue[N - 1];
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_reference
Array<T, N>::back() const {
  return mValue[N - 1];
}


template<typename T, core::sizeT N>
constexpr inline T *Array<T, N>::data() noexcept {
  return mValue;
}


template<typename T, core::sizeT N>
constexpr inline const T *Array<T, N>::data() const noexcept {
  return mValue;
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::const_reference Array<T, N>::at(size_type i) const {
  return static_cast<const_reference>(mValue[i]);
}


template<typename T, core::sizeT N>
constexpr inline typename Array<T, N>::reference Array<T, N>::at(size_type i) {
  return static_cast<reference>(mValue[i]);
}


template<typename T, core::sizeT N>
inline bool Array<T, N>::validate() const {
  return true; // There is nothing to do.
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core