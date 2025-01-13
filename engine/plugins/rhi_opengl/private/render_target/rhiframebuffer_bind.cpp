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
#include "rhi_opengl/render_target/rhiframebuffer_bind.h"
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
RHIFramebufferBind::RHIFramebufferBind(rhi::RHIRenderPass &renderPass, const rhi::FramebufferAttachment *colorFramebufferAttachments, const rhi::FramebufferAttachment *depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIFramebuffer(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
  // Texture reference handling is done within the base class "Framebuffer"
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently bound OpenGL framebuffer
  GLint openGLFramebufferBackup = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &openGLFramebufferBackup);
#endif

  // Create the OpenGL framebuffer
  glGenFramebuffers(1, &mOpenGLFramebuffer);

  // Bind this OpenGL framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, mOpenGLFramebuffer);

  // Loop through all framebuffer color attachments
#ifdef DEBUG
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(renderPass.get_rhi());
#endif
  const rhi::FramebufferAttachment *colorFramebufferAttachment = colorFramebufferAttachments;
  const rhi::FramebufferAttachment *colorFramebufferAttachmentEnd = colorFramebufferAttachments + mNumberOfColorTextures;
  for (GLenum openGLAttachment = GL_COLOR_ATTACHMENT0; colorFramebufferAttachment < colorFramebufferAttachmentEnd; ++colorFramebufferAttachment, ++openGLAttachment) {
    rhi::RHITexture *texture = colorFramebufferAttachment->texture;

    // Sanity check: Is the given resource owned by this RHI?
    BE_ASSERT(&openGLRhi == &texture->get_rhi(), "OpenGL error: The given color texture at index %u is owned by another RHI instance", colorFramebufferAttachment - colorFramebufferAttachments)

    // Evaluate the color texture type
    switch (texture->get_resource_type()) {
      case rhi::ResourceType::TEXTURE_2D: {
        // Set the OpenGL framebuffer color attachment
        const RHITexture2D *texture2D = static_cast<const RHITexture2D *>(texture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, openGLAttachment,
                               static_cast<GLenum>((texture2D->get_number_of_multisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->get_opengl_texture(),
                               static_cast<GLint>(colorFramebufferAttachment->mipmapIndex));
        if (!mMultisampleRenderTarget && texture2D->get_number_of_multisamples() > 1) {
          mMultisampleRenderTarget = true;
        }
        break;
      }

      case rhi::ResourceType::TEXTURE_2D_ARRAY: {
        // Set the OpenGL framebuffer color attachment
        const RHITexture2DArray *texture2DArray = static_cast<const RHITexture2DArray *>(texture);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, openGLAttachment, texture2DArray->get_opengl_texture(),
                                  static_cast<GLint>(colorFramebufferAttachment->mipmapIndex),
                                  static_cast<GLint>(colorFramebufferAttachment->layerIndex));
        if (!mMultisampleRenderTarget && texture2DArray->get_number_of_multisamples() > 1) {
          mMultisampleRenderTarget = true;
        }
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
        BE_ASSERT(false, "The type of the given color texture at index %ld is not supported by the OpenGL RHI implementation", colorFramebufferAttachment - colorFramebufferAttachments)
        break;
    }
  }

  // Depth stencil texture
  if (nullptr != mDepthStencilTexture) {
    // Sanity check: Is the given resource owned by this RHI?
    BE_ASSERT(&openGLRhi == &mDepthStencilTexture->get_rhi(), "OpenGL error: The given depth stencil texture is owned by another RHI instance")

    // Evaluate the depth stencil texture type
    switch (mDepthStencilTexture->get_resource_type()) {
      case rhi::ResourceType::TEXTURE_2D: {
        const RHITexture2D *texture2D = static_cast<const RHITexture2D *>(mDepthStencilTexture);

        // Sanity checks
        BE_ASSERT(depthStencilFramebufferAttachment->mipmapIndex < RHITexture2D::get_number_of_mipmaps(texture2D->get_width(), texture2D->get_height()), "Invalid OpenGL depth stencil framebuffer attachment mipmap index")
        BE_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex, "Invalid OpenGL depth stencil framebuffer attachment layer index")

        // Bind the depth stencil texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               static_cast<GLenum>((texture2D->get_number_of_multisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
                               texture2D->get_opengl_texture(),
                               static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex));
        if (!mMultisampleRenderTarget && texture2D->get_number_of_multisamples() > 1) {
          mMultisampleRenderTarget = true;
        }
        break;
      }

      case rhi::ResourceType::TEXTURE_2D_ARRAY: {
        // Bind the depth stencil texture to framebuffer
        const RHITexture2DArray *texture2DArray = static_cast<const RHITexture2DArray *>(mDepthStencilTexture);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture2DArray->get_opengl_texture(),
                                  static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex),
                                  static_cast<GLint>(depthStencilFramebufferAttachment->layerIndex));
        if (!mMultisampleRenderTarget && texture2DArray->get_number_of_multisamples() > 1) {
          mMultisampleRenderTarget = true;
        }
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
        BE_ASSERT(false,
                  "OpenGL error: The type of the given depth stencil texture is not supported by the OpenGL RHI implementation")
        break;
    }
  }

  // Check the status of the OpenGL framebuffer
  const GLenum openGLStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (openGLStatus) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      BE_ASSERT(false, "OpenGL error: Not all framebuffer attachment points are framebuffer attachment complete (\"GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      BE_ASSERT(false, "OpenGL error: No images are attached to the framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      BE_ASSERT(false, "OpenGL error: Incomplete draw buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      BE_ASSERT(false, "OpenGL error: Incomplete read buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      BE_ASSERT(false, "OpenGL error: Incomplete multisample framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\")")
      break;

    case GL_FRAMEBUFFER_UNDEFINED:
      BE_ASSERT(false, "OpenGL error: Undefined framebuffer (\"GL_FRAMEBUFFER_UNDEFINED\")")
      break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
      BE_ASSERT(false, "OpenGL error: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions (\"GL_FRAMEBUFFER_UNSUPPORTED\")")
      break;

    // From "GL_EXT_framebuffer_object" (should no longer matter, should)
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      BE_ASSERT(false, "OpenGL error: Not all attached images have the same width and height (\"GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\")")
      break;

    // From "GL_EXT_framebuffer_object" (should no longer matter, should)
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      BE_ASSERT(false, "OpenGL error: Incomplete formats framebuffer object (\"GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\")")
      break;

    default:
    case GL_FRAMEBUFFER_COMPLETE:
    // Nothing here
      break;
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(openGLFramebufferBackup));
#endif

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
    glObjectLabel(GL_FRAMEBUFFER, mOpenGLFramebuffer, -1, detailedDebugName);
  }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
