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
#include "unittest/unittest.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace unittest {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
UnitTest::UnitTest(const char* name)
: mName(name) {

}

UnitTest::~UnitTest() {

}

const char* UnitTest::name() {
  return mName;
}

UnitTestRegistry &UnitTestRegistry::instance() {
  static UnitTestRegistry SInstance;
  return SInstance;
}


UnitTestRegistry::~UnitTestRegistry() {
  clear();
}

void UnitTestRegistry::runAll() {
  printf("\033[32;1m= Starting Unit Tests =\033[0m\n");
  for(auto ut: mlstUnitTests) {
    printf("\033[32;1m=== Running '%s' ===\033[0m\n", ut->name());
    ut->test();
  }

  clear();
}

void UnitTestRegistry::clear() {
  for (UnitTest* ut: mlstUnitTests) {
    re_delete(ut);
  }
  mlstUnitTests.clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core