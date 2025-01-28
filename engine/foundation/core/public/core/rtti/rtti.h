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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/rtti/rtti_type_server.h"


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
#define be_begin_enum(NAME) \
template<> \
class core::EnumRegistrar<NAME> { \
public: \
  static core::EnumTypeInfo* type_info; \
public: \
  EnumRegistrar() { \
    type_info = new core::EnumTypeInfo(#NAME); \
    register_enum(); \
    core::RttiTypeServer::instance().register_enum(type_info); \
  } \
  ~EnumRegistrar() { \
    delete type_info; \
  } \
  void register_enum() override { \
    core::EnumTypeInfoBuilder builder(type_info); \

#define be_end_enum(NAME, NAMESPACE) \
} \
}; \
core::EnumTypeInfo* core::EnumRegistrar<NAMESPACE::NAME>::type_info = nullptr; \
core::EnumRegistrar<NAMESPACE::NAME> NAMESPACE##_##NAME##_EnumRegistrar; \


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
class EnumRegistrar {
public:

  static core::EnumTypeInfo* type_info;

public:

  EnumRegistrar() {}

  ~EnumRegistrar() {}

  virtual void register_enum() {}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}