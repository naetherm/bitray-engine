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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class WindowDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * WindowSystemImpl
 *
 * @brief
 * Abstract window system implementation.
 */
class WindowDeviceImpl {
public:
  /**
   * @brief
   * Constructor.
   */
  WindowDeviceImpl() = default;

  /**
   * @brief
   * Destructor.
   */
  virtual ~WindowDeviceImpl() = default;

public:
  /**
   * @brief
   * Returns whether we are currently in dragging mode.
   *
   * @return
   * True if currently in dragging mode.
   */
  [[nodiscard]] virtual bool is_dragging() const = 0;

  /**
   * @brief
   * Returns whether the dragging started right now.
   *
   * @return
   * True if dragging started right now.
   */
  [[nodiscard]] virtual bool dragging_begins() const = 0;

  /**
   * @brief
   * Sets the current dnd position.
   *
   * @param[in] x
   * The current x position
   * @param[in] y
   * The current y position
   */
  virtual void set_dnd_position(int x, int y) = 0;

  /**
   * @brief
   * Returns the current dnd x position.
   *
   * @return
   * Current dnd x position.
   */
  [[nodiscard]] virtual core::int32 get_dnd_x() const = 0;

  /**
   * @brief
   * Returns the current dnd y position.
   *
   * @return
   * Current dnd y position.
   */
  [[nodiscard]] virtual core::int32 get_dnd_y() const = 0;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core