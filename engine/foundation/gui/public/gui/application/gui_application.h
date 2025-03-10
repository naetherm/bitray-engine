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
#include "gui/gui.h"
#include <core/frontend/frontend_application.h>
#include "gui/gui/gui_settings.h"
#include <core/core/ptr.h>
#include <core/math/rect.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiServer;
class GuiContext;
class GuiWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class GuiApplication : public core::FrontendApplication {
public:

  GuiApplication();

  ~GuiApplication() override;


  [[nodiscard]] const core::Ptr<GuiServer>& get_gui_server() const;

  [[nodiscard]] core::Ptr<GuiServer>& get_gui_server();
  
  
  [[nodiscard]] virtual core::Ptr<GuiWindow> create_window(const core::WindowCreateDesc& windowCreateDesc);

  //[-------------------------------------------------------]
  //[ Protected virtual core::AbstractLifecycle functions   ]
  //[-------------------------------------------------------]
protected:
  /**
   *  @brief
   *    Initialization function that is called prior to on_init()
   *
   *  @return
   *    'true' if all went fine, else 'false' which will stop the application
   *
   *  @remarks
   *    The default implementation does the following tasks:
   *    - Everything that CoreApplication::on_start() does
   *    - Call OnCreateMainWindow()
   *    - Return and go on with on_init()
   */
  [[nodiscard]] GUI_API bool on_start() override;

  /**
   *  @brief
   *    De-initialization function that is called after on_de_init()
   *
   *  @remarks
   *    The default implementation does the following tasks:
   *    - Everything that CoreApplication::on_stop() does
   *    - De-initialize system GUI
   */
  GUI_API void on_stop() override;

  /**
   * @brief
   * Called to let the frontend draw into it's window
   *
   * @note
   * - The default implementation is empty
   */
  GUI_API void on_draw() override;

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
  GUI_API void on_update() override;

  //[-------------------------------------------------------]
  //[ Protected virtual core::CoreApplication functions   ]
  //[-------------------------------------------------------]
protected:
  /**
   *  @brief
   *    Main function
   *
   *  @remarks
   *    The default implementation does the following tasks:
   *    - Run GUI main loop (processing GUI messages)
   *    - Exit loop when either the GUI or the application has been stopped
   */
  GUI_API void main() override;

protected:
  /** Pointer to the global, owning gui device instance */
  core::Ptr<GuiServer> mGuiServer;
  /** The gui context */
  core::Ptr<GuiContext> mGuiContext;
  /** Gui settings to use */
  GuiSettings mGuiSettings;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}