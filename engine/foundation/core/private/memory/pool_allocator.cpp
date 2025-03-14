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
#include "core/memory/pool_allocator.h"
#include "core/memory/memory.h"
#include "core/memory/memory_tracker.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
PoolAllocator::PoolAllocator(core::sizeT size, core::sizeT chunk_size)
: AllocatorImpl()
, mBlockSize(std::max(size, sizeof(FreeBlock)))
, mBlocksPerChunk(chunk_size)
, mFreeList(nullptr) {
  allocate_chunk();
}

PoolAllocator::~PoolAllocator() {
  for (void* chunk : mChunks) {
    Memory::free(chunk);
  }
}

void* PoolAllocator::allocate(core::sizeT newNumberOfBytes, core::sizeT) {
  if (newNumberOfBytes > mBlockSize) {
    return nullptr; // Size too large for this pool
  }

  if (!mFreeList) {
    allocate_chunk();
  }

  FreeBlock* block = mFreeList;
  mFreeList = mFreeList->next;

  BE_TRACK_ALLOC(block, newNumberOfBytes, "PoolAllocator");
  return block;
}

void* PoolAllocator::reallocate(void* oldPointer, core::sizeT oldNumberOfBytes, core::sizeT newNumberOfBytes,
  core::sizeT alignment) {
  // Pool allocator doesn't support reallocation with different sizes
  if (newNumberOfBytes <= mBlockSize && oldNumberOfBytes <= mBlockSize) {
    // If new size still fits in a block, just return the old pointer
    return oldPointer;
  }

  // If new size doesn't fit, we need to allocate a new block elsewhere
  return nullptr; // Indicate reallocation failure, caller should handle this
}

void PoolAllocator::deallocate(void* ptr, core::sizeT) {
  BE_TRACK_DEALLOC(ptr);

  FreeBlock* block = static_cast<FreeBlock*>(ptr);
  block->next = mFreeList;
  mFreeList = block;
}

core::sizeT PoolAllocator::get_block_size() const {
  return mBlockSize;
}

void PoolAllocator::allocate_chunk() {
  char* chunk = static_cast<char*>(Memory::allocate(mBlockSize * mBlocksPerChunk));
  if (!chunk) throw std::bad_alloc();

  mChunks.push_back(chunk);

  // Initialize free list with new blocks
  for (core::sizeT i = 0; i < mBlocksPerChunk - 1; ++i) {
    FreeBlock* block = reinterpret_cast<FreeBlock*>(chunk + i * mBlockSize);
    block->next = reinterpret_cast<FreeBlock*>(chunk + (i + 1) * mBlockSize);
  }

  // Last block points to current free list
  FreeBlock* last_block = reinterpret_cast<FreeBlock*>(chunk + (mBlocksPerChunk - 1) * mBlockSize);
  last_block->next = mFreeList;
  mFreeList = reinterpret_cast<FreeBlock*>(chunk);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}