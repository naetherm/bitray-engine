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
#include "core/frontend/window_device_impl.h"
#include "core/linux/xcb_backend.h"


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
 * LinuxWindowSystem
 *
 * @brief
 * Linux specific implementation for the window system.
 */
class LinuxWindowDevice : public WindowDeviceImpl {

  friend class LinuxWindow;

public:

  explicit LinuxWindowDevice(WindowDevice* windowSystem);

  /**
   * @brief
   * Destructor.
   */
  ~LinuxWindowDevice() override;


  /**
   * @brief
   * Returns the xcb backend implementation.
   *
   * @return
   * Xcb backend implementation.
   */
  [[nodiscard]] XcbBackend& get_xcb_backend() const;

  /**
   * @brief
   * Returns the main xcb connection.
   *
   * @return
   * Main xcb connection.
   */
  [[nodiscard]] ::xcb_connection_t* get_xcb_connection() const;

  /**
   * @brief
   * Returns the main xcb screen.
   *
   * @return
   * Main xcb screen.
   */
  [[nodiscard]] ::xcb_screen_t* get_xcb_main_screen() const;


  void set_support_uri_list(bool support);

  [[nodiscard]] bool support_uri_list() const;

public:
  /**
   * @brief
   * Sets whether we are currently dragging.
   */
  void set_dragging(bool b);

  /**
   * @brief
   * Sets that dragging became right now.
   */
  void set_dragging_begins(bool b);
  /**
   * @brief
   * Returns whether we are currently in dragging mode.
   *
   * @return
   * True if currently in dragging mode.
   */
  [[nodiscard]] bool is_dragging() const override;

  /**
   * @brief
   * Returns whether the dragging started right now.
   *
   * @return
   * True if dragging started right now.
   */
  [[nodiscard]] bool dragging_begins() const override;

  /**
   * @brief
   * Sets the current dnd position.
   *
   * @param[in] x
   * The current x position
   * @param[in] y
   * The current y position
   */
  void set_dnd_position(int x, int y) override;

  /**
   * @brief
   * Returns the current dnd x position.
   *
   * @return
   * Current dnd x position.
   */
  [[nodiscard]] core::int32 get_dnd_x() const override;

  /**
   * @brief
   * Returns the current dnd y position.
   *
   * @return
   * Current dnd y position.
   */
  [[nodiscard]] core::int32 get_dnd_y() const override;

private:

  /**
  * @brief
  * Initializes the window system.
  */
  void initialize_window_system();

  /**
   * @brief
   * Deinitializes the window system.
   */
  void deinitialize_window_system();

private:

  WindowDevice* mWindowSystem;

  XcbBackend& mXcbBackend;

  core::int32 mDnDX;
  core::int32 mDnDY;

  bool mIsDragging;
  bool mDraggingBegin;
  bool mSupportUriList;

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
