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
 * @class StackAllocator
 *
 * @brief A memory allocator that allocates memory from a stack.
 *
 * The StackAllocator is designed to allocate memory from a stack. It is
 * efficient for scenarios where memory is allocated and deallocated in a
 * LIFO (Last In First Out) order.
 */
class StackAllocator : public AllocatorImpl {
public:
    /**
     * @struct Marker
     *
     * @brief Marker struct
     *
     * The Marker struct is used to mark positions in the stack. It is
     * used by the StackAllocator to keep track of the current position
     * in the stack.
     */
    struct Marker {
        core::sizeT position;
    };

public:
    /**
     * @brief Constructor
     *
     * @param[in] bytes The total number of bytes this allocator should manage.
     */
    explicit StackAllocator(core::sizeT bytes);

    /**
     * @brief Destructor
     *
     * This destructor will deallocate the memory block allocated by the
     * StackAllocator.
     */
    ~StackAllocator() override;

    /**
     * @brief Allocates memory from the stack.
     *
     * @param[in] newNumberOfBytes The size of the memory block to allocate.
     * @param[in] alignment The alignment of the memory block.
     *
     * @return A pointer to the allocated memory block, or nullptr if allocation fails.
     */
    void* allocate(core::sizeT newNumberOfBytes, core::sizeT alignment = 1) override;

    /**
     * @brief Reallocates memory from the stack.
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
     * @brief Deallocates memory from the stack.
     *
     * This function will rollback the stack to the given pointer. If the pointer
     * is not a valid allocation, nothing will happen.
     *
     * @param[in] ptr The pointer to the memory block to deallocate.
     * @param[in] size The size of the memory block to deallocate.
     */
    void deallocate(void* ptr, core::sizeT size) override;

    /**
     * @brief Returns a stack marker that can be used to roll back the stack
     * to a previous state.
     *
     * @return A stack marker.
     */
    [[nodiscard]] Marker get_marker() const;

    /**
     * @brief Rolls back the stack to a previous state using a marker.
     *
     * This function will deallocate all memory allocated after the given marker
     * and reset the stack to the state it was in when the marker was set.
     *
     * @param[in] marker The marker to roll back to.
     */
    void roll_back(Marker marker);

    /**
     * @brief Returns the number of bytes that are currently allocated.
     *
     * @return The number of bytes that are currently allocated.
     */
    [[nodiscard]] core::sizeT get_used() const;

    /**
     * @brief Returns the number of bytes that are available for allocation.
     *
     * @return The number of bytes that are available for allocation.
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