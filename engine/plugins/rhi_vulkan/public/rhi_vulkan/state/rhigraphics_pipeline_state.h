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
#include "rhi_vulkan/rhi_vulkan.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 *  @brief
 *    Vulkan graphics pipeline state class
 */
class RHIGraphicsPipelineState final : public rhi::RHIGraphicsPipelineState
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   *  @brief
   *    Constructor
   *
   *  @param[in] vulkanRhi
   *    Owner Vulkan RHI instance
   *  @param[in] graphicsPipelineState
   *    Graphics pipeline state to use
   *  @param[in] id
   *    The unique compact graphics pipeline state ID
   */
  RHIGraphicsPipelineState(RHIDevice& vulkanRhi, const rhi::GraphicsPipelineStateDescriptor& graphicsPipelineState, core::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   *  @brief
   *    Destructor
   */
  ~RHIGraphicsPipelineState() override;

  /**
   *  @brief
   *    Return the Vulkan graphics pipeline
   *
   *  @return
   *    The Vulkan graphics pipeline
   */
  [[nodiscard]] VkPipeline get_vk_pipeline() const;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline void self_destruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIGraphicsPipelineState(const RHIGraphicsPipelineState& source) = delete;
  RHIGraphicsPipelineState& operator =(const RHIGraphicsPipelineState& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  rhi::RHIRootSignature*   mRootSignature;
  rhi::RHIGraphicsProgram* mGraphicsProgram;
  rhi::RHIRenderPass*	   mRenderPass;
  /** The Vulkan graphics pipeline */
  VkPipeline			   mVkPipeline;	
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
