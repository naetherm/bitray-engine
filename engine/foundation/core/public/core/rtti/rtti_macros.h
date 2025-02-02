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
#include "core/rtti/enum.h"
#include "core/rtti/builder/enum_builder.h"


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
#define be_declare_basic_type(TYPE) \
  template<> struct core::StaticTypeInfo<TYPE> { \
    static core::TypeInfo* get() { \
      static core::PrimitiveTypeInfo info(#TYPE); \
      static core::PrimitiveTypeInfo* infoPtr = nullptr; \
      if (!infoPtr) { \
        core::PrimitiveTypeInfo* regInfo = core::RttiTypeServer::instance().get_primitive_type(#TYPE); \
        if (regInfo) { \
          infoPtr = regInfo; \
        } else { \
          core::RttiTypeServer::instance().register_primitive_type(#TYPE, &info); \
          infoPtr = &info; \
        } \
      } \
      return infoPtr; \
    } \
    enum { Defined = true, Copyable = true }; \
  };


#define __be_declare_enum(ENUM) \
  template<> struct core::StaticTypeInfo<ENUM> { \
    static core::TypeInfo* get() { \
      static core::EnumTypeInfo info(#ENUM); \
      static core::EnumTypeInfo* infoPtr = nullptr; \
      if (!infoPtr) { \
        core::EnumTypeInfo* regInfo = core::RttiTypeServer::instance().get_enum_type(#ENUM); \
        if (regInfo) { \
          infoPtr = regInfo; \
        } else { \
          core::RttiTypeServer::instance().register_enum_type(#ENUM, &info); \
          infoPtr = &info; \
        } \
      } \
      return infoPtr; \
    } \
    enum { Defined = true, Copyable = true }; \
  };


#define be_declare_enum(ENUM) \
  __be_declare_enum(ENUM)



#define be_begin_enum(ENUM, NAMESPACE) \
  struct AutoEnumRegister_##NAMESPACE##_##ENUM { \
    AutoEnumRegister_##NAMESPACE##_##ENUM() { \
      register_enum(); \
    } \
    static void register_enum(); \
  }; \
  static AutoEnumRegister_##NAMESPACE##_##ENUM NAMESPACE##_##ENUM_declare_enum_register; \
  void AutoEnumRegister_##NAMESPACE##_##ENUM::register_enum() { \
    core::Enum::declare<NAMESPACE::ENUM>(#NAMESPACE"::"#ENUM) \


#define be_end_enum() \
  ;}
