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
#include "core/container/hash_map.h"
#include "core/container/vector.h"
#include "core/rtti/rtti_member.h"
#include "core/rtti/func/class_constructor.h"
#include "core/rtti/func/class_method.h"
#include "core/rtti/func/class_field.h"
#include "core/rtti/func/class_property.h"


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
template<typename TClass>
class ClassBuilder;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class Class : public RttiMember {

  template<typename TClass> friend class ClassBuilder;

public:

  /**
   * @brief
   * A static method to create a ClassBuilder instance and start declaring a class.
   *
   * @param name The name of the class to be declared.
   *
   * @return A ClassBuilder instance for the class with the given name.
   */
  template<typename TClass>
  static ClassBuilder<TClass> declare(const String& name);

public:

  /**
   * @brief
   * Default constructor for the Class class.
   *
   * @param name The name of the class.
   */
  Class(const String& name);

  /**
   * @brief
   * The destructor for `Class`.
   *
   * Cleans up any resources allocated by the `Class` instance.
   */
  ~Class() override;


  /**
   * @brief Get the name of the class.
   *
   * @return The name of the class as a constant reference to a String.
   */
  [[nodiscard]] const String& get_name() const;

  /**
   * @brief Get all base classes of this class.
   *
   * @return A constant reference to a vector of pointers to base classes.
   */
  [[nodiscard]] const Vector<const Class*>& get_base_classes() const;

  /**
   * @brief Check if this class is derived from a specified base class.
   *
   * @param baseClass Pointer to the base class to check against.
   * @return True if this class is derived from the specified base class, false otherwise.
   */
  [[nodiscard]] bool is_derived_from(const Class* baseClass) const;

  /**
   * @brief Get the default constructor of the class.
   *
   * @return A pointer to the default constructor, or nullptr if none exists.
   */
  [[nodiscard]] const ClassConstructor* get_default_constructor() const;

  /**
   * @brief Get all constructors of the class.
   *
   * @return A constant reference to a vector of class constructors.
   */
  [[nodiscard]] const Vector<ClassConstructor>& get_constructors() const;

  /**
   * @brief Check if the class has a default constructor.
   *
   * @return True if a default constructor exists, false otherwise.
   */
  [[nodiscard]] bool has_default_constructor() const;

  [[nodiscard]] uint32 get_num_constructors() const;

  [[nodiscard]] uint32 get_num_methods() const;

  [[nodiscard]] uint32 get_num_fields() const;

  [[nodiscard]] uint32 get_num_properties() const;

  /**
   * @brief Get a method by name.
   *
   * @param name The name of the method to retrieve.
   *
   * @return A pointer to the method, or nullptr if the method does not exist.
   */
  [[nodiscard]] const ClassMethod* get_method(const String& name) const;

  [[nodiscard]] const ClassField* get_field(const String& name) const;

  [[nodiscard]] const ClassProperty* get_property(const String& name) const;

private:
  /** The name of the class */
  String mName;

  /** A list of all the base classes of this class. */
  Vector<const Class*> mBaseClasses;

  /** The default constructor for the class */
  ClassConstructor mDefaultConstructor;

  /** The list of constructors for the class */
  Vector<ClassConstructor> mConstructors;

  core::hash_map<String, ClassMethod> mMethodsMap;

  core::hash_map<String, ClassField> mFieldsMap;

  core::hash_map<String, ClassProperty> mPropertiesMap;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/class.inl"