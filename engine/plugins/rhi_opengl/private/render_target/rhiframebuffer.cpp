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
#include "rhi_opengl/render_target/rhiframebuffer.h"
#include "rhi_opengl/render_target/rhirender_pass.h"
#include "rhi_opengl/texture/rhitexture_2d.h"
#include "rhi_opengl/texture/rhitexture_2d_array.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIFramebuffer::~RHIFramebuffer() {
  // Destroy the OpenGL framebuffer
  // -> Silently ignores 0's and names that do not correspond to existing buffer objects
  glDeleteFramebuffers(1, &mOpenGLFramebuffer);

  // release the reference to the used color textures
  if (nullptr != mColorTextures) {
    // release references
    rhi::RHITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (rhi::RHITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture) {
      (*colorTexture)->release();
    }

    // Cleanup
    re_free(mColorTextures);
  }

  // release the reference to the used depth stencil texture
  if (nullptr != mDepthStencilTexture) {
    // release reference
    mDepthStencilTexture->release();
  }
}

RHIFramebuffer::RHIFramebuffer(rhi::RHIRenderPass& renderPass, const rhi::FramebufferAttachment* colorFramebufferAttachments, const rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLFramebuffer(0)
, mNumberOfColorTextures(static_cast<RHIRenderPass &>(renderPass).get_number_of_color_attachments())
, mColorTextures(nullptr)	// Set below
, mDepthStencilTexture(nullptr)
, mWidth(UINT_MAX)
, mHeight(UINT_MAX)
, mMultisampleRenderTarget(false) {
  // The "GL_ARB_framebuffer_object"-extension documentation says the following about the framebuffer width and height
  //   "If the attachment sizes are not all identical, rendering will be limited to the largest area that can fit in
  //    all of the attachments (i.e. an intersection of rectangles having a lower left of (0,0) and an upper right of
  //    (width,height) for each attachment)"

  // Add a reference to the used color textures
  if (mNumberOfColorTextures > 0) {
    mColorTextures = re_typed_alloc<rhi::RHITexture*>(mNumberOfColorTextures);

    // Loop through all color textures
    rhi::RHITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (rhi::RHITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture, ++colorFramebufferAttachments) {
      // Sanity check
      BE_ASSERT(nullptr != colorFramebufferAttachments->texture, "Invalid OpenGL color framebuffer attachment texture")

      // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
      *colorTexture = colorFramebufferAttachments->texture;
      (*colorTexture)->add_ref();

      // Evaluate the color texture type
      switch ((*colorTexture)->get_resource_type()) {
        case rhi::ResourceType::TEXTURE_2D:
        {
          const RHITexture2D* texture2D = static_cast<RHITexture2D*>(*colorTexture);

          // Sanity checks
          BE_ASSERT(colorFramebufferAttachments->mipmapIndex <
                    RHITexture2D::get_number_of_mipmaps(texture2D->get_width(), texture2D->get_height()), "Invalid OpenGL color framebuffer attachment mipmap index")
          BE_ASSERT(0 == colorFramebufferAttachments->layerIndex, "Invalid OpenGL color framebuffer attachment layer index")

          // Update the framebuffer width and height if required
          Helper::update_width_height(colorFramebufferAttachments->mipmapIndex, texture2D->get_width(), texture2D->get_height(), mWidth, mHeight);
          break;
        }

        case rhi::ResourceType::TEXTURE_2D_ARRAY:
        {
          // Update the framebuffer width and height if required
          const RHITexture2DArray* texture2DArray = static_cast<RHITexture2DArray*>(*colorTexture);
          Helper::update_width_height(colorFramebufferAttachments->mipmapIndex, texture2DArray->get_width(), texture2DArray->get_height(), mWidth, mHeight);
          break;
        }

        case rhi::ResourceType::ROOT_SIGNATURE:
        case rhi::ResourceType::RESOURCE_GROUP:
        case rhi::ResourceType::GRAPHICS_PROGRAM:
        case rhi::ResourceType::VERTEX_ARRAY:
        case rhi::ResourceType::RENDER_PASS:
        case rhi::ResourceType::QUERY_POOL:
        case rhi::ResourceType::SWAP_CHAIN:
        case rhi::ResourceType::FRAMEBUFFER:
        case rhi::ResourceType::VERTEX_BUFFER:
        case rhi::ResourceType::INDEX_BUFFER:
        case rhi::ResourceType::TEXTURE_BUFFER:
        case rhi::ResourceType::STRUCTURED_BUFFER:
        case rhi::ResourceType::INDIRECT_BUFFER:
        case rhi::ResourceType::UNIFORM_BUFFER:
        case rhi::ResourceType::TEXTURE_1D:
        case rhi::ResourceType::TEXTURE_1D_ARRAY:
        case rhi::ResourceType::TEXTURE_3D:
        case rhi::ResourceType::TEXTURE_CUBE:
        case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
        case rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
        case rhi::ResourceType::COMPUTE_PIPELINE_STATE:
        case rhi::ResourceType::SAMPLER_STATE:
        case rhi::ResourceType::VERTEX_SHADER:
        case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
        case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
        case rhi::ResourceType::GEOMETRY_SHADER:
        case rhi::ResourceType::FRAGMENT_SHADER:
        case rhi::ResourceType::TASK_SHADER:
        case rhi::ResourceType::MESH_SHADER:
        case rhi::ResourceType::COMPUTE_SHADER:
        default:
          // Nothing here
          break;
      }
    }
  }

  // Add a reference to the used depth stencil texture
  if (nullptr != depthStencilFramebufferAttachment) {
    mDepthStencilTexture = depthStencilFramebufferAttachment->texture;
    BE_ASSERT(nullptr != mDepthStencilTexture, "Invalid OpenGL depth stencil framebuffer attachment texture")
    mDepthStencilTexture->add_ref();

    // Evaluate the depth stencil texture type
    switch (mDepthStencilTexture->get_resource_type()) {
      case rhi::ResourceType::TEXTURE_2D:
      {
        const RHITexture2D* texture2D = static_cast<RHITexture2D*>(mDepthStencilTexture);

        // Sanity checks
        BE_ASSERT(depthStencilFramebufferAttachment->mipmapIndex < RHITexture2D::get_number_of_mipmaps(texture2D->get_width(), texture2D->get_height()), "Invalid OpenGL depth stencil framebuffer attachment mipmap index")
        BE_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex, "Invalid OpenGL depth stencil framebuffer attachment layer index")

        // Update the framebuffer width and height if required
        Helper::update_width_height(depthStencilFramebufferAttachment->mipmapIndex, texture2D->get_width(), texture2D->get_height(), mWidth, mHeight);
        break;
      }

      case rhi::ResourceType::TEXTURE_2D_ARRAY:
      {
        // Update the framebuffer width and height if required
        const RHITexture2DArray* texture2DArray = static_cast<RHITexture2DArray*>(mDepthStencilTexture);
        Helper::update_width_height(depthStencilFramebufferAttachment->mipmapIndex, texture2DArray->get_width(), texture2DArray->get_height(), mWidth, mHeight);
        break;
      }

      case rhi::ResourceType::ROOT_SIGNATURE:
      case rhi::ResourceType::RESOURCE_GROUP:
      case rhi::ResourceType::GRAPHICS_PROGRAM:
      case rhi::ResourceType::VERTEX_ARRAY:
      case rhi::ResourceType::RENDER_PASS:
      case rhi::ResourceType::QUERY_POOL:
      case rhi::ResourceType::SWAP_CHAIN:
      case rhi::ResourceType::FRAMEBUFFER:
      case rhi::ResourceType::VERTEX_BUFFER:
      case rhi::ResourceType::INDEX_BUFFER:
      case rhi::ResourceType::TEXTURE_BUFFER:
      case rhi::ResourceType::STRUCTURED_BUFFER:
      case rhi::ResourceType::INDIRECT_BUFFER:
      case rhi::ResourceType::UNIFORM_BUFFER:
      case rhi::ResourceType::TEXTURE_1D:
      case rhi::ResourceType::TEXTURE_1D_ARRAY:
      case rhi::ResourceType::TEXTURE_3D:
      case rhi::ResourceType::TEXTURE_CUBE:
      case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
      case rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
      case rhi::ResourceType::COMPUTE_PIPELINE_STATE:
      case rhi::ResourceType::SAMPLER_STATE:
      case rhi::ResourceType::VERTEX_SHADER:
      case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
      case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
      case rhi::ResourceType::GEOMETRY_SHADER:
      case rhi::ResourceType::FRAGMENT_SHADER:
      case rhi::ResourceType::TASK_SHADER:
      case rhi::ResourceType::MESH_SHADER:
      case rhi::ResourceType::COMPUTE_SHADER:
      default:
        // Nothing here
        break;
    }
  }

  // Validate the framebuffer width and height
  if (0 == mWidth || UINT_MAX == mWidth) {
    BE_ASSERT(false, "Invalid OpenGL framebuffer width")
    mWidth = 1;
  }
  if (0 == mHeight || UINT_MAX == mHeight) {
    BE_ASSERT(false, "Invalid OpenGL framebuffer height")
    mHeight = 1;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
