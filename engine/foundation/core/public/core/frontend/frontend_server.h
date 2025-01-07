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
#include "core/core/server_impl.h"
#include "core/core/ptr.h"


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
class FrontendApplication;
class CursorDevice;
class DisplayDevice;
class WindowDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * FrontendServer
 *
 * @brief
 * Frontend server implementation.
 */
class FrontendServer : public ServerImpl {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] frontendApplication
   * Pointer to the frontend application implementation.
   */
  explicit FrontendServer(FrontendApplication* frontendApplication);

  /**
   * @brief
   * Destructor.
   */
  ~FrontendServer() override;


  /**
   * @brief
   * Pointer to display device.
   *
   * @return
   * Pointer to display device.
   */
  [[nodiscard]] inline const Ptr<DisplayDevice>& get_display_device() const;

  /**
   * @brief
   * Pointer to display device.
   *
   * @return
   * Pointer to display device.
   */
  [[nodiscard]] inline Ptr<DisplayDevice>& get_display_device();

  /**
   * @brief
   * Pointer to window device.
   *
   * @return
   * Pointer to window device.
   */
  [[nodiscard]] inline const Ptr<WindowDevice>& get_window_device() const;

  /**
   * @brief
   * Pointer to window device.
   *
   * @return
   * Pointer to window device.
   */
  [[nodiscard]] inline Ptr<WindowDevice>& get_window_device();

  /**
   * @brief
   * Pointer to cursor device.
   *
   * @return
   * Pointer to cursor device.
   */
  [[nodiscard]] inline const Ptr<CursorDevice>& get_cursor_device() const;

  /**
   * @brief
   * Pointer to cursor device.
   *
   * @return
   * Pointer to cursor device.
   */
  [[nodiscard]] inline Ptr<CursorDevice>& get_cursor_device();

private:
  /** Pointer to cursor device */
  Ptr<CursorDevice> mCursorDevice;
  /** Pointer to the display device */
  Ptr<DisplayDevice> mDisplayDevice;
  /** Pointer to the window device */
  Ptr<WindowDevice> mWindowDevice;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/frontend/frontend_server.inl"