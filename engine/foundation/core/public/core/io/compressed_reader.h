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
#include "core/io/stream_reader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * CompressedReader
 *
 * @brief
 * A reader class for handling the decompression of data streams.
 *
 * The `CompressedReader` class provides functionality to read and decompress data from a stream.
 */
class CompressedReader : public StreamReader {
public:
  /**
   * @brief
   * Constructor.
   *
   * Initializes the `CompressedReader` for reading and decompressing data streams.
   */
  CompressedReader();

  /**
   * @brief
   * Destructor.
   *
   * Cleans up resources associated with the `CompressedReader`.
   */
  ~CompressedReader() override;

public:
  /**
   * @brief
   * Gets the size of the uncompressed data.
   *
   * @return
   * The size of the data in bytes after decompression.
   *
   * This function provides the total size of the data that would result after decompressing the stream.
   */
  [[nodiscard]] sizeT get_size() const;

  /**
   * @brief
   * Skips a specified number of bytes in the decompressed stream.
   *
   * @param[in] numOfBytes
   * Number of bytes to skip.
   *
   * This function advances the read pointer in the decompressed data by the specified number of bytes.
   */
  void skip(sizeT numOfBytes);

  /**
   * @brief
   * Reads and decompresses data from the stream.
   *
   * @param[out] dstBuffer
   * Pointer to the destination buffer where the decompressed data will be stored.
   *
   * @param[in] numBytes
   * Maximum number of bytes to read into the destination buffer.
   *
   * @return
   * The actual number of bytes read and decompressed.
   *
   * This function reads the compressed data from the stream, decompresses it, and stores the result
   * in the provided destination buffer.
   */
  sizeT read(void* dstBuffer, sizeT numBytes);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}