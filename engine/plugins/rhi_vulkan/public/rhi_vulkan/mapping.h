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
#include "rhi_vulkan/rhi_vulkan.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Mapping
 *
 * @brief
 * Mapping helper between vulkan and internal formats.
 */
class Mapping {
  //[-------------------------------------------------------]
  //[ Public static methods                                 ]
  //[-------------------------------------------------------]
public:
  //[-------------------------------------------------------]
  //[ rhi::FilterMode                                    ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    "rhi::FilterMode" to Vulkan magnification filter mode
   *
   *  @param[in] context
   *    RHI context to use
   *  @param[in] filterMode
   *    "rhi::FilterMode" to map
   *
   *  @return
   *    Vulkan magnification filter mode
   */
  [[nodiscard]] static VkFilter get_vulkan_mag_filter_mode(rhi::FilterMode filterMode);

  /**
   *  @brief
   *    "rhi::FilterMode" to Vulkan minification filter mode
   *
   *  @param[in] context
   *    RHI context to use
   *  @param[in] filterMode
   *    "rhi::FilterMode" to map
   *
   *  @return
   *    Vulkan minification filter mode
   */
  [[nodiscard]] static VkFilter get_vulkan_min_filter_mode(rhi::FilterMode filterMode);

  /**
   *  @brief
   *    "rhi::FilterMode" to Vulkan sampler mipmap mode
   *
   *  @param[in] context
   *    Rhi context to use
   *  @param[in] filterMode
   *    "rhi::FilterMode" to map
   *
   *  @return
   *    Vulkan sampler mipmap mode
   */
  [[nodiscard]] static VkSamplerMipmapMode get_vulkan_mipmap_mode(rhi::FilterMode filterMode);

  //[-------------------------------------------------------]
  //[ rhi::TextureAddressMode                            ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    "rhi::TextureAddressMode" to Vulkan texture address mode
   *
   *  @param[in] textureAddressMode
   *    "rhi::TextureAddressMode" to map
   *
   *  @return
   *    Vulkan texture address mode
   */
  [[nodiscard]] static VkSamplerAddressMode get_vulkan_texture_address_mode(rhi::TextureAddressMode textureAddressMode);

  //[-------------------------------------------------------]
  //[ rhi::Blend                                         ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    "rhi::Blend" to Vulkan blend factor
   *
   *  @param[in] blend
   *    "rhi::Blend" to map
   *
   *  @return
   *    Vulkan blend factor
   */
  [[nodiscard]] static VkBlendFactor get_vulkan_blend_factor(rhi::Blend blend);

  /**
   *  @brief
   *    "rhi::BlendOp" to Vulkan blend operation
   *
   *  @param[in] blendOp
   *    "rhi::BlendOp" to map
   *
   *  @return
   *    Vulkan blend operation
   */
  [[nodiscard]] static VkBlendOp get_vulkan_blend_op(rhi::BlendOp blendOp);

  //[-------------------------------------------------------]
  //[ rhi::ComparisonFunc                                ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    "rhi::ComparisonFunc" to Vulkan comparison function
   *
   *  @param[in] comparisonFunc
   *    "rhi::ComparisonFunc" to map
   *
   *  @return
   *    Vulkan comparison function
   */
  [[nodiscard]] static VkCompareOp get_vulkan_comparison_func(rhi::ComparisonFunc comparisonFunc);

  //[-------------------------------------------------------]
  //[ rhi::VertexAttributeFormat and semantic            ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    "rhi::VertexAttributeFormat" to Vulkan format
   *
   *  @param[in] vertexAttributeFormat
   *    "rhi::VertexAttributeFormat" to map
   *
   *  @return
   *    Vulkan format
   */
  [[nodiscard]] static VkFormat get_vulkan_format(rhi::VertexAttributeFormat vertexAttributeFormat);

  //[-------------------------------------------------------]
  //[ rhi::IndexBufferFormat                             ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    "rhi::IndexBufferFormat" to Vulkan type
   *
   *  @param[in] context
   *    RHI context to use
   *  @param[in] indexBufferFormat
   *    "rhi::IndexBufferFormat" to map
   *
   *  @return
   *    Vulkan index type
   */
  [[nodiscard]] static VkIndexType get_vulkan_type(rhi::IndexBufferFormat::Enum indexBufferFormat);

  //[-------------------------------------------------------]
  //[ rhi::PrimitiveTopology                             ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    "rhi::PrimitiveTopology" to Vulkan type
   *
   *  @param[in] primitiveTopology
   *    "rhi::PrimitiveTopology" to map
   *
   *  @return
   *    Vulkan type
   */
  [[nodiscard]] static VkPrimitiveTopology get_vulkan_type(rhi::PrimitiveTopology primitiveTopology);

  //[-------------------------------------------------------]
  //[ rhi::TextureFormat                                 ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    "rhi::TextureFormat" to Vulkan format
   *
   *  @param[in] textureFormat
   *    "rhi::TextureFormat" to map
   *
   *  @return
   *    Vulkan format
   */
  [[nodiscard]] static VkFormat get_vulkan_format(rhi::TextureFormat::Enum textureFormat);

  /**
   *  @brief
   *    Number of multisamples to Vulkan sample count flag bits
   *
   *  @param[in] context
   *    RHI context to use
   *  @param[in] numberOfMultisamples
   *    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
   *
   *  @return
   *    Vulkan sample count flag bits
   */
  [[nodiscard]] static VkSampleCountFlagBits get_vulkan_sample_count_flag_bits(core::uint8 numberOfMultisamples);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan