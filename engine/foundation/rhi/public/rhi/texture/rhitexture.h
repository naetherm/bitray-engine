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
#include "rhi/rhiresource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract texture interface
 */
class RHITexture : public RHIResource {

  // Public static methods
public:
  /**
   * @brief
   * Calculate the number of mipmaps for a 1D texture
   *
   * @param[in] width
   * Texture width
   *
   * @return
   * Number of mipmaps
   */
  [[nodiscard]] static inline core::uint32 get_number_of_mipmaps(core::uint32 width) {
    // If "std::log2()" wouldn't be supported, we could write the following: "return static_cast<core::uint32>(1 + std::floor(std::log(width) / 0.69314718055994529));"
    // -> log2(x) = log(x) / log(2)
    // -> log(2) = 0.69314718055994529
    return static_cast<core::uint32>(1 + std::floor(std::log2(width)));
  }

  /**
   * @brief
   * Calculate the number of mipmaps for a 2D texture
   *
   * @param[in] width
   * Texture width
   * @param[in] height
   * Texture height
   *
   * @return
   * Number of mipmaps
   */
  [[nodiscard]] static inline core::uint32 get_number_of_mipmaps(core::uint32 width, core::uint32 height) {
    return get_number_of_mipmaps((width > height) ? width : height);
  }

  /**
   * @brief
   * Calculate the number of mipmaps for a 3D texture
   *
   * @param[in] width
   * Texture width
   * @param[in] height
   * Texture height
   * @param[in] depth
   * Texture depth
   *
   * @return
   * Number of mipmaps
   */
  [[nodiscard]] static inline core::uint32 get_number_of_mipmaps(core::uint32 width, core::uint32 height, core::uint32 depth) {
    return get_number_of_mipmaps(width, (height > depth) ? height : depth);
  }

  /**
   * @brief
   * Calculate the half size
   *
   * @param[in] size
   * Size
   *
   * @return
   * Half size, 1 as minimum
   */
  [[nodiscard]] static inline core::uint32 get_half_size(core::uint32 size) {
    size = (size >> 1);  // /= 2
    return (0u == size) ? 1u : size;
  }

  /**
   * @brief
   * Calculate the mipmap size at the given mipmap index
   *
   * @param[in] mipmapIndex
   * Mipmap index
   * @param[in,out] width
   * Texture width
   */
  static inline void get_mipmap_size(core::uint32 mipmapIndex, core::uint32 &width) {
    if (0u != mipmapIndex) {
      width = static_cast<core::uint32>(static_cast<float>(width) / std::exp2f(static_cast<float>(mipmapIndex)));
      if (0u == width) {
        width = 1u;
      }
    }
  }

  /**
   * @brief
   * Calculate the mipmap size at the given mipmap index
   *
   * @param[in] mipmapIndex
   * Mipmap index
   * @param[in,out] width
   * Texture width
   * @param[in,out] height
   * Texture height
   */
  static inline void get_mipmap_size(core::uint32 mipmapIndex, core::uint32 &width, core::uint32 &height) {
    get_mipmap_size(mipmapIndex, width);
    get_mipmap_size(mipmapIndex, height);
  }

  /**
   * @brief
   * Calculate the mipmap size at the given mipmap index
   *
   * @param[in] mipmapIndex
   * Mipmap index
   * @param[in,out] width
   * Texture width
   * @param[in,out] height
   * Texture height
   * @param[in,out] depth
   * Texture depth
   */
  static inline void
  get_mipmap_size(core::uint32 mipmapIndex, core::uint32 &width, core::uint32 &height, core::uint32 &depth) {
    get_mipmap_size(mipmapIndex, width);
    get_mipmap_size(mipmapIndex, height);
    get_mipmap_size(mipmapIndex, depth);
  }

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHITexture() override = default;

  // Protected methods
protected:
  /**
   * @brief
   * Constructor
   *
   * @param[in] resourceType
   * The resource type
   * @param[in] rhi
   * Owner RHI instance
   */
  inline RHITexture(ResourceType resourceType, RHIDevice &rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
  : RHIResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER) {

  }

  explicit RHITexture(const RHITexture &source) = delete;

  RHITexture &operator=(const RHITexture &source) = delete;

};

typedef core::Ptr<RHITexture> RHITexturePtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
