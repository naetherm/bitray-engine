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
#include "core/core/non_copyable.h"
#include "core/frontend/types.h"
#include "core/input/types.h"
#include "core/math/rect.h"
#include "core/math/vec2.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Window;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * WindowCallbacks
 *
 * @brief
 * Windows callback implementation.
 */
class WindowCallbacks : public NonCopyable {
public:
  /**
   * @brief
   * Constructor.
   */
  WindowCallbacks() = default;

  /**
   * @brief
   * Destructor.
   */
  ~WindowCallbacks() override = default;

public:

  /**
  * @brief
  * Called on tick.
  *
  * @param[in] deltaTime
  * The delta time till last call.
  */
  virtual inline void on_tick(float deltaTime);

  /**
   * @brief
   * Called on draw.
   */
  virtual inline void on_draw();

  /**
   * @brief
   * Called on resizing.
   *
   * @param[in] x
   * New x size
   * @param[in] y
   * New y size
   */
  virtual inline void on_size(int x, int y);

  /**
   * @brief
   * Called on moving.
   *
   * @param[in] x
   * New x position
   * @param[in] y
   * New y position
   */
  virtual inline void on_move(int x, int y);


  /**
   * @brief
   * Called on leaving.
   */
  virtual inline void on_leave();

  /**
   * @brief
   * Called on entering.
   */
  virtual inline void on_enter();

  /**
   * @brief
   * Called on mouse position change.
   *
   * @param[in] x
   * Mouse x position
   * @param[in] y
   * Mouse y position
   */
  virtual inline void on_mouse_position(int x, int y);

  /**
   * @brief
   * Called on mouse move.
   *
   * @param[in] x
   * Mouse x position
   * @param[in] y
   * Mouse y position
   */
  virtual inline void on_mouse_move(int x, int y);

  /**
   * @brief
   * Called on mouse wheel change.
   *
   * @param[in] x
   * Mouse wheel difference.
   */
  virtual inline void on_mouse_wheel(float x);

  /**
   * @brief
   * Called on mouse button down events.
   *
   * @param[in] inputMouseItem
   * The item that was triggered.
   */
  virtual inline void on_mouse_button_down(InputMouseItem inputMouseItem);

  /**
   * @brief
   * Called on mouse button up events.
   *
   * @param[in] inputMouseItem
   * The item that was triggered.
   */
  virtual inline void on_mouse_button_up(InputMouseItem inputMouseItem);

  /**
   * @brief
   * Called on key press events.
   *
   * @param[in] key
   * The key that was triggered.
   */
  virtual inline void on_key_press(InputKeyboardItem key);

  /**
   * @brief
   * Called on key release events.
   *
   * @param[in] key
   * The key that was triggered.
   */
  virtual inline void on_key_release(InputKeyboardItem key);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/frontend/window_callbacks.inl"