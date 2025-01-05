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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * MemoryStream
 *
 * @brief
 * Provides a stream implementation that operates on an in-memory buffer.
 */
class MemoryStream : public Stream {
public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes an empty memory stream with a default initial capacity.
   */
  MemoryStream();

  /**
   * @brief
   * Destructor.
   *
   * Releases the allocated memory.
   */
  ~MemoryStream() override;

  /**
   * @brief
   * Retrieves the raw pointer to the memory buffer.
   *
   * @return
   * A void pointer to the internal memory buffer.
   */
  void* get_raw_pointer() const;

public:
  /**
   * @brief
   * Checks if the stream supports reading.
   *
   * @return
   * `true` if the stream is readable, otherwise `false`.
   */
  [[nodiscard]] bool can_read() const override;

  /**
   * @brief
   * Checks if the stream supports writing.
   *
   * @return
   * `true` if the stream is writable, otherwise `false`.
   */
  [[nodiscard]] bool can_write() const override;

  /**
   * @brief
   * Checks if the stream supports seeking.
   *
   * @return
   * `true` if the stream supports seeking, otherwise `false`.
   */
  [[nodiscard]] bool can_seek() const override;

  /**
   * @brief
   * Checks if the stream can be memory-mapped.
   *
   * @return
   * `true` if the stream supports memory mapping, otherwise `false`.
   */
  [[nodiscard]] bool can_be_mapped() const override;

  /**
   * @brief
   * Retrieves the total size of the memory stream.
   *
   * @return
   * The size of the stream in bytes.
   */
  [[nodiscard]] sizeT get_size() const override;

  /**
   * @brief
   * Retrieves the current position in the memory stream.
   *
   * @return
   * The current position in bytes from the beginning of the stream.
   */
  [[nodiscard]] sizeT get_position() const override;

  /**
   * @brief
   * Opens the memory stream for use.
   *
   * @return
   * `true` if the stream was successfully opened, otherwise `false`.
   */
  bool open() override;

  /**
   * @brief
   * Closes the memory stream and releases resources.
   */
  void close() override;

  /**
   * @brief
   * Writes data to the memory stream.
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
   * Reads data from the memory stream.
   *
   * @param[out] ptr
   * A pointer to the buffer to receive the data.
   *
   * @param[in] numBytes
   * The number of bytes to read.
   *
   * @return
   * The actual number of bytes read.
   */
  sizeT read(void* ptr, sizeT numBytes) override;

  /**
   * @brief
   * Moves the stream position.
   *
   * @param[in] offset
   * The byte offset relative to the position specified by `seek`.
   *
   * @param[in] seek
   * The seek origin.
   */
  void seek(sizeT offset, EFileSeek seek) override;

  /**
   * @brief
   * Flushes any buffered data to the memory stream.
   */
  void flush() override;

  /**
   * @brief
   * Checks if the end of the stream has been reached.
   *
   * @return
   * `true` if the end of the stream is reached, otherwise `false`.
   */
  [[nodiscard]] bool eof() const override;

  /**
   * @brief
   * Retrieves the content of the memory stream as a string.
   *
   * @return
   * A string containing the content of the stream.
   */
  [[nodiscard]] String get_content_as_string() override;

  /**
   * @brief
   * Maps the memory stream to a raw pointer.
   *
   * @return
   * A pointer to the mapped memory buffer.
   */
  void* map() override;

  /**
   * @brief
   * Unmaps the memory stream from the raw pointer.
   */
  void unmap() override;

  /**
   * @brief
   * Maps the memory stream to a raw pointer for low-level operations.
   *
   * @return
   * A pointer to the mapped memory buffer.
   */
  void* memory_map() override;

  /**
   * @brief
   * Unmaps the memory stream from the low-level pointer.
   */
  void memory_unmap() override;

protected:
  /**
   * @brief
   * Resizes the internal buffer to accommodate more data.
   *
   * @param[in] newSize
   * The new size of the buffer in bytes.
   */
  void realloc(sizeT newSize);

  /**
   * @brief
   * Checks if the memory stream has enough space for additional data.
   *
   * @param[in] numBytes
   * The number of bytes to check for.
   *
   * @return
   * `true` if there is enough space, otherwise `false`.
   */
  bool has_space(sizeT numBytes) const;

  /**
   * @brief
   * Expands the memory stream to make space for additional data.
   *
   * @param[in] numBytes
   * The number of bytes to allocate.
   */
  void make_space(sizeT numBytes);

private:
  /** The initial buffer size. */
  static const sizeT InitialSize = 256;

  /** The current capacity of the buffer. */
  sizeT mCapacity;

  /** The current size of the data in the buffer. */
  sizeT mSize;

  /** The current position in the buffer. */
  sizeT mPosition;

  /** The internal memory buffer. */
  unsigned char* mBuffer;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}