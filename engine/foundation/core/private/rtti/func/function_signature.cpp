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
#include "core/rtti/func/function_signature.h"
#include "core/rtti/type_info/type_info.h"


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
FunctionSignature::FunctionSignature()
: mReturnType(nullptr) {
}

FunctionSignature::FunctionSignature(const core::TypeInfo* returnType, const Vector<const TypeInfo*>& parameterTypes)
: mReturnType(returnType)
, mParameterTypes(parameterTypes) {
}

FunctionSignature::FunctionSignature(const FunctionSignature& other)
: mReturnType(other.mReturnType)
, mParameterTypes(other.mParameterTypes) {
}

FunctionSignature& FunctionSignature::operator=(const FunctionSignature& other) {
  mReturnType = other.mReturnType;
  mParameterTypes = other.mParameterTypes;

  return *this;
}

bool FunctionSignature::operator==(const FunctionSignature& other) const {
  return ((mReturnType == other.mReturnType) && (mParameterTypes == other.mParameterTypes));
}

const TypeInfo* FunctionSignature::get_return_type() const {
  return mReturnType;
}

const Vector<const TypeInfo*>& FunctionSignature::get_parameter_types() const {
  return mParameterTypes;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}