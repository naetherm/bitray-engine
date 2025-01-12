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
#include "core/math/quaternion.h"
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
 * Transform
 *
 * @brief
 * Represents a 3D transformation consisting of position, rotation, and scale.
 * This class is commonly used for spatial transformations in 3D graphics.
 */
class Transform {
public:

  /**
   * @brief
   * A constant representing the identity transform.
   *
   * The identity transform has no translation, no rotation, and uniform scale of 1.
   */
  static const Transform IDENTITY;

public:

  /**
   * @brief
   * Default constructor.
   *
   * Initializes the transform to the identity transform.
   */
  Transform();

  /**
   * @brief
   * Constructs a transform from a 4x4 transformation matrix.
   *
   * @param matrix
   * The 4x4 matrix used to initialize the transform.
   */
  //explicit Transform(const glm::mat4& matrix);

  /**
   * @brief
   * Constructs a transform with a specified position.
   *
   * The rotation is initialized to identity, and the scale is set to 1.
   *
   * @param position
   * The position of the transform in 3D space.
   */
  explicit Transform(const core::Vec3f& position);

  /**
   * @brief
   * Constructs a transform with a specified position and rotation.
   *
   * The scale is initialized to 1.
   *
   * @param position
   * The position of the transform in 3D space.
   * @param rotation
   * The rotation of the transform, represented as a quaternion.
   */
  explicit Transform(const core::Vec3f& position, const core::Quaternionf& rotation);

  /**
   * @brief
   * Constructs a transform with a specified position, rotation, and scale.
   *
   * @param position
   * The position of the transform in 3D space.
   * @param rotation
   * The rotation of the transform, represented as a quaternion.
   * @param scale
   * The scaling factor of the transform.
   */
  explicit Transform(const core::Vec3f& position, const core::Quaternionf& rotation, const core::Vec3f& scale);

  /**
   * @brief
   * Adds another transform to this transform.
   *
   * This operation combines the translation, rotation, and scale of the two transforms.
   *
   * @param other
   * The transform to add to this transform.
   * @return
   * A reference to this transform after the operation.
   */
  Transform& operator+=(const Transform& other);

  /**
   * @brief
   * Retrieves the transform as a 4x4 matrix.
   *
   * @param[out] objectSpaceToWorldSpace
   * The 4x4 matrix representing this transform.
   */
  //void get_as_matrix(glm::mat4& objectSpaceToWorldSpace) const;

  /**
   * @brief
   * Sets the transform from a 4x4 matrix.
   *
   * @param objectSpaceToWorldSpace
   * The 4x4 matrix used to set the transform.
   */
  //void set_by_matrix(const glm::mat4& objectSpaceToWorldSpace);

protected:
  core::Vec3f  mPosition; ///< The position component of the transform.
  core::Quaternionf  mRotation; ///< The rotation component of the transform, stored as a quaternion.
  core::Vec3f  mScale;    ///< The scaling component of the transform.
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}