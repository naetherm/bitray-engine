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
#include "rtti/constructor.h"
#include <core/rtti/object.h>
#include <core/rtti/func/constructor.h>
#include <core/log/log.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


class ConstructorSampleObject {
public:

  ConstructorSampleObject() {
    BE_LOG(Info, "Called ConstructorSampleObject()")
  }

  ConstructorSampleObject(bool b) {
    BE_LOG(Info, "Called ConstructorSampleObject(bool)")
  }

  ConstructorSampleObject(bool b, core::int8 i) {
    BE_LOG(Info, "Called ConstructorSampleObject(bool, core::int8)")
  }
};


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ConstructorTests::ConstructorTests()
  : UnitTest("ConstructorTests") {

}

ConstructorTests::~ConstructorTests() {

}

void ConstructorTests::test() {
  // Constructor1
  {
    core::Constructor<ConstructorSampleObject> c;

    ConstructorSampleObject* p = c.invoke();

    be_expect_true(p != nullptr)
    delete p;
  }

  // Constructor2
  {
    core::Constructor<ConstructorSampleObject, bool> c;

    ConstructorSampleObject* p = c.invoke(false);

    be_expect_true(p != nullptr)
    delete p;
  }

  // Constructor3
  {
    core::Constructor<ConstructorSampleObject, bool, core::int8> c;

    ConstructorSampleObject* p = c.invoke(true, 42);

    be_expect_true(p != nullptr)
    delete p;
  }
}

be_unittest_autoregister(ConstructorTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests