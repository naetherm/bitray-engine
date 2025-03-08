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
class InheritedObject : public core::Object {
public:

  be_rtti()

public:

  InheritedObject()
  : mOtherProp(13.37f) {}


  void some_other_test() {
    BE_LOG(Info, "Called InheritedObject::some_other_test()");
  }

  float get_other_prop() const {
    return mOtherProp;
  }

  void set_other_prop(float foo) {
    mOtherProp = foo;
  }

private:

  float mOtherProp;
};
}

be_declare_class(core_tests::InheritedObject)

be_begin_class(InheritedObject, core_tests)
  be_default_constructor()
  be_base_class(core::Object)
  be_method(some_other_test)
    be_tag("test", "true")
  be_property_getset(OtherProp, get_other_prop, set_other_prop)
be_end_class()

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

  {
    core::ClassTypeInfo* cls = rttiTypeServer.get_class_type("core::Object");

    core::Object* obj = cls->get_class()->create<core::Object>();

    be_expect_eq(0, obj->get_some_prop());

    delete obj;
  }

  core::Object obj;
  core_tests::InheritedObject obj2;

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
      const core::ClassMethod* method = cls->get_class()->get_method("some_test");

      be_expect_true(method != nullptr);

      core::Vector<core::DynamicObject> args;
      args.push_back(&obj);

      method->invoke(&args);
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

  {
    core::ClassTypeInfo* cls = rttiTypeServer.get_class_type("core_tests::InheritedObject");
    be_expect_true(cls != nullptr);
    be_expect_str_eq("core_tests::InheritedObject", cls->get_name().c_str());

    {
      const core::ClassMethod* method = cls->get_class()->get_method("some_other_test");
      be_expect_true(method->has_tag("test"));

      be_expect_true(method != nullptr);
      be_expect_true(cls->get_class()->is_derived_from(rttiTypeServer.get_class_type("core::Object")->get_class()));

      method->call_direct<void>(&obj2);
    }

    {
      const core::ClassMethod* method = cls->get_class()->get_method("some_test");

      be_expect_true(method != nullptr);

      method->call_direct<void>(&obj2);
    }
  }
}

be_unittest_autoregister(RttiTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests