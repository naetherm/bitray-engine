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
#include "core/frontend/cursor_device.h"
#include "core/frontend/cursor.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
CursorDevice::CursorDevice(WindowDevice &windowSystem)
: mWindowSystem(&windowSystem)
, mCurrentCursor(CursorDefault)
, mLastCursor(CursorDefault) {
  for (auto & cursor : mCursors) {
    cursor = nullptr;
  }
}

CursorDevice::~CursorDevice() {
  for (auto & cursor : mCursors) {
    delete cursor;
  }
}

const WindowDevice *CursorDevice::get_window_system() const {
  return mWindowSystem;
}

WindowDevice *CursorDevice::get_window_system() {
  return mWindowSystem;
}

Cursor *CursorDevice::get_cursor(EMouseCursor mouseCursor) {
  // Get pointer to standard cursor
  int nIndex = mouseCursor;
  if (!mCursors[nIndex]) {
    mLastCursor = mCurrentCursor;
    mCurrentCursor = mouseCursor;
    // Create cursor
    mCursors[nIndex] = new Cursor(*mWindowSystem);
    mCursors[nIndex]->load(mouseCursor);
  }

  // Return cursor
  return mCursors[nIndex];
}

void CursorDevice::restore_last_cursor() {
  get_cursor(mLastCursor);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
