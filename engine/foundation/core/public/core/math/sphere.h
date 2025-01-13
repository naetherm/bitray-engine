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
#include "core/math/vec3.h"


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
 * Sphere
 *
 * @brief
 * Represents a sphere in 3D space.
 *
 * This template class models a sphere defined by a center position and a radius.
 * It supports basic operations for managing and comparing spheres.
 *
 * @tparam TType
 * The numerical type used for the sphere's radius and position (e.g., `float`, `double`).
 */
template<typename TType>
class Sphere {
public:

  /**
   * @brief
   * Default constructor.
   *
   * Creates a sphere with a default radius of zero and a center position at the origin.
   */
  Sphere();

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] other
   * The sphere to copy.
   */
  Sphere(const Sphere& other);

  /**
   * @brief
   * Destructor.
   */
  ~Sphere();

  /**
   * @brief
   * Assignment operator.
   *
   * @param[in] other
   * The sphere to assign from.
   *
   * @return
   * A reference to the current sphere after assignment.
   */
  Sphere& operator=(const Sphere& other);

  /**
   * @brief
   * Equality operator.
   *
   * Compares two spheres for equality, checking both radius and center position.
   *
   * @param[in] other
   * The sphere to compare with.
   *
   * @return
   * `true` if the spheres are equal; otherwise, `false`.
   */
  bool operator==(const Sphere& other) const;

  /**
   * @brief
   * Inequality operator.
   *
   * Compares two spheres for inequality, checking both radius and center position.
   *
   * @param[in] other
   * The sphere to compare with.
   *
   * @return
   * `true` if the spheres are not equal; otherwise, `false`.
   */
  bool operator!=(const Sphere& other) const;

  /**
   * @brief
   * Retrieves the radius of the sphere.
   *
   * @return
   * The radius of the sphere.
   */
  TType get_radius() const;

  /**
   * @brief
   * Sets the radius of the sphere.
   *
   * @param[in] radius
   * The new radius of the sphere.
   */
  void set_radius(TType radius);

  /**
   * @brief
   * Retrieves the center position of the sphere.
   *
   * @return
   * The center position as a 3D vector.
   */
  const Vec3<TType>& get_position() const;

  /**
   * @brief
   * Sets the center position of the sphere.
   *
   * @param[in] position
   * A 3D vector representing the new center position.
   */
  void set_position(const Vec3<TType>& position);

  /**
   * @brief
   * Sets the center position of the sphere using individual coordinates.
   *
   * @param[in] x
   * The X-coordinate of the new center position.
   * @param[in] y
   * The Y-coordinate of the new center position.
   * @param[in] z
   * The Z-coordinate of the new center position.
   */
  void set_position(TType x, TType y, TType z);

private:
  /** The radius of the sphere. */
  TType mRadius;

  /** The center position of the sphere in 3D space. */
  Vec3<TType> mCenterPosition;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/sphere.inl"