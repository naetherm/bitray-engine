////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
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
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


class ConsoleImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract base class for platform specific 'Console' implementations
 */
class Console {

public:

  static Console& instance();

  //[-------------------------------------------------------]
  //[ Public Console functions                              ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Print a string to the console
   *
   * @param[in] sString
   * String that shall be printed
   */
  void print(const String &sString) const;

  /**
   * @brief
   * Checks whether or not there's some keyboard input waiting on the console ('_kbhit()')
   *
   * @return
   * 0 if no key has been pressed, else not null
   */
  int is_key_hit() const;

  /**
   * @brief
   * Reads a single character from the console ('_getch()')
   *
   * @param[in] bEcho
   * Echo on the console?
   *
   * @return
   * The read character - note that there's no error code
   */
  int get_character(bool bEcho = false) const;

  /**
   * @brief
   * Clears the console screen ('clrscr()')
   */
  void clear_screens() const;

  /**
   * @brief
   * Gets the absolute console cursor position ('wherex()' and 'wherey()')
   *
   * @param[out] nX
   * Receives the absolute x position of the console cursor, (0,0)=(left,top)
   * @param[out] nY
   * Receives the absolute y position of the console cursor, (0,0)=(left,top)
   */
  void get_cursor_position(uint16 &nX, uint16 &nY) const;

  /**
   * @brief
   * Sets the absolute console cursor position ('gotoxy()')
   *
   * @param[in] nX
   * New x absolute position of the console cursor, (0,0)=(left,top)
   * @param[in] nY
   * New y absolute position of the console cursor, (0,0)=(left,top)
   */
  void set_cursor_position(uint16 nX, uint16 nY) const;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:

  Console();

  ~Console();

  /**
   * @brief
   * Copy constructor
   *
   * @param[in] cSource
   * Source to copy from
   */
  Console(const Console &cSource) = default;

  /**
   * @brief
   * Copy operator
   *
   * @param[in] cSource
   * Source to copy from
   *
   * @return
   * Reference to this instance
   */
  Console &operator=(const Console &cSource) = default;

private:
  /** Pointer to internal console implementation */
  ConsoleImpl* mConsoleImpl;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core