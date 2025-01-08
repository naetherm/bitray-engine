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
#include "core/math/line.h"
#include "core/math/coordinate_system.h"
#include "core/math/math.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Line Line::from_two_points(const glm::vec3& from, const glm::vec3& to) {
  glm::vec3 direction = glm::normalize(to - from);
  return Line(from, direction);
}

Line Line::from_origin_and_direction(const glm::vec3& origin, const glm::vec3& direction) {
  return Line(origin, direction);
}

Line Line::from_up_direction() {
  return Line(Math::VEC3_ZERO, CoordinateSystem::get_up());
}

Line::Line(const glm::vec3& origin, const glm::vec3& direction)
: mOrigin(origin)
, mDirection(direction) {
}

void Line::set_origin(const glm::vec3& origin) {
  mOrigin = origin;
}

void Line::set_direction(const glm::vec3& direction) {
  mDirection = direction;
}

const glm::vec3& Line::get_origin() const {
  return mOrigin;
}

const glm::vec3& Line::get_direction() const {
  return mDirection;
}

glm::vec3 Line::get_point(float32 t) const {
  return mOrigin + t * mDirection;
}

glm::vec3 Line::get_nearest_point(const glm::vec3& point) const {
  glm::vec3 origin_to_point = point - mOrigin;
  float32 t = glm::dot(origin_to_point, mDirection); // Projection scalar
  return get_point(t);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}