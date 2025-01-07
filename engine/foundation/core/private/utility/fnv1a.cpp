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
#include "core/utility/fnv1a.h"
#include "core/core/default_assert.h"


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
uint32 FNV1a::calculate_fnv1a_32(const uint8* content, uint32 numberOfBytes, uint32 hash) {
  // Sanity checks
  BE_ASSERT(nullptr != content, "The content must be valid to be able to calculate a FNV1a32 hash")
  BE_ASSERT(0 != numberOfBytes, "The content must be valid to be able to calculate a FNV1a32 hash")

  // 32-bit FNV-1a implementation basing on http://www.isthe.com/chongo/tech/comp/fnv/
  static constexpr uint32 FNV1a_MAGIC_PRIME_32 = 0x1000193u;
  const uint8* end = content + numberOfBytes;
  for ( ; content < end; ++content)
  {
    hash = (hash ^ *content) * FNV1a_MAGIC_PRIME_32;
  }
  return hash;
}

uint64 FNV1a::calculate_fnv1a_64(const uint8* content, uint32 numberOfBytes, uint64 hash) {
  // Sanity checks
  BE_ASSERT(nullptr != content, "The content must be valid to be able to calculate a FNV1a32 hash")
  BE_ASSERT(0 != numberOfBytes, "The content must be valid to be able to calculate a FNV1a32 hash")

  // 64-bit FNV-1a implementation basing on http://www.isthe.com/chongo/tech/comp/fnv/
  static constexpr uint64 FNV1a_MAGIC_PRIME_64 = 0x100000001B3u;
  const uint8* end = content + numberOfBytes;
  for ( ; content < end; ++content)
  {
    hash = (hash ^ *content) * FNV1a_MAGIC_PRIME_64;
  }
  return hash;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}