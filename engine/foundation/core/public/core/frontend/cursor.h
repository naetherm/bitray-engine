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
#include "core/frontend/types.h"
#include "core/math/vec2.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class CursorImpl;
class WindowDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Cursor
 *
 * @brief
 * Platform independent cursor implementation.
 */
class Cursor {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] windowSystem
   * Reference to window system.
   */
  explicit Cursor(WindowDevice& windowSystem);

  /**
   * @brief
   * Constructor.
   *
   * @param[in] windowSystem
   * Reference to window system.
   * @param[in] mouseCursor
   * The cursor type to load.
   */
  Cursor(WindowDevice& windowSystem, EMouseCursor mouseCursor);

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  Cursor(const Cursor& rhs);

  /**
   * @brief
   * Destructor.
   */
  ~Cursor();


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * Object to copy.
   *
   * @return
   * Reference to this.
   */
  Cursor& operator=(const Cursor& rhs);


  /**
   * @brief
   * Pointer to window system.
   *
   * @return
   * Window system.
   */
  const WindowDevice* get_window_system() const;

  /**
   * @brief
   * Pointer to window system.
   *
   * @return
   * Window system.
   */
  WindowDevice* get_window_system();

  /**
   * @brief
   * Pointer to internal implementation.
   *
   * @return
   * Internal implementation.
   */
  CursorImpl* get_impl() const;

  /**
   * @brief
   * Loads the mouse cursor @p mouseCursor.
   *
   * @param[in] mouseCursor
   * The mouse cursor type to load.
   */
  void load(EMouseCursor mouseCursor);

  /**
   * @brief
   * Returns the mouse cursor type, represented by this cursor.
   *
   * @return
   * Mouse cursor type.
   */
  EMouseCursor get_cursor_id() const;

protected:
  /** Pointer to window system */
  WindowDevice* mWindowSystem;
  /** Pointer to internal implementation */
  CursorImpl* mImpl;
  /** The mouse cursor type. */
  EMouseCursor mMouseCursor;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core