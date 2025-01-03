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
#include "core/core/non_copyable.h"
#include "core/std/math.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class Math final : public NonCopyable {
public:

  static constexpr double BE_EPSILON = 0.00001;

public:

  template<typename TType>
  [[nodiscard]] static TType clamp(const TType& x, const TType& minVal, const TType& maxVal);

  template<typename TType>
  [[nodiscard]] static TType radians(TType degrees);

  template<typename TType>
  [[nodiscard]] static TType tan(TType degrees);

  template<typename TType>
  [[nodiscard]] static TType abs(TType x);

	template<typename TType>
	[[nodiscard]] static TType min(TType a, TType b);

	template<typename TType>
	[[nodiscard]] static TType max(TType a, TType b);

  template<typename TType>
  [[nodiscard]] static bool near(TType a, TType b, TType epsilon = BE_EPSILON);

  template<typename TType>
  [[nodiscard]] static TType wrap_to_interval(TType a, TType min, TType max);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/math.inl"