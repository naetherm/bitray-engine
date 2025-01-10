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
#include "core/threading/thread_impl.h"


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
 * LinuxThread
 *
 * @brief
 * Linux implementation of a thread.
 */
class LinuxThread : public ThreadImpl {

  friend class Thread;

public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] thread
   * Parent thread instance.
   */
  LinuxThread(Thread& thread, bool useThreadId, handle id);

  /**
   * @brief
   * Destructor.
   */
  ~LinuxThread() override;

protected:
  /**
   * @brief
   * Returns the unique system ID of the thread
   *
   * @return
   * Thread ID
   */
  [[nodiscard]] handle get_id() const override;

  /**
   * @brief
   * Returns whether the thread is active
   *
   * @return
   * 'true' if the thread is active
   */
  [[nodiscard]] bool is_active() const override;

  /**
   * @brief
   * Starts the execution of the thread
   *
   * @return
   * 'true' if the thread could be started
   */
  bool start() override;

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
  bool kill() override;

  /**
   * @brief
   * Waits until the thread has been stopped
   *
   * @return
   * 'true' if the thread has been stopped
   */
  bool join() override;

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
  bool join(uint64 timeout) override;

  /**
   * @brief
   * Returns the priority class the thread is in
   *
   * @return
   * The priority class the thread is in (type: ThreadPriorityClass)
   */
  [[nodiscard]] uint32 get_priority_class() const override;

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
  bool set_priority_class(uint32 priorityClass) override;

  /**
   * @brief
   * Returns the thread priority within the priority class it is in
   *
   * @return
   * The thread priority within the priority class it is in (type: ThreadPriority)
   */
  [[nodiscard]] uint32 get_priority() const override;

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
  bool set_priority(uint32 priority) override;

private:

  static void* run_thread(void* parameter);

private:

  pthread_t mThreadId;
  Mutex* mMutex;
  uint32 mPriorityClass;
  uint32 mPriority;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core