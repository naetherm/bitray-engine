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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * UniqueLock
 *
 * @tparam TMutex
 * The mutex object to take care about.
 *
 * @brief
 * Basic and simple unique lock implementation.
 */
template<typename TMutex>
class UniqueLock {

  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:

  /**
   * @brief
   * Constructor.
   */
  UniqueLock();

  /**
   * @brief
   * Constructor.
   *
   * @param[in] mutex
   * The mutex to lock.
   */
  UniqueLock(TMutex& mutex);

  /**
   * @brief
   * Move constructor.
   *
   * @param[in] uniqueLock
   * The other unique lock to swap with.
   */
  UniqueLock(UniqueLock&& uniqueLock);

  /**
   * @brief
   * Destructor.
   */
  ~UniqueLock();


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * The other unique lock to swap.
   *
   * @return
   * Reference to this.
   */
  UniqueLock& operator=(UniqueLock&& rhs);

  /**
   * @brief
   * bool operator to check whether the mutex is owned.
   * @return
   */
  operator bool() const;


  /**
   * @brief
   * Returns whether the mutex is owned by this lock.
   *
   * @return
   * 'true' if mutex is owned by this unique lock, 'false' otherwise.
   */
  bool owns_lock() const;


  /**
   * @brief
   * Returns pointer to the mutex of this unique lock
   *
   * @return
   * Pointer to the mutex owned by this lock.
   */
  TMutex* mutex() const;


  /**
   * @brief
   * Locks the mutex and returns whether that was successful.
   *
   * @return
   * 'true' if locking was successful, false otherwise.
   */
  bool lock();

  /**
   * @brief
   * Try locking the mutex and returns whether that was successful.
   *
   * @return
   * 'true' if locking was successful, false otherwise.
   */
  bool try_lock();

  /**
   * @brief
   * Unlocks the mutex.
   *
   * @return
   * 'true' if unlocking was successful, false otherwise.
   */
  bool unlock();

  /**
   * @brief
   * Unlocks the mutex and returns it.
   *
   * @return
   * Pointer to mutex.
   */
  TMutex* release();

  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** Pointer to mutex */
  TMutex* mMutex;
  /** Flag specifying whether this mutex is currently owned by this unique lock */
  bool mOwnsMutex;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/threading/unique_lock.inl"