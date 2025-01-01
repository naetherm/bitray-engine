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
#include "core/memory/mimalloc_allocator.h"
#include "core/memory/std_allocator.h"
#include "core/std/typetraits/conditional.h"
#include "core/std/utils.h"
#include "core/std/utility/move.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Memory
 *
 * @brief
 * Basic memory interface
 */
class Memory {
public:
  /**
   * @brief
   * Copies memory
   *
   * @param[in] pDestination
   * Destination buffer (MUST be valid!)
   * @param[in] pSource
   * Source buffer (MUST be valid!)
   * @param[in] nNumOfBytes
   * Number of bytes to copy
   *
   * @note
   * - 'memcpy'-wrapper
   * -  The memory areas should not overlap
   */
  static inline void copy(void *pDestination, const void *pSource, uint32 nNumOfBytes);

  /**
   * @brief
   * Moves memory
   *
   * @param[in] pDestination
   * Destination buffer (MUST be valid!)
   * @param[in] pSource
   * Source buffer (MUST be valid!)
   * @param[in] nNumOfBytes
   * Number of bytes to copy
   *
   * @note
   * - 'memmove'-wrapper
   * -  The memory areas may overlap
   */
  static inline void move(void *pDestination, const void *pSource, uint32 nNumOfBytes);

  /**
   * @brief
   * Sets memory to a specified character
   *
   * @param[in] pDestination
   * Destination buffer (MUST be valid!)
   * @param[in] nCharacter
   * Character to set
   * @param[in] nNumOfBytes
   * Number of bytes to set
   *
   * @note
   * - 'memset'-wrapper
   */
  static inline void set(void *pDestination, int nCharacter, uint32 nNumOfBytes);

  /**
   * @brief
   * Compares memory
   *
   * @param[in] pFirstBuffer
   * First buffer (MUST be valid!)
   * @param[in] pSecondBuffer
   * Second buffer (MUST be valid!)
   * @param[in] nNumOfBytes
   * Number of bytes to compare
   *
   * @return
   * < 0 = First buffer is less than second buffer
   *   0 = First buffer identical to second buffer
   * > 0 = First buffer is greater than second buffer
   *
   * @note
   * - 'memcmp'-wrapper
   */
  static inline int compare(const void *pFirstBuffer, const void *pSecondBuffer, uint32 nNumOfBytes);

  static inline void* reallocate(void* oldPointer, core::sizeT oldNumberOfBytes, core::sizeT newNumberOfBytes, core::sizeT alignment);

  static inline void* allocate(core::sizeT numberOfBytes);

  static inline void free(void *ptr);

private:

  //static StdAllocator SAllocator;
  static MimallocAllocator SAllocator;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/memory/memory.inl"


template<typename TType>
inline typename core::enable_if<!core::is_trivially_constructible<TType>::value>::type construct_item(TType* dst) {
  new (dst) TType();
}

template<typename TType>
inline typename core::enable_if<core::is_trivially_constructible<TType>::value>::type construct_item(TType* dst) {

}

template<typename TType>
inline typename core::enable_if<!core::is_trivially_constructible<TType>::value>::type construct_item(TType* dst, core::sizeT numElements) {
  while (numElements--) {
    new (dst) TType();
    ++(TType*&)dst;
  }
}

template<typename TType>
inline typename core::enable_if<core::is_trivially_constructible<TType>::value>::type construct_item(TType* dst, core::sizeT numElements) {

}

template<typename TType>
inline typename core::enable_if<!core::is_trivially_destructible<TType>::value>::type destruct_item(TType* dst) {
  dst->~TType();
}

template<typename TType>
inline typename core::enable_if<core::is_trivially_destructible<TType>::value>::type destruct_item(TType* dst) {

}

template<typename TType>
inline typename core::enable_if<!core::is_trivially_destructible<TType>::value>::type destruct_item(TType* dst, core::sizeT numElements) {
  while (numElements--) {
    dst->~TType();
    ++dst;
  }
}

template<typename TType>
inline typename core::enable_if<core::is_trivially_destructible<TType>::value>::type destruct_item(TType* dst, core::sizeT numElements) {

}


inline void re_free(void* ptr) {
  core::Memory::free(ptr);
}

template<class TType>
inline TType* re_typed_alloc() {
  TType* ptr = (TType*)core::Memory::allocate(sizeof(TType));
  return ptr;
}

template<class TType>
inline TType* re_typed_alloc(core::sizeT numElements) {
  TType* ptr = (TType*)core::Memory::allocate(sizeof(TType) * numElements);
  return ptr;
}

template<class TType>
inline TType* re_new() {
  TType* ptr = (TType*)core::Memory::allocate(sizeof(TType));
  construct_item(ptr);
  return ptr;
}

template<class TType, typename ... TArgs>
inline TType* re_new(TArgs&&... args) {
  TType* ptr = (TType*)core::Memory::allocate(sizeof(TType));
  new (ptr) TType(core::forward<TArgs>(args)...);
  return ptr;
}

template<class TType>
inline TType* re_new_array(core::sizeT numElements) {
  TType* ptr = (TType*)core::Memory::allocate(sizeof(TType) * numElements);
  construct_item(ptr, numElements);
  return ptr;
}

template<class TType>
inline void re_delete(TType* ptr) {
  destruct_item(ptr);
  core::Memory::free(ptr);
}

template<class TType>
inline void re_delete_array(TType* ptr, core::sizeT numElements) {
  destruct_item(ptr, numElements);
  core::Memory::free(static_cast<void *>(ptr));
}

template<class TType>
inline void re_delete_array(const TType* ptr, core::sizeT numElements) {
  destruct_item(ptr, numElements);
  core::Memory::free(static_cast<void *>(const_cast<TType*>(ptr)));
}