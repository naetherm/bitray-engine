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
#include "core/math/plane.h"
#include "core/math/coordinate_system.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/constants.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Plane::Plane()
: mNormal(glm::vec3(0.0f, 1.0f, 0.0f)), mDistance(0.0f) {}

Plane::Plane(float32 a, float32 b, float32 c, float32 d)
: mNormal(glm::normalize(glm::vec3(a, b, c))), mDistance(d / glm::length(glm::vec3(a, b, c))) {}

Plane::Plane(const glm::vec3& normal, float32 distance)
    : mNormal(glm::normalize(normal)), mDistance(distance) {}

Plane::Plane(const glm::vec3& point, const glm::vec3& normal)
    : mNormal(glm::normalize(normal)), mDistance(glm::dot(normal, point)) {}

Plane::Plane(const glm::vec3& point, const glm::vec3& point1, const glm::vec3& point2) {
  mNormal = glm::normalize(glm::cross(point1 - point, point2 - point));
  mDistance = glm::dot(mNormal, point);
}

bool Plane::operator==(const Plane& rhs) const {
  return glm::all(glm::epsilonEqual(mNormal, rhs.mNormal, glm::epsilon<float32>())) &&
         glm::epsilonEqual(mDistance, rhs.mDistance, glm::epsilon<float32>());
}

bool Plane::operator!=(const Plane& rhs) const {
  return !(*this == rhs);
}

const glm::vec3& Plane::get_normal() const {
  return mNormal;
}

float32 Plane::get_distance() const {
  return mDistance;
}

float32 Plane::normalize() {
  float32 length = glm::length(mNormal);
  if (length > glm::epsilon<float32>()) {
    mNormal /= length;
    mDistance /= length;
  }
  return length;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}