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
#include <glm/glm.hpp>


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
 * Line
 *
 * @brief
 * A class representing a 3D line defined by an origin and a direction vector.
 *
 * This class provides methods to create lines, manipulate their origin and direction,
 * and perform calculations such as finding a point on the line or the nearest point to a given point.
 */
class Line {
public:

  /**
   * @brief
   * Creates a line from two points.
   *
   * The line is constructed with the first point as the origin and the vector from the first
   * point to the second point as the direction.
   *
   * @param[in] from
   * The starting point of the line.
   * @param[in] to
   * The end point of the line.
   *
   * @return
   * A `Line` instance representing the line between the two points.
   */
  static Line from_two_points(const glm::vec3& from, const glm::vec3& to);

  /**
   * @brief
   * Creates a line from an origin and a direction vector.
   *
   * @param[in] origin
   * The origin point of the line.
   * @param[in] direction
   * The direction vector of the line.
   *
   * @return
   * A `Line` instance representing the specified line.
   */
  static Line from_origin_and_direction(const glm::vec3& origin, const glm::vec3& direction);

  /**
   * @brief
   * Creates a line that points in the "up" direction from the origin.
   *
   * @return
   * A `Line` instance with its origin at `(0, 0, 0)` and direction pointing upward.
   */
  static Line from_up_direction();

public:

  /**
   * @brief
   * Constructs a line with a specified origin and direction.
   *
   * @param[in] origin
   * The starting point of the line.
   * @param[in] direction
   * The direction vector of the line.
   */
  Line(const glm::vec3& origin, const glm::vec3& direction);

  /**
   * @brief
   * Sets the origin of the line.
   *
   * @param[in] origin
   * The new origin point of the line.
   */
  void set_origin(const glm::vec3& origin);

  /**
   * @brief
   * Sets the direction of the line.
   *
   * @param[in] direction
   * The new direction vector of the line.
   */
  void set_direction(const glm::vec3& direction);

  /**
   * @brief
   * Gets the origin of the line.
   *
   * @return
   * A reference to the origin point of the line.
   */
  const glm::vec3& get_origin() const;

  /**
   * @brief
   * Gets the direction of the line.
   *
   * @return
   * A reference to the direction vector of the line.
   */
  const glm::vec3& get_direction() const;

  /**
   * @brief
   * Computes a point on the line at a given parameter `t`.
   *
   * The point is calculated as `origin + t * direction`.
   *
   * @param[in] t
   * The parameter defining the distance along the direction vector.
   *
   * @return
   * A `glm::vec3` representing the computed point.
   */
  glm::vec3 get_point(float32 t) const;

  /**
   * @brief
   * Computes the nearest point on the line to a given point in space.
   *
   * @param[in] point
   * The target point for which the nearest point on the line is calculated.
   *
   * @return
   * A `glm::vec3` representing the nearest point on the line.
   */
  glm::vec3 get_nearest_point(const glm::vec3& point) const;

protected:

  /** The origin point of the line. */
  glm::vec3 mOrigin;
  /** The direction vector of the line. */
  glm::vec3 mDirection;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}