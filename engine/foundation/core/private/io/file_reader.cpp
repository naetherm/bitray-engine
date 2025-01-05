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
#include "core/io/file_reader.h"

#include <core/io/stream.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
FileReader::FileReader() {
}

FileReader::~FileReader() {
  if (is_open()) {
    close();
  }
}


sizeT FileReader::get_size() const {
  if (is_open()) {
    return mStream->get_size();
  } else {
    BE_ASSERT(false, "File reader is not open");
  }

  return 0;
}

sizeT FileReader::get_position() const {
  BE_ASSERT(is_open(), "File reader is not open");

  return mStream->get_position();
}

void FileReader::seek(sizeT pos, EFileSeek mode) {
  BE_ASSERT(is_open(), "File reader is not open")

  return mStream->seek(pos, mode);
}


bool FileReader::open() {
  return StreamReader::open();
}

void FileReader::close() {
  StreamReader::close();
}

sizeT FileReader::read(void* dstBuffer, sizeT numBytes) {
  BE_ASSERT(is_open(), "File reader is not open")
  BE_ASSERT(numBytes > 0, "Number of bytes to read is less than zero")
  return mStream->read(dstBuffer, numBytes);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}