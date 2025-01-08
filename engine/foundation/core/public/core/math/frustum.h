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
#include "core/math/plane.h"


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
 * Frustum
 *
 * @brief
 * Represents a view frustum in 3D space, defined by six planes (left, right, top, bottom, near, far).
 * The frustum is typically used for culling and visibility testing in 3D graphics.
 */
class Frustum {
public:

  /**
   * @enum
   * PlaneIndex
   *
   * @brief
   * Enum defining the indices of the six planes that make up the frustum.
   */
  enum PlaneIndex {
    PLANE_LEFT = 0,  ///< The left plane of the frustum.
    PLANE_RIGHT,     ///< The right plane of the frustum.
    PLANE_TOP,       ///< The top plane of the frustum.
    PLANE_BOTTOM,    ///< The bottom plane of the frustum.
    PLANE_NEAR,      ///< The near plane of the frustum.
    PLANE_FAR,       ///< The far plane of the frustum.
    NUMBER_OF_PLANES ///< Total number of planes in the frustum.
  };

public:

  /**
   * @brief
   * Default constructor.
   *
   * Creates an uninitialized frustum.
   */
  Frustum() = default;

  /**
   * @brief
   * Constructs a frustum from a projection-view matrix.
   *
   * The provided matrix is used to extract the six planes of the frustum.
   *
   * @param matrix
   * The combined projection-view matrix used to define the frustum.
   */
  explicit Frustum(const glm::mat4& matrix);

  /**
   * @brief
   * Default destructor.
   */
  ~Frustum() = default;

protected:
  /** The planes that define the frustum. */
  Plane mPlanes[NUMBER_OF_PLANES];
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}