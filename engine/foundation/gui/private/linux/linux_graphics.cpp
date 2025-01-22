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
#include "gui/linux/linux_graphics.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
LinuxGraphics::LinuxGraphics(cairo_t* cairoContext)
: mCairoContext(cairoContext)
, mCurrentColor(core::Color3::WHITE) {
}

LinuxGraphics::~LinuxGraphics() {
}


void LinuxGraphics::draw_line(const core::Vec2i& start, const core::Vec2i& end, core::Color3& rgb) {
  set_current_color(rgb);
  // Draw
  cairo_move_to(mCairoContext, start.x, start.y);
  cairo_line_to(mCairoContext, end.x, end.y);
  cairo_stroke(mCairoContext);
}

void LinuxGraphics::set_current_color(const core::Color3& rgb) {
  if (mCurrentColor != rgb) {
    mCurrentColor = rgb;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}