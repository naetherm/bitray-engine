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
#include "core/rtti/type_info/type_info.h"
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
namespace internal {

template<int N, typename...> struct TemplateToTypeInfoVector;

template<int N, typename TFirst, typename... TTail>
struct TemplateToTypeInfoVector<N, TFirst, TTail...> {
  /**
   * @brief
   * Adds the type information associated with `TFirst` and the type information associated with the
   * types in `TTail` to the specified vector.
   *
   * @param parameterTypes
   * The vector which will receive the type information associated with `TFirst` and the types in
   * `TTail`.
   */
  static void Make(core::Vector<const TypeInfo*>& parameterTypes) {
    parameterTypes.push_back(core::StaticTypeInfo<TFirst>::get());
    TemplateToTypeInfoVector<N-1, TTail...>::Make(parameterTypes);
  }
};

template<int N, typename TFirst>
struct TemplateToTypeInfoVector<N, TFirst> {
  /**
   * @brief
   * Adds the type information associated with `TFirst` to the specified vector.
   *
   * @param parameterTypes
   * The vector which will receive the type information associated with `TFirst`.
   */
  static void Make(core::Vector<const TypeInfo*>& parameterTypes) {
    parameterTypes.push_back(core::StaticTypeInfo<TFirst>::get());
  }
};

template<int N>
struct TemplateToTypeInfoVector<N> {
  /**
   * @brief
   * Adds no type information to the specified vector.
   *
   * @param parameterTypes
   * The vector which will not receive any type information.
   */
  static void Make(core::Vector<const TypeInfo*>& parameterTypes) {

  }
};

}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TReturn, typename ... TArgs>
FunctionSignature FunctionSignature::from_template() {
  core::TypeInfo* returnType = core::StaticTypeInfo<TReturn>::get();
  core::Vector<const core::TypeInfo*> parameterTypes;
  parameterTypes.reserve(sizeof...(TArgs));

  internal::TemplateToTypeInfoVector<sizeof...(TArgs), TArgs...>::Make(parameterTypes);

  return FunctionSignature(returnType, parameterTypes);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}