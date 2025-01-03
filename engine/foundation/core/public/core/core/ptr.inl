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
#include "core/core/default_assert.h"
#include "core/std/typetraits/is_base_of.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


template<class TType>
Ptr<TType>::Ptr() : mPtr(nullptr) {
  // empty
}

template<class TType>
Ptr<TType>::Ptr(TType *p) : mPtr(p) {
  // static_assert(std::is_base_of<Core::RefCounted, TType>::value, "Ptr only works on RefCounted types");
  if (nullptr != mPtr) {
    mPtr->add_ref();
  }
}

template<class TType>
Ptr<TType>::Ptr(const Ptr<TType> &p) : mPtr(p.mPtr) {
  if (nullptr != mPtr) {
    mPtr->add_ref();
  }
}

template<class TType>
Ptr<TType>::Ptr(Ptr<TType> &&p) noexcept : mPtr(p.mPtr) {
  p.mPtr = nullptr;
}

template<class TType>
template<class TOtherType>
Ptr<TType>::Ptr(TOtherType* rhs) : mPtr(nullptr)
{
  mPtr = nullptr;
  static_assert(core::is_base_of<TType, TOtherType>::value, "Implicit cast assumes left hand side must be base of right");
  TType* p = reinterpret_cast<TType*>(rhs);
  if (p != mPtr)
  {
    mPtr = p;
    if (mPtr)
      mPtr->add_ref();
  }
}

template<class TType>
template <class TOtherType>
Ptr<TType>::Ptr(const Ptr<TOtherType>& rhs) : mPtr(nullptr)
{
  mPtr = nullptr;
  static_assert(core::is_base_of<TType, TOtherType>::value, "Implicit cast assumes left hand side must be base of right");
  TType* p = reinterpret_cast<TType*>(rhs.mPtr);
  if (p != mPtr)
  {
    mPtr = p;
    if (nullptr != mPtr)
      mPtr->add_ref();
  }
}

template<class TType>
template <class TOtherType>
Ptr<TType>::Ptr(Ptr<TOtherType>&& rhs)
{
  mPtr = nullptr;
  static_assert(core::is_base_of<TType, TOtherType>::value, "Implicit cast assumes left hand side must be base of right");
  TType* p = reinterpret_cast<TType*>(rhs.mPtr);
  mPtr = p;
  rhs.mPtr = nullptr;
}

template<class TType>
Ptr<TType>::~Ptr() {
  if (nullptr != mPtr) {
    mPtr->release();
    mPtr = nullptr;
  }
}


template<class TType>
template<class TOtherType>
Ptr<TType>& Ptr<TType>::operator=(TOtherType* rhs)
{
  static_assert(std::is_base_of<TType, TOtherType>::value, "Implicit assignment assumes left hand side must be base of right");
  TType* p = reinterpret_cast<TType*>(rhs);
  if (mPtr != p)
  {
    if (mPtr != nullptr)
      mPtr->release();
    mPtr = p;
    if (mPtr != nullptr) mPtr->add_ref();
  }

  return *this;
}

template<class TType>
template<class TOtherType>
Ptr<TType>& Ptr<TType>::operator=(const Ptr<TOtherType>& rhs)
{
  static_assert(std::is_base_of<TType, TOtherType>::value, "Implicit assignment assumes left hand side must be base of right");
  TType* p = reinterpret_cast<TType*>(rhs.mPtr);
  if (mPtr != p)
  {
    if (mPtr != nullptr)
      mPtr->release();
    mPtr = p;
    if (mPtr != nullptr) mPtr->add_ref();
  }

  return *this;
}

template<class TType>
template<class TOtherType>
Ptr<TType>& Ptr<TType>::operator=(Ptr<TOtherType>&& rhs)
{
  static_assert(core::is_base_of<TType, TOtherType>::value, "Implicit assignment assumes left hand side must be base of right");
  TType* p = reinterpret_cast<TType*>(rhs.mPtr);
  if (mPtr != p)
  {
    if (mPtr != nullptr)
      mPtr->release();
    mPtr = p;
    rhs.mPtr = nullptr;
  }

  return *this;
}

template<class TType>
Ptr<TType> &Ptr<TType>::operator=(TType *rhs) {
  if (mPtr != rhs) {
    if (mPtr != nullptr)
      mPtr->release();
    mPtr = rhs;
    if (mPtr != nullptr)
      mPtr->add_ref();
  }

  return *this;
}

template<class TType>
Ptr<TType> &Ptr<TType>::operator=(const Ptr<TType> &rhs) {
  if (mPtr != rhs.mPtr) {
    if (mPtr != nullptr)
      mPtr->release();
    mPtr = rhs.mPtr;
    if (mPtr != nullptr)
      mPtr->add_ref();
  }

  return *this;
}

template<class TType>
Ptr<TType> &Ptr<TType>::operator=(Ptr<TType> &&rhs) {
  if (mPtr != rhs.mPtr) {
    if (mPtr)
      mPtr->release();
    mPtr = rhs.mPtr;
    rhs.mPtr = nullptr;
  }

  return *this;
}

template<class TType>
bool Ptr<TType>::operator==(const Ptr<TType> &rhs) const {
  return (mPtr == rhs.mPtr);
}

template<class TType>
bool Ptr<TType>::operator!=(const Ptr<TType> &rhs) const {
  return (mPtr != rhs.mPtr);
}

template<class TType>
bool Ptr<TType>::operator==(const TType *rhs) const {
  return (mPtr == rhs);
}

template<class TType>
bool Ptr<TType>::operator!=(const TType *rhs) const {
  return (mPtr != rhs);
}

template<class TType>
TType *Ptr<TType>::operator->() const {
  RE_ASSERT(mPtr, "NULL pointer access in Ptr::operator->()!");
  return mPtr;
}

template<class TType>
TType &Ptr<TType>::operator*() const {
  RE_ASSERT(mPtr, "NULL pointer access in Ptr::operator*()!");
  return *mPtr;
}

template<class TType>
Ptr<TType>::operator TType *() const {
  RE_ASSERT(mPtr, "NULL pointer access in Ptr::operator TType*()!");
  return mPtr;
}

template<class TType>
template<class TDerived>
const Ptr<TDerived> &Ptr<TType>::downcast() const {
#if (NEBULA_DEBUG == 1)
  // if DERIVED is not a derived class of TType, compiler complains here
  // compile-time inheritance-test
  static_assert(std::is_base_of<TType, DERIVED>::value, "Incompatible types");
#endif

  return *reinterpret_cast<const Ptr<TDerived> *>(this);
}

template<class TType>
template<class TBase>
const Ptr<TBase> &Ptr<TType>::upcast() const {
#if (NEBULA_DEBUG == 1)
  // if BASE is not a base-class of TType, compiler complains here
  // compile-time inheritance-test
  static_assert(std::is_base_of<BASE, TType>::value, "Incompatible types");
#endif

  return *reinterpret_cast<const Ptr<TBase> *>(this);
}

template<class TType>
template<class TOtherType>
const Ptr<TOtherType> &Ptr<TType>::cast() const {
  // note: this is an unsafe cast
  return *reinterpret_cast<const Ptr<TOtherType> *>(this);
}

template<class TType>
bool Ptr<TType>::is_valid() const {
  return (nullptr != mPtr);
}

template<class TType>
TType *Ptr<TType>::get() const {
  RE_ASSERT(mPtr, "NULL pointer access in Ptr::get()!");
  return mPtr;
}

template<class TType>
TType *Ptr<TType>::get_unsafe() const {
  return mPtr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // namespace core
