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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "gui/application/gui_application.h"
#include "gui/gui/gui_context.h"
#include "gui/gui/gui_server.h"
#include "gui/gui/gui_window.h"
#include <core/frontend/window_device.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiApplication::GuiApplication()
: mGuiServer(GuiServer::instance()) {
  // Add default settings to gui settings
  mGuiSettings.mRhiName = "opengl";
}

GuiApplication::~GuiApplication() {

}
    

const core::Ptr<GuiServer>& GuiApplication::get_gui_server() const {
  return mGuiServer;
}

core::Ptr<GuiServer>& GuiApplication::get_gui_server() {
  return mGuiServer;
}


core::Ptr<GuiWindow> GuiApplication::create_window(const core::WindowCreateDesc& windowCreateDesc) {
  // Create native window
  core::Window* nativeWindow = get_window_device()->create_window(windowCreateDesc);

  // Create new gui window
  GuiWindow* guiWindow = new GuiWindow(mGuiServer);
  get_gui_server()->create_window(guiWindow, nativeWindow);
  
  return core::Ptr<GuiWindow>(guiWindow);
}

bool GuiApplication::on_start() {
  if (FrontendApplication::on_start()) {

    // Initialize the gui context/renderer?
    mGuiContext = new GuiContext();
    if (mGuiContext->initialize(mGuiSettings)) {
      // Initialize the gui system
      mGuiServer->initialize(*mGuiContext);

      //

      // Done
      return true;
    }

    // Error
    return false;
  }

  // Error
  return false;
}

void GuiApplication::on_stop() {
  mGuiServer->shutdown();
  FrontendApplication::on_stop();
}

void GuiApplication::on_draw() {
  FrontendApplication::on_draw();

  mGuiServer->draw();
}

void GuiApplication::on_update() {
  FrontendApplication::on_update();
}

void GuiApplication::main() {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}