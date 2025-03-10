////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
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
#include "application.h"
#include "sample_window.h"
#include <core/frontend/cursor_device.h>
#include <core/frontend/window_device.h>
#include <gui/gui/gui_server.h>
#include <gui/gui/gui_window_callback.h>


Application::Application()
: gui::GuiApplication() {

}

Application::~Application() {

}



core::Ptr<gui::GuiWindow> Application::create_window(const core::WindowCreateDesc& windowCreateDesc) {
  core::uint32 colorId = get_window_device()->get_num_windows();
  auto window = get_window_device()->create_window(windowCreateDesc);

  // Append gui_window_callbacks
  window->add_window_callback(new gui::GuiWindowCallbacks());

  SampleWindow* guiWindow = new SampleWindow(mGuiServer, colorId);
  gui::GuiWindow* mainWindow = get_gui_server()->create_window(guiWindow, window);

  return core::Ptr<gui::GuiWindow>(guiWindow);
}


void Application::main() {
  auto window = create_window({"Some window", core::Recti(100, 100, 800, 600), core::EWindowStyle::WindowStyle_NoBorder});
  //auto window2 = create_window("Some window", core::Recti(200, 200, 800, 600), core::EWindowStyle::WindowStyle_NoBorder);

  while (true && get_window_device()->get_num_windows() > 0) {
    gui::GuiApplication::on_update();
    gui::GuiApplication::on_draw();
  }
}