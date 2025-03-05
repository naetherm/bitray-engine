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
#include "rhi_vulkan/buffer/rhistructured_buffer.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIStructuredBuffer::RHIStructuredBuffer(RHIDevice& vulkanRhi, rhi::StructuredBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIStructuredBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVkBuffer(VK_NULL_HANDLE)
, mVkDeviceMemory(VK_NULL_HANDLE) {
  // Sanity checks
  BE_ASSERT((descriptor.numberOfBytes % descriptor.numberOfStructuredBytes) == 0, "The Vulkan structured buffer size must be a multiple of the given number of structure bytes")
  BE_ASSERT((descriptor.numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The Vulkan structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

  // Create the structured buffer
  VulkanHelper::create_and_allocate_vk_buffer(vulkanRhi, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, descriptor.numberOfBytes,
                                              descriptor.data, mVkBuffer, mVkDeviceMemory);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "SBO", 6)	// 6 = "SBO: " including terminating zero
    const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (core::uint64)mVkBuffer, detailedDebugName);
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (core::uint64)mVkDeviceMemory, detailedDebugName);
  }
#endif
}

RHIStructuredBuffer::~RHIStructuredBuffer() {
  VulkanHelper::destroy_and_free_vk_buffer(static_cast<const RHIDevice &>(get_rhi()), mVkBuffer, mVkDeviceMemory);
}


VkBuffer RHIStructuredBuffer::get_vk_buffer() const {
  return mVkBuffer;
}

VkDeviceMemory RHIStructuredBuffer::get_vk_device_memory() const {
  return mVkDeviceMemory;
}

void RHIStructuredBuffer::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
