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
#include <core/container/array.h>
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
class VulkanHelper {
public:

  static const char* GLSL_NAME;

public:

  static void update_width_and_height(core::uint32 mipmapIndex, core::uint32 textureWidth, core::uint32 textureHeight, core::uint32& width, core::uint32& height);

  static void add_vk_pipeline_shader_stage_create_info(VkShaderStageFlagBits vkShaderStageFlagBits, VkShaderModule vkShaderModule, core::Array<VkPipelineShaderStageCreateInfo, 5>& vkPipelineShaderStageCreateInfos, core::uint32 stageCount);
public:

  [[nodiscard]] static VkSurfaceKHR create_presentation_surface(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkInstance vkInstance, VkPhysicalDevice vkPhysicalDevice, core::uint32 graphicsQueueFamilyIndex, core::FrontendPlatformData platformData);

  [[nodiscard]] static core::uint32 get_number_of_swap_chain_images(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR);

  [[nodiscard]] static VkSurfaceFormatKHR get_swap_chain_format(const rhi::RHIContext& context, VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurfaceKHR);

  [[nodiscard]] static VkExtent2D get_swap_chain_extent(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR);

  [[nodiscard]] static VkImageUsageFlags get_swap_chain_usage_flags(const rhi::RHIContext& context, const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR);

  [[nodiscard]] static VkSurfaceTransformFlagBitsKHR get_swap_chain_transform(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR);

  [[nodiscard]] static VkPresentModeKHR get_swap_chain_present_mode(const rhi::RHIContext& context, VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurfaceKHR);

  [[nodiscard]] static VkRenderPass create_render_pass(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, VkFormat colorVkFormat, VkFormat depthVkFormat, VkSampleCountFlagBits vkSampleCountFlagBits);

  [[nodiscard]] static VkFormat find_supported_vk_format(VkPhysicalDevice vkPhysicalDevice, const core::Vector<VkFormat>& vkFormatCandidates, VkImageTiling vkImageTiling, VkFormatFeatureFlags vkFormatFeatureFlags);

  [[nodiscard]] static VkResult create_vk_device(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkPhysicalDevice vkPhysicalDevice, const VkDeviceQueueCreateInfo& vkDeviceQueueCreateInfo, bool enableValidation, bool enableDebugMarker, bool hasMeshShaderSupport, VkDevice& vkDevice);

  [[nodiscard]] static VkDevice create_vk_device(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkPhysicalDevice vkPhysicalDevice, bool enableValidation, bool enableDebugMarker, bool hasMeshShaderSupport, core::uint32& graphicsQueueFamilyIndex, core::uint32& presentQueueFamilyIndex);

  [[nodiscard]] static bool has_vk_format_stencil_component(VkFormat vkFormat);

  [[nodiscard]] static VkCommandPool create_vk_command_pool(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, core::uint32 graphicsQueueFamilyIndex);

  [[nodiscard]] static VkCommandBuffer create_vk_command_buffer(const rhi::RHIContext& context, VkDevice vkDevice, VkCommandPool vkCommandPool);


  [[nodiscard]] static bool is_extension_available(const char* extensionName, const core::Vector<VkExtensionProperties>& vkExtensionPropertiesVector);

  static void enumerate_all_physical_devices(VkInstance vkInstance, core::Vector<VkPhysicalDevice>& physicalDevices);

  [[nodiscard]] static VkPhysicalDevice select_physical_device(const core::Vector<VkPhysicalDevice>& physicalDevices, bool validationEnabled, bool& enableDebugMarker, bool& hasMeshShaderSupport);


  static void begin_vulkan_render_pass(const rhi::RHIRenderTarget& renderTarget, VkRenderPass vkRenderPass, VkFramebuffer vkFramebuffer, core::uint32 numberOfAttachments, const core::Array<VkClearValue, 9>& vkClearValues, VkCommandBuffer vkCommandBuffer);

  //[-------------------------------------------------------]
  //[ Command                                               ]
  //[-------------------------------------------------------]
  [[nodiscard]] static VkCommandBuffer begin_single_time_commands(const RHIDevice& vulkanRhi);

  static void end_single_time_commands(const RHIDevice& vulkanRhi, VkCommandBuffer vkCommandBuffer);

  //[-------------------------------------------------------]
  //[ Transition                                            ]
  //[-------------------------------------------------------]
  static void transition_vk_image_layout(const RHIDevice& vulkanRhi, VkImage vkImage, VkImageAspectFlags vkImageAspectFlags, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout);

  static void transition_vk_image_layout(const RHIDevice& vulkanRhi, VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageAspectFlags vkImageAspectFlags, core::uint32 levelCount, core::uint32 layerCount, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout);

  static void transition_vk_image_layout(const RHIDevice& vulkanRhi, VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout, VkImageSubresourceRange vkImageSubresourceRange, VkPipelineStageFlags sourceVkPipelineStageFlags, VkPipelineStageFlags destinationVkPipelineStageFlags);


  //[-------------------------------------------------------]
  //[ Buffer                                                ]
  //[-------------------------------------------------------]
  static void create_and_allocate_vk_buffer(const RHIDevice& vulkanRhi, VkBufferUsageFlagBits vkBufferUsageFlagBits, VkMemoryPropertyFlags vkMemoryPropertyFlags, VkDeviceSize numberOfBytes, const void* data, VkBuffer& vkBuffer, VkDeviceMemory& vkDeviceMemory);

  static void destroy_and_free_vk_buffer(const RHIDevice& vulkanRhi, VkBuffer& vkBuffer, VkDeviceMemory& vkDeviceMemory);


  //[-------------------------------------------------------]
  //[ Image                                                 ]
  //[-------------------------------------------------------]
  [[nodiscard]] static VkImageLayout get_vk_image_layout_by_texture_flags(core::uint32 textureFlags);

  static VkFormat create_and_fill_vk_image(const RHIDevice& vulkanRhi, VkImageType vkImageType, VkImageViewType vkImageViewType, const VkExtent3D& vkExtent3D, rhi::TextureFormat::Enum textureFormat, const void* data, core::uint32 textureFlags, core::uint8 numberOfMultisamples, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory, VkImageView& vkImageView);

  static void create_and_allocate_vk_image(const RHIDevice& vulkanRhi, VkImageCreateFlags vkImageCreateFlags, VkImageType vkImageType, const VkExtent3D& vkExtent3D, core::uint32 mipLevels, core::uint32 arrayLayers, VkFormat vkFormat, VkSampleCountFlagBits vkSampleCountFlagBits, VkImageTiling vkImageTiling, VkImageUsageFlags vkImageUsageFlags, VkMemoryPropertyFlags vkMemoryPropertyFlags, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory);

  static void destroy_and_free_vk_image(const RHIDevice& vulkanRhi, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory);

  static void destroy_and_free_vk_image(const RHIDevice& vulkanRhi, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory, VkImageView& vkImageView);

  static void create_vk_image_view(const RHIDevice& vulkanRhi, VkImage vkImage, VkImageViewType vkImageViewType, core::uint32 levelCount, core::uint32 layerCount, VkFormat vkFormat, VkImageAspectFlags vkImageAspectFlags, VkImageView& vkImageView);

  //[-------------------------------------------------------]
  //[ Debug                                                 ]
  //[-------------------------------------------------------]
#ifdef DEBUG
  static void set_debug_object_name(VkDevice vkDevice, VkDebugReportObjectTypeEXT vkDebugReportObjectTypeEXT, core::uint64 object, const char* objectName);
#endif
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan