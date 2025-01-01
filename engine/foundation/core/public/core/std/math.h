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


namespace Internal
{
template<typename T>
constexpr T lerp(T a, T b, T t) noexcept
{
  if ((a <= 0 && b >= 0) || (a >= 0 && b <= 0))
  {
    return t * b + (1 - t) * a;
  }
  if (t == 1)
  {
    return b;
  }
  const T x = a + t * (b - a);
  if ((t > 1) == (b > a))
  {
    return b < x ? x : b;
  }
  else
  {
    return x < b ? x : b;
  }
}
}


using std::abs;
using std::acos;
using std::acosh;
using std::asin;
using std::asinh;
using std::atan;
using std::atanh;
using std::atan2;
using std::ceil;
using std::cos;
using std::cosh;
using std::exp;
using std::exp2;
using std::floor;
using std::fmod;
using std::llround;
using std::lround;
using std::modf;
using std::pow;
using std::round;
using std::sin;
using std::sinh;
using std::sqrt;
using std::tan;
using std::tanh;
using std::trunc;
using std::log;
using std::log10;
using std::log2;
using std::remainder;


constexpr float lerp(float a, float b, float t) noexcept {
  return Internal::lerp(a, b, t);
}

constexpr double lerp(double a, double b, double t) noexcept {
  return Internal::lerp(a, b, t);
}

constexpr long double lerp(long double a, long double b, long double t) noexcept {
  return Internal::lerp(a, b, t);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}