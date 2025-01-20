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
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_runtime_linking.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/mapping.h"
#include "rhi_vulkan/volk.h"
#include <core/container/array.h>


namespace internal {

typedef core::Vector<VkExtensionProperties> VkExtensionPropertiesVector;

#ifdef __ANDROID__
// On Android we need to explicitly select all layers
#warning "TODO(naetherm) Not tested"
static constexpr core::uint32 NUMBER_OF_VALIDATION_LAYERS = 6;
static constexpr const char* VALIDATION_LAYER_NAMES[] = {
  "VK_LAYER_GOOGLE_threading",
  "VK_LAYER_LUNARG_parameter_validation",
  "VK_LAYER_LUNARG_object_tracker",
  "VK_LAYER_LUNARG_core_validation",
  "VK_LAYER_LUNARG_swapchain",
  "VK_LAYER_GOOGLE_unique_objects"
};
#else
// On desktop the LunarG loaders exposes a meta layer that contains all layers
static constexpr core::uint32 NUMBER_OF_VALIDATION_LAYERS = 1;
static constexpr const char *VALIDATION_LAYER_NAMES[] = {
  "VK_LAYER_LUNARG_standard_validation"
};
#endif

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {

const char* VulkanHelper::GLSL_NAME = "GLSL";

//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
void VulkanHelper::update_width_and_height(core::uint32 mipmapIndex, core::uint32 textureWidth, core::uint32 textureHeight, core::uint32& width, core::uint32& height) {
  rhi::RHITexture::get_mipmap_size(mipmapIndex, textureWidth, textureHeight);
  if (width > textureWidth) {
    width = textureWidth;
  }
  if (height > textureHeight) {
    height = textureHeight;
  }
}

void VulkanHelper::add_vk_pipeline_shader_stage_create_info(VkShaderStageFlagBits vkShaderStageFlagBits, VkShaderModule vkShaderModule, core::Array<VkPipelineShaderStageCreateInfo, 5>& vkPipelineShaderStageCreateInfos, core::uint32 stageCount) {
  VkPipelineShaderStageCreateInfo& vkPipelineShaderStageCreateInfo = vkPipelineShaderStageCreateInfos[stageCount];
  vkPipelineShaderStageCreateInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;	// sType (VkStructureType)
  vkPipelineShaderStageCreateInfo.pNext				= nullptr;												// pNext (const void*)
  vkPipelineShaderStageCreateInfo.flags				= 0;													// flags (VkPipelineShaderStageCreateFlags)
  vkPipelineShaderStageCreateInfo.stage				= vkShaderStageFlagBits;								// stage (VkShaderStageFlagBits)
  vkPipelineShaderStageCreateInfo.module				= vkShaderModule;										// module (VkShaderModule)
  vkPipelineShaderStageCreateInfo.pName				= "main";												// pName (const char*)
  vkPipelineShaderStageCreateInfo.pSpecializationInfo	= nullptr;												// pSpecializationInfo (const VkSpecializationInfo*)
}

VkSurfaceKHR VulkanHelper::create_presentation_surface(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkInstance vkInstance, VkPhysicalDevice vkPhysicalDevice, core::uint32 graphicsQueueFamilyIndex, core::FrontendPlatformData platformData) {
  VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE;

#ifdef VK_USE_PLATFORM_WIN32_KHR
  const VkWin32SurfaceCreateInfoKHR vkWin32SurfaceCreateInfoKHR =
  {
    VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,																		// sType (VkStructureType)
    nullptr,																												// pNext (const void*)
    0,																														// flags (VkWin32SurfaceCreateFlagsKHR)
    reinterpret_cast<HINSTANCE>(::GetWindowLongPtr(reinterpret_cast<HWND>(windoInfo.nativeWindowHandle), GWLP_HINSTANCE)),	// hinstance (HINSTANCE)
    reinterpret_cast<HWND>(windoInfo.nativeWindowHandle)																	// hwnd (HWND)
  };
  if (vkCreateWin32SurfaceKHR(vkInstance, &vkWin32SurfaceCreateInfoKHR, vkAllocationCallbacks, &vkSurfaceKHR) != VK_SUCCESS) {
    // TODO(naetherm) Can we ensure "vkSurfaceKHR" doesn't get touched by "vkCreateWin32SurfaceKHR()" in case of failure?
    vkSurfaceKHR = VK_NULL_HANDLE;
  }
#elif defined VK_USE_PLATFORM_ANDROID_KHR
  #warning "TODO(naetherm) Not tested"
  const VkAndroidSurfaceCreateInfoKHR vkAndroidSurfaceCreateInfoKHR =
  {
    VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,				// sType (VkStructureType)
    nullptr,														// pNext (const void*)
    0,																// flags (VkAndroidSurfaceCreateFlagsKHR)
    reinterpret_cast<ANativeWindow*>(windoInfo.nativeWindowHandle)	// window (ANativeWindow*)
  };
  if (vkCreateAndroidSurfaceKHR(vkInstance, &vkAndroidSurfaceCreateInfoKHR, vkAllocationCallbacks, &vkSurfaceKHR) != VK_SUCCESS) {
    // TODO(naetherm) Can we ensure "vkSurfaceKHR" doesn't get touched by "vkCreateAndroidSurfaceKHR()" in case of failure?
    vkSurfaceKHR = VK_NULL_HANDLE;
  }
#elif defined VK_USE_PLATFORM_XCB_KHR

  const VkXcbSurfaceCreateInfoKHR vkXcbSurfaceCreateInfoKHR = {
    VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,	// sType (VkStructureType)
    nullptr,										// pNext (const void*)
    0,												// flags (VkXcbSurfaceCreateFlagsKHR)
    static_cast<xcb_connection_t *>(platformData.os_linux.connection),										// connection (xcb_connection_t*)
    static_cast<xcb_window_t>(platformData.os_linux.window)										// window (xcb_window_t)
  };
  if (vkCreateXcbSurfaceKHR(vkInstance, &vkXcbSurfaceCreateInfoKHR, vkAllocationCallbacks, &vkSurfaceKHR) != VK_SUCCESS) {
    // TODO(naetherm) Can we ensure "vkSurfaceKHR" doesn't get touched by "vkCreateXcbSurfaceKHR()" in case of failure?
    vkSurfaceKHR = VK_NULL_HANDLE;
  }
  #else
  #error "Unsupported platform"
#endif

  { // Sanity check: Does the physical Vulkan device support the Vulkan presentation surface?
    // TODO(naetherm) Inside our RHI implementation the swap chain is physical device independent, which is a nice thing usability wise.
    //          On the other hand, the sanity check here can only detect issues but it would be better to not get into such issues in the first place.
    VkBool32 queuePresentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, graphicsQueueFamilyIndex, vkSurfaceKHR, &queuePresentSupport);
    if (VK_FALSE == queuePresentSupport) {
      BE_LOG(Critical, "The created Vulkan presentation surface has no queue present support")
    }
  }

  // Done
  return vkSurfaceKHR;
}

core::uint32 VulkanHelper::get_number_of_swap_chain_images(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR) {
  // Set of images defined in a swap chain may not always be available for application to render to:
  // - One may be displayed and one may wait in a queue to be presented
  // - If application wants to use more images at the same time it must ask for more images
  core::uint32 numberOfImages = vkSurfaceCapabilitiesKHR.minImageCount + 1;
  if ((vkSurfaceCapabilitiesKHR.maxImageCount > 0) && (numberOfImages > vkSurfaceCapabilitiesKHR.maxImageCount)) {
    numberOfImages = vkSurfaceCapabilitiesKHR.maxImageCount;
  }
  return numberOfImages;
}

VkExtent2D VulkanHelper::get_swap_chain_extent(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR) {
  // Special value of surface extent is width == height == -1
  // -> If this is so we define the size by ourselves but it must fit within defined confines, else it's already set to the operation window dimension
  if (-1 == vkSurfaceCapabilitiesKHR.currentExtent.width) {
    VkExtent2D swapChainExtent = { 640, 480 };
    if (swapChainExtent.width < vkSurfaceCapabilitiesKHR.minImageExtent.width) {
      swapChainExtent.width = vkSurfaceCapabilitiesKHR.minImageExtent.width;
    }
    if (swapChainExtent.height < vkSurfaceCapabilitiesKHR.minImageExtent.height) {
      swapChainExtent.height = vkSurfaceCapabilitiesKHR.minImageExtent.height;
    }
    if (swapChainExtent.width > vkSurfaceCapabilitiesKHR.maxImageExtent.width) {
      swapChainExtent.width = vkSurfaceCapabilitiesKHR.maxImageExtent.width;
    }
    if (swapChainExtent.height > vkSurfaceCapabilitiesKHR.maxImageExtent.height) {
      swapChainExtent.height = vkSurfaceCapabilitiesKHR.maxImageExtent.height;
    }
    return swapChainExtent;
  }

  // Most of the cases we define size of the swap chain images equal to current window's size
  return vkSurfaceCapabilitiesKHR.currentExtent;
}

VkImageUsageFlags VulkanHelper::get_swap_chain_usage_flags(const rhi::RHIContext& context, const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR) {
  // Color attachment flag must always be supported. We can define other usage flags but we always need to check if they are supported.
  if (vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
    return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  }

  /// TODO(naetherm): Implement me
  /*
  // Construct the log message
  std::stringstream message;
  message << "VK_IMAGE_USAGE_TRANSFER_DST image usage is not supported by the swap chain: Supported swap chain image usages include:\n";
  message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)				? "  VK_IMAGE_USAGE_TRANSFER_SRC\n" : "");
  message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)				? "  VK_IMAGE_USAGE_TRANSFER_DST\n" : "");
  message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT)						? "  VK_IMAGE_USAGE_SAMPLED\n" : "");
  message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT)						? "  VK_IMAGE_USAGE_STORAGE\n" : "");
  message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)			? "  VK_IMAGE_USAGE_COLOR_ATTACHMENT\n" : "");
  message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)	? "  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT\n" : "");
  message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)		? "  VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT\n" : "");
  message << ((vkSurfaceCapabilitiesKHR.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)			? "  VK_IMAGE_USAGE_INPUT_ATTACHMENT" : "");

  // Print log message
  BE_LOG(Critical, message.str().c_str())
*/
  // Error!
  return static_cast<VkImageUsageFlags>(-1);
}

VkSurfaceTransformFlagBitsKHR VulkanHelper::get_swap_chain_transform(const VkSurfaceCapabilitiesKHR& vkSurfaceCapabilitiesKHR) {
  // - Sometimes images must be transformed before they are presented (i.e. due to device's orientation being other than default orientation)
  // - If the specified transform is other than current transform, presentation engine will transform image during presentation operation; this operation may hit performance on some platforms
  // - Here we don't want any transformations to occur so if the identity transform is supported use it otherwise just use the same transform as current transform
  return (vkSurfaceCapabilitiesKHR.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR : vkSurfaceCapabilitiesKHR.currentTransform;
}

VkPresentModeKHR VulkanHelper::get_swap_chain_present_mode(const rhi::RHIContext& context, VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurfaceKHR) {
  core::uint32 presentModeCount = 0;
  if ((vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSurfaceKHR, &presentModeCount, nullptr) != VK_SUCCESS) || (0 == presentModeCount)) {
    BE_LOG(Critical, "Failed to get physical Vulkan device surface present modes")
    return VK_PRESENT_MODE_MAX_ENUM_KHR;
  }

  core::Vector<VkPresentModeKHR> presentModes(presentModeCount);
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSurfaceKHR, &presentModeCount, presentModes.data()) != VK_SUCCESS) {
    BE_LOG(Critical, "Failed to get physical Vulkan device surface present modes")
    return VK_PRESENT_MODE_MAX_ENUM_KHR;
  }

  // - FIFO present mode is always available
  // - MAILBOX is the lowest latency V-Sync enabled mode (something like triple-buffering) so use it if available
  for (const VkPresentModeKHR& presentMode : presentModes) {
    if (VK_PRESENT_MODE_MAILBOX_KHR == presentMode) {
      return presentMode;
    }
  }
  for (const VkPresentModeKHR& presentMode : presentModes) {
    if (VK_PRESENT_MODE_FIFO_KHR == presentMode) {
      return presentMode;
    }
  }

  // Error!
  BE_LOG(Critical, "FIFO present mode is not supported by the Vulkan swap chain")
  return VK_PRESENT_MODE_MAX_ENUM_KHR;
}

VkRenderPass VulkanHelper::create_render_pass(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, VkFormat colorVkFormat, VkFormat depthVkFormat, VkSampleCountFlagBits vkSampleCountFlagBits) {
  const bool hasDepthStencilAttachment = (VK_FORMAT_UNDEFINED != depthVkFormat);

   // Render pass configuration
   const core::Array<VkAttachmentDescription, 2> vkAttachmentDescriptions = {{
    {
      0,									// flags (VkAttachmentDescriptionFlags)
      colorVkFormat,						// format (VkFormat)
      vkSampleCountFlagBits,				// samples (VkSampleCountFlagBits)
      VK_ATTACHMENT_LOAD_OP_CLEAR,		// loadOp (VkAttachmentLoadOp)
      VK_ATTACHMENT_STORE_OP_STORE,		// storeOp (VkAttachmentStoreOp)
      VK_ATTACHMENT_LOAD_OP_DONT_CARE,	// stencilLoadOp (VkAttachmentLoadOp)
      VK_ATTACHMENT_STORE_OP_DONT_CARE,	// stencilStoreOp (VkAttachmentStoreOp)
      VK_IMAGE_LAYOUT_UNDEFINED,			// initialLayout (VkImageLayout)
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR		// finalLayout (VkImageLayout)
    },
    {
      0,													// flags (VkAttachmentDescriptionFlags)
      depthVkFormat,										// format (VkFormat)
      vkSampleCountFlagBits,								// samples (VkSampleCountFlagBits)
      VK_ATTACHMENT_LOAD_OP_CLEAR,						// loadOp (VkAttachmentLoadOp)
      VK_ATTACHMENT_STORE_OP_STORE,						// storeOp (VkAttachmentStoreOp)
      VK_ATTACHMENT_LOAD_OP_DONT_CARE,					// stencilLoadOp (VkAttachmentLoadOp)
      VK_ATTACHMENT_STORE_OP_DONT_CARE,					// stencilStoreOp (VkAttachmentStoreOp)
      VK_IMAGE_LAYOUT_UNDEFINED,							// initialLayout (VkImageLayout)
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL	// finalLayout (VkImageLayout)
    }
  }};
  static constexpr VkAttachmentReference colorVkAttachmentReference = {
    0,											// attachment (core::uint32)
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL	// layout (VkImageLayout)
  };
  static constexpr VkAttachmentReference depthVkAttachmentReference = {
    1,													// attachment (core::uint32)
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL	// layout (VkImageLayout)
  };
  const VkSubpassDescription vkSubpassDescription =
    {
      0,																	// flags (VkSubpassDescriptionFlags)
      VK_PIPELINE_BIND_POINT_GRAPHICS,									// pipelineBindPoint (VkPipelineBindPoint)
      0,																	// inputAttachmentCount (core::uint32)
      nullptr,															// pInputAttachments (const VkAttachmentReference*)
      1,																	// colorAttachmentCount (core::uint32)
      &colorVkAttachmentReference,										// pColorAttachments (const VkAttachmentReference*)
      nullptr,															// pResolveAttachments (const VkAttachmentReference*)
      hasDepthStencilAttachment ? &depthVkAttachmentReference : nullptr,	// pDepthStencilAttachment (const VkAttachmentReference*)
      0,																	// preserveAttachmentCount (core::uint32)
      nullptr																// pPreserveAttachments (const core::uint32*)
    };
  static constexpr VkSubpassDependency vkSubpassDependency =
    {
      VK_SUBPASS_EXTERNAL,														// srcSubpass (core::uint32)
      0,																			// dstSubpass (core::uint32)
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// srcStageMask (VkPipelineStageFlags)
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// dstStageMask (VkPipelineStageFlags)
      0,																			// srcAccessMask (VkAccessFlags)
      VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,	// dstAccessMask (VkAccessFlags)
      0																			// dependencyFlags (VkDependencyFlags)
    };
  const VkRenderPassCreateInfo vkRenderPassCreateInfo =
    {
      VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,	// sType (VkStructureType)
      nullptr,									// pNext (const void*)
      0,											// flags (VkRenderPassCreateFlags)
      hasDepthStencilAttachment ? 2u : 1u,		// attachmentCount (core::uint32)
      vkAttachmentDescriptions.data(),			// pAttachments (const VkAttachmentDescription*)
      1,											// subpassCount (core::uint32)
      &vkSubpassDescription,						// pSubpasses (const VkSubpassDescription*)
      1,											// dependencyCount (core::uint32)
      &vkSubpassDependency						// pDependencies (const VkSubpassDependency*)
    }; 

  // Create render pass
  VkRenderPass vkRenderPass = VK_NULL_HANDLE;
  if (vkCreateRenderPass(vkDevice, &vkRenderPassCreateInfo, vkAllocationCallbacks, &vkRenderPass) != VK_SUCCESS)
  {
    BE_LOG(Critical, "Failed to create Vulkan render pass")
  }

  // Done
  return vkRenderPass;
}

VkSurfaceFormatKHR VulkanHelper::get_swap_chain_format(const rhi::RHIContext& context, VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurfaceKHR) {
  core::uint32 surfaceFormatCount = 0;
  if ((vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurfaceKHR, &surfaceFormatCount, nullptr) != VK_SUCCESS) || (0 == surfaceFormatCount)) {
    BE_LOG(Critical, "Failed to get physical Vulkan device surface formats")
    return { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_MAX_ENUM_KHR };
  }

  core::Vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurfaceKHR, &surfaceFormatCount, surfaceFormats.data()) != VK_SUCCESS) {
    BE_LOG(Critical, "Failed to get physical Vulkan device surface formats")
    return { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_MAX_ENUM_KHR };
  }

  // If the list contains only one entry with undefined format it means that there are no preferred surface formats and any can be chosen
  if ((surfaceFormats.size() == 1) && (VK_FORMAT_UNDEFINED == surfaceFormats[0].format)) {
    return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
  }

  // Check if list contains most widely used R8 G8 B8 A8 format with nonlinear color space
  // -> Not all implementations support RGBA8, some only support BGRA8 formats (e.g. xlib surface under Linux with RADV), so check for both
  for (const VkSurfaceFormatKHR& surfaceFormat : surfaceFormats) {
    if (VK_FORMAT_R8G8B8A8_UNORM == surfaceFormat.format || VK_FORMAT_B8G8R8A8_UNORM == surfaceFormat.format) {
      return surfaceFormat;
    }
  }

  // Return the first format from the list
  return surfaceFormats[0];
}

VkFormat VulkanHelper::find_supported_vk_format(VkPhysicalDevice vkPhysicalDevice, const core::Vector<VkFormat>& vkFormatCandidates, VkImageTiling vkImageTiling, VkFormatFeatureFlags vkFormatFeatureFlags) {
  for (VkFormat vkFormat : vkFormatCandidates) {
    VkFormatProperties vkFormatProperties;
    vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, vkFormat, &vkFormatProperties);
    if (VK_IMAGE_TILING_LINEAR == vkImageTiling && (vkFormatProperties.linearTilingFeatures & vkFormatFeatureFlags) == vkFormatFeatureFlags) {
      return vkFormat;
    } else if (VK_IMAGE_TILING_OPTIMAL == vkImageTiling && (vkFormatProperties.optimalTilingFeatures & vkFormatFeatureFlags) == vkFormatFeatureFlags) {
      return vkFormat;
    }
  }

  // Failed to find supported Vulkan depth format
  return VK_FORMAT_UNDEFINED;
}

VkResult VulkanHelper::create_vk_device(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkPhysicalDevice vkPhysicalDevice, const VkDeviceQueueCreateInfo& vkDeviceQueueCreateInfo, bool enableValidation, bool enableDebugMarker, bool hasMeshShaderSupport, VkDevice& vkDevice) {
  // See http://vulkan.gpuinfo.org/listfeatures.php to check out GPU hardware capabilities
  core::Vector<const char*> enabledExtensions;
  enabledExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  enabledExtensions.push_back(VK_KHR_MAINTENANCE1_EXTENSION_NAME);
  //{
  //  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  //  VK_KHR_MAINTENANCE1_EXTENSION_NAME		// We want to be able to specify a negative viewport height, this way we don't have to apply "<output position>.y = -<output position>.y" inside vertex shaders to compensate for the Vulkan coordinate system
  //};
  if (enableDebugMarker) {
    enabledExtensions.emplace_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
  }
  if (hasMeshShaderSupport) {
    enabledExtensions.emplace_back(VK_NV_MESH_SHADER_EXTENSION_NAME);
  }

  // This will only be used if meshShadingSupported=true (see below)
  VkPhysicalDeviceMeshShaderFeaturesNV vkPhysicalDeviceMeshShaderFeaturesNV = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV };
  vkPhysicalDeviceMeshShaderFeaturesNV.taskShader = true;
  vkPhysicalDeviceMeshShaderFeaturesNV.meshShader = true;
  static constexpr VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = {
    VK_FALSE,	// robustBufferAccess (VkBool32)
    VK_FALSE,	// fullDrawIndexUint32 (VkBool32)
    VK_TRUE,	// imageCubeArray (VkBool32)
    VK_FALSE,	// independentBlend (VkBool32)
    VK_TRUE,	// geometryShader (VkBool32)
    VK_TRUE,	// tessellationShader (VkBool32)
    VK_FALSE,	// sampleRateShading (VkBool32)
    VK_FALSE,	// dualSrcBlend (VkBool32)
    VK_FALSE,	// logicOp (VkBool32)
    VK_TRUE,	// multiDrawIndirect (VkBool32)
    VK_FALSE,	// drawIndirectFirstInstance (VkBool32)
    VK_TRUE,	// depthClamp (VkBool32)
    VK_FALSE,	// depthBiasClamp (VkBool32)
    VK_TRUE,	// fillModeNonSolid (VkBool32)
    VK_FALSE,	// depthBounds (VkBool32)
    VK_FALSE,	// wideLines (VkBool32)
    VK_FALSE,	// largePoints (VkBool32)
    VK_FALSE,	// alphaToOne (VkBool32)
    VK_FALSE,	// multiViewport (VkBool32)
    VK_TRUE,	// samplerAnisotropy (VkBool32)
    VK_FALSE,	// textureCompressionETC2 (VkBool32)
    VK_FALSE,	// textureCompressionASTC_LDR (VkBool32)
    VK_TRUE,	// textureCompressionBC (VkBool32)
    VK_TRUE,	// occlusionQueryPrecise (VkBool32)
    VK_TRUE,	// pipelineStatisticsQuery (VkBool32)
    VK_FALSE,	// vertexPipelineStoresAndAtomics (VkBool32)
    VK_FALSE,	// fragmentStoresAndAtomics (VkBool32)
    VK_FALSE,	// shaderTessellationAndGeometryPointSize (VkBool32)
    VK_FALSE,	// shaderImageGatherExtended (VkBool32)
    VK_FALSE,	// shaderStorageImageExtendedFormats (VkBool32)
    VK_FALSE,	// shaderStorageImageMultisample (VkBool32)
    VK_FALSE,	// shaderStorageImageReadWithoutFormat (VkBool32)
    VK_FALSE,	// shaderStorageImageWriteWithoutFormat (VkBool32)
    VK_FALSE,	// shaderUniformBufferArrayDynamicIndexing (VkBool32)
    VK_FALSE,	// shaderSampledImageArrayDynamicIndexing (VkBool32)
    VK_FALSE,	// shaderStorageBufferArrayDynamicIndexing (VkBool32)
    VK_FALSE,	// shaderStorageImageArrayDynamicIndexing (VkBool32)
    VK_FALSE,	// shaderClipDistance (VkBool32)
    VK_FALSE,	// shaderCullDistance (VkBool32)
    VK_FALSE,	// shaderFloat64 (VkBool32)
    VK_FALSE,	// shaderInt64 (VkBool32)
    VK_FALSE,	// shaderInt16 (VkBool32)
    VK_FALSE,	// shaderResourceResidency (VkBool32)
    VK_FALSE,	// shaderResourceMinLod (VkBool32)
    VK_FALSE,	// sparseBinding (VkBool32)
    VK_FALSE,	// sparseResidencyBuffer (VkBool32)
    VK_FALSE,	// sparseResidencyImage2D (VkBool32)
    VK_FALSE,	// sparseResidencyImage3D (VkBool32)
    VK_FALSE,	// sparseResidency2Samples (VkBool32)
    VK_FALSE,	// sparseResidency4Samples (VkBool32)
    VK_FALSE,	// sparseResidency8Samples (VkBool32)
    VK_FALSE,	// sparseResidency16Samples (VkBool32)
    VK_FALSE,	// sparseResidencyAliased (VkBool32)
    VK_FALSE,	// variableMultisampleRate (VkBool32)
    VK_FALSE	// inheritedQueries (VkBool32)
  };
  const VkDeviceCreateInfo vkDeviceCreateInfo = {
    VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,									// sType (VkStructureType)
    hasMeshShaderSupport ? &vkPhysicalDeviceMeshShaderFeaturesNV : nullptr,	// pNext (const void*)
    0,																		// flags (VkDeviceCreateFlags)
    1,																		// queueCreateInfoCount (core::uint32)
    &vkDeviceQueueCreateInfo,												// pQueueCreateInfos (const VkDeviceQueueCreateInfo*)
    enableValidation ? internal::NUMBER_OF_VALIDATION_LAYERS : 0,						// enabledLayerCount (core::uint32)
    enableValidation ? internal::VALIDATION_LAYER_NAMES : nullptr,					// ppEnabledLayerNames (const char* const*)
    static_cast<core::uint32>(enabledExtensions.size()),						// enabledExtensionCount (core::uint32)
    enabledExtensions.empty() ? nullptr : enabledExtensions.data(),			// ppEnabledExtensionNames (const char* const*)
    &vkPhysicalDeviceFeatures												// pEnabledFeatures (const VkPhysicalDeviceFeatures*)
  };
  const VkResult vkResult = vkCreateDevice(vkPhysicalDevice, &vkDeviceCreateInfo, vkAllocationCallbacks, &vkDevice);

  // Done
  return vkResult;
}

VkDevice VulkanHelper::create_vk_device(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkPhysicalDevice vkPhysicalDevice, bool enableValidation, bool enableDebugMarker, bool hasMeshShaderSupport, core::uint32& graphicsQueueFamilyIndex, core::uint32& presentQueueFamilyIndex) {
  VkDevice vkDevice = VK_NULL_HANDLE;

  // Get physical device queue family properties
  core::uint32 queueFamilyPropertyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertyCount, nullptr);
  if (queueFamilyPropertyCount > 0) {
    core::Vector<VkQueueFamilyProperties> vkQueueFamilyProperties;
    vkQueueFamilyProperties.resize(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertyCount, vkQueueFamilyProperties.data());

    // Find a queue that supports graphics operations
    core::uint32 graphicsQueueIndex = 0;
    for (; graphicsQueueIndex < queueFamilyPropertyCount; ++graphicsQueueIndex) {
      if (vkQueueFamilyProperties[graphicsQueueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        // Create logical Vulkan device instance
        static constexpr core::Array<float, 1> queuePriorities = { 0.0f };
        const VkDeviceQueueCreateInfo vkDeviceQueueCreateInfo = {
          VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,	// sType (VkStructureType)
          nullptr,									// pNext (const void*)
          0,											// flags (VkDeviceQueueCreateFlags)
          graphicsQueueIndex,							// queueFamilyIndex (core::uint32)
          1,											// queueCount (core::uint32)
          queuePriorities.data()						// pQueuePriorities (const float*)
        };
        VkResult vkResult = create_vk_device(context, vkAllocationCallbacks, vkPhysicalDevice, vkDeviceQueueCreateInfo, enableValidation, enableDebugMarker, hasMeshShaderSupport, vkDevice);
        if (VK_ERROR_LAYER_NOT_PRESENT == vkResult && enableValidation) {
          // Error! Since the show must go on, try creating a Vulkan device instance without validation enabled...
          BE_LOG(Warning, "Failed to create the Vulkan device instance with validation enabled, layer is not present")
          vkResult = create_vk_device(context, vkAllocationCallbacks, vkPhysicalDevice, vkDeviceQueueCreateInfo, false, enableDebugMarker, hasMeshShaderSupport, vkDevice);
        }
        // TODO(naetherm) Error handling: Evaluate "vkResult"?
        graphicsQueueFamilyIndex = graphicsQueueIndex;
        presentQueueFamilyIndex = graphicsQueueIndex;	// TODO(naetherm) Handle the case of the graphics queue doesn't support present

        // We're done, get us out of the loop
        graphicsQueueIndex = queueFamilyPropertyCount;
      }
    }
  } else { 
    // Error!
    BE_LOG(Critical, "Failed to get physical Vulkan device queue family properties")
  }

  // Done
  return vkDevice;
}

bool VulkanHelper::has_vk_format_stencil_component(VkFormat vkFormat) {
  return (VK_FORMAT_D32_SFLOAT_S8_UINT == vkFormat || VK_FORMAT_D24_UNORM_S8_UINT == vkFormat);
}

VkCommandPool VulkanHelper::create_vk_command_pool(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, core::uint32 graphicsQueueFamilyIndex) {
  VkCommandPool vkCommandPool = VK_NULL_HANDLE;

  // Create Vulkan command pool instance
  const VkCommandPoolCreateInfo vkCommandPoolCreateInfo = {
    VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,			// sType (VkStructureType)
    nullptr,											// pNext (const void*)
    VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,	// flags (VkCommandPoolCreateFlags)
    graphicsQueueFamilyIndex,							/// queueFamilyIndex (core::uint32)
  };
  const VkResult vkResult = vkCreateCommandPool(vkDevice, &vkCommandPoolCreateInfo, vkAllocationCallbacks, &vkCommandPool);
  if (VK_SUCCESS != vkResult) {
    // Error!
    BE_LOG(Critical, "Failed to create Vulkan command pool instance")
  }

  // Done
  return vkCommandPool;
}

VkCommandBuffer VulkanHelper::create_vk_command_buffer(const rhi::RHIContext& context, VkDevice vkDevice, VkCommandPool vkCommandPool) {
  VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;

  // Create Vulkan command buffer instance
  const VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,	// sType (VkStructureType)
    nullptr,										// pNext (const void*)
    vkCommandPool,									// commandPool (VkCommandPool)
    VK_COMMAND_BUFFER_LEVEL_PRIMARY,				// level (VkCommandBufferLevel)
    1												// commandBufferCount (core::uint32)
  };
  VkResult vkResult = vkAllocateCommandBuffers(vkDevice, &vkCommandBufferAllocateInfo, &vkCommandBuffer);
  if (VK_SUCCESS != vkResult) {
    // Error!
    BE_LOG(Critical, "Failed to create Vulkan command buffer instance")
  }

  // Done
  return vkCommandBuffer;
}

bool VulkanHelper::is_extension_available(const char* extensionName, const core::Vector<VkExtensionProperties>& vkExtensionPropertiesVector) {
  for (const VkExtensionProperties& vkExtensionProperties : vkExtensionPropertiesVector) {
    if (strcmp(vkExtensionProperties.extensionName, extensionName) == 0) {
      // The extension is available
      return true;
    }
  }

  // The extension isn't available
  return false;
}


void VulkanHelper::enumerate_all_physical_devices(VkInstance vkInstance, core::Vector<VkPhysicalDevice>& physicalDevices) {
  core::uint32 numPhysicalDevices = 0;
  VkResult result = vkEnumeratePhysicalDevices(vkInstance, &numPhysicalDevices, nullptr);

  if (result == VK_SUCCESS) {
    if (numPhysicalDevices > 0) {
      // We found some physical devices, now let's fill the vector
      physicalDevices.resize(numPhysicalDevices);
      result = vkEnumeratePhysicalDevices(vkInstance, &numPhysicalDevices, physicalDevices.data());

      if (result != VK_SUCCESS) {
        BE_LOG(Critical, "Failed listing all physical devices")
      }
    } else {
      BE_LOG(Critical, "There are no physical devices")
    }
  } else {
    BE_LOG(Critical, "Failed to get the number of physical devices")
  }
}

VkPhysicalDevice VulkanHelper::select_physical_device(const core::Vector<VkPhysicalDevice>& physicalDevices, bool validationEnabled, bool& enableDebugMarker, bool& hasMeshShaderSupport) {
  for (const VkPhysicalDevice& vkPhysicalDevice : physicalDevices) {
    // Get of device extensions
    core::uint32 propertyCount = 0;
    if ((vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &propertyCount, nullptr) != VK_SUCCESS) || (0 == propertyCount)) {
      // Reject physical Vulkan device
      continue;
    }
    core::Vector<VkExtensionProperties> vkExtensionPropertiesVector(propertyCount);
    if (vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &propertyCount, vkExtensionPropertiesVector.data()) != VK_SUCCESS) {
      // Reject physical Vulkan device
      continue;
    }

    { // Reject physical Vulkan devices basing on swap chain support
      // Check device extensions
      static constexpr core::Array<const char*, 2> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_MAINTENANCE1_EXTENSION_NAME	// We want to be able to specify a negative viewport height, this way we don't have to apply "<output position>.y = -<output position>.y" inside vertex shaders to compensate for the Vulkan coordinate system
      };
      bool rejectDevice = false;
      for (const char* deviceExtension : deviceExtensions) {
        if (!is_extension_available(deviceExtension, vkExtensionPropertiesVector)) {
          rejectDevice = true;
          break;
        }
      }
      if (rejectDevice) {
        // Reject physical Vulkan device
        continue;
      }
    }
    hasMeshShaderSupport = is_extension_available(VK_NV_MESH_SHADER_EXTENSION_NAME, vkExtensionPropertiesVector);

    { // Reject physical Vulkan devices basing on supported API version and some basic limits
      VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
      vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkPhysicalDeviceProperties);
      const core::uint32 majorVersion = VK_VERSION_MAJOR(vkPhysicalDeviceProperties.apiVersion);
      if ((majorVersion < 1) || (vkPhysicalDeviceProperties.limits.maxImageDimension2D < 4096)) {
        // Reject physical Vulkan device
        continue;
      }
    }

    // Reject physical Vulkan devices basing on supported queue family
    core::uint32 queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertyCount, nullptr);
    if (0 == queueFamilyPropertyCount) {
      // Reject physical Vulkan device
      continue;
    }
    core::Vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());
    for (core::uint32 i = 0; i < queueFamilyPropertyCount; ++i) {
      if ((queueFamilyProperties[i].queueCount > 0) && (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
        // Check whether the "VK_EXT_debug_marker"-extension is available
        // -> The "VK_EXT_debug_marker"-extension is only available when the application gets started by tools like RenderDoc ( https://renderdoc.org/ )
        // -> See "Offline debugging in Vulkan with VK_EXT_debug_marker and RenderDoc" - https://www.saschawillems.de/?page_id=2017
        if (enableDebugMarker) {
          // Check whether the "VK_EXT_debug_marker"-extension is available
          if (is_extension_available(VK_EXT_DEBUG_MARKER_EXTENSION_NAME, vkExtensionPropertiesVector)) {
            if (validationEnabled) {
              enableDebugMarker = false;
              BE_LOG(Warning, "Vulkan validation layers are enabled: If you want to use debug markers (\"VK_EXT_debug_marker\"-extension) please disable the validation layers")
            }
          } else {
            // Silently disable debug marker
            enableDebugMarker = false;
          }
        }

        // Select physical Vulkan device
        return vkPhysicalDevice;
      }
    }
  }

  // Error!
  BE_LOG(Critical, "Failed to select a physical Vulkan device")
  return VK_NULL_HANDLE;
}


void VulkanHelper::begin_vulkan_render_pass(const rhi::RHIRenderTarget& renderTarget, VkRenderPass vkRenderPass, VkFramebuffer vkFramebuffer, core::uint32 numberOfAttachments, const core::Array<VkClearValue, 9>& vkClearValues, VkCommandBuffer vkCommandBuffer) {
  // Get render target dimension
  core::uint32 width = 1;
  core::uint32 height = 1;
  renderTarget.get_width_and_height(width, height);

  // Begin Vulkan render pass
  const VkRenderPassBeginInfo vkRenderPassBeginInfo = {
    VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,	// sType (VkStructureType)
    nullptr,									// pNext (const void*)
    vkRenderPass,								// renderPass (VkRenderPass)
    vkFramebuffer,								// framebuffer (VkFramebuffer)
    { // renderArea (VkRect2D)
      { 0, 0 },								// offset (VkOffset2D)
      { width, height }						// extent (VkExtent2D)
    },
    numberOfAttachments,						// clearValueCount (core::uint32)
    vkClearValues.data()						// pClearValues (const VkClearValue*)
  };
  vkCmdBeginRenderPass(vkCommandBuffer, &vkRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}


//[-------------------------------------------------------]
//[ Command                                               ]
//[-------------------------------------------------------]
VkCommandBuffer VulkanHelper::begin_single_time_commands(const RHIDevice& vulkanRhi) {
  // Create and begin Vulkan command buffer
  VkCommandBuffer vkCommandBuffer = vulkanRhi.get_vulkan_context().create_vk_command_buffer();
  static constexpr VkCommandBufferBeginInfo vkCommandBufferBeginInfo = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	// sType (VkStructureType)
    nullptr,										// pNext (const void*)
    0,												// flags (VkCommandBufferUsageFlags)
    nullptr											// pInheritanceInfo (const VkCommandBufferInheritanceInfo*)
  };
  if (vkBeginCommandBuffer(vkCommandBuffer, &vkCommandBufferBeginInfo) == VK_SUCCESS) {
    // Done
    return vkCommandBuffer;
  } else {
    // Error!
    BE_LOG(Critical, "Failed to begin Vulkan command buffer instance")
    return VK_NULL_HANDLE;
  }
}

void VulkanHelper::end_single_time_commands(const RHIDevice& vulkanRhi, VkCommandBuffer vkCommandBuffer) {
  const VulkanContext& vulkanContext = vulkanRhi.get_vulkan_context();
  const VkQueue vkQueue = vulkanContext.get_graphics_vk_queue();

  // End Vulkan command buffer
  vkEndCommandBuffer(vkCommandBuffer);

  // Submit Vulkan command buffer
  const VkSubmitInfo vkSubmitInfo = {
    VK_STRUCTURE_TYPE_SUBMIT_INFO,	// sType (VkStructureType)
    nullptr,						// pNext (const void*)
    0,								// waitSemaphoreCount (core::uint32)
    nullptr,						// pWaitSemaphores (const VkSemaphore*)
    nullptr,						// pWaitDstStageMask (const VkPipelineStageFlags*)
    1,								// commandBufferCount (core::uint32)
    &vkCommandBuffer,				// pCommandBuffers (const VkCommandBuffer*)
    0,								// signalSemaphoreCount (core::uint32)
    nullptr							// pSignalSemaphores (const VkSemaphore*)
  };
  if (vkQueueSubmit(vkQueue, 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
    // Error!
    BE_LOG(Critical, "Vulkan queue submit failed")
    return;
  }
  if (vkQueueWaitIdle(vkQueue) != VK_SUCCESS) {
    // Error!
    BE_LOG(Critical, "Vulkan Queue wait idle failed")
    return;
  }

  // Destroy Vulkan command buffer
  vulkanContext.destroy_vk_command_buffer(vkCommandBuffer);
}

//[-------------------------------------------------------]
//[ Transition                                            ]
//[-------------------------------------------------------]
void VulkanHelper::transition_vk_image_layout(const RHIDevice& vulkanRhi, VkImage vkImage, VkImageAspectFlags vkImageAspectFlags, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout) {
  // Create and begin Vulkan command buffer
  VkCommandBuffer vkCommandBuffer = begin_single_time_commands(vulkanRhi);

  // Vulkan image memory barrier
  transition_vk_image_layout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, 1, 1, oldVkImageLayout, newVkImageLayout);

  // End and destroy Vulkan command buffer
  end_single_time_commands(vulkanRhi, vkCommandBuffer);
}

void VulkanHelper::transition_vk_image_layout(const RHIDevice& vulkanRhi, VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageAspectFlags vkImageAspectFlags, core::uint32 levelCount, core::uint32 layerCount, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout) {
  VkImageMemoryBarrier vkImageMemoryBarrier = {
    VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,	// sType (VkStructureType)
    nullptr,								// pNext (const void*)
    0,										// srcAccessMask (VkAccessFlags)
    0,										// dstAccessMask (VkAccessFlags)
    oldVkImageLayout,						// oldLayout (VkImageLayout)
    newVkImageLayout,						// newLayout (VkImageLayout)
    VK_QUEUE_FAMILY_IGNORED,				// srcQueueFamilyIndex (core::uint32)
    VK_QUEUE_FAMILY_IGNORED,				// dstQueueFamilyIndex (core::uint32)
    vkImage,								// image (VkImage)
    { // subresourceRange (VkImageSubresourceRange)
      vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
      0,					// baseMipLevel (core::uint32)
      levelCount,			// levelCount (core::uint32)
      0,					// baseArrayLayer (core::uint32)
      layerCount			// layerCount (core::uint32)
    }
  };

  // "srcAccessMask" and "dstAccessMask" configuration
  VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
  VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
  if (VK_IMAGE_LAYOUT_PREINITIALIZED == oldVkImageLayout && VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == newVkImageLayout) {
    vkImageMemoryBarrier.srcAccessMask = 0;
    vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }
  else if (VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == oldVkImageLayout && VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL == newVkImageLayout) {
    vkImageMemoryBarrier.srcAccessMask = 0;
    vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }
  else if (VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == oldVkImageLayout && VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL == newVkImageLayout) {
    vkImageMemoryBarrier.srcAccessMask = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    vkImageMemoryBarrier.dstAccessMask = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    srcStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dstStageMask = VK_PIPELINE_STAGE_HOST_BIT;
  }
  else if (VK_IMAGE_LAYOUT_UNDEFINED == oldVkImageLayout && VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL == newVkImageLayout) {
    vkImageMemoryBarrier.srcAccessMask = 0;
    vkImageMemoryBarrier.dstAccessMask = (VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
  } else {
    BE_LOG(Critical, "Unsupported Vulkan image layout transition")
  }

  // Create Vulkan pipeline barrier command
  vkCmdPipelineBarrier(vkCommandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &vkImageMemoryBarrier);
}

void VulkanHelper::transition_vk_image_layout(const RHIDevice& vulkanRhi, VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout, VkImageSubresourceRange vkImageSubresourceRange, VkPipelineStageFlags sourceVkPipelineStageFlags, VkPipelineStageFlags destinationVkPipelineStageFlags) {
  // Basing on https://github.com/SaschaWillems/Vulkan/tree/master

  // Create an image barrier object
  VkImageMemoryBarrier vkImageMemoryBarrier = {
    VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,	// sType (VkStructureType)
    nullptr,								// pNext (const void*)
    0,										// srcAccessMask (VkAccessFlags)
    0,										// dstAccessMask (VkAccessFlags)
    oldVkImageLayout,						// oldLayout (VkImageLayout)
    newVkImageLayout,						// newLayout (VkImageLayout)
    VK_QUEUE_FAMILY_IGNORED,				// srcQueueFamilyIndex (core::uint32)
    VK_QUEUE_FAMILY_IGNORED,				// dstQueueFamilyIndex (core::uint32)
    vkImage,								// image (VkImage)
    vkImageSubresourceRange					// subresourceRange (VkImageSubresourceRange)
  };

  // Source layouts (old)
  // -> Source access mask controls actions that have to be finished on the old layout before it will be transitioned to the new layout
  switch (oldVkImageLayout) {
    case VK_IMAGE_LAYOUT_UNDEFINED:
      // Image layout is undefined (or does not matter)
      // Only valid as initial layout
      // No flags required, listed only for completeness
      vkImageMemoryBarrier.srcAccessMask = 0;
      break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED:
      // Image is preinitialized
      // Only valid as initial layout for linear images, preserves memory contents
      // Make sure host writes have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      // Image is a color attachment
      // Make sure any writes to the color buffer have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      // Image is a depth/stencil attachment
      // Make sure any writes to the depth/stencil buffer have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      // Image is a transfer source 
      // Make sure any reads from the image have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      // Image is a transfer destination
      // Make sure any writes to the image have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      // Image is read by a shader
      // Make sure any shader reads from the image have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_GENERAL:
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
    case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
      // case VK_IMAGE_LAYOUT_BEGIN_RANGE:	not possible
      // case VK_IMAGE_LAYOUT_END_RANGE:		not possible
    case VK_IMAGE_LAYOUT_MAX_ENUM:
    default:
      // Other source layouts aren't handled (yet)
      BE_LOG(Critical, "Unsupported Vulkan image old layout transition")
      break;
  }

  // Target layouts (new)
  // -> Destination access mask controls the dependency for the new image layout
  switch (newVkImageLayout) {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      // Image will be used as a transfer destination
      // Make sure any writes to the image have been finished
      vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      // Image will be used as a transfer source
      // Make sure any reads from the image have been finished
      vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      // Image will be used as a color attachment
      // Make sure any writes to the color buffer have been finished
      vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      // Image layout will be used as a depth/stencil attachment
      // Make sure any writes to depth/stencil buffer have been finished
      vkImageMemoryBarrier.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      // Image will be read in a shader (sampler, input attachment)
      // Make sure any writes to the image have been finished
      if (vkImageMemoryBarrier.srcAccessMask == 0)
      {
        vkImageMemoryBarrier.srcAccessMask = (VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
      }
      vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_UNDEFINED:
    case VK_IMAGE_LAYOUT_GENERAL:
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
    case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
      // case VK_IMAGE_LAYOUT_BEGIN_RANGE:	not possible
      // case VK_IMAGE_LAYOUT_END_RANGE:		not possible
    case VK_IMAGE_LAYOUT_MAX_ENUM:
    default:
      // Other source layouts aren't handled (yet)
      BE_LOG(Critical, "Unsupported Vulkan image new layout transition")
      break;
  }

  // Put barrier inside setup command buffer
  // -> "Table 4. Supported access types": https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#synchronization-access-types-supported
  vkCmdPipelineBarrier(vkCommandBuffer, sourceVkPipelineStageFlags, destinationVkPipelineStageFlags, 0, 0, nullptr, 0, nullptr, 1, &vkImageMemoryBarrier);
}

//[-------------------------------------------------------]
//[ Buffer                                                ]
//[-------------------------------------------------------]
// TODO(naetherm) Trivial implementation to have something to start with. Need to use more clever memory management and stating buffers later on.
void VulkanHelper::create_and_allocate_vk_buffer(const RHIDevice& vulkanRhi, VkBufferUsageFlagBits vkBufferUsageFlagBits, VkMemoryPropertyFlags vkMemoryPropertyFlags, VkDeviceSize numberOfBytes, const void* data, VkBuffer& vkBuffer, VkDeviceMemory& vkDeviceMemory) {
  const VulkanContext& vulkanContext = vulkanRhi.get_vulkan_context();
  const VkDevice vkDevice = vulkanContext.get_vk_device();

  // Create the Vulkan buffer
  const VkBufferCreateInfo vkBufferCreateInfo = {
    VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,					// sType (VkStructureType)
    nullptr,												// pNext (const void*)
    0,														// flags (VkBufferCreateFlags)
    numberOfBytes,											// size (VkDeviceSize)
    static_cast<VkBufferUsageFlags>(vkBufferUsageFlagBits),	// usage (VkBufferUsageFlags)
    VK_SHARING_MODE_EXCLUSIVE,								// sharingMode (VkSharingMode)
    0,														// queueFamilyIndexCount (core::uint32)
    nullptr													// pQueueFamilyIndices (const core::uint32*)
  };
  if (vkCreateBuffer(vkDevice, &vkBufferCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &vkBuffer) != VK_SUCCESS) {
    BE_LOG(Critical, "Failed to create the Vulkan buffer")
  }

  // Allocate memory for the Vulkan buffer
  VkMemoryRequirements vkMemoryRequirements = {};
  vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &vkMemoryRequirements);
  const VkMemoryAllocateInfo vkMemoryAllocateInfo = {
    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,															// sType (VkStructureType)
    nullptr,																						// pNext (const void*)
    vkMemoryRequirements.size,																		// allocationSize (VkDeviceSize)
    vulkanContext.find_memory_type_index(vkMemoryRequirements.memoryTypeBits, vkMemoryPropertyFlags)	// memoryTypeIndex (core::uint32)
  };
  if (vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, vulkanRhi.get_vk_allocation_callbacks(), &vkDeviceMemory) != VK_SUCCESS) {
    BE_LOG(Critical, "Failed to allocate the Vulkan buffer memory")
  }

  // Bind and fill memory
  vkBindBufferMemory(vkDevice, vkBuffer, vkDeviceMemory, 0);
  if (nullptr != data) {
    void* mappedData = nullptr;
    if (vkMapMemory(vkDevice, vkDeviceMemory, 0, vkBufferCreateInfo.size, 0, &mappedData) == VK_SUCCESS) {
      memcpy(mappedData, data, static_cast<size_t>(vkBufferCreateInfo.size));
      vkUnmapMemory(vkDevice, vkDeviceMemory);
    } else {
      BE_LOG(Critical, "Failed to map the Vulkan memory")
    }
  }
}

void VulkanHelper::destroy_and_free_vk_buffer(const RHIDevice& vulkanRhi, VkBuffer& vkBuffer, VkDeviceMemory& vkDeviceMemory) {
  if (VK_NULL_HANDLE != vkBuffer) {
    const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
    vkDestroyBuffer(vkDevice, vkBuffer, vulkanRhi.get_vk_allocation_callbacks());
    if (VK_NULL_HANDLE != vkDeviceMemory) {
      vkFreeMemory(vkDevice, vkDeviceMemory, vulkanRhi.get_vk_allocation_callbacks());
    }
  }
}

//[-------------------------------------------------------]
//[ Image                                                 ]
//[-------------------------------------------------------]
VkImageLayout VulkanHelper::get_vk_image_layout_by_texture_flags(core::uint32 textureFlags) {
  if (textureFlags & rhi::TextureFlag::RENDER_TARGET) {
    return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  }
  else if (textureFlags & rhi::TextureFlag::UNORDERED_ACCESS) {
    return VK_IMAGE_LAYOUT_GENERAL;
  }
  return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

// TODO(naetherm) Trivial implementation to have something to start with. Need to use more clever memory management and stating buffers later on.
VkFormat VulkanHelper::create_and_fill_vk_image(const RHIDevice& vulkanRhi, VkImageType vkImageType, VkImageViewType vkImageViewType, const VkExtent3D& vkExtent3D, rhi::TextureFormat::Enum textureFormat, const void* data, core::uint32 textureFlags, core::uint8 numberOfMultisamples, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory, VkImageView& vkImageView) {
  // Calculate the number of mipmaps
  const bool dataContainsMipmaps = (textureFlags & rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
  const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & rhi::TextureFlag::GENERATE_MIPMAPS));
  core::uint32 numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? rhi::RHITexture::get_number_of_mipmaps(vkExtent3D.width, vkExtent3D.height) : 1;

  // Get Vulkan image usage flags
  BE_ASSERT((textureFlags & rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "Vulkan render target textures can't be filled using provided data")
  const bool isDepthTextureFormat = rhi::TextureFormat::isDepth(textureFormat);
  VkImageUsageFlags vkImageUsageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  if (textureFlags & rhi::TextureFlag::SHADER_RESOURCE) {
    vkImageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;
  }
  if (textureFlags & rhi::TextureFlag::UNORDERED_ACCESS) {
    vkImageUsageFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
  }
  if (textureFlags & rhi::TextureFlag::RENDER_TARGET) {
    if (isDepthTextureFormat) {
      vkImageUsageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    } else {
      vkImageUsageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
  }
  if (generateMipmaps) {
    vkImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  }

  // Get Vulkan format
  const VkFormat vkFormat   = Mapping::get_vulkan_format(textureFormat);
  const bool     layered    = (VK_IMAGE_VIEW_TYPE_1D_ARRAY == vkImageViewType || VK_IMAGE_VIEW_TYPE_2D_ARRAY == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE_ARRAY == vkImageViewType);
  const core::uint32 layerCount = layered ? vkExtent3D.depth : 1;
  const core::uint32 depth	  = layered ? 1 : vkExtent3D.depth;
  const VkSampleCountFlagBits vkSampleCountFlagBits = Mapping::get_vulkan_sample_count_flag_bits(numberOfMultisamples);
  VkImageAspectFlags vkImageAspectFlags = isDepthTextureFormat ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
  if (VulkanHelper::has_vk_format_stencil_component(vkFormat)) {
    vkImageAspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
  }

  // Calculate the number of bytes
  core::uint32 numberOfBytes = 0;
  if (dataContainsMipmaps) {
    core::uint32 currentWidth  = vkExtent3D.width;
    core::uint32 currentHeight = vkExtent3D.height;
    core::uint32 currentDepth  = depth;
    for (core::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
      numberOfBytes += rhi::TextureFormat::getNumberOfBytesPerSlice(static_cast<rhi::TextureFormat::Enum>(textureFormat), currentWidth, currentHeight) * currentDepth;
      currentWidth = rhi::RHITexture::get_half_size(currentWidth);
      currentHeight = rhi::RHITexture::get_half_size(currentHeight);
      currentDepth = rhi::RHITexture::get_half_size(currentDepth);
    }
    numberOfBytes *= vkExtent3D.depth;
  } else {
    numberOfBytes = rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, vkExtent3D.width, vkExtent3D.height) * vkExtent3D.depth;
  }

  { // Create and fill Vulkan image
    const VkImageCreateFlags vkImageCreateFlags = (VK_IMAGE_VIEW_TYPE_CUBE == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE_ARRAY == vkImageViewType) ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u;
    create_and_allocate_vk_image(vulkanRhi, vkImageCreateFlags, vkImageType,
                                 VkExtent3D{vkExtent3D.width, vkExtent3D.height, depth}, numberOfMipmaps, layerCount,
                                 vkFormat, vkSampleCountFlagBits, VK_IMAGE_TILING_OPTIMAL, vkImageUsageFlags,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkImage, vkDeviceMemory);
  }

  // Create the Vulkan image view
  if ((textureFlags & rhi::TextureFlag::SHADER_RESOURCE) != 0 || (textureFlags & rhi::TextureFlag::RENDER_TARGET) != 0 || (textureFlags & rhi::TextureFlag::UNORDERED_ACCESS) != 0) {
    create_vk_image_view(vulkanRhi, vkImage, vkImageViewType, numberOfMipmaps, layerCount, vkFormat, vkImageAspectFlags, vkImageView);
  }

  // Upload all mipmaps
  if (nullptr != data) {
    // Create Vulkan staging buffer
    VkBuffer stagingVkBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingVkDeviceMemory = VK_NULL_HANDLE;
    create_and_allocate_vk_buffer(vulkanRhi, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                  numberOfBytes, data, stagingVkBuffer, stagingVkDeviceMemory);

    { // Upload all mipmaps
      const core::uint32 numberOfUploadedMipmaps = generateMipmaps ? 1 : numberOfMipmaps;

      // Create and begin Vulkan command buffer
      VkCommandBuffer vkCommandBuffer = begin_single_time_commands(vulkanRhi);
      transition_vk_image_layout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, numberOfUploadedMipmaps,
                                 layerCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

      // Upload all mipmaps
      core::uint32 bufferOffset  = 0;
      core::uint32 currentWidth  = vkExtent3D.width;
      core::uint32 currentHeight = vkExtent3D.height;
      core::uint32 currentDepth  = depth;

      // Allocate list of VkBufferImageCopy and setup VkBufferImageCopy data for each mipmap level
      core::Vector<VkBufferImageCopy> vkBufferImageCopyList;
      vkBufferImageCopyList.reserve(numberOfUploadedMipmaps);
      for (core::uint32 mipmap = 0; mipmap < numberOfUploadedMipmaps; ++mipmap) {
        vkBufferImageCopyList.push_back({
          bufferOffset,									// bufferOffset (VkDeviceSize)
          0,												// bufferRowLength (core::uint32)
          0,												// bufferImageHeight (core::uint32)
          { // imageSubresource (VkImageSubresourceLayers)
            vkImageAspectFlags,							// aspectMask (VkImageAspectFlags)
            mipmap,										// mipLevel (core::uint32)
            0,											// baseArrayLayer (core::uint32)
            layerCount									// layerCount (core::uint32)
          },
          { 0, 0, 0 },									// imageOffset (VkOffset3D)
          { currentWidth, currentHeight, currentDepth }	// imageExtent (VkExtent3D)
        });

        // Move on to the next mipmap
        bufferOffset += rhi::TextureFormat::getNumberOfBytesPerSlice(static_cast<rhi::TextureFormat::Enum>(textureFormat), currentWidth, currentHeight) * currentDepth;
        currentWidth = rhi::RHITexture::get_half_size(currentWidth);
        currentHeight = rhi::RHITexture::get_half_size(currentHeight);
        currentDepth = rhi::RHITexture::get_half_size(currentDepth);
      }

      // Copy Vulkan buffer to Vulkan image
      vkCmdCopyBufferToImage(vkCommandBuffer, stagingVkBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<core::uint32>(vkBufferImageCopyList.size()), vkBufferImageCopyList.data());

      // End and destroy Vulkan command buffer
      if (generateMipmaps) {
        const VkImageSubresourceRange vkImageSubresourceRange = {
          vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
          0,					// baseMipLevel (core::uint32)
          1,					// levelCount (core::uint32)
          0,					// baseArrayLayer (core::uint32)
          layerCount			// layerCount (core::uint32)
        };
        transition_vk_image_layout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImageSubresourceRange,
                                   VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
      } else {
        transition_vk_image_layout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, numberOfUploadedMipmaps,
                                   layerCount, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
      }
      end_single_time_commands(vulkanRhi, vkCommandBuffer);
    }

    // Destroy Vulkan staging buffer
    destroy_and_free_vk_buffer(vulkanRhi, stagingVkBuffer, stagingVkDeviceMemory);

    // Generate a complete texture mip-chain at runtime from a base image using image blits and proper image barriers
    // -> Basing on https://github.com/SaschaWillems/Vulkan/tree/master/examples/texturemipmapgen and "Mipmap generation : Transfers, transition layout" by Antoine MORRIER published January 12, 2017 at http://cpp-rendering.io/mipmap-generation/
    // -> We copy down the whole mip chain doing a blit from mip-1 to mip. An alternative way would be to always blit from the first mip level and sample that one down.
    // TODO(naetherm) Some GPUs also offer "asynchronous transfer queues" (check for queue families with only the "VK_QUEUE_TRANSFER_BIT" set) that may be used to speed up such operations
    if (generateMipmaps) {
#ifdef RHI_DEBUG
      {
        // Get device properties for the requested Vulkan texture format
        VkFormatProperties vkFormatProperties;
        vkGetPhysicalDeviceFormatProperties(vulkanRhi.get_vulkan_context().getVkPhysicalDevice(), vkFormat, &vkFormatProperties);
        
        // Mip-chain generation requires support for blit source and destination
        RHI_ASSERT(vulkanRhi.getContext(), vkFormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT, "Invalid Vulkan optimal tiling features")
        RHI_ASSERT(vulkanRhi.getContext(), vkFormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT, "Invalid Vulkan optimal tiling features")
      }
#endif

      // Create and begin Vulkan command buffer
      VkCommandBuffer vkCommandBuffer = begin_single_time_commands(vulkanRhi);

      // Copy down mips from n-1 to n
      for (core::uint32 i = 1; i < numberOfMipmaps; ++i) {
        const VkImageBlit VkImageBlit = {
          { // srcSubresource (VkImageSubresourceLayers)
            vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
            i - 1,				// mipLevel (core::uint32)
            0,					// baseArrayLayer (core::uint32)
            layerCount			// layerCount (core::uint32)
          },
          { // srcOffsets[2] (VkOffset3D)
            { 0, 0, 0 },
            { std::max(int32_t(vkExtent3D.width >> (i - 1)), 1), std::max(int32_t(vkExtent3D.height >> (i - 1)), 1), 1 }
          },
          { // dstSubresource (VkImageSubresourceLayers)
            vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
            i,					// mipLevel (core::uint32)
            0,					// baseArrayLayer (core::uint32)
            layerCount			// layerCount (core::uint32)
          },
          { // dstOffsets[2] (VkOffset3D)
            { 0, 0, 0 },
            { std::max(int32_t(vkExtent3D.width >> i), 1), std::max(int32_t(vkExtent3D.height >> i), 1), 1 }
          }
        };
        const VkImageSubresourceRange vkImageSubresourceRange = {
          vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
          i,					// baseMipLevel (core::uint32)
          1,					// levelCount (core::uint32)
          0,					// baseArrayLayer (core::uint32)
          layerCount			// layerCount (core::uint32)
        };

        // Transition current mip level to transfer destination
        transition_vk_image_layout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vkImageSubresourceRange,
                                   VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        // Blit from previous level
        vkCmdBlitImage(vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &VkImageBlit, VK_FILTER_LINEAR);

        // Transition current mip level to transfer source for read in next iteration
        transition_vk_image_layout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImageSubresourceRange,
                                   VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
      }

      { // After the loop, all mip layers are in "VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL"-layout, so transition all to "VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL"-layout
        const VkImageSubresourceRange vkImageSubresourceRange = {
          vkImageAspectFlags,		// aspectMask (VkImageAspectFlags)
          0,						// baseMipLevel (core::uint32)
          numberOfMipmaps,		// levelCount (core::uint32)
          0,						// baseArrayLayer (core::uint32)
          layerCount				// layerCount (core::uint32)
        };
        transition_vk_image_layout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, vkImageSubresourceRange,
                                   VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
      }

      // End and destroy Vulkan command buffer
      end_single_time_commands(vulkanRhi, vkCommandBuffer);
    }
  }

  // Done
  return vkFormat;
}

void VulkanHelper::create_and_allocate_vk_image(const RHIDevice& vulkanRhi, VkImageCreateFlags vkImageCreateFlags, VkImageType vkImageType, const VkExtent3D& vkExtent3D, core::uint32 mipLevels, core::uint32 arrayLayers, VkFormat vkFormat, VkSampleCountFlagBits vkSampleCountFlagBits, VkImageTiling vkImageTiling, VkImageUsageFlags vkImageUsageFlags, VkMemoryPropertyFlags vkMemoryPropertyFlags, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory) {
  const VulkanContext& vulkanContext = vulkanRhi.get_vulkan_context();
  const VkDevice vkDevice = vulkanContext.get_vk_device();

  { // Create Vulkan image
    const VkImageCreateInfo vkImageCreateInfo = {
      VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,	// sType (VkStructureType)
      nullptr,								// pNext (const void*)
      vkImageCreateFlags,						// flags (VkImageCreateFlags)
      vkImageType,							// imageType (VkImageType)
      vkFormat,								// format (VkFormat)
      vkExtent3D,								// extent (VkExtent3D)
      mipLevels,								// mipLevels (core::uint32)
      arrayLayers,							// arrayLayers (core::uint32)
      vkSampleCountFlagBits,					// samples (VkSampleCountFlagBits)
      vkImageTiling,							// tiling (VkImageTiling)
      vkImageUsageFlags,						// usage (VkImageUsageFlags)
      VK_SHARING_MODE_EXCLUSIVE,				// sharingMode (VkSharingMode)
      0,										// queueFamilyIndexCount (core::uint32)
      nullptr,								// pQueueFamilyIndices (const core::uint32*)
      VK_IMAGE_LAYOUT_PREINITIALIZED			// initialLayout (VkImageLayout)
    };
    if (vkCreateImage(vkDevice, &vkImageCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &vkImage) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to create the Vulkan image")
    }
  }

  { // Allocate Vulkan memory
    VkMemoryRequirements vkMemoryRequirements = {};
    vkGetImageMemoryRequirements(vkDevice, vkImage, &vkMemoryRequirements);
    const VkMemoryAllocateInfo vkMemoryAllocateInfo = {
      VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,															// sType (VkStructureType)
      nullptr,																						// pNext (const void*)
      vkMemoryRequirements.size,																		// allocationSize (VkDeviceSize)
      vulkanContext.find_memory_type_index(vkMemoryRequirements.memoryTypeBits, vkMemoryPropertyFlags)	// memoryTypeIndex (core::uint32)
    };
    if (vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, vulkanRhi.get_vk_allocation_callbacks(), &vkDeviceMemory) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to allocate the Vulkan memory")
    }
    if (vkBindImageMemory(vkDevice, vkImage, vkDeviceMemory, 0) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to bind the Vulkan image memory")
    }
  }
}

void VulkanHelper::destroy_and_free_vk_image(const RHIDevice& vulkanRhi, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory) {
  if (VK_NULL_HANDLE != vkImage) {
    const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
    vkDestroyImage(vkDevice, vkImage, vulkanRhi.get_vk_allocation_callbacks());
    vkImage = VK_NULL_HANDLE;
    if (VK_NULL_HANDLE != vkDeviceMemory) {
      vkFreeMemory(vkDevice, vkDeviceMemory, vulkanRhi.get_vk_allocation_callbacks());
      vkDeviceMemory = VK_NULL_HANDLE;
    }
  }
}

void VulkanHelper::destroy_and_free_vk_image(const RHIDevice& vulkanRhi, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory, VkImageView& vkImageView) {
  if (VK_NULL_HANDLE != vkImageView) {
    vkDestroyImageView(vulkanRhi.get_vulkan_context().get_vk_device(), vkImageView, vulkanRhi.get_vk_allocation_callbacks());
    vkImageView = VK_NULL_HANDLE;
  }
  destroy_and_free_vk_image(vulkanRhi, vkImage, vkDeviceMemory);
}

void VulkanHelper::create_vk_image_view(const RHIDevice& vulkanRhi, VkImage vkImage, VkImageViewType vkImageViewType, core::uint32 levelCount, core::uint32 layerCount, VkFormat vkFormat, VkImageAspectFlags vkImageAspectFlags, VkImageView& vkImageView) {
  const VkImageViewCreateInfo vkImageViewCreateInfo = {
    VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	// sType (VkStructureType)
    nullptr,									// pNext (const void*)
    0,											// flags (VkImageViewCreateFlags)
    vkImage,									// image (VkImage)
    vkImageViewType,							// viewType (VkImageViewType)
    vkFormat,									// format (VkFormat)
    { // components (VkComponentMapping)
      VK_COMPONENT_SWIZZLE_IDENTITY,			// r (VkComponentSwizzle)
      VK_COMPONENT_SWIZZLE_IDENTITY,			// g (VkComponentSwizzle)
      VK_COMPONENT_SWIZZLE_IDENTITY,			// b (VkComponentSwizzle)
      VK_COMPONENT_SWIZZLE_IDENTITY			// a (VkComponentSwizzle)
    },
    { // subresourceRange (VkImageSubresourceRange)
      vkImageAspectFlags,						// aspectMask (VkImageAspectFlags)
      0,										// baseMipLevel (core::uint32)
      levelCount,								// levelCount (core::uint32)
      0,										// baseArrayLayer (core::uint32)
      layerCount								// layerCount (core::uint32)
    }
  };
  if (vkCreateImageView(vulkanRhi.get_vulkan_context().get_vk_device(), &vkImageViewCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &vkImageView) != VK_SUCCESS) {
    BE_LOG(Critical, "Failed to create Vulkan image view")
  }
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void VulkanHelper::set_debug_object_name(VkDevice vkDevice, VkDebugReportObjectTypeEXT vkDebugReportObjectTypeEXT, core::uint64 object, const char* objectName) {
  //*
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    const VkDebugMarkerObjectNameInfoEXT vkDebugMarkerObjectNameInfoEXT = {
      VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT,	// sType (VkStructureType)
      nullptr,												// pNext (const void*)
      vkDebugReportObjectTypeEXT,			// objectType (VkDebugReportObjectTypeEXT)
      object,													// object (core::uint64)
      objectName											// pObjectName (const char*)
    };
    //vkDebugMarkerSetObjectNameEXT(vkDevice, &vkDebugMarkerObjectNameInfoEXT);
  }
 //*/
}
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan