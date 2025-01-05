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
#include "core/io/file_stream.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
FileStream::FileStream()
: mMappedContent(nullptr) {

}

FileStream::~FileStream() {
  if (is_open()) {
    close();
  }
}


String FileStream::get_content_as_string() {
  return mFileHandle.get_content_as_string();
}


bool FileStream::can_read() const {
  return true;
}

bool FileStream::can_write() const {
  return true;
}

bool FileStream::can_seek() const {
  return true;
}

bool FileStream::can_be_mapped() const {
  return true;
}

sizeT FileStream::get_size() const {
  return mFileHandle.get_file_size();
}

sizeT FileStream::get_position() const {
  return mFileHandle.tell();
}


bool FileStream::open() {
  // Sanity check
  BE_ASSERT(!is_open(), "There is already an opened file")
  // Call the base class
  if (Stream::open()) {
    return mFileHandle.open(mPath.get_native_path(), mFileAccessMode);
  }

  Stream::close();
  return false;
}

void FileStream::close() {
  // Sanity check
  BE_ASSERT(is_open(), "There is already a closed file")

  if (is_mapped()) {
    if (!mMapFileHandle.is_invalid()) {
      unmap();
    } else {
      memory_unmap();
    }
  }

  mFileHandle.close();
  Stream::close();
}

void FileStream::write(const void* ptr, sizeT numBytes) {
  BE_ASSERT(!is_mapped(), "The file is mapped")
  if (numBytes > 0) {
    BE_ASSERT(is_open(), "There is no file opened")
    BE_ASSERT(ptr, "The pointer is null")
    mFileHandle.write(ptr, numBytes);
  }
}

sizeT FileStream::read(void* ptr, sizeT numBytes) {
  BE_ASSERT(is_open(), "There is no file opened")
  BE_ASSERT(ptr, "The pointer is null")
  BE_ASSERT(numBytes > 0, "The number of bytes is zero")
  return mFileHandle.read(ptr, numBytes);
}

void FileStream::seek(sizeT offset, EFileSeek seek) {
  mFileHandle.seek(offset);
}

void FileStream::flush() {
  mFileHandle.flush();
}

bool FileStream::eof() const {
  return mFileHandle.is_eof();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}