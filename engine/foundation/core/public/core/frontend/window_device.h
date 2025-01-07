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
#include "core/core/refcounted.h"
#include "core/frontend/types.h"
#include "core/container/vector.h"
#include "core/math/rect.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FrontendApplication;
class Window;
class WindowDeviceImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * WindowSystem
 *
 * @brief
 * Window management system for operating system windows.
 */
class WindowDevice : public RefCounted {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] frontendApplication
   * Pointer to frontend application
   */
  WindowDevice(FrontendApplication* frontendApplication);

  /**
   * @brief
   * Destructor.
   */
  virtual ~WindowDevice();


  /**
   * @brief
   * Returns pointer to internally window system implementation.
   *
   * @return
   * Pointer to internally window system implementation.
   */
  [[nodiscard]] inline const WindowDeviceImpl* get_impl() const;

  /**
   * @brief
   * Returns pointer to internally window system implementation.
   *
   * @return
   * Pointer to internally window system implementation.
   */
  [[nodiscard]] inline WindowDeviceImpl* get_impl();


  /**
   * @brief
   * Returns the number of currently created and active windows.
   *
   * @return
   * Number of windows.
   */
  [[nodiscard]] inline uint32 get_num_windows() const;

  /**
   * @brief
   * Returns list of all windows.
   *
   * @return
   * All windows.
   */
  [[nodiscard]] inline const Vector<Window*>& get_windows() const;

  /**
   * @brief
   * Returns list of all windows.
   *
   * @return
   * All windows.
   */
  [[nodiscard]] inline Vector<Window*>& get_windows();

  /**
   * @brief
   * Returns pointer to window at index position @p index.
   *
   * @param[in] index
   * Index position.
   *
   * @return
   * Window at position @p index.
   */
  [[nodiscard]] inline Window* get_window(uint32 index) const;

  /**
   * @brief
   * Returns pointer to the currently active window.
   *
   * @return
   * Pointer to the currently active window.
   */
  [[nodiscard]] inline Window* get_active_window() const;

  /**
   * @brief
   * Sets the currently active window.
   *
   * @param[in] activeWindow
   * Index position of the currently active window.
   */
  inline void set_active_window(core::uint32 activeWindow);

  /**
   * @brief
   * Creates and returns a nwe window.
   * @param title
   * @param shape
   * @param windowStyle
   * @return
   */
  [[nodiscard]] inline Window* create_window(const WindowCreateDesc& windowCreateDesc);

  /**
   * @brief
   * Destroys the @p window.
   * @param[in] window
   * Pointer to window to destroy.
   */
  inline void destroy_window(Window* window);

  /**
   * @brief
   * Updates all windows.
   */
  inline void update();

  void clear();

private:

  FrontendApplication* mFrontendApplication;

  WindowDeviceImpl* mImpl;

  Vector<Window*> mWindows;

  core::uint32 mActiveWindow;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/frontend/window_device.inl"