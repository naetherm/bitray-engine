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
#include "core/rtti/type/class_type_info.h"
#include "core/rtti/type/enum_type_info.h"
#include "core/rtti/type/primitive_type_info.h"


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
  // Clean up
  for (auto* enumTypeInfo: mEnumTypeInfos) {
    //delete enumTypeInfo;
  }
  mEnumTypeInfos.clear();
  mEnumTypeInfoMap.clear();
}


void RttiTypeServer::register_enum(EnumTypeInfo* enumTypeInfo) {
  mEnumTypeInfos.push_back(enumTypeInfo);
  mEnumTypeInfoMap.set(enumTypeInfo->get_name(), enumTypeInfo);
}

void RttiTypeServer::register_class(ClassTypeInfo* classTypeInfo) {
  mClassTypeInfos.push_back(classTypeInfo);
  mClassTypeInfoMap.set(classTypeInfo->get_name(), classTypeInfo);
}

void RttiTypeServer::register_primitive(PrimitiveTypeInfo* primitiveTypeInfo) {
  mPrimitiveTypeInfos.push_back(primitiveTypeInfo);
  mPrimitiveTypeInfoMap.set(primitiveTypeInfo->get_name(), primitiveTypeInfo);
}

const EnumTypeInfo* RttiTypeServer::get_enum(const String& name) const {
  return mEnumTypeInfoMap[name];
}

EnumTypeInfo* RttiTypeServer::get_enum(const String& name) {
  return mEnumTypeInfoMap[name];
}

const ClassTypeInfo* RttiTypeServer::get_class(const String& name) const {
  return mClassTypeInfoMap[name];
}

ClassTypeInfo* RttiTypeServer::get_class(const String& name) {
  return mClassTypeInfoMap[name];
}

const PrimitiveTypeInfo* RttiTypeServer::get_primitive(const String& name) const {
  return mPrimitiveTypeInfoMap[name];
}

PrimitiveTypeInfo* RttiTypeServer::get_primitive(const String& name) {
  return mPrimitiveTypeInfoMap[name];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}