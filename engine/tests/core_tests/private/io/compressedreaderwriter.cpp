////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2023 RacoonStudios
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
#include "io/compressedreaderwriter.h"
#include <core/io/compressed_reader.h>
#include <core/io/compressed_writer.h>
#include <core/io/compressed_stream.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
CompressedReaderWriterTests::CompressedReaderWriterTests()
  : UnitTest("CompressedReaderWriterTests") {

}

CompressedReaderWriterTests::~CompressedReaderWriterTests() {

}

void CompressedReaderWriterTests::test() {
  // First write something
  core::String stuffToWrite = "This is some random text to write to the stream.";
  core::String stuffToRead;
  {
    core::Ptr<core::CompressedStream> cs(new core::CompressedStream());
    cs->set_path(core::Path("./sample_data/compressed_data.shader_piece"));
    cs->set_access_mode(core::EFileMode::Write);
    core::CompressedWriter cw;
    cw.set_stream(cs);

    bool canOpen = cw.open();
    be_expect_true(canOpen)

    if (canOpen) {
      cw.write(stuffToWrite.data(), stuffToWrite.length());

      cw.close();
    }
  }

  {
    core::Ptr<core::CompressedStream> cs(new core::CompressedStream());
    cs->set_path(core::Path("./sample_data/compressed_data.shader_piece"));
    cs->set_access_mode(core::EFileMode::Read);
    core::CompressedReader cw;
    cw.set_stream(cs);

    bool canOpen = cw.open();
    be_expect_true(canOpen)

    if (canOpen) {
      core::String stuffToWrite = "This is some random text to write to the stream.";
      stuffToRead.resize(stuffToWrite.length());
      cw.read(stuffToRead.data(), stuffToWrite.length());

      printf("Read: %s\n", stuffToRead.c_str());
      be_expect_str_eq(stuffToWrite.c_str(), stuffToRead.c_str())

      cw.close();
    }
  }
}

be_unittest_autoregister(CompressedReaderWriterTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests