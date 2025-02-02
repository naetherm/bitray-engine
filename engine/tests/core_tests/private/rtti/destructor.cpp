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
#include "rtti/destructor.h"
#include <core/rtti/object.h>
#include <core/rtti/func/constructor.h>
#include <core/rtti/func/destructor.h>
#include <core/log/log.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


class DestructorSampleObject {
public:

  DestructorSampleObject() {
    BE_LOG(Info, "Called DestructorSampleObject()")
  }

  DestructorSampleObject(bool b) {
    BE_LOG(Info, "Called DestructorSampleObject(bool)")
  }

  DestructorSampleObject(bool b, core::int8 i) {
    BE_LOG(Info, "Called DestructorSampleObject(bool, core::int8)")
  }

  ~DestructorSampleObject() {
    BE_LOG(Info, "Called ~DestructorSampleObject()")
  }
};


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
DestructorTests::DestructorTests()
  : UnitTest("DestructorTests") {

}

DestructorTests::~DestructorTests() {

}

void DestructorTests::test() {
  core::Destructor d = core::Destructor::create<DestructorSampleObject>();
  // Destructor1
  {
    core::Constructor<DestructorSampleObject> c;

    DestructorSampleObject* p = c.invoke();

    be_expect_true(p != nullptr)
    d.destroy(p);
  }

  // Destructor2
  {
    core::Constructor<DestructorSampleObject, bool> c;

    DestructorSampleObject* p = c.invoke(false);

    be_expect_true(p != nullptr)
    d.destroy(p);
  }

  // Destructor3
  {
    core::Constructor<DestructorSampleObject, bool, core::int8> c;

    DestructorSampleObject* p = c.invoke(true, 42);

    be_expect_true(p != nullptr)
    d.destroy(p);
  }
}

be_unittest_autoregister(DestructorTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests