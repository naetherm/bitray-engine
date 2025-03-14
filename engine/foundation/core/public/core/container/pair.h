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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


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
class Pair {
public:
  using first_type = T1;
  using second_type = T2;

  // Default values
  T1 first;
  T2 second;

  // Default constructor
  constexpr Pair() noexcept;

  // Constructor from values
  constexpr Pair(const T1& x, const T2& y);

  // Constructor from value and movable value
  template<typename U1 = T1, typename U2 = T2>
  constexpr Pair(U1&& x, const U2& y);

  // Constructor from const value and movable value
  template<typename U1 = T1, typename U2 = T2>
  constexpr Pair(const U1& x, U2&& y);

  // Constructor from movable values
  template<typename U1 = T1, typename U2 = T2>
  constexpr Pair(U1&& x, U2&& y);

  // Constructor from different Pair
  template<typename U1, typename U2>
  constexpr Pair(const Pair<U1, U2>& other);

  // Constructor from different movable Pair
  template<typename U1, typename U2>
  constexpr Pair(Pair<U1, U2>&& other);

  // Assignment operators
  constexpr Pair& operator=(const Pair& other) = default;

  constexpr Pair& operator=(Pair&& other)
      noexcept(std::is_nothrow_move_assignable_v<T1> && std::is_nothrow_move_assignable_v<T2>) = default;

  template<typename U1, typename U2>
  constexpr Pair& operator=(const Pair<U1, U2>& other);

  template<typename U1, typename U2>
  constexpr Pair& operator=(Pair<U1, U2>&& other);

  // Swap function
  constexpr void swap(Pair& other) noexcept;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/container/pair.inl"