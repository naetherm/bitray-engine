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
template<typename R, typename... Args>
class Func {
public:
  typedef Func<R, Args...> _MethodType;

  Func() = default;
  virtual ~Func() = default;

  virtual R operator()(Args... args) {
    return R();
  }

  virtual _MethodType* clone() const {
    return nullptr;
  }
};

// Specialization for void return type
template<typename... Args>
class Func<void, Args...> {
public:
  typedef Func<void, Args...> _MethodType;

  Func() = default;
  virtual ~Func() = default;

  virtual void operator()(Args... args) {
  }

  virtual _MethodType* clone() const {
    return nullptr;
  }
};

template<typename R>
class Func<R> {
public:
  typedef Func<R> _MethodType;

  Func() = default;
  virtual ~Func() = default;

  virtual R operator()() {
    return R();
  }

  virtual _MethodType* clone() const {
    return nullptr;
  }
};

// Specialization for void return type
template<>
class Func<void> {
public:
  typedef Func<void> _MethodType;

  Func() = default;
  virtual ~Func() = default;

  virtual void operator()() {
  }

  virtual _MethodType* clone() const {
    return nullptr;
  }
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}