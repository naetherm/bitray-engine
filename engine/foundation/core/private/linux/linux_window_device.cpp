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
#include "core/linux/linux_window_device.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxWindowDevice::LinuxWindowDevice(WindowDevice* windowSystem)
: mWindowSystem(windowSystem)
, mXcbBackend(XcbBackend::instance())
, mDnDX(0)
, mDnDY(0)
, mIsDragging(false)
, mDraggingBegin(false)
, mSupportUriList(false) {
  initialize_window_system();
}

LinuxWindowDevice::~LinuxWindowDevice() {
  deinitialize_window_system();
}


XcbBackend &LinuxWindowDevice::get_xcb_backend() const {
  return mXcbBackend;
}

::xcb_connection_t *LinuxWindowDevice::get_xcb_connection() const {
  return mXcbBackend.get_xcb_connection();
}

::xcb_screen_t *LinuxWindowDevice::get_xcb_main_screen() const {
  return mXcbBackend.get_xcb_main_screen();
}

void LinuxWindowDevice::set_support_uri_list(bool support) {
  mSupportUriList = support;
}

bool LinuxWindowDevice::support_uri_list() const {
  return mSupportUriList;
}


void LinuxWindowDevice::set_dragging(bool b) {
  mIsDragging = b;
}

void LinuxWindowDevice::set_dragging_begins(bool b) {
  mDraggingBegin = b;
}

bool LinuxWindowDevice::is_dragging() const {
  return mIsDragging;
}

bool LinuxWindowDevice::dragging_begins() const {
  return mDraggingBegin;
}

void LinuxWindowDevice::set_dnd_position(int x, int y) {
  mDnDX = x;
  mDnDY = y;
}

core::int32 LinuxWindowDevice::get_dnd_x() const {
  return mDnDX;
}

core::int32 LinuxWindowDevice::get_dnd_y() const {
  return mDnDY;
}


void LinuxWindowDevice::initialize_window_system() {
  if (!mXcbBackend.is_initialized()) {
    mXcbBackend.initialize();
  }
}

void LinuxWindowDevice::deinitialize_window_system() {
  mXcbBackend.shutdown();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
