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
#include "rtti/function_signature.h"
#include <core/rtti/object.h>
#include <core/rtti/func/function_signature.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
FunctionSignatureTests::FunctionSignatureTests()
  : UnitTest("FunctionSignatureTests") {

}

FunctionSignatureTests::~FunctionSignatureTests() {

}

void FunctionSignatureTests::test() {
  // Default constructor
  {
    core::FunctionSignature fs;

    be_expect_true(nullptr == fs.get_return_type())
    be_expect_eq(0, fs.get_parameter_types().size())
  }

  // from_template<void>()
  {
    core::FunctionSignature fs = core::FunctionSignature::from_template<void>();

    be_expect_true(core::StaticTypeInfo<void>::get() == fs.get_return_type())
    be_expect_eq(0, fs.get_parameter_types().size())
  }

  // from_template<void, core::int8>()
  {
    core::FunctionSignature fs = core::FunctionSignature::from_template<void, core::int8>();

    be_expect_true(core::StaticTypeInfo<void>::get() == fs.get_return_type())
    be_expect_eq(1, fs.get_parameter_types().size())
    be_expect_true(core::StaticTypeInfo<core::int8>::get() == fs.get_parameter_types()[0])
  }

  // from_template<void, core::int8, bool>()
  {
    core::FunctionSignature fs = core::FunctionSignature::from_template<void, core::int8, bool>();

    be_expect_true(core::StaticTypeInfo<void>::get() == fs.get_return_type())
    be_expect_eq(2, fs.get_parameter_types().size())
    be_expect_true(core::StaticTypeInfo<core::int8>::get() == fs.get_parameter_types()[0])
    be_expect_true(core::StaticTypeInfo<bool>::get() == fs.get_parameter_types()[1])
  }
}

be_unittest_autoregister(FunctionSignatureTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests