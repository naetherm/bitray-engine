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
template<typename TType> class Func;

template<typename TReturn, class TClass, typename... TArgs>
class Func<TReturn (TClass::*)(TArgs...)> : public Invokable<TReturn, TClass*, TArgs...> {
public:

  typedef Func<TReturn (TClass::*)(TArgs...)> ThisType;
  typedef TReturn (TClass::*FUNC)(TArgs...);

public:

  Func(TReturn (TClass::*TMethod)(TArgs...));

  TReturn invoke(TClass* object, TArgs... args) const override;

  FunctionSignature get_signature() const override;

private:

  FUNC mFunc;
};

template<typename TReturn, class TClass, typename... TArgs>
class Func<TReturn (TClass::*)(TArgs...) const> : public Invokable<TReturn, const TClass*, TArgs...> {
public:

  typedef Func<TReturn (TClass::*)(TArgs...) const> ThisType;
  typedef TReturn (TClass::*FUNC)(TArgs...) const;

public:

  Func(TReturn (TClass::*TMethod)(TArgs...) const);

  TReturn invoke(const TClass* object, TArgs... args) const override;

  FunctionSignature get_signature() const override;

private:

  FUNC mFunc;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/func/func.inl"