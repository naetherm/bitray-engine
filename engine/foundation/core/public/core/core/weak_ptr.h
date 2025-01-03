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
#include "core/core/ptr.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<class TType>
class WeakPtr {
public:
  /// constructor
  WeakPtr();
  /// construct from C++ pointer
  WeakPtr(TType* p);
  /// construct from Ptr<> pointer
  WeakPtr(const Ptr<TType>& p);
  /// construct from WeakPtr<> pointer
  WeakPtr(const WeakPtr<TType>& p);
  /// destructor
  ~WeakPtr();

  /// assignment operator from Ptr<>
  WeakPtr<TType>& operator=(const Ptr<TType>& rhs);
  /// assignment operator from WeakPtr<>
  WeakPtr<TType>& operator=(const WeakPtr<TType>& rhs);
  /// assignment operator
  WeakPtr<TType>& operator=(TType* rhs);
  /// safe -> operator
  TType* operator->() const;
  /// safe dereference operator
  TType& operator*() const;
  /// safe pointer cast operator
  operator TType*() const;
  /// check if pointer is valid
  bool is_valid() const;
  /// return direct pointer (asserts if null pointer)
  TType* get() const;
  /// return direct pointer (returns null pointer)
  TType* get_unsafe() const;

private:
  TType* mPtr;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/core/weak_ptr.inl"