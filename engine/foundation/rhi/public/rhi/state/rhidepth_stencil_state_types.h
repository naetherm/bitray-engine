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
#include "rhi/rhidynamicrhi_types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Depth write mask
 *
 * @note
 * - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
 *
 * @see
 * - "D3D12_DEPTH_WRITE_MASK"-documentation for details
 */
enum class DepthWriteMask {
  ZERO = 0,
  ALL = 1
};

/**
 * @brief
 * Stencil operation
 *
 * @note
 * - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
 *
 * @see
 * - "D3D12_STENCIL_OP"-documentation for details
 */
enum class StencilOp {
  KEEP = 1,
  ZERO = 2,
  REPLACE = 3,
  INCR_SAT = 4,
  DECR_SAT = 5,
  INVERT = 6,
  INCREASE = 7,
  DECREASE = 8
};

/**
 * @brief
 * Depth stencil operation description
 *
 * @note
 * - This depth stencil operation description maps directly to Direct3D 10 & 11 & 12, do not change it
 * - If you want to know how the default values were chosen, have a look into the "rhi::DepthStencilStateBuilder::getDefaultDepthStencilState()"-implementation
 *
 * @see
 * - "D3D12_DEPTH_STENCILOP_DESC"-documentation for details
 */
struct DepthStencilOpDesc final {
  /// Default: "rhi::StencilOp::KEEP"
  StencilOp stencilFailOp;
  /// Default: "rhi::StencilOp::KEEP"
  StencilOp stencilDepthFailOp;
  /// Default: "rhi::StencilOp::KEEP"
  StencilOp stencilPassOp;
  /// Default: "rhi::ComparisonFunc::ALWAYS"
  ComparisonFunc stencilFunc;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
