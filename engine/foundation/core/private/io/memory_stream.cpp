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
#include "core/io/memory_stream.h"
#include "core/math/math.h"
#include "core/memory/memory.h"
#include "core/std/math.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
MemoryStream::MemoryStream()
: mCapacity(0)
, mSize(0)
, mPosition(0)
, mBuffer(nullptr) {

}

MemoryStream::~MemoryStream() {
  if (is_open()) {
    close();
  }

  // Release memory buffer if still allocated
  if (mBuffer) {
    Memory::free(mBuffer);
    mBuffer = nullptr;
  }
}

void *MemoryStream::get_raw_pointer() const {
  BE_ASSERT(mBuffer != 0, "There is no memory allocated")

  return mBuffer;
}

bool MemoryStream::can_read() const {
  return true;
}

bool MemoryStream::can_write() const {
  return true;
}

bool MemoryStream::can_seek() const {
  return true;
}

bool MemoryStream::can_be_mapped() const {
  return true;
}

sizeT MemoryStream::get_size() const {
  return mSize;
}

sizeT MemoryStream::get_position() const {
  return mPosition;
}

bool MemoryStream::open() {
  BE_ASSERT(!is_open(), "Memory Stream already open")

  if (Stream::open()) {
    if (mFileAccessMode == EFileMode::Write) {
      mPosition = 0;
    } else if (mFileAccessMode == EFileMode::Append) {
      mPosition = mSize;
    } else {
      mPosition = 0;
    }
    return true;
  }

  return false;
}

void MemoryStream::close() {
  BE_ASSERT(is_open(), "No need to close, was never opened")

  if (Stream::is_mapped()) {
    unmap();
  }

  Stream::close();
}

void MemoryStream::write(const void *ptr, sizeT numBytes) {
  // Sanity check
  BE_ASSERT(is_open(), "No need to write, was never opened")
  BE_ASSERT(!is_mapped(), "Memory stream already mapped")
  BE_ASSERT(mFileAccessMode == EFileMode::Write, "Memory stream already write")
  BE_ASSERT(mPosition >= 0 && mPosition <= mSize, "Memory stream is out of bounds")

  if (!has_space(numBytes)) {
    make_space(numBytes);
  }

  // Write data to stream
  BE_ASSERT(mPosition + numBytes <= mCapacity, "Memory stream is out of bounds")
  Memory::copy(mBuffer + mPosition, ptr, numBytes);
  mPosition += numBytes;
  if (mPosition > mSize) {
    mSize = mPosition;
  }
}

sizeT MemoryStream::read(void *ptr, sizeT numBytes) {
  // Sanity check
  BE_ASSERT(is_open(), "No need to read, was never opened")
  BE_ASSERT(!is_mapped(), "Memory stream already mapped")
  BE_ASSERT(mFileAccessMode == EFileMode::Read, "Memory stream already mapped")
  BE_ASSERT(mPosition >= 0 && mPosition <= mSize, "Memory stream already mapped")

  sizeT readBytes = numBytes <= mSize - mPosition ? numBytes : mSize - mPosition;

  BE_ASSERT(mPosition + readBytes <= mSize, "Memory stream read out of bounds")

  if (readBytes > 0) {
    Memory::copy(mBuffer + mPosition, ptr, readBytes);
    mPosition += readBytes;
  }
  return readBytes;
}

void MemoryStream::seek(sizeT offset, EFileSeek seek) {
  switch (seek)
  {
    case EFileSeek::Set:
      mPosition = offset;
      break;
    case EFileSeek::Current:
      mPosition += offset;
      break;
    case EFileSeek::End:
      mPosition = mSize + offset;
      break;
  }

  // make sure read/write position doesn't become invalid
  mPosition = Math::clamp(mPosition, (sizeT)(0), mSize);
}

void MemoryStream::flush() {
  Stream::flush();
}

bool MemoryStream::eof() const {
  return (mPosition == mSize);
}

String MemoryStream::get_content_as_string() {
  return Stream::get_content_as_string();
}

void *MemoryStream::map() {
  Stream::map();

  return mBuffer;
}

void MemoryStream::unmap() {
  Stream::unmap();
}

void *MemoryStream::memory_map() {
  return map();
}

void MemoryStream::memory_unmap() {
  unmap();
}

void MemoryStream::realloc(sizeT newCapacity) {
  unsigned char* newBuffer = (unsigned char*)Memory::allocate(newCapacity);
  unsigned char* endOfNewBuffer = newBuffer + newCapacity;

  BE_ASSERT(nullptr != newBuffer, "Memory stream realloc failed")

  sizeT newSize = newCapacity < mSize ? newCapacity : mSize;
  if (nullptr != mBuffer) {
    BE_ASSERT(newBuffer + newSize < endOfNewBuffer, "Memory stream realloc failed")

    Memory::copy(newBuffer, mBuffer, newSize);
    Memory::free(mBuffer);
  }
  mBuffer = newBuffer;
  mSize = newSize;
  mCapacity = newCapacity;
  if (mPosition > mSize) {
    mPosition = mSize;
  }
}

bool MemoryStream::has_space(sizeT numBytes) const {
  return ((mPosition + numBytes) <= mCapacity);
}

void MemoryStream::make_space(sizeT numBytes) {
  // Sanity check
  BE_ASSERT(numBytes > 0, "Memory stream is out of bounds")
  BE_ASSERT(mSize + numBytes > mCapacity, "Memory stream is out of bounds")

  // Compute new capacity
  sizeT oneDotFiveCurrentSize = mCapacity + (mCapacity >> 1);
  sizeT newCapacity = mSize + numBytes;

  if (oneDotFiveCurrentSize > newCapacity) {
    newCapacity = oneDotFiveCurrentSize;
  }
  if (16 > newCapacity) {
    newCapacity = 16;
  }

  BE_ASSERT(newCapacity > mCapacity, "Memory stream is out of bounds")

  realloc(newCapacity);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}