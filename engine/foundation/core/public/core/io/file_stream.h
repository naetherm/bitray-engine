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
#include "core/io/file.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * FileStream
 *
 * @brief
 * FileStream provides functionality to read from, write to, and manage files
 * as streams of bytes. It supports file operations such as seeking, reading,
 * writing, and mapping file content into memory. This class extends the
 * Stream base class.
 */
class FileStream : public Stream {
public:
  /**
   * @brief
   * Constructor.
   */
  FileStream();

  /**
   * @brief
   * Destructor.
   */
  ~FileStream() override;

public:
  /**
   * @brief
   * Checks if the stream supports reading.
   *
   * @return
   * True if the stream supports reading, false otherwise.
   */
  [[nodiscard]] bool can_read() const override;

  /**
   * @brief
   * Checks if the stream supports writing.
   *
   * @return
   * True if the stream supports writing, false otherwise.
   */
  [[nodiscard]] bool can_write() const override;

  /**
   * @brief
   * Checks if the stream supports seeking.
   *
   * @return
   * True if the stream supports seeking, false otherwise.
   */
  [[nodiscard]] bool can_seek() const override;

  /**
   * @brief
   * Checks if the stream can be memory-mapped.
   *
   * @return
   * True if the stream can be memory-mapped, false otherwise.
   */
  [[nodiscard]] bool can_be_mapped() const override;

  /**
   * @brief
   * Retrieves the size of the file.
   *
   * @return
   * The size of the file in bytes.
   */
  [[nodiscard]] sizeT get_size() const override;

  /**
   * @brief
   * Retrieves the current position of the stream cursor.
   *
   * @return
   * The current position in the stream in bytes.
   */
  [[nodiscard]] sizeT get_position() const override;

  /**
   * @brief
   * Opens the file stream for reading or writing.
   *
   * @return
   * True if the file was successfully opened, false otherwise.
   */
  bool open() override;

  /**
   * @brief
   * Closes the file stream and releases any associated resources.
   */
  void close() override;

  /**
   * @brief
   * Writes data to the file stream.
   *
   * @param[in] ptr
   * A pointer to the data to write.
   *
   * @param[in] numBytes
   * The number of bytes to write.
   */
  void write(const void* ptr, sizeT numBytes) override;

  /**
   * @brief
   * Reads data from the file stream.
   *
   * @param[out] ptr
   * A pointer to the buffer where the read data will be stored.
   *
   * @param[in] numBytes
   * The number of bytes to read.
   *
   * @return
   * The number of bytes successfully read.
   */
  sizeT read(void* ptr, sizeT numBytes) override;

  /**
   * @brief
   * Moves the stream cursor to a new position.
   *
   * @param[in] offset
   * The offset to seek to.
   *
   * @param[in] seek
   * The seek direction, specified by EFileSeek.
   */
  void seek(sizeT offset, EFileSeek seek) override;

  /**
   * @brief
   * Flushes any buffered data to the file.
   */
  void flush() override;

  /**
   * @brief
   * Checks if the stream has reached the end of the file.
   *
   * @return
   * True if the end of the file is reached, false otherwise.
   */
  [[nodiscard]] bool eof() const override;

  /**
   * @brief
   * Reads the entire file content as a string.
   *
   * @return
   * A string containing the file content.
   */
  [[nodiscard]] String get_content_as_string() override;

protected:
  /** Handle to the file being accessed by this stream. */
  File mFileHandle;

  /** Handle to the mapped file, used for memory-mapping the file content. */
  File mMapFileHandle;

  /** Pointer to the mapped file content in memory. */
  void* mMappedContent;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}