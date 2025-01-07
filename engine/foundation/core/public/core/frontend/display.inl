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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Display::Display(const Recti& shape, float dpiFactor)
: mShape(shape)
, mDpiFactor(dpiFactor) {

}

Display::Display(const Display& rhs)
: mShape(rhs.mShape)
, mDpiFactor(rhs.mDpiFactor) {

}


inline bool Display::operator==(const Display& rhs) const {
  return mShape == rhs.mShape && mDpiFactor == rhs.mDpiFactor;
}

inline bool Display::operator!=(const Display& rhs) const {
  return !operator==(rhs);
}

Display& Display::operator=(const Display& rhs) {
  mShape = rhs.mShape;
  mDpiFactor = rhs.mDpiFactor;

  return *this;
}


const Recti& Display::get_shape() const {
  return mShape;
}

float Display::get_dpi_factor() const {
  return mDpiFactor;
}

void Display::set_shape(const Recti& shape) {
  mShape = shape;
}

void Display::set_dpi_factor(float dpiFactor) {
  mDpiFactor = dpiFactor;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core