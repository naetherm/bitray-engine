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
#include "core/linux/linux_cursor.h"
#include "core/linux/linux_window_device.h"
#include "core/frontend/cursor.h"
#include "core/frontend/window_device.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxCursor::LinuxCursor(Cursor &cursor)
: CursorImpl(cursor)
, mXcbCursorContext(nullptr)
, mXcbCursor(0) {
  xcb_cursor_context_new(
    reinterpret_cast<LinuxWindowDevice*>(mCursor->get_window_system()->get_impl())->get_xcb_connection(),
    reinterpret_cast<LinuxWindowDevice*>(mCursor->get_window_system()->get_impl())->get_xcb_main_screen(),
    &mXcbCursorContext
    );
}

LinuxCursor::~LinuxCursor() {
  if (mXcbCursorContext)
    xcb_cursor_context_free(mXcbCursorContext);
}


void LinuxCursor::load(EMouseCursor mouseCursor) {
  if (mXcbCursor == 0) {
    const char* shape = nullptr;
    switch (mouseCursor) {

      case CursorDefault:
        shape = "left_ptr"; break;
      case CursorPointer:
        shape = "hand1"; break;
      case CursorText:
        shape = "xterm"; break;
      case CursorMove:
        shape = "fleur"; break;
      case CursorAllScroll:
        shape = "fleur"; break;
      case CursorColResize:
        shape = "sb_h_double_arrow"; break;
      case CursorRowResize:
        shape = "sb_v_double_arrow"; break;
      case CursorEWResize:
        shape = "sb_h_double_arrow"; break;
      case CursorNSResize:
        shape = "sb_v_double_arrow"; break;
      case CursorNESWResize:
        shape = "diamond_cross"; break;
      case CursorNWSEResize:
        shape = "diamond_cross"; break;
      case CursorDragAndDrop:
        shape = "hand1"; break;
      case CursorDragNoDrop:
        shape = "X_cursor"; break;
      case CursorNotAllowed:
        shape = "X_cursor"; break;
      case CursorCursorHide:
        shape = "left_ptr"; break;
      default:
        shape = "left_ptr"; break;
    }
    mXcbCursor = xcb_cursor_load_cursor(mXcbCursorContext, shape);
  }

  xcb_change_window_attributes(
    reinterpret_cast<LinuxWindowDevice*>(mCursor->get_window_system()->get_impl())->get_xcb_connection(),
    mCursor->get_window_system()->get_active_window()->get_native_window_handle(),
    XCB_CW_CURSOR,
    &mXcbCursor);
  xcb_flush(reinterpret_cast<LinuxWindowDevice*>(mCursor->get_window_system()->get_impl())->get_xcb_connection());
}

xcb_cursor_t LinuxCursor::get_impl() const {
  return mXcbCursor;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
