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
#include "io/textreaderwriter.h"
#include <core/io/file_stream.h>
#include <core/io/text_reader.h>
#include <core/io/text_writer.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
TextReaderWriterTests::TextReaderWriterTests()
  : UnitTest("TextReaderWriterTests") {

}

TextReaderWriterTests::~TextReaderWriterTests() {

}

void TextReaderWriterTests::test() {
  {
    core::Ptr<core::FileStream> fs(new core::FileStream());
    fs->set_path(core::Path("./sample_data/sample_text.txt"));
    core::TextReader tr;
    tr.set_stream(fs);

    bool canOpen = tr.open();
    be_expect_true(canOpen)

    if (canOpen) {
      core::String firstLine = tr.read_line();
      printf("The first line is: %s\n", firstLine.c_str());
      core::String secondLine = tr.read_line();
      printf("The second line is: %s\n", secondLine.c_str());
      char singleChar = tr.read_char();
      printf("The char is: %c\n", singleChar);
    }

    tr.close();
  }

  {
    core::Ptr<core::FileStream> fs(new core::FileStream());
    fs->set_path(core::Path("./sample_data/new_text.txt"));
    fs->set_access_mode(core::EFileMode::Write);
    core::TextWriter tr;
    tr.set_stream(fs);

    bool canOpen = tr.open();
    be_expect_true(canOpen)

    if (canOpen) {
      core::String firstLine = "This is the first line to write";
      core::String firstString = "The string to write";
      char singleChar = '!';

      tr.write_line(firstLine);
      tr.write_string(firstString);
      tr.write_char(singleChar);
    }

    tr.close();
  }
}

be_unittest_autoregister(TextReaderWriterTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests