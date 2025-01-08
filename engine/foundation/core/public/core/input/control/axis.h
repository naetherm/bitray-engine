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
#include "core/input/control/control.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Axis
 *
 * @brief
 * Axis control.
 */
class Axis : public Control {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] controller
   * The owning controller.
   * @param[in] name
   * The name.
   * @param[in] description
   * The description.
   */
  Axis(Controller& controller, const String& name, const String& description);
  
  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] axis
   * Object to copy.
   */
  explicit Axis(const Axis& axis);
  
  /**
   * @brief
   * Destructor.
   */
  ~Axis() override;
  
  
  /**
   * @brief
   * Copy operator.
   *
   * @param[in] axis
   * Object to copy.
   *
   * @return
   * Reference to this instance.
   */
  Axis& operator=(const Axis& axis);
  
  [[nodiscard]] bool operator==(const Axis& axis) const;
  
  [[nodiscard]] bool operator!=(const Axis& axis) const;
  
  
  /**
   * @brief
   * Returns the value of the axis.
   *
   * @return
   * Value of axis.
   */
  [[nodiscard]] inline float get_value() const;
  
  /**
   * @brief
   * Sets the value of the axis.
   *
   * @param[in] value
   * The value.
   * @param[in] relativeValue
   * Determines whether the value is relative
   */
  inline void set_value(float value, bool relativeValue);
  
  /**
   * @brief
   * Returns whether value is relative.
   *
   * @return
   * 'true' if value is relative, false otherwise.
   */
  [[nodiscard]] inline bool is_relative_value() const;
  
private:
  /** The value of the axis */
  float mValue;
  /** True if this value is a relative one */
  bool mRelativeValue;
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/input/control/axis.inl"