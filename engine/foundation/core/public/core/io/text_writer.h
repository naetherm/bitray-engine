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
#include "core/container/vector.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * TextWriter
 *
 * @brief
 * A specialized stream writer for writing text data to a stream.
 * Provides methods to write characters, strings, lines, and multiple lines
 * to the associated stream. It extends the StreamWriter class.
 */
class TextWriter : public StreamWriter {
public:
  /**
   * @brief
   * Constructor.
   */
  TextWriter();

  /**
   * @brief
   * Destructor.
   */
  ~TextWriter() override;

public:
  /**
   * @brief
   * Writes a single character to the stream.
   *
   * @param[in] c
   * The character to write to the stream.
   */
  void write_char(unsigned char c);

  /**
   * @brief
   * Writes a string to the stream.
   *
   * @param[in] str
   * The string to write to the stream.
   */
  void write_string(const String& str);

  /**
   * @brief
   * Writes a single line of text to the stream.
   * A newline character is appended after the line.
   *
   * @param[in] line
   * The line of text to write to the stream.
   */
  void write_line(const String& line);

  /**
   * @brief
   * Writes multiple lines of text to the stream.
   * Each line is written with a newline character appended.
   *
   * @param[in] lines
   * A vector of strings, where each string represents a line to write.
   */
  void write_lines(const Vector<String>& lines);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}