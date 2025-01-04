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
#include "core/core/ptr.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Stream;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * StreamReader
 *
 * @brief
 * Stream reader interface implementation.
 * Streamer readers are specialised reading interfaces for streams.
 * This one is an abstract base class for all stream readers.
 * It is possible to attach any number of readers and writers to the
 * same stream.
 */
class StreamWriter {
public:
  /**
   * @brief
   * Constructor.
   */
  StreamWriter();

  /**
   * @brief
   * Destructor.
   */
  virtual ~StreamWriter();

  /**
   * @brief
   * Sets the stream to be used by the writer.
   *
   * @param[in] stream
   * A shared pointer to the Stream object to be associated with this writer.
   */
  void set_stream(const Ptr<Stream>& stream);

  /**
   * @brief
   * Retrieves the stream associated with this writer.
   *
   * @return
   * A constant shared pointer to the associated Stream object.
   */
  [[nodiscard]] const Ptr<Stream>& get_stream() const;

  /**
   * @brief
   * Retrieves the stream associated with this writer.
   *
   * @return
   * A shared pointer to the associated Stream object.
   */
  [[nodiscard]] Ptr<Stream>& get_stream();

  /**
   * @brief
   * Checks if a stream is associated with this writer.
   *
   * @return
   * True if a stream is set, false otherwise.
   */
  [[nodiscard]] bool has_stream() const;

  /**
   * @brief
   * Checks if the writer has reached the end of the stream.
   *
   * @return
   * True if the end of the stream is reached, false otherwise.
   */
  [[nodiscard]] bool is_eof() const;

  /**
   * @brief
   * Checks if the writer is currently open.
   *
   * @return
   * True if the writer is open, false otherwise.
   */
  [[nodiscard]] bool is_open() const;

public:
  /**
   * @brief
   * Opens the stream writer for writing operations.
   *
   * @return
   * True if the writer was successfully opened, false otherwise.
   */
  virtual bool open();

  /**
   * @brief
   * Closes the stream writer and releases any associated resources.
   */
  virtual void close();

protected:
  /** A shared pointer to the stream associated with this writer. */
  Ptr<Stream> mStream;

  /** Indicates whether the stream writer is currently open. */
  bool mIsOpen;

  /** Indicates whether the associated stream was open when the writer was set. */
  bool mStreamWasOpen;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}