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
#include "core/frontend/frontend_server.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
inline const Ptr<FrontendServer>& FrontendApplication::get_frontend() const {
  return mFrontend;
}

inline Ptr<FrontendServer>& FrontendApplication::get_frontend() {
  return mFrontend;
}

inline const Ptr<DisplayDevice>& FrontendApplication::get_display_device() const {
  return mFrontend->get_display_device();
}

inline Ptr<DisplayDevice>& FrontendApplication::get_display_device() {
  return mFrontend->get_display_device();
}

inline const Ptr<WindowDevice>& FrontendApplication::get_window_device() const {
  return mFrontend->get_window_device();
}

inline Ptr<WindowDevice>& FrontendApplication::get_window_device() {
  return mFrontend->get_window_device();
}

inline const Ptr<CursorDevice>& FrontendApplication::get_cursor_device() const {
  return mFrontend->get_cursor_device();
}

inline Ptr<CursorDevice>& FrontendApplication::get_cursor_device() {
  return mFrontend->get_cursor_device();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core