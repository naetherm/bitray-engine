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
#include "core/memory/linear_allocator.h"
#include "core/memory/memory.h"
#include "core/memory/memory_tracker.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
LinearAllocator::LinearAllocator(core::sizeT bytes)
: AllocatorImpl()
, mCapacity(bytes)
, mUsed(0) {
  mBuffer = static_cast<char*>(Memory::allocate(mCapacity));
}

LinearAllocator::~LinearAllocator() {
  Memory::free(mBuffer);
}

void* LinearAllocator::allocate(core::sizeT newNumberOfBytes, core::sizeT alignment) {
  // Align the current position
  core::sizeT aligned_used = (mUsed + alignment - 1) & ~(alignment - 1);

  if (aligned_used + newNumberOfBytes > mCapacity) {
    return nullptr; // Out of memory
  }

  void* result = mBuffer + aligned_used;
  mUsed = aligned_used + newNumberOfBytes;

  BE_TRACK_ALLOC(result, newNumberOfBytes, "LinearAllocator");
  return result;
}

void* LinearAllocator::reallocate(void* oldPointer, core::sizeT oldNumberOfBytes, core::sizeT newNumberOfBytes, core::sizeT alignment) {
  // Linear allocator doesn't support reallocation within the buffer
  // We need to allocate a new block and copy the data

  // Check if pointer is in our buffer range
  char* charPtr = static_cast<char*>(oldPointer);
  if (charPtr < mBuffer || charPtr >= mBuffer + mCapacity) {
    return nullptr; // Not our pointer
  }

  // If new size is smaller, we can just return the existing pointer
  if (newNumberOfBytes <= oldNumberOfBytes) {
    return oldPointer;
  }

  // Otherwise we need to allocate a new block
  void* newPtr = allocate(newNumberOfBytes, alignment);
  if (newPtr) {
    // Copy old data
    Memory::copy(newPtr, oldPointer, oldNumberOfBytes);
  }

  // Don't deallocate old memory - linear allocator doesn't support individual deallocations
  return newPtr;
}

void LinearAllocator::deallocate(void* ptr, core::sizeT) {
  // Just track the deallocation but don't actually free (will be freed in bulk)
  BE_TRACK_DEALLOC(ptr);
}

void LinearAllocator::reset() {
  mUsed = 0;
}

core::sizeT LinearAllocator::get_used() const {
  return mUsed;
}

core::sizeT LinearAllocator::get_available() const {
  return mCapacity - mUsed;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}