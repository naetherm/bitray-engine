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
#include "core/std/utils.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template <typename T1, typename T2>
constexpr Pair<T1, T2>::Pair() noexcept
: first()
, second() {}

// Constructor from values
template <typename T1, typename T2>
constexpr Pair<T1, T2>::Pair(const T1& x, const T2& y)
: first(x), second(y) {}

// Constructor from value and movable value
template <typename T1, typename T2>
template<typename U1, typename U2>
constexpr Pair<T1, T2>::Pair(U1&& x, const U2& y)
: first(core::forward<U1>(x)), second(y) {}

// Constructor from const value and movable value
template <typename T1, typename T2>
template<typename U1, typename U2>
constexpr Pair<T1, T2>::Pair(const U1& x, U2&& y)
: first(x), second(core::forward<U2>(y)) {}

// Constructor from movable values
template <typename T1, typename T2>
template<typename U1, typename U2>
constexpr Pair<T1, T2>::Pair(U1&& x, U2&& y)
: first(core::forward<U1>(x)), second(core::forward<U2>(y)) {}

// Constructor from different Pair
template <typename T1, typename T2>
template<typename U1, typename U2>
constexpr Pair<T1, T2>::Pair(const Pair<U1, U2>& other)
: first(other.first), second(other.second) {}

// Constructor from different movable Pair
template <typename T1, typename T2>
template<typename U1, typename U2>
constexpr Pair<T1, T2>::Pair(Pair<U1, U2>&& other)
: first(core::forward<U1>(other.first)), second(core::forward<U2>(other.second)) {}


template <typename T1, typename T2>
template<typename U1, typename U2>
constexpr Pair<T1, T2>& Pair<T1, T2>::operator=(const Pair<U1, U2>& other) {
  first = other.first;
  second = other.second;
  return *this;
}

template <typename T1, typename T2>
template<typename U1, typename U2>
constexpr Pair<T1, T2>& Pair<T1, T2>::operator=(Pair<U1, U2>&& other) {
  first = core::forward<U1>(other.first);
  second = core::forward<U2>(other.second);
  return *this;
}

// Swap function
template <typename T1, typename T2>
constexpr void Pair<T1, T2>::swap(Pair<T1, T2>& other) noexcept {
  using core::swap;
  swap(first, other.first);
  swap(second, other.second);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}