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

// "glm::vec2" constants
const glm::vec2 Math::VEC2_ZERO		   = glm::vec2(0.0f, 0.0f);
const glm::vec2 Math::VEC2_ONE		   = glm::vec2(1.0f, 1.0f);
const glm::vec2 Math::VEC2_UNIT_X	   = glm::vec2(1.0f, 0.0f);
const glm::vec2 Math::VEC2_UNIT_Y	   = glm::vec2(0.0f, 1.0f);

// "glm::vec3" constants
const glm::vec3 Math::VEC3_ZERO		   = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 Math::VEC3_ONE		   = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 Math::VEC3_UNIT_X	   = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Math::VEC3_UNIT_Y	   = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Math::VEC3_UNIT_Z	   = glm::vec3(0.0f, 0.0f, 1.0f);
// "glm::dvec3" constants
const glm::dvec3 Math::DVEC3_ZERO      = glm::dvec3(0.0, 0.0, 0.0);
const glm::dvec3 Math::DVEC3_ONE       = glm::dvec3(1.0, 1.0, 1.0);
const glm::dvec3 Math::DVEC3_UNIT_X    = glm::dvec3(1.0, 0.0, 0.0);
const glm::dvec3 Math::DVEC3_UNIT_Y    = glm::dvec3(0.0, 1.0, 0.0);
const glm::dvec3 Math::DVEC3_UNIT_Z    = glm::dvec3(0.0, 0.0, 1.0);
// "glm::vec4" constants
const glm::vec4 Math::VEC4_ZERO        = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
const glm::vec4 Math::VEC4_ONE         = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 Math::VEC4_UNIT_X      = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
const glm::vec4 Math::VEC4_UNIT_Y      = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
const glm::vec4 Math::VEC4_UNIT_Z      = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
const glm::vec4 Math::VEC4_UNIT_W      = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
// "glm::dvec4" constants
const glm::dvec4 Math::DVEC4_ZERO      = glm::dvec4(0.0, 0.0, 0.0, 0.0);
const glm::dvec4 Math::DVEC4_ONE       = glm::dvec4(1.0, 1.0, 1.0, 1.0);
const glm::dvec4 Math::DVEC4_UNIT_X    = glm::dvec4(1.0, 0.0, 0.0, 0.0);
const glm::dvec4 Math::DVEC4_UNIT_Y    = glm::dvec4(0.0, 1.0, 0.0, 0.0);
const glm::dvec4 Math::DVEC4_UNIT_Z    = glm::dvec4(0.0, 0.0, 1.0, 0.0);
const glm::dvec4 Math::DVEC4_UNIT_W    = glm::dvec4(0.0, 0.0, 0.0, 1.0);
// "glm::mat4" constants
const glm::mat4 Math::MAT4_IDENTITY = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
// "glm::dmat4" constants
const glm::dmat4 Math::DMAT4_IDENTITY = glm::dmat4(1.0, 0.0, 0.0, 0.0,
                           0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0,
                           0.0, 0.0, 0.0, 1.0);
// "glm::quat" constants
const glm::quat Math::QUAT_IDENTITY = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
const glm::dquat Math::DQUAT_IDENTITY = glm::dquat(1.0, 0.0, 0.0, 0.0);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core