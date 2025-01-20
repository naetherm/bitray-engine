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
#include "rhi_vulkan/mapping.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
VkFilter Mapping::get_vulkan_mag_filter_mode(rhi::FilterMode filterMode) {
  switch (filterMode) {
    case rhi::FilterMode::MIN_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::MIN_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::ANISOTROPIC:
      return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

    case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::COMPARISON_ANISOTROPIC:
      return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

    case rhi::FilterMode::UNKNOWN:
      BE_ASSERT(false, "Vulkan filter mode must not be unknown")
      return VK_FILTER_NEAREST;

    default:
      return VK_FILTER_NEAREST;	// We should never be in here
  }
}

VkFilter Mapping::get_vulkan_min_filter_mode(rhi::FilterMode filterMode) {
  switch (filterMode) {
    case rhi::FilterMode::MIN_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::MIN_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::ANISOTROPIC:
      return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

    case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case rhi::FilterMode::COMPARISON_ANISOTROPIC:
      return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

    case rhi::FilterMode::UNKNOWN:
      BE_ASSERT(false, "Vulkan filter mode must not be unknown")
      return VK_FILTER_NEAREST;

    default:
      return VK_FILTER_NEAREST;	// We should never be in here
  }
}

VkSamplerMipmapMode Mapping::get_vulkan_mipmap_mode(rhi::FilterMode filterMode) {
  switch (filterMode) {
    case rhi::FilterMode::MIN_MAG_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case rhi::FilterMode::MIN_MAG_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case rhi::FilterMode::ANISOTROPIC:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;	// There's no special setting in Vulkan

    case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case rhi::FilterMode::COMPARISON_ANISOTROPIC:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;	// There's no special setting in Vulkan

    case rhi::FilterMode::UNKNOWN:
      BE_ASSERT(false, "Vulkan filter mode must not be unknown")
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    default:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;	// We should never be in here
  }
}

VkSamplerAddressMode Mapping::get_vulkan_texture_address_mode(rhi::TextureAddressMode textureAddressMode) {
  static constexpr VkSamplerAddressMode MAPPING[] = {
    VK_SAMPLER_ADDRESS_MODE_REPEAT,					// rhi::TextureAddressMode::WRAP
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,		// rhi::TextureAddressMode::MIRROR
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,			// rhi::TextureAddressMode::CLAMP
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,		// rhi::TextureAddressMode::BORDER
    VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE	// rhi::TextureAddressMode::MIRROR_ONCE
  };
  return MAPPING[static_cast<int>(textureAddressMode) - 1];	// Lookout! The "rhi::TextureAddressMode"-values start with 1, not 0
}

VkBlendFactor Mapping::get_vulkan_blend_factor(rhi::Blend blend) {
  static constexpr VkBlendFactor MAPPING[] = {
    VK_BLEND_FACTOR_ZERO,						// rhi::Blend::ZERO				 = 1
    VK_BLEND_FACTOR_ONE,						// rhi::Blend::ONE				 = 2
    VK_BLEND_FACTOR_SRC_COLOR,					// rhi::Blend::SRC_COLOR		 = 3
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,		// rhi::Blend::INV_SRC_COLOR	 = 4
    VK_BLEND_FACTOR_SRC_ALPHA,					// rhi::Blend::SRC_ALPHA		 = 5
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,		// rhi::Blend::INV_SRC_ALPHA	 = 6
    VK_BLEND_FACTOR_DST_ALPHA,					// rhi::Blend::DEST_ALPHA		 = 7
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,		// rhi::Blend::INV_DEST_ALPHA	 = 8
    VK_BLEND_FACTOR_DST_COLOR,					// rhi::Blend::DEST_COLOR		 = 9
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,		// rhi::Blend::INV_DEST_COLOR	 = 10
    VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,			// rhi::Blend::SRC_ALPHA_SAT	 = 11
    VK_BLEND_FACTOR_MAX_ENUM,					// <undefined>					 = 12 !
    VK_BLEND_FACTOR_MAX_ENUM,					// <undefined>					 = 13 !
    VK_BLEND_FACTOR_CONSTANT_COLOR,				// rhi::Blend::BLEND_FACTOR		 = 14
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,	// rhi::Blend::INV_BLEND_FACTOR  = 15
    VK_BLEND_FACTOR_SRC1_COLOR,					// rhi::Blend::SRC_1_COLOR		 = 16
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,		// rhi::Blend::INV_SRC_1_COLOR	 = 17
    VK_BLEND_FACTOR_SRC1_ALPHA,					// rhi::Blend::SRC_1_ALPHA		 = 18
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA		// rhi::Blend::INV_SRC_1_ALPHA	 = 19
  };
  return MAPPING[static_cast<int>(blend) - 1];	// Lookout! The "rhi::Blend"-values start with 1, not 0, there are also holes
}

VkBlendOp Mapping::get_vulkan_blend_op(rhi::BlendOp blendOp) {
  static constexpr VkBlendOp MAPPING[] = {
    VK_BLEND_OP_ADD,				// rhi::BlendOp::ADD
    VK_BLEND_OP_SUBTRACT,			// rhi::BlendOp::SUBTRACT
    VK_BLEND_OP_REVERSE_SUBTRACT,	// rhi::BlendOp::REV_SUBTRACT
    VK_BLEND_OP_MIN,				// rhi::BlendOp::MIN
    VK_BLEND_OP_MAX					// rhi::BlendOp::MAX
  };
  return MAPPING[static_cast<int>(blendOp) - 1];	// Lookout! The "rhi::Blend"-values start with 1, not 0
}

VkCompareOp Mapping::get_vulkan_comparison_func(rhi::ComparisonFunc comparisonFunc) {
  static constexpr VkCompareOp MAPPING[] = {
    VK_COMPARE_OP_NEVER,			// rhi::ComparisonFunc::NEVER
    VK_COMPARE_OP_LESS,				// rhi::ComparisonFunc::LESS
    VK_COMPARE_OP_EQUAL,			// rhi::ComparisonFunc::EQUAL
    VK_COMPARE_OP_LESS_OR_EQUAL,	// rhi::ComparisonFunc::LESS_EQUAL
    VK_COMPARE_OP_GREATER,			// rhi::ComparisonFunc::GREATER
    VK_COMPARE_OP_NOT_EQUAL,		// rhi::ComparisonFunc::NOT_EQUAL
    VK_COMPARE_OP_GREATER_OR_EQUAL,	// rhi::ComparisonFunc::GREATER_EQUAL
    VK_COMPARE_OP_ALWAYS			// rhi::ComparisonFunc::ALWAYS
  };
  return MAPPING[static_cast<int>(comparisonFunc) - 1];	// Lookout! The "rhi::ComparisonFunc"-values start with 1, not 0
}

VkFormat Mapping::get_vulkan_format(rhi::VertexAttributeFormat vertexAttributeFormat) {
  static constexpr VkFormat MAPPING[] = {
    VK_FORMAT_R32_SFLOAT,			// rhi::VertexAttributeFormat::FLOAT_1
    VK_FORMAT_R32G32_SFLOAT,		// rhi::VertexAttributeFormat::FLOAT_2
    VK_FORMAT_R32G32B32_SFLOAT,		// rhi::VertexAttributeFormat::FLOAT_3
    VK_FORMAT_R32G32B32A32_SFLOAT,	// rhi::VertexAttributeFormat::FLOAT_4
    VK_FORMAT_R8G8B8A8_UNORM,		// rhi::VertexAttributeFormat::R8G8B8A8_UNORM
    VK_FORMAT_R8G8B8A8_UINT,		// rhi::VertexAttributeFormat::R8G8B8A8_UINT
    VK_FORMAT_R16G16_SINT,			// rhi::VertexAttributeFormat::SHORT_2
    VK_FORMAT_R16G16B16A16_SINT,	// rhi::VertexAttributeFormat::SHORT_4
    VK_FORMAT_R32_UINT				// rhi::VertexAttributeFormat::UINT_1
  };
  return MAPPING[static_cast<int>(vertexAttributeFormat)];
}

VkIndexType Mapping::get_vulkan_type(rhi::IndexBufferFormat::Enum indexBufferFormat) {
  BE_ASSERT(rhi::IndexBufferFormat::UNSIGNED_CHAR != indexBufferFormat, "One byte per element index buffer format isn't supported by Vulkan")
  static constexpr VkIndexType MAPPING[] = {
    VK_INDEX_TYPE_MAX_ENUM,	// rhi::IndexBufferFormat::UNSIGNED_CHAR  - One byte per element, core::uint8 (may not be supported by each API) - Not supported by Vulkan
    VK_INDEX_TYPE_UINT16,	// rhi::IndexBufferFormat::UNSIGNED_SHORT - Two bytes per element, core::uint16
    VK_INDEX_TYPE_UINT32	// rhi::IndexBufferFormat::UNSIGNED_INT   - Four bytes per element, core::uint32 (may not be supported by each API)
  };
  return MAPPING[indexBufferFormat];
}

VkPrimitiveTopology Mapping::get_vulkan_type(rhi::PrimitiveTopology primitiveTopology) {
  // Tessellation support: Up to 32 vertices per patch are supported "rhi::PrimitiveTopology::PATCH_LIST_1" ... "rhi::PrimitiveTopology::PATCH_LIST_32"
  if (primitiveTopology >= rhi::PrimitiveTopology::PATCH_LIST_1) {
    // Use tessellation
    return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
  } else {
    static constexpr VkPrimitiveTopology MAPPING[] = {
      VK_PRIMITIVE_TOPOLOGY_POINT_LIST,					// rhi::PrimitiveTopology::POINT_LIST
      VK_PRIMITIVE_TOPOLOGY_LINE_LIST,					// rhi::PrimitiveTopology::LINE_LIST
      VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,					// rhi::PrimitiveTopology::LINE_STRIP
      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,				// rhi::PrimitiveTopology::TRIANGLE_LIST
      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,				// rhi::PrimitiveTopology::TRIANGLE_STRIP
      VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,						// Unused value 6
      VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,						// Unused value 7
      VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,						// Unused value 8
      VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,						// Unused value 9
      VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,		// rhi::PrimitiveTopology::LINE_LIST_ADJ
      VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,	// rhi::PrimitiveTopology::LINE_STRIP_ADJ
      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,	// rhi::PrimitiveTopology::TRIANGLE_LIST_ADJ
      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY	// rhi::PrimitiveTopology::TRIANGLE_STRIP_ADJ
    };
    return MAPPING[static_cast<int>(primitiveTopology) - 1];	// Lookout! The "rhi::PrimitiveTopology"-values start with 1, not 0
  }
}

VkFormat Mapping::get_vulkan_format(rhi::TextureFormat::Enum textureFormat) {
  static constexpr VkFormat MAPPING[] = {
    VK_FORMAT_R8_UNORM,					// rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
    VK_FORMAT_R8G8B8_UNORM,				// rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
    VK_FORMAT_R8G8B8A8_UNORM,			// rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
    VK_FORMAT_R8G8B8A8_SRGB,			// rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
    VK_FORMAT_B8G8R8A8_UNORM,			// rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
    VK_FORMAT_B10G11R11_UFLOAT_PACK32,	// rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
    VK_FORMAT_R16G16B16A16_SFLOAT,		// rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
    VK_FORMAT_R32G32B32A32_SFLOAT,		// rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
    VK_FORMAT_BC1_RGB_UNORM_BLOCK,		// rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
    VK_FORMAT_BC1_RGB_SRGB_BLOCK,		// rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
    VK_FORMAT_BC2_UNORM_BLOCK,			// rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
    VK_FORMAT_BC2_SRGB_BLOCK,			// rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
    VK_FORMAT_BC3_UNORM_BLOCK,			// rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
    VK_FORMAT_BC3_SRGB_BLOCK,			// rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
    VK_FORMAT_BC4_UNORM_BLOCK,			// rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
    VK_FORMAT_BC5_UNORM_BLOCK,			// rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
    VK_FORMAT_UNDEFINED,				// rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in Direct3D 11 - TODO(naetherm) Check for Vulkan format
    VK_FORMAT_R16_UNORM,				// rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
    VK_FORMAT_R32_UINT,					// rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
    VK_FORMAT_R32_SFLOAT,				// rhi::TextureFormat::R32_FLOAT     - 32-bit float format
    VK_FORMAT_D32_SFLOAT,				// rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
    VK_FORMAT_R16G16_UNORM,				// rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
    VK_FORMAT_R16G16_SFLOAT,			// rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
    VK_FORMAT_UNDEFINED					// rhi::TextureFormat::UNKNOWN       - Unknown
  };
  return MAPPING[textureFormat];
}

VkSampleCountFlagBits Mapping::get_vulkan_sample_count_flag_bits(core::uint8 numberOfMultisamples) {
  BE_ASSERT(numberOfMultisamples <= 8, "Invalid number of Vulkan multisamples")
  static constexpr VkSampleCountFlagBits MAPPING[] = {
    VK_SAMPLE_COUNT_1_BIT,
    VK_SAMPLE_COUNT_2_BIT,
    VK_SAMPLE_COUNT_4_BIT,
    VK_SAMPLE_COUNT_8_BIT
  };
  return MAPPING[numberOfMultisamples - 1];	// Lookout! The "numberOfMultisamples"-values start with 1, not 0
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan