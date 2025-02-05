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


#define __be_declare_class(CLASS, COPYABLE) \
  template<> struct core::StaticTypeInfo<CLASS> { \
    static core::TypeInfo* get() { \
      static core::ClassTypeInfo info(#CLASS); \
      static core::ClassTypeInfo* infoPtr = nullptr; \
      if (!infoPtr) { \
        core::ClassTypeInfo* regInfo = core::RttiTypeServer::instance().get_class_type(#CLASS); \
        if (regInfo) { \
          infoPtr = regInfo; \
        } else { \
          core::RttiTypeServer::instance().register_class_type(#CLASS, &info); \
          infoPtr = &info; \
        } \
      } \
      return infoPtr; \
    } \
    enum { Defined = true, Copyable = COPYABLE }; \
  };


#define be_declare_class(CLASS) \
  __be_declare_class(CLASS, true)


#define be_declare_non_copyable_class(CLASS) \
  __be_declare_class(CLASS, false)


#define be_rtti() \
public: \
  static void register_reflection(); \
  const core::ClassTypeInfo* get_class_type_info() const { \
    return (const core::ClassTypeInfo*)core::get_static_type_info(this); \
  } \
private:


#define be_begin_class(CLASS, NAMESPACE) \
struct Register_##NAMESPACE_##CLASS { \
  Register_##NAMESPACE_##CLASS() { \
    NAMESPACE::CLASS::register_reflection(); \
  } \
}; \
static Register_##NAMESPACE_##CLASS NAMESPACE_##CLASS_declare_class_register; \
void NAMESPACE::CLASS::register_reflection() { \
  typedef NAMESPACE::CLASS _CLASS; \
  core::Class::declare<NAMESPACE::CLASS>(#NAMESPACE"::"#CLASS) \


#define be_end_class() \
;}


#define be_base_class(NAME) \
    .base(#NAME)


#define be_default_constructor() \
    .constructor(new core::Constructor<_CLASS>())


#define be_constructor(...) \
  .constructor(new core::Constructor<_CLASS, __VA_ARGS__>())


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
