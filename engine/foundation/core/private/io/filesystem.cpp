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
#include "core/io/filesystem.h"
#if defined(LINUX)
#include "core/linux/linux_filesystem.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Filesystem::Filesystem() {
#if defined(LINUX)
  mFilesystemImpl = new LinuxFilesystem();
#elif defined(WINDOWS)
#endif
}

Filesystem::~Filesystem() {
  delete mFilesystemImpl;
}

void Filesystem::enumerate_files(const String &directoryName, const String &realDirectoryName,
                                 EEnumerationMode enumerationMode, Vector<String> &filenames, bool recursive,
                                 bool returnAsRelativePaths) const {
  return mFilesystemImpl->enumerate_files(directoryName, realDirectoryName, enumerationMode, filenames, recursive, returnAsRelativePaths);
}

bool Filesystem::is_directory(const String &directory) const {
  return mFilesystemImpl->is_directory(directory);
}

bool Filesystem::create_directory(const String &directory) const {
  return mFilesystemImpl->create_directory(directory);
}

bool Filesystem::delete_directory(const String &directory) const {
  return mFilesystemImpl->delete_directory(directory);
}

bool Filesystem::directory_exists(const String &virtualFilename) const {
  return mFilesystemImpl->directory_exists(virtualFilename);
}

bool Filesystem::is_file(const String &directory) const {
  return mFilesystemImpl->is_file(directory);
}

bool Filesystem::file_exists(const String &virtualFilename) const {
  return mFilesystemImpl->file_exists(virtualFilename);
}

FileImpl *Filesystem::open_file(EFileMode fileMode, const String &filename) const {
  return mFilesystemImpl->open_file(fileMode, filename);
}

void Filesystem::close_file(FileImpl &file) const {
  mFilesystemImpl->close_file(file);
}

core::int64 Filesystem::get_last_modification_time(const String &filename) const {
  return mFilesystemImpl->get_last_modification_time(filename);
}

core::int64 Filesystem::get_file_size(const String &filename) const {
  return mFilesystemImpl->get_file_size(filename);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}