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
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract base class for platform specific 'ConsoleImpl' implementations
 */
class ConsoleImpl {

  friend class Console;

  //[-------------------------------------------------------]
  //[ Public virtual ConsoleImpl functions                  ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Print a string to the console
   *
   * @param[in] sString
   * String that shall be printed
   */
  virtual void print(const String &sString) const = 0;

  /**
   * @brief
   * Checks whether or not there's some keyboard input waiting on the console ('_kbhit()')
   *
   * @return
   * 0 if no key has been pressed, else not null
   */
  virtual int is_key_hit() const = 0;

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
  virtual int get_character(bool bEcho = false) const = 0;

  /**
   * @brief
   * Clears the console screen ('clrscr()')
   */
  virtual void clear_screens() const = 0;

  /**
   * @brief
   * Gets the absolute console cursor position ('wherex()' and 'wherey()')
   *
   * @param[out] nX
   * Receives the absolute x position of the console cursor, (0,0)=(left,top)
   * @param[out] nY
   * Receives the absolute y position of the console cursor, (0,0)=(left,top)
   */
  virtual void get_cursor_position(uint16 &nX, uint16 &nY) const = 0;

  /**
   * @brief
   * Sets the absolute console cursor position ('gotoxy()')
   *
   * @param[in] nX
   * New x absolute position of the console cursor, (0,0)=(left,top)
   * @param[in] nY
   * New y absolute position of the console cursor, (0,0)=(left,top)
   */
  virtual void set_cursor_position(uint16 nX, uint16 nY) const = 0;


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Constructor
   */
  ConsoleImpl() = default;

  /**
   * @brief
   * Destructor
   */
  virtual ~ConsoleImpl() = default;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
   * @brief
   * Copy constructor
   *
   * @param[in] cSource
   * Source to copy from
   */
  ConsoleImpl(const ConsoleImpl &cSource) = default;

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
  ConsoleImpl &operator=(const ConsoleImpl &cSource) = default;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core