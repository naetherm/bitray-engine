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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Compression
 *
 * @brief
 * Abstract compression implementation.
 */
class Compression {
public:

  /**
   * @brief
   * Constructor.
   */
  Compression() = default;

  /**
   * @brief
   * Destructor.
   */
  virtual ~Compression() = default;


  /**
   * @brief
   * Compresses the input data using the LZ4 algorithm.
   *
   * Compresses the data in `src` and stores the compressed output in `dst`.
   *
   * @param[in] dst Pointer to the destination buffer where compressed data will be stored.
   * @param[in] src Pointer to the source buffer containing data to compress.
   * @param[in] srcSize Size of the input data in bytes.
   * @param[in] dstCapacity Size of the destination buffer in bytes.
   *
   * @return The size of the compressed data written to `dst`.
   */
  virtual core::sizeT compress(core::uint8* dst, const core::uint8* src, core::sizeT srcSize, core::sizeT dstCapacity) const = 0;

  /**
   * @brief
   * Decompresses the input data using the LZ4 algorithm.
   *
   * Decompresses the data in `src` and stores the decompressed output in `dst`.
   *
   * @param[in] dst Pointer to the destination buffer where decompressed data will be stored.
   * @param[in] src Pointer to the source buffer containing compressed data.
   * @param[in] srcSize Size of the compressed data in bytes.
   * @param[in] dstSize Expected size of the decompressed data in bytes.
   *
   * @return The size of the decompressed data written to `dst`.
   */
  virtual core::sizeT decompress(core::uint8* dst, const core::uint8* src, core::sizeT srcSize, core::sizeT dstSize) const = 0;

  /**
   * @brief
   * Calculates the maximum size required for compression.
   *
   * Determines the upper bound for the compressed size of data with `srcSize`
   * bytes, which can be used to allocate a sufficient buffer for compression.
   *
   * @param[in] srcSize Size of the input data in bytes.
   *
   * @return The maximum size in bytes that the compressed data may occupy.
   */
  [[nodiscard]] virtual core::sizeT compress_bound(core::uint32 srcSize) const = 0;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
