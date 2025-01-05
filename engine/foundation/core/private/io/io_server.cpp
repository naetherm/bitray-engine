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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
#include "core/io/io_server.h"
#include "core/io/filesystem.h"
#include "core/io/mount_point_registry.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
IoServer* IoServer::instance() {
  static IoServer* SInstance = new IoServer();
  return SInstance;
}


IoServer::IoServer()
: mMountPointRegistry(new MountPointRegistry())
, mFilesystem(new Filesystem()) {
}

IoServer::~IoServer() {

}

const Ptr<MountPointRegistry> &IoServer::get_mount_point_registry() const {
  return mMountPointRegistry;
}

Ptr<MountPointRegistry> &IoServer::get_mount_point_registry() {
  return mMountPointRegistry;
}

bool IoServer::mount(const String &realDirectory, const String &mountPoint, bool appendToPath) {
  if (!mMountPointRegistry->has_mount_point(mountPoint)) {
    mMountPointRegistry->add_mount_point(mountPoint, realDirectory);

    return true;
  }
  return false;
}

bool IoServer::unmount(const String &mountPoint) {
  if (mMountPointRegistry->has_mount_point(mountPoint)) {
    mMountPointRegistry->remove_mount_point(mountPoint);
    return true;
  }
  return false;
}

bool IoServer::is_mounted(const String& mountPoint) const {
  return mMountPointRegistry->has_mount_point(mountPoint);
}

String IoServer::get_real_directory(const String& mountPoint) const {
  return mMountPointRegistry->get_real_path_of_mount(mountPoint);
}

Path IoServer::resolve_path(const Path& path) const {
  return mMountPointRegistry->resolve_path(path);
}

String IoServer::resolve_path(const String& path) const {
  return mMountPointRegistry->resolve_path(path);
}


bool IoServer::create_directory(const Path &path) const {
  return mFilesystem->create_directory(mMountPointRegistry->resolve_path(path.get_native_path()));
}

bool IoServer::delete_directory(const Path &path) const {
  return mFilesystem->delete_directory(mMountPointRegistry->resolve_path(path.get_native_path()));
}

bool IoServer::is_directory(const core::Path &path) const {
  return mFilesystem->is_directory(mMountPointRegistry->resolve_path(path.get_native_path()));
}

bool IoServer::directory_exists(const Path &path) const {
  return mFilesystem->directory_exists(mMountPointRegistry->resolve_path(path.get_native_path()));
}


bool IoServer::is_file(const Path &path) const {
  return mFilesystem->is_file(mMountPointRegistry->resolve_path(path.get_native_path()));
}

bool IoServer::file_exists(const Path &path) const {
  return mFilesystem->file_exists(mMountPointRegistry->resolve_path(path.get_native_path()));
}

bool IoServer::copy_file(const Path &from, const Path &to) const {
  return false;
}

bool IoServer::delete_file(const Path &path) {
  return false;
}

void IoServer::enumerate_files(const String &directoryName, const String &realDirectoryName,
                               EEnumerationMode enumerationMode, Vector<String> &entries, bool recursive,
                               bool returnAsRelativePaths) const {
  mFilesystem->enumerate_files(
    directoryName,
    mMountPointRegistry->resolve_path(realDirectoryName),
    enumerationMode,
    entries,
    recursive,
    returnAsRelativePaths);
}


// Filesystem

const Ptr<Filesystem>& IoServer::get_filesystem() const {
  return mFilesystem;
}

Ptr<Filesystem>& IoServer::get_filesystem() {
  return mFilesystem;
}

core::int64 IoServer::get_last_modification_time(const String &filename) const {
  return mFilesystem->get_last_modification_time(mMountPointRegistry->resolve_path(filename));
}

core::int64 IoServer::get_file_size(const String &filename) const {
  return mFilesystem->get_file_size(mMountPointRegistry->resolve_path(filename));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}