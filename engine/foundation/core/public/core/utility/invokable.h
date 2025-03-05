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
#include "core/rtti/func/func_base.h"
#include "core/rtti/tools/tuple.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Invokable
 *
 * @brief
 * A template base class designed to define callable objects. It provides an
 * interface for invoking a function-like behavior with specified arguments
 * and a return type.
 */
template<typename TReturn, typename... TArgs>
class Invokable : public FuncBase {
public:

  typedef Tuple<TArgs...> TupleType;

public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes an instance of the Invokable class.
   */
  Invokable() = default;

  /**
   * @brief
   * Virtual destructor.
   *
   * Ensures proper cleanup for derived classes.
   */
  virtual ~Invokable() = default;

  /**
   * @brief
   * Callable operator.
   *
   * Enables the object to be invoked like a function. Internally calls
   * the `invoke` method.
   *
   * @param args... Variadic arguments passed to the `invoke` method.
   * @return The result of the `invoke` method.
   */
  TReturn operator()(TArgs... args) const {
    return invoke(args...);
  }
  TReturn operator()(const TupleType &cTuple) const {
    return tuple_invoke(cTuple);
  }
  TReturn tuple_invoke(const TupleType &cTuple) const {
    return TupleInvokeHelper(cTuple, MakeIndexSequence<sizeof...(TArgs)>());
  }

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
  virtual TReturn invoke(TArgs... args) const = 0;

public:

  /**
   * @brief
   * Invokes the function with the given arguments.
   *
   * This function invokes the function with the given arguments and returns the result.
   *
   * @param[in] args A vector of DynamicObjects representing the arguments to
   * invoke the function with.
   *
   * @return A DynamicObject containing the result of the function invocation.
   */
  DynamicObject dyn_invoke(Vector<DynamicObject>* args) override {
    // Initialize a tuple of the correct type from the dynamic argument list
    TupleType t;
    if (args)
      TupleFromDynamicObject(args, t);

    // Invoke the function using the tuple
    DynamicObject ret;
    DynInvokeHelper<TupleType, TReturn>::call(ret, this, t);
    return ret;
  }

private:
  /**
  *  @brief
  *    Tuple invoke helper
  */
  template <int... I>
  TReturn TupleInvokeHelper(const TupleType &cTuple, IndexSequence<I...>) const
  {
    return invoke(TupleGet<I>(cTuple)...);
  }

private:

  template <class TTuple, typename TRet2>
    struct DynInvokeHelper {

    static void call(DynamicObject &cRet, const Invokable *pToInvoke, const TTuple &tuple) {
      cRet.set<TRet2>(pToInvoke->tuple_invoke(tuple));
    }
  };

  template <class TTuple>
  struct DynInvokeHelper<TTuple, void> {

    static void call(DynamicObject &cRet, const Invokable *pToInvoke, const TTuple &tuple) {
      pToInvoke->tuple_invoke(tuple);
    }
  };
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/utility/invokable.inl"