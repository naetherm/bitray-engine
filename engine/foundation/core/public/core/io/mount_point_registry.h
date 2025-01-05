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
#include "core/core/refcounted.h"
#include "core/container/hash_map.h"
#include "core/container/vector.h"
#include "core/io/mount_point.h"
#include "core/io/path.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * MountPointRegistry
 *
 * @brief
 * Manages a collection of mount points, allowing for the registration,
 * removal, and querying of virtual-to-physical path mappings.
 */
class MountPointRegistry : public RefCounted {
public:
  /**
   * @brief
   * Constructor.
   *
   * Initializes an empty mount point registry.
   */
  MountPointRegistry();

  /**
   * @brief
   * Destructor.
   *
   * Cleans up resources used by the mount point registry.
   */
  ~MountPointRegistry();

  /**
   * @brief
   * Adds a mount point to the registry.
   *
   * @param[in] mountPoint
   * The MountPoint instance to add.
   */
  void add_mount_point(const MountPoint& mountPoint);

  /**
   * @brief
   * Adds a mount point to the registry by specifying its components.
   *
   * @param[in] mountPoint
   * The virtual mount point name.
   *
   * @param[in] realDirectory
   * The real filesystem directory corresponding to the mount point.
   */
  void add_mount_point(const String& mountPoint, const String& realDirectory);

  /**
   * @brief
   * Removes a mount point from the registry.
   *
   * @param[in] mountPoint
   * The name of the virtual mount point to remove.
   */
  void remove_mount_point(const String& mountPoint);

  /**
   * @brief
   * Checks if a mount point exists in the registry.
   *
   * @param[in] mountPoint
   * The name of the virtual mount point to check.
   *
   * @return
   * `true` if the mount point exists, otherwise `false`.
   */
  [[nodiscard]] bool has_mount_point(const core::String& mountPoint) const;

  /**
   * @brief
   * Retrieves a mount point by its index in the registry.
   *
   * @param[in] index
   * The index of the mount point to retrieve.
   *
   * @return
   * A reference to the MountPoint at the specified index.
   *
   * @note
   * Ensure the index is within bounds to avoid undefined behavior.
   */
  [[nodiscard]] const MountPoint& get_mount_point_by_index(core::uint32 index) const;

  /**
   * @brief
   * Retrieves a mount point by its name.
   *
   * @param[in] mountPoint
   * The name of the virtual mount point to retrieve.
   *
   * @return
   * A reference to the MountPoint associated with the given name.
   *
   * @throws std::out_of_range
   * If the mount point does not exist in the registry.
   */
  [[nodiscard]] const MountPoint& get_mount_point_by_name(const core::String& mountPoint) const;

  /**
   * @brief
   * Retrieves all mount points in the registry.
   *
   * @return
   * A constant reference to a vector containing all registered MountPoint instances.
   */
  [[nodiscard]] const Vector<MountPoint>& get_mount_points() const;

  /**
   * @brief
   * Gets the total number of mount points in the registry.
   *
   * @return
   * The number of mount points currently registered.
   */
  [[nodiscard]] core::uint32 get_num_of_mount_points() const;

  /**
   * @brief
   * Retrieves the real filesystem path for a given mount point.
   *
   * @param[in] mountPoint
   * The name of the virtual mount point.
   *
   * @return
   * The real filesystem path associated with the given mount point.
   *
   * @throws std::out_of_range
   * If the mount point does not exist in the registry.
   */
  [[nodiscard]] String get_real_path_of_mount(const String& mountPoint) const;

  /**
   * @brief
   * Resolves a virtual path to its corresponding real filesystem path.
   *
   * @param[in] path
   * The virtual path to resolve.
   *
   * @return
   * The resolved real filesystem path.
   */
  [[nodiscard]] Path resolve_path(const Path& path) const;

  /**
   * @brief
   * Resolves a virtual path string to its corresponding real filesystem path.
   *
   * @param[in] path
   * The virtual path string to resolve.
   *
   * @return
   * The resolved real filesystem path string.
   */
  [[nodiscard]] String resolve_path(const String& path) const;

private:
  /** Vector storing all registered mount points. */
  Vector<MountPoint> mMountPoints;

  /** Hash map for quick lookup of mount points by name. */
  hash_map<String, MountPoint> mMapMountPoints;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
