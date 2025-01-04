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
 * TextReader
 *
 * @brief
 * A specialized stream reader for reading text data from a stream.
 * Provides methods to read characters, lines, and the entire text content
 * from the associated stream. It extends the StreamReader class.
 */
class TextReader : public StreamReader {
public:
  /**
   * @brief
   * Constructor.
   */
  TextReader();

  /**
   * @brief
   * Destructor.
   */
  ~TextReader() override;

public:
  /**
   * @brief
   * Reads a single character from the stream.
   *
   * @return
   * The character read from the stream as an unsigned char.
   */
  unsigned char read_char();

  /**
   * @brief
   * Reads a single line of text from the stream.
   *
   * @return
   * A string containing the text of the line read.
   * The line does not include the newline character.
   */
  String read_line();

  /**
   * @brief
   * Reads the entire content of the stream as a string.
   *
   * @return
   * A string containing all the text from the stream.
   */
  String read_all();

  /**
   * @brief
   * Reads all lines of text from the stream.
   *
   * @return
   * A vector of strings, where each element represents a line of text.
   * Lines do not include newline characters.
   */
  Vector<String> read_all_lines();
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}