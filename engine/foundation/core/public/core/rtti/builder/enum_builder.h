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
class Enum;
class TypeInfo;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * EnumBuilder is a helper class that is used to register the values of an Enum.
 *
 * EnumBuilder is used to register the values of an Enum with the Runtime Type
 * Information (RTTI) system. It is used by the MACRO core_ENUM to register the
 * values of an Enum, but it can also be used directly to register the values of
 * an Enum.
 */
template<typename TEnum>
class EnumBuilder {
public:

  /**
   * @brief
   * Constructs an EnumBuilder object that is used to register the values of an
   * Enum.
   *
   * @param e The Enum instance that the EnumBuilder will be used to register the
   * values of.
   */
  explicit EnumBuilder(Enum& e);


  /**
   * @brief
   * Adds a new value to the Enum with the specified name and value.
   *
   * @param name The name of the value to add.
   * @param value The value of the value to add.
   *
   * @return The EnumBuilder instance, allowing method chaining.
   */
  EnumBuilder<TEnum>& value(const String& name, core::int32 value);

private:

  Enum* mEnum;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/builder/enum_builder.inl"