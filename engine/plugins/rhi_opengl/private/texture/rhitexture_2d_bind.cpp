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
#include "rhi_opengl/texture/rhitexture_2d_bind.h"
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
RHITexture2DBind::RHITexture2DBind(RHIDevice &openGLRhi, rhi::TextureDescriptor descriptor, const void *data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHITexture2D(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
  // Sanity checks
  BE_ASSERT(descriptor.numberOfMultisamples == 1 || descriptor.numberOfMultisamples == 2 || descriptor.numberOfMultisamples == 4 || descriptor.numberOfMultisamples == 8, "Invalid OpenGL texture parameters")
  BE_ASSERT(descriptor.numberOfMultisamples == 1 || nullptr == data, "Invalid OpenGL texture parameters")
  BE_ASSERT(descriptor.numberOfMultisamples == 1 || 0 == (descriptor.textureFlags & rhi::TextureFlag::DATA_CONTAINS_MIPMAPS), "Invalid OpenGL texture parameters")
  BE_ASSERT(descriptor.numberOfMultisamples == 1 || 0 == (descriptor.textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS), "Invalid OpenGL texture parameters")
  BE_ASSERT(descriptor.numberOfMultisamples == 1 || 0 != (descriptor.textureFlags & rhi::TextureFlag::RENDER_TARGET), "Invalid OpenGL texture parameters")
  BE_ASSERT(0 == (descriptor.textureFlags & rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
  BE_ASSERT((descriptor.textureFlags & rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

  // Create the OpenGL texture instance
  glGenTextures(1, &mOpenGLTexture);

//  Multisample texture?
  if (descriptor.numberOfMultisamples > 1) {
#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL texture
    GLint openGLTextureBackup = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, &openGLTextureBackup);
#endif

  // Make this OpenGL texture instance to the currently used one
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mOpenGLTexture);

  // Define the texture
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, descriptor.numberOfMultisamples, mOpenGLInternalFormat,
                            static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height),
                            GL_TRUE);

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL texture
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLuint>(openGLTextureBackup));
#endif
  } else {
#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently set alignment
    GLint openGLAlignmentBackup = 0;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

    // Backup the currently bound OpenGL texture
    GLint openGLTextureBackup = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLTextureBackup);
#endif

    // Set correct unpack alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT,
                  (rhi::TextureFormat::getNumberOfBytesPerElement(descriptor.textureFormat) & 3) ? 1 : 4);

    // Calculate the number of mipmaps
    const bool dataContainsMipmaps = (descriptor.textureFlags & rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
    const bool generateMipmaps = (!dataContainsMipmaps &&
                                  (descriptor.textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS));
    const core::uint32 numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? get_number_of_mipmaps(
      descriptor.size.width, descriptor.size.height) : 1;

    // Make this OpenGL texture instance to the currently used one
    glBindTexture(GL_TEXTURE_2D, mOpenGLTexture);

    // Upload the texture data
    if (rhi::TextureFormat::isCompressed(descriptor.textureFormat)) {
      // Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
      if (dataContainsMipmaps) {
        // Upload all mipmaps
        for (core::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
          // Upload the current mipmap
          const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(rhi::TextureFormat::getNumberOfBytesPerSlice(
            descriptor.textureFormat, descriptor.size.width, descriptor.size.height));
          glCompressedTexImage2DARB(GL_TEXTURE_2D, static_cast<GLint>(mipmap), mOpenGLInternalFormat,
                                    static_cast<GLsizei>(descriptor.size.width),
                                    static_cast<GLsizei>(descriptor.size.height), 0, numberOfBytesPerSlice, data);

          // Move on to the next mipmap and ensure the size is always at least 1x1
          data = static_cast<const core::uint8 *>(data) + numberOfBytesPerSlice;
          descriptor.size.width = get_half_size(descriptor.size.width);
          descriptor.size.height = get_half_size(descriptor.size.height);
        }
      } else {
        // The user only provided us with the base texture, no mipmaps
        glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(descriptor.size.width),
                                  static_cast<GLsizei>(descriptor.size.height), 0,
                                  static_cast<GLsizei>(rhi::TextureFormat::getNumberOfBytesPerSlice(
                                    descriptor.textureFormat, descriptor.size.width, descriptor.size.height)), data);
      }
    } else {
      // Texture format is not compressed

      // Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
      if (dataContainsMipmaps) {
        // Upload all mipmaps
        const core::uint32 format = Mapping::get_opengl_format(descriptor.textureFormat);
        const core::uint32 type = Mapping::get_opengl_type(descriptor.textureFormat);
        for (core::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
          // Upload the current mipmap
          const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(rhi::TextureFormat::getNumberOfBytesPerSlice(
            descriptor.textureFormat, descriptor.size.width, descriptor.size.height));
          glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat),
                       static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height), 0,
                       format, type, data);

          // Move on to the next mipmap and ensure the size is always at least 1x1
          data = static_cast<const core::uint8 *>(data) + numberOfBytesPerSlice;
          descriptor.size.width = get_half_size(descriptor.size.width);
          descriptor.size.height = get_half_size(descriptor.size.height);
        }
      } else {
        // The user only provided us with the base texture, no mipmaps
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(mOpenGLInternalFormat),
                     static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height), 0,
                     Mapping::get_opengl_format(descriptor.textureFormat),
                     Mapping::get_opengl_type(descriptor.textureFormat), data);
      }
    }

    // Build mipmaps automatically on the GPU? (or GPU driver)
    if ((descriptor.textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS) &&
        openGLRhi.get_extensions().isGL_ARB_framebuffer_object()) {
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL texture
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLTextureBackup));

    // Restore previous alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)  // 13 = "2D texture: " including terminating zero
    glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
  }
#endif
}

void RHITexture2DBind::setMinimumMaximumMipmapIndex(core::uint32 minimumMipmapIndex, core::uint32 maximumMipmapIndex) {
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently bound OpenGL texture
  GLint openGLTextureBackup = 0;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLTextureBackup);
#endif

  // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_sampler_objects.txt
  // "
  //  2) What is the set of state associated with a sampler object?
  //     Specifically, should TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL be
  //     part of the sampler or the texture?
  //  DISCUSSION: TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL are presently
  //  part of the image state (texture) and are thus not included in the
  //  sampler object.
  // "
  glBindTexture(GL_TEXTURE_2D, mOpenGLTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, static_cast<GLint>(minimumMipmapIndex));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(maximumMipmapIndex));

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL texture
  glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLTextureBackup));
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
