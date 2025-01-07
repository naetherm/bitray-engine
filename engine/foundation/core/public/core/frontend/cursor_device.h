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
#include "core/core/refcounted.h"
#include "core/frontend/types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Cursor;
class WindowDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * CursorDevice
 *
 * @brief
 * Manages and controls the cursor for a given window system.
 *
 * The `CursorDevice` class handles the retrieval, management, and restoration of mouse cursors
 * for a window system. It maintains the current and last cursor states, and provides methods
 * for interacting with the cursor system.
 */
class CursorDevice : public RefCounted {
public:

  /**
   * @brief
   * Constructor.
   *
   * Initializes the `CursorDevice` with a reference to the associated window system.
   *
   * @param[in] windowSystem
   * A reference to the `WindowDevice` representing the window system to which the cursor belongs.
   */
  explicit CursorDevice(WindowDevice& windowSystem);

  /**
   * @brief
   * Destructor.
   *
   * Cleans up the `CursorDevice` instance and releases any resources associated with the cursors.
   */
  ~CursorDevice() override;

public:

  /**
   * @brief
   * Gets the associated window system.
   *
   * Returns a constant pointer to the `WindowDevice` associated with this cursor device.
   *
   * @return
   * A constant pointer to the associated `WindowDevice`.
   */
  [[nodiscard]] inline const WindowDevice* get_window_system() const;

  /**
   * @brief
   * Gets the associated window system.
   *
   * Returns a pointer to the `WindowDevice` associated with this cursor device.
   *
   * @return
   * A pointer to the associated `WindowDevice`.
   */
  [[nodiscard]] inline WindowDevice* get_window_system();

  /**
   * @brief
   * Retrieves a cursor based on the specified mouse cursor type.
   *
   * @param[in] mouseCursor
   * An enumerator of type `EMouseCursor` representing the desired cursor type.
   *
   * @return
   * A pointer to the `Cursor` corresponding to the specified cursor type.
   */
  Cursor* get_cursor(EMouseCursor mouseCursor);

  /**
   * @brief
   * Restores the last active cursor.
   *
   * Switches the current cursor back to the one that was active before the last cursor change.
   */
  void restore_last_cursor();

private:
  /** Pointer to the associated window system. */
  WindowDevice* mWindowSystem;
  /** Array of available cursors managed by this device. */
  Cursor* mCursors[Cursor_Count];
  /** The currently active mouse cursor. */
  EMouseCursor mCurrentCursor;
  /** The previously active mouse cursor. */
  EMouseCursor mLastCursor;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core