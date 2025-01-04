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
#include "core/core/refcounted.h"
#include "core/io/media_type.h"
#include "core/io/path.h"
#include "core/io/types.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Stream
 *
 * @brief
 * Stream offers an abstract interface for read and write access to a sequential
 * stream of bytes.
 * It is the base class for all other classes that can provide read and write to
 * data, e.g. a FileStream, MemoryStream, CompressedStream, etc.
 *
 * Stream instances can be accessed directly or through stream readers and writers
 * which offer specific reading and writing interfaces for streams.
 */
class Stream : public RefCounted {
public:
  /**
   * @brief
   * Constructor.
   */
  Stream();

  /**
   * @brief
   * Destructor.
   */
  virtual ~Stream();

  /**
   * @brief
   * Sets the path of the stream.
   *
   * @param[in] path
   * A reference to a Path object that represents the path of the stream.
   */
  void set_path(const Path& path);

  /**
   * @brief
   * This function retrieves the path of the stream.
   *
   * @return
   * A reference to a Path object that represents the path of the stream.
   */
  [[nodiscard]] const Path& get_path() const;

  /**
   * @brief
   * Sets the access mode for the stream.
   *
   * @param[in] fileMode
   * The file access mode, specified as an EFileMode enumeration value.
   */
  void set_access_mode(EFileMode fileMode);

  /**
   * @brief
   * Retrieves the current access mode of the stream.
   *
   * @return
   * The file access mode, specified as an EFileMode enumeration value.
   */
  [[nodiscard]] EFileMode get_access_mode() const;


  /**
   * @brief
   * Checks whether the stream is currently open.
   *
   * @return
   * True if the stream is open, false otherwise.
   */
  [[nodiscard]] bool is_open() const;

  /**
   * @brief
   * Checks whether the stream is memory-mapped.
   *
   * @return
   * True if the stream is mapped, false otherwise.
   */
  [[nodiscard]] bool is_mapped() const;


  /**
   * @brief
   * Checks if the stream supports reading.
   *
   * @return
   * True if the stream supports reading, false otherwise.
   */
  [[nodiscard]] virtual bool can_read() const;

  /**
   * @brief
   * Checks if the stream supports writing.
   *
   * @return
   * True if the stream supports writing, false otherwise.
   */
  [[nodiscard]] virtual bool can_write() const;

  /**
   * @brief
   * Checks if the stream supports seeking.
   *
   * @return
   * True if the stream supports seeking, false otherwise.
   */
  [[nodiscard]] virtual bool can_seek() const;

  /**
   * @brief
   * Checks if the stream can be memory-mapped.
   *
   * @return
   * True if the stream can be mapped, false otherwise.
   */
  [[nodiscard]] virtual bool can_be_mapped() const;

  /**
   * @brief
   * Retrieves the total size of the stream in bytes.
   *
   * @return
   * The size of the stream in bytes.
   */
  [[nodiscard]] virtual sizeT get_size() const;

  /**
   * @brief
   * Retrieves the current read/write position in the stream.
   *
   * @return
   * The current position in the stream.
   */
  [[nodiscard]] virtual sizeT get_position() const;


  /**
   * @brief
   * Opens the stream for reading or writing based on its access mode.
   *
   * @return
   * True if the stream was successfully opened, false otherwise.
   */
  virtual bool open();

  /**
   * @brief
   * Closes the stream and releases associated resources.
   */
  virtual void close();

  /**
   * @brief
   * Writes data to the stream.
   *
   * @param[in] ptr
   * A pointer to the data to be written.
   * @param[in] numBytes
   * The number of bytes to write.
   */
  virtual void write(const void* ptr, sizeT numBytes);

  /**
   * @brief
   * Reads data from the stream.
   *
   * @param[out] ptr
   * A pointer to the buffer where the read data will be stored.
   * @param[in] numBytes
   * The number of bytes to read.
   *
   * @return
   * The number of bytes actually read.
   */
  virtual sizeT read(void* ptr, sizeT numBytes);

  /**
   * @brief
   * Moves the stream's read/write position.
   *
   * @param[in] offset
   * The offset from the reference position.
   * @param[in] seek
   * The reference position, specified as an EFileSeek enumeration value.
   */
  virtual void seek(sizeT offset, EFileSeek seek);

  /**
   * @brief
   * Flushes any buffered data to the stream.
   */
  virtual void flush();

  /**
   * @brief
   * Checks if the end of the stream has been reached.
   *
   * @return
   * True if the end of the stream is reached, false otherwise.
   */
  [[nodiscard]] virtual bool eof() const;

  /**
   * @brief
   * Reads the entire content of the stream and returns it as a string.
   *
   * @return
   * A string containing the stream's content.
   */
  [[nodiscard]] virtual String get_content_as_string();


  /**
   * @brief
   * Maps the entire stream into memory for direct access.
   *
   * @return
   * A pointer to the mapped memory.
   */
  virtual void* map();

  /**
   * @brief
   * Unmaps a previously memory-mapped stream.
   */
  virtual void unmap();

  /**
   * @brief
   * Maps the stream using platform-specific memory mapping.
   *
   * @return
   * A pointer to the mapped memory.
   */
  virtual void* memory_map();

  /**
   * @brief
   * Unmaps a previously memory-mapped stream using platform-specific methods.
   */
  virtual void memory_unmap();

protected:
  /** The path associated with the stream. */
  Path mPath;

  /** The media type of the stream. */
  MediaType mMediaType;

  /** The file access mode for the stream. */
  EFileMode mFileAccessMode;

  /** The access pattern used by the stream. */
  EAccessPattern mAccessPattern;

  /** Indicates whether the stream is currently open. */
  bool mIsOpen;

  /** Indicates whether the stream is memory-mapped. */
  bool mIsMapped;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}