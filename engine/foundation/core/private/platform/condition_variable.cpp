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
#include "core/platform/condition_variable.h"
#include "core/memory/memory.h"
#if defined(LINUX)
#include "core/linux/linux_condition_variable.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
ConditionVariable::ConditionVariable()
  : mConditionVariableImpl(nullptr) {
#if defined(LINUX)
  this->mConditionVariableImpl = new LinuxConditionVariable();
#elif defined(WIN32)
  this->mConditionVariableImpl = re_new<WindowsConditionVariable>();
#endif
}

ConditionVariable::~ConditionVariable() {
  delete this->mConditionVariableImpl;
}

void ConditionVariable::wait(Mutex &cMutex) {
  this->mConditionVariableImpl->wait(cMutex);
}

void ConditionVariable::wait(UniqueLock<Mutex> &cMutex) {
  this->mConditionVariableImpl->wait(cMutex);
}

void ConditionVariable::wait(Mutex &cMutex, uint64 nTime) {
  this->mConditionVariableImpl->wait(cMutex, nTime);
}

void ConditionVariable::wake_one() {
  this->mConditionVariableImpl->wake_one();
}

void ConditionVariable::wake_all() {
  this->mConditionVariableImpl->wake_all();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core