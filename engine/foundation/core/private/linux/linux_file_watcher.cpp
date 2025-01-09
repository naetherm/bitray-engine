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
#include "core/linux/linux_file_watcher.h"
#include "core/memory/memory.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/inotify.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


namespace detail {

static core::Vector<core::String> list_directory_entries(const char* dirPath) {
  core::Vector<core::String> entries;

  DIR* dir = opendir(dirPath);

  if (dir != nullptr) {
    while (true) {
      struct dirent* dirEntry = readdir(dir);

      if (dirEntry == nullptr) {
        break;
      }

      core::uint32 numBytes = strlen(dirEntry->d_name);
      char* stringBuffer = new char[numBytes];

      core::Memory::copy(&stringBuffer[0], dirEntry->d_name, numBytes + 1);

      entries.push_back(stringBuffer);
    }
  }

  return entries;
}

static void create_watcher_recursively(core::int32 fileHandle, const char* dirPath) {
  core::Path path(dirPath);
  if (path.is_directory()) {
    // Entry for a directory, add a watcher for that as well
    inotify_add_watch(
      fileHandle,
      dirPath,
      IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM | IN_MOVE_SELF | IN_DELETE_SELF);

    // Get all elements of the directory and call this recursively
    core::Vector<core::String> fileEntries = list_directory_entries(dirPath);

    for (core::uint32 i = 0; i < fileEntries.size(); ++i) {
      core::String& fileEntry = fileEntries[i];
      // Skip the entries for the current directory and the parent directory
      if (fileEntry[0] == '.') {
        continue;
      }

      core::Path fullFilePath = path / fileEntry;

      // Call recursively
      create_watcher_recursively(
        fileHandle,
        fullFilePath.get_native_path().c_str());
    }
  }
}

}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
LinuxFileWatcher::LinuxFileWatcher(const Path &path)
  : FileWatcherImpl(path)
  , mHandle(0) {
  // Initialize the notification handle
  initialize_notification_handle();
}

LinuxFileWatcher::LinuxFileWatcher(const String &watcherPath)
  : FileWatcherImpl(watcherPath)
  , mHandle(0) {
  // Initialize the notification handle
  initialize_notification_handle();
}

LinuxFileWatcher::~LinuxFileWatcher() {
  // Nothing to do here
  close();
}

bool LinuxFileWatcher::any_changes() const {
  fd_set fileHandles;
  FD_ZERO(&fileHandles);
  FD_SET(mHandle, &fileHandles);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  bool anyChanges = select(FD_SETSIZE, &fileHandles, nullptr, nullptr, &timeout) > 0;

  // Read flush element
  if (anyChanges) {
    read(mHandle, nullptr, 1024);
  }

  return anyChanges;
}

void LinuxFileWatcher::close() {
  ::close(mHandle);
}


void LinuxFileWatcher::initialize_notification_handle() {
  mHandle = inotify_init();

  // Add watcher
  if (mPath.is_directory()) {
    // Add watcher recursively here
    detail::create_watcher_recursively(
      mHandle,
      mPath.get_native_path().c_str());
  } else {
    // Just single file watching
    inotify_add_watch(
      mHandle,
      mPath.get_native_path().c_str(),
      IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM | IN_MOVE_SELF | IN_DELETE_SELF);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}