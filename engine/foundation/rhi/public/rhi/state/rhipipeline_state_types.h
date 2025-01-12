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
#include "rhi/rhi.h"
#include "rhi/buffer/rhivertex_array_types.h"
#include "rhi/state/rhisampler_state_types.h"
#include "rhi/state/rhirasterizer_state_types.h"
#include "rhi/state/rhidepth_stencil_state_types.h"
#include "rhi/state/rhiblend_state_types.h"
#include "rhi/texture/rhitexture_types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class RHIRootSignature;
class RHIGraphicsProgram;
class RHIRenderPass;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Input-assembler (IA) stage: Primitive topology types
 *
 * @note
 * - These constants directly map to Direct3D 9 & 10 & 11 constants, do not change them
 * - For a visual overview see "Primitive Topologies" -> "The following illustration shows the vertex ordering for all of the primitive types that the input assembler can produce." at https://docs.microsoft.com/en-gb/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies?redirectedfrom=MSDN
 */
enum class PrimitiveTopology {
  /// Unknown primitive type
  UNKNOWN = 0,
  /// Point list, use "PATCH_LIST_1" for tessellation
  POINT_LIST = 1,
  /// Line list, use "PATCH_LIST_2" for tessellation
  LINE_LIST = 2,
  /// Line strip
  LINE_STRIP = 3,
  /// Triangle list, use "PATCH_LIST_3" for tessellation
  TRIANGLE_LIST = 4,
  /// Triangle strip
  TRIANGLE_STRIP = 5,
  /// Line list with adjacency data, use "PATCH_LIST_2" for tessellation
  LINE_LIST_ADJ = 10,
  /// Line strip with adjacency data
  LINE_STRIP_ADJ = 11,
  /// Triangle list with adjacency data, use "PATCH_LIST_3" for tessellation
  TRIANGLE_LIST_ADJ = 12,
  /// Triangle strip with adjacency data
  TRIANGLE_STRIP_ADJ = 13,
  /// Patch list with 1 vertex per patch (tessellation relevant topology type) - "POINT_LIST" used for tessellation
  PATCH_LIST_1 = 33,
  /// Patch list with 2 vertices per patch (tessellation relevant topology type) - "LINE_LIST" used for tessellation
  PATCH_LIST_2 = 34,
  /// Patch list with 3 vertices per patch (tessellation relevant topology type) - "TRIANGLE_LIST" used for tessellation
  PATCH_LIST_3 = 35,
  /// Patch list with 4 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_4 = 36,
  /// Patch list with 5 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_5 = 37,
  /// Patch list with 6 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_6 = 38,
  /// Patch list with 7 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_7 = 39,
  /// Patch list with 8 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_8 = 40,
  /// Patch list with 9 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_9 = 41,
  /// Patch list with 10 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_10 = 42,
  /// Patch list with 11 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_11 = 43,
  /// Patch list with 12 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_12 = 44,
  /// Patch list with 13 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_13 = 45,
  /// Patch list with 14 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_14 = 46,
  /// Patch list with 15 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_15 = 47,
  /// Patch list with 16 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_16 = 48,
  /// Patch list with 17 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_17 = 49,
  /// Patch list with 18 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_18 = 50,
  /// Patch list with 19 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_19 = 51,
  /// Patch list with 20 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_20 = 52,
  /// Patch list with 21 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_21 = 53,
  /// Patch list with 22 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_22 = 54,
  /// Patch list with 23 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_23 = 55,
  /// Patch list with 24 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_24 = 56,
  /// Patch list with 25 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_25 = 57,
  /// Patch list with 26 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_26 = 58,
  /// Patch list with 27 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_27 = 59,
  /// Patch list with 28 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_28 = 60,
  /// Patch list with 29 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_29 = 61,
  /// Patch list with 30 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_30 = 62,
  /// Patch list with 31 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_31 = 63,
  /// Patch list with 32 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_32 = 64
};

/**
 * @brief
 * Primitive topology type specifying how the graphics pipeline interprets geometry or hull shader input primitives
 *
 * @note
 * - These constants directly map to Direct3D 12 constants, do not change them
 *
 * @see
 * - "D3D12_PRIMITIVE_TOPOLOGY_TYPE"-documentation for details
 */
enum class PrimitiveTopologyType {
  /// The shader has not been initialized with an input primitive type
  UNDEFINED = 0,
  /// Interpret the input primitive as a point
  POINT = 1,
  /// Interpret the input primitive as a line
  LINE = 2,
  /// Interpret the input primitive as a triangle
  TRIANGLE = 3,
  /// Interpret the input primitive as a control point patch
  PATCH = 4
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
