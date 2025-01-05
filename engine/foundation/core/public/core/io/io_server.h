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
#include "core/core/server_impl.h"
#include "core/core/ptr.h"
#include "core/io/path.h"
#include "core/io/types.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Filesystem;
class MountPointRegistry;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * IoServer
 *
 * @brief
 * A server implementation for managing I/O operations, including files, directories, mount points, and paths.
 */
class IoServer : public ServerImpl {
public:
  /**
   * @brief
   * Retrieves the singleton instance of the IoServer.
   *
   * @return
   * A pointer to the IoServer instance.
   */
  static IoServer* instance();

public:
  /**
   * @brief
   * Constructor.
   *
   * Initializes the IoServer with required components.
   */
  IoServer();

  /**
   * @brief
   * Destructor.
   *
   * Cleans up resources used by the IoServer.
   */
  ~IoServer() override;

  // Getter and Setter

  /**
   * @brief
   * Retrieves a constant reference to the mount point registry.
   *
   * @return
   * A constant pointer to the MountPointRegistry.
   */
  [[nodiscard]] const Ptr<MountPointRegistry>& get_mount_point_registry() const;

  /**
   * @brief
   * Retrieves a mutable reference to the mount point registry.
   *
   * @return
   * A pointer to the MountPointRegistry.
   */
  [[nodiscard]] Ptr<MountPointRegistry>& get_mount_point_registry();

  /**
   * @brief
   * Mounts a real directory to a virtual mount point.
   *
   * @param[in] realDirectory
   * The real directory path.
   *
   * @param[in] mountPoint
   * The virtual mount point.
   *
   * @param[in] appendToPath
   * Whether to append to the search path if the mount already exists.
   *
   * @return
   * `true` if the directory was successfully mounted, otherwise `false`.
   */
  bool mount(const String& realDirectory, const String& mountPoint, bool appendToPath);

  /**
   * @brief
   * Unmounts a real directory.
   *
   * @param[in] realDirectory
   * The real directory path.
   *
   * @return
   * `true` if the directory was successfully unmounted, otherwise `false`.
   */
  bool unmount(const String& realDirectory);

  /**
   * @brief
   * Checks if a mount point is currently mounted.
   *
   * @param[in] mountPoint
   * The virtual mount point.
   *
   * @return
   * `true` if the mount point is mounted, otherwise `false`.
   */
  [[nodiscard]] bool is_mounted(const String& mountPoint) const;

  /**
   * @brief
   * Retrieves the real directory corresponding to a virtual mount point.
   *
   * @param[in] mountPoint
   * The virtual mount point.
   *
   * @return
   * The real directory path.
   */
  [[nodiscard]] String get_real_directory(const String& mountPoint) const;

  /**
   * @brief
   * Resolves a virtual path to its real path representation.
   *
   * @param[in] path
   * The virtual path.
   *
   * @return
   * The resolved real path.
   */
  [[nodiscard]] Path resolve_path(const Path& path) const;

  /**
   * @brief
   * Resolves a virtual path string to its real path representation.
   *
   * @param[in] path
   * The virtual path string.
   *
   * @return
   * The resolved real path string.
   */
  [[nodiscard]] String resolve_path(const String& path) const;

  // Streams

  // Directory

  /**
   * @brief
   * Creates a directory at the specified path.
   *
   * @param[in] path
   * The path to the directory.
   *
   * @return
   * `true` if the directory was successfully created, otherwise `false`.
   */
  bool create_directory(const Path& path) const;

  /**
   * @brief
   * Deletes the specified directory.
   *
   * @param[in] path
   * The path to the directory.
   *
   * @return
   * `true` if the directory was successfully deleted, otherwise `false`.
   */
  [[nodiscard]] bool delete_directory(const Path& path) const;

  /**
   * @brief
   * Checks if the specified path is a directory.
   *
   * @param[in] path
   * The path to check.
   *
   * @return
   * `true` if the path is a directory, otherwise `false`.
   */
  [[nodiscard]] bool is_directory(const Path& path) const;

  /**
   * @brief
   * Checks if the specified directory exists.
   *
   * @param[in] path
   * The path to the directory.
   *
   * @return
   * `true` if the directory exists, otherwise `false`.
   */
  [[nodiscard]] bool directory_exists(const Path& path) const;

  // File

  /**
   * @brief
   * Checks if the specified path is a file.
   *
   * @param[in] path
   * The path to check.
   *
   * @return
   * `true` if the path is a file, otherwise `false`.
   */
  [[nodiscard]] bool is_file(const Path& path) const;

  /**
   * @brief
   * Checks if a file exists at the specified path.
   *
   * @param[in] path
   * The path to the file.
   *
   * @return
   * `true` if the file exists, otherwise `false`.
   */
  [[nodiscard]] bool file_exists(const Path& path) const;

  /**
   * @brief
   * Copies a file from one location to another.
   *
   * @param[in] from
   * The source file path.
   *
   * @param[in] to
   * The destination file path.
   *
   * @return
   * `true` if the file was successfully copied, otherwise `false`.
   */
  [[nodiscard]] bool copy_file(const Path& from, const Path& to) const;

  /**
   * @brief
   * Deletes the specified file.
   *
   * @param[in] path
   * The path to the file.
   *
   * @return
   * `true` if the file was successfully deleted, otherwise `false`.
   */
  [[nodiscard]] bool delete_file(const Path& path);

  // Filesystem

  /**
   * @brief
   * Retrieves a constant reference to the filesystem implementation.
   *
   * @return
   * A constant pointer to the Filesystem.
   */
  [[nodiscard]] const Ptr<Filesystem>& get_filesystem() const;

  /**
   * @brief
   * Retrieves a mutable reference to the filesystem implementation.
   *
   * @return
   * A pointer to the Filesystem.
   */
  [[nodiscard]] Ptr<Filesystem>& get_filesystem();

  /**
   * @brief
   * Enumerates files in a directory with specified options.
   *
   * @param[in] directoryName
   * The virtual directory name.
   *
   * @param[in] realDirectoryName
   * The real directory name.
   *
   * @param[in] enumerationMode
   * The mode of file enumeration.
   *
   * @param[out] entries
   * A vector to store the enumerated file names.
   *
   * @param[in] recursive
   * Whether to enumerate files recursively.
   *
   * @param[in] returnAsRelativePaths
   * Whether to return paths as relative to the base directory.
   */
  void enumerate_files(const String& directoryName, const String& realDirectoryName, EEnumerationMode enumerationMode, Vector<String>& entries, bool recursive = true, bool returnAsRelativePaths = false) const;

  /**
   * @brief
   * Retrieves the last modification time of a file.
   *
   * @param[in] filename
   * The name of the file.
   *
   * @return
   * The last modification time in Unix timestamp format.
   */
  [[nodiscard]] core::int64 get_last_modification_time(const String& filename) const;

  /**
   * @brief
   * Retrieves the size of a file.
   *
   * @param[in] filename
   * The name of the file.
   *
   * @return
   * The size of the file in bytes.
   */
  [[nodiscard]] core::int64 get_file_size(const String& filename) const;

private:
  /** Pointer to the mount point registry. */
  Ptr<MountPointRegistry> mMountPointRegistry;

  /** Pointer to the filesystem implementation. */
  Ptr<Filesystem> mFilesystem;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}