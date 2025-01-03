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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class RefCounted;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<class TType>
class Ptr {

  template<class TOtherType> friend class Ptr;

public:

  Ptr();

  Ptr(TType* ptr);

  Ptr(const Ptr<TType>& ptr);

  Ptr(Ptr<TType>&& ptr) noexcept;

  template<class TOtherType>
  Ptr(TOtherType* ptr);

  template<class TOtherType>
  Ptr(const Ptr<TOtherType>& ptr);

  template<class TOtherType>
  Ptr(Ptr<TOtherType>&& ptr);

  ~Ptr();


  Ptr& operator=(TType* ptr);

  Ptr<TType>& operator=(const Ptr<TType>& ptr);

  Ptr<TType>& operator=(Ptr<TType>&& ptr);

  template<class TOtherType>
  Ptr& operator=(TOtherType* ptr);

  template<class TOtherType>
  Ptr& operator=(const Ptr<TOtherType>& ptr);

  template<class TOtherType>
  Ptr& operator=(Ptr<TOtherType>&& ptr);

  bool operator==(const Ptr<TType>& ptr) const;

  bool operator!=(const Ptr<TType>& ptr) const;

  bool operator==(const TType* ptr) const;

  bool operator!=(const TType* ptr) const;

  TType* operator->() const;

  TType& operator*() const;

  operator TType*() const;


  template<class TDerived>
  const Ptr<TDerived>& downcast() const;

  template<class TBase>
  const Ptr<TBase>& upcast() const;

  template<class TOtherType>
  const Ptr<TOtherType>& cast() const;

  bool is_valid() const;

  TType* get() const;

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
#include "core/core/ptr.inl"