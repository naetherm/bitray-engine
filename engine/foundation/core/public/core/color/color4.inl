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
//[ Classes                                               ]
//[-------------------------------------------------------]
inline Color4::Color4()
: value{0.0f, 0.0f, 0.0f, 0.0f} {
  // Nothing here
}

inline Color4::Color4(const Color4 &source)
: value{source[0], source[1], source[2], source[3]} {
  // Nothing here
}

inline Color4::Color4(Color4 &source)
: value{source[0], source[1], source[2], source[3]} {
  // Nothing here
}

inline Color4::Color4(float _r, float _g, float _b, float _a)
: value{_r, _g, _b, _a} {
  // Nothing here
}

inline Color4::~Color4() {
  // Nothing here
}

inline Color4 &Color4::operator=(const Color4 &source) {
  value[0] = source.value[0];
  value[1] = source.value[1];
  value[2] = source.value[2];
  value[3] = source.value[3];
  return *this;
}

[[nodiscard]] inline Color4::operator float *() {
  return &value[0];
}

[[nodiscard]] inline Color4::operator const float *() const {
  return &value[0];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}