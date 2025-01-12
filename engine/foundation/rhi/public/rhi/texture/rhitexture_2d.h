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
 * Abstract 2D texture interface
 *
 * @note
 * - Common use case for 2D texture: Pretty much every standard 2D texture
 */
class RHITexture2D : public RHITexture {

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHITexture2D() override {
#ifdef DEBUG
    // Update the statistics
    --get_rhi().get_statistics().currentNumberOfTexture2Ds;
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
   * Return the height of the texture
   *
   * @return
   * The height of the texture
   */
  [[nodiscard]] inline core::uint32 get_height() const {
    return mDescriptor.size.height;
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
   * @param[in] height
   * The height of the texture
   */
  inline RHITexture2D(RHIDevice &rhi, rhi::TextureDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
  : RHITexture(ResourceType::TEXTURE_2D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  , mDescriptor(descriptor) {
#ifdef DEBUG
    // Update the statistics
    ++rhi.get_statistics().numberOfCreatedTexture2Ds;
    ++rhi.get_statistics().currentNumberOfTexture2Ds;
#endif
  }

  explicit RHITexture2D(const RHITexture2D &source) = delete;

  RHITexture2D &operator=(const RHITexture2D &source) = delete;

  // Private data
private:
  rhi::TextureDescriptor mDescriptor;

};

typedef core::Ptr<RHITexture2D> RHITexture2DPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
