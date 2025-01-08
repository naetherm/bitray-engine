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
#include "core/input/control/axis.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {

Axis::Axis(Controller &controller, const String &name, const String &description)
: Control(controller, ControlType::ControlType_Axis, name, description)
, mValue(0.0f)
, mRelativeValue(false) {

}

Axis::Axis(const Axis &axis)
: Control(axis.mController, ControlType::ControlType_Axis, axis.mName, axis.mDescription)
, mValue(axis.mValue)
, mRelativeValue(axis.mRelativeValue) {

}

Axis::~Axis() {

}

Axis &Axis::operator=(const Axis &axis) {
  mValue = axis.mValue;
  mRelativeValue = axis.mRelativeValue;

  inform_update();

  return *this;
}

bool Axis::operator==(const Axis &axis) const {
  return ((mValue == axis.mValue) && (mRelativeValue == axis.mRelativeValue));
}

bool Axis::operator!=(const Axis &axis) const {
  return !operator==(axis);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core