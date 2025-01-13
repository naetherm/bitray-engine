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
 * OpenGL mapping
 */
class Mapping final {


  //[-------------------------------------------------------]
  //[ Public static methods                                 ]
  //[-------------------------------------------------------]
public:
  //[-------------------------------------------------------]
  //[ rhi::FilterMode                                       ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::FilterMode" to OpenGL magnification filter mode
   * 
   * @param[in] context
   * RHI context to use
   * @param[in] filterMode
   * "rhi::FilterMode" to map
   * 
   * @return
   * OpenGL magnification filter mode
   */
  [[nodiscard]] static GLint
  get_opengl_mag_filter_mode([[maybe_unused]] const rhi::RHIContext &context, rhi::FilterMode filterMode) {
    switch (filterMode) {
      case rhi::FilterMode::MIN_MAG_MIP_POINT:
        return GL_NEAREST;

      case rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
        return GL_NEAREST;

      case rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
        return GL_LINEAR;

      case rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
        return GL_LINEAR;

      case rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
        return GL_NEAREST;

      case rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
        return GL_NEAREST;

      case rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
        return GL_LINEAR;

      case rhi::FilterMode::MIN_MAG_MIP_LINEAR:
        return GL_LINEAR;

      case rhi::FilterMode::ANISOTROPIC:
        return GL_LINEAR;  // There's no special setting in OpenGL

      case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
        return GL_NEAREST;

      case rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
        return GL_NEAREST;

      case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
        return GL_LINEAR;

      case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
        return GL_LINEAR;

      case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
        return GL_NEAREST;

      case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
        return GL_NEAREST;

      case rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
        return GL_LINEAR;

      case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
        return GL_LINEAR;

      case rhi::FilterMode::COMPARISON_ANISOTROPIC:
        return GL_LINEAR;  // There's no special setting in OpenGL

      case rhi::FilterMode::UNKNOWN:
        BE_ASSERT(false, "OpenGL filter mode must not be unknown")
        return GL_NEAREST;

      default:
        return GL_NEAREST;  // We should never be in here
    }
  }

  /**
   * @brief
   * "rhi::FilterMode" to OpenGL minification filter mode
   * 
   * @param[in] context
   * RHI context to use
   * @param[in] filterMode
   * "rhi::FilterMode" to map
   * @param[in] hasMipmaps
   * Are mipmaps available?
   * 
   * @return
   * OpenGL minification filter mode
   */
  [[nodiscard]] static GLint
  get_opengl_min_filter_mode([[maybe_unused]] const rhi::RHIContext &context, rhi::FilterMode filterMode,
                             bool hasMipmaps) {
    switch (filterMode) {
      case rhi::FilterMode::MIN_MAG_MIP_POINT:
        return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

      case rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
        return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

      case rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
        return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

      case rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
        return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

      case rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
        return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

      case rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
        return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

      case rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
        return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

      case rhi::FilterMode::MIN_MAG_MIP_LINEAR:
        return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

      case rhi::FilterMode::ANISOTROPIC:
        return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;  // There's no special setting in OpenGL

      case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
        return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

      case rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
        return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

      case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
        return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

      case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
        return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

      case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
        return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

      case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
        return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

      case rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
        return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

      case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
        return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

      case rhi::FilterMode::COMPARISON_ANISOTROPIC:
        return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;  // There's no special setting in OpenGL

      case rhi::FilterMode::UNKNOWN:
        BE_ASSERT(false, "OpenGL filter mode must not be unknown")
        return GL_NEAREST;

      default:
        return GL_NEAREST;  // We should never be in here
    }
  }

  /**
   * @brief
   * "rhi::FilterMode" to OpenGL compare mode
   * 
   * @param[in] context
   * RHI context to use
   * @param[in] filterMode
   * "rhi::FilterMode" to map
   * 
   * @return
   * OpenGL compare mode
   */
  [[nodiscard]] static GLint
  get_opengl_compare_mode([[maybe_unused]] const rhi::RHIContext &context, rhi::FilterMode filterMode) {
    switch (filterMode) {
      case rhi::FilterMode::MIN_MAG_MIP_POINT:
      case rhi::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      case rhi::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      case rhi::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      case rhi::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      case rhi::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      case rhi::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      case rhi::FilterMode::MIN_MAG_MIP_LINEAR:
      case rhi::FilterMode::ANISOTROPIC:
        return GL_NONE;

      case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      case rhi::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      case rhi::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      case rhi::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      case rhi::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      case rhi::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      case rhi::FilterMode::COMPARISON_ANISOTROPIC:
        return GL_COMPARE_REF_TO_TEXTURE;

      case rhi::FilterMode::UNKNOWN:
        BE_ASSERT(false, "OpenGL filter mode must not be unknown")
        return GL_NEAREST;

      default:
        return GL_NEAREST;  // We should never be in here
    }
  }

  //[-------------------------------------------------------]
  //[ rhi::TextureAddressMode                               ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::TextureAddressMode" to OpenGL texture address mode
   * 
   * @param[in] textureAddressMode
   * "rhi::TextureAddressMode" to map
   * 
   * @return
   * OpenGL texture address mode
   */
  [[nodiscard]] static GLint get_opengl_texture_address_mode(rhi::TextureAddressMode textureAddressMode) {
    static constexpr GLint MAPPING[] =
      {
        GL_REPEAT,      // rhi::TextureAddressMode::WRAP
        GL_MIRRORED_REPEAT,  // rhi::TextureAddressMode::MIRROR
        GL_CLAMP_TO_EDGE,  // rhi::TextureAddressMode::CLAMP
        GL_CLAMP_TO_BORDER,  // rhi::TextureAddressMode::BORDER
        GL_MIRRORED_REPEAT  // rhi::TextureAddressMode::MIRROR_ONCE	// TODO(naetherm) Maybe GL_ATI_texture_mirror_once ?
      };
    return MAPPING[static_cast<int>(textureAddressMode) -
                   1];  // Lookout! The "rhi::TextureAddressMode"-values start with 1, not 0
  }

  //[-------------------------------------------------------]
  //[ rhi::ComparisonFunc                                   ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::ComparisonFunc" to OpenGL comparison function
   * 
   * @param[in] comparisonFunc
   * "rhi::ComparisonFunc" to map
   * 
   * @return
   * OpenGL comparison function
   */
  [[nodiscard]] static GLenum get_opengl_comparison_func(rhi::ComparisonFunc comparisonFunc) {
    static constexpr GLenum MAPPING[] =
      {
        GL_NEVER,    // rhi::ComparisonFunc::NEVER
        GL_LESS,    // rhi::ComparisonFunc::LESS
        GL_EQUAL,    // rhi::ComparisonFunc::EQUAL
        GL_LEQUAL,    // rhi::ComparisonFunc::LESS_EQUAL
        GL_GREATER,    // rhi::ComparisonFunc::GREATER
        GL_NOTEQUAL,  // rhi::ComparisonFunc::NOT_EQUAL
        GL_GEQUAL,    // rhi::ComparisonFunc::GREATER_EQUAL
        GL_ALWAYS    // rhi::ComparisonFunc::ALWAYS
      };
    return MAPPING[static_cast<int>(comparisonFunc) -
                   1];  // Lookout! The "rhi::ComparisonFunc"-values start with 1, not 0
  }

  //[-------------------------------------------------------]
  //[ rhi::VertexAttributeFormat                            ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::VertexAttributeFormat" to OpenGL size (number of elements)
   * 
   * @param[in] vertexAttributeFormat
   * "rhi::VertexAttributeFormat" to map
   * 
   * @return
   * OpenGL size
   */
  [[nodiscard]] static GLint get_opengl_size(rhi::VertexAttributeFormat vertexAttributeFormat) {
    static constexpr GLint MAPPING[] =
      {
        1,  // rhi::VertexAttributeFormat::FLOAT_1
        2,  // rhi::VertexAttributeFormat::FLOAT_2
        3,  // rhi::VertexAttributeFormat::FLOAT_3
        4,  // rhi::VertexAttributeFormat::FLOAT_4
        4,  // rhi::VertexAttributeFormat::R8G8B8A8_UNORM
        4,  // rhi::VertexAttributeFormat::R8G8B8A8_UINT
        2,  // rhi::VertexAttributeFormat::SHORT_2
        4,  // rhi::VertexAttributeFormat::SHORT_4
        1  // rhi::VertexAttributeFormat::UINT_1
      };
    return MAPPING[static_cast<int>(vertexAttributeFormat)];
  }

  /**
   * @brief
   * "rhi::VertexAttributeFormat" to OpenGL type
   * 
   * @param[in] vertexAttributeFormat
   * "rhi::VertexAttributeFormat" to map
   * 
   * @return
   * OpenGL type
   */
  [[nodiscard]] static GLenum get_opengl_type(rhi::VertexAttributeFormat vertexAttributeFormat) {
    static constexpr GLenum MAPPING[] =
      {
        GL_FLOAT,      // rhi::VertexAttributeFormat::FLOAT_1
        GL_FLOAT,      // rhi::VertexAttributeFormat::FLOAT_2
        GL_FLOAT,      // rhi::VertexAttributeFormat::FLOAT_3
        GL_FLOAT,      // rhi::VertexAttributeFormat::FLOAT_4
        GL_UNSIGNED_BYTE,  // rhi::VertexAttributeFormat::R8G8B8A8_UNORM
        GL_UNSIGNED_BYTE,  // rhi::VertexAttributeFormat::R8G8B8A8_UINT
        GL_SHORT,      // rhi::VertexAttributeFormat::SHORT_2
        GL_SHORT,      // rhi::VertexAttributeFormat::SHORT_4
        GL_UNSIGNED_INT    // rhi::VertexAttributeFormat::UINT_1
      };
    return MAPPING[static_cast<int>(vertexAttributeFormat)];
  }

  /**
   * @brief
   * Return whether or not "rhi::VertexAttributeFormat" is a normalized format
   * 
   * @param[in] vertexAttributeFormat
   * "rhi::VertexAttributeFormat" to check
   * 
   * @return
   * "GL_TRUE" if the format is normalized, else "GL_FALSE"
   */
  [[nodiscard]] static GLboolean
  is_opengl_vertex_attribute_format_normalized(rhi::VertexAttributeFormat vertexAttributeFormat) {
    static constexpr GLboolean MAPPING[] =
      {
        GL_FALSE,  // rhi::VertexAttributeFormat::FLOAT_1
        GL_FALSE,  // rhi::VertexAttributeFormat::FLOAT_2
        GL_FALSE,  // rhi::VertexAttributeFormat::FLOAT_3
        GL_FALSE,  // rhi::VertexAttributeFormat::FLOAT_4
        GL_TRUE,  // rhi::VertexAttributeFormat::R8G8B8A8_UNORM
        GL_FALSE,  // rhi::VertexAttributeFormat::R8G8B8A8_UINT
        GL_FALSE,  // rhi::VertexAttributeFormat::SHORT_2
        GL_FALSE,  // rhi::VertexAttributeFormat::SHORT_4
        GL_FALSE  // rhi::VertexAttributeFormat::UINT_1
      };
    return MAPPING[static_cast<int>(vertexAttributeFormat)];
  }

  /**
   * @brief
   * Return whether or not "rhi::VertexAttributeFormat" is an integer format
   * 
   * @param[in] vertexAttributeFormat
   * "rhi::VertexAttributeFormat" to check
   * 
   * @return
   * "GL_TRUE" if the format is integer, else "GL_FALSE"
   */
  [[nodiscard]] static GLboolean
  is_opengl_vertex_attribute_format_integer(rhi::VertexAttributeFormat vertexAttributeFormat) {
    static constexpr GLboolean MAPPING[] =
      {
        GL_FALSE,  // rhi::VertexAttributeFormat::FLOAT_1
        GL_FALSE,  // rhi::VertexAttributeFormat::FLOAT_2
        GL_FALSE,  // rhi::VertexAttributeFormat::FLOAT_3
        GL_FALSE,  // rhi::VertexAttributeFormat::FLOAT_4
        GL_FALSE,  // rhi::VertexAttributeFormat::R8G8B8A8_UNORM
        GL_TRUE,  // rhi::VertexAttributeFormat::R8G8B8A8_UINT
        GL_TRUE,  // rhi::VertexAttributeFormat::SHORT_2
        GL_TRUE,  // rhi::VertexAttributeFormat::SHORT_4
        GL_TRUE    // rhi::VertexAttributeFormat::UINT_1
      };
    return MAPPING[static_cast<int>(vertexAttributeFormat)];
  }

  //[-------------------------------------------------------]
  //[ rhi::IndexBufferFormat                                ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::IndexBufferFormat" to OpenGL type
   * 
   * @param[in] indexBufferFormat
   * "rhi::IndexBufferFormat" to map
   * 
   * @return
   * OpenGL type
   */
  [[nodiscard]] static GLenum get_opengl_type(rhi::IndexBufferFormat::Enum indexBufferFormat) {
    static constexpr GLenum MAPPING[] =
      {
        GL_UNSIGNED_BYTE,  // rhi::IndexBufferFormat::UNSIGNED_CHAR  - One byte per element, core::uint8 (may not be supported by each API)
        GL_UNSIGNED_SHORT,  // rhi::IndexBufferFormat::UNSIGNED_SHORT - Two bytes per element, core::uint16
        GL_UNSIGNED_INT    // rhi::IndexBufferFormat::UNSIGNED_INT   - Four bytes per element, core::uint32 (may not be supported by each API)
      };
    return MAPPING[indexBufferFormat];
  }

  //[-------------------------------------------------------]
  //[ rhi::TextureFormat                                    ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::TextureFormat" to OpenGL internal format
   * 
   * @param[in] textureFormat
   * "rhi::TextureFormat" to map
   * 
   * @return
   * OpenGL internal format
   */
  [[nodiscard]] static GLuint get_opengl_internal_format(rhi::TextureFormat::Enum textureFormat) {
    static constexpr GLuint MAPPING[] =
      {
        GL_R8,                    // rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
        GL_RGB8,                  // rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
        GL_RGBA8,                  // rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        GL_SRGB8_ALPHA8,              // rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_RGBA8,                  // rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        GL_R11F_G11F_B10F_EXT,            // rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - "GL_EXT_packed_float" OpenGL extension
        GL_RGBA16F_ARB,                // rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
        GL_RGBA32F_ARB,                // rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha),
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,      // rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
        GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT,    // rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,      // rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
        GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT,    // rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,      // rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
        GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT,    // rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_COMPRESSED_LUMINANCE_LATC1_EXT,      // rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
        GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,  // rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
        0,                      // rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in OpenGL
        GL_R16,                    // rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
        GL_R32UI,                  // rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
        GL_R32F,                  // rhi::TextureFormat::R32_FLOAT     - 32-bit float format
        GL_DEPTH_COMPONENT32F,            // rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
        GL_RG16_SNORM,                // rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
        GL_RG16F,                  // rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
        0                      // rhi::TextureFormat::UNKNOWN       - Unknown
      };
    return MAPPING[textureFormat];
  }

  /**
   * @brief
   * "rhi::TextureFormat" to OpenGL format
   * 
   * @param[in] textureFormat
   * "rhi::TextureFormat" to map
   * 
   * @return
   * OpenGL format
   */
  [[nodiscard]] static GLuint get_opengl_format(rhi::TextureFormat::Enum textureFormat) {
    static constexpr GLuint MAPPING[] =
      {
        GL_RED,                    // rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
        GL_RGB,                    // rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
        GL_RGBA,                  // rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        GL_RGBA,                  // rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_BGRA,                  // rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        GL_RGB,                    // rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - "GL_EXT_packed_float" OpenGL extension
        GL_RGBA,                  // rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
        GL_RGBA,                  // rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,      // rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
        GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT,    // rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,      // rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
        GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT,    // rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,      // rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
        GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT,    // rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_COMPRESSED_LUMINANCE_LATC1_EXT,      // rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
        GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,  // rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
        0,                      // rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in OpenGL
        GL_RED,                    // rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
        GL_RED_INTEGER,                // rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
        GL_RED,                    // rhi::TextureFormat::R32_FLOAT     - 32-bit float format
        GL_DEPTH_COMPONENT,              // rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
        GL_RG,                    // rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
        GL_RG,                    // rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
        0                      // rhi::TextureFormat::UNKNOWN       - Unknown
      };
    return MAPPING[textureFormat];
  }

  /**
   * @brief
   * "rhi::TextureFormat" to OpenGL type
   * 
   * @param[in] textureFormat
   * "rhi::TextureFormat" to map
   * 
   * @return
   * OpenGL type
   */
  [[nodiscard]] static GLenum get_opengl_type(rhi::TextureFormat::Enum textureFormat) {
    static constexpr GLenum MAPPING[] =
      {
        GL_UNSIGNED_BYTE,            // rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
        GL_UNSIGNED_BYTE,            // rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
        GL_UNSIGNED_BYTE,            // rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        GL_UNSIGNED_BYTE,            // rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        GL_UNSIGNED_BYTE,            // rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        GL_UNSIGNED_INT_10F_11F_11F_REV_EXT,  // rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - "GL_EXT_packed_float" OpenGL extension
        GL_HALF_FLOAT_ARB,            // rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
        GL_FLOAT,                // rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
        0,                    // rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
        0,                    // rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        0,                    // rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
        0,                    // rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        0,                    // rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
        0,                    // rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        0,                    // rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
        0,                    // rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
        0,                    // rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in OpenGL
        GL_UNSIGNED_SHORT,            // rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
        GL_UNSIGNED_INT,            // rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
        GL_FLOAT,                // rhi::TextureFormat::R32_FLOAT     - 32-bit float format
        GL_FLOAT,                // rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
        GL_BYTE,                // rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
        GL_FLOAT,                // rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
        0                    // rhi::TextureFormat::UNKNOWN       - Unknown
      };
    return MAPPING[textureFormat];
  }

  //[-------------------------------------------------------]
  //[ rhi::PrimitiveTopology                                ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::PrimitiveTopology" to OpenGL type
   * 
   * @param[in] primitiveTopology
   * "rhi::PrimitiveTopology" to map
   * 
   * @return
   * OpenGL type
   */
  [[nodiscard]] static GLenum get_opengl_type(rhi::PrimitiveTopology primitiveTopology) {
    static constexpr GLenum MAPPING[] =
      {
        GL_POINTS,            // rhi::PrimitiveTopology::POINT_LIST
        GL_LINES,            // rhi::PrimitiveTopology::LINE_LIST
        GL_LINE_STRIP,          // rhi::PrimitiveTopology::LINE_STRIP
        GL_TRIANGLES,          // rhi::PrimitiveTopology::TRIANGLE_LIST
        GL_TRIANGLE_STRIP,        // rhi::PrimitiveTopology::TRIANGLE_STRIP
        0,                // Unused value 6
        0,                // Unused value 7
        0,                // Unused value 8
        0,                // Unused value 9
        GL_LINES_ADJACENCY_EXT,      // rhi::PrimitiveTopology::LINE_LIST_ADJ - "GL_EXT_geometry_shader"-extension
        GL_LINE_STRIP_ADJACENCY_EXT,  // rhi::PrimitiveTopology::LINE_STRIP_ADJ - "GL_EXT_geometry_shader"-extension
        GL_TRIANGLES_ADJACENCY_EXT,    // rhi::PrimitiveTopology::TRIANGLE_LIST_ADJ - "GL_EXT_geometry_shader"-extension
        GL_TRIANGLE_STRIP_ADJACENCY_EXT  // rhi::PrimitiveTopology::TRIANGLE_STRIP_ADJ - "GL_EXT_geometry_shader"-extension
      };
    return MAPPING[static_cast<int>(primitiveTopology) -
                   1];  // Lookout! The "rhi::PrimitiveTopology"-values start with 1, not 0
  }

  //[-------------------------------------------------------]
  //[ rhi::MapType                                          ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::MapType" to OpenGL type
   * 
   * @param[in] mapType
   * "rhi::MapType" to map
   * 
   * @return
   * OpenGL type
   */
  [[nodiscard]] static GLenum get_opengl_map_type(rhi::MapType mapType) {
    static constexpr GLenum MAPPING[] =
      {
        GL_READ_ONLY,  // rhi::MapType::READ
        GL_WRITE_ONLY,  // rhi::MapType::WRITE
        GL_READ_WRITE,  // rhi::MapType::READ_WRITE
        GL_WRITE_ONLY,  // rhi::MapType::WRITE_DISCARD
        GL_WRITE_ONLY  // rhi::MapType::WRITE_NO_OVERWRITE
      };
    return MAPPING[static_cast<int>(mapType) - 1];  // Lookout! The "rhi::MapType"-values start with 1, not 0
  }

  //[-------------------------------------------------------]
  //[ rhi::Blend                                            ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * "rhi::Blend" to OpenGL type
   * 
   * @param[in] blend
   * "rhi::Blend" to map
   * 
   * @return
   * OpenGL type
   */
  [[nodiscard]] static GLenum get_opengl_blend_type(rhi::Blend blend) {
    if (blend <= rhi::Blend::SRC_ALPHA_SAT) {
      static constexpr GLenum MAPPING[] =
        {
          GL_ZERO,        // rhi::Blend::ZERO
          GL_ONE,          // rhi::Blend::ONE
          GL_SRC_COLOR,      // rhi::Blend::SRC_COLOR
          GL_ONE_MINUS_SRC_COLOR,  // rhi::Blend::INV_SRC_COLOR
          GL_SRC_ALPHA,      // rhi::Blend::SRC_ALPHA
          GL_ONE_MINUS_SRC_ALPHA,  // rhi::Blend::INV_SRC_ALPHA
          GL_DST_ALPHA,      // rhi::Blend::DEST_ALPHA
          GL_ONE_MINUS_DST_ALPHA,  // rhi::Blend::INV_DEST_ALPHA
          GL_DST_COLOR,      // rhi::Blend::DEST_COLOR
          GL_ONE_MINUS_DST_COLOR,  // rhi::Blend::INV_DEST_COLOR
          GL_SRC_ALPHA_SATURATE  // rhi::Blend::SRC_ALPHA_SAT
        };
      return MAPPING[static_cast<int>(blend) - static_cast<int>(rhi::Blend::ZERO)];
    } else {
      static constexpr GLenum MAPPING[] =
        {
          GL_SRC_COLOR,        // rhi::Blend::BLEND_FACTOR		TODO(naetherm) Mapping "rhi::Blend::BLEND_FACTOR" to OpenGL possible?
          GL_ONE_MINUS_SRC_COLOR,    // rhi::Blend::INV_BLEND_FACTOR	TODO(naetherm) Mapping "rhi::Blend::INV_BLEND_FACTOR" to OpenGL possible?
          GL_SRC1_COLOR,        // rhi::Blend::SRC_1_COLOR
          GL_ONE_MINUS_SRC1_COLOR,  // rhi::Blend::INV_SRC_1_COLOR
          GL_SRC1_ALPHA,        // rhi::Blend::SRC_1_ALPHA
          GL_ONE_MINUS_SRC1_ALPHA,  // rhi::Blend::INV_SRC_1_ALPHA
        };
      return MAPPING[static_cast<int>(blend) - static_cast<int>(rhi::Blend::BLEND_FACTOR)];
    }
  }


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
