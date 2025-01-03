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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


template<class TType>
WeakPtr<TType>::WeakPtr() : mPtr(nullptr) {
  // empty
}

template<class TType>
WeakPtr<TType>::WeakPtr(TType *p) : mPtr(p) {
  // empty
}

template<class TType>
WeakPtr<TType>::WeakPtr(const Ptr<TType> &p) : mPtr(p.get_unsafe()) {
  // empty
}

template<class TType>
WeakPtr<TType>::WeakPtr(const WeakPtr<TType> &p) : mPtr(p.mPtr) {
  // empty
}

template<class TType>
WeakPtr<TType>::~WeakPtr() {
  mPtr = nullptr;
}

template<class TType>
WeakPtr<TType> &WeakPtr<TType>::operator=(const Ptr<TType> &rhs) {
  mPtr = rhs.get_unsafe();

  return *this;
}

template<class TType>
WeakPtr<TType> &WeakPtr<TType>::operator=(const WeakPtr<TType> &rhs) {
  mPtr = rhs.mPtr;

  return *this;
}

template<class TType>
WeakPtr<TType> &WeakPtr<TType>::operator=(TType *rhs) {
  mPtr = rhs;

  return *this;
}

template<class TType>
TType *WeakPtr<TType>::operator->() const {
  RE_ASSERT(mPtr, "NULL pointer access in WeakPtr::operator->()!");
  return mPtr;
}

template<class TType>
TType &WeakPtr<TType>::operator*() const {
  RE_ASSERT(mPtr, "NULL pointer access in WeakPtr::operator*()!");
  return *mPtr;
}

template<class TType>
WeakPtr<TType>::operator TType *() const {
  RE_ASSERT(mPtr, "NULL pointer access in WeakPtr::operator TType*()!");
  return mPtr;
}

template<class TType>
bool WeakPtr<TType>::is_valid() const {
  return (nullptr != mPtr);
}

template<class TType>
TType *WeakPtr<TType>::get() const {
  RE_ASSERT(mPtr, "NULL pointer access in WeakPtr::get()!");
  return mPtr;
}

template<class TType>
TType *WeakPtr<TType>::get_unsafe() const {
  return mPtr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // namespace core
