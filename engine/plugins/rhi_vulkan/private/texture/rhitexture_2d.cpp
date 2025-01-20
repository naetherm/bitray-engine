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
#include "rhi_vulkan/texture/rhitexture_2d.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_runtime_linking.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHITexture2D::RHITexture2D(RHIDevice &vulkanRhi, rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHITexture2D(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVrVulkanTextureData{}
, mVkImageLayout(VulkanHelper::get_vk_image_layout_by_texture_flags(descriptor.textureFlags))
, mVkDeviceMemory(VK_NULL_HANDLE)
, mVkImageView(VK_NULL_HANDLE) {
  mVrVulkanTextureData.m_nFormat = VulkanHelper::create_and_fill_vk_image(
      vulkanRhi, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D,
      {descriptor.size.width, descriptor.size.height, 1},
      descriptor.textureFormat, data, descriptor.textureFlags,
      descriptor.numberOfMultisamples,
      mVrVulkanTextureData.m_nImage,
      mVkDeviceMemory, mVkImageView);

  // Fill the rest of the "VRVulkanTextureData_t"-structure
  const VulkanContext &vulkanContext = vulkanRhi.get_vulkan_context();
  const VulkanRuntimeLinking &vulkanRuntimeLinking = vulkanRhi.get_vulkan_runtime_linking();
  // m_nImage (VkImage) was set by "RHIDevice::Helper::createAndFillVkImage()" above
  mVrVulkanTextureData.m_pDevice = vulkanContext.get_vk_device();          // m_pDevice (VkDevice)
  mVrVulkanTextureData.m_pPhysicalDevice = vulkanContext.get_vk_physical_device();      // m_pPhysicalDevice (VkPhysicalDevice)
  mVrVulkanTextureData.m_pInstance = vulkanRuntimeLinking.get_vk_instance();    // m_pInstance (VkInstance)
  mVrVulkanTextureData.m_pQueue = vulkanContext.get_graphics_vk_queue();      // m_pQueue (VkQueue)
  mVrVulkanTextureData.m_nQueueFamilyIndex = vulkanContext.get_graphics_queue_family_index();  // m_nQueueFamilyIndex (core::uint32)
  mVrVulkanTextureData.m_nWidth = descriptor.size.width;                    // m_nWidth (core::uint32)
  mVrVulkanTextureData.m_nHeight = descriptor.size.height;                    // m_nHeight (core::uint32)
  // m_nFormat (VkFormat)  was set by "RHIDevice::Helper::createAndFillVkImage()" above
  mVrVulkanTextureData.m_nSampleCount = descriptor.numberOfMultisamples;            // m_nSampleCount (core::uint32)

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)	// 13 = "2D texture: " including terminating zero
    const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
    VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (core::uint64)mVrVulkanTextureData.m_nImage, detailedDebugName);
    VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (core::uint64)mVkDeviceMemory, detailedDebugName);
    VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (core::uint64)mVkImageView, detailedDebugName);
  }
#endif
}

RHITexture2D::~RHITexture2D() {
  VulkanHelper::destroy_and_free_vk_image(static_cast<RHIDevice &>(get_rhi()), mVrVulkanTextureData.m_nImage, mVkDeviceMemory, mVkImageView);
}

VkImageView RHITexture2D::get_vk_image_view() const {
  return mVkImageView;
}

VkImageLayout RHITexture2D::get_vk_image_layout() const {
  return mVkImageLayout;
}

VkFormat RHITexture2D::get_vk_format() const {
  return mVrVulkanTextureData.m_nFormat;
}

void RHITexture2D::set_minimum_maximum_mipmap_index([[maybe_unused]] core::uint32 minimumMipmapIndex, [[maybe_unused]] core::uint32 maximumMipmapIndex) {
  // TODO(naetherm) Implement me
}

void* RHITexture2D::get_internal_resource_handle() const override {
  return reinterpret_cast<void*>(const_cast<VRVulkanTextureData_t*>(&mVrVulkanTextureData));
}

void RHITexture2D::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
