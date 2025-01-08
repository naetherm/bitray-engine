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
#include "core/math/ray.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Ray Ray::from_two_points(const glm::vec3& origin, const glm::vec3& direction) {
  glm::vec3 normalized_direction = glm::normalize(direction - origin);
  return Ray(origin, normalized_direction);
}

Ray Ray::from_origin_and_direction(const glm::vec3& origin, const glm::vec3& direction) {
  glm::vec3 normalized_direction = glm::normalize(direction);
  return Ray(origin, normalized_direction);
}

Ray Ray::from_up_direction(const glm::vec3& up, const glm::vec3& direction) {
  glm::vec3 normalized_direction = glm::normalize(direction);
  return Ray(up, normalized_direction);
}


Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
: Line(origin, glm::normalize(direction)) {

}



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}