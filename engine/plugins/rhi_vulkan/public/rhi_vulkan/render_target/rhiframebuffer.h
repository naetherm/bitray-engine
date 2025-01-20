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
 *  @brief
 *    Vulkan framebuffer interface
 */
class RHIFramebuffer final : public rhi::RHIFramebuffer
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   *  @brief
   *    Constructor
   *
   *  @param[in] renderPass
   *    Render pass to use, the swap chain keeps a reference to the render pass
   *  @param[in] colorFramebufferAttachments
   *    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
   *    least "rhi::RHIRenderPass::get_number_of_color_attachments()" textures in the provided C-array of pointers
   *  @param[in] depthStencilFramebufferAttachment
   *    The depth stencil render target texture, can be a null pointer
   *
   *  @note
   *    - The framebuffer keeps a reference to the provided texture instances
   */
  RHIFramebuffer(rhi::RHIRenderPass& renderPass, const rhi::FramebufferAttachment* colorFramebufferAttachments, const rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   *  @brief
   *    Destructor
   */
  ~RHIFramebuffer() override;

  /**
   *  @brief
   *    Return the Vulkan render pass
   *
   *  @return
   *    The Vulkan render pass
   */
  [[nodiscard]] VkRenderPass get_vk_render_pass() const;

  /**
   *  @brief
   *    Return the Vulkan framebuffer to render into
   *
   *  @return
   *    The Vulkan framebuffer to render into
   */
  [[nodiscard]] VkFramebuffer get_vk_framebuffer() const;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIRenderTarget methods             ]
  //[-------------------------------------------------------]
public:
  void get_width_and_height(core::uint32& width, core::uint32& height) const override;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  void self_destruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIFramebuffer(const RHIFramebuffer& source) = delete;
  RHIFramebuffer& operator =(const RHIFramebuffer& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** Number of color render target textures */
  core::uint32		mNumberOfColorTextures;
  /** The color render target textures (we keep a reference to it), can be a null pointer or can contain null pointers, if not a null pointer there must be at least "mNumberOfColorTextures" textures in the provided C-array of pointers */
  rhi::RHITexture** mColorTextures;
  /** The depth stencil render target texture (we keep a reference to it), can be a null pointer */
  rhi::RHITexture*  mDepthStencilTexture;
  /** The framebuffer width */
  core::uint32		mWidth;
  /** The framebuffer height */
  core::uint32		mHeight;
  /** Vulkan render pass instance, can be a null handle, we don't own it */
  VkRenderPass	mVkRenderPass;
  /** Vulkan framebuffer instance, can be a null handle */
  VkFramebuffer	mVkFramebuffer;			
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
