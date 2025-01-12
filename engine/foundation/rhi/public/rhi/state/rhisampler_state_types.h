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
 * Filter mode
 *
 * @note
 * - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb205060%28v=vs.85%29.aspx are used in here
 * - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
 * - "Point" = "nearest" in OpenGL terminology
 *
 * @see
 * - "D3D12_FILTER"-documentation for details
 */
enum class FilterMode {
  /// Use point sampling for minification, magnification, and mip-level sampling.
  MIN_MAG_MIP_POINT = 0,
  /// Use point sampling for minification and magnification; use linear interpolation for mip-level sampling.
  MIN_MAG_POINT_MIP_LINEAR = 0x1,
  /// Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling.
  MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
  /// Use point sampling for minification; use linear interpolation for magnification and mip-level sampling.
  MIN_POINT_MAG_MIP_LINEAR = 0x5,
  /// Use linear interpolation for minification; use point sampling for magnification and mip-level sampling.
  MIN_LINEAR_MAG_MIP_POINT = 0x10,
  /// Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling.
  MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
  /// Use linear interpolation for minification and magnification; use point sampling for mip-level sampling.
  MIN_MAG_LINEAR_MIP_POINT = 0x14,
  /// Use linear interpolation for minification, magnification, and mip-level sampling.
  MIN_MAG_MIP_LINEAR = 0x15,
  /// Use anisotropic interpolation for minification, magnification, and mip-level sampling.
  ANISOTROPIC = 0x55,
  /// Use point sampling for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_MAG_MIP_POINT = 0x80,
  /// Use point sampling for minification and magnification; use linear interpolation for mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
  /// Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
  /// Use point sampling for minification; use linear interpolation for magnification and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
  /// Use linear interpolation for minification; use point sampling for magnification and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
  /// Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
  /// Use linear interpolation for minification and magnification; use point sampling for mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
  /// Use linear interpolation for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
  /// Use anisotropic interpolation for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_ANISOTROPIC = 0xd5,
  /// Unknown invalid setting
  UNKNOWN = 0xd6
};

/**
 * @brief
 * Texture address mode
 *
 * @note
 * - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb172483%28v=vs.85%29.aspx are used in here
 * - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
 *
 * @see
 * - "D3D12_TEXTURE_ADDRESS_MODE"-documentation for details
 */
enum class TextureAddressMode {
  /// Tile the texture at every integer junction. For example, for u values between 0 and 3, the texture is repeated three times.
  WRAP = 1,
  /// Flip the texture at every integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on.
  MIRROR = 2,
  /// Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
  CLAMP = 3,
  /// Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in "SamplerState::borderColor".
  BORDER = 4,
  /// Similar to "MIRROR" and "CLAMP". Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.
  MIRROR_ONCE = 5
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
