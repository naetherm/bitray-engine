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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * CoordinateSystem
 *
 * @brief
 * A utility class that provides information about a 3D coordinate system, including axis directions
 * and whether the system is right-handed or left-handed.
 */
class CoordinateSystem {
public:

  /**
   * @brief
   * Gets the "up" direction vector of the coordinate system.
   *
   * @return
   * A reference to the `glm::vec3` representing the "up" direction.
   */
  static const glm::vec3& get_up();

  /**
   * @brief
   * Gets the "right" direction vector of the coordinate system.
   *
   * @return
   * A reference to the `glm::vec3` representing the "right" direction.
   */
  static const glm::vec3& get_right();

  /**
   * @brief
   * Gets the "forward" direction vector of the coordinate system.
   *
   * @return
   * A reference to the `glm::vec3` representing the "forward" direction.
   */
  static const glm::vec3& get_forward();

  /**
   * @brief
   * Determines whether the coordinate system is right-handed.
   *
   * In a right-handed coordinate system, the cross product of the "right" and "forward" vectors
   * yields the "up" vector.
   *
   * @return
   * `true` if the coordinate system is right-handed; `false` otherwise.
   */
  static bool is_right_handed();

  /**
   * @brief
   * Determines whether the coordinate system is left-handed.
   *
   * In a left-handed coordinate system, the cross product of the "right" and "forward" vectors
   * yields the opposite of the "up" vector.
   *
   * @return
   * `true` if the coordinate system is left-handed; `false` otherwise.
   */
  static bool is_left_handed();
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core