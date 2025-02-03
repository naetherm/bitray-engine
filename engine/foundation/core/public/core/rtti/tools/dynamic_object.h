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
#include "core/rtti/typetraits/untyped_variant.h"


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
class TypeInfo;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * DynamicObject
 *
 * @brief A class representing a dynamic object in the RTTI system.
 *
 * This class represents a dynamic object in the RTTI system. It is used to
 * store values of any type and provides functions to set and retrieve the
 * stored value.
 */
class DynamicObject {
public:

  /**
   * @brief Constructs a DynamicObject with an empty internal variant.
   *
   * This constructor initializes the DynamicObject with an empty internal variant.
   * The dynamic object is empty and cannot be used until a value has been set
   * using the set() function.
   */
  DynamicObject();

  /**
   * @brief Constructs a DynamicObject with the given instance.
   *
   * This constructor initializes the DynamicObject by setting its internal
   * variant to the provided instance. The type information is also updated
   * based on the instance type.
   *
   * @tparam TType The type of the instance to be stored in the DynamicObject.
   * @param instance The instance used to initialize the DynamicObject.
   */
  template<typename TType>
  inline DynamicObject(TType instance);

  /**
   * @brief Constructs a DynamicObject with the given variant and type information.
   *
   * This constructor initializes the DynamicObject by setting its internal
   * variant to the provided variant and its type information to the given
   * type information.
   *
   * @param variant The variant used to initialize the DynamicObject.
   * @param typeInfo The type information of the variant used to initialize the
   * DynamicObject.
   */
  DynamicObject(const core::UntypedVariant<>& variant, const TypeInfo* typeInfo);


  /**
   * @brief Checks if two DynamicObjects are equal.
   *
   * This function compares two DynamicObjects and returns true if they are
   * equal, false otherwise.
   *
   * @param other The other DynamicObject used for comparison.
   * @return True if the DynamicObjects are equal, false otherwise.
   */
  bool operator==(const DynamicObject& other) const;


  /**
   * @brief Sets the internal variant of the DynamicObject to the given instance.
   *
   * This function sets the internal variant of the DynamicObject to the given
   * instance. The type information is also updated based on the instance type.
   *
   * @tparam TType The type of the instance to be stored in the DynamicObject.
   * @param instance The instance used to set the internal variant of the
   * DynamicObject.
   *
   * @return A constant reference to the DynamicObject.
   */
  template<typename TType>
  inline void set(TType instance);


  /**
   * @brief Retrieves the internal variant of the DynamicObject.
   *
   * This function returns a constant reference to the internal variant of the
   * DynamicObject. The internal variant contains the stored value.
   *
   * @return A constant reference to the internal variant of the DynamicObject.
   */
  [[nodiscard]] inline const core::UntypedVariant<>& get_untyped() const;

  /**
   * @brief Retrieves the type information of the stored value.
   *
   * This function returns the type information of the stored value.
   *
   * @return The type information of the stored value.
   */
  [[nodiscard]] inline const TypeInfo* get_type_info() const;

  /**
   * @brief Retrieves the stored value as the specified type.
   *
   * This function attempts to retrieve the stored value within the DynamicObject
   * as the specified type. It is the caller's responsibility to ensure that the
   * requested type matches the actual stored type, otherwise the behavior is undefined.
   *
   * @tparam TType The type to retrieve the stored value as.
   * @return The stored value as the specified type.
   */
  template<typename TType>
  [[nodiscard]] const TType get_as() const;

  /**
   * @brief Retrieves the stored value as the specified type.
   *
   * This function attempts to retrieve the stored value within the DynamicObject
   * as the specified type. It is the caller's responsibility to ensure that the
   * requested type matches the actual stored type, otherwise the behavior is undefined.
   *
   * @tparam TType The type to retrieve the stored value as.
   * @return The stored value as the specified type.
   */
  template<typename TType>
  [[nodiscard]] TType get_as();

private:

  /**
   * @brief The variant that stores the value associated with the DynamicObject.
   *
   * This is the variant that stores the value associated with the DynamicObject.
   */
  core::UntypedVariant<> mUntypedVariant;

  /**
   * @brief The type information of the value associated with the DynamicObject.
   *
   * This is the type information of the value associated with the DynamicObject.
   */
  const TypeInfo* mTypeInfo;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/tools/dynamic_object.inl"