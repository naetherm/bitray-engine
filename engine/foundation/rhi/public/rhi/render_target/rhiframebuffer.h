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
#include "rhi/render_target/rhirender_target.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class RHITexture;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
struct FramebufferAttachment final {
  RHITexture *texture;
  core::uint32 mipmapIndex;
  core::uint32 layerIndex;  ///< "slice" in Direct3D terminology, depending on the texture type it's a 2D texture array layer, 3D texture slice or cube map face
  inline FramebufferAttachment() :
    texture(nullptr),
    mipmapIndex(0),
    layerIndex(0) {}

  inline FramebufferAttachment(RHITexture *_texture, core::uint32 _mipmapIndex = 0, core::uint32 _layerIndex = 0) :
    texture(_texture),
    mipmapIndex(_mipmapIndex),
    layerIndex(_layerIndex) {}
};

/**
 * @brief
 * Abstract framebuffer (FBO) interface
 */
class RHIFramebuffer : public RHIRenderTarget {

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHIFramebuffer() override {
#ifdef DEBUG
    // Update the statistics
    --get_rhi().get_statistics().currentNumberOfFramebuffers;
#endif
  }

  // Protected methods
protected:
  /**
   * @brief
   * Constructor
   *
   * @param[in] renderPass
   * Render pass to use, the framebuffer a reference to the render pass
   */
  inline explicit RHIFramebuffer(RHIRenderPass &renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
  : RHIRenderTarget(ResourceType::FRAMEBUFFER, renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef DEBUG
    // Update the statistics
    ++get_rhi().get_statistics().numberOfCreatedFramebuffers;
    ++get_rhi().get_statistics().currentNumberOfFramebuffers;
#endif
  }

  explicit RHIFramebuffer(const RHIFramebuffer &source) = delete;

  RHIFramebuffer &operator=(const RHIFramebuffer &source) = delete;

};

typedef core::Ptr<RHIFramebuffer> RHIFramebufferPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
