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
#include "core/rtti/tools/dynamic_object.h"
#include "core/rtti/func/function_signature.h"
#include "core/rtti/func/func_base.h"


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
 * ClassConstructor
 *
 * @brief
 * Represents a constructor for a class.
 *
 * This class represents a constructor for a class. It is used by the RTTI system to
 * store metadata about a class constructor.
 */
class ClassConstructor : public RttiMember {
public:

  /**
   * @brief
   * Default constructor for the ClassConstructor class.
   *
   * Initializes the ClassConstructor instance and sets up any necessary
   * resources or default values.
   */
  ClassConstructor();

  /**
   * @brief
   * Constructor for the ClassConstructor class, taking a raw pointer to a FuncBase.
   *
   * Initializes the ClassConstructor instance and sets up any necessary
   * resources or default values.
   *
   * @param func A raw pointer to a FuncBase object, which will be owned by this
   *             ClassConstructor.
   */
  ClassConstructor(FuncBase* func);

  /**
   * @brief
   * Destructor for the ClassConstructor class.
   *
   * Cleans up any resources or memory allocated by the ClassConstructor instance.
   */
  ~ClassConstructor() override;


  /**
   * @brief
   * Equality operator for ClassConstructor instances.
   *
   * Checks whether the given ClassConstructor instance is equal to this one.
   *
   * @param other The other ClassConstructor instance.
   *
   * @return true if the two instances are equal, false otherwise.
   */
  bool operator==(const ClassConstructor& other) const;


  /**
   * @brief
   * Invokes the constructor represented by this ClassConstructor, using the given
   * arguments.
   *
   * This function will invoke the constructor represented by this ClassConstructor,
   * using the given arguments. It will return a DynamicObject containing the result
   * of the constructor invocation.
   *
   * @param args A vector of DynamicObject instances, representing the arguments to
   *             pass to the constructor.
   *
   * @return A DynamicObject containing the result of the constructor invocation.
   */
  DynamicObject invoke(Vector<DynamicObject>* args) const;

  /**
   * @brief
   * Returns the function signature of the constructor represented by this
   * ClassConstructor.
   *
   * @return A reference to a FunctionSignature object that describes the type of
   *         the constructor represented by this ClassConstructor.
   */
  [[nodiscard]] const FunctionSignature& get_function_signature() const;

private:

  /**
   * @brief Pointer to the base function.
   *
   * This member stores a pointer to the base function and is used
   * to access function-specific operations.
   */
  FuncBase* mFunc;

  /**
   * @brief The function signature of this constructor.
   *
   * This member holds the function signature, which describes
   * the types of the constructor, including its parameter types
   * and return type.
   */
  FunctionSignature mFunctionSignature;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}