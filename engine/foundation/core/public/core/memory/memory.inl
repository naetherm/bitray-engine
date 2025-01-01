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
#include <cstring>	// For "memcpy()", "memmove()", "memset()" and "memcmp()"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
inline void Memory::copy(void *pDestination, const void *pSource, uint32 nNumOfBytes) {
  memcpy(pDestination, pSource, nNumOfBytes);
}

inline void Memory::move(void *pDestination, const void *pSource, uint32 nNumOfBytes) {
  memmove(pDestination, pSource, nNumOfBytes);
}

inline void Memory::set(void *pDestination, int nCharacter, uint32 nNumOfBytes) {
  memset(pDestination, nCharacter, nNumOfBytes);
}

inline int Memory::compare(const void *pFirstBuffer, const void *pSecondBuffer, uint32 nNumOfBytes) {
  return memcmp(pFirstBuffer, pSecondBuffer, nNumOfBytes);
}

inline void* Memory::reallocate(void* oldPointer, core::sizeT oldNumberOfBytes, core::sizeT newNumberOfBytes, core::sizeT alignment) {
  return SAllocator.reallocate(oldPointer, oldNumberOfBytes, newNumberOfBytes, alignment);
}

inline void* Memory::allocate(core::sizeT numberOfBytes) {
  return SAllocator.allocate(numberOfBytes, 1);
}

inline void Memory::free(void *ptr) {
  SAllocator.deallocate(ptr, sizeof(ptr));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
