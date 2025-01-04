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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/linux/xcb_includes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * XcbBackend
 *
 * @brief
 * A backend class for managing XCB (X protocol C-language Binding) connections,
 * screens, events, and related resources. Provides functionality to interact with
 * X server resources, handle atoms, and manage keyboard configurations.
 */
class XcbBackend {
public:

  /**
   * @brief
   * Retrieves the singleton instance of the `XcbBackend` class.
   *
   * @return A reference to the singleton instance.
   */
  static XcbBackend& instance();

public:
  /**
   * @brief
   * Constructor.
   *
   * Initializes the `XcbBackend` instance.
   */
  XcbBackend();

  /**
   * @brief
   * Destructor.
   *
   * Cleans up resources used by the `XcbBackend` instance.
   */
  ~XcbBackend();

  /**
   * @brief
   * Initializes the XCB backend and its associated resources.
   */
  void initialize();

  /**
   * @brief
   * Shuts down the XCB backend and releases all resources.
   */
  void shutdown();

  /**
   * @brief
   * Checks whether the XCB backend is initialized.
   *
   * @return True if initialized, false otherwise.
   */
  [[nodiscard]] inline bool is_initialized() const;

public:
  /**
   * @brief
   * Gets the XCB connection handle.
   *
   * @return A pointer to the XCB connection.
   */
  [[nodiscard]] inline ::xcb_connection_t* get_xcb_connection() const;

  /**
   * @brief
   * Gets the main XCB screen.
   *
   * @return A pointer to the main XCB screen.
   */
  [[nodiscard]] inline ::xcb_screen_t* get_xcb_main_screen() const;

  /**
   * @brief
   * Gets the current XCB generic error.
   *
   * @return A pointer to the XCB generic error.
   */
  [[nodiscard]] inline ::xcb_generic_error_t* get_xcb_generic_error() const;

  /**
   * @brief
   * Gets the current XCB generic event.
   *
   * @return A pointer to the XCB generic event.
   */
  [[nodiscard]] inline ::xcb_generic_event_t* get_xcb_generic_event() const;

  /**
   * @brief
   * Sets the current XCB generic event.
   *
   * @param[in] event A pointer to the XCB generic event.
   */
  inline void set_xcb_generic_event(::xcb_generic_event_t* event);

  /**
   * @brief
   * Sets the current XCB generic error.
   *
   * @param[in] error A pointer to the XCB generic error.
   */
  inline void set_xcb_generic_error(::xcb_generic_error_t* error);

  /**
   * @brief
   * Gets the XCB key symbols.
   *
   * @return A pointer to the XCB key symbols.
   */
  [[nodiscard]] inline xcb_key_symbols_t* get_xcb_key_symbols() const;

  /**
   * @brief
   * Gets the XKB state.
   *
   * @return A pointer to the XKB state.
   */
  [[nodiscard]] inline struct xkb_state* get_xkb_state() const;

  /**
   * @brief
   * Gets the XKB context.
   *
   * @return A pointer to the XKB context.
   */
  [[nodiscard]] inline struct xkb_context* get_xkb_context() const;

  /**
   * @brief
   * Gets the XKB keymap.
   *
   * @return A pointer to the XKB keymap.
   */
  [[nodiscard]] inline struct xkb_keymap* get_xkb_keymap() const;

  /**
   * @brief
   * Gets all XCB atoms.
   *
   * @return A pointer to an array of XCB atoms.
   */
  [[nodiscard]] inline const xcb_atom_t* get_atoms() const;

  /**
   * @brief
   * Gets a specific XCB atom by type.
   *
   * @param[in] type The type of atom to retrieve.
   *
   * @return The requested XCB atom.
   */
  [[nodiscard]] inline xcb_atom_t get_atom(AtomTypes type) const;

private:
  /**
   * @brief
   * Creates an XCB atom.
   *
   * @param[in] atom The atom to be created.
   * @param[in] cookie The cookie representing the atom creation request.
   * @param[in] name The name of the atom to create.
   */
  void create_atom(xcb_atom_t& atom, xcb_intern_atom_cookie_t& cookie, const char* name);

private:

  /** The XCB connection handle. */
  ::xcb_connection_t* mXcbConnection;

  /** The main XCB screen. */
  ::xcb_screen_t* mXcbMainScreen;

  /** The current XCB generic error. */
  ::xcb_generic_error_t* mXcbError;

  /** The current XCB generic event. */
  ::xcb_generic_event_t* mXcbEvent;

  /** The XCB key symbols. */
  xcb_key_symbols_t* mXcbKeySymbols;

  /** The current XKB state. */
  struct xkb_state* mXkbState;

  /** The current XKB context. */
  struct xkb_context* mXkbContext;

  /** The current XKB keymap. */
  struct xkb_keymap* mXkbKeymap;

  /** Array of XCB atoms. */
  xcb_atom_t mXcbAtoms[ATOMS_COUNT];
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/linux/xcb_backend.inl"