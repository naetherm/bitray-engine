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
#include "core/io/stream_writer.h"
#include "core/io/stream.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
StreamWriter::StreamWriter()
: mIsOpen(false)
, mStreamWasOpen(false) {

}

StreamWriter::~StreamWriter() {
  BE_ASSERT(!is_open(), "Stream is still opened")
}

void StreamWriter::set_stream(const Ptr<Stream> &stream) {
  mStream = stream;
}

const Ptr<Stream> &StreamWriter::get_stream() const {
  return mStream;
}

Ptr<Stream> &StreamWriter::get_stream() {
  return mStream;
}

bool StreamWriter::has_stream() const {
  return mStream.is_valid();
}

bool StreamWriter::is_eof() const {
  return mStream->eof();
}

bool StreamWriter::is_open() const {
  return mIsOpen;
}

bool StreamWriter::open() {

  if (mStream->is_open()) {
    mStreamWasOpen = true;
    mIsOpen = true;
  } else {
    mStreamWasOpen = false;
    mStream->set_access_mode(EFileMode::Write);
    mIsOpen = mStream->open();
  }

  return mIsOpen;
}

void StreamWriter::close() {
  BE_ASSERT(mIsOpen, "Trying to close an already closed stream")

  if ((!mStreamWasOpen) && mStream->is_open()) {
    mStream->close();
  }

  mIsOpen = false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}