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
#include "core/linux/linux_condition_variable.h"
#include "core/linux/linux_mutex.h"
#include "core/platform/mutex.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxConditionVariable::LinuxConditionVariable() {
  this->mConditionVariable = PTHREAD_COND_INITIALIZER;
  pthread_cond_init(&this->mConditionVariable, nullptr);
}

LinuxConditionVariable::~LinuxConditionVariable() {
  pthread_cond_destroy(&this->mConditionVariable);
}

void LinuxConditionVariable::wait(const Mutex &cMutex) {
  pthread_mutex_t* pMutex = reinterpret_cast<LinuxMutex*>(cMutex.get_impl())->get_impl();

  pthread_cond_wait(&this->mConditionVariable, pMutex);
}

void LinuxConditionVariable::wait(UniqueLock<Mutex> &cMutex) {
  pthread_mutex_t* pMutex = reinterpret_cast<LinuxMutex*>(cMutex.mutex()->get_impl())->get_impl();

  pthread_cond_wait(&this->mConditionVariable, pMutex);
}

void LinuxConditionVariable::wait(const Mutex &cMutex, uint64 nTimeout) {
  pthread_mutex_t* pMutex = reinterpret_cast<LinuxMutex*>(cMutex.get_impl())->get_impl();
  if (nTimeout == -1) {
    pthread_cond_wait(&this->mConditionVariable, pMutex);
  } else {
    timespec ts;
    ts.tv_sec = nTimeout / 1000;
    ts.tv_nsec = (nTimeout % 1000) * 1000;
    pthread_cond_timedwait(&this->mConditionVariable, pMutex, &ts);
  }
}

void LinuxConditionVariable::wake_one() {
  pthread_cond_signal(&this->mConditionVariable);
}

void LinuxConditionVariable::wake_all() {
  pthread_cond_broadcast(&this->mConditionVariable);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core