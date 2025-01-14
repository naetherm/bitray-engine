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

namespace internal {
template<typename TType>
[[nodiscard]] static inline TType max(TType a, TType b) {
  return std::max(a, b);
}
template<typename TType>
[[nodiscard]] static inline TType min(TType a, TType b) {
  return std::min(a, b);
}
}



template<typename TType>
[[nodiscard]] TType Math::clamp(const TType& x, const TType& minVal, const TType& maxVal) {
  return internal::min(internal::max(x, minVal), maxVal);
}

template<typename TType>
[[nodiscard]] TType Math::radians(TType degrees) {
  return degrees * M_PI / 180.0;
}

template<typename TType>
[[nodiscard]] TType Math::tan(TType degrees) {
  return std::tan(degrees);
}


template<typename TType>
[[nodiscard]] TType Math::abs(TType x) {
  return x < TType(0) ? -x : x;
}

template<typename TType>
[[nodiscard]] TType Math::min(TType a, TType b) {
  return a < b ? a : b;
}

template<typename TType>
[[nodiscard]] TType Math::max(TType a, TType b) {
  return a > b ? a : b;
}

template<typename TType>
[[nodiscard]] TType Math::sqrt(TType a) {
  return ::sqrt(a);
}

template<typename TType>
[[nodiscard]] TType Math::sin(TType a) {
  return ::sin(a);
}

template<typename TType>
[[nodiscard]] TType Math::cos(TType a) {
  return ::cos(a);
}

template<typename TType>
[[nodiscard]] TType Math::acos(TType a) {
  return ::acos(a);
}

template<typename TType>
[[nodiscard]] bool Math::near(TType a, TType b, TType epsilon) {
  return Math::abs(a - b) < epsilon;
}

template<typename TType>
TType Math::wrap_to_interval(TType a, TType min, TType max) {
  const TType interval = max - min;
  return interval ? (a - floor((a - min) / interval) * (max - min)) : min;
}



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core