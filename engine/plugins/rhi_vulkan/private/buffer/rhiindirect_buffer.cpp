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
#include "rhi_vulkan/buffer/rhiindirect_buffer.h"
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
RHIIndirectBuffer::RHIIndirectBuffer(RHIDevice& vulkanRhi, rhi::IndirectBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIIndirectBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVkBuffer(VK_NULL_HANDLE)
, mVkDeviceMemory(VK_NULL_HANDLE) {
  // Sanity checks
  BE_ASSERT((descriptor.flags & rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 || (descriptor.flags & rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0, "Invalid Vulkan flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" is missing")
  BE_ASSERT(!((descriptor.flags & rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 && (descriptor.flags & rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0), "Invalid Vulkan flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" must be set, but not both at one and the same time")
  BE_ASSERT((descriptor.flags & rhi::IndirectBufferFlag::DRAW_ARGUMENTS) == 0 || (descriptor.numberOfBytes % sizeof(rhi::DrawArguments)) == 0, "Vulkan indirect buffer element type flags specification is \"DRAW_ARGUMENTS\" but the given number of bytes don't align to this")
  BE_ASSERT((descriptor.flags & rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) == 0 || (descriptor.numberOfBytes % sizeof(rhi::DrawIndexedArguments)) == 0, "Vulkan indirect buffer element type flags specification is \"DRAW_INDEXED_ARGUMENTS\" but the given number of bytes don't align to this")

  // Create indirect buffer
  int vkBufferUsageFlagBits = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
  if ((descriptor.flags & rhi::IndirectBufferFlag::UNORDERED_ACCESS) != 0 || (descriptor.flags & rhi::IndirectBufferFlag::SHADER_RESOURCE) != 0) {
    vkBufferUsageFlagBits |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  }
  VulkanHelper::create_and_allocate_vk_buffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(vkBufferUsageFlagBits),
                                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, descriptor.numberOfBytes,
                                              descriptor.data, mVkBuffer, mVkDeviceMemory);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IndirectBufferObject", 23)	// 23 = "IndirectBufferObject: " including terminating zero
    const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (core::uint64)mVkBuffer, detailedDebugName);
            VulkanHelper::set_debug_object_name(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (core::uint64)mVkDeviceMemory, detailedDebugName);
  }
#endif
}

RHIIndirectBuffer::~RHIIndirectBuffer() {
  VulkanHelper::destroy_and_free_vk_buffer(static_cast<const RHIDevice &>(get_rhi()), mVkBuffer, mVkDeviceMemory);
}


VkBuffer RHIIndirectBuffer::get_vk_buffer() const {
  return mVkBuffer;
}

VkDeviceMemory RHIIndirectBuffer::get_vk_device_memory() const {
  return mVkDeviceMemory;
}

const core::uint8* RHIIndirectBuffer::get_emulation_data() const override {
  return nullptr;
}

void RHIIndirectBuffer::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
