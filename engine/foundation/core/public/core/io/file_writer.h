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
#include "core/io/stream_writer.h"
#include "core/io/types.h"


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
 * FileWriter
 *
 * @brief
 * FileWriter provides functionality for writing data to a file stream.
 * This class extends the StreamWriter base class and offers additional
 * methods for managing file position, size, and write operations.
 */
class FileWriter : public StreamWriter {
public:
  /**
   * @brief
   * Constructor.
   */
  FileWriter();

  /**
   * @brief
   * Destructor.
   */
  ~FileWriter() override;

  /**
   * @brief
   * Retrieves the size of the file being written.
   *
   * @return
   * The size of the file in bytes.
   */
  sizeT get_size() const;

  /**
   * @brief
   * Retrieves the current position of the write cursor in the file.
   *
   * @return
   * The current position in the file in bytes.
   */
  sizeT get_position() const;

  /**
   * @brief
   * Moves the write cursor to a new position in the file.
   *
   * @param[in] pos
   * The position to seek to, relative to the mode.
   *
   * @param[in] mode
   * The seek mode, specified by EFileSeek.
   */
  void seek(sizeT pos, EFileSeek mode);

public:
  /**
   * @brief
   * Opens the file for writing.
   *
   * @return
   * True if the file was successfully opened, false otherwise.
   */
  bool open() override;

  /**
   * @brief
   * Closes the file, flushing any pending writes and releasing resources.
   */
  void close() override;

  /**
   * @brief
   * Writes data to the file.
   *
   * @param[in] dstBuffer
   * A pointer to the buffer containing the data to write.
   *
   * @param[in] numBytes
   * The number of bytes to write from the buffer.
   */
  void write(const void* dstBuffer, sizeT numBytes);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}