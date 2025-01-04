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
#include "core/io/stream.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Stream::Stream()
: mFileAccessMode(EFileMode::Read)
, mAccessPattern(EAccessPattern::Sequential)
, mIsOpen(false)
, mIsMapped(false) {

}

Stream::~Stream() {
  BE_ASSERT(!is_open(), "The stream is still open")
  BE_ASSERT(!is_mapped(), "The stream is still mapped")
}


void Stream::set_path(const core::Path &path) {
  BE_ASSERT(!is_open(), "The currently set path is already opened")
  mPath = path;
}

const Path& Stream::get_path() const {
  return mPath;
}

void Stream::set_access_mode(core::EFileMode fileMode) {
  mFileAccessMode = fileMode;
}

EFileMode Stream::get_access_mode() const {
  return mFileAccessMode;
}


bool Stream::is_open() const {
  return mIsOpen;
}

bool Stream::is_mapped() const {
  return mIsMapped;
}


bool Stream::can_read() const {
  return false;
}

bool Stream::can_write() const {
  return false;
}

bool Stream::can_seek() const {
  return false;
}

bool Stream::can_be_mapped() const {
  return false;
}

sizeT Stream::get_size() const {
  return 0;
}

sizeT Stream::get_position() const {
  return 0;
}


bool Stream::open() {
  BE_ASSERT(!is_open(), "There is already an opened stream")
  mIsOpen = true;
  return mIsOpen;
}

void Stream::close() {
  BE_ASSERT(is_open(), "There is no opened stream to close")
  mIsOpen = false;
}

void Stream::write(const void *ptr, sizeT numBytes) {

}

sizeT Stream::read(void *ptr, sizeT numBytes) {
  return 0;
}

void Stream::seek(sizeT offset, EFileSeek seek) {

}

void Stream::flush() {

}

bool Stream::eof() const {
  return true;
}

String Stream::get_content_as_string() {
  return String();
}


void *Stream::map() {
  return nullptr;
}

void Stream::unmap() {

}

void *Stream::memory_map() {
  return nullptr;
}

void Stream::memory_unmap() {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}