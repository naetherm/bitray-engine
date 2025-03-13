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
#include "core/memory/mimalloc_allocator.h"
#include "core/memory/memory_tracker.h"
#include <mimalloc.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


MimallocAllocator::MimallocAllocator()
  : AllocatorImpl() {

}

MimallocAllocator::MimallocAllocator(const MimallocAllocator& rhs)
  : AllocatorImpl() {

}

MimallocAllocator::~MimallocAllocator() {

}

void *MimallocAllocator::allocate(core::sizeT newNumberOfBytes, core::sizeT alignment) {
  if (0 != newNumberOfBytes) {
    void* ptr = mi_malloc_aligned(newNumberOfBytes, alignment);

    BE_TRACK_ALLOC(ptr, newNumberOfBytes, "MimallocAllocator");
    return ptr;
  }

  return nullptr;
}

void *MimallocAllocator::reallocate(void *oldPointer, core::sizeT oldNumberOfBytes, core::sizeT newNumberOfBytes, core::sizeT alignment) {
  BE_TRACK_DEALLOC(oldPointer);
  void* newPtr = mi_realloc_aligned(oldPointer, newNumberOfBytes, alignment);
  BE_TRACK_ALLOC(newPtr, newNumberOfBytes, "MimallocAllocator");
  return newPtr;
}

void MimallocAllocator::deallocate(void *ptr, core::sizeT numberOfBytes) {
  if (ptr) {
    BE_TRACK_DEALLOC(ptr);
    mi_free(ptr);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core