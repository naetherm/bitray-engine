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
#include "core/memory/allocator_impl.h"
#include "core/container/vector.h"


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
 * @class PoolAllocator
 *
 * @brief A memory pool allocator.
 *
 * PoolAllocator is designed to manage memory in fixed-size blocks. It allocates memory in chunks,
 * and each chunk is divided into blocks of a specified size. This allocator is efficient for
 * scenarios where many small memory allocations and deallocations are needed.
 */
class PoolAllocator : public AllocatorImpl {

public:
  /**
   * @brief Constructor for PoolAllocator.
   *
   * @param[in] size The size of each block in the memory pool.
   * @param[in] chunk_size The number of blocks in each chunk (default is 1024).
   */
  PoolAllocator(core::sizeT size, core::sizeT chunk_size = 1024);

  /**
   * @brief Destructor for PoolAllocator.
   *
   * Deallocates all chunks managed by this allocator.
   */
  ~PoolAllocator() override;

  /**
   * @brief Allocates memory from the pool.
   *
   * @param[in] newNumberOfBytes The size of the memory block to allocate.
   * @param[in] alignment The alignment of the memory block.
   *
   * @return A pointer to the allocated memory block, or nullptr if allocation fails.
   */
  void* allocate(core::sizeT newNumberOfBytes, core::sizeT) override;

  /**
   * @brief Reallocates memory from the pool.
   *
   * @param[in] oldPointer The pointer to the memory block to reallocate.
   * @param[in] oldNumberOfBytes The size of the original memory block.
   * @param[in] newNumberOfBytes The new size of the memory block.
   * @param[in] alignment The alignment of the memory block.
   *
   * @return A pointer to the reallocated memory block, or nullptr if reallocation fails.
   */
  void* reallocate(void* oldPointer, core::sizeT oldNumberOfBytes, core::sizeT newNumberOfBytes, core::sizeT alignment) override;

  /**
   * @brief Deallocates memory from the pool.
   *
   * @param[in] ptr The pointer to the memory block to deallocate.
   * @param[in] size The size of the memory block to deallocate.
   */
  void deallocate(void* ptr, core::sizeT) override;

  /**
   * @brief Returns the block size of this allocator.
   *
   * @return The size of each block in the memory pool.
   */
  [[nodiscard]] core::sizeT get_block_size() const;

private:
  /**
   * @brief A single free block in the pool.
   *
   * FreeBlock is a struct used to maintain the free list of blocks in the pool.
   * It contains a pointer to the next free block in the list.
   */
  struct FreeBlock {
    FreeBlock* next;
  };

  /**
   * @brief Allocates a new chunk of memory for the pool.
   *
   * This function allocates a new chunk of memory with the given block size
   * and the number of blocks specified by blocks_per_chunk. It then initializes
   * the free list with the new blocks.
   */
  void allocate_chunk();

private:

  core::sizeT mBlockSize;
  core::sizeT mBlocksPerChunk;
  FreeBlock* mFreeList;
  Vector<void*> mChunks;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}