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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/io/mount_point_registry.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
MountPointRegistry::MountPointRegistry() {

}

MountPointRegistry::~MountPointRegistry() {

}

void MountPointRegistry::add_mount_point(const MountPoint &mountPoint) {
  if (!has_mount_point(mountPoint.get_mount())) {
    mMountPoints.push_back(mountPoint);
    mMapMountPoints[mountPoint.get_mount()] = mountPoint;
  }
}

void MountPointRegistry::add_mount_point(const String &mountPoint, const String &realDirectory) {
  if (!has_mount_point(mountPoint)) {
    mMountPoints.emplace_back(mountPoint, realDirectory);
    mMapMountPoints[mountPoint] = mMountPoints.back();
  }
}

void MountPointRegistry::remove_mount_point(const String &mountPoint) {
  BE_ASSERT(has_mount_point(mountPoint), "Trying to unmount not registered mount point")

  auto element = mMapMountPoints.find(mountPoint);
  mMountPoints.erase(element.value());
  mMapMountPoints.erase(mountPoint);
}

bool MountPointRegistry::has_mount_point(const core::String &mountPoint) const {
  return mMapMountPoints.find(mountPoint) != mMapMountPoints.cend();
}

const MountPoint &MountPointRegistry::get_mount_point_by_index(core::uint32 index) const {
  return mMountPoints[index];
}

const MountPoint &MountPointRegistry::get_mount_point_by_name(const String &mountPoint) const {
  return mMapMountPoints.at(mountPoint);
}

const Vector<MountPoint> &MountPointRegistry::get_mount_points() const {
  return mMountPoints;
}

core::uint32 MountPointRegistry::get_num_of_mount_points() const {
  return mMountPoints.size();
}

String MountPointRegistry::get_real_path_of_mount(const String& mountPoint) const {
  if (has_mount_point(mountPoint)) {
    return mMapMountPoints.at(mountPoint).get_real_path();
  } else {
    static String SEmpty = "";
    return SEmpty;
  }
}

Path MountPointRegistry::resolve_path(const Path &path) const {
  return Path(resolve_path(path.get_native_path()));
}

String MountPointRegistry::resolve_path(const String &path) const {
  String resultPath = path;

  String::SizeType  colonPosition = resultPath.find(":");

  while (colonPosition != String::NPOS) {
    String mountString = resultPath.substr(0, colonPosition);
    if (has_mount_point(mountString)) {
      resultPath.replace(colonPosition, 1, "/");
      resultPath = resultPath.replace(0, colonPosition, get_real_path_of_mount(mountString));
    } else {
      break; // No more mount points found, break out of the loop
    }

    colonPosition = resultPath.find(":");
  }

  return resultPath;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
