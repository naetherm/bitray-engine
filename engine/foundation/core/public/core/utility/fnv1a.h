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
 * @class
 * FNV1a
 *
 * @brief
 * A utility class implementing the Fowler-Noll-Vo (FNV-1a) hash algorithm.
 *
 * The FNV-1a hash is a fast, non-cryptographic hash function commonly used in hash tables and checksums.
 */
class FNV1a {
public:

  /** Initial hash value for 32-bit FNV-1a hashing. */
  static constexpr uint32 FNV1a_INITIAL_HASH_32 = 0xcbf29ce4u;

  /** Initial hash value for 64-bit FNV-1a hashing. */
  static constexpr uint64 FNV1a_INITIAL_HASH_64 = 0xcbf29ce484222325u;

public:

  /**
   * @brief
   * Calculates the 32-bit FNV-1a hash for the given content.
   *
   * This function computes the hash based on the FNV-1a algorithm for a specified block of data.
   *
   * @param[in] content
   * Pointer to the data to be hashed. Must not be `nullptr`.
   * @param[in] numberOfBytes
   * The number of bytes to process from the `content` pointer.
   * @param[in] hash
   * An optional seed value for the hash. Defaults to `FNV1a_INITIAL_HASH_32`.
   *
   * @return
   * The calculated 32-bit FNV-1a hash value.
   */
  [[nodiscard]] static uint32 calculate_fnv1a_32(const uint8 *content, uint32 numberOfBytes, uint32 hash = FNV1a_INITIAL_HASH_32);

  /**
   * @brief
   * Calculates the 64-bit FNV-1a hash for the given content.
   *
   * This function computes the hash based on the FNV-1a algorithm for a specified block of data.
   *
   * @param[in] content
   * Pointer to the data to be hashed. Must not be `nullptr`.
   * @param[in] numberOfBytes
   * The number of bytes to process from the `content` pointer.
   * @param[in] hash
   * An optional seed value for the hash. Defaults to `FNV1a_INITIAL_HASH_64`.
   *
   * @return
   * The calculated 64-bit FNV-1a hash value.
   */
  [[nodiscard]] static uint64 calculate_fnv1a_64(const uint8 *content, uint32 numberOfBytes, uint64 hash = FNV1a_INITIAL_HASH_64);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}