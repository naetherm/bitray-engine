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
#include "core/container/vector.h"
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
class WindowCallbacks;
class WindowImpl;
class WindowDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Window
 *
 * @brief
 * Platform independent window representation.
 */
class Window {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] windowSystem
   * Pointer to window system.
   */
  Window(WindowDevice* windowSystem);

  /**
   * @brief
   * Destructor.
   */
  virtual ~Window();


  inline void add_window_callback(WindowCallbacks* windowCallback);


  /**
   * @brief
   * Pointer to window system.
   *
   * @return
   * Pointer to window system.
   */
  [[nodiscard]] inline const WindowDevice* get_window_system() const;

  /**
   * @brief
   * Pointer to window system.
   *
   * @return
   * Pointer to window system.
   */
  [[nodiscard]] inline WindowDevice* get_window_system();

  /**
   * @brief
   * Return pointer to platform specific implementation.
   *
   * @return
   * Pointer to platform specific implementation.
   */
  [[nodiscard]] inline const WindowImpl* get_impl() const;

  /**
   * @brief
   * Return pointer to platform specific implementation.
   *
   * @return
   * Pointer to platform specific implementation.
   */
  [[nodiscard]] inline WindowImpl* get_impl();


  /**
   * @brief
   * Return native window handle.
   *
   * @return
   * Native window handle
   */
  [[nodiscard]] inline handle get_native_window_handle() const;

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
  [[nodiscard]] inline bool close_requested() const;


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
  void create(const String& title, Recti shape, EWindowStyle windowStyle);

  /**
   * @brief
   * Destroys this window.
   */
  void destroy();

  /**
   * @brief
   * Sets the shape of the window.
   *
   * @param[in] shape
   * The new shape of the window.
   */
  inline void set_shape(const Recti& shape);

  /**
   * @brief
   * Sets the size of the window.
   *
   * @param[in] size
   * The size of the window.
   */
  inline void set_size(const Vec2i& size);

  /**
   * @brief
   * Sets the position of the window.
   *
   * @param[in] position
   * The position of the window.
   */
  inline void set_position(const Vec2i& position);

  /**
   * @brief
   * Returns platform data of the window.
   *
   * @return
   * Platform data of the window.
   */
  [[nodiscard]] inline FrontendPlatformData get_platform_data() const;


  /**
   * @brief
   * Updates the window.
   */
  void update();

public:

  /**
   * @brief
   * Called on tick.
   *
   * @param[in] deltaTime
   * The delta time till last call.
   */
  virtual void on_tick(float deltaTime);

  /**
   * @brief
   * Called on draw.
   */
  virtual void on_draw();

  /**
   * @brief
   * Called on resizing.
   *
   * @param[in] x
   * New x size
   * @param[in] y
   * New y size
   */
  virtual void on_size(int x, int y);

  /**
   * @brief
   * Called on moving.
   *
   * @param[in] x
   * New x position
   * @param[in] y
   * New y position
   */
  virtual void on_move(int x, int y);


  /**
   * @brief
   * Called on leaving.
   */
  virtual void on_leave();

  /**
   * @brief
   * Called on entering.
   */
  virtual void on_enter();

  /**
   * @brief
   * Called on mouse position change.
   *
   * @param[in] x
   * Mouse x position
   * @param[in] y
   * Mouse y position
   */
  virtual void on_mouse_position(int x, int y);

  /**
   * @brief
   * Called on mouse move.
   *
   * @param[in] x
   * Mouse x position
   * @param[in] y
   * Mouse y position
   */
  virtual void on_mouse_move(int x, int y);

  /**
   * @brief
   * Called on mouse wheel change.
   *
   * @param[in] x
   * Mouse wheel difference.
   */
  virtual void on_mouse_wheel(float x);

  /**
   * @brief
   * Called on mouse button down events.
   *
   * @param[in] inputMouseItem
   * The item that was triggered.
   */
  virtual void on_mouse_button_down(InputMouseItem inputMouseItem);

  /**
   * @brief
   * Called on mouse button up events.
   *
   * @param[in] inputMouseItem
   * The item that was triggered.
   */
  virtual void on_mouse_button_up(InputMouseItem inputMouseItem);

  /**
   * @brief
   * Called on key press events.
   *
   * @param[in] key
   * The key that was triggered.
   */
  virtual void on_key_press(InputKeyboardItem key);

  /**
   * @brief
   * Called on key release events.
   *
   * @param[in] key
   * The key that was triggered.
   */
  virtual void on_key_release(InputKeyboardItem key);

private:
  /** Pointer to window system */
  WindowDevice* mWindowSystem;
  /** Pointer to internal window implementation */
  WindowImpl* mImpl;
  /** List of all window callbacks that are registered on this window */
  Vector<WindowCallbacks*> mlstWindowCallbacks;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/frontend/window.inl"