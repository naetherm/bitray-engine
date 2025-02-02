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
#include "core/rtti/type_info/type_info.h"
#include "core/rtti/type_info/pointer_type_info.h"
#include "core/rtti/type_info/reference_type_info.h"
#include "core/std/typetraits/raw_type.h"


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
template<typename TType>
struct StaticTypeInfo;

template<typename TType>
struct StaticTypeInfo {
  /**
   * @brief
   * Get the type info object associated with the current type.
   *
   * @return
   * The type info object associated with the current type, or nullptr if no type info object is
   * associated with the current type.
   */
  static TypeInfo* get() {
    return nullptr;
  }

  enum { Defined = false, Copyable = true };
};

template<typename TType>
struct StaticTypeInfo<TType*> {
  /**
   * @brief
   * Get the type info object associated with the current type.
   *
   * @return
   * The type info object associated with the current type, or nullptr if no type info object is
   * associated with the current type.
   */
  static TypeInfo* get() {
    static PointerTypeInfo info(StaticTypeInfo<TType>::get(), false);
    return &info;
  }

  enum { Defined = true, Copyable = true };
};
template<typename TType>
struct StaticTypeInfo<const TType*> {
  /**
   * @brief
   * Get the type info object associated with the current type.
   *
   * @return
   * The type info object associated with the current type, or nullptr if no type info object is
   * associated with the current type.
   */
  static TypeInfo* get() {
    static PointerTypeInfo info(StaticTypeInfo<TType>::get(), true);
    return &info;
  }

  enum { Defined = true, Copyable = true };
};

template<typename TType>
struct StaticTypeInfo<TType&> {
  /**
   * @brief
   * Get the type info object associated with the current type.
   *
   * @return
   * The type info object associated with the current type, or nullptr if no type info object is
   * associated with the current type.
   */
  static TypeInfo* get() {
    static ReferenceTypeInfo info(StaticTypeInfo<TType>::get(), false);
    return &info;
  }

  enum { Defined = true, Copyable = false };
};
template<typename TType>
struct StaticTypeInfo<const TType&> {
  /**
   * @brief
   * Get the type info object associated with the current type.
   *
   * @return
   * The type info object associated with the current type, or nullptr if no type info object is
   * associated with the current type.
   */
  static TypeInfo* get() {
    static ReferenceTypeInfo info(StaticTypeInfo<TType>::get(), true);
    return &info;
  }

  enum { Defined = true, Copyable = false };
};



template<typename TType>
struct HashStaticTypeInfo {
  enum Value {
    value = StaticTypeInfo<core::raw_type<TType>>::Defined
  };
};

/**
 * @brief
 * Retrieve the static type information object associated with the specified type.
 *
 * @tparam TType
 * The type for which the static type information is requested.
 *
 * @return
 * A pointer to the `TypeInfo` object associated with the specified type, or nullptr
 * if no type info object is associated with it.
 */
template<typename TType>
TypeInfo* get_static_type_info() {
  return StaticTypeInfo<TType>::get();
}

/**
 * @brief
 * Retrieve the static type information object associated with the specified type.
 *
 * @tparam TType
 * The type for which the static type information is requested.
 *
 * @return
 * A pointer to the `TypeInfo` object associated with the specified type, or nullptr
 * if no type info object is associated with it.
 */
template<typename TType>
TypeInfo* get_static_type_info(TType o) {
  (void)o;

  return StaticTypeInfo<TType>::get();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}