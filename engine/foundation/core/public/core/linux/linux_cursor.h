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
#include "core/frontend/cursor_impl.h"
#include "core/linux/xcb_includes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Cursor;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * LinuxCursor
 *
 * @brief
 * Linux cursor implementation.
 */
class LinuxCursor : public CursorImpl {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] cursor
   * Platform independent cursor object, holding this one.
   */
  LinuxCursor(Cursor& cursor);

  /**
   * @brief
   * Destructor.
   */
  ~LinuxCursor() override;

public:

  /**
   * @brief
   * Load cursor
   *
   * @param[in] mouseCursor
   * Default cursor ID
   */
  void load(EMouseCursor mouseCursor) override;

  /**
   * @brief
   * Returns the xcb cursor implementation.
   *
   * @return
   * The xcb cursor implementation.
   */
  xcb_cursor_t get_impl() const;

private:
  /** Pointer to xcb cursor context */
  xcb_cursor_context_t* mXcbCursorContext;
  /** The xcb cursor */
  xcb_cursor_t mXcbCursor;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core