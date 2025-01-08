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
#include "core/math/transform.h"
#include "core/math/math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Transform::Transform()
: mPosition(Math::VEC3_ZERO)
, mRotation(Math::QUAT_IDENTITY)
, mScale(Math::VEC3_ONE) {
}

Transform::Transform(const glm::mat4& matrix) {
  set_by_matrix(matrix);
}

Transform::Transform(const glm::vec3& position)
: mPosition(position)
, mRotation(Math::QUAT_IDENTITY)
, mScale(Math::VEC3_ONE)  {
}

Transform::Transform(const glm::vec3& position, const glm::quat& rotation)
: mPosition(position)
, mRotation(rotation)
, mScale(Math::VEC3_ONE) {
}

Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
: mPosition(position)
, mRotation(rotation)
, mScale(scale) {
}

Transform& Transform::operator+=(const Transform& other) {
  // Set update position, rotation and scale
  mPosition += glm::quat(mRotation) * other.mPosition;
  mRotation  = mRotation * other.mRotation;
  mScale    *= other.mScale;

  // Return a reference to this instance
  return *this;
}

void Transform::get_as_matrix(glm::mat4& objectSpaceToWorldSpace) const {
  objectSpaceToWorldSpace = glm::translate(Math::MAT4_IDENTITY, mPosition) * glm::mat4_cast(mRotation) * glm::scale(Math::MAT4_IDENTITY, mScale);
}

void Transform::set_by_matrix(const glm::mat4& objectSpaceToWorldSpace) {
  // TODO(naetherm) Optimize
  glm::vec3 scale64Bit;
  glm::quat rotation64Bit;
  glm::vec3 skew;
  glm::vec4 perspective;
  glm::decompose(objectSpaceToWorldSpace, scale64Bit, rotation64Bit, mPosition, skew, perspective);
  mScale = scale64Bit;
  mRotation = rotation64Bit;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}