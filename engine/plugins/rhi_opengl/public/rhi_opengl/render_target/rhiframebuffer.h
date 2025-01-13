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
 * Abstract OpenGL framebuffer interface
 */
class RHIFramebuffer : public rhi::RHIFramebuffer {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Destructor
   */
  virtual ~RHIFramebuffer() override;

  /**
   * @brief
   * Return the OpenGL framebuffer
   * 
   * @return
   * The OpenGL framebuffer, can be zero if no resource is allocated, do not destroy the returned resource
   */
  [[nodiscard]] inline GLuint get_opengl_framebuffer() const {
    return mOpenGLFramebuffer;
  }

  /**
   * @brief
   * Return the number of color render target textures
   * 
   * @return
   * The number of color render target textures
   */
  [[nodiscard]] inline core::uint32 get_number_of_color_textures() const {
    return mNumberOfColorTextures;
  }

  /**
   * @brief
   * Return whether or not the framebuffer is a multisample render target
   * 
   * @return
   * "true" if the framebuffer is a multisample render target, else "false"
   */
  [[nodiscard]] inline bool is_multisample_render_target() const {
    return mMultisampleRenderTarget;
  }


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIResource methods                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual void *get_internal_resource_handle() const override {
    return reinterpret_cast<void *>(static_cast<uintptr_t>(mOpenGLFramebuffer));
  }


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIRenderTarget methods             ]
  //[-------------------------------------------------------]
public:
  inline virtual void get_width_and_height(core::uint32 &width, core::uint32 &height) const override {
    // No fancy implementation in here, just copy over the internal information
    width = mWidth;
    height = mHeight;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] renderPass
   * Render pass to use, the swap chain keeps a reference to the render pass
   * @param[in] colorFramebufferAttachments
   * The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
   * least "rhi::RHIRenderPass::get_number_of_color_attachments()" textures in the provided C-array of pointers
   * @param[in] depthStencilFramebufferAttachment
   * The depth stencil render target texture, can be a null pointer
   * 
   * @note
   * - The framebuffer keeps a reference to the provided texture instances
   */
  RHIFramebuffer(rhi::RHIRenderPass &renderPass, const rhi::FramebufferAttachment *colorFramebufferAttachments,
                 const rhi::FramebufferAttachment *depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint mOpenGLFramebuffer;      ///< OpenGL framebuffer ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
  core::uint32 mNumberOfColorTextures;    ///< Number of color render target textures
  rhi::RHITexture **mColorTextures;        ///< The color render target textures (we keep a reference to it), can be a null pointer or can contain null pointers, if not a null pointer there must be at least "mNumberOfColorTextures" textures in the provided C-array of pointers
  rhi::RHITexture *mDepthStencilTexture;    ///< The depth stencil render target texture (we keep a reference to it), can be a null pointer
  core::uint32 mWidth;            ///< The framebuffer width
  core::uint32 mHeight;          ///< The framebuffer height
  bool mMultisampleRenderTarget;  ///< Multisample render target?


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIFramebuffer(const RHIFramebuffer &source) = delete;

  RHIFramebuffer &operator=(const RHIFramebuffer &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
