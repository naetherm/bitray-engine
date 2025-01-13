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
#include "rhi_opengl/texture/rhitexture_1d_array_dsa.h"
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
RHITexture1DArrayDsa::RHITexture1DArrayDsa(RHIDevice &openGLRhi, rhi::TextureDescriptor descriptor, const void *data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHITexture1DArray(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently set alignment
  GLint openGLAlignmentBackup = 0;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

  // Set correct unpack alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT,
                (rhi::TextureFormat::getNumberOfBytesPerElement(descriptor.textureFormat) & 3) ? 1 : 4);

  // Calculate the number of mipmaps
  const bool dataContainsMipmaps = (descriptor.textureFlags & rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
  const bool generateMipmaps = (!dataContainsMipmaps &&
                                (descriptor.textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS));
  const core::uint32 numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? get_number_of_mipmaps(
    descriptor.size.width) : 1;

  // Create the OpenGL texture instance
#ifdef _WIN32
  // TODO(naetherm) It appears that DSA "glGenerateTextureMipmap()" is not working (one notices the noise) or we're using it wrong, tested with
  //			- "InstancedCubes"-example -> "CubeRendereDrawInstanced"
  //		    - AMD 290X Radeon software version 17.7.2 as well as with GeForce 980m 384.94
  //		    - Windows 10 x64
  const bool isArbDsa = (openGLRhi.get_extensions().isGL_ARB_direct_state_access() && (textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS) == 0);
#else
  const bool isArbDsa = openGLRhi.get_extensions().isGL_ARB_direct_state_access();
#endif
  if (isArbDsa) {
    glCreateTextures(GL_TEXTURE_1D_ARRAY_EXT, 1, &mOpenGLTexture);
    glTextureStorage2D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat,
                       static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.depth));
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
        const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(
          rhi::TextureFormat::getNumberOfBytesPerSlice(descriptor.textureFormat, descriptor.size.width, 1) *
          descriptor.size.depth);
        if (isArbDsa) {
          // We know that "data" must be valid when we're in here due to the "rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
          glCompressedTextureSubImage2D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0,
                                        static_cast<GLsizei>(descriptor.size.width),
                                        static_cast<GLsizei>(descriptor.size.depth), format, numberOfBytesPerSlice,
                                        data);
        } else {
          glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, static_cast<GLint>(mipmap), format,
                                        static_cast<GLsizei>(descriptor.size.width),
                                        static_cast<GLsizei>(descriptor.size.depth), 0, numberOfBytesPerSlice, data);
        }

        // Move on to the next mipmap and ensure the size is always at least 1x1
        data = static_cast<const core::uint8 *>(data) + numberOfBytesPerSlice;
        descriptor.size.width = get_half_size(descriptor.size.width);
      }
    } else {
      // The user only provided us with the base texture, no mipmaps
      if (isArbDsa) {
        if (nullptr != data) {
          glCompressedTextureSubImage2D(mOpenGLTexture, 0, 0, 0, static_cast<GLsizei>(descriptor.size.width),
                                        static_cast<GLsizei>(descriptor.size.depth),
                                        Mapping::get_opengl_format(descriptor.textureFormat), static_cast<GLsizei>(
                                          rhi::TextureFormat::getNumberOfBytesPerSlice(descriptor.textureFormat,
                                                                                         descriptor.size.width, 1) *
                                          descriptor.size.depth), data);
        }
      } else {
        glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, 0, mOpenGLInternalFormat,
                                      static_cast<GLsizei>(descriptor.size.width),
                                      static_cast<GLsizei>(descriptor.size.depth), 0, static_cast<GLsizei>(
                                        rhi::TextureFormat::getNumberOfBytesPerSlice(descriptor.textureFormat,
                                                                                       descriptor.size.width, 1) *
                                        descriptor.size.depth), data);
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
        const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(
          rhi::TextureFormat::getNumberOfBytesPerSlice(descriptor.textureFormat, descriptor.size.width, 1) *
          descriptor.size.depth);
        if (isArbDsa) {
          // We know that "data" must be valid when we're in here due to the "rhi::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
          glTextureSubImage2D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0,
                              static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.depth),
                              format, type, data);
        } else {
          glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, static_cast<GLint>(mipmap),
                              static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(descriptor.size.width),
                              static_cast<GLsizei>(descriptor.size.depth), 0, format, type, data);
        }

        // Move on to the next mipmap and ensure the size is always at least 1x1
        data = static_cast<const core::uint8 *>(data) + numberOfBytesPerSlice;
        descriptor.size.width = get_half_size(descriptor.size.width);
      }
    } else {
      // The user only provided us with the base texture, no mipmaps
      if (isArbDsa) {
        if (nullptr != data) {
          glTextureSubImage2D(mOpenGLTexture, 0, 0, 0, static_cast<GLsizei>(descriptor.size.width),
                              static_cast<GLsizei>(descriptor.size.depth),
                              Mapping::get_opengl_format(descriptor.textureFormat),
                              Mapping::get_opengl_type(descriptor.textureFormat), data);
        }
      } else {
        glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, 0, static_cast<GLint>(mOpenGLInternalFormat),
                            static_cast<GLsizei>(descriptor.size.width), static_cast<GLsizei>(descriptor.size.depth), 0,
                            Mapping::get_opengl_format(descriptor.textureFormat),
                            Mapping::get_opengl_type(descriptor.textureFormat), data);
      }
    }
  }

  // Build mipmaps automatically on the GPU? (or GPU driver)
  if (descriptor.textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS) {
    if (isArbDsa) {
      glGenerateTextureMipmap(mOpenGLTexture);
      glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    } else {
      glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT);
      glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
  } else {
    if (isArbDsa) {
      glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
      glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  }

  if (isArbDsa) {
    glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Restore previous alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture array", 19)  // 19 = "1D texture array: " including terminating zero
    glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
  }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
