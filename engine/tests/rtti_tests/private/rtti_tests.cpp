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
#include <core/rtti/rtti.h>
#include "rtti_tests.h"

#include <core/log/log.h>
#include <core/rtti/type/enum_type_info.h>
#include <core/rtti/type/type_info.h>

#include "some_object.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
RttiTests::RttiTests()
: UnitTest("RttiTests") {

}

RttiTests::~RttiTests() {

}

void RttiTests::test() {
  core::RttiTypeServer& rttiTypeServer = core::RttiTypeServer::instance();
  {
    be_expect_str_eq("Object", rtti_tests::Object::get_static_type_info()->get_name().c_str())
    be_expect_str_eq("rtti_tests::SomeObject", rtti_tests::SomeObject::get_static_type_info()->get_name().c_str())
  }

  // rtti_tests::SomeEnum tests
  {
    {
      core::EnumTypeInfo* someEnumTypeInfo = rttiTypeServer.get_enum("rtti_tests::SomeEnum");

      be_expect_str_eq("rtti_tests::SomeEnum", someEnumTypeInfo->get_name().c_str())
      be_expect_eq(rtti_tests::SomeEnum::FirstValue, someEnumTypeInfo->get_value("FirstValue"))
      be_expect_eq(rtti_tests::SomeEnum::SecondValue, someEnumTypeInfo->get_value("SecondValue"))
      be_expect_eq(rtti_tests::SomeEnum::ThirdValue, someEnumTypeInfo->get_value("ThirdValue"))

      someEnumTypeInfo = nullptr;
    }
  }
}

be_unittest_autoregister(RttiTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests