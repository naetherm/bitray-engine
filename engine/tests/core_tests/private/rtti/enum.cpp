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
#include "rtti/enum.h"
#include <core/rtti/rtti_type_server.h>


be_begin_enum(SomeEnum, core_tests)
.value("FirstValue", core_tests::SomeEnum::FirstValue)
.value("SecondValue", core_tests::SomeEnum::SecondValue)
.value("ThirdValue", core_tests::SomeEnum::ThirdValue)
be_end_enum()


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
EnumTests::EnumTests()
  : UnitTest("EnumTests") {

}

EnumTests::~EnumTests() {

}

void EnumTests::test() {
  core::RttiTypeServer& rtti = core::RttiTypeServer::instance();
  //core::StaticTypeInfo<core_tests::SomeEnum>::get();

  {
    core::EnumTypeInfo* enumTypeInfo = rtti.get_enum_type("core_tests::SomeEnum");
    const core::Enum* enm = enumTypeInfo->get_enum();

    be_expect_true(enumTypeInfo != nullptr)
    be_expect_true(enumTypeInfo->get_enum() != nullptr)
    be_expect_eq(3, enm->get_num_of_enum_values())
    be_expect_str_eq("FirstValue", enm->get_enum_value_by_index(0).get_name().c_str())
    be_expect_str_eq("SecondValue", enm->get_enum_value_by_index(1).get_name().c_str())
    be_expect_str_eq("ThirdValue", enm->get_enum_value_by_index(2).get_name().c_str())
    be_expect_eq(core_tests::SomeEnum::FirstValue, enm->get_enum_value_by_name("FirstValue").get_value())
    be_expect_eq(core_tests::SomeEnum::SecondValue, enm->get_enum_value_by_name("SecondValue").get_value())
    be_expect_eq(core_tests::SomeEnum::ThirdValue, enm->get_enum_value_by_name("ThirdValue").get_value())
  }
}

be_unittest_autoregister(EnumTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests