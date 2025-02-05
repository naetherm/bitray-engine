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
#include "core/std/function.h"
#include <functional>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Slot
 *
 * @tparam TArgs
 *
 * @brief
 * A slot is a wrapper around a class method.
 */
template<typename... TArgs>
class Slot : public Invokable<void, void*, TArgs...> {
public:
  /**
   * @brief
   * Constructor.
   */
  Slot();

  /**
   * @brief
   * Constructor.
   *
   * @param cls
   * @param METHOD
   */
  template<typename TClass>
  Slot(TClass* cls, void(TClass::*METHOD)(TArgs...));

  /**
   * @brief
   * Constructor.
   *
   * @param cls
   * @param METHOD
   */
  template<typename TClass>
  Slot(TClass* cls, void(TClass::*METHOD)(TArgs...) const);

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  Slot(const Slot& rhs);

  /**
   * @brief
   * Destructor.
   */
  ~Slot() override;


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * Object to copy.
   *
   * @return
   * Reference to this instance.
   */
  Slot& operator=(const Slot& rhs);

  bool operator==(const Slot& rhs) const;


  /**
   * @brief
   * Calls the internally saved class method.
   *
   * @param[in] args
   * The arguments for the invoking.
   */
  void call(TArgs... args);

public:
  /**
   * @brief
   * The invoking.
   *
   * @param[in] obj
   * Pointer to the object instance.
   * @param[in] args
   * The arguments to use for the invoke.
   */
  void invoke(void* obj, TArgs... args) const override;

public:

  /**
   * @brief
   * Returns the function signature of this function.
   *
   * The function signature describes the type of the function, including its return type
   * and its parameter types.
   *
   * @return A FunctionSignature object that describes the type of the function.
   */
  FunctionSignature get_signature() const override;

protected:
  /** Function wrapper */
  std::function<void(TArgs...)> mCallback;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/event/slot.inl"
