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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
[[nodiscard]] inline bool XcbBackend::is_initialized() const {
  return (mXcbConnection != nullptr);
}

[[nodiscard]] inline ::xcb_connection_t* XcbBackend::get_xcb_connection() const {
  return mXcbConnection;
}

[[nodiscard]] inline ::xcb_screen_t* XcbBackend::get_xcb_main_screen() const {
  return mXcbMainScreen;
}

[[nodiscard]] inline ::xcb_generic_error_t* XcbBackend::get_xcb_generic_error() const {
  return mXcbError;
}

[[nodiscard]] inline ::xcb_generic_event_t* XcbBackend::get_xcb_generic_event() const {
  return mXcbEvent;
}

inline void XcbBackend::set_xcb_generic_event(::xcb_generic_event_t* event) {
  mXcbEvent = event;
}

inline void XcbBackend::set_xcb_generic_error(::xcb_generic_error_t* error) {
  mXcbError = error;
}


[[nodiscard]] inline xcb_key_symbols_t* XcbBackend::get_xcb_key_symbols() const {
  return mXcbKeySymbols;
}

[[nodiscard]] inline struct xkb_state* XcbBackend::get_xkb_state() const {
  return mXkbState;
}

[[nodiscard]] inline struct xkb_context* XcbBackend::get_xkb_context() const {
  return mXkbContext;
}

[[nodiscard]] inline struct xkb_keymap* XcbBackend::get_xkb_keymap() const {
  return mXkbKeymap;
}

[[nodiscard]] inline const xcb_atom_t* XcbBackend::get_atoms() const {
  return &mXcbAtoms[0];
}

[[nodiscard]] inline xcb_atom_t XcbBackend::get_atom(AtomTypes type) const {
  return mXcbAtoms[type];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
