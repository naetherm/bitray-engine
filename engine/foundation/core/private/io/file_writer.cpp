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
#include "core/io/file_writer.h"

#include <core/io/stream.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
FileWriter::FileWriter() {
}

FileWriter::~FileWriter() {
  if (is_open()) {
    close();
  }
}


sizeT FileWriter::get_size() const {
  BE_ASSERT(is_open(), "File stream is not open")

  return mStream->get_size();
}

sizeT FileWriter::get_position() const {
  BE_ASSERT(is_open(), "File stream is not open")

  return mStream->get_position();
}

void FileWriter::seek(sizeT pos, EFileSeek mode) {
  BE_ASSERT(is_open(), "File stream is not open")

  return mStream->seek(pos, mode);
}


bool FileWriter::open() {
  return StreamWriter::open();
}

void FileWriter::close() {
  StreamWriter::close();
}

void FileWriter::write(const void* dstBuffer, sizeT numBytes) {
  mStream->write(dstBuffer, numBytes);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}