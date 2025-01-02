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
#include "core/linux/linux_file.h"
#include <fcntl.h>
#include <sys/stat.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxFile::LinuxFile()
: mFile(nullptr) {

}

LinuxFile::~LinuxFile() {
  close();
}

uint64 LinuxFile::get_number_of_bytes() {
  if (mFile) {
    const int curr_pos = ftell(mFile);
    fseek(mFile, 0, SEEK_END);
    const int32 size = ftell(mFile);
    fseek(mFile, curr_pos, SEEK_SET);

    return size;
  }
  return 0;
}

bool LinuxFile::is_eof() const {
  return (mFile && feof(mFile));
}

bool LinuxFile::flush() {
  return mFile && fflush(mFile);
}

bool LinuxFile::open(const String& absoluteFilename, EFileMode fileMode) {
  if (mFile) {
    close();
  }

  mAccess = fileMode;
  mAbsoluteFilename = absoluteFilename;

  // Directly open the file
  mFile = ::fopen(mAbsoluteFilename.c_str(), mAccess == EFileMode::Read ? "r" : "w");

  return (mFile != nullptr);
}

void LinuxFile::close() {
  if (mFile) {
    ::fclose(mFile);

    mFile = nullptr;
  }
}

uint64 LinuxFile::tell() const {
  return mFile ? ftell(mFile) : -1;
}

void LinuxFile::seek(uint64 seek) {
  ::fseek(mFile, seek, SEEK_SET);
}

uint64 LinuxFile::read(void *destinationBuffer, uint64 numberOfBytes) {
  if (mFile) {
    return ::fread(destinationBuffer, numberOfBytes, 1, mFile);
  }
  return 0;
}

void LinuxFile::skip(uint64 numberOfBytes) {

}

uint64 LinuxFile::write(const void *sourceBuffer, uint64 numberOfBytes) {
  if (mFile) {
    return ::fwrite(sourceBuffer, numberOfBytes, 1, mFile);
  }
  return 0;
}

int64 LinuxFile::get_last_modification_time() const {
  struct stat fileStat;

  if (stat(mAbsoluteFilename.c_str(), &fileStat) == 0) {
    return fileStat.st_mtim.tv_sec;
  }

  return 0;
}

int64 LinuxFile::get_file_size() const {
  struct stat fileStat;

  if (stat(mAbsoluteFilename.c_str(), &fileStat) == 0) {
    return fileStat.st_size;
  }

  return 0;
}

String LinuxFile::get_str() {
  if (mAccess == EFileMode::Read) {
    char temp[2048] = "";

    if (fgets(temp, 2048, mFile)) {
      return temp;
    }
  }
  return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}