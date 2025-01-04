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
class MutexImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Mutex
 *
 * @brief
 * Platform independent mutex implementation.
 */
class Mutex {

//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:

  /**
   * @brief
   * Constructor.
   */
  Mutex();

  /**
   * @brief
   * Destructor.
   */
  ~Mutex();


  /**
   * @brief
   * Returns the internal implementation.
   *
   * @return
   * The internal implementation.
   */
  [[nodiscard]] MutexImpl* get_impl() const;


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

//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:

  MutexImpl* mMutexImpl;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core