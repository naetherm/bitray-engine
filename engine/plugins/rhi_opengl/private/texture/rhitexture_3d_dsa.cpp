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
#include "rhi_opengl/texture/rhitexture_3d_dsa.h"
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
RHITexture3DDsa::RHITexture3DDsa(RHIDevice &openGLRhi, rhi::TextureDescriptor descriptor, const void *data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHITexture3D(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
  // Sanity checks
  BE_ASSERT(0 == (descriptor.textureFlags & rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
  BE_ASSERT((descriptor.textureFlags & rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently set alignment
  GLint openGLAlignmentBackup = 0;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

  // Set correct unpack alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, (rhi::TextureFormat::getNumberOfBytesPerElement(descriptor.textureFormat) & 3) ? 1 : 4);

  // Create OpenGL pixel unpack buffer for dynamic textures, if necessary
  if (rhi::TextureUsage::IMMUTABLE != descriptor.textureUsage) {
    // Create the OpenGL pixel unpack buffer
    glCreateBuffers(1, &mOpenGLPixelUnpackBuffer);

    // The OpenGL pixel unpack buffer must be able to hold the top-level mipmap
    // TODO(naetherm) Or must the OpenGL pixel unpack buffer able to hold the entire texture including all mipmaps? Depends on the later usage I assume.
    const core::uint32 numberOfBytes = rhi::TextureFormat::getNumberOfBytesPerSlice(descriptor.textureFormat, descriptor.size.width, descriptor.size.height) * descriptor.size.depth;
    glNamedBufferData(mOpenGLPixelUnpackBuffer, static_cast<GLsizeiptr>(numberOfBytes), nullptr, GL_STREAM_DRAW);
  }

  // Calculate the number of mipmaps
  const bool dataContainsMipmaps = (descriptor.textureFlags & rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
  const bool generateMipmaps = (!dataContainsMipmaps && (descriptor.textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS));
  BE_ASSERT(rhi::TextureUsage::IMMUTABLE != descriptor.textureUsage || !generateMipmaps, "OpenGL immutable texture usage can't be combined with automatic mipmap generation")
  const core::uint32 numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? get_number_of_mipmaps(descriptor.size.width, descriptor.size.height, descriptor.size.depth) : 1;

  // Create the OpenGL texture instance
  const bool isArbDsa = openGLRhi.get_extensions().isGL_ARB_direct_state_access();
  if (isArbDsa) {
    glCreateTextures(GL_TEXTURE_3D, 1, &mOpenGLTexture);
    glTextureStorage3D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height), static_cast<GLsizei>(descriptor.size.depth));
  } else {
    glGenTextures(1, &mOpenGLTexture);
  }

  // Upload the texture data
  if (rhi::TextureFormat::isCompressed(descriptor.textureFormat)) {
  // Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
      // Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
      //   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
      //   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
      //   etc.

      // Upload all mipmaps
      const core::uint32 format = Mapping::get_opengl_format(descriptor.textureFormat);
      for (core::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
        // Upload the current mipmap
        const GLsizei numberOfBytesPerMipmap = static_cast<GLsizei>(rhi::TextureFormat::getNumberOfBytesPerSlice(descriptor.textureFormat, descriptor.size.width, descriptor.size.height) * descriptor.size.depth);
        if (isArbDsa) {
          // We know that "data" must be valid when we're in here due to the "rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
          glCompressedTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0,
                                        static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height),
                                        static_cast<GLsizei>(descriptor.size.depth), format, numberOfBytesPerMipmap, data);
        } else {
          glCompressedTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, static_cast<GLint>(mipmap), format,
                                        static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height),
                                        static_cast<GLsizei>(descriptor.size.depth), 0, numberOfBytesPerMipmap, data);
        }

        // Move on to the next mipmap and ensure the size is always at least 1x1x1
        data = static_cast<const core::uint8 *>(data) + numberOfBytesPerMipmap;
        descriptor.size.width = get_half_size(descriptor.size.width);
        descriptor.size.height = get_half_size(descriptor.size.height);
        descriptor.size.depth = get_half_size(descriptor.size.depth);
      }
    } else {
      // The user only provided us with the base texture, no mipmaps
      if (isArbDsa) {
        if (nullptr != data) {
          glCompressedTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(descriptor.size.width),
                                        static_cast<GLsizei>(descriptor.size.height), static_cast<GLsizei>(descriptor.size.depth),
                                        Mapping::get_opengl_format(descriptor.textureFormat),
                                        static_cast<GLsizei>(rhi::TextureFormat::getNumberOfBytesPerSlice(
                                          descriptor.textureFormat, descriptor.size.width, descriptor.size.height)), data);
        }
      } else {
        glCompressedTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, 0, mOpenGLInternalFormat,
                                      static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height),
                                      static_cast<GLsizei>(descriptor.size.depth), 0,
                                      static_cast<GLsizei>(rhi::TextureFormat::getNumberOfBytesPerSlice(descriptor.textureFormat,
                                                                                                          descriptor.size.width,
                                                                                                          descriptor.size.height)),
                                      data);
      }
    }
  } else {
    // Texture format is not compressed

    // Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
      // Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
      //   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
      //   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
      //   etc.

      // Upload all mipmaps
      const core::uint32 format = Mapping::get_opengl_format(descriptor.textureFormat);
      const core::uint32 type = Mapping::get_opengl_type(descriptor.textureFormat);
      for (core::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
        // Upload the current mipmap
        const GLsizei numberOfBytesPerMipmap = static_cast<GLsizei>(
          rhi::TextureFormat::getNumberOfBytesPerSlice(descriptor.textureFormat, descriptor.size.width, descriptor.size.height) * descriptor.size.depth);
        if (isArbDsa) {
          // We know that "data" must be valid when we're in here due to the "rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
          glTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0, static_cast<GLsizei>(descriptor.size.width),
                              static_cast<GLsizei>(descriptor.size.height), static_cast<GLsizei>(descriptor.size.depth), format, type, data);
        } else {
          glTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, static_cast<GLint>(mipmap),
                              static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(descriptor.size.width),
                              static_cast<GLsizei>(descriptor.size.height), static_cast<GLsizei>(descriptor.size.depth), 0, format, type, data);
        }

        // Move on to the next mipmap and ensure the size is always at least 1x1x1
        data = static_cast<const core::uint8 *>(data) + numberOfBytesPerMipmap;
        descriptor.size.width = get_half_size(descriptor.size.width);
        descriptor.size.height = get_half_size(descriptor.size.height);
        descriptor.size.depth = get_half_size(descriptor.size.depth);
      }
    } else {
      // The user only provided us with the base texture, no mipmaps
      if (isArbDsa) {
        if (nullptr != data) {
          glTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height),
                              static_cast<GLsizei>(descriptor.size.depth), Mapping::get_opengl_format(descriptor.textureFormat),
                              Mapping::get_opengl_type(descriptor.textureFormat), data);
        }
      } else {
        glTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, 0, static_cast<GLint>(mOpenGLInternalFormat),
                            static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.height), static_cast<GLsizei>(descriptor.size.depth), 0,
                            Mapping::get_opengl_format(descriptor.textureFormat), Mapping::get_opengl_type(descriptor.textureFormat), data);
      }
    }
  }

  // Build mipmaps automatically on the GPU? (or GPU driver)
  if (descriptor.textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS) {
    if (isArbDsa) {
      glGenerateTextureMipmap(mOpenGLTexture);
      glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    } else {
      glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_3D);
      glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
  } else {
    if (isArbDsa) {
      glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
      glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  }

  if (isArbDsa) {
    glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Restore previous alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()){
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "3D texture", 13)	// 13 = "3D texture: " including terminating zero
    glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
  }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
