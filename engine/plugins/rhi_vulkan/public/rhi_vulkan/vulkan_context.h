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
class VulkanContext {
public:

  VulkanContext(RHIDevice& rhiDynamicRhi);

  ~VulkanContext();

public:

  [[nodiscard]] bool is_initialized() const;

  [[nodiscard]] const RHIDevice& get_rhidynamicrhi() const;

  [[nodiscard]] VkDevice get_vk_device() const;

  [[nodiscard]] VkPhysicalDevice get_vk_physical_device() const;

  [[nodiscard]] core::uint32 get_graphics_queue_family_index() const;

  [[nodiscard]] core::uint32 get_present_queue_family_index() const;

  [[nodiscard]] VkQueue get_graphics_vk_queue() const;

  [[nodiscard]] VkQueue get_present_vk_queue() const;

  [[nodiscard]] VkCommandPool get_vk_command_pool() const;

  [[nodiscard]] VkCommandBuffer get_vk_command_buffer() const;

  [[nodiscard]] core::uint32 find_memory_type_index(core::uint32 typeFilter, VkMemoryPropertyFlags vkMemoryPropertyFlags) const;

  [[nodiscard]] VkCommandBuffer create_vk_command_buffer() const;

  void destroy_vk_command_buffer(VkCommandBuffer vkCommandBuffer) const;

private:

  RHIDevice& mRhiDynamicRhi;
  VkPhysicalDevice mVkPhysicalDevice;
  VkDevice mVkDevice;
  core::uint32 mGraphicsQueueFamilyIndex;
  core::uint32 mPresentQueueFamilyIndex;
  VkQueue	mGraphicsVkQueue;
  VkQueue mPresentVkQueue;
  VkCommandPool	mVkCommandPool;
  VkCommandBuffer mVkCommandBuffer;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan