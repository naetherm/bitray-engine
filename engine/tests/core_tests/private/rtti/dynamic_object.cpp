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
#include "rtti/dynamic_object.h"
#include <core/rtti/object.h>
#include <core/rtti/tools/dynamic_object.h>
#include <core/log/log.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
DynamicObjectTests::DynamicObjectTests()
  : UnitTest("DynamicObjectTests") {

}

DynamicObjectTests::~DynamicObjectTests() {

}

void DynamicObjectTests::test() {
  {
    core::DynamicObject v;

    be_expect_true(nullptr == v.get_type_info())
  }

  {
    core::int32 i = 5;
    core::DynamicObject v(i);

    be_expect_eq(i, v.get_as<core::uint32>())
    be_expect_true(v.get_type_info() != nullptr)
    be_expect_true(core::get_static_type_info<core::int32>() == v.get_type_info())
    be_expect_str_eq(core::get_static_type_info<core::int32>()->get_name().c_str(), v.get_type_info()->get_name().c_str())
  }

  {
    core::int32 i = 5;
    core::DynamicObject v;

    v.set(i);

    be_expect_eq(i, v.get_as<core::uint32>())
    be_expect_true(v.get_type_info() != nullptr)
    be_expect_true(core::get_static_type_info<core::int32>() == v.get_type_info())
    be_expect_str_eq(core::get_static_type_info<core::int32>()->get_name().c_str(), v.get_type_info()->get_name().c_str())
  }

  {
    core::DynamicObject v1(5);
    core::DynamicObject v2(10);

    be_expect_false(v1 == v2)
  }
}

be_unittest_autoregister(DynamicObjectTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests