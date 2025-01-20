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
#include "rhi_vulkan/buffer/rhiuniform_buffer.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIUniformBuffer::RHIUniformBuffer(RHIDevice& vulkanRhi, rhi::UniformBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIUniformBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVkBuffer(VK_NULL_HANDLE)
, mVkDeviceMemory(VK_NULL_HANDLE) {
  VulkanHelper::create_and_allocate_vk_buffer(vulkanRhi,
                                              static_cast<VkBufferUsageFlagBits>(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
                                                                                 VK_BUFFER_USAGE_STORAGE_BUFFER_BIT),
                                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, descriptor.numberOfBytes,
                                              descriptor.data, mVkBuffer, mVkDeviceMemory);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "UBO", 6)	// 6 = "UBO: " including terminating zero
    const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (core::uint64)mVkBuffer, detailedDebugName);
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (core::uint64)mVkDeviceMemory, detailedDebugName);
  }
#endif
}

RHIUniformBuffer::~RHIUniformBuffer() {
  VulkanHelper::destroy_and_free_vk_buffer(static_cast<const RHIDevice &>(get_rhi()), mVkBuffer, mVkDeviceMemory);
}

VkBuffer RHIUniformBuffer::get_vk_buffer() const {
  return mVkBuffer;
}

VkDeviceMemory RHIUniformBuffer::get_vk_device_memory() const {
  return mVkDeviceMemory;
}

void RHIUniformBuffer::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
