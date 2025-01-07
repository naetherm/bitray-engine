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
#include "core/frontend/window_impl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
inline void Window::add_window_callback(WindowCallbacks* windowCallback) {
  mlstWindowCallbacks.push_back(windowCallback);
}

inline const WindowDevice* Window::get_window_system() const {
  return mWindowSystem;
}

inline WindowDevice* Window::get_window_system() {
  return mWindowSystem;
}

inline const WindowImpl* Window::get_impl() const {
  return mImpl;
}

inline WindowImpl* Window::get_impl() {
  return mImpl;
}

inline bool Window::close_requested() const {
  return mImpl->close_requested();
}

inline handle Window::get_native_window_handle() const {
  return mImpl->get_native_window_handle();
}

inline const SWindowStatus& Window::get_window_status() const {
  return mImpl->get_window_status();
}

inline SWindowStatus& Window::get_window_status() {
  return mImpl->get_window_status();
}

inline void Window::set_shape(const Recti& shape) {
  mImpl->set_shape(shape);
}

inline void Window::set_size(const Vec2i& size) {
  mImpl->set_size(size);
}

inline void Window::set_position(const Vec2i& position) {
  mImpl->set_position(position);
}

inline FrontendPlatformData Window::get_platform_data() const {
  return mImpl->get_platform_data();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core