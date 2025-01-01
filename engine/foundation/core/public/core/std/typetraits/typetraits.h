////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
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
#include "core/std/typetraits/add_const.h"
#include "core/std/typetraits/add_cv.h"
#include "core/std/typetraits/add_pointer.h"
#include "core/std/typetraits/add_reference.h"
#include "core/std/typetraits/add_volatile.h"
#include "core/std/typetraits/aligned_storage.h"
#include "core/std/typetraits/alignment_of.h"
#include "core/std/typetraits/common_type.h"
#include "core/std/typetraits/conditional.h"
#include "core/std/typetraits/conjunction.h"
#include "core/std/typetraits/decay.h"
#include "core/std/typetraits/disjunction.h"
#include "core/std/typetraits/extent.h"
#include "core/std/typetraits/has_virtual_destructor.h"
#include "core/std/typetraits/integral_constant.h"
#include "core/std/typetraits/intrinsics.h"
#include "core/std/typetraits/is_abstract.h"
#include "core/std/typetraits/is_arithmetic.h"
#include "core/std/typetraits/is_array.h"
#include "core/std/typetraits/is_assignable.h"
#include "core/std/typetraits/is_base_of.h"
#include "core/std/typetraits/is_class.h"
#include "core/std/typetraits/is_compound.h"
#include "core/std/typetraits/is_const.h"
#include "core/std/typetraits/is_constructible.h"
#include "core/std/typetraits/is_convertible.h"
#include "core/std/typetraits/is_destructible.h"
#include "core/std/typetraits/is_empty.h"
#include "core/std/typetraits/is_enum.h"
#include "core/std/typetraits/is_floating_point.h"
#include "core/std/typetraits/is_function.h"
#include "core/std/typetraits/is_fundamental.h"
#include "core/std/typetraits/is_integral.h"
#include "core/std/typetraits/is_lvalue_reference.h"
#include "core/std/typetraits/is_member_function_pointer.h"
#include "core/std/typetraits/is_member_object_pointer.h"
#include "core/std/typetraits/is_member_pointer.h"
#include "core/std/typetraits/is_object.h"
#include "core/std/typetraits/is_pod.h"
#include "core/std/typetraits/is_pointer.h"
#include "core/std/typetraits/is_polymorphic.h"
#include "core/std/typetraits/is_reference.h"
#include "core/std/typetraits/is_rvalue_reference.h"
#include "core/std/typetraits/is_same.h"
#include "core/std/typetraits/is_scalar.h"
#include "core/std/typetraits/is_signed.h"
#include "core/std/typetraits/is_swappable.h"
#include "core/std/typetraits/is_trivial.h"
#include "core/std/typetraits/is_trivially_copyable.h"
#include "core/std/typetraits/is_union.h"
#include "core/std/typetraits/is_unsigned.h"
#include "core/std/typetraits/is_void.h"
#include "core/std/typetraits/is_volatile.h"
#include "core/std/typetraits/negation.h"
#include "core/std/typetraits/rank.h"
#include "core/std/typetraits/remove_all_extents.h"
#include "core/std/typetraits/remove_const.h"
#include "core/std/typetraits/remove_cv.h"
#include "core/std/typetraits/remove_cvref.h"
#include "core/std/typetraits/remove_extent.h"
#include "core/std/typetraits/remove_pointer.h"
#include "core/std/typetraits/remove_reference.h"
#include "core/std/typetraits/remove_volatile.h"
#include "core/std/typetraits/type_identity.h"
#include "core/std/typetraits/underlying_type.h"
#include "core/std/typetraits/void_t.h"
