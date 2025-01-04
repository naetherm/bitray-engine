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
#include "core/linux/linux_filesystem.h"
#include "core/io/types.h"
#include "core/io/filesystem_helper.h"
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
void LinuxFilesystem::enumerate_files(const String &directoryName, const String &realDirectoryName,
                                      EEnumerationMode enumerationMode, Vector<String> &filenames, bool recursive,
                                      bool returnAsRelativePaths) const {
  DIR* directory;
  struct dirent *dirEntry;
  if ((directory = opendir(realDirectoryName.c_str())) != nullptr) {
    /* print all the files and directories within directory */
    while ((dirEntry = readdir (directory)) != nullptr) {
      // Add all directories
      if ((dirEntry->d_type == DT_DIR) &&
          (enumerationMode == EEnumerationMode::All or enumerationMode == EEnumerationMode::Directories)) {
        // Recursive call
        if (dirEntry->d_name[0] != '.') {
          if (returnAsRelativePaths) {
            filenames.push_back(FilesystemHelper::normalize(dirEntry->d_name));
          } else {
            filenames.push_back(FilesystemHelper::normalize(directoryName + "/" + dirEntry->d_name));
          }
        }
        // Add all files
      } else if ((dirEntry->d_type == DT_REG) &&
                 (enumerationMode == EEnumerationMode::All or enumerationMode == EEnumerationMode::Files)) {
        // Add file
        if (returnAsRelativePaths) {
          filenames.push_back(FilesystemHelper::normalize(dirEntry->d_name));
        } else {
          filenames.push_back(FilesystemHelper::normalize(directoryName + "/" + dirEntry->d_name));
        }
      }
      // This is recursive, so iterate through all elements
      if (recursive && (dirEntry->d_type == DT_DIR) && (dirEntry->d_name[strlen(dirEntry->d_name)-1] != '.')) {
        enumerate_files(
          FilesystemHelper::normalize(directoryName + "/" + dirEntry->d_name),
          FilesystemHelper::normalize(realDirectoryName + "/" + dirEntry->d_name),
          enumerationMode,
          filenames,
          recursive,
          returnAsRelativePaths);
      }
    }
    closedir (directory);
  }
}

bool LinuxFilesystem::is_directory(const String &directory) const {
  struct stat fileStat;

  if (stat(directory.c_str(), &fileStat) == 0) {
    return ((fileStat.st_mode & S_IFDIR) != 0);
  }

  return false;
}

bool LinuxFilesystem::create_directory(const String &directory) const {
  return (mkdir(directory.c_str(), 0711) != -1);
}

bool LinuxFilesystem::delete_directory(const String &directory) const {
  return (rmdir(directory.c_str()) != 0);
}

bool LinuxFilesystem::directory_exists(const String &directory) const {
  struct stat fileStat;

  return (stat(directory.c_str(), &fileStat) == 0);
}

bool LinuxFilesystem::is_file(const String &directory) const {
  struct stat fileStat;

  if (stat(directory.c_str(), &fileStat) == 0) {
    return ((fileStat.st_mode & S_IFREG) != 0);
  }

  return false;
}

bool LinuxFilesystem::file_exists(const String &filename) const {
  struct stat fileStat;

  return (stat(filename.c_str(), &fileStat) == 0);
}

FileImpl *LinuxFilesystem::open_file(EFileMode fileMode, const String &filename) const {
  return nullptr;
}

void LinuxFilesystem::close_file(FileImpl &file) const {

}

core::int64 LinuxFilesystem::get_last_modification_time(const String &filename) const {
  struct stat fileStat;

  if (stat(filename.c_str(), &fileStat) == 0) {
    return fileStat.st_mtim.tv_sec;
  }

  return 0;
}

core::int64 LinuxFilesystem::get_file_size(const String &filename) const {
  struct stat fileStat;

  if (stat(filename.c_str(), &fileStat) == 0) {
    return fileStat.st_size;
  }

  return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core