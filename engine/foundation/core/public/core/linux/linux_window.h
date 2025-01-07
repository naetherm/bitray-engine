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
#include "core/frontend/window_impl.h"
#include "core/linux/xcb_includes.h"


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
 * LinuxWindow
 *
 * @brief
 * Linux specific window implementation.
 */
class LinuxWindow : public WindowImpl {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] window
   * Parental, platform independent, window implementation.
   */
  LinuxWindow(Window* window);

  /**
   * @brief
   * Destructor.
   */
  ~LinuxWindow() override;


public:

  /**
  * @brief
  * Return whether a closing of this window was requested.
  *
  * @return
  * Was closing of this window requested.
  */
  [[nodiscard]] bool close_requested() const override;


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
  void create(const String& title, Recti shape, EWindowStyle windowStyle) override;

  /**
   * @brief
   * Destroys this window.
   */
  void destroy() override;

  /**
   * @brief
   * Return native window handle.
   *
   * @return
   * Native window handle
   */
  [[nodiscard]] handle get_native_window_handle() const override;

  /**
   * @brief
   * Sets the shape of the window.
   *
   * @param[in] shape
   * The new shape of the window.
   */
  void set_shape(const Recti& shape) override;

  /**
   * @brief
   * Sets the size of the window.
   *
   * @param[in] size
   * The size of the window.
   */
  void set_size(const Vec2i& size) override;

  /**
   * @brief
   * Sets the position of the window.
   *
   * @param[in] position
   * The position of the window.
   */
  void set_position(const Vec2i& position) override;

  /**
   * @brief
   * Returns platform data of the window.
   *
   * @return
   * Platform data of the window.
   */
  [[nodiscard]] FrontendPlatformData get_platform_data() const override;

  /**
   * @brief
   * Updates the window.
   */
  void update() override;

private:
  /**
   * @brief
   * Internal used initialization method.
   */
  void initialize();

private:

  ::xcb_connection_t *mXcbConnection;
  ::xcb_screen_t *mXcbScreen;
  ::xcb_ewmh_connection_t mXcbEwmh;
  ::xcb_window_t mXcbNativeWindow;
  ::xcb_generic_event_t* mXcbEvent;

  String mTitle;
  Recti mShape;

  bool mWaitEvent;

  bool mRequestClosing;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core