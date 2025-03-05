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
#include <rhi/render_target/rhirender_pass.h>


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
class RHIRenderPass : public rhi::RHIRenderPass {
  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   *  @brief
   *    Constructor
   *
   *  @param[in] vulkanRhi
   *    Owner Vulkan RHI instance
   *  @param[in] numberOfColorAttachments
   *    Number of color render target textures, must be <="rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets"
   *  @param[in] colorAttachmentTextureFormats
   *    The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
   *    least "numberOfColorAttachments" textures in the provided C-array of pointers
   *  @param[in] depthStencilAttachmentTextureFormat
   *    The optional depth stencil render target texture format, can be a "rhi::TextureFormat::UNKNOWN" if there should be no depth buffer
   *  @param[in] numberOfMultisamples
   *    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
   */
  RHIRenderPass(RHIDevice& vulkanRhi, core::uint32 numberOfColorAttachments, const rhi::TextureFormat::Enum* colorAttachmentTextureFormats, rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, core::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  ~RHIRenderPass() override;


  [[nodiscard]] VkRenderPass get_vk_render_pass() const;

  [[nodiscard]] core::uint32 get_number_of_color_attachments() const;

  [[nodiscard]] core::uint32 get_number_of_attachments() const;

  [[nodiscard]] rhi::TextureFormat::Enum get_depth_stencil_attachment_texture_format() const;

  [[nodiscard]] VkSampleCountFlagBits get_vk_sample_count_flag_bits() const;

protected:

  void self_destruct() override;

private:

  explicit RHIRenderPass(const RHIRenderPass&) = delete;
  RHIRenderPass& operator=(const RHIRenderPass&) = delete;

private:

  VkRenderPass mVkRenderPass;
  core::uint32 mNumberOfColorAttachments;
  rhi::TextureFormat::Enum mDepthStencilAttachmentTextureFormat;
  VkSampleCountFlagBits mVkSampleCountFlagBits;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan