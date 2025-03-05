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
 * ClassMethod
 *
 * @brief
 * Represents a method for a class.
 *
 * This class represents a method for a class. It is used by the RTTI system to
 * store metadata about a class method.
 */
class ClassMethod : public RttiMember {
public:

  /**
   * @brief Default constructor for the ClassMethod class.
   *
   * Initializes an instance of the ClassMethod class with a default name and a nullptr
   * as the function to be invoked.
   */
  ClassMethod();

  /**
   * @brief Constructor for the ClassMethod class.
   *
   * Initializes an instance of the ClassMethod class with a specified name and a
   * function to be invoked.
   *
   * @param name The name of the method.
   * @param func The function to be invoked.
   */
  ClassMethod(const String& name, FuncBase* func);

  /**
   * @brief Destructor for the ClassMethod class.
   *
   * Cleans up any resources associated with the ClassMethod instance.
   */
  ~ClassMethod() override;


  /**
   * @brief
   * Checks if the current ClassMethod instance is equal to another one.
   *
   * This method checks if the current ClassMethod instance is equal to
   * another ClassMethod instance. Two ClassMethod instances are considered
   * equal if they have the same name and the same function associated.
   *
   * @param[in] other The other ClassMethod instance to compare with.
   *
   * @return True if the current ClassMethod instance is equal to the other
   * one, false otherwise.
   */
  bool operator==(const ClassMethod& other) const;


  /**
   * @brief
   * Invokes the method with the specified arguments.
   *
   * This method calls the underlying function associated with this ClassMethod
   * instance, passing the provided arguments to it, and returns the result.
   *
   * @param[in] args A vector of DynamicObjects representing the arguments to
   * invoke the function with.
   *
   * @return A DynamicObject containing the result of the method invocation.
   */
  DynamicObject invoke(Vector<DynamicObject>* args) const;

  /**
   * @brief
   * Directly calls the method with the given arguments.
   *
   * This method directly calls the method with the given arguments. It is
   * a low-level call and does not perform any of the additional checks
   * that the `invoke` method performs.
   *
   * @param obj The object to call the method on.
   * @param args... The arguments to pass to the method.
   *
   * @return The return value of the method.
   */
  template<typename TReturn, typename TObject, typename... TArgs>
  TReturn call_direct(TObject* obj, TArgs... args) const;

private:

  /** Name of the method. */
  String mName;

  /** Pointer to the function to be invoked. */
  FuncBase* mFunc;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/func/class_method.inl"