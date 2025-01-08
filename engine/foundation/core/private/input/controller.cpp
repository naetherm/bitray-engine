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
#include "core/input/controller.h"
#include "core/input/connection.h"
#include "core/input/input_server.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


Controller::Controller(InputServer& inputManager, ControllerType controllerType, const String& name, const String& description)
: mInputManager(inputManager)
, mControllerType(controllerType)
, mName(name)
, mDescription(description)
, mConfirmed(false)
, mActive(true)
, mChanged(false)
, mChar(0) {
  
}

Controller::~Controller() {
  // Remove all connections
  while (!mConnections.empty()) {
    disconnect(mConnections[0]);
  }
}



void Controller::connect(const String &outputControlName, Control &inputControl, float scale) {
  // get output control
  Control *pOutput = get_control(outputControlName);
  if (nullptr != pOutput && (&inputControl != pOutput) && (&inputControl.get_controller() != &pOutput->get_controller()))
  {
    // Create connection
    Connection *pConnection = new Connection(inputControl, *pOutput, scale);
    if (pConnection->is_valid())
    {
      // Add connection to both controllers
      inputControl.get_controller().add_connection(pConnection);
      add_connection(pConnection);
    }
    else
    {
      // Connection is invalid!
      // TODO(naetherm) Log entry
      delete pConnection;
    }
  }
}

void Controller::connect_all(Controller *controller, const String &prefixOut, const String &prefixIn) {
  if (controller) {
    // get all controls of input controller
    const Vector<Control*> &lstControls = controller->get_controls();
    for (uint32 i=0; i<lstControls.size(); i++) {
      // get control
      Control *pInput = lstControls[i];

      // Check if input control has the right prefix
      String sNameIn = pInput->get_name();
      if (prefixIn.empty() || sNameIn.find(prefixIn) == String::NPOS) {
        // get output name
        String sRoot = sNameIn.substr(0, prefixIn.size());
        String sNameOut = prefixOut + sRoot;

        // Try to create connection
        connect(sNameOut, *pInput);
      }
    }
  }
}

void Controller::disconnect(Connection *connection) {
  // Check connection
  if (connection && std::find(mConnections.cbegin(), mConnections.cend(), connection) != mConnections.cend()) {
    // get other controller
    Controller* controller = &connection->get_input_control().get_controller();
    if (controller == this)
    {
      controller = &connection->get_output_control().get_controller();
    }

    // Remove connection from both controllers
    controller->remove_connection(connection);
    remove_connection(connection);

    // Destroy connection
    delete connection;
  }
}

void Controller::update() {

}

void Controller::update_output_control(Control *control) {

}

void Controller::add_control(Control *control) {
  // Check if control is valid
  if (control) {
    // Add control to list
    mControls.push_back(control);

    // Add control to hash map
    mMapControls.set(control->get_name(), control);
  }
}

void Controller::inform_control(Control *control) {
  // Check if controller is active and control is valid
  if (mActive && control) {
    // Set changed-flag
    mChanged = true;

    // Check if a button was hit
    if (control->get_control_type() == ControlType::ControlType_Button && dynamic_cast<Button *>(control)->is_hit())
    {
      // Save key character
      mChar = dynamic_cast<Button *>(control)->get_character();
    }

    // Check if an output value was changed
    if (control->get_control_type() == ControlType::ControlType_Led || control->get_control_type() == ControlType::ControlType_Effect)
    {
      // Update output control
      update_output_control(control);
    }

    // Emit event
    SignalOnControl(*control);

    // Check connections
    for (uint32 i=0; i<mConnections.size(); i++) {
      // get connection
      Connection *connection = mConnections[i];

      // Check 'direction' that we must take
      if (control->is_input_control() && (&connection->get_input_control() == control))
      {
        // get the pointer to the controller that owns the output control
        // -> In case there's a controller, do only pass on the control event in case the controller is active
        if (connection->get_output_control().get_controller().get_active())
        {
          // Input control, pass from connection-input to connection-output
          connection->pass_value();
        }
      }
      else if (!control->is_input_control() && (&connection->get_output_control() == control))
      {
        // get the pointer to the controller that owns the input control
        // -> In case there's a controller, do only pass on the control event in case the controller is active
        if (connection->get_input_control().get_controller().get_active())
        {
          // Output control, pass backwards: from connection-output to connection-input
          connection->pass_value_backwards();
        }
      }
    }
  }
}

void Controller::init_control_list(ControlType controlType) const {
  // Clear list
  if (ControlType::ControlType_Button == controlType)
  {
    mButtons.clear();
  }
  else if (ControlType::ControlType_Axis == controlType)
  {
    mAxes.clear();
  }

  // Loop over all controls
  for (uint32 i=0; i<mControls.size(); i++) {
    // get control
    Control *pControl = mControls[i];

    // Add control to appropriate list
    if (pControl->get_control_type() == controlType)
    {
      if (ControlType::ControlType_Button == controlType)
      {
        mButtons.push_back(dynamic_cast<Button*>(pControl));
      }
      else if (ControlType::ControlType_Axis == controlType)
      {
        mAxes.push_back(dynamic_cast<Axis*>(pControl));
      }
    }
  }
}

void Controller::add_connection(Connection *connection) {
  // Avoid duplications
  if (connection && core::find(mConnections.cbegin(), mConnections.cend(), connection) == mConnections.cend()) {
    mConnections.push_back(connection);
  }
}

void Controller::remove_connection(Connection *connection) {
  if (connection) {
    Vector<Connection*>::iterator iter = core::find(mConnections.begin(), mConnections.end(), connection);

    if (iter != mConnections.cend()) {
      mConnections.erase(iter);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core