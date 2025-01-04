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
#include "core/linux/xcb_backend.h"
#include <cstring>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
XcbBackend &XcbBackend::instance() {
  static XcbBackend SInstance;
  return SInstance;
}

XcbBackend::XcbBackend()
: mXcbConnection(nullptr)
, mXcbMainScreen(nullptr)
, mXcbError(nullptr)
, mXcbEvent(nullptr)
, mXcbKeySymbols(nullptr)
, mXkbState(nullptr)
, mXkbContext(nullptr)
, mXkbKeymap(nullptr) {

}

XcbBackend::~XcbBackend() {

}

void XcbBackend::initialize() {
  // And now, initialize everything
  // Initialize xcb
  {
    mXcbConnection = xcb_connect(nullptr, nullptr);

    mXcbMainScreen = xcb_setup_roots_iterator(xcb_get_setup(mXcbConnection)).data;

    xcb_intern_atom_cookie_t cookies[ATOMS_COUNT];
    for (uint32 i = 0; i < ATOMS_COUNT; ++i) {
      create_atom(mXcbAtoms[i], cookies[i], SXcbAtomNames[i]);
    }
  }

  // Initialize xkb
  {
    // Initialize xkb subsystem
    uint16_t xkb_version;
    uint8_t base_event;
    xkb_x11_setup_xkb_extension(mXcbConnection, 1, 0, XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS, &xkb_version, nullptr, &base_event, nullptr);
    mXkbContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    core::int32 xkbDeviceId = xkb_x11_get_core_keyboard_device_id(mXcbConnection);
    mXkbKeymap = xkb_x11_keymap_new_from_device(mXkbContext, mXcbConnection, xkbDeviceId, XKB_KEYMAP_COMPILE_NO_FLAGS);
    mXkbState = xkb_x11_state_new_from_device(mXkbKeymap, mXcbConnection, xkbDeviceId);
  }
}

void XcbBackend::shutdown() {
  xcb_disconnect(mXcbConnection);
}

void XcbBackend::create_atom(xcb_atom_t& atom, xcb_intern_atom_cookie_t& cookie, const char* name) {
  cookie = xcb_intern_atom(mXcbConnection, 0, strlen(name), name);

  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(mXcbConnection, cookie, nullptr);
  if (reply) {
    atom = reply->atom;
    free(reply);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
