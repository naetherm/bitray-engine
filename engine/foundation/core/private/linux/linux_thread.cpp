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
#include "core/linux/linux_thread.h"
#include "core/log/log.h"
#include "core/memory/memory.h"
#include "core/platform/mutex.h"
#include "core/threading/thread.h"
#include "core/threading/types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
LinuxThread::LinuxThread(Thread &thread, bool useThreadId, handle id)
: ThreadImpl(thread)
, mThreadId(0)
, mMutex(nullptr)
, mPriorityClass(ThreadPriorityClass::TPC_Normal)
, mPriority(ThreadPriority::TP_Normal) {
  if (useThreadId && id == NULL_HANDLE) {
    mThreadId = pthread_self();
  } else {
    mMutex = re_new<Mutex>();
  }
}

LinuxThread::~LinuxThread() {
  if (mMutex) {
    re_delete(mMutex);
  }
}

handle LinuxThread::get_id() const {
  return (handle)mThreadId;
}

bool LinuxThread::is_active() const {
  return (mThreadId > 0);
}

bool LinuxThread::start() {
  if (!mThreadId) {
    const int status = pthread_create(&mThreadId, nullptr, &run_thread, static_cast<void*>(&get_thread()));

    if (!status) {
      return true;
    }
  }
  return false;
}

bool LinuxThread::kill() {
  if (mThreadId) {
    pthread_cancel(mThreadId);

    mThreadId = 0;

    if (mMutex) {
      mMutex->unlock();
    }

    return true;
  }
  return false;
}

bool LinuxThread::join() {
  if (mThreadId) {
    if (!pthread_join(mThreadId, nullptr)) {
      mThreadId = 0;

      return true;
    }
  }
  return false;
}

bool LinuxThread::join(uint64 timeout) {
  return join();
}

uint32 LinuxThread::get_priority_class() const {
  return mPriorityClass;
}

bool LinuxThread::set_priority_class(uint32 priorityClass) {
  mPriorityClass = priorityClass;
  return true;
}

uint32 LinuxThread::get_priority() const {
  return mPriority;
}

bool LinuxThread::set_priority(uint32 priority) {
  mPriority = priority;

  return true;
}

void *LinuxThread::run_thread(void *parameter) {
  if (parameter) {
    Thread* thread = static_cast<Thread*>(parameter);

    // Lock the thread
    Mutex* mutex = static_cast<LinuxThread*>(thread->mImpl)->mMutex;
    mutex->lock();

    // run thread
    int returnValue = thread->run();

    mutex->unlock();

    return reinterpret_cast<void*>(&returnValue);
  }
  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core