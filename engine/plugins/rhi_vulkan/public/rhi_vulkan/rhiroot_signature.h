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
#include <rhi/rhiroot_signature.h>
#include <core/container/vector.h>


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
*    Vulkan root signature ("pipeline layout" in Vulkan terminology) class
*/
class RHIRootSignature final : public rhi::RHIRootSignature {
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
   *  @param[in] rootSignature
   *    Root signature to use
   */
  RHIRootSignature(RHIDevice& vulkanRhi, const rhi::RootSignatureDescriptor& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   *  @brief
   *    Destructor
   */
  ~RHIRootSignature() override;

  /**
   *  @brief
   *    Return the root signature data
   *
   *  @return
   *    The root signature data
   */
  [[nodiscard]] const rhi::RootSignatureDescriptor& get_root_signature() const;

  /**
   *  @brief
   *    Return the Vulkan pipeline layout
   *
   *  @return
   *    The Vulkan pipeline layout
   */
  [[nodiscard]] VkPipelineLayout get_vk_pipeline_layout() const;

  /**
   *  @brief
   *    Return the Vulkan descriptor pool
   *
   *  @return
   *    The Vulkan descriptor pool
   */
  [[nodiscard]] VkDescriptorPool get_vk_descriptor_pool() const;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIRootSignature methods            ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual rhi::RHIResourceGroup* create_resource_group(core::uint32 rootParameterIndex, core::uint32 numberOfResources, rhi::RHIResource** resources, rhi::RHISamplerState** samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void self_destruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIRootSignature(const RHIRootSignature& source) = delete;
  RHIRootSignature& operator =(const RHIRootSignature& source) = delete;


  //[-------------------------------------------------------]
  //[ Private definitions                                   ]
  //[-------------------------------------------------------]
private:
  typedef core::Vector<VkDescriptorSetLayout> VkDescriptorSetLayouts;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  rhi::RootSignatureDescriptor	   mRootSignature;
  VkDescriptorSetLayouts mVkDescriptorSetLayouts;
  VkPipelineLayout	   mVkPipelineLayout;
  VkDescriptorPool	   mVkDescriptorPool;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
