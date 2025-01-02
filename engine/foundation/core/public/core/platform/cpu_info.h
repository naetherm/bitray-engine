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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @struct
 * CPUInfo
 *
 * @brief
 * Contains information about CPU (Central Processing Unit).
 */
struct CPUInfo {

  /**
   * The number of physical processor packages.
   */
  uint32 ProcessorPackageCount;

  /**
   * The number of processor cores (physical).
   */
  uint32 ProcessorCoreCount;

  /**
   * The number of logical processors (including hyper-threading).
   */
  uint32 LogicalProcessorCount;

  /**
   * The size of processor L1 caches (in bytes).
   */
  uint32 L1CacheSize;

  /**
   * The size of processor L2 caches (in bytes).
   */
  uint32 L2CacheSize;

  /**
   * The size of processor L3 caches (in bytes).
   */
  uint32 L3CacheSize;

  /**
   * The CPU memory page size (in bytes).
   */
  uint32 PageSize;

  /**
   * The CPU clock speed (in Hz).
   */
  uint64 ClockSpeed;

  /**
   * The CPU cache line size (in bytes).
   */
  uint32 CacheLineSize;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
