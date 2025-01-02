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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/platform/console.h"
#if defined(LINUX)
#include "core/linux/linux_console.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Console &Console::instance() {
  static Console SInstance;
  return SInstance;
}


Console::Console() {
#if defined(LINUX)
  mConsoleImpl = new LinuxConsole();
#endif
}

Console::~Console() {
  delete mConsoleImpl;
}

void Console::print(const String &sString) const {
  mConsoleImpl->print(sString);
}

int Console::is_key_hit() const {
  return mConsoleImpl->is_key_hit();
}

int Console::get_character(bool bEcho) const {
  return mConsoleImpl->get_character(bEcho);
}

void Console::clear_screens() const {
  mConsoleImpl->clear_screens();
}

void Console::get_cursor_position(uint16 &nX, uint16 &nY) const {
  mConsoleImpl->get_cursor_position(nX, nY);
}

void Console::set_cursor_position(uint16 nX, uint16 nY) const {
  mConsoleImpl->set_cursor_position(nX, nY);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}