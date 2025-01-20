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
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_runtime_linking.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include <core/container/vector.h>
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
VulkanContext::VulkanContext(RHIDevice &rhiDynamicRhi)
: mRhiDynamicRhi(rhiDynamicRhi)
, mVkPhysicalDevice(VK_NULL_HANDLE)
, mVkDevice(VK_NULL_HANDLE)
, mGraphicsQueueFamilyIndex(~0u)
, mPresentQueueFamilyIndex(~0u)
, mGraphicsVkQueue(VK_NULL_HANDLE)
, mPresentVkQueue(VK_NULL_HANDLE)
, mVkCommandPool(VK_NULL_HANDLE)
, mVkCommandBuffer(VK_NULL_HANDLE) {
  // Get runtime linking, we will need that
  const VulkanRuntimeLinking& vulkanRuntimeLinking = mRhiDynamicRhi.get_vulkan_runtime_linking();

#if defined(DEBUG)
  bool enableDebugMarker = true;
#else
  bool enableDebugMarker = false;
#endif
  bool hasMeshShaderSupport = false;

  // Enumerate all physical devices
  {
    BE_LOG(Warning, "Before")
    core::Vector<VkPhysicalDevice> physicalDevices;
    VulkanHelper::enumerate_all_physical_devices(vulkanRuntimeLinking.get_vk_instance(), physicalDevices);
    //core::uint32 numPhysicalDevices = 0;
    //vkEnumeratePhysicalDevices(vulkanRuntimeLinking.get_vk_instance(), &numPhysicalDevices, null);
    BE_LOG(Warning, "After")
    if (!physicalDevices.empty()) {
      mVkPhysicalDevice = VulkanHelper::select_physical_device(physicalDevices, vulkanRuntimeLinking.is_validation_enabled(), enableDebugMarker, hasMeshShaderSupport);
    }
  }

  // Now that we have the physical device, let's create the logical device
  if (mVkPhysicalDevice != VK_NULL_HANDLE) {
    mVkDevice = VulkanHelper::create_vk_device(mRhiDynamicRhi.get_context(), mRhiDynamicRhi.get_vk_allocation_callbacks(),
                                           mVkPhysicalDevice, vulkanRuntimeLinking.is_validation_enabled(),
                                           enableDebugMarker, hasMeshShaderSupport, mGraphicsQueueFamilyIndex,
                                           mPresentQueueFamilyIndex);

    if (VK_NULL_HANDLE != mVkDevice) {
      // Get the Vulkan device graphics queue that command buffers are submitted to
      vkGetDeviceQueue(mVkDevice, mGraphicsQueueFamilyIndex, 0, &mGraphicsVkQueue);
      if (VK_NULL_HANDLE != mGraphicsVkQueue) {
        // Get the Vulkan device present queue
        vkGetDeviceQueue(mVkDevice, mPresentQueueFamilyIndex, 0, &mPresentVkQueue);
        if (VK_NULL_HANDLE != mPresentVkQueue) {
          // Create Vulkan command pool instance
          mVkCommandPool = VulkanHelper::create_vk_command_pool(mRhiDynamicRhi.get_context(),
                                                            mRhiDynamicRhi.get_vk_allocation_callbacks(), mVkDevice,
                                                            mGraphicsQueueFamilyIndex);
          if (VK_NULL_HANDLE != mVkCommandPool) {
            // Create Vulkan command buffer instance
            mVkCommandBuffer = VulkanHelper::create_vk_command_buffer(mRhiDynamicRhi.get_context(), mVkDevice, mVkCommandPool);
          } else {
            // Error!
            BE_LOG(Critical, "Failed to create Vulkan command pool instance")
          }
        }
      } else {
        // Error!
        BE_LOG(Critical, "Failed to get the Vulkan device graphics queue that command buffers are submitted to")
      }
    }
  }
}

VulkanContext::~VulkanContext() {
  if (VK_NULL_HANDLE != mVkDevice) {
    if (VK_NULL_HANDLE != mVkCommandPool) {
      if (VK_NULL_HANDLE != mVkCommandBuffer) {
        vkFreeCommandBuffers(mVkDevice, mVkCommandPool, 1, &mVkCommandBuffer);
      }
      vkDestroyCommandPool(mVkDevice, mVkCommandPool, mRhiDynamicRhi.get_vk_allocation_callbacks());
    }
    vkDeviceWaitIdle(mVkDevice);
    vkDestroyDevice(mVkDevice, mRhiDynamicRhi.get_vk_allocation_callbacks());
  }
}


bool VulkanContext::is_initialized() const {
  return (VK_NULL_HANDLE != mVkCommandBuffer);
}

const RHIDevice& VulkanContext::get_rhidynamicrhi() const {
  return mRhiDynamicRhi;
}

VkDevice VulkanContext::get_vk_device() const {
  return mVkDevice;
}

VkPhysicalDevice VulkanContext::get_vk_physical_device() const {
  return mVkPhysicalDevice;
}

core::uint32 VulkanContext::get_graphics_queue_family_index() const {
  return mGraphicsQueueFamilyIndex;
}

core::uint32 VulkanContext::get_present_queue_family_index() const {
  return mPresentQueueFamilyIndex;
}

VkQueue VulkanContext::get_graphics_vk_queue() const {
  return mGraphicsVkQueue;
}

VkQueue VulkanContext::get_present_vk_queue() const {
  return mPresentVkQueue;
}

VkCommandPool VulkanContext::get_vk_command_pool() const {
  return mVkCommandPool;
}

VkCommandBuffer VulkanContext::get_vk_command_buffer() const {
  return mVkCommandBuffer;
}

core::uint32 VulkanContext::find_memory_type_index(core::uint32 typeFilter, VkMemoryPropertyFlags vkMemoryPropertyFlags) const {
  VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;
  vkGetPhysicalDeviceMemoryProperties(mVkPhysicalDevice, &vkPhysicalDeviceMemoryProperties);
  for (core::uint32 i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) && (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & vkMemoryPropertyFlags) == vkMemoryPropertyFlags) {
      return i;
    }
  }

  // Error!
  BE_LOG(Critical, "Failed to find suitable Vulkan memory type")
  return ~0u;
}

VkCommandBuffer VulkanContext::create_vk_command_buffer() const {
  return VulkanHelper::create_vk_command_buffer(mRhiDynamicRhi.get_context(), mVkDevice, mVkCommandPool);
}

void VulkanContext::destroy_vk_command_buffer(VkCommandBuffer vkCommandBuffer) const {
  if (VK_NULL_HANDLE != mVkCommandBuffer) {
    vkFreeCommandBuffers(mVkDevice, mVkCommandPool, 1, &vkCommandBuffer);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan