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
#include "rhi/texture/rhitexture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract cube texture array interface
 *
 * @note
 * - Common use case for cube texture array: Multiple environment cube maps (created e.g. via environment probes)
 */
class RHITextureCubeArray : public RHITexture {

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHITextureCubeArray() override {
#ifdef DEBUG
    // Update the statistics
    --get_rhi().get_statistics().currentNumberOfTextureCubeArrays;
#endif
  }

  /**
   * @brief
   * Return the width of the texture
   *
   * @return
   * The width of the texture
   */
  [[nodiscard]] inline core::uint32 get_width() const {
    return mDescriptor.size.width;
  }

  /**
   * @brief
   * Return the number of slices
   *
   * @return
   * The number of slices
   */
  [[nodiscard]] inline core::uint32 get_number_of_slices() const {
    return mDescriptor.size.depth;
  }

  // Protected methods
protected:
  /**
   * @brief
   * Constructor
   *
   * @param[in] rhi
   * Owner RHI instance
   * @param[in] width
   * The width of the texture
   * @param[in] numberOfSlices
   * The number of slices
   */
  inline RHITextureCubeArray(RHIDevice &rhi, rhi::TextureDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
  : RHITexture(ResourceType::TEXTURE_CUBE_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  , mDescriptor(descriptor) {
#ifdef DEBUG
    // Update the statistics
    ++rhi.get_statistics().numberOfCreatedTextureCubeArrays;
    ++rhi.get_statistics().currentNumberOfTextureCubeArrays;
#endif
  }

  explicit RHITextureCubeArray(const RHITextureCubeArray &source) = delete;

  RHITextureCubeArray &operator=(const RHITextureCubeArray &source) = delete;

  // Private data
private:
  rhi::TextureDescriptor mDescriptor;

};

typedef core::Ptr<RHITextureCubeArray> RHITextureCubeArrayPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
