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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * BinaryWriter
 *
 * @brief
 * A class for writing binary data to a stream.
 *
 * The `BinaryWriter` class provides methods to write basic data types (like `char`, `short`,
 * and `int`) to a stream in a binary format. It allows you to open and close streams for
 * writing binary data and manage internal memory mapping for efficiency.
 */
class BinaryWriter : public StreamWriter {
public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes a `BinaryWriter` instance, setting up internal variables for stream handling.
   */
  BinaryWriter();

  /**
   * @brief
   * Destructor.
   *
   * Cleans up resources used by the `BinaryWriter` and ensures any open streams are properly
   * closed.
   */
  ~BinaryWriter() override;

public:
  /**
   * @brief
   * Writes a single character to the stream.
   *
   * @param[in] c
   * The character to be written to the stream.
   */
  void write_char(char c);

  /**
   * @brief
   * Writes a short integer (16 bits) to the stream.
   *
   * @param[in] s
   * The short integer to be written to the stream.
   */
  void write_short(short s);

  /**
   * @brief
   * Writes an integer (32 bits) to the stream.
   *
   * @param[in] i
   * The integer to be written to the stream.
   */
  void write_int(int i);

public:
  /**
   * @brief
   * Opens the stream for writing.
   *
   * This method prepares the stream for writing binary data. It may handle memory mapping
   * or any other initialization required for the stream.
   *
   * @return
   * `true` if the stream was successfully opened, `false` otherwise.
   */
  bool open() override;

  /**
   * @brief
   * Closes the stream after writing.
   *
   * This method finalizes any pending write operations and releases resources associated
   * with the stream.
   */
  void close() override;

private:
  /** Pointer to the current position in the memory-mapped stream */
  unsigned char* mMapCursor;
  /** Pointer to the end of the memory-mapped stream */
  unsigned char* mMapEnd;

  /** Flag indicating if memory mapping is enabled */
  bool mEnableMapping;
  /** Flag indicating if the stream is currently memory-mapped */
  bool mIsMapped;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}