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
//[ Includes                                              ]
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
template<typename TReturn, class TClass, typename ... TArgs>
Func<TReturn(TClass::*)(TArgs...)>::Func(TReturn(TClass::* TMethod)(TArgs...))
: mFunc(TMethod) {
}

template<typename TReturn, class TClass, typename ... TArgs>
TReturn Func<TReturn(TClass::*)(TArgs...)>::invoke(TClass* object, TArgs... args) const {
  return (object->*mFunc)(args...);
}

template<typename TReturn, class TClass, typename ... TArgs>
FunctionSignature Func<TReturn(TClass::*)(TArgs...)>::get_signature() const {
  return FunctionSignature::from_template<TReturn, TClass*, TArgs...>();
}


template<typename TReturn, class TClass, typename ... TArgs>
Func<TReturn(TClass::*)(TArgs...) const>::Func(TReturn(TClass::* TMethod)(TArgs...) const)
: mFunc(TMethod) {
}

template<typename TReturn, class TClass, typename ... TArgs>
TReturn Func<TReturn(TClass::*)(TArgs...) const>::invoke(const TClass* object, TArgs... args) const {
  return (object->*mFunc)(args...);
}

template<typename TReturn, class TClass, typename ... TArgs>
FunctionSignature Func<TReturn(TClass::*)(TArgs...) const>::get_signature() const {
  return FunctionSignature::from_template<TReturn, const TClass*, TArgs...>();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}