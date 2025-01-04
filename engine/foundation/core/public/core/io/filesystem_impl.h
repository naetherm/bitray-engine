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
#include "core/core/manager.h"
#include "core/container/vector.h"
#include "core/io/types.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FileImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * FilesystemImpl
 *
 * @brief
 * Abstract base class for implementing filesystem operations. This class defines the
 * interface for concrete filesystem implementations to provide functionalities such as
 * file enumeration, directory management, and file manipulation.
 */
class FilesystemImpl {
public:
  /**
   * @brief
   * Default constructor.
   */
  FilesystemImpl() = default;

  /**
   * @brief
   * Virtual destructor.
   */
  virtual ~FilesystemImpl() = default;

public:
  /**
   * @brief
   * Enumerates files in a directory.
   *
   * @param[in] directoryName
   * The virtual name of the directory to enumerate.
   *
   * @param[in] realDirectoryName
   * The actual name of the directory to enumerate.
   *
   * @param[in] enumerationMode
   * Mode of enumeration (e.g., files only, directories only, or both).
   *
   * @param[out] filenames
   * Vector to store the list of filenames found.
   *
   * @param[in] recursive
   * Whether to include files in subdirectories recursively (default is true).
   *
   * @param[in] returnAsRelativePaths
   * Whether to return paths relative to the given directory (default is false).
   */
  virtual void enumerate_files(const String& directoryName,
                               const String& realDirectoryName,
                               EEnumerationMode enumerationMode,
                               Vector<String>& filenames,
                               bool recursive = true,
                               bool returnAsRelativePaths = false) const = 0;

  //[ Directories ]

  /**
   * @brief
   * Checks if the specified path is a directory.
   *
   * @param[in] directory
   * The virtual path to check.
   *
   * @return
   * `true` if the path is a directory, otherwise `false`.
   */
  [[nodiscard]] virtual bool is_directory(const String& directory) const = 0;

  /**
   * @brief
   * Creates directories recursively.
   *
   * @param[in] directory
   * Virtual UTF-8 name of the directory to create, including all parent directories.
   *
   * @return
   * `true` if the directories were successfully created or already exist, otherwise `false`.
   */
  virtual bool create_directory(const String& directory) const = 0;

  /**
   * @brief
   * Deletes a directory.
   *
   * @param[in] directory
   * Virtual name of the directory to delete.
   *
   * @return
   * `true` if the directory was successfully deleted, otherwise `false`.
   */
  virtual bool delete_directory(const String& directory) const = 0;

  /**
   * @brief
   * Checks whether a directory exists.
   *
   * @param[in] directory
   * Path to the directory.
   *
   * @return
   * `true` if the directory exists, otherwise `false`.
   */
  virtual bool directory_exists(const String& directory) const = 0;

  //[ Files ]

  /**
   * @brief
   * Checks if the specified path is a file.
   *
   * @param[in] directory
   * The virtual path to check.
   *
   * @return
   * `true` if the path is a file, otherwise `false`.
   */
  virtual bool is_file(const String& directory) const = 0;

  /**
   * @brief
   * Checks whether a file exists.
   *
   * @param[in] virtualFilename
   * UTF-8 virtual filename of the file to check.
   *
   * @return
   * `true` if the file exists, otherwise `false`.
   */
  [[nodiscard]] virtual bool file_exists(const String& virtualFilename) const = 0;

  /**
   * @brief
   * Opens a file with the specified mode.
   *
   * @param[in] fileMode
   * The mode in which to open the file (e.g., read, write).
   *
   * @param[in] filename
   * UTF-8 virtual filename of the file to open.
   *
   * @return
   * A pointer to a `FileImpl` instance representing the opened file,
   * or `nullptr` if the file could not be opened.
   */
  [[nodiscard]] virtual FileImpl* open_file(EFileMode fileMode, const String& filename) const = 0;

  /**
   * @brief
   * Closes an open file.
   *
   * @param[in] file
   * Reference to the file to close.
   */
  virtual void close_file(FileImpl& file) const = 0;

  /**
   * @brief
   * Gets the last modification time of a file.
   *
   * @param[in] filename
   * UTF-8 filename of the file.
   *
   * @return
   * The last modification time of the file as an integer.
   */
  [[nodiscard]] virtual core::int64 get_last_modification_time(const String& filename) const = 0;

  /**
   * @brief
   * Gets the size of a file.
   *
   * @param[in] filename
   * UTF-8 filename of the file.
   *
   * @return
   * The size of the file in bytes.
   */
  [[nodiscard]] virtual core::int64 get_file_size(const String& filename) const = 0;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core