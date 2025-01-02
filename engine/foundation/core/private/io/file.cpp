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
#include "core/io/file.h"
#if defined(LINUX)
#include "core/linux/linux_file.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
File::File()
: mFileImpl(nullptr) {
#if defined(LINUX)
  mFileImpl = new LinuxFile();
#endif
}

File::~File() {
  delete mFileImpl;
}

bool File::is_invalid() const {
  return mFileImpl == nullptr;
}

uint64 File::get_number_of_bytes() {
  return mFileImpl->get_number_of_bytes();
}

bool File::is_eof() const {
  return mFileImpl->is_eof();
}

bool File::flush() {
  return mFileImpl->flush();
}

bool File::open(const String& absoluteFilename, EFileMode fileMode) {
  return mFileImpl->open(absoluteFilename, fileMode);
}

void File::close() {
  mFileImpl->close();
}

uint64 File::tell() const {
  return mFileImpl->tell();
}

void File::seek(uint64 seek) {
  mFileImpl->seek(seek);
}

uint64 File::read(void *destinationBuffer, uint64 numberOfBytes) {
  return mFileImpl->read(destinationBuffer, numberOfBytes);
}

void File::skip(core::uint64 numberOfBytes) {
  mFileImpl->skip(numberOfBytes);
}

uint64 File::write(const void *sourceBuffer, uint64 numberOfBytes) {
  return mFileImpl->write(sourceBuffer, numberOfBytes);
}

int64 File::get_last_modification_time() const {
  return mFileImpl->get_last_modification_time();
}

int64 File::get_file_size() const {
  return mFileImpl->get_file_size();
}


String File::get_content_as_string() {
  const uint32 fileSize = get_number_of_bytes();

  if (fileSize) {
    char* data = new char[fileSize + 1];
    read(data, fileSize);
    data[fileSize] = '\0';

    return String(data);
  }

  return "";
}

String File::get_str() {
  return mFileImpl->get_str();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}