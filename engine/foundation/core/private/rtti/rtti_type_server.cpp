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
#include "core/rtti/rtti_type_server.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
RttiTypeServer& RttiTypeServer::instance() {
  static RttiTypeServer SInstance;
  return SInstance;
}


RttiTypeServer::RttiTypeServer() {
}

RttiTypeServer::~RttiTypeServer() {

}

void RttiTypeServer::register_primitive_type(const String& name, PrimitiveTypeInfo* primitiveTypeInfo) {
  auto ti = mPrimitiveTypesMap.find(name);
  if (ti == mPrimitiveTypesMap.end()) {
    mPrimitiveTypesMap[name] = primitiveTypeInfo;
  }
}

PrimitiveTypeInfo* RttiTypeServer::get_primitive_type(const String& name) {
  auto ti = mPrimitiveTypesMap.find(name);
  if (ti == mPrimitiveTypesMap.end()) {
    return nullptr;
  }
  return ti.value();
}

void RttiTypeServer::register_class_type(const String& name, ClassTypeInfo* classTypeInfo) {
  auto ti = mClassTypeMap.find(name);
  if (ti == mClassTypeMap.end()) {
    mClassTypeMap[name] = classTypeInfo;
  }
}

ClassTypeInfo* RttiTypeServer::get_class_type(const String& name) {
  auto ti = mClassTypeMap.find(name);
  if (ti == mClassTypeMap.end()) {
    return nullptr;
  }
  return ti.value();
}

void RttiTypeServer::register_enum_type(const String& name, EnumTypeInfo* enumTypeInfo) {
  auto ti = mEnumTypeMap.find(name);
  if (ti == mEnumTypeMap.end()) {
    mEnumTypeMap[name] = enumTypeInfo;
  }
}

EnumTypeInfo* RttiTypeServer::get_enum_type(const String& name) {
  auto ti = mEnumTypeMap.find(name);
  if (ti == mEnumTypeMap.end()) {
    return nullptr;
  }
  return ti.value();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}