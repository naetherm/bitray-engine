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
class RHIRootSignature;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Vulkan resource group class
*/
class RHIResourceGroup final : public rhi::RHIResourceGroup
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   *  @brief
   *    Constructor
   *
   *  @param[in] rootSignature
   *    Root signature
   *  @param[in] rootParameterIndex
   *    Root parameter index
   *  @param[in] vkDescriptorSet
   *    Wrapped Vulkan descriptor set
   *  @param[in] numberOfResources
   *    Number of resources, having no resources is invalid
   *  @param[in] resources
   *    At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
   *  @param[in] samplerStates
   *    If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
   */
  RHIResourceGroup(RHIRootSignature& rootSignature, core::uint32 rootParameterIndex, VkDescriptorSet vkDescriptorSet, core::uint32 numberOfResources, rhi::RHIResource** resources, rhi::RHISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   *  @brief
   *    Destructor
   */
  ~RHIResourceGroup() override;

  /**
   *  @brief
   *    Return the Vulkan descriptor set
   *
   *  @return
   *    The Vulkan descriptor set, can be a null handle
   */
  [[nodiscard]] VkDescriptorSet get_vk_descriptor_set() const;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void self_destruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIResourceGroup(const RHIResourceGroup& source) = delete;
  RHIResourceGroup& operator =(const RHIResourceGroup& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** Root signature */
  RHIRootSignature&		 mRootSignature;
  /** "mVkDescriptorPool" of the root signature is the owner which manages the memory, can be a null handle (e.g. for a sampler resource group) */
  VkDescriptorSet		 mVkDescriptorSet;
  /** Number of resources this resource group groups together */
  core::uint32			 mNumberOfResources;
  /** RHI resource, we keep a reference to it */
  rhi::RHIResource**	 mResources;
  /** Sampler states, we keep a reference to it */
  rhi::RHISamplerState** mSamplerStates;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
