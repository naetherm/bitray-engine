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
#include "core/linux/linux_mouse_device.h"
#include "core/input/device/mouse.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxMouseDevice::LinuxMouseDevice()
: mXcbReply(nullptr)
, mMouseX(0)
, mMouseY(0) {

}

LinuxMouseDevice::~LinuxMouseDevice() {

}

void LinuxMouseDevice::update() {
  if (mDevice) {
    Mouse* mouse = reinterpret_cast<Mouse*>(mDevice);

    mXcbQueryPointerCookie = xcb_query_pointer (core::XcbBackend::instance().get_xcb_connection(), core::XcbBackend::instance().get_xcb_main_screen()->root);
    mXcbReply = xcb_query_pointer_reply (core::XcbBackend::instance().get_xcb_connection(), mXcbQueryPointerCookie, nullptr);

    // Update axes
    float nDeltaX = (float)(mXcbReply->root_x - mMouseX);
    float nDeltaY = (float)(mXcbReply->root_y - mMouseY);
    mMouseX = mXcbReply->root_x;
    mMouseY = mXcbReply->root_y;
    if (mouse->X.get_value() != nDeltaX)
      mouse->X.set_value(nDeltaX, true);
    if (mouse->Y.get_value() != nDeltaY)
      mouse->Y.set_value(nDeltaY, true);

    // Update buttons
    bool bPressed = ((mXcbReply->mask & XCB_BUTTON_MASK_1) != 0);
    if (mouse->Left.is_pressed() != bPressed)
    {
      mouse->Left.set_pressed(bPressed);
    }
    bPressed = ((mXcbReply->mask & XCB_BUTTON_MASK_2) != 0);
    if (mouse->Right.is_pressed() != bPressed)
    {
      mouse->Right.set_pressed(bPressed);
    }
    bPressed = ((mXcbReply->mask & XCB_BUTTON_MASK_3) != 0);
    if (mouse->Middle.is_pressed() != bPressed)
    {
      mouse->Middle.set_pressed(bPressed);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core