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
#include "core/threading/unique_lock.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Mutex;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * ConditionVariableImpl
 *
 * @brief
 * Condition variables are synchronization primitives that enable threads to wait until a particular condition occurs.
 * Condition variables enable threads to atomically release a lock and enter the sleeping state.
 */
class ConditionVariableImpl {
//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   */
  ConditionVariableImpl() = default;

  /**
   * @brief
   * Destructor
   */
  virtual ~ConditionVariableImpl() = default;


  /**
   * @brief
   * Blocks the current thread execution until the condition variable is woken up.
   * @param [in]cMutex
   * The mutex section locked by the current thread.
   */
  virtual void wait(const Mutex& cMutex) = 0;

  virtual void wait(UniqueLock<Mutex>& mutex) = 0;

  /**
   * @brief
   * Blocks the current thread execution until the condition variable is woken up or after the specified timeout duration.
   *
   * @param [in]cMutex
   * The mutex section locked by the current thread.
   * @param [in]nTime
   * The time-out interval, in milliseconds. If the time-out interval elapses, the function re-acquires the critical
   * section and returns zero. If timeout is zero, the function tests the states of the specified objects and returns
   * immediately. If timeout is INFINITE, the function's time-out interval never elapses.
   */
  virtual void wait(const Mutex& cMutex, uint64 nTime) = 0;

  /**
   * @brief
   * Notifies one waiting thread.
   */
  virtual void wake_one() = 0;

  /**
   * @brief
   * Notifies all waiting threads.
   */
  virtual void wake_all() = 0;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core