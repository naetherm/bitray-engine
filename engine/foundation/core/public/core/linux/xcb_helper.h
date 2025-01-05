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
#include "core/input/types.h"
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
 * XcbHelper
 *
 * @brief
 * A helper class for converting XCB input events to application-specific items.
 *
 * The `XcbHelper` class provides static methods to convert XCB keysyms and mouse buttons
 * into application-specific input items, such as keyboard and mouse events.
 */
class XcbHelper {
public:
  /**
   * @brief
   * Converts an XCB keysym to an input keyboard item.
   *
   * This method takes an XCB keysym and converts it into an application-specific
   * `InputKeyboardItem` that can be used for handling keyboard input.
   *
   * @param[in] key
   * The XCB keysym to be converted.
   *
   * @return
   * The corresponding `InputKeyboardItem`.
   */
  static InputKeyboardItem convert_xcb_to_keysym(xcb_keysym_t key);

  /**
   * @brief
   * Converts an XCB mouse button to an input mouse item.
   *
   * This method takes an XCB mouse button and converts it into an application-specific
   * `InputMouseItem` that can be used for handling mouse input.
   *
   * @param[in] button
   * The XCB mouse button to be converted.
   *
   * @return
   * The corresponding `InputMouseItem`.
   */
  static InputMouseItem convert_xcb_to_mouse(xcb_button_t button);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core