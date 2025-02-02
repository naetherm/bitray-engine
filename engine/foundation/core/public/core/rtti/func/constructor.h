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
#include "core/utility/invokable.h"
#include "core/rtti/func/function_signature.h"


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
 * Constructor
 *
 * @brief
 * A template class implementing a constructor invoker. It provides an interface
 * for invoking a constructor with specified arguments and a return type.
 *
 * @param TClass The type of the class to be constructed.
 * @param TArgs... The types of arguments to be passed to the constructor.
 */
template<class TClass, typename... TArgs>
class Constructor : public Invokable<TClass*, TArgs...> {
public:

  /**
   * @brief
   * Pure virtual function to be implemented by derived classes.
   *
   * Defines the core functionality of the Invokable object. Derived classes
   * must implement this method to specify what happens when the object is
   * invoked.
   *
   * @param args... Variadic arguments required for the implementation.
   * @return The result of the implemented function.
   */
  TClass* invoke(TArgs... args) const override;

public:

  /**
   * @brief
   * Retrieves the function signature of the constructor.
   *
   * @return
   * A FunctionSignature object representing the types of arguments and the return type
   * of the constructor.
   */
  virtual FunctionSignature get_signature() const override;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/func/constructor.inl"