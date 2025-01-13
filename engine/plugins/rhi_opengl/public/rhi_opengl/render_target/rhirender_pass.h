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
#include "rhi_opengl/rhi_opengl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * OpenGL render pass interface
 */
class RHIRenderPass final : public rhi::RHIRenderPass {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] rhi
   * Owner RHI instance
   * @param[in] numberOfColorAttachments
   * Number of color render target textures, must be <="rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets"
   * @param[in] colorAttachmentTextureFormats
   * The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
   * least "numberOfColorAttachments" textures in the provided C-array of pointers
   * @param[in] depthStencilAttachmentTextureFormat
   * The optional depth stencil render target texture format, can be a "rhi::TextureFormat::UNKNOWN" if there should be no depth buffer
   * @param[in] numberOfMultisamples
   * The number of multisamples per pixel (valid values: 1, 2, 4, 8)
   */
  RHIRenderPass(rhi::RHIDevice &rhi, core::uint32 numberOfColorAttachments,
                const rhi::TextureFormat::Enum *colorAttachmentTextureFormats,
                rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat,
                core::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIRenderPass() override = default;

  /**
   * @brief
   * Return the number of color render target textures
   * 
   * @return
   * The number of color render target textures
   */
  [[nodiscard]] inline core::uint32 get_number_of_color_attachments() const {
    return mNumberOfColorAttachments;
  }

  /**
   * @brief
   * Return the number of render target textures (color and depth stencil)
   * 
   * @return
   * The number of render target textures (color and depth stencil)
   */
  [[nodiscard]] inline core::uint32 get_number_of_attachments() const {
    return (mDepthStencilAttachmentTextureFormat != rhi::TextureFormat::Enum::UNKNOWN) ? (mNumberOfColorAttachments +
                                                                                            1)
                                                                                         : mNumberOfColorAttachments;
  }

  /**
   * @brief
   * Return the color attachment texture format
   * 
   * @param[in] colorAttachmentIndex
   * Color attachment index
   * 
   * @return
   * The color attachment texture format
   */
  [[nodiscard]] inline rhi::TextureFormat::Enum
  get_color_attachment_texture_format(core::uint32 colorAttachmentIndex) const {
    BE_ASSERT(colorAttachmentIndex < mNumberOfColorAttachments, "Invalid OpenGL color attachment index")
    return mColorAttachmentTextureFormats[colorAttachmentIndex];
  }

  /**
   * @brief
   * Return the depth stencil attachment texture format
   * 
   * @return
   * The depth stencil attachment texture format
   */
  [[nodiscard]] inline rhi::TextureFormat::Enum get_depth_stencil_attachment_texture_format() const {
    return mDepthStencilAttachmentTextureFormat;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIRenderPass(const RHIRenderPass &source) = delete;

  RHIRenderPass &operator=(const RHIRenderPass &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  core::uint32 mNumberOfColorAttachments;
  rhi::TextureFormat::Enum mColorAttachmentTextureFormats[8];
  rhi::TextureFormat::Enum mDepthStencilAttachmentTextureFormat;
  core::uint8 mNumberOfMultisamples;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
