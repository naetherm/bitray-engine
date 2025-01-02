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
#include "string/string_view.h"
#include <core/string/string_view.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
StringViewTests::StringViewTests()
: UnitTest("core::StringViewests") {

}

StringViewTests::~StringViewTests() {

}

void StringViewTests::test() {
  // default constructor
  {
    core::StringView sv;

    be_expect(sv.length() == 0)
    be_expect(sv.size() == 0)
    be_expect_true(sv.empty())
    be_expect(sv.data() == nullptr)
  }

  {
    {
      core::StringView sw(nullptr);
      be_expect(sw.empty())
      be_expect(sw.data() == nullptr)
      be_expect(sw.size() == 0)
      be_expect(sw.size() == sw.length())
    }
    {
      core::StringView sw(0);
      be_expect(sw.empty())
      be_expect(sw.data() == nullptr)
      be_expect(sw.size() == 0)
      be_expect(sw.size() == sw.length())
    }
    {
      core::StringView sw(NULL);
      be_expect(sw.empty())
      be_expect(sw.data() == nullptr)
      be_expect(sw.size() == 0)
      be_expect(sw.size() == sw.length())
    }
  }

  {
    const char* pLiteral = "Hello, World";
    core::StringView sw1(pLiteral);
    core::StringView sw2(sw1);
    be_expect(sw1.size() == sw2.size())
    be_expect_str_eq(sw1.data(), sw2.data())
  }

  {
    {
      core::StringView sw("Hello, World", 12);
      be_expect(!sw.empty());
      be_expect(sw.data() != nullptr);
      be_expect(sw.size() == 12);
      be_expect(sw.size() == sw.length());
    }

    {
      core::StringView sw("Hello, World", 5);
      be_expect(!sw.empty());
      be_expect(sw.data() != nullptr);
      be_expect(sw.size() == 5);
      be_expect(sw.size() == sw.length());
      be_expect(sw.compare(0, 5, sw.data()) == 0)
    }
  }

  {
    core::StringView sw("abcdefg");
    {
      auto i = sw.begin();
      auto ci = sw.cbegin();

      be_expect(*i++ == 'a');
      be_expect(*i++ == 'b');

      be_expect(*ci++ == 'a');
      be_expect(*ci++ == 'b');
    }

    {
      auto i = sw.end();
      auto ci = sw.cend();

      be_expect(*i-- == '\0');
      be_expect(*i-- == 'g');

      be_expect(*ci-- == '\0');
      be_expect(*ci-- == 'g');
    }
  }

  {
    {
      core::StringView sw("Vancouver, Canada");
      be_expect(sw.front() == 'V');
      be_expect(sw.back() == 'a');

    }
    {
      core::StringView sw("Canada");
      be_expect(sw.front() == 'C');
      be_expect(sw.back() == 'a');
    }
  }

  {
    core::StringView sw("Vancouver");
    be_expect(sw[0] == 'V');
    be_expect(sw[1] == 'a');
    be_expect(sw[2] == 'n');
    be_expect(sw[3] == 'c');
    be_expect(sw[4] == 'o');
    be_expect(sw[5] == 'u');
    be_expect(sw[6] == 'v');
    be_expect(sw[7] == 'e');
    be_expect(sw[8] == 'r');
  }
}

be_unittest_autoregister(StringViewTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests