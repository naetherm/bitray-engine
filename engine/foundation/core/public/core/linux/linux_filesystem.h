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
#include "core/io/filesystem_impl.h"


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
 * LinuxFilesystem
 *
 * @brief
 * Implementation of the Filesystem interface for Linux-based systems.
 * This class provides Linux-specific implementations of common filesystem
 * operations, including file and directory management.
 */
class LinuxFilesystem : public FilesystemImpl {
public:
  /**
   * @brief
   * Default constructor.
   */
  LinuxFilesystem() = default;

  /**
   * @brief
   * Virtual destructor.
   */
  ~LinuxFilesystem() override = default;

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
  void enumerate_files(const String& directoryName,
                       const String& realDirectoryName,
                       EEnumerationMode enumerationMode,
                       Vector<String>& filenames,
                       bool recursive = true,
                       bool returnAsRelativePaths = false) const override;

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
  [[nodiscard]] bool is_directory(const String& directory) const override;

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
  bool create_directory(const String& directory) const override;

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
  bool delete_directory(const String& directory) const override;

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
  bool directory_exists(const String& directory) const override;

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
  bool is_file(const String& directory) const override;

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
  [[nodiscard]] bool file_exists(const String& virtualFilename) const override;

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
   *
   * @note
   * - ZIP files may be password-protected. Each file may have a different password provided
   *   using the syntax "<filename>$<password>" (e.g., "myfile$mypassword").
   * - Due to the potential negative impact on loading time, password-secured asset packages
   *   are not supported.
   */
  [[nodiscard]] FileImpl* open_file(EFileMode fileMode, const String& filename) const override;

  /**
   * @brief
   * Closes an open file.
   *
   * @param[in] file
   * Reference to the file to close.
   */
  void close_file(FileImpl& file) const override;

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
  [[nodiscard]] core::int64 get_last_modification_time(const String& filename) const override;

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
  [[nodiscard]] core::int64 get_file_size(const String& filename) const override;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
