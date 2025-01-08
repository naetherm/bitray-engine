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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/container/map.h"
#include "core/container/vector.h"
#include "core/event/signal.h"
#include "core/input/types.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Control;
class Axis;
class Button;
class Connection;
class InputServer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Controller
 *
 * @brief
 * Basic input controller implementation.
 */
class Controller {

  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class InputServer;
  friend class DeviceProvider;
  friend class Control;

public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] inputManager
   * Reference to the input manager.
   * @param[in] controllerType
   * The controller type.
   * @param[in] name
   * The name of the controller.
   * @param[in] description
   * The description of the controller.
   */
  Controller(InputServer& inputManager, ControllerType controllerType, const String& name, const String& description);
  
  /**
   * @brief
   * Destructor.
   */
  virtual ~Controller();


  /**
   * @brief
   * Reference to the input manager.
   *
   * @return
   * Input manager.
   */
  [[nodiscard]] inline InputServer& get_input_manager() const;

  /**
   * @brief
   * The controller type.
   *
   * @return
   * The controller type.
   */
  [[nodiscard]] inline ControllerType get_controller_type() const;

  /**
   * @brief
   * Returns the name of the controller.
   *
   * @return
   * Name of controller.
   */
  [[nodiscard]] inline const String& get_name() const;

  /**
   * @brief
   * Returns the description of the controller.
   *
   * @return
   * Description of controller.
   */
  [[nodiscard]] inline const String& get_description() const;

  /**
   * @brief
   * Returns 'true' if controller is active.
   *
   * @return
   * 'true' if controller is active, 'false' otherwise.
   */
  [[nodiscard]] inline bool get_active() const;

  /**
   * @brief
   * 'true' if everything was changed.
   *
   * @return
   * 'true' if anything, regarding controller, was changed.
   */
  [[nodiscard]] inline bool has_changed() const;

  /**
   * @brief
   * Returns all controls of the controller.
   *
   * @return
   * All controls of controller.
   */
  [[nodiscard]] inline const Vector<Control*>& get_controls() const;

  /**
   * @brief
   * Returns all buttons of the controller.
   *
   * @return
   * All buttons of controller.
   */
  [[nodiscard]] inline const Vector<Button*>& get_buttons() const;

  /**
   * @brief
   * Returns all axes of the controller.
   *
   * @return
   * All axes of controller.
   */
  [[nodiscard]] inline const Vector<Axis*>& get_axes() const;

  /**
   * @brief
   * Returns pointer to controller by name.
   *
   * @param[in] name
   * The name of the control.
   *
   * @return
   * Pointer to control, if available, nullptr otherwise.
   */
  [[nodiscard]] inline Control* get_control(const String& name) const;

  /**
   * @brief
   * Returns character of controller.
   *
   * @return
   * Character of control.
   */
  [[nodiscard]] inline char get_char();

  /**
   * @brief
   * Returns all connections of controller.
   *
   * @return
   * All connections of controller.
   */
  [[nodiscard]] inline const Vector<Connection*>& get_connections();

  /**
   * @brief
   * Sets whether the controller is active.
   *
   * @param[in] active
   * Is controller active.
   */
  inline void set_active(bool active);

  /**
   * @brief
   * Responsible for connecting the output control to the input control.
   *
   * @param[in] outputControlName
   * The name of the output control.
   * @param[in] inputControl
   * The input control.
   * @param[in] scale
   * The scale factor.
   */
  void connect(const String& outputControlName, Control& inputControl, float scale = 1.0f);

  /**
   * @brief
   * Connect all controls to another controller.
   *
   * @param[in] controller
   * Pointer to the controller, contaiing the input controls.
   * @param[in] prefixOut
   * Prefix for controls of this controller
   * @param[in] prefixIn
   * Prefix for controlers of the other controller
   */
  void connect_all(Controller* controller, const String& prefixOut, const String& prefixIn);

  /**
   * @brief
   * Disconnect connection.
   *
   * @param[in] connection
   * Pointer to the connection to disconnect.
   */
  void disconnect(Connection* connection);

public:
  /**
   * @brief
   * Update controller once per frame.
   */
  virtual void update();

  /**
   * @brief
   * Updates the given output control.
   *
   * @param[in] control
   * The outut control that has been changed.
   */
  virtual void update_output_control(Control* control);

protected:
  /**
   * @brief
   * Add control.
   *
   * @param[in] control
   * Pointer to control.
   */
  void add_control(Control* control);

  /**
   * @brief
   * Inform controller that the given control has changed its state.
   *
   * @param[in] control
   * Pointer to control.
   */
  void inform_control(Control* control);

  /**
   * @brief
   * Initialize the control list.
   *
   * @param[in] controlType
   * The control type which should be initialized.
   */
  void init_control_list(ControlType controlType) const;

  /**
   * @brief
   * Add connection.
   *
   * @param[in] connection
   * Pointer to the connection.
   */
  void add_connection(Connection* connection);

  /**
   * @brief
   * Remove connection.
   *
   * @param[in] connection
   * Pointer to the connection.
   */
  void remove_connection(Connection* connection);

public:

  Signal<bool> SignalOnActivate;
  Signal<Control&> SignalOnControl;
  Signal<> SignalOnChanged;

protected:
  /** Input manager instance */
  InputServer& mInputManager;
  /** Controller type */
  ControllerType mControllerType;
  /** Name of controller */
  String mName;
  /** Description of controller */
  String mDescription;

  /** List of all controls */
  Vector<Control*> mControls;
  /** Map of all controls */
  Map<String, Control*> mMapControls;
  /** List of all buttons */
  mutable Vector<Button*> mButtons;
  /** List of all axis */
  mutable Vector<Axis*> mAxes;
  /** The last hit character key */
  char mChar;
  /** List of all connections */
  Vector<Connection*> mConnections;

  /** The confirmation flag from detect_devices() method */
  bool mConfirmed;
  /** Determines whether the controller is active */
  bool mActive;
  /** Determines whether the controller state has changed */
  mutable bool mChanged;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/input/controller.inl"