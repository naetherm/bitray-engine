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
#include "core/math/math.h"
#include "core/core/assert_impl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]

// "core::Vec2f" constants
const core::Vec2f Math::VEC2_ZERO		   = core::Vec2f(0.0f, 0.0f);
const core::Vec2f Math::VEC2_ONE		   = core::Vec2f(1.0f, 1.0f);
const core::Vec2f Math::VEC2_UNIT_X	   = core::Vec2f(1.0f, 0.0f);
const core::Vec2f Math::VEC2_UNIT_Y	   = core::Vec2f(0.0f, 1.0f);

// "core::Vec3f" constants
const core::Vec3f Math::VEC3_ZERO		   = core::Vec3f(0.0f, 0.0f, 0.0f);
const core::Vec3f Math::VEC3_ONE		   = core::Vec3f(1.0f, 1.0f, 1.0f);
const core::Vec3f Math::VEC3_UNIT_X	   = core::Vec3f(1.0f, 0.0f, 0.0f);
const core::Vec3f Math::VEC3_UNIT_Y	   = core::Vec3f(0.0f, 1.0f, 0.0f);
const core::Vec3f Math::VEC3_UNIT_Z	   = core::Vec3f(0.0f, 0.0f, 1.0f);
// "core::Vec3d" constants
const core::Vec3d Math::DVEC3_ZERO      = core::Vec3d(0.0, 0.0, 0.0);
const core::Vec3d Math::DVEC3_ONE       = core::Vec3d(1.0, 1.0, 1.0);
const core::Vec3d Math::DVEC3_UNIT_X    = core::Vec3d(1.0, 0.0, 0.0);
const core::Vec3d Math::DVEC3_UNIT_Y    = core::Vec3d(0.0, 1.0, 0.0);
const core::Vec3d Math::DVEC3_UNIT_Z    = core::Vec3d(0.0, 0.0, 1.0);
// "core::Vec4f" constants
const core::Vec4f Math::VEC4_ZERO        = core::Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
const core::Vec4f Math::VEC4_ONE         = core::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
const core::Vec4f Math::VEC4_UNIT_X      = core::Vec4f(1.0f, 0.0f, 0.0f, 0.0f);
const core::Vec4f Math::VEC4_UNIT_Y      = core::Vec4f(0.0f, 1.0f, 0.0f, 0.0f);
const core::Vec4f Math::VEC4_UNIT_Z      = core::Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
const core::Vec4f Math::VEC4_UNIT_W      = core::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
// "core::Vec4d" constants
const core::Vec4d Math::DVEC4_ZERO      = core::Vec4d(0.0, 0.0, 0.0, 0.0);
const core::Vec4d Math::DVEC4_ONE       = core::Vec4d(1.0, 1.0, 1.0, 1.0);
const core::Vec4d Math::DVEC4_UNIT_X    = core::Vec4d(1.0, 0.0, 0.0, 0.0);
const core::Vec4d Math::DVEC4_UNIT_Y    = core::Vec4d(0.0, 1.0, 0.0, 0.0);
const core::Vec4d Math::DVEC4_UNIT_Z    = core::Vec4d(0.0, 0.0, 1.0, 0.0);
const core::Vec4d Math::DVEC4_UNIT_W    = core::Vec4d(0.0, 0.0, 0.0, 1.0);
// "glm::mat4" constants
// const glm::mat4 Math::MAT4_IDENTITY = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
//                         0.0f, 1.0f, 0.0f, 0.0f,
//                         0.0f, 0.0f, 1.0f, 0.0f,
//                         0.0f, 0.0f, 0.0f, 1.0f);
// "glm::dmat4" constants
// const glm::dmat4 Math::DMAT4_IDENTITY = glm::dmat4(1.0, 0.0, 0.0, 0.0,
//                            0.0, 1.0, 0.0, 0.0,
//                            0.0, 0.0, 1.0, 0.0,
//                            0.0, 0.0, 0.0, 1.0);
// "glm::quat" constants
// const glm::quat Math::QUAT_IDENTITY = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
// const glm::dquat Math::DQUAT_IDENTITY = glm::dquat(1.0, 0.0, 0.0, 0.0);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core