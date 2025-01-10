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
#include "core/threading/types.h"
#include "core/std/typetraits/typetraits.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Thread;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * ThreadImpl
 *
 * @brief
 * Abstract implementation for platform specific thread implementations.
 */
class ThreadImpl {

  friend class Thread;

  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] thread
   * Parent thread instance.
   */
  ThreadImpl(Thread &thread);

  /**
   * @brief
   * Destructor.
   */
  virtual ~ThreadImpl();


  /**
   * @brief
   * Returns reference to the owning thread implementation.
   *
   * @return
   * Reference to the owning thread implementation.
   */
  Thread &get_thread() const;

  //[-------------------------------------------------------]
  //[ Protected virtual ThreadImpl functions                ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Returns the unique system ID of the thread
   *
   * @return
   * Thread ID
   */
  virtual handle get_id() const = 0;

  /**
   * @brief
   * Returns whether the thread is active
   *
   * @return
   * 'true' if the thread is active
   */
  virtual bool is_active() const = 0;

  /**
   * @brief
   * Starts the execution of the thread
   *
   * @return
   * 'true' if the thread could be started
   */
  virtual bool start() = 0;

  /**
   * @brief
   * Stops the execution of the thread
   *
   * @return
   * 'true' if the thread could be stopped
   *
   * @remarks
   * Terminates the thread ungracefully (does not allow proper thread clean up!). Instead of
   * using this function you should signal the thread and wait until it has quit by itself.
   * The internal platform implementation may or may not accept this violent act. For example,
   * Androids Bionic doesn't support it and a call of this method will have no effect at all.
   */
  virtual bool kill() = 0;

  /**
   * @brief
   * Waits until the thread has been stopped
   *
   * @return
   * 'true' if the thread has been stopped
   */
  virtual bool join() = 0;

  /**
   * @brief
   * Waits until the thread has been stopped
   *
   * @param[in] timeout
   * Time in milliseconds to wait
   *
   * @return
   * 'true' if the thread has been stopped
   */
  virtual bool join(uint64 timeout) = 0;

  /**
   * @brief
   * Returns the priority class the thread is in
   *
   * @return
   * The priority class the thread is in (type: ThreadPriorityClass)
   */
  virtual uint32 get_priority_class() const = 0;

  /**
   * @brief
   * Sets the priority class the thread is in
   *
   * @param[in] priorityClass
   * New priority class the thread is in (type: ThreadPriorityClass)
   *
   * @return
   * 'true' if all went fine, else 'false'
   */
  virtual bool set_priority_class(uint32 priorityClass) = 0;

  /**
   * @brief
   * Returns the thread priority within the priority class it is in
   *
   * @return
   * The thread priority within the priority class it is in (type: ThreadPriority)
   */
  virtual uint32 get_priority() const = 0;

  /**
   * @brief
   * Sets the thread priority within the priority class it is in
   *
   * @param[in] priority
   * New thread priority within the priority class it is in (type: ThreadPriority)
   *
   * @return
   * 'true' if all went fine, else 'false'
   */
  virtual bool set_priority(uint32 priority) = 0;

private:

  /** Pointer to parent instance */
  Thread *mThread;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core