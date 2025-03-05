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
#include "core/rtti/type_info/class_type_info.h"
#include "core/rtti/rtti_type_server.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TClass>
ClassBuilder<TClass>::ClassBuilder(Class& c)
: mClass(&c) {
}

template<typename TClass>
ClassBuilder<TClass>& ClassBuilder<TClass>::base(const String& name) {
  const ClassTypeInfo* baseType = RttiTypeServer::instance().get_class_type(name);
  if (baseType) {
    const Class* base = baseType->get_class();
    if (base) {
      mClass->mBaseClasses.push_back(base);
    } else {
      /// TODO(naetherm): Late binding
    }
  }

  return *this;
}


template<typename TClass>
ClassBuilder<TClass>& ClassBuilder<TClass>::constructor(FuncBase* func) {
  // Check if this is the default constructor
  if (func->get_signature().get_parameter_types().empty()) {
    mClass->mDefaultConstructor = ClassConstructor(func);
    mLastRttiMember = &mClass->mDefaultConstructor;
  } else {
    mClass->mConstructors.push_back(ClassConstructor(func));
    mLastRttiMember = &mClass->mConstructors.back();
  }
  return *this;
}

template<typename TClass>
ClassBuilder<TClass>& ClassBuilder<TClass>::method(const String& name, FuncBase* func) {
  mClass->mMethodsMap[name] = ClassMethod(name, func);
  mLastRttiMember = &mClass->mMethodsMap[name];

  return *this;
}

template<typename TClass>
ClassBuilder<TClass>& ClassBuilder<TClass>::field(const String& name, core::uint32 offset) {
  mClass->mFieldsMap[name] = ClassField(name, offset);
  mLastRttiMember = &mClass->mFieldsMap[name];

  return *this;
}

template<typename TClass>
ClassBuilder<TClass>& ClassBuilder<TClass>::property(const String& name, FuncBase* setter, FuncBase* getter) {
  mClass->mPropertiesMap[name] = ClassProperty(name, setter, getter);
  mLastRttiMember = &mClass->mPropertiesMap[name];

  return *this;
}

template<typename TClass>
ClassBuilder<TClass>& ClassBuilder<TClass>::add_tag(const String& name, const DynamicObject& value) {
  mLastRttiMember->add_tag(name, value);

  return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}