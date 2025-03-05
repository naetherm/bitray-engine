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
#include "core/container/hash_map.h"
#include "core/container/vector.h"
#include "core/rtti/enum/enum_value.h"


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
template<typename TEnum>
class EnumBuilder;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Enum
 *
 * @brief
 * A class representing a named enumeration in the RTTI system.
 *
 * Enum is a part of the RTTI system that allows for the dynamic querying and
 * registration of named enumerations.
 */
class Enum : public RttiMember {

  template<typename TEnum> friend class EnumBuilder;

public:

  /**
   * @brief
   * A static method to create an EnumBuilder instance and start declaring an enum.
   *
   * @param name The name of the enum to be declared.
   *
   * @return An EnumBuilder instance for the enum with the given name.
   */
  template<typename TEnum>
  static EnumBuilder<TEnum> declare(const String& name);

public:

  /**
   * @brief
   * Constructs an Enum object with the specified name.
   *
   * @param name The name of the enum.
   */
  Enum(const String& name);

  /**
   * @brief
   * The destructor for `Enum`.
   *
   * Cleans up any dynamically allocated memory used by the `Enum`.
   */
  ~Enum() override;


  /**
   * @brief
   * Returns the number of enum values registered in this enum.
   *
   * @return The number of enum values.
   */
  [[nodiscard]] const core::sizeT get_num_of_enum_values() const;

  /**
   * @brief
   * Returns the enum value with the specified name.
   *
   * @param name The name of the enum value.
   *
   * @return The enum value with the specified name.
   */
  [[nodiscard]] const EnumValue& get_enum_value_by_name(const String& name) const;

  /**
   * @brief
   * Returns the enum value with the specified name.
   *
   * @param name The name of the enum value.
   *
   * @return The enum value with the specified name.
   */
  [[nodiscard]] EnumValue& get_enum_value_by_name(const String& name);

  /**
   * @brief
   * Returns the enum value at the specified index.
   *
   * @param index The index of the enum value.
   *
   * @return The enum value at the specified index.
   */
  [[nodiscard]] const EnumValue& get_enum_value_by_index(core::sizeT index) const;

  /**
   * @brief
   * Returns the enum value at the specified index.
   *
   * @param index The index of the enum value.
   *
   * @return The enum value at the specified index.
   */
  [[nodiscard]] EnumValue& get_enum_value_by_index(core::sizeT index);

private:

  /**
   * @brief
   * Adds an enum value to the enum.
   *
   * @param name The name of the enum value.
   * @param value The value of the enum value.
   */
  void add_enum_value(const String& name, core::int32 value);

private:
  /** The name of the enum */
  String mName;
  /** vector of all enum values */
  core::Vector<EnumValue> mEnumValues;
  /** Map of all enum values */
  core::hash_map<String, EnumValue> mEnumValueMap;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/enum.inl"