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
#include "core/container/array.h"


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
class TypeInfo;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * FunctionSignature
 *
 * @brief
 * A `FunctionSignature` class represents the signature of a function.
 *
 * This class is used to store the return type and the types of the arguments
 * of a function.
 */
class FunctionSignature {
public:

  /**
   * @brief
   * Create a `FunctionSignature` object from a template signature.
   *
   * @tparam TReturn
   * The return type of the function.
   * @tparam TArgs
   * A variadic list of argument types.
   *
   * @return
   * A `FunctionSignature` object representing the function signature.
   */
  template<typename TReturn, typename... TArgs>
  static FunctionSignature from_template();

public:

    /**
     * @brief
     * Construct a function signature object with an empty parameter type list and a void return
     * type.
     */
    FunctionSignature();

    /**
     * @brief
     * Construct a function signature object with the specified return type and parameter type list.
     *
     * @param returnType
     * The return type of the function.
     * @param parameterTypes
     * The parameter type list of the function.
     */
    FunctionSignature(const core::TypeInfo* returnType, const Array<const TypeInfo*>& parameterTypes);


    /**
     * @brief
     * Equality operator to compare two `FunctionSignature` objects.
     *
     * @param other
     * The other `FunctionSignature` object to compare with.
     *
     * @return
     * `true` if the two `FunctionSignature` objects are equal, `false` otherwise.
     */
    bool operator==(const FunctionSignature& other) const;


    /**
     * @brief
     * Get the return type of the function.
     *
     * @return
     * A pointer to the return type of the function.
     */
    [[nodiscard]] const TypeInfo* get_return_type() const;

    /**
     * @brief
     * Get the parameter types of the function.
     *
     * @return
     * A reference to an array containing the parameter types.
     */
    [[nodiscard]] const Array<const TypeInfo*>& get_parameter_types() const;

private:
    /** The return type of the function. */
    const TypeInfo* mReturnType;
    /** The parameter types of the function. */
    Array<const TypeInfo*> mParameterTypes;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/func/function_signature.inl"