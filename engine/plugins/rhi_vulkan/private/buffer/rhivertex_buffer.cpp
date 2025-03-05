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
#include "rhi_vulkan/buffer/rhivertex_buffer.h"
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
RHIVertexBuffer::RHIVertexBuffer(RHIDevice& vulkanRhi, rhi::VertexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIVertexBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVkBuffer(VK_NULL_HANDLE)
, mVkDeviceMemory(VK_NULL_HANDLE) {
  int vkBufferUsageFlagBits = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  if ((descriptor.flags & rhi::BufferFlag::UNORDERED_ACCESS) != 0 || (descriptor.flags & rhi::BufferFlag::SHADER_RESOURCE) != 0) {
    vkBufferUsageFlagBits |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  }
  VulkanHelper::create_and_allocate_vk_buffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(vkBufferUsageFlagBits),
                                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, descriptor.numberOfBytes,
                                              descriptor.data, mVkBuffer, mVkDeviceMemory);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VBO", 6)	// 6 = "VBO: " including terminating zero
    const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (core::uint64)mVkBuffer, detailedDebugName);
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (core::uint64)mVkDeviceMemory, detailedDebugName);
  }
#endif
}

RHIVertexBuffer::~RHIVertexBuffer() {
  VulkanHelper::destroy_and_free_vk_buffer(static_cast<const RHIDevice &>(get_rhi()), mVkBuffer, mVkDeviceMemory);
}


VkBuffer RHIVertexBuffer::get_vk_buffer() const {
  return mVkBuffer;
}

VkDeviceMemory RHIVertexBuffer::get_vk_device_memory() const {
  return mVkDeviceMemory;
}

void RHIVertexBuffer::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
