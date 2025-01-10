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
#include "core/threading/thread.h"
#include "core/threading/runnable.h"
#if defined(LINUX)
#include "core/linux/linux_thread.h"
#endif


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
Thread::Thread()
: mImpl(nullptr)
, mRunnable(nullptr) {

}

Thread::Thread(Runnable *runnable, const String &name)
: mRunnable(runnable)
, mStaticFunction(nullptr)
, mStaticData(nullptr)
, mName(name) {
#if defined(LINUX)
  mImpl = re_new<LinuxThread>(*this, false, NULL_HANDLE);
#endif
}

Thread::Thread(THREADFUNCTION threadFunction, void* data)
: mRunnable(nullptr)
, mStaticFunction(threadFunction)
, mStaticData(data)
, mName() {
#if defined(LINUX)
  mImpl = re_new<LinuxThread>(*this, false, NULL_HANDLE);
#endif
}

Thread::Thread(handle threadId)
: mImpl(nullptr)
, mRunnable(nullptr) {
#if defined(LINUX)
  mImpl = re_new<LinuxThread>(*this, true, threadId);
#endif
}

Thread::~Thread() {
  re_delete(mImpl);
}

const String &Thread::get_name() const {
  return mName;
}

void Thread::set_name(const String &name) {
  mName = name;
}

handle Thread::get_id() const {
  return mImpl->get_id();
}

bool Thread::is_active() const {
  return mImpl->is_active();
}

int32 Thread::run() {
  if (mRunnable) {
    return mRunnable->run();
  } else if (mStaticFunction) {
    // Call the static function
    return mStaticFunction(mStaticData);
  }
  return 0;
}

bool Thread::start() {
  return mImpl->start();
}

void Thread::kill() {
  mImpl->kill();
}

bool Thread::join() {
  return mImpl->join();
}

bool Thread::join(uint64 timeout) {
  return mImpl->join(timeout);
}

ThreadPriorityClass Thread::get_priority_class() const {
  return static_cast<ThreadPriorityClass>(mImpl->get_priority_class());
}

bool Thread::set_priority_class(ThreadPriorityClass priorityClass) {
  return mImpl->set_priority_class(priorityClass);
}

ThreadPriority Thread::get_priority() const {
  return static_cast<ThreadPriority>(mImpl->get_priority());
}

bool Thread::set_priority(ThreadPriority priority) {
  return mImpl->set_priority(priority);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core