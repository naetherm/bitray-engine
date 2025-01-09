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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/color/color4.h"


namespace core {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Color4 Color4::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const Color4 Color4::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const Color4 Color4::RED(1.0f, 0.0f, 0.0f, 1.0f);
const Color4 Color4::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const Color4 Color4::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const Color4 Color4::GRAY(0.5f, 0.5f, 0.5f, 1.0f);
const Color4 Color4::CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const Color4 Color4::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const Color4 Color4::ORANGE(1.0f, 0.647059f, 0.0f, 1.0f);


Color4::Color4(const String &str)
: Color4() {
  sizeT pos = str.find(" ");
  sizeT curPos = 0;
  int idx = 0;
  while (pos != String::NPOS) {
    value[idx] = str.substr(curPos, pos).get_float();
    curPos = pos;
    pos = str.find(" ", curPos + 1);
  }
}

}