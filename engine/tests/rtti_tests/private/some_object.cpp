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
#include "some_object.h"

#include <core/rtti/rtti.h>
#include <core/rtti/type/class_type_info.h>
#include <core/rtti/type/enum_type_info.h>
#include <core/rtti/type/enum_type_info_builder.h>


// Register SomEnum
template<>
class core::EnumRegistrar<rtti_tests::SomeEnum> {
public:

  static core::EnumTypeInfo* type_info;
public:

  EnumRegistrar() {
    type_info = new core::EnumTypeInfo("rtti_tests::SomeEnum");
    register_enum();
    // Register in type_server
    core::RttiTypeServer::instance().register_enum(type_info);
  }

  ~EnumRegistrar() {
    delete type_info;
  }

  void register_enum() override {
    core::EnumTypeInfoBuilder builder(type_info);
    builder.add_value("FirstValue", rtti_tests::SomeEnum::FirstValue);
    builder.add_value("SecondValue", rtti_tests::SomeEnum::SecondValue);
    builder.add_value("ThirdValue", rtti_tests::SomeEnum::ThirdValue);
  }
};
core::EnumTypeInfo* core::EnumRegistrar<rtti_tests::SomeEnum>::type_info = nullptr;
core::EnumRegistrar<rtti_tests::SomeEnum> SomeEnum_EnumRegistrar;

// Register SomEnum
be_begin_enum(rtti_tests::SomeOtherEnum)
    builder.add_value("FirstOtherValue", rtti_tests::SomeOtherEnum::FirstOtherValue);
    builder.add_value("SecondOtherValue", rtti_tests::SomeOtherEnum::SecondOtherValue);
    builder.add_value("ThirdOtherValue", rtti_tests::SomeOtherEnum::ThirdOtherValue);
be_end_enum(SomeOtherEnum, rtti_tests)

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rtti_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
// Register Object
core::TypeInfo* Object::get_static_type_info() {
  static core::ClassTypeInfo STypeInfo("Object");
  return &STypeInfo;
}

core::TypeInfo* Object::get_type_info() const {
  return get_static_type_info();
}

Object::Object() {
}

Object::~Object() {
}

// Register SomeObject
core::TypeInfo* SomeObject::get_static_type_info() {
  static core::ClassTypeInfo STypeInfo("rtti_tests::SomeObject");
  return &STypeInfo;
}

core::TypeInfo* SomeObject::get_type_info() const {
  return get_static_type_info();
}

SomeObject::SomeObject() {
}

SomeObject::~SomeObject() {
}
//core::EnumTypeInfo SomeEnum_EnumTypeInfo("rtti_tests::SomeEnum");
//core::EnumTypeInfoBuilder SomeEnum_enumBuilder(&SomeEnum_EnumTypeInfo);
//SomeEnum_enumBuilder.add_value("Foo", SomeEnum::FirstValue);

// Register SomeOtherEnum


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}