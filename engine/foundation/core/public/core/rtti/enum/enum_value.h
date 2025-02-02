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
#include "core/rtti/rtti_member.h"


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
 * EnumValue
 *
 * @brief
 * Represents a named value in an enumeration.
 *
 * EnumValue is a part of the RTTI system that holds a name and a value,
 * allowing for the dynamic querying of enum members.
 */
class EnumValue : public RttiMember {
public:

  /**
   * @brief
   * Constructs an EnumValue object with no name and the value 0.
   *
   * This is a convenience constructor that is useful for creating EnumValue objects
   * without having to specify a name and value.
   */
  EnumValue();

  /**
   * @brief
   * Constructs an EnumValue object with the specified name and value.
   *
   * @param name The name of the enum value.
   * @param value The value of the enum value.
   */
  EnumValue(const String& name, core::int32 value);


  /**
   * @brief
   * Compares two EnumValue objects for equality.
   *
   * The two EnumValue objects are considered equal if their names and values are equal.
   *
   * @param rhs The EnumValue object to compare with.
   *
   * @return true if the two EnumValue objects are equal, and false otherwise.
   */
  bool operator==(const EnumValue& rhs) const;


  /**
   * @brief
   * Returns the name of the enum value.
   *
   * @return A constant reference to the name of the enum value.
   */
  [[nodiscard]] const String& get_name() const;

  /**
   * @brief
   * Returns the value of the enum value.
   *
   * @return The value of the enum value.
   */
  [[nodiscard]] core::int32 get_value() const;

private:

  /** The name of the enum value. */
  String mName;

  /** The value of the enum value. */
  core::int32 mValue;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}