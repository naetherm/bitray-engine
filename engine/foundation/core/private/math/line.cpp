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
Line Line::from_two_points(const core::Vec3f& from, const core::Vec3f& to) {
  core::Vec3f direction = (to - from).get_normalized();
  return Line(from, direction);
}

Line Line::from_origin_and_direction(const core::Vec3f& origin, const core::Vec3f& direction) {
  return Line(origin, direction);
}

Line Line::from_up_direction() {
  return Line(Math::VEC3_ZERO, CoordinateSystem::get_up());
}

Line::Line(const core::Vec3f& origin, const core::Vec3f& direction)
: mOrigin(origin)
, mDirection(direction) {
}

void Line::set_origin(const core::Vec3f& origin) {
  mOrigin = origin;
}

void Line::set_direction(const core::Vec3f& direction) {
  mDirection = direction;
}

const core::Vec3f& Line::get_origin() const {
  return mOrigin;
}

const core::Vec3f& Line::get_direction() const {
  return mDirection;
}

core::Vec3f Line::get_point(float32 t) const {
  return mOrigin + mDirection * t;
}

core::Vec3f Line::get_nearest_point(const core::Vec3f& point) const {
  core::Vec3f origin_to_point = point - mOrigin;
  float32 t = origin_to_point.dot(mDirection); // Projection scalar
  return get_point(t);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}