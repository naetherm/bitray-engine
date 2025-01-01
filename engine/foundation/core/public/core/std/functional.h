////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
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
#include "core/std/typetraits/void_t.h"
#include "core/std/utils.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<class T = void>
struct plus {
  constexpr T operator()(const T& l, const T& r) const { return l + r; }
};
template<>
struct plus<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) + core::forward<U>(r)) {
    return core::forward<T>(l) + core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct minus {
  constexpr T operator()(const T& l, const T& r) const { return l - r; }
};
template<>
struct minus<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) - core::forward<U>(r)) {
    return core::forward<T>(l) - core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct multiplies {
  constexpr T operator()(const T& l, const T& r) const { return l * r; }
};
template<>
struct multiplies<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) * core::forward<U>(r)) {
    return core::forward<T>(l) * core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct divides {
  constexpr T operator()(const T& l, const T& r) const { return l / r; }
};
template<>
struct divides<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) / core::forward<U>(r)) {
    return core::forward<T>(l) / core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct modulus {
  constexpr T operator()(const T& l, const T& r) const { return l % r; }
};
template<>
struct modulus<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) % core::forward<U>(r)) {
    return core::forward<T>(l) % core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct negate {
  constexpr T operator()(const T& l) const { return -l; }
};
template<>
struct negate<void> {
  template<class T>
  constexpr auto operator()(T&& l) const -> decltype(-core::forward<T>(l)) {
    return -core::forward<T>(l);
  }
  using is_transparent = void;
};

template<class T = void>
struct equal_to {
  constexpr bool operator()(const T& l, const T& r) const { return l == r; }
};
template<>
struct equal_to<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) == core::forward<U>(r)) {
    return core::forward<T>(l) == core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct not_equal_to {
  constexpr bool operator()(const T& l, const T& r) const { return l != r; }
};
template<>
struct not_equal_to<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) != core::forward<U>(r)) {
    return core::forward<T>(l) != core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct greater {
  constexpr bool operator()(const T& l, const T& r) const { return l > r; }
};
template<>
struct greater<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) > core::forward<U>(r)) {
    return core::forward<T>(l) > core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct less {
  constexpr bool operator()(const T& l, const T& r) const { return l < r; }
};
template<>
struct less<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) < core::forward<U>(r)) {
    return core::forward<T>(l) < core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct greater_equal {
  constexpr bool operator()(const T& l, const T& r) const { return l >= r; }
};
template<>
struct greater_equal<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) >= core::forward<U>(r)) {
    return core::forward<T>(l) >= core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct less_equal {
  constexpr bool operator()(const T& l, const T& r) const { return l <= r; }
};
template<>
struct less_equal<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) <= core::forward<U>(r)) {
    return core::forward<T>(l) <= core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct logical_and {
  constexpr bool operator()(const T& l, const T& r) const { return l && r; }
};
template<>
struct logical_and<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) && core::forward<U>(r)) {
    return core::forward<T>(l) && core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct logical_or {
  constexpr bool operator()(const T& l, const T& r) const { return l != r; }
};
template<>
struct logical_or<void> {
  template<class T, class U>
  constexpr auto operator()(T&& l, U&& r) const -> decltype(core::forward<T>(l) != core::forward<U>(r)) {
    return core::forward<T>(l) != core::forward<U>(r);
  }
  using is_transparent = void;
};

template<class T = void>
struct logical_not {
  constexpr bool operator()(const T& l) const { return !l; }
};
template<>
struct logical_not<void> {
  template<class T>
  constexpr auto operator()(T&& l) const -> decltype(!core::forward<T>(l)) {
    return !core::forward<T>(l);
  }
  using is_transparent = void;
};

template <class T = void>
struct bit_and
{
  constexpr T operator()(const T& left, const T& right) const { return left & right; }
};

template <>
struct bit_and<void>
{
  template <class T, class U>
  constexpr auto operator()(T&& left, U&& right) const -> decltype(core::forward<T>(left) & core::forward<U>(right))
  {
    return core::forward<T>(left) & core::forward<U>(right);
  }
  using is_transparent = void;
};

template <class T = void>
struct bit_or
{
  constexpr T operator()(const T& left, const T& right) const { return left | right; }
};

template <>
struct bit_or<void>
{
  template <class T, class U>
  constexpr auto operator()(T&& left, U&& right) const -> decltype(core::forward<T>(left) | core::forward<U>(right))
  {
    return core::forward<T>(left) | core::forward<U>(right);
  }
  using is_transparent = void;
};

template <class T = void>
struct bit_xor
{
  constexpr T operator()(const T& left, const T& right) const { return left ^ right; }
};

template <>
struct bit_xor<void>
{
  template <class T, class U>
  constexpr auto operator()(T&& left, U&& right) const -> decltype(core::forward<T>(left) ^ core::forward<U>(right))
  {
    return core::forward<T>(left) ^ core::forward<U>(right);
  }
  using is_transparent = void;
};

template <class T = void>
struct bit_not
{
  constexpr T operator()(const T& left) const { return ~left; }
};

template <>
struct bit_not<void>
{
  template <class T>
  constexpr auto operator()(T&& left) const -> decltype(~core::forward<T>(left))
  {
    return ~core::forward<T>(left);
  }
  using is_transparent = void;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}