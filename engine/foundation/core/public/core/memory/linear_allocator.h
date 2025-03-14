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
 * @class LinearAllocator
 *
 * @brief A memory allocator that allocates memory linearly.
 *
 * The LinearAllocator is designed to allocate memory in a linear fashion,
 * meaning that it allocates a continuous block of memory and provides
 * allocations in a sequential manner. This allocator is efficient for
 * scenarios where memory is allocated once and deallocated all at once.
 */
class LinearAllocator : public AllocatorImpl {
public:

    /**
     * @brief Constructor
     *
     * @param[in] bytes The total number of bytes this allocator should manage.
     */
    explicit LinearAllocator(core::sizeT bytes);

    /**
     * @brief Destructor
     *
     * This destructor will free the memory allocated by the
     * LinearAllocator.
     */
    ~LinearAllocator();

    /**
     * @brief Allocates a block of memory with the specified size and alignment.
     *
     * This function allocates a block of memory of the given size and alignment.
     * If the memory cannot be allocated, it returns nullptr.
     *
     * @param[in] newNumberOfBytes The size of the memory block to allocate.
     * @param[in] alignment The alignment of the memory block.
     *
     * @return A pointer to the allocated memory block, or nullptr if allocation fails.
     */
    void* allocate(core::sizeT newNumberOfBytes, core::sizeT alignment = 1) override;

    /**
     * @brief Reallocates memory from the linear allocator.
     *
     * This function attempts to resize a previously allocated memory block
     * to a new size, while maintaining the original data. If the reallocation
     * cannot be performed, it returns nullptr.
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
     * @brief Deallocates memory from the linear allocator.
     *
     * Note: The LinearAllocator does not support individual deallocation of memory blocks.
     * This method only tracks the deallocation for debugging purposes.
     *
     * @param[in] ptr The pointer to the memory block to deallocate.
     */
    void deallocate(void* ptr, core::sizeT) override;

    /**
     * @brief Resets the allocator to its initial state.
     *
     * This function will deallocate all allocated memory and reset the allocator
     * to its initial state.
     */
    void reset();

    /**
     * @brief Returns the number of bytes that are currently allocated.
     *
     * @return The number of bytes that are currently allocated.
     */
    [[nodiscard]] core::sizeT get_used() const;

    /**
     * @brief Returns the number of bytes that are currently available for allocation.
     *
     * This function returns the number of bytes that are currently available for
     * allocation. This is equal to the total capacity of the allocator minus the
     * number of bytes already allocated.
     *
     * @return The number of bytes that are currently available for allocation.
     */
    [[nodiscard]] core::sizeT get_available() const;

private:
    char* mBuffer;
    core::sizeT mCapacity;
    core::sizeT mUsed;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}