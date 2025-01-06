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
#include "core/io/compressed_stream.h"
#include "core/io/file.h"
#include "core/log/log.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
CompressedStream::CompressedStream()
:mCompressedFile(0, 4096) {

}

CompressedStream::~CompressedStream() {

}


void CompressedStream::skip(sizeT numOfBytes) {
  mCompressedFile.skip(numOfBytes);
}



bool CompressedStream::can_read() const {
  return true;
}

bool CompressedStream::can_write() const {
  return true;
}

bool CompressedStream::can_seek() const {
  return false;
}

bool CompressedStream::can_be_mapped() const {
  return true;
}

sizeT CompressedStream::get_size() const {
  return mCompressedFile.get_file_size();
}

sizeT CompressedStream::get_position() const {
  return 0;
}


bool CompressedStream::open() {
  // Sanity check
  BE_ASSERT(!is_open(), "There is already an opened file")

  // In here, it depends on what we want
  if (Stream::open()) {
    // Now open the file
    core::File file;
    if (file.open(mPath.get_native_path(), mFileAccessMode)) {

      if (mFileAccessMode == EFileMode::Read) {
        if (mCompressedFile.load_compressed_data_from_file(0, 0, file)) {
          mCompressedFile.decompress();
        }
      }

      return true;
    }
  }


  return false;
}

void CompressedStream::close() {
  if (mCompressedFile.get_byte_vector().size() != 0) {
    if (mFileAccessMode == EFileMode::Write) {
      // write to file first
      core::File file;
      if (file.open(mPath.get_native_path(), mFileAccessMode)) {
        mCompressedFile.write_compressed_data_to_file(0, 0, file);
      }
    }
    mCompressedFile.close();
  } else {
    BE_LOG(Warning, "There is no information in the compressed file to write")
  }

  Stream::close();
}

void CompressedStream::write(const void* ptr, sizeT numBytes) {
  mCompressedFile.write(ptr, numBytes);
}

sizeT CompressedStream::read(void* ptr, sizeT numBytes) {
  return mCompressedFile.read(ptr, numBytes);
}

void CompressedStream::seek(sizeT offset, EFileSeek seek) {
  mCompressedFile.skip(offset);
}

void CompressedStream::flush() {
  mCompressedFile.flush();
}

bool CompressedStream::eof() const {
  return mCompressedFile.is_eof();
}

String CompressedStream::get_content_as_string() {
  String result;
  result.resize(get_size());

  read(result.data(), get_size());

  return result;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}