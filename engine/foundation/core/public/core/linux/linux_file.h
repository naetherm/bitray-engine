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
#include "core/io/file_impl.h"
#include "core/io/types.h"
#include "core/string/string.h"


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
class LinuxFile : public FileImpl {
public:

  LinuxFile();

  ~LinuxFile() override;
  
public:

  //[-------------------------------------------------------]
  //[ Read                                                  ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Return the number of bytes inside the file
   *
   * @return
   * The number of bytes inside the file
   */
  [[nodiscard]] uint64 get_number_of_bytes() override;

  /**
   * @brief
   * Returns whether end of file has been reached.
   *
   * @return
   * 'true' if the end of the file has been reached, 'false' otherwise.
   */
  bool is_eof() const;

  /**
   * @brief
   * Flushes the file buffer content.
   *
   * @return
   * 'true' if everything went fine, 'false' otherwise.
   */
  bool flush();

  /**
   * @brief
   * Opens the file.
   *
   * @return
   * True if everything went fine.
   */
  bool open(const String& absoluteFilename, EFileMode fileMode) override;

  /**
   * @brief
   * Closes the file.
   */
  void close();

  /**
   * @brief
   * Gets the current position of the file pointer.
   *
   * @return
   * The current byte position of the file pointer, < 0 if there was an error.
   */
  uint64 tell() const;

  /**
   * @brief
   * Seeks the proposed position of the file.
   *
   * @param[in] seek
   * The position to seek.
   */
  void seek(uint64 seek);

  /**
   * @brief
   * Read a requested number of bytes from the file
   *
   * @param[out] destinationBuffer
   * Destination buffer were to write to, must be at least "numberOfBytes" long, never ever a
   * null pointer
   * @param[in] numberOfBytes
   * Number of bytes to write into the destination buffer, it's the callers responsibility that
   * this number of byte is correct
   */
  uint64 read(void *destinationBuffer, uint64 numberOfBytes) override;

  /**
   * @brief
   * Skip a requested number of bytes
   *
   * @param[in] numberOfBytes
   * Number of bytes to skip, it's the callers responsibility that this number of byte is
   * correct
   */
  void skip(uint64 numberOfBytes) override;

  //[-------------------------------------------------------]
  //[ Write                                                 ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Write a requested number of bytes into the file
   *
   * @param[in] sourceBuffer
   * Source buffer were to read from, must be at least "numberOfBytes" long, never ever a null
   * pointer
   * @param[in] numberOfBytes
   * Number of bytes to read from the source buffer and write into the file, it's the callers
   * responsibility that this number of byte is correct
   */
  uint64 write(const void *sourceBuffer, uint64 numberOfBytes) override;

  /**
   * @brief
   * Gets the last modification time of the file.
   *
   * @return
   * The last modification time of the file.
   */
  [[nodiscard]] core::int64 get_last_modification_time() const override;

  /**
   * @brief
   * Gets the file size.
   *
   * @return
   * The file size, -1 if it cannot be determined.
   */
  [[nodiscard]] core::int64 get_file_size() const override;

  String get_str() override;

private:

  FILE* mFile;
  EFileMode mAccess;
  core::String mAbsoluteFilename;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}