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
#include "core/rtti/class.h"
#include "core/rtti/rtti.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Class::Class(const String& name)
: mName(name) {
}

Class::~Class() {
}

const String& Class::get_name() const {
  return mName;
}

const Vector<const Class*>& Class::get_base_classes() const {
  return mBaseClasses;
}

bool Class::is_derived_from(const Class* baseClass) const {
  for (auto base: mBaseClasses) {
    if (base == baseClass) {
      return true;
    }

    if (base->is_derived_from(baseClass)) {
      return true;
    }
  }

  return false;
}

const ClassConstructor* Class::get_default_constructor() const {
  return has_default_constructor() ? &mDefaultConstructor : nullptr;
}

const Vector<ClassConstructor>& Class::get_constructors() const {
  return mConstructors;
}

bool Class::has_default_constructor() const {
  return *mDefaultConstructor.get_function_signature().get_return_type() != *StaticTypeInfo<void>::get();
}

uint32 Class::get_num_constructors() const {
  uint32 result = has_default_constructor() ? 1 : 0;

  result += mConstructors.size();

  return result;
}

uint32 Class::get_num_methods() const {
  return mMethodsMap.size();
}

uint32 Class::get_num_fields() const {
  return mFieldsMap.size();
}

uint32 Class::get_num_properties() const {
  return mPropertiesMap.size();
}

const ClassMethod* Class::get_method(const String& name) const {
  const auto& meth = mMethodsMap.find(name);
  if (meth != mMethodsMap.end()) {
    return &meth.value();
  }
  // Check all base classes
  for (auto base: mBaseClasses) {
    const ClassMethod* method = base->get_method(name);
    if (method) {
      return method;
    }
  }

  return nullptr;
}

const ClassField* Class::get_field(const String& name) const {
  const auto& field = mFieldsMap.find(name);
  if (field != mFieldsMap.end()) {
    return &field.value();
  }
  // Check all base classes
  for (auto base: mBaseClasses) {
    const ClassField* field = base->get_field(name);
    if (field) {
      return field;
    }
  }

  return nullptr;
}

const ClassProperty* Class::get_property(const String& name) const {
  const auto& prop = mPropertiesMap.find(name);
  if (prop != mPropertiesMap.end()) {
    return &prop.value();
  }
  // Check all base classes
  for (auto base: mBaseClasses) {
    const ClassProperty* property = base->get_property(name);
    if (property) {
      return property;
    }
  }

  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}