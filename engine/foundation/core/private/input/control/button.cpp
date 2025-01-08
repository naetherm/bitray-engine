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
#include "core/input/control/button.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


Button::Button(Controller &controller, const String &name, const String &description, char character)
: Control(controller, ControlType::ControlType_Button, name, description)
, mCharacter(character)
, mPressed(false)
, mHit(false) {

}

Button::Button(const Button &button)
: Control(button.mController, ControlType::ControlType_Button, button.mName, button.mDescription)
, mCharacter(button.mCharacter)
, mPressed(button.mPressed)
, mHit(button.mHit) {

}

Button::~Button() {

}

Button &Button::operator=(const Button &button) {
  mCharacter = button.mCharacter;
  mPressed = button.mPressed;
  mHit = button.mHit;

  inform_update();

  return *this;
}

bool Button::operator==(const Button &button) const {
  return ((mCharacter == button.mCharacter) && (mPressed == button.mPressed) && (mHit == button.mHit));
}

bool Button::operator!=(const Button &button) const {
  return !operator==(button);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core