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
#include "core/frontend/window.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
const WindowDeviceImpl *WindowDevice::get_impl() const {
  return mImpl;
}

WindowDeviceImpl *WindowDevice::get_impl() {
  return mImpl;
}

inline uint32 WindowDevice::get_num_windows() const {
  return mWindows.size();
}

inline const Vector<Window *> &WindowDevice::get_windows() const {
  return mWindows;
}

inline Vector<Window *> &WindowDevice::get_windows() {
  return mWindows;
}

inline Window *WindowDevice::get_window(uint32 index) const {
  return mWindows[index];
}

inline Window *WindowDevice::get_active_window() const {
  return mWindows[mActiveWindow];
}

inline void WindowDevice::set_active_window(core::uint32 activeWindow) {
  mActiveWindow = activeWindow;
}

inline Window *WindowDevice::create_window(const WindowCreateDesc& windowCreateDesc) {
  Window* window = new Window(this);

  window->create(windowCreateDesc.title, windowCreateDesc.shape, windowCreateDesc.windowStyle);

  mWindows.push_back(window);

  mActiveWindow = mWindows.size() - 1;

  return window;
}

inline void WindowDevice::destroy_window(Window *window) {
  // TODO(naetherm): Should this be different
  window->destroy();
  delete window;
  size_t index = mWindows.find(window);
  mWindows.erase_at(index);
}

inline void WindowDevice::update() {
  for (auto window: mWindows) {
    window->update();
  }

  // Check if there are any windows to close
  for (auto window: mWindows) {
    if (window->close_requested()) {
      window->destroy();
      delete window;
      sizeT index = mWindows.find(window);
      mWindows.erase_at(index);
    }
  }
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core