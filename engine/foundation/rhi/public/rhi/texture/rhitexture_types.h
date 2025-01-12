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
#include "rhi/rhi.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Texture format
 */
struct TextureFormat final {
  enum Enum : core::uint8 {
    /// 8-bit pixel format, all bits red
    R8 = 0,
    /// 24-bit pixel format, 8 bits for red, green and blue
    R8G8B8 = 1,
    /// 32-bit pixel format, 8 bits for red, green, blue and alpha
    R8G8B8A8 = 2,
    /// 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
    R8G8B8A8_SRGB = 3,
    /// 32-bit pixel format, 8 bits for red, green, blue and alpha
    B8G8R8A8 = 4,
    /// 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent, see e.g. https://www.khronos.org/opengl/wiki/Small_Float_Formats#R11F_G11F_B10F -> "Small Float Formats" -> "Numeric limits and precision" and "Small float formats � R11G11B10F precision" by Bart Wronski ( https://bartwronski.com/2017/04/02/small-float-formats-r11g11b10f-precision/ )
    R11G11B10F = 5,
    /// 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
    R16G16B16A16F = 6,
    /// 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
    R32G32B32A32F = 7,
    /// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
    BC1 = 8,
    /// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
    BC1_SRGB = 9,
    /// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
    BC2 = 10,
    /// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
    BC2_SRGB = 11,
    /// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
    BC3 = 12,
    /// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
    BC3_SRGB = 13,
    /// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
    BC4 = 14,
    /// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
    BC5 = 15,
    /// 3 component texture compression meant for mobile devices
    ETC1 = 16,
    /// 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
    R16_UNORM = 17,
    /// 32-bit unsigned integer format
    R32_UINT = 18,
    /// 32-bit float format
    R32_FLOAT = 19,
    /// 32-bit float depth format
    D32_FLOAT = 20,
    /// A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
    R16G16_SNORM = 21,
    /// A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
    R16G16_FLOAT = 22,
    /// Unknown
    UNKNOWN = 23,
    /// Number of texture formats
    NUMBER_OF_FORMATS = 24
  };

  /**
  *  @brief
  *    Return whether or not the given "rhi::TextureFormat" is a compressed format
  *
  *  @param[in] textureFormat
  *    "rhi::TextureFormat" to check
  *
  *  @return
  *    "true" if the given "rhi::TextureFormat" is a compressed format, else "false"
  */
  [[nodiscard]] static inline bool isCompressed(Enum textureFormat) {
    static constexpr bool MAPPING[] =
      {
        false,  // rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
        false,  // rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
        false,  // rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        false,  // rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        false,  // rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        false,  // rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
        false,  // rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
        false,  // rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
        true,  // rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
        true,  // rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        true,  // rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
        true,  // rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        true,  // rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
        true,  // rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        true,  // rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
        true,  // rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
        true,  // rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices
        false,  // rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
        false,  // rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
        false,  // rhi::TextureFormat::R32_FLOAT     - 32-bit float format
        false,  // rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
        false,  // rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
        false,  // rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
        false  // rhi::TextureFormat::UNKNOWN       - Unknown
      };
    return MAPPING[textureFormat];
  }

  /**
  *  @brief
  *    Return whether or not the given "rhi::TextureFormat" is a depth format
  *
  *  @param[in] textureFormat
  *    "rhi::TextureFormat" to check
  *
  *  @return
  *    "true" if the given "rhi::TextureFormat" is a depth format, else "false"
  */
  [[nodiscard]] static inline bool isDepth(Enum textureFormat) {
    static constexpr bool MAPPING[] =
      {
        false,  // rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
        false,  // rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
        false,  // rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        false,  // rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        false,  // rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        false,  // rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
        false,  // rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
        false,  // rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
        false,  // rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
        false,  // rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        false,  // rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
        false,  // rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        false,  // rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
        false,  // rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        false,  // rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
        false,  // rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
        false,  // rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices
        false,  // rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
        false,  // rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
        false,  // rhi::TextureFormat::R32_FLOAT     - 32-bit float format
        true,  // rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
        false,  // rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
        false,  // rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
        false  // rhi::TextureFormat::UNKNOWN       - Unknown
      };
    return MAPPING[textureFormat];
  }

  /**
  *  @brief
  *    "rhi::TextureFormat" to number of bytes per element
  *
  *  @param[in] textureFormat
  *    "rhi::TextureFormat" to map
  *
  *  @return
  *    Number of bytes per element
  */
  [[nodiscard]] static inline core::uint32 getNumberOfBytesPerElement(Enum textureFormat) {
    static constexpr core::uint32 MAPPING[] =
      {
        sizeof(core::uint8),    // rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
        sizeof(core::uint8) *
        3,  // rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
        sizeof(core::uint8) *
        4,  // rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        sizeof(core::uint8) *
        4,  // rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        sizeof(core::uint8) *
        4,  // rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
        sizeof(float),      // rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
        sizeof(float) *
        2,    // rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
        sizeof(float) *
        4,    // rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
        sizeof(core::uint8) *
        3,  // rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
        sizeof(core::uint8) *
        3,  // rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        sizeof(core::uint8) *
        4,  // rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
        sizeof(core::uint8) *
        4,  // rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        sizeof(core::uint8) *
        4,  // rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
        sizeof(core::uint8) *
        4,  // rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
        sizeof(core::uint8) *
        1,  // rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
        sizeof(core::uint8) *
        2,  // rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
        sizeof(core::uint8) *
        3,  // rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - when being uncompressed
        sizeof(core::uint16),    // rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
        sizeof(core::uint32),    // rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
        sizeof(float),      // rhi::TextureFormat::R32_FLOAT     - 32-bit float format
        sizeof(float),      // rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
        sizeof(core::uint32),    // rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
        sizeof(float),      // rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
        0            // rhi::TextureFormat::UNKNOWN       - Unknown
      };
    return MAPPING[textureFormat];
  }

  /**
  *  @brief
  *    "rhi::TextureFormat" to number of bytes per row
  *
  *  @param[in] textureFormat
  *    "rhi::TextureFormat" to map
  *  @param[in] width
  *    Row width
  *
  *  @return
  *    Number of bytes per row
  */
  [[nodiscard]] static inline core::uint32 getNumberOfBytesPerRow(Enum textureFormat, core::uint32 width) {
    switch (textureFormat) {
      // 8-bit pixel format, all bits red
      case R8:
        return width;

        // 24-bit pixel format, 8 bits for red, green and blue
      case R8G8B8:
        return 3 * width;

        // 32-bit pixel format, 8 bits for red, green, blue and alpha
      case R8G8B8A8:
      case R8G8B8A8_SRGB:
      case B8G8R8A8:
        return 4 * width;

        // 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
      case R11G11B10F:
        return 4 * width;

        // 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
      case R16G16B16A16F:
        return 8 * width;

        // 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
      case R32G32B32A32F:
        return 16 * width;

        // DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
      case BC1:
      case BC1_SRGB:
        return ((width + 3) >> 2) * 8;

        // DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
      case BC2:
      case BC2_SRGB:
        return ((width + 3) >> 2) * 16;

        // DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
      case BC3:
      case BC3_SRGB:
        return ((width + 3) >> 2) * 16;

        // 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
      case BC4:
        return ((width + 3) >> 2) * 8;

        // 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
      case BC5:
        return ((width + 3) >> 2) * 16;

        // 3 component texture compression meant for mobile devices
      case ETC1:
        return (width >> 1);

        // 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
      case R16_UNORM:
        return sizeof(core::uint16) * width;

        // 32-bit unsigned integer format
      case R32_UINT:
        return sizeof(core::uint32) * width;

        // 32-bit float red/depth format
      case R32_FLOAT:
      case D32_FLOAT:
        return sizeof(float) * width;

        // A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
      case R16G16_SNORM:
        return sizeof(core::uint32) * width;

        // A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
      case R16G16_FLOAT:
        return sizeof(float) * width;

        // Unknown
      case UNKNOWN:
      case NUMBER_OF_FORMATS:
        return 0;

      default:
        return 0;
    }
  }

  /**
  *  @brief
  *    "rhi::TextureFormat" to number of bytes per slice
  *
  *  @param[in] textureFormat
  *    "rhi::TextureFormat" to map
  *  @param[in] width
  *    Slice width
  *  @param[in] height
  *    Slice height
  *
  *  @return
  *    Number of bytes per slice
  */
  [[nodiscard]] static inline core::uint32
  getNumberOfBytesPerSlice(Enum textureFormat, core::uint32 width, core::uint32 height) {
    switch (textureFormat) {
      // 8-bit pixel format, all bits red
      case R8:
        return width * height;

        // 24-bit pixel format, 8 bits for red, green and blue
      case R8G8B8:
        return 3 * width * height;

        // 32-bit pixel format, 8 bits for red, green, blue and alpha
      case R8G8B8A8:
      case R8G8B8A8_SRGB:
      case B8G8R8A8:
        return 4 * width * height;

        // 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
      case R11G11B10F:
        return 4 * width * height;

        // 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
      case R16G16B16A16F:
        return 8 * width * height;

        // 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
      case R32G32B32A32F:
        return 16 * width * height;

        // DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
      case BC1:
      case BC1_SRGB:
        return ((width + 3) >> 2) * ((height + 3) >> 2) * 8;

        // DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
      case BC2:
      case BC2_SRGB:
        return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

        // DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
      case BC3:
      case BC3_SRGB:
        return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

        // 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
      case BC4:
        return ((width + 3) >> 2) * ((height + 3) >> 2) * 8;

        // 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
      case BC5:
        return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

        // 3 component texture compression meant for mobile devices
      case ETC1: {
        const core::uint32 numberOfBytesPerSlice = (width * height) >> 1;
        return (numberOfBytesPerSlice > 8) ? numberOfBytesPerSlice : 8;
      }

        // 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
      case R16_UNORM:
        return sizeof(core::uint16) * width * height;

        // 32-bit unsigned integer format
      case R32_UINT:
        return sizeof(core::uint32) * width * height;

        // 32-bit float depth format
      case R32_FLOAT:
      case D32_FLOAT:
        return sizeof(float) * width * height;

        // A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
      case R16G16_SNORM:
        return sizeof(core::uint32) * width * height;

        // A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
      case R16G16_FLOAT:
        return sizeof(float) * width * height;

        // Unknown
      case UNKNOWN:
      case NUMBER_OF_FORMATS:
        return 0;

      default:
        return 0;
    }
  }

};

/**
 * @brief
 * Texture flags
 */
struct TextureFlag final {
  enum Enum : core::uint32 {
    /// This texture can be used for unordered access which is needed for compute shader read/write textures (when using Direct3D 11 a unordered access view (UAV) will be generated)
    UNORDERED_ACCESS = 1 << 0,
    /// This texture can be used as shader resource (when using Direct3D 11 a shader resource view (SRV) will be generated)
    SHADER_RESOURCE = 1 << 1,
    /// This texture can be used as framebuffer object (FBO) attachment render target
    RENDER_TARGET = 1 << 2,
    /// The user provided data containing mipmaps from 0-n down to 1x1 linearly in memory
    DATA_CONTAINS_MIPMAPS = 1 << 3,
    /// Allocate mipmap chain, automatically generate mipmaps if static data has been provided (avoid this if you can, will be ignored in case the "DATA_CONTAINS_MIPMAPS"-flag is set), for depth textures the mipmaps can only be allocated but not automatically be generated
    GENERATE_MIPMAPS = 1 << 4
  };
};

/**
 * @brief
 * Texture usage indication
 *
 * @note
 * - Only relevant for Direct3D, OpenGL has no texture usage indication
 * - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/ff476259%28v=vs.85%29.aspx are used in here
 * - These constants directly map to Direct3D 10 & 11 constants, do not change them
 */
enum class TextureUsage {
  /// A resource that requires read and write access by the GPU. This is likely to be the most common usage choice.
  DEFAULT = 0,
  /// A resource that can only be read by the GPU. It cannot be written by the GPU, and cannot be accessed at all by the CPU. This type of resource must be initialized when it is created, since it cannot be changed after creation.
  IMMUTABLE = 1,
  /// A resource that is accessible by both the GPU (read only) and the CPU (write only). A dynamic resource is a good choice for a resource that will be updated by the CPU at least once per frame. To update a dynamic resource, use a map method.
  DYNAMIC = 2,
  /// A resource that supports data transfer (copy) from the GPU to the CPU.
  STAGING = 3
};

/**
 * @brief
 * Optimized clear value
 *
 * @see
 * - "ID3D12Device::CreateCommittedResource method" documentation at https://msdn.microsoft.com/de-de/library/windows/desktop/dn899178%28v=vs.85%29.aspx
 */
struct OptimizedTextureClearValue final {
  union {
    float color[4];
    struct {
      float depth;
      core::uint8 stencil;
    } DepthStencil;
  };
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
