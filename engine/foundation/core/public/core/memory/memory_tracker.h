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
#include "core/std/atomic.h"
#include "core/platform/mutex.h"
#include "core/container/hash_map.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


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
/**
 * @class
 * MemoryTracker
 *
 * @brief
 * MemoryTracker is a utility class for tracking memory allocations and deallocations.
 *
 * By default all allocations are tracked, but this can be disabled by calling
 * enable(false).
 *
 * The MemoryTracker can be used to detect memory leaks, in which case it will
 * print out a log message when the check_leaks() method returns false.
 *
 * MemoryTracker is a singleton class, so you can use the instance() method to
 * access it.
 *
 */
class MemoryTracker {
public:

  /**
   * @brief
   * Retrieves the instance of the MemoryTracker.
   *
   * @return
   * The MemoryTracker instance.
   */
  static MemoryTracker& instance();

public:

  /**
   * @brief
   * Creates a MemoryTracker instance.
   */
  MemoryTracker();

  /**
   * @brief
   * Destroys the MemoryTracker instance.
   */
  ~MemoryTracker();


  /**
   * @brief
   * Retrieves the total allocated memory.
   *
   * @return
   * The total allocated memory.
   */
  sizeT get_total_allocated() const;

  /**
   * @brief
   * Retrieves the current allocated memory.
   *
   * @return
   * The current allocated memory.
   */
  sizeT get_current_allocated() const;

  /**
   * @brief
   * Retrieves the number of memory allocations.
   *
   * @return
   * The number of memory allocations.
   */
  sizeT get_num_allocations() const;

  /**
   * @brief
   * Retrieves the number of memory deallocations.
   *
   * @return
   * The number of memory deallocations.
   */
  sizeT get_num_deallocations() const;


  /**
   * @brief
   * Enables or disables memory tracking.
   *
   * @param enable
   * A boolean indicating whether to enable (true) or disable (false) memory tracking.
   */
  void enable(bool enable=true);

  /**
   * @brief
   * Tracks a memory allocation.
   *
   * @param ptr
   * Pointer to the allocated memory.
   *
   * @param size
   * Size of the allocated memory.
   *
   * @param file
   * Name of the file where the allocation occurred.
   *
   * @param line
   * Line number in the file where the allocation occurred.
   *
   * @param func
   * Name of the function where the allocation occurred.
   *
   * @param allocatorName
   * Name of the allocator used for the allocation.
   */
  void track_alloc(void* ptr, sizeT size, const char* file, int line, const char* func, const String& allocatorName);

  /**
   * @brief
   * Tracks a memory deallocation.
   *
   * @param ptr
   * Pointer to the memory being deallocated.
   */
  void track_dealloc(void* ptr);

  /**
   * @brief
   * Checks if there are any memory leaks.
   *
   * If there are any memory leaks the function will print out a log message.
   *
   * @return
   * True if there are no leaks, otherwise false.
   */
  [[nodiscard]] bool check_leaks() const;

  /**
   * @brief
   * Prints the memory allocation and deallocation statistics.
   *
   * This function outputs the current memory usage statistics,
   * including the total number of allocations and deallocations.
   */
  void print_stats() const;

  void leak_report() const;

  /**
   * @brief
   * Resets the memory tracker.
   *
   * This function resets all memory tracking counters to zero.
   */
  void reset();

private:

  /**
   * @struct
   * AllocationInfo
   *
   * @brief
   * Holds information about a memory allocation.
   *
   * This structure stores details about a specific memory allocation,
   * including its size, source file, line number, and the associated function.
   */
  struct AllocationInfo {
    sizeT size;                 ///< Size of the allocated memory.
    const char* file;            ///< File where the allocation was made.
    int line;                    ///< Line number of the allocation.
    const char* function;        ///< Function name where the allocation occurred.
    String allocator_name; ///< Name of the allocator used.
  };

private:

  Mutex mMutex;
  hash_map<void*, AllocationInfo> mAllocations;
  atomic<sizeT> mTotalAllocatedBytes;
  atomic<sizeT> mCurrentAllocatedBytes;
  atomic<sizeT> mNumAllocations;
  atomic<sizeT> mNumDeallocations;
  bool mEnabled;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}

#if defined(DEBUG)
#define BE_TRACK_ALLOC(ptr, size, allocator_name) \
core::MemoryTracker::instance().track_alloc(ptr, size, __FILE__, __LINE__, __func__, allocator_name)

#define BE_TRACK_DEALLOC(ptr) \
core::MemoryTracker::instance().track_dealloc(ptr)
#else
#define BE_TRACK_ALLOC(ptr, size, allocator_name)
#define BE_TRACK_DEALLOC(ptr)
#endif