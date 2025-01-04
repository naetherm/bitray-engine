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
#include "core/container/vector.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FileImpl;
class FilesystemImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * FilesystemHelper
 *
 * @brief
 * FilesystemHelper provides utility functions to assist with common filesystem
 * operations, such as normalizing paths, concatenating strings, and reading files
 * as strings. These static methods are designed for convenience and efficiency
 * when working with filesystem-related tasks.
 */
class FilesystemHelper {
public:
  /**
   * @brief
   * Normalizes a file path by resolving redundant separators, dots, or other irregularities.
   *
   * @param[in] path
   * A string representing the file path to be normalized.
   *
   * @return
   * A normalized version of the input path as a string.
   */
  static String normalize(String path);

  /**
   * @brief
   * Constructs a single string by concatenating a vector of strings, separated by a specified delimiter.
   *
   * @param[in] strings
   * A vector of strings to concatenate.
   *
   * @param[in] fence
   * A string delimiter to place between each string in the vector.
   *
   * @return
   * The concatenated string.
   */
  static String build_string(const Vector<String>& strings, String fence);

  /**
   * @brief
   * Reads the contents of a file as a single string using a provided Filesystem implementation.
   *
   * @param[in] filesystem
   * A reference to a FilesystemImpl object used to interact with the filesystem.
   *
   * @param[in] filename
   * The name of the file to read.
   *
   * @return
   * The contents of the file as a string.
   */
  static String read_file_as_string(const FilesystemImpl& filesystem, const String& filename);

  /**
   * @brief
   * Reads the contents of an open file as a single string.
   *
   * @param[in] file
   * A reference to a FileImpl object representing the open file.
   *
   * @return
   * The contents of the file as a string.
   */
  static String read_file_as_string(FileImpl& file);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
