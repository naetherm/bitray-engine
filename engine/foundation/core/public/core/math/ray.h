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
#include "core/math/line.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Ray
 *
 * @brief
 * A class representing a 3D ray, which is a directed line that starts at an origin and extends infinitely in one direction.
 *
 * This class inherits from `Line` and provides specialized methods for creating rays.
 */
class Ray : public Line {
public:

  /**
   * @brief
   * Creates a ray from two points.
   *
   * The ray starts at the `origin` point and extends in the direction of the vector from `origin` to `direction`.
   *
   * @param origin
   * The starting point of the ray.
   * @param direction
   * The point that determines the direction of the ray.
   *
   * @return
   * A `Ray` instance representing the specified ray.
   */
  static Ray from_two_points(const glm::vec3& origin, const glm::vec3& direction);

  /**
   * @brief
   * Creates a ray from an origin point and a direction vector.
   *
   * @param origin
   * The starting point of the ray.
   * @param direction
   * The direction vector of the ray.
   *
   * @return
   * A `Ray` instance with the specified origin and direction.
   */
  static Ray from_origin_and_direction(const glm::vec3& origin, const glm::vec3& direction);

  /**
   * @brief
   * Creates a ray pointing in an "up" direction from the origin.
   *
   * @param up
   * The "up" vector that determines the vertical component of the ray.
   * @param direction
   * The direction vector that determines the ray's orientation.
   *
   * @return
   * A `Ray` instance pointing in the specified up and direction vectors.
   */
  static Ray from_up_direction(const glm::vec3& up, const glm::vec3& direction);

public:

  /**
   * @brief
   * Constructs a ray with a specified origin and direction.
   *
   * @param origin
   * The starting point of the ray.
   * @param direction
   * The direction vector of the ray.
   */
  Ray(const glm::vec3& origin, const glm::vec3& direction);
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}