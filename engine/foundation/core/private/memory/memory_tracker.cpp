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
#include "core/memory/memory_tracker.h"
#include "core/log/log.h"
#include "core/threading/lock_guard.h"


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
MemoryTracker& MemoryTracker::instance() {
  static MemoryTracker SInstance;
  return SInstance;
}

MemoryTracker::MemoryTracker()
: mTotalAllocatedBytes(0)
, mCurrentAllocatedBytes(0)
, mNumAllocations(0)
, mNumDeallocations(0)
, mEnabled(true) {
}

MemoryTracker::~MemoryTracker() {
}

sizeT MemoryTracker::get_total_allocated() const {
  return mTotalAllocatedBytes;
}

sizeT MemoryTracker::get_current_allocated() const {
  return mCurrentAllocatedBytes;
}

sizeT MemoryTracker::get_num_allocations() const {
  return mNumAllocations;
}

sizeT MemoryTracker::get_num_deallocations() const {
  return mNumDeallocations;
}

void MemoryTracker::enable(bool enable) {
  mEnabled = enable;
}

void MemoryTracker::track_alloc(void* ptr, sizeT size, const char* file, int line, const char* func, const String& allocatorName) {
  if (!mEnabled || ptr == nullptr) return;

  LockGuard<Mutex> lock(mMutex);

  AllocationInfo info;
  info.size = size;
  info.file = file;
  info.line = line;
  info.function = func;
  info.allocator_name = allocatorName;

  mAllocations[ptr] = info;
  mTotalAllocatedBytes += size;
  mCurrentAllocatedBytes += size;
  ++mNumAllocations;
}

void MemoryTracker::track_dealloc(void* ptr) {
  if (!mEnabled || ptr == nullptr) return;

  LockGuard<Mutex> lock(mMutex);

  auto it = mAllocations.find(ptr);
  if (it != mAllocations.end()) {
    mCurrentAllocatedBytes -= it->second.size;
    mAllocations.erase(it);
    ++mNumDeallocations;
  } else {
    // This could be a double-free or freeing unallocated memory
    BE_LOG(Warning, String().append_format("Deallocating untracked memory at %p", ptr))
  }
}

bool MemoryTracker::check_leaks() const {
  if (!mEnabled) return true;

  LockGuard<Mutex> lock(const_cast<Mutex&>(mMutex));
  return mAllocations.empty();
}

void MemoryTracker::print_stats() const {
  if (!mEnabled) {
    BE_LOG(Info, "Memory tracking disabled")
    return;
  }

  BE_LOG(Info, "Memory Statistics:")
  BE_LOG(Info, "Total Allocated Bytes: " + core::to_string(mTotalAllocatedBytes))
  BE_LOG(Info, "Total Current Allocated Bytes: " + core::to_string(mCurrentAllocatedBytes))
  BE_LOG(Info, "Num Allocations: " + core::to_string(mNumAllocations))
  BE_LOG(Info, "Num Deallocations: " + core::to_string(mNumDeallocations))
  BE_LOG(Info, "Active Allocations: " + core::to_string(mNumAllocations - mNumDeallocations))

  if (!check_leaks()) {
    leak_report();
  }
}

void MemoryTracker::leak_report() const {
  if (!check_leaks()) {
    BE_LOG(Info, "=== No memory leaks detected ===")

    for (auto& info: mAllocations) {
      String msg = "Unresolved allocation from " + core::to_string(info.second.line) + " in " + String(info.second.file);
    }
  }
}

void MemoryTracker::reset() {
  LockGuard<Mutex> lock(mMutex);
  mAllocations.clear();
  mTotalAllocatedBytes = 0;
  mCurrentAllocatedBytes = 0;
  mNumAllocations = 0;
  mNumDeallocations = 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}