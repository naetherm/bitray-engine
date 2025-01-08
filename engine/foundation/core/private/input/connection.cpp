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
#include "core/input/connection.h"
#include "core/input/control/control.h"
#include "core/input/controller.h"
#include "core/input/control/axis.h"
#include "core/input/control/button.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {

Connection::Connection(Control &inputControl, Control &outputControl, float scale)
: mInputControl(inputControl)
, mOutputControl(outputControl)
, mScale(scale)
, mValid(false) {
  // Perform sanity checks
  // -> Check if both controls are valid and of the same control type
  // -> Check that the controls are either both input or both output controls
  // -> Check that the output control does not belong to a device
  if (&mInputControl != &mOutputControl && mInputControl.get_control_type() == mOutputControl.get_control_type() &&
      mInputControl.is_input_control() ==
      mOutputControl.is_input_control() &&
      mOutputControl.get_controller().get_controller_type() != ControllerType::ControllerType_Device)
  {
    // Connection is valid (can be changed by derived classes that have additional tests)
    mValid = true;
  }
}

Connection::~Connection() {

}

void Connection::pass_value() {
  // Check if connection is valid
  if (mValid)
  {
    // Pass value
    switch (mInputControl.get_control_type())
    {
      case ControlType::ControlType_Button:
        dynamic_cast<Button &>(mOutputControl).set_pressed(dynamic_cast<Button &>(mInputControl).is_pressed());
        break;

      case ControlType::ControlType_Axis:
      {
        const Axis& inputAxisControl = dynamic_cast<Axis&>(mInputControl);
        dynamic_cast<Axis &>(mOutputControl).set_value(inputAxisControl.get_value() * mScale,
                                                      inputAxisControl.is_relative_value());
        break;
      }

      case ControlType::ControlType_Unknown:
      case ControlType::ControlType_Led:
      case ControlType::ControlType_Effect:
      default:
        // There's nothing to pass on
        break;
    }
  }
}

void Connection::pass_value_backwards() {
  // Check if connection is valid
  if (mValid)
  {
    // Pass value
    switch (mOutputControl.get_control_type())
    {
      case ControlType::ControlType_Led:
        /// TODO(naetherm): Implement me!
        break;

      case ControlType::ControlType_Effect:
        /// TODO(naetherm): Implement me!
        break;

      case ControlType::ControlType_Unknown:
      case ControlType::ControlType_Button:
      case ControlType::ControlType_Axis:
      default:
        // There's nothing to pass backwards
        break;
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core