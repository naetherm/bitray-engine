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
#include "core/core/non_copyable.h"
#include "core/std/math.h"
#include "core/math/vec2.h"
#include "core/math/vec3.h"
#include "core/math/vec4.h"


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

	// "core::Vec2" constants
	static const core::Vec2f VEC2_ZERO;			///< 0 0
	static const core::Vec2f VEC2_ONE;			///< 1 1
	static const core::Vec2f VEC2_UNIT_X;		///< 1 0
	static const core::Vec2f VEC2_UNIT_Y;		///< 0 1

	// "core::Vec3" constants
	static const core::Vec3f VEC3_ZERO;			///< 0 0 0
	static const core::Vec3f VEC3_ONE;			///< 1 1 1
	static const core::Vec3f VEC3_UNIT_X;		///< 1 0 0
	static const core::Vec3f VEC3_UNIT_Y;		///< 0 1 0
	static const core::Vec3f VEC3_UNIT_Z;		///< 0 0 1
	// "core::Vec3d" constants
	static const core::Vec3d DVEC3_ZERO;		///< 0 0 0
	static const core::Vec3d DVEC3_ONE;		///< 1 1 1
	static const core::Vec3d DVEC3_UNIT_X;	///< 1 0 0
	static const core::Vec3d DVEC3_UNIT_Y;	///< 0 1 0
	static const core::Vec3d DVEC3_UNIT_Z;	///< 0 0 1
	// "core::Vec4f" constants
	static const core::Vec4f VEC4_ZERO;			///< 0 0 0 0
	static const core::Vec4f VEC4_ONE;			///< 1 1 1 1
	static const core::Vec4f VEC4_UNIT_X;		///< 1 0 0 0
	static const core::Vec4f VEC4_UNIT_Y;		///< 0 1 0 0
	static const core::Vec4f VEC4_UNIT_Z;		///< 0 0 1 0
	static const core::Vec4f VEC4_UNIT_W;		///< 0 0 0 1
	// "core::Vec4d" constants
	static const core::Vec4d DVEC4_ZERO;		///< 0 0 0 0
	static const core::Vec4d DVEC4_ONE;		///< 1 1 1 1
	static const core::Vec4d DVEC4_UNIT_X;		///< 1 0 0 0
	static const core::Vec4d DVEC4_UNIT_Y;		///< 0 1 0 0
	static const core::Vec4d DVEC4_UNIT_Z;		///< 0 0 1 0
	static const core::Vec4d DVEC4_UNIT_W;		///< 0 0 0 1
	// "glm::mat4" constants
	//static const glm::mat4 MAT4_IDENTITY;
	// "glm::dmat4" constants
	//static const glm::dmat4 DMAT4_IDENTITY;
	// "glm::quat" constants
	//static const glm::quat QUAT_IDENTITY;
	// "glm::squat" constants
	//static const glm::dquat DQUAT_IDENTITY;

public:

  template<typename TType>
  [[nodiscard]] static TType clamp(const TType& x, const TType& minVal, const TType& maxVal);

  template<typename TType>
	[[nodiscard]] static TType radians(TType degrees);

	template<typename TType>
	[[nodiscard]] static TType degrees(TType degrees);

  template<typename TType>
  [[nodiscard]] static TType tan(TType degrees);

  template<typename TType>
  [[nodiscard]] static TType abs(TType x);

	template<typename TType>
	[[nodiscard]] static TType min(TType a, TType b);

	template<typename TType>
	[[nodiscard]] static TType max(TType a, TType b);

	template<typename TType>
	[[nodiscard]] static TType sqrt(TType a);

	template<typename TType>
	[[nodiscard]] static TType sin(TType a);

	template<typename TType>
	[[nodiscard]] static TType cos(TType a);

	template<typename TType>
	[[nodiscard]] static TType acos(TType a);

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