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
#include "core/memory/stack_allocator.h"
#include "core/memory/memory.h"
#include "core/memory/memory_tracker.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
StackAllocator::StackAllocator(core::sizeT bytes)
: AllocatorImpl()
, mCapacity(bytes)
, mUsed(0) {
  mBuffer = static_cast<char*>(Memory::allocate(mCapacity));
}

StackAllocator::~StackAllocator() {
  Memory::free(mBuffer);
}

void* StackAllocator::allocate(core::sizeT newNumberOfBytes, core::sizeT alignment) {
  // Align the current position
  core::sizeT aligned_used = (mUsed + alignment - 1) & ~(alignment - 1);

  if (aligned_used + newNumberOfBytes > mCapacity) {
    return nullptr;  // Out of memory
  }

  void* result = mBuffer + aligned_used;
  mUsed = aligned_used + newNumberOfBytes;

  BE_TRACK_ALLOC(result, newNumberOfBytes, "StackAllocator");
  return result;
}

void* StackAllocator::reallocate(void* oldPointer, core::sizeT oldNumberOfBytes, core::sizeT newNumberOfBytes,
  core::sizeT alignment) {
  // Check if this is the top of the stack (the most recent allocation)
  char* oldCharPtr = static_cast<char*>(oldPointer);
  core::sizeT old_offset = oldCharPtr - mBuffer;

  // If this is the most recent allocation and we're growing, we can just extend it
  if (old_offset + oldNumberOfBytes == mUsed && newNumberOfBytes > oldNumberOfBytes) {
    core::sizeT additionalBytes = newNumberOfBytes - oldNumberOfBytes;
    if (mUsed + additionalBytes <= mCapacity) {
      mUsed += additionalBytes;
      BE_TRACK_DEALLOC(oldPointer);
      BE_TRACK_ALLOC(oldPointer, newNumberOfBytes, "StackAllocator");
      return oldPointer;
    }
  }

  // Otherwise, we need to allocate a new block and copy
  void* newPtr = allocate(newNumberOfBytes, alignment);
  if (newPtr && oldPointer) {
    // Copy the old data
    Memory::copy(newPtr, oldPointer, std::min(oldNumberOfBytes, newNumberOfBytes));

    // Don't actually deallocate in a stack allocator
    BE_TRACK_DEALLOC(oldPointer);
  }

  return newPtr;
}

void StackAllocator::deallocate(void* ptr, core::sizeT size) {
  // Just track the deallocation
  BE_TRACK_DEALLOC(ptr);

  // If this is the top of the stack, we can actually reclaim the space
  char* charPtr = static_cast<char*>(ptr);
  core::sizeT offset = charPtr - mBuffer;
  if (offset + size == mUsed) {
    mUsed = offset;
  }
  // Otherwise, memory will be reclaimed when marker is rolled back or allocator is reset
}

StackAllocator::Marker StackAllocator::get_marker() const {
  return Marker{mUsed};
}

void StackAllocator::roll_back(Marker marker) {
  // When rolling back, notify the tracker about the deallocations
  if (marker.position < mUsed) {
    // This is a simplification - ideally we'd track each allocation individually
    BE_TRACK_DEALLOC(mBuffer + marker.position);
  }
  mUsed = marker.position;
}

core::sizeT StackAllocator::get_used() const {
  return mUsed;
}

core::sizeT StackAllocator::get_available() const {
  return mCapacity - mUsed;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}