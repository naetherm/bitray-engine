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
#include "core/frontend/types.h"
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
 * WindowImpl
 *
 * @brief
 * Platform independent window implementation, have to be implemented for each support platform.
 */
class WindowImpl {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] window
   * Parental, platform independent, window implementation.
   */
  WindowImpl(Window* window);

  /**
   * @brief
   * Destructor.
   */
  virtual ~WindowImpl();


  /**
   * @brief
   * Return reference to window status.
   *
   * @return
   * Window status.
   */
  [[nodiscard]] inline const SWindowStatus& get_window_status() const;

  /**
   * @brief
   * Return reference to window status.
   *
   * @return
   * Window status.
   */
  [[nodiscard]] inline SWindowStatus& get_window_status();

  /**
   * @brief
   * Return whether a closing of this window was requested.
   *
   * @return
   * Was closing of this window requested.
   */
  [[nodiscard]] virtual bool close_requested() const = 0;


  /**
   * @brief
   * Creates this window with the proposed parameters.
   *
   * @param[in] title
   * The title of the window.
   * @param[in] shape
   * The shape of the window.
   * @param[in] windowStyle
   * The window style.
   */
  virtual void create(const String& title, Recti shape, EWindowStyle windowStyle) = 0;

  /**
   * @brief
   * Destroys this window.
   */
  virtual void destroy() = 0;

  /**
   * @brief
   * Return native window handle.
   *
   * @return
   * Native window handle
   */
  [[nodiscard]] virtual handle get_native_window_handle() const = 0;

  /**
   * @brief
   * Sets the shape of the window.
   *
   * @param[in] shape
   * The new shape of the window.
   */
  virtual void set_shape(const Recti& shape) = 0;

  /**
   * @brief
   * Sets the size of the window.
   *
   * @param[in] size
   * The size of the window.
   */
  virtual void set_size(const Vec2i& size) = 0;

  /**
   * @brief
   * Sets the position of the window.
   *
   * @param[in] position
   * The position of the window.
   */
  virtual void set_position(const Vec2i& position) = 0;

  /**
   * @brief
   * Returns platform data of the window.
   *
   * @return
   * Platform data of the window.
   */
  [[nodiscard]] virtual FrontendPlatformData get_platform_data() const = 0;

  /**
   * @brief
   * Updates the window.
   */
  virtual void update() = 0;

protected:
  /** Pointer to platform independent parent */
  Window* mWindow;
  /** The window status */
  SWindowStatus mWindowStatus;
  /** Window bordering information */
  SWindowBorders mWindowBorders;
  /** Window style information */
  EWindowStyle mWindowStyle;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/frontend/window_impl.inl"