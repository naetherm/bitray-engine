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
#include "core/math/rect.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Struct                                                ]
//[-------------------------------------------------------]
/**
 * @struct
 * LinuxFrontendPlatformData
 *
 * @brief
 * Linux dependent platform data information. THis is the common
 * data that is exchanged, e.g. when initializing the rhi libraries.
 */
struct LinuxFrontendPlatformData {
  void* connection;
  core::handle window;
};

/**
 * @struct
 * WindowsFrontendPlatformData
 *
 * @brief
 * Windows dependent platform data information. THis is the common
 * data that is exchanged, e.g. when initializing the rhi libraries.
 */
struct WindowsFrontendPlatformData {
  core::uint64 hwnd;
  core::uint64 window;
};

/**
 * @struct
 * FrontendPlatformData
 *
 * @brief
 * Basic platform independent representation of the platform data.
 * Each operating system has its own representation here, it is up
 * to us to use the correct one.
 */
struct FrontendPlatformData {
  union {
    LinuxFrontendPlatformData os_linux;
    WindowsFrontendPlatformData os_windows;
  };
};


/**
 * @struct
 * SWindowStatus
 *
 * @brief
 * Represents the current status of a window, including its focus, visibility,
 * and state (e.g., minimized or maximized).
 */
struct SWindowStatus {
  /** Indicates whether the window currently has focus. */
  bool HasFocus;

  /** Indicates whether the window is currently under the mouse cursor. */
  bool IsUnderMouse;

  /** Indicates whether the window is currently minimized. */
  bool IsMinimized;

  /** Indicates whether the window is currently maximized. */
  bool IsMaximized;

  /** Indicates whether the window is currently hidden. */
  bool IsHidden;

  /** Indicates whether the window was maximized before its current state. */
  bool WasMaximizedBefore;
};

/**
 * @struct
 * SWindowBorders
 *
 * @brief
 * Defines the border properties of a window, including resize margins
 * and title bar dimensions.
 */
struct SWindowBorders {
  /** The margin size around the window for resizing. */
  float ResizeMargin;

  /** The height of the window's caption area. */
  float CaptionHeight;

  /** The left margin of the title bar. */
  float TitlebarMarginLeft;

  /** The right margin of the title bar. */
  float TitlebarMarginRight;
};

/**
 * @enum
 * EWindowState
 *
 * @brief
 * Represents the possible states of a window.
 */
enum EWindowState {
  /** Window is maximized. */
  WindowState_Maximized,
  /** Window is minimized. */
  WindowState_Minimized,
  /** Window is in its default state. */
  WindowState_Default
};

/**
 * @enum
 * EWindowStyle
 *
 * @brief
 * Defines various styles and configurations for windows.
 */
enum EWindowStyle {
  /** Window is centered on the screen. */
  WindowStyle_Centered = 1,
  /** Window has a custom border. */
  WindowStyle_CustomBorder = 2,
  /** Disable DPI scaling for the window. */
  WindowStyle_NoDPIScaling = 4,
  /** Window is borderless. */
  WindowStyle_NoBorder = 8,
  /** Default position and scale for the window. */
  WindowStyle_DefaultPositionAndScale = 16
};


/**
 * @struct
 * WindowCreateDesc
 *
 * @brief
 * Describes the properties required to create a window, including its
 * title, shape, style, and borders.
 */
struct WindowCreateDesc {
  /** The title of the window. */
  String title;

  /** The dimensions and position of the window, represented as a rectangle. */
  Recti shape;

  /** The style of the window (e.g., resizable, borderless). */
  EWindowStyle windowStyle;

  /** The border properties of the window. */
  SWindowBorders borders;
};


/**
 * @enum
 * EMouseCursor
 *
 * @brief
 * Standard mouse cursors
 */
enum EMouseCursor {
  CursorDefault,
  CursorPointer,
  CursorText,
  CursorMove,
  CursorAllScroll,
  CursorColResize,
  CursorRowResize,
  CursorEWResize,
  CursorNSResize,
  CursorNESWResize,
  CursorNWSEResize,
  CursorDragAndDrop,
  CursorDragNoDrop,
  CursorNotAllowed,
  CursorCursorHide,
  Cursor_Count
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core