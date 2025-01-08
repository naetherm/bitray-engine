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
 * Plane
 *
 * @brief
 * Represents a mathematical plane in 3D space, defined by a normal vector and a distance from the origin.
 */
class Plane {
public:

  /**
   * @brief
   * Default constructor.
   *
   * Initializes the plane with a default normal vector `(0, 0, 0)` and a distance of `0`.
   */
  Plane();

  /**
   * @brief
   * Constructs a plane using its general equation coefficients.
   *
   * The plane equation is given by: \( ax + by + cz + d = 0 \).
   *
   * @param a
   * The x-component of the plane's normal vector.
   * @param b
   * The y-component of the plane's normal vector.
   * @param c
   * The z-component of the plane's normal vector.
   * @param d
   * The distance from the origin along the plane's normal.
   */
  Plane(float32 a, float32 b, float32 c, float32 d);

  /**
   * @brief
   * Constructs a plane using a normal vector and distance from the origin.
   *
   * @param normal
   * The normal vector defining the plane's orientation.
   * @param distance
   * The distance of the plane from the origin along its normal.
   */
  Plane(const glm::vec3& normal, float32 distance);

  /**
   * @brief
   * Constructs a plane from a point on the plane and a normal vector.
   *
   * @param point
   * A point on the plane.
   * @param normal
   * The normal vector defining the plane's orientation.
   */
  Plane(const glm::vec3& point, const glm::vec3& normal);

  /**
   * @brief
   * Constructs a plane passing through three points in 3D space.
   *
   * @param point
   * The first point on the plane.
   * @param point1
   * The second point on the plane.
   * @param point2
   * The third point on the plane.
   */
  Plane(const glm::vec3& point, const glm::vec3& point1, const glm::vec3& point2);

  /**
   * @brief
   * Equality operator.
   *
   * Compares two planes for equality based on their normal vectors and distances.
   *
   * @param rhs
   * The plane to compare with.
   *
   * @return
   * `true` if the planes are equal, `false` otherwise.
   */
  bool operator==(const Plane& rhs) const;

  /**
   * @brief
   * Inequality operator.
   *
   * Compares two planes for inequality.
   *
   * @param rhs
   * The plane to compare with.
   *
   * @return
   * `true` if the planes are not equal, `false` otherwise.
   */
  bool operator!=(const Plane& rhs) const;

  /**
   * @brief
   * Gets the normal vector of the plane.
   *
   * @return
   * The normal vector of the plane.
   */
  const glm::vec3& get_normal() const;

  /**
   * @brief
   * Gets the distance of the plane from the origin.
   *
   * @return
   * The signed distance from the origin along the plane's normal.
   */
  float32 get_distance() const;

  /**
   * @brief
   * Normalizes the plane.
   *
   * Adjusts the normal vector to have a length of 1 and adjusts the distance accordingly.
   *
   * @return
   * The length of the original normal vector before normalization.
   */
  float32 normalize();

public:
  /** The normal vector of the plane, defining its orientation. */
  glm::vec3 mNormal;

  /** The signed distance of the plane from the origin along its normal. */
  float32 mDistance;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}