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
#include "core/std/typetraits/is_nothrow_move_assignable.h"


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
/**
 * @class
 * Pair
 *
 * @brief
 * A pair of objects.
 */
template <typename T1, typename T2>
class Pair {
public:
  using first_type = T1;
  using second_type = T2;

  // Default values
  T1 first;
  T2 second;


  /**
   * @brief
   * Construct a pair from nothing.
   *
   * @warning
   * This function is marked as `noexcept` to indicate that it never throws an exception.
   */
  constexpr Pair() noexcept;

  /**
   * @brief
   * Construct a pair from two values.
   *
   * @param[in] x The first value.
   * @param[in] y The second value.
   */
  constexpr Pair(const T1& x, const T2& y);

  /**
   * @brief
   * Construct a pair from a movable value and a const value.
   *
   * @param[in] x The first movable value.
   * @param[in] y The second const value.
   */
  template<typename U1 = T1, typename U2 = T2>
  constexpr Pair(U1&& x, const U2& y);

  /**
   * @brief
   * Construct a pair from a const value and a movable value.
   *
   * @param[in] x The first const value.
   * @param[in] y The second movable value.
   */
  template<typename U1 = T1, typename U2 = T2>
  constexpr Pair(const U1& x, U2&& y);

  /**
   * @brief
   * Construct a pair from two movable values.
   *
   * @param[in] x The first movable value.
   * @param[in] y The second movable value.
   */
  template<typename U1 = T1, typename U2 = T2>
  constexpr Pair(U1&& x, U2&& y);

  /**
   * @brief
   * Construct a pair from a const pair.
   *
   * @param[in] other The const pair to construct from.
   */
  template<typename U1, typename U2>
  constexpr explicit Pair(const Pair<U1, U2>& other);

  /**
   * @brief
   * Construct a pair from a movable pair.
   *
   * @param[in] other The movable pair to construct from.
   */
  template<typename U1, typename U2>
  constexpr explicit Pair(Pair<U1, U2>&& other);

  /**
   * @brief
   * Assignment operators.
   */
  constexpr Pair& operator=(const Pair& other) = default;

  /**
   * @brief
   * Move assignment operator.
   *
   * @details
   * Assigns the contents of another pair to this pair using move semantics.
   * This operator is marked as `noexcept` if both `T1` and `T2` are nothrow move assignable.
   *
   * @param[in] other The pair to move from.
   *
   * @return
   * A reference to this pair after assignment.
   */
  constexpr Pair& operator=(Pair&& other) noexcept(core::is_nothrow_move_assignable_v<T1> && core::is_nothrow_move_assignable_v<T2>) = default;

/**
 * @brief
 * Assigns the contents of another pair to this pair.
 *
 * @param[in] other The pair whose contents are to be assigned.
 *
 * @return
 * A reference to this pair after assignment.
 */
  template<typename U1, typename U2>
  constexpr Pair& operator=(const Pair<U1, U2>& other);

  /**
   * @brief
   * Assigns the contents of another pair to this pair.
   *
   * @details
   * This assignment operator is implemented by calling the assignment operators of the
   * first and second types. The assignment operators for the first and second types are
   * called in order to assign the contents of another pair to this pair.
   *
   * @param[in] other The pair whose contents are to be assigned.
   *
   * @return
   * A reference to this pair after assignment.
   */
  template<typename U1, typename U2>
  constexpr Pair& operator=(Pair<U1, U2>&& other);

  /**
   * @brief
   * Swaps the contents of this pair with another pair.
   *
   * @details
   * This function swaps the first and second elements of this pair with the
   * first and second elements of the provided pair. This operation is
   * marked as `noexcept`, indicating it does not throw exceptions.
   *
   * @param[in,out] other The pair to swap contents with.
   */
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