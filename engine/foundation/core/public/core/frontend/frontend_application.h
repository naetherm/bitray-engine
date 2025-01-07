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
#include "core/app/core_application.h"
#include "core/frontend/abstract_frontend.h"
#include "core/core/ptr.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class CursorDevice;
class DisplayDevice;
class WindowDevice;
class FrontendServer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * FrontendApplication
 *
 * @brief
 * Frontend application implementation.
 */
class FrontendApplication : public CoreApplication, public AbstractFrontend {
public:
  /**
   * @brief
   * Constructor.
   */
  FrontendApplication();

  /**
   * @brief
   * Destructor.
   */
  ~FrontendApplication() override;


  /**
   * @brief
   * Pointer to frontend.
   *
   * @return
   * Pointer to frontend.
   */
  [[nodiscard]] inline const Ptr<FrontendServer>& get_frontend() const;

  /**
   * @brief
   * Pointer to frontend.
   *
   * @return
   * Pointer to frontend.
   */
  [[nodiscard]] inline Ptr<FrontendServer>& get_frontend();

  /**
   * @brief
   * Pointer to display system.
   *
   * @return
   * Pointer to display system.
   */
  [[nodiscard]] inline const Ptr<DisplayDevice>& get_display_device() const;

  /**
   * @brief
   * Pointer to display system.
   *
   * @return
   * Pointer to display system.
   */
  [[nodiscard]] inline Ptr<DisplayDevice>& get_display_device();

  /**
   * @brief
   * Pointer to window system.
   *
   * @return
   * Pointer to window system.
   */
  [[nodiscard]] inline const Ptr<WindowDevice>& get_window_device() const;

  /**
   * @brief
   * Pointer to window system.
   *
   * @return
   * Pointer to window system.
   */
  [[nodiscard]] inline Ptr<WindowDevice>& get_window_device();

  /**
   * @brief
   * Pointer to cursor system.
   *
   * @return
   * Pointer to cursor system.
   */
  [[nodiscard]] inline const Ptr<CursorDevice>& get_cursor_device() const;

  /**
   * @brief
   * Pointer to cursor system.
   *
   * @return
   * Pointer to cursor system.
   */
  [[nodiscard]] inline Ptr<CursorDevice>& get_cursor_device();
  

  //[-------------------------------------------------------]
  //[ Protected virtual AbstractLifecycle functions         ]
  //[-------------------------------------------------------]
protected:
  [[nodiscard]] CORE_API bool on_start() override;

  /**
   * @brief
   * De-initialization function that is called after on_de_init()
   *
   * @remarks
   * The default implementation does the following tasks:
   * - get frontend position and size of the current session and write frontend configuration
   * - Everything that CoreApplication::on_stop() does
   */
  CORE_API void on_stop() override;


  //[-------------------------------------------------------]
  //[ Protected virtual AbstractFrontend functions          ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Called when the window size has been changed
   *
   * @note
   * - The default implementation is empty
  */
  CORE_API void on_size() override;

  /**
   * @brief
   * Called when the fullscreen mode was changed
   *
   * @remarks
   * This method just says "something related to fullscreen mode has been changed". Whether we
   * changed from window mode into fullscreen mode or changed e.g. the resolution used in
   * fullscreen mode is not really interesting in here.
   *
   * @note
   * - The default implementation is empty
  */
  CORE_API void on_fullscreen_mode() override;

  /**
   * @brief
   * Called to let the frontend draw into it's window
   *
   * @note
   * - The default implementation is empty
  */
  CORE_API void on_draw() override;

  /**
   * @brief
   * Called to let the frontend update it's states
   *
   * @remarks
   * You can use this method to do work you have to perform on a regular basis. It's
   * recommended to keep the work done within the implementation as compact as possible.
   * Don't use this function to perform 'polling'-everything, use events or if required
   * for example timers or threads instead.
   *
   * @note
   * - The default implementation is empty
  */
  CORE_API void on_update() override;

  /**
   * @brief
   * Called when string data has been dropped onto the frontend window
   *
   * @param[in] lstFiles
   * List of file names
   *
   * @note
   * - The default implementation is empty
  */
  CORE_API void on_drop(const core::Vector<String> &lstFiles) override;

private:
  /** Pointer to frontend implementation */
  Ptr<FrontendServer> mFrontend;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/frontend/frontend_application.inl"