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
#include "core/rtti/func/func_base.h"
#include "core/container/vector.h"


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
 * ClassProperty
 *
 * @brief
 * Represents a property for a class.
 *
 * This class represents a property for a class. It is used by the RTTI system to
 * store metadata about a class property.
 */
class ClassProperty : public RttiMember {
public:

  /**
   * @brief
   * Default constructor.
   *
   * The default constructor constructs an invalid `ClassProperty` instance.
   */
  ClassProperty();

  /**
   * @brief
   * Constructor for the ClassProperty class.
   *
   * Initializes the ClassProperty instance with the specified name, setter and getter.
   *
   * @param[in] name The name of the property.
   * @param[in] setter The setter function for the property.
   * @param[in] getter The getter function for the property.
   */
  ClassProperty(const String& name, FuncBase* setter, FuncBase* getter);

  /**
   * @brief Destructor for the ClassProperty class.
   *
   * Cleans up any resources associated with the ClassProperty instance.
   */
  ~ClassProperty() override;


  /**
   * @brief
   * Compare the current ClassProperty object with another ClassProperty object for equality.
   *
   * @param[in] other
   * The ClassProperty object to compare with the current ClassProperty object.
   *
   * @return
   * If the current ClassProperty object is equal to the other ClassProperty object, return true, otherwise
   * return false.
   */
  bool operator==(const ClassProperty& other) const;


  /**
   * @brief
   * Calls the setter function associated with this ClassProperty using the supplied arguments.
   *
   * @param[in] args The arguments to pass to the setter function.
   *
   * This function is used to set the value of an instance of a class using the setter function associated with
   * this property.
   */
  void set(Vector<DynamicObject>* args) const;

  /**
   * @brief
   * Calls the getter function associated with this ClassProperty using the supplied arguments.
   *
   * @param[in] args The arguments to pass to the getter function.
   *
   * This function is used to get the value of an instance of a class using the getter function associated with
   * this property.
   *
   * @return
   * The value returned by the getter function.
   */
  DynamicObject get(Vector<DynamicObject>* args) const;

  /**
   * @brief
   * Sets the value of this ClassProperty directly on the given object.
   *
   * This function bypasses additional checks and directly sets the value
   * of the specified property on the provided object.
   *
   * @param obj The object on which to set the property value.
   * @param value The value to set for the property.
   */
  template<typename TType, class TObject>
  void set_direct(TObject* obj, TType value) const;

  /**
   * @brief
   * Gets the value of this ClassProperty directly from the given object.
   *
   * This function bypasses additional checks and directly gets the value
   * of the specified property from the provided object.
   *
   * @param obj The object from which to get the property value.
   *
   * @return
   * The value of the specified property.
   */
  template<typename TType, class TObject>
  TType get_direct(TObject* obj) const;

private:
  /** The name of the property */
  String mName;
  /** Pointer to the setter function */
  FuncBase* mSetter;
  /** Pointer to the getter function */
  FuncBase* mGetter;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/func/class_property.inl"