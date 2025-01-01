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
//[ Public functions                                      ]
//[-------------------------------------------------------]
inline int32 AtomicIncrement(volatile int32 * p32)
{
#if defined(RE_COMPILER_GNUC)
int32 result;
      __asm__ __volatile__ ("lock; xaddl %0, %1"
      : "=r" (result), "=m" (*p32)
      : "0" (1), "m" (*p32)
      : "memory"
      );
      return result + 1;
#endif
  return ++*p32;
}

inline int32 AtomicDecrement(volatile int32 * p32)
{
#if defined(RE_COMPILER_GNUC)
int32 result;
      __asm__ __volatile__ ("lock; xaddl %0, %1"
      : "=r" (result), "=m" (*p32)
      : "0" (-1), "m" (*p32)
      : "memory"
      );
      return result - 1;
#endif
  return --*p32;
}

inline bool AtomicCompareAndSwap(int32 * p32, int32 newValue, int32 nCondition)
{
#if defined(RE_COMPILER_GNUC)
int32 result;
      __asm__ __volatile__(
      "lock; cmpxchgl %3, (%1) \n"                    // Test *p32 against EAX, if same, then *p32 = newValue
      : "=a" (result), "=r" (p32)                     // outputs
      : "a" (nCondition), "r" (newValue), "1" (p32)    // inputs
      : "memory"                                      // clobbered
      );
      return result == nCondition;
#endif
  if (*p32 == nCondition) {
    *p32 = newValue;
    return true;
  }
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
