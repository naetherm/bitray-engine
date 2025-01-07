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
 * CursorImpl
 *
 * @brief
 * Platform independent cursor implementation, have to be implemented for all supported platforms.
 */
class CursorImpl {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] cursor
   * Platform independent cursor object, holding this one.
   */
  explicit CursorImpl(Cursor& cursor);

  /**
   * @brief
   * Destructor.
   */
  virtual ~CursorImpl();


  /**
   * @brief
   * Returns reference to cursor.
   *
   * @return
   * Reference to this cursor.
   */
  [[nodiscard]] const Cursor& get_cursor() const;

  /**
   * @brief
   * Get cursor ID
   *
   * @return
   * Cursor ID or CursorCustom, if no default cursor is used
   */
  EMouseCursor get_cursor_id() const;

public:

  /**
   * @brief
   * Load cursor
   *
   * @param[in] mouseCursor
   * Default cursor ID
   */
  virtual void load(EMouseCursor mouseCursor) = 0;

protected:
  /** Pointer to the platform independent cursor object */
  Cursor* mCursor;
  /** Default cursor ID (CustomCursor if a custom image has been loaded) */
  EMouseCursor mMouseCursor;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core