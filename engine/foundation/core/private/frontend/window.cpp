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
#include "core/frontend/window_callbacks.h"
#include "core/frontend/window_device.h"
#if defined(LINUX)
#include "core/linux/linux_window.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Window::Window(WindowDevice* windowSystem)
: mWindowSystem(windowSystem) {
#if defined(LINUX)
  mImpl = new LinuxWindow(this);
#endif
}

Window::~Window() {
  // Clear all window callbacks
  for (auto wndCallback: mlstWindowCallbacks) {
    delete wndCallback;
  }
  mlstWindowCallbacks.clear();

  // Delete implementation
  delete mImpl;
}


void Window::create(const String &title, Recti shape, EWindowStyle windowStyle) {
  mImpl->create(title, shape, windowStyle);
}

void Window::destroy() {
  mImpl->destroy();
}


void Window::update() {
  mImpl->update();
}


void Window::on_tick(float deltaTime) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_tick(deltaTime);
    }
  }
}

void Window::on_draw() {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_draw();
    }
  }
}

void Window::on_size(int x, int y) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_size(x, y);
    }
  }
}

void Window::on_move(int x, int y) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_move(x, y);
    }
  }
}

void Window::on_leave() {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_leave();
    }
  }
}

void Window::on_enter() {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_enter();
    }
  }
}

void Window::on_mouse_position(int x, int y) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_mouse_position(x, y);
    }
  }
}

void Window::on_mouse_move(int x, int y) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_mouse_move(x, y);
    }
  }
}

void Window::on_mouse_wheel(float x) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_mouse_wheel(x);
    }
  }
}

void Window::on_mouse_button_down(InputMouseItem inputMouseItem) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_mouse_button_down(inputMouseItem);
    }
  }
}

void Window::on_mouse_button_up(InputMouseItem inputMouseItem) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_mouse_button_up(inputMouseItem);
    }
  }
}

void Window::on_key_press(InputKeyboardItem key) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_key_press(key);
    }
  }
}

void Window::on_key_release(InputKeyboardItem key) {
  for (auto wndCallback: mlstWindowCallbacks) {
    if (wndCallback) {
      wndCallback->on_key_release(key);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core