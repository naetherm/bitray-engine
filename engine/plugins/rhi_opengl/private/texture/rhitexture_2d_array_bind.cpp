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
#include "rhi_opengl/texture/rhitexture_2d_array_bind.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHITexture2DArrayBind::RHITexture2DArrayBind(RHIDevice &openGLRhi, rhi::TextureDescriptor descriptor, const void *data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHITexture2DArray(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently set alignment
  GLint openGLAlignmentBackup = 0;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

  // Backup the currently bound OpenGL texture
  GLint openGLTextureBackup = 0;
  glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY_EXT, &openGLTextureBackup);
#endif

  // Set correct unpack alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, (rhi::TextureFormat::getNumberOfBytesPerElement(descriptor.textureFormat) & 3) ? 1 : 4);

  // Create the OpenGL texture instance
  glGenTextures(1, &mOpenGLTexture);

  // Make this OpenGL texture instance to the currently used one
  glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, mOpenGLTexture);

  // TODO(naetherm) Add support for user provided mipmaps
  // Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
  //   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
  //   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
  //   etc.

  // Upload the base map of the texture (mipmaps are automatically created as soon as the base map is changed)
  glTexImage3DEXT(GL_TEXTURE_2D_ARRAY_EXT, 0, mOpenGLInternalFormat, static_cast<GLsizei>(descriptor.size.width),
                  static_cast<GLsizei>(descriptor.size.height), static_cast<GLsizei>(descriptor.size.depth), 0,
                  Mapping::get_opengl_format(descriptor.textureFormat), Mapping::get_opengl_type(descriptor.textureFormat), data);

  // Build mipmaps automatically on the GPU? (or GPU driver)
  if ((descriptor.textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS) &&
    openGLRhi.get_extensions().isGL_ARB_framebuffer_object()) {
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY_EXT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  } else {
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL texture
  glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, static_cast<GLuint>(openGLTextureBackup));

  // Restore previous alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture array", 19)	// 19 = "2D texture array: " including terminating zero
    glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
  }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
