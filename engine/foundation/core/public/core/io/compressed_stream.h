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
#include "core/io/stream.h"
#include "core/io/compressed_file.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * CompressedStream
 *
 * @brief
 * A stream implementation for reading and writing compressed data, using LZ4 compression.
 *
 * This class provides the ability to work with compressed files seamlessly as streams.
 */
class CompressedStream : public Stream {
public:
  /**
   * @brief
   * Constructor.
   *
   * Initializes the CompressedStream for managing compressed data.
   */
  CompressedStream();

  /**
   * @brief
   * Destructor.
   *
   * Releases resources associated with the CompressedStream.
   */
  ~CompressedStream() override;

  /**
   * @brief
   * Skips a specified number of bytes in the stream.
   *
   * @param[in] numOfBytes
   * The number of bytes to skip.
   */
  void skip(sizeT numOfBytes);

public:
  /**
   * @brief
   * Determines if the stream can be read.
   *
   * @return
   * `true` if the stream supports reading; otherwise, `false`.
   */
  [[nodiscard]] bool can_read() const override;

  /**
   * @brief
   * Determines if the stream can be written to.
   *
   * @return
   * `true` if the stream supports writing; otherwise, `false`.
   */
  [[nodiscard]] bool can_write() const override;

  /**
   * @brief
   * Determines if the stream supports seeking.
   *
   * @return
   * `true` if the stream supports seeking; otherwise, `false`.
   */
  [[nodiscard]] bool can_seek() const override;

  /**
   * @brief
   * Determines if the stream can be memory-mapped.
   *
   * @return
   * `true` if the stream supports memory mapping; otherwise, `false`.
   */
  [[nodiscard]] bool can_be_mapped() const override;

  /**
   * @brief
   * Gets the size of the stream content in bytes.
   *
   * @return
   * The size of the stream in bytes.
   */
  [[nodiscard]] sizeT get_size() const override;

  /**
   * @brief
   * Gets the current position in the stream.
   *
   * @return
   * The current position in bytes from the beginning of the stream.
   */
  [[nodiscard]] sizeT get_position() const override;

  /**
   * @brief
   * Opens the compressed stream for use.
   *
   * @return
   * `true` if the stream was successfully opened; otherwise, `false`.
   */
  bool open() override;

  /**
   * @brief
   * Closes the compressed stream and releases associated resources.
   */
  void close() override;

  /**
   * @brief
   * Writes data to the stream.
   *
   * @param[in] ptr
   * Pointer to the data to write.
   * @param[in] numBytes
   * Number of bytes to write.
   */
  void write(const void* ptr, sizeT numBytes) override;

  /**
   * @brief
   * Reads data from the stream.
   *
   * @param[out] ptr
   * Pointer to the buffer to store read data.
   * @param[in] numBytes
   * Number of bytes to read.
   *
   * @return
   * The actual number of bytes read.
   */
  sizeT read(void* ptr, sizeT numBytes) override;

  /**
   * @brief
   * Seeks to a specified offset in the stream.
   *
   * @param[in] offset
   * Offset to seek to.
   * @param[in] seek
   * Seek mode (e.g., from beginning, current position, or end).
   */
  void seek(sizeT offset, EFileSeek seek) override;

  /**
   * @brief
   * Flushes any buffered data to the stream.
   */
  void flush() override;

  /**
   * @brief
   * Checks whether the end of the stream has been reached.
   *
   * @return
   * `true` if the end of the stream is reached; otherwise, `false`.
   */
  [[nodiscard]] bool eof() const override;

  /**
   * @brief
   * Retrieves the content of the stream as a string.
   *
   * @return
   * The stream content as a UTF-8 string.
   */
  [[nodiscard]] String get_content_as_string() override;

private:
  /** Internal LZ4 compressed file representation */
  CompressedFileLZ4 mCompressedFile;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}