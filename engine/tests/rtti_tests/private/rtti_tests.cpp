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
#include <core/log/log.h>
#include <core/rtti/object.h>
#include "rtti_tests.h"
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
    core::TypeInfo* ti = core::StaticTypeInfo<void>::get();

    be_expect_str_eq("void", ti->get_name().c_str());
  }

  core::Object obj;

  {
    core::ClassTypeInfo* cls = rttiTypeServer.get_class_type("core::Object");
    be_expect_true(cls != nullptr);
    be_expect_str_eq("core::Object", cls->get_name().c_str());
    //be_expect_true(cls->get_class()->has_default_constructor());
    be_expect_eq(1, cls->get_class()->get_num_methods());
    be_expect_eq(1, cls->get_class()->get_num_properties());
    be_expect_eq(0, cls->get_class()->get_num_fields());

    {
      const core::ClassMethod* method = cls->get_class()->get_method("some_test");

      be_expect_true(method != nullptr);

      method->call_direct<void>(&obj);
    }

    {
      const core::ClassProperty* property = cls->get_class()->get_property("SomeProp");

      be_expect_true(property != nullptr);
      be_expect_eq(0, property->get_direct<int>(&obj));
      obj.set_some_prop(42);
      be_expect_eq(42, property->get_direct<int>(&obj));
      property->set_direct(&obj, 2);
      be_expect_eq(2, property->get_direct<int>(&obj));
    }
  }
}

be_unittest_autoregister(RttiTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests