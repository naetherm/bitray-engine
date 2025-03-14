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
#include "core/rtti/builder/class_builder.h"
#include "core/rtti/rtti.h"
#include "core/rtti/type_info/class_type_info.h"
#include "core/rtti/type_info/static_type_info.h"


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
template<typename TClass>
TClass* Class::create() const {
  if (!has_default_constructor()) {
    return nullptr;
  }

  DynamicObject obj = mDefaultConstructor.invoke(nullptr);

  return obj.get_as<TClass*>();
}

template<typename TClass>
ClassBuilder<TClass> Class::declare(const String& name) {
  Class* clss = new Class(name);

  TypeInfo* classTypeInfo = StaticTypeInfo<TClass>::get();
  static_cast<ClassTypeInfo *>(classTypeInfo)->mClass = clss;

  return ClassBuilder<TClass>(*clss);
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}