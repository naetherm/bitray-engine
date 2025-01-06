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
#include "core/io/binary_writer.h"
#include "core/io/stream.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
BinaryWriter::BinaryWriter()
: mMapCursor(nullptr)
, mMapEnd(nullptr)
, mEnableMapping(false)
, mIsMapped(false) {

}

BinaryWriter::~BinaryWriter() {
  if (is_open()) {
    close();
  }
}

bool BinaryWriter::open() {
  if (StreamWriter::open()) {
    if (mEnableMapping && mStream->can_be_mapped()) {
      mIsMapped = true;
      mMapCursor = (unsigned char*)mStream->map();
      mMapEnd = mMapCursor + mStream->get_size();
    } else {
      mIsMapped = false;
      mMapCursor = nullptr;
      mMapEnd = nullptr;
    }
    // Done
    return true;
  }
  // Done
  return false;
}

void BinaryWriter::close() {
  StreamWriter::close();
  mIsMapped = false;
  mMapCursor = nullptr;
  mMapEnd = nullptr;
}


void BinaryWriter::write_char(char c) {
  if (mIsMapped) {
    BE_ASSERT(mMapCursor < mMapEnd, "BinaryWriter::write_char: map cursor out of bounds")
    *mMapCursor++ = c;
  } else {
    mStream->write(&c, sizeof(c));
  }
}

void BinaryWriter::write_short(short s) {
  if (mIsMapped) {
    BE_ASSERT(mMapCursor < mMapEnd, "BinaryWriter::write_char: map cursor out of bounds")
    Memory::copy(mMapCursor, &s, sizeof(s));
  } else {
    mStream->write(&s, sizeof(s));
  }
}

void BinaryWriter::write_int(int i) {
  if (mIsMapped) {
    BE_ASSERT(mMapCursor < mMapEnd, "BinaryWriter::write_char: map cursor out of bounds")
    Memory::copy(mMapCursor, &i, sizeof(i));
  } else {
    mStream->write(&i, sizeof(i));
  }
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}