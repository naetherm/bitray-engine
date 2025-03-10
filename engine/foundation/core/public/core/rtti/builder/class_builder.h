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
#include <core/rtti/func/func_base.h>

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
class Class;
class DefaultValue;
class TypeInfo;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * ClassBuilder
 *
 * @brief
 * A helper class for registering members of a class in the RTTI system.
 *
 * The ClassBuilder class is a helper class for registering members of a class in the
 * RTTI system. It is used to register constructors, methods, fields, and properties of a
 * class.
 *
 * @tparam TClass The type of the class that the ClassBuilder is being used to register.
 */
template<typename TClass>
class ClassBuilder {

  friend class RttiMember;

public:

  /**
   * @brief
   * Constructs an ClassBuilder object that is used to register the values of a
   * Class.
   *
   * @param c The Class instance that the ClassBuilder will be used to register
   * constructors, methods, fields, and properties.
   */
  explicit ClassBuilder(Class& c);


  /**
   * @brief
   * Specify a base class for the class being registered.
   *
   * @param name The name of the base class.
   *
   * @return The ClassBuilder instance.
   */
  ClassBuilder& base(const String& name);

  /**
   * @brief
   * Register a constructor for the class being registered.
   *
   * @param func The constructor to be registered.
   *
   * @return The ClassBuilder instance.
   */
  ClassBuilder& constructor(FuncBase* func);

  /**
   * @brief
   * Register a method for the class being registered.
   *
   * @param name The name of the method.
   * @param func The method to be registered.
   *
   * @return The ClassBuilder instance.
   */
  ClassBuilder& method(const String& name, FuncBase* func);

  /**
   * @brief
   * Register a field for the class being registered.
   *
   * @param name The name of the field.
   * @param offset The offset of the field from the start of the object.
   *
   * @return The ClassBuilder instance.
   */
  ClassBuilder& field(const String& name, core::uint32 offset);

  /**
   * @brief
   * Register a property for the class being registered.
   *
   * @param name The name of the property.
   * @param setter The setter function for the property.
   * @param getter The getter function for the property.
   *
   * @return The ClassBuilder instance.
   */
  ClassBuilder& property(const String& name, FuncBase* setter, FuncBase* getter);


  /**
   * @brief
   * Add a tag to the last registered RTTI member.
   *
   * @param name The name of the tag.
   * @param value The value associated with the tag.
   *
   * @return The ClassBuilder instance.
   */
  ClassBuilder& add_tag(const String& name, const DynamicObject& value);

private:

  /** Pointer to the class being registered. */
  Class* mClass;

  /** Pointer to the last registered RTTI member. */
  RttiMember* mLastRttiMember;

  /** Pointer to the last default value registered. */
  DefaultValue* mLastDefaultValue;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/builder/class_builder.inl"