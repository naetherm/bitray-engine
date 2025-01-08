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


InputServer &Controller::get_input_manager() const {
  return mInputManager;
}

ControllerType Controller::get_controller_type() const {
  return mControllerType;
}

const String &Controller::get_name() const {
  return mName;
}

const String &Controller::get_description() const {
  return mDescription;
}

bool Controller::get_active() const {
  return mActive;
}

bool Controller::has_changed() const {
  const bool changed = mChanged;
  mChanged = false;

  return changed;
}

const Vector<Control *> &Controller::get_controls() const {
  return mControls;
}

const Vector<Button *> &Controller::get_buttons() const {
  if (mButtons.empty()) {
    init_control_list(ControlType::ControlType_Button);
  }
  return mButtons;
}

const Vector<Axis *> &Controller::get_axes() const {
  if (mAxes.empty()) {
    init_control_list(ControlType::ControlType_Axis);
  }
  return mAxes;
}

Control *Controller::get_control(const String &name) const {
  Map<String, Control*>::const_iterator iter = mMapControls.find(name);
  return (iter != mMapControls.cend()) ? iter.value() : nullptr;
}

char Controller::get_char() {
  const char result = mChar;
  mChar = 0;
  return result;
}

const Vector<Connection *> &Controller::get_connections() {
  return mConnections;
}

void Controller::set_active(bool active) {
  if (mControllerType == ControllerType::ControllerType_Virtual) {
    mActive = active;

    SignalOnActivate(mActive);
  }
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core