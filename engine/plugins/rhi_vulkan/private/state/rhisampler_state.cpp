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
#include "rhi_vulkan/state/rhisampler_state.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_runtime_linking.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/mapping.h"
#include "rhi_vulkan/rhidynamicrhi.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHISamplerState::RHISamplerState(rhi_vulkan::RHIDevice &vulkanRhi, const rhi::SamplerStateDescriptor &samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHISamplerState(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVkSampler(VK_NULL_HANDLE) {
  const bool anisotropyEnable = (rhi::FilterMode::ANISOTROPIC == samplerState.filter || rhi::FilterMode::COMPARISON_ANISOTROPIC == samplerState.filter);
  const VkSamplerCreateInfo vkSamplerCreateInfo = {
    VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,                      // sType (VkStructureType)
    nullptr,                                    // pNext (const void*)
    0,                                        // flags (VkSamplerCreateFlags)
    Mapping::get_vulkan_mag_filter_mode(samplerState.filter),  // magFilter (VkFilter)
    Mapping::get_vulkan_min_filter_mode(samplerState.filter),  // minFilter (VkFilter)
    Mapping::get_vulkan_mipmap_mode(samplerState.filter),    // mipmapMode (VkSamplerMipmapMode)
    Mapping::get_vulkan_texture_address_mode(samplerState.addressU),          // addressModeU (VkSamplerAddressMode)
    Mapping::get_vulkan_texture_address_mode(samplerState.addressV),          // addressModeV (VkSamplerAddressMode)
    Mapping::get_vulkan_texture_address_mode(samplerState.addressW),          // addressModeW (VkSamplerAddressMode)
    samplerState.mipLodBias,                            // mipLodBias (float)
    static_cast<VkBool32>(anisotropyEnable),                    // anisotropyEnable (VkBool32)
    static_cast<float>(samplerState.maxAnisotropy),                  // maxAnisotropy (float)
    VK_FALSE,                                    // compareEnable (VkBool32)
    VK_COMPARE_OP_ALWAYS,                              // compareOp (VkCompareOp)
    samplerState.minLod,                              // minLod (float)
    samplerState.maxLod,                              // maxLod (float)
    VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,                    // borderColor (VkBorderColor)
    VK_FALSE                                    // unnormalizedCoordinates (VkBool32)
  };
  if (vkCreateSampler(vulkanRhi.get_vulkan_context().get_vk_device(), &vkSamplerCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &mVkSampler) == VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT) {
      RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Sampler state", 16)	// 16 = "Sampler state: " including terminating zero
      VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT, (core::uint64)mVkSampler, detailedDebugName);
    }
#endif
  } else {
    BE_LOG(Critical, "Failed to create Vulkan sampler instance")
  }
}

RHISamplerState::~RHISamplerState() {

}


VkSampler RHISamplerState::get_vk_sampler() const {
  return mVkSampler;
}

void RHISamplerState::self_destruct() {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan