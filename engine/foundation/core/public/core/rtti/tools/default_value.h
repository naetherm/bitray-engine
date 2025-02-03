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
#include "core/rtti/tools/dynamic_object.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * DefaultValue
 *
 * @brief Represents a default value for a DynamicObject.
 *
 * This class represents a default value for a DynamicObject. It is used by the
 * RTTI system to set default values for a DynamicObject.
 */
class DefaultValue {
public:

  /**
   * @brief Constructs a DefaultValue object with an empty DynamicObject.
   *
   * This default constructor initializes the DefaultValue object by setting
   * its internal mValue to an empty DynamicObject.
   */
  DefaultValue();

  /**
   * @brief Constructs a DefaultValue object with the given DynamicObject.
   *
   * This constructor initializes the DefaultValue object by setting its
   * internal mValue to the provided DynamicObject value.
   *
   * @param[in] value The DynamicObject used to initialize the DefaultValue.
   */
  DefaultValue(const DynamicObject& value);

  /**
   * @brief Destructor for the DefaultValue class.
   *
   * This is the default virtual destructor for the DefaultValue class,
   * ensuring proper cleanup of resources in derived classes.
   */
  virtual ~DefaultValue() = default;


  /**
   * @brief Retrieves the default value.
   *
   * This function returns a reference to the current default value
   * stored within the DynamicObject object.
   *
   * @return A constant reference to the default value.
   */
  const DynamicObject& get_default_value() const;

  /**
   * @brief Sets the default value.
   *
   * This function assigns the provided DynamicObject to the internal
   * representation of the default value.
   *
   * @param[in] value The DynamicObject to be set as the default value.
   */
  void set_default_value(const DynamicObject& value);

public:

  /**
   * @brief Restores the default value.
   *
   * This function should be overriden by derived classes to provide the
   * correct implementation for restoring the default value based on the
   * provided DynamicObject.
   *
   * @param[in] value The DynamicObject containing the default value.
   */
  virtual void restore_default_value(const DynamicObject& value) const = 0;

private:

  /**
   * @brief Holds the default value.
   *
   * This member variable stores the current default value as a DynamicObject.
   */
  DynamicObject mValue;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}