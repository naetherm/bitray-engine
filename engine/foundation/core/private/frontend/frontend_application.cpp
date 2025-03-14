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
#include "core/frontend/frontend_application.h"
#include "core/frontend/frontend_server.h"
#include "core/frontend/cursor_device.h"
#include "core/frontend/display_device.h"
#include "core/frontend/window_device.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
FrontendApplication::FrontendApplication()
: mFrontend(new FrontendServer(this)) {

}

FrontendApplication::~FrontendApplication() {
  
}


bool FrontendApplication::on_start() {
  if (CoreApplication::on_start()) {
    // Update list of screens
    mFrontend->get_display_device()->enumerate_displays();

    return true;
  }

  return false;
}

void FrontendApplication::on_stop() {
  mFrontend->get_display_device()->clear();
  mFrontend->get_window_device()->clear();
  CoreApplication::on_stop();
}

void FrontendApplication::on_size() {

}

void FrontendApplication::on_fullscreen_mode() {

}

void FrontendApplication::on_draw() {

}

void FrontendApplication::on_update() {
  // Update the window system here
  mFrontend->get_window_device()->update();
}

void FrontendApplication::on_drop(const Vector<String> &lstFiles) {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core