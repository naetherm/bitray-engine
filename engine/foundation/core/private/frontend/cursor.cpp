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
#include "core/frontend/cursor.h"
#if defined(LINUX)
#include "core/linux/linux_cursor.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Cursor::Cursor(WindowDevice& windowSystem)
: mWindowSystem(&windowSystem) {
#if defined(LINUX)
  mImpl = new LinuxCursor(*this);
#endif
}

Cursor::Cursor(WindowDevice& windowSystem, EMouseCursor mouseCursor)
: mWindowSystem(&windowSystem) {
#if defined(LINUX)
  mImpl = new LinuxCursor(*this);
#endif
  mImpl->load(mouseCursor);
}

Cursor::Cursor(const Cursor& rhs) {
  mWindowSystem = rhs.mWindowSystem;
  mMouseCursor = rhs.mMouseCursor;
  mImpl = rhs.mImpl;
}

Cursor::~Cursor() {
  delete mImpl;
}


Cursor& Cursor::operator=(const Cursor& rhs) {
  mWindowSystem = rhs.mWindowSystem;
  mMouseCursor = rhs.mMouseCursor;
  mImpl = rhs.mImpl;

  return *this;
}


const WindowDevice* Cursor::get_window_system() const {
  return mWindowSystem;
}

WindowDevice* Cursor::get_window_system() {
  return mWindowSystem;
}

CursorImpl* Cursor::get_impl() const {
  return mImpl;
}

void Cursor::load(EMouseCursor mouseCursor) {
  mMouseCursor = mouseCursor;

  mImpl->load(mouseCursor);
}

EMouseCursor Cursor::get_cursor_id() const {
  return mMouseCursor;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
