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


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
template<typename TMutex>
UniqueLock<TMutex>::UniqueLock()
: mMutex(nullptr)
, mOwnsMutex(false) {

}

template<typename TMutex>
UniqueLock<TMutex>::UniqueLock(TMutex &mutex)
: mMutex(&mutex)
, mOwnsMutex(false) {
  mMutex->lock();
  mOwnsMutex = true;
}

template<typename TMutex>
UniqueLock<TMutex>::UniqueLock(UniqueLock &&uniqueLock)
: mMutex(uniqueLock.mMutex)
, mOwnsMutex(uniqueLock.mOwnsMutex) {
  uniqueLock.mMutex = nullptr;
  uniqueLock.mOwnsMutex = false;
}

template<typename TMutex>
UniqueLock<TMutex>::~UniqueLock() {
  if (mOwnsMutex) {
    mMutex->unlock();
  }
}

template<typename TMutex>
UniqueLock<TMutex> &UniqueLock<TMutex>::operator=(UniqueLock &&rhs) {
  if (mOwnsMutex) {
    mMutex->unlock();
  }

  mMutex = rhs.mMutex;
  mOwnsMutex = rhs.mOwnsMutex;

  rhs.mMutex = nullptr;
  rhs.mOwnsMutex = false;

  return *this;
}

template<typename TMutex>
UniqueLock<TMutex>::operator bool() const {
  return mOwnsMutex;
}

template<typename TMutex>
bool UniqueLock<TMutex>::owns_lock() const {
  return mOwnsMutex;
}

template<typename TMutex>
TMutex* UniqueLock<TMutex>::mutex() const {
  return mMutex;
}

template<typename TMutex>
bool UniqueLock<TMutex>::lock() {
  mOwnsMutex = mMutex->lock();
  return mOwnsMutex;
}

template<typename TMutex>
bool UniqueLock<TMutex>::try_lock() {
  mOwnsMutex = mMutex->try_lock();
  return mOwnsMutex;
}

template<typename TMutex>
bool UniqueLock<TMutex>::unlock() {
  mMutex->unlock();
  mOwnsMutex = false;
  return true;
}

template<typename TMutex>
TMutex *UniqueLock<TMutex>::release() {
  TMutex* mutex = mMutex;

  mMutex = nullptr;
  mOwnsMutex = false;

  return mMutex;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core