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
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ThreadImpl;
class Runnable;


typedef int (*THREADFUNCTION)(void*);


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Thread class
 *
 * @note
 * - Implementation of the bridge design pattern, this class is the abstraction
 */
class Thread {

  friend class LinuxThread;

public:
  /**
   * @brief
   * Constructor.
   */
  Thread();

  /**
   * @brief
   * Constructor.
   *
   * @param[in] runnable
   * Pointer to the runnable that should be executed within this thread.
   * @param[in] name
   * The name of the thread.
   */
  Thread(Runnable* runnable, const String& name);

  /**
   * @brief
   * Constructor.
   *
   * @param[in] threadFunction
   * A static function pointer of the function to execute within this thread.
   * @param[in] data
   * Additional data for the runner.
   */
  Thread(THREADFUNCTION threadFunction, void* data);

  /**
   * @brief
   * Constructor.
   *
   * @param[in] threadId
   * The thread id to assign to this thread.
   */
  Thread(handle threadId);

  /**
   * @brief
   * Destructor.
   */
  ~Thread();


  /**
   * @brief
   * Returns the name of the thread
   *
   * @return
   * Thread name
   *
   * @remarks
   * The thread name is not used internally, it's for user usage only and especially
   * for debugging purposes quite useful. It's recommended to give threads unique
   * names. By default, no thread name is set.
   */
  const String& get_name() const;

  /**
   * @brief
   * Sets the name of the thread
   *
   * @param[in] name
   * Thread name
   *
   * @see
   * - get_name()
   */
  void set_name(const String& name);

  /**
   * @brief
   * Returns the unique system ID of the thread
   *
   * @return
   * Thread ID
   */
  [[nodiscard]] handle get_id() const;

  /**
   * @brief
   * Returns whether the thread is active
   *
   * @return
   * 'true' if the thread is active, else 'false'
   */
  [[nodiscard]] bool is_active() const;


  /**
   * @brief
   * Returns whether the thread is active
   *
   * @return
   * 'true' if the thread is active
   */
  int32 run();

  /**
   * @brief
   * Starts the execution of the thread
   *
   * @return
   * 'true' if the thread could be started
   */
  bool start();

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
  void kill();

  /**
   * @brief
   * Waits until the thread has been stopped
   *
   * @return
   * 'true' if the thread has been stopped
   */
  bool join();

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
  bool join(uint64 timeout);

  /**
   * @brief
   * Returns the priority class the thread is in
   *
   * @return
   * The priority class the thread is in (type: ThreadPriorityClass)
   */
  [[nodiscard]] ThreadPriorityClass get_priority_class() const;

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
  bool set_priority_class(ThreadPriorityClass priorityClass = ThreadPriorityClass::TPC_Normal);

  /**
   * @brief
   * Returns the thread priority within the priority class it is in
   *
   * @return
   * The thread priority within the priority class it is in (type: ThreadPriority)
   */
  [[nodiscard]] ThreadPriority get_priority() const;

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
  bool set_priority(ThreadPriority priority = ThreadPriority::TP_Normal);

protected:

  Thread(const Thread& rhs) = delete;

  Thread& operator=(const Thread& rhs) = delete;

private:
  /** Pointer to internally platform dependent implementation */
  ThreadImpl* mImpl;
  /** Pointer to the runnable that should be executed */
  Runnable* mRunnable;
  /** Pointer to the static function to be executed */
  THREADFUNCTION mStaticFunction;
  /** Raw pointer to additional data */
  void* mStaticData;
  /** The name of the string */
  String mName;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core