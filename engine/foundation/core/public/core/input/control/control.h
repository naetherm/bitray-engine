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
#include "core/input/types.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Controller;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Control
 *
 * @brief
 * Input control base implementation.
 */
class Control {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] controller
   * The owning controller.
   * @param[in] controlType
   * The control Type.
   * @param[in] name
   * The name.
   * @param[in] description
   * The description.
   */
  Control(Controller& controller, ControlType controlType, const String& name, const String& description);
  
  /**
   * @brief
   * Destructor.
   */
  virtual ~Control();
  
  
  /**
   * @brief
   * Returns reference to controller, owning this control.
   *
   * @return
   * Controller.
   */
  [[nodiscard]] inline Controller& get_controller() const;
  
  /**
   * @brief
   * Returns the control type.
   *
   * @return
   * The control type.
   */
  [[nodiscard]] inline ControlType get_control_type() const;
  
  /**
   * @brief
   * Returns true if this is an input control.
   *
   * @return
   * 'true' if this is an input control.
   */
  [[nodiscard]] inline bool is_input_control() const;
  
  /**
   * @brief
   * Returns the name of the control.
   *
   * @return
   * Name of control.
   */
  [[nodiscard]] inline const String& get_name() const;
  
  /**
   * @brief
   * Returns the description of the control.
   *
   * @return
   * Description of control.
   */
  [[nodiscard]] inline const String& get_description() const;
  
protected:
  /**
   * @brief
   * This method will inform the input manager that this control has changed.
   */
  void inform_update();
  
  Control& operator=(const Control& rhs) = delete;
  
protected:
  /** The owning controller */
  Controller& mController;
  /** The control type */
  ControlType mControlType;
  /** The name of the control */
  String mName;
  /** The description of the control */
  String mDescription;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/input/control/control.inl"