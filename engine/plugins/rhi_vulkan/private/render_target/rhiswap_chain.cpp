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
#include "rhi_vulkan/render_target/rhiswap_chain.h"
#include "rhi_vulkan/render_target/rhirender_pass.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/vulkan_runtime_linking.h"
#include "rhi_vulkan/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
VkFormat RHISwapChain::find_color_vk_format(
  const rhi::RHIContext &context, VkInstance vkInstance, const VulkanContext &vulkanContext) {
  const VkPhysicalDevice vkPhysicalDevice = vulkanContext.get_vk_physical_device();
  const VkSurfaceKHR vkSurfaceKHR = VulkanHelper::create_presentation_surface(context,
                                                                        vulkanContext.get_rhidynamicrhi().get_vk_allocation_callbacks(),
                                                                        vkInstance, vkPhysicalDevice,
                                                                        vulkanContext.get_graphics_queue_family_index(),
                                                                        context.get_frontend_platform_data());
  const VkSurfaceFormatKHR desiredVkSurfaceFormatKHR = VulkanHelper::get_swap_chain_format(context, vkPhysicalDevice, vkSurfaceKHR);
  vkDestroySurfaceKHR(vkInstance, vkSurfaceKHR, vulkanContext.get_rhidynamicrhi().get_vk_allocation_callbacks());
  return desiredVkSurfaceFormatKHR.format;
}

VkFormat RHISwapChain::find_depth_vk_format(VkPhysicalDevice vkPhysicalDevice) {
  core::Vector<VkFormat> formats;
  formats.push_back(VK_FORMAT_D32_SFLOAT);
  formats.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
  formats.push_back(VK_FORMAT_D24_UNORM_S8_UINT);
  return VulkanHelper::find_supported_vk_format(
    vkPhysicalDevice,
    formats,
    VK_IMAGE_TILING_OPTIMAL,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}


RHISwapChain::RHISwapChain(rhi::RHIRenderPass &renderPass, core::FrontendPlatformData windowHandle RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHISwapChain(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  // Operation system window
, mFrontendPlatformData(windowHandle)
#if defined(LINUX)
, mNativeWindowHandle(windowHandle.os_linux.window)
, mRenderWindow(nullptr)
#endif
  // Vulkan presentation surface
, mVkSurfaceKHR(VK_NULL_HANDLE)
  // Vulkan swap chain and color render target related
, mVkSwapchainKHR(VK_NULL_HANDLE)
, mVkRenderPass(VK_NULL_HANDLE)
, mImageAvailableVkSemaphore(VK_NULL_HANDLE)
, mRenderingFinishedVkSemaphore(VK_NULL_HANDLE)
, mCurrentImageIndex(~0u)
  // Depth render target related
, mDepthVkFormat(Mapping::get_vulkan_format(static_cast<RHIRenderPass&>(renderPass).get_depth_stencil_attachment_texture_format()))
, mDepthVkImage(VK_NULL_HANDLE)
, mDepthVkDeviceMemory(VK_NULL_HANDLE)
, mDepthVkImageView(VK_NULL_HANDLE) {
  // Create the Vulkan presentation surface instance depending on the operation system
  RHIDevice& vulkanRhi = static_cast<RHIDevice&>(renderPass.get_rhi());
  const VulkanContext&   vulkanContext	= vulkanRhi.get_vulkan_context();
  const VkInstance	   vkInstance		= vulkanRhi.get_vulkan_runtime_linking().get_vk_instance();
  const VkPhysicalDevice vkPhysicalDevice	= vulkanContext.get_vk_physical_device();
  mVkSurfaceKHR = VulkanHelper::create_presentation_surface(vulkanRhi.get_context(), vulkanRhi.get_vk_allocation_callbacks(), vkInstance, vkPhysicalDevice, vulkanContext.get_graphics_queue_family_index(), windowHandle);
  if (VK_NULL_HANDLE != mVkSurfaceKHR) {
    // Create the Vulkan swap chain
    create_vk_swap_chain();
  } else {
    // Error!
    BE_LOG(Critical, "The swap chain failed to create the Vulkan presentation surface")
  }
}

RHISwapChain::~RHISwapChain() {
  if (VK_NULL_HANDLE != mVkSurfaceKHR) {
    destroy_vk_swap_chain();
    const RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
    vkDestroySurfaceKHR(vulkanRhi.get_vulkan_runtime_linking().get_vk_instance(), mVkSurfaceKHR, vulkanRhi.get_vk_allocation_callbacks());
  }
}

VkRenderPass RHISwapChain::get_vk_render_pass() const {
  return mVkRenderPass;
}

VkImage RHISwapChain::get_color_current_vk_image() const {
  BE_ASSERT(~0u != mCurrentImageIndex, "Invalid index of the current Vulkan swap chain image to render into (Vulkan swap chain creation failed?)")
  BE_ASSERT(mCurrentImageIndex < mSwapChainBuffer.size(), "Out-of-bounds index of the current Vulkan swap chain image to render into")
  return mSwapChainBuffer[mCurrentImageIndex].vkImage;
}

VkImage RHISwapChain::get_depth_vk_image() const {
  return mDepthVkImage;
}

VkFramebuffer RHISwapChain::get_current_vk_framebuffer() const {
  BE_ASSERT(~0u != mCurrentImageIndex, "Invalid index of the current Vulkan swap chain image to render into (Vulkan swap chain creation failed?)")
  BE_ASSERT(mCurrentImageIndex < mSwapChainBuffer.size(), "Out-of-bounds index of the current Vulkan swap chain image to render into")
  return mSwapChainBuffer[mCurrentImageIndex].vkFramebuffer;
}


void RHISwapChain::get_width_and_height(core::uint32& width, core::uint32& height) const override {
  // Return stored width and height when both valid
  if (nullptr != mRenderWindow) {
    mRenderWindow->get_width_and_height(width, height);
    return;
  }
#ifdef _WIN32
  // Is there a valid native OS window?
  if (NULL_HANDLE != mNativeWindowHandle) {
    // Get the width and height
    long swapChainWidth  = 1;
    long swapChainHeight = 1;
    {
      // Get the client rectangle of the native output window
      // -> Don't use the width and height stored in "DXGI_SWAP_CHAIN_DESC" -> "DXGI_MODE_DESC"
      //    because it might have been modified in order to avoid zero values
      RECT rect;
      ::GetClientRect(reinterpret_cast<HWND>(mNativeWindowHandle), &rect);
      
      // Get the width and height...
      swapChainWidth  = rect.right  - rect.left;
      swapChainHeight = rect.bottom - rect.top;
      
      // ... and ensure that none of them is ever zero
      if (swapChainWidth < 1) {
        swapChainWidth = 1;
      }
      if (swapChainHeight < 1) {
        swapChainHeight = 1;
      }
    }
    
    // Write out the width and height
    width  = static_cast<UINT>(swapChainWidth);
    height = static_cast<UINT>(swapChainHeight);
  }
  else
#elif defined(__ANDROID__)
  if (NULL_HANDLE != mNativeWindowHandle) {
    
    width = height = 1;
  }
  else
#elif defined LINUX
  if (NULL_HANDLE != mNativeWindowHandle) {
    /// TODO(naetherm): Implement me!
    //RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
    //Display* display = static_cast<const OpenGLContextLinux &>(vulkanRhi.get_opengl_context()).get_display();

    // get the width and height...
    unsigned int unsignedWidth = 0, unsignedHeight = 0;
    xcb_get_geometry_cookie_t cookie = xcb_get_geometry(reinterpret_cast<xcb_connection_t*>(mFrontendPlatformData.os_linux.connection), mNativeWindowHandle);
    xcb_get_geometry_reply_t* reply = xcb_get_geometry_reply(reinterpret_cast<xcb_connection_t*>(mFrontendPlatformData.os_linux.connection), cookie, nullptr);

    if (reply) {
      unsignedWidth = reply->width;
      unsignedHeight = reply->height;
    }

    // ... and ensure that none of them is ever zero
    if (unsignedWidth < 1) {
      unsignedWidth = 1;
    }
    if (unsignedHeight < 1) {
      unsignedHeight = 1;
    }

    // Done
    width = unsignedWidth;
    height = unsignedHeight;
  }
  else
#else
#error "Unsupported platform"
#endif
  {
    // Set known default return values
    width  = 1;
    height = 1;
  }
}


core::handle RHISwapChain::get_native_window_handle() const {
  return mNativeWindowHandle;
}

void RHISwapChain::set_vertical_synchronization_interval(core::uint32 synchronizationInterval) {

}

void RHISwapChain::present() {
  // Get the Vulkan context
  const RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
  const VulkanContext& vulkanContext = vulkanRhi.get_vulkan_context();

  { // Queue submit
    const VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    const VkCommandBuffer vkCommandBuffer = vulkanContext.get_vk_command_buffer();
    const VkSubmitInfo vkSubmitInfo = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,	      // sType (VkStructureType)
        nullptr,						                  // pNext (const void*)
        1,								                    // waitSemaphoreCount (core::uint32)
        &mImageAvailableVkSemaphore,	        // pWaitSemaphores (const VkSemaphore*)
        &waitDstStageMask,				            // pWaitDstStageMask (const VkPipelineStageFlags*)
        1,								                    // commandBufferCount (core::uint32)
        &vkCommandBuffer,				              // pCommandBuffers (const VkCommandBuffer*)
        1,								                    // signalSemaphoreCount (core::uint32)
        &mRenderingFinishedVkSemaphore	      // pSignalSemaphores (const VkSemaphore*)
      };
    if (vkQueueSubmit(vulkanContext.get_graphics_vk_queue(), 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
      // Error!
      BE_LOG(Critical, "Vulkan queue submit failed")
      return;
    }
  }

  { // Queue present
    const VkPresentInfoKHR vkPresentInfoKHR = {
        VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,	// sType (VkStructureType)
        nullptr,							// pNext (const void*)
        1,									// waitSemaphoreCount (core::uint32)
        &mRenderingFinishedVkSemaphore,		// pWaitSemaphores (const VkSemaphore*)
        1,									// swapchainCount (core::uint32)
        &mVkSwapchainKHR,					// pSwapchains (const VkSwapchainKHR*)
        &mCurrentImageIndex,				// pImageIndices (const core::uint32*)
        nullptr								// pResults (VkResult*)
      };
    const VkResult vkResult = vkQueuePresentKHR(vulkanContext.get_present_vk_queue(), &vkPresentInfoKHR);
    if (VK_SUCCESS != vkResult) {
      if (VK_ERROR_OUT_OF_DATE_KHR == vkResult || VK_SUBOPTIMAL_KHR == vkResult) {
        // Recreate the Vulkan swap chain
        create_vk_swap_chain();
        return;
      } else {
        // Error!
        BE_LOG(Critical, "Failed to present Vulkan queue")
        return;
      }
    }
    vkQueueWaitIdle(vulkanContext.get_present_vk_queue());
  }

  // Acquire next image
  acquire_next_image(true);
}

void RHISwapChain::resize_buffers() {
  // Recreate the Vulkan swap chain
  create_vk_swap_chain();
}

bool RHISwapChain::get_fullscreen_state() const {
  return false;
}

void RHISwapChain::set_fullscreen_state(bool fullscreen) {

}

void RHISwapChain::set_render_window(rhi::IRenderWindow *renderWindow) {
  mRenderWindow = renderWindow;
}

void RHISwapChain::create_vk_swap_chain() {
  // Get the Vulkan physical device
  const RHIDevice&	   vulkanRhi		    = static_cast<const RHIDevice&>(get_rhi());
  const rhi::RHIContext&	   context			= vulkanRhi.get_context();
  const VulkanContext&   vulkanContext	    = vulkanRhi.get_vulkan_context();
  const VkPhysicalDevice vkPhysicalDevice	  = vulkanContext.get_vk_physical_device();
  const VkDevice		   vkDevice			        = vulkanContext.get_vk_device();

  // Sanity checks
  BE_ASSERT(VK_NULL_HANDLE != vkPhysicalDevice, "Invalid physical Vulkan device")
  BE_ASSERT(VK_NULL_HANDLE != vkDevice, "Invalid Vulkan device")

  // Wait for the Vulkan device to become idle
  vkDeviceWaitIdle(vkDevice);

  // Get Vulkan surface capabilities
  VkSurfaceCapabilitiesKHR vkSurfaceCapabilitiesKHR;
  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, mVkSurfaceKHR, &vkSurfaceCapabilitiesKHR) != VK_SUCCESS) {
    BE_LOG(Critical, "Failed to get physical Vulkan device surface capabilities")
    return;
  }

  // Get Vulkan swap chain settings
  const core::uint32                      desiredNumberOfImages				 = VulkanHelper::get_number_of_swap_chain_images(vkSurfaceCapabilitiesKHR);
  const VkSurfaceFormatKHR            desiredVkSurfaceFormatKHR			 = VulkanHelper::get_swap_chain_format(context, vkPhysicalDevice, mVkSurfaceKHR);
  const VkExtent2D                    desiredVkExtent2D					 = VulkanHelper::get_swap_chain_extent(vkSurfaceCapabilitiesKHR);
  const VkImageUsageFlags             desiredVkImageUsageFlags			 = VulkanHelper::get_swap_chain_usage_flags(context, vkSurfaceCapabilitiesKHR);
  const VkSurfaceTransformFlagBitsKHR desiredVkSurfaceTransformFlagBitsKHR = VulkanHelper::get_swap_chain_transform(vkSurfaceCapabilitiesKHR);
  const VkPresentModeKHR              desiredVkPresentModeKHR				 = VulkanHelper::get_swap_chain_present_mode(context, vkPhysicalDevice, mVkSurfaceKHR);

  // Validate Vulkan swap chain settings
  if (-1 == static_cast<int>(desiredVkImageUsageFlags)) {
    BE_LOG(Critical, "Invalid desired Vulkan image usage flags")
    return;
  }
  if (VK_PRESENT_MODE_MAX_ENUM_KHR == desiredVkPresentModeKHR) {
    BE_LOG(Critical, "Invalid desired Vulkan presentation mode")
    return;
  }
  if ((0 == desiredVkExtent2D.width) || (0 == desiredVkExtent2D.height)) {
    // Current surface size is (0, 0) so we can't create a swap chain and render anything (CanRender == false)
    // But we don't wont to kill the application as this situation may occur i.e. when window gets minimized
    destroy_vk_swap_chain();
    return;
  }

  { // Create Vulkan swap chain
    VkSwapchainKHR newVkSwapchainKHR = VK_NULL_HANDLE;
    const VkSwapchainCreateInfoKHR vkSwapchainCreateInfoKHR = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,	// sType (VkStructureType)
        nullptr,										// pNext (const void*)
        0,												// flags (VkSwapchainCreateFlagsKHR)
        mVkSurfaceKHR,									// surface (VkSurfaceKHR)
        desiredNumberOfImages,							// minImageCount (core::uint32)
        desiredVkSurfaceFormatKHR.format,				// imageFormat (VkFormat)
        desiredVkSurfaceFormatKHR.colorSpace,			// imageColorSpace (VkColorSpaceKHR)
        desiredVkExtent2D,								// imageExtent (VkExtent2D)
        1,												// imageArrayLayers (core::uint32)
        desiredVkImageUsageFlags,						// imageUsage (VkImageUsageFlags)
        VK_SHARING_MODE_EXCLUSIVE,						// imageSharingMode (VkSharingMode)
        0,												// queueFamilyIndexCount (core::uint32)
        nullptr,										// pQueueFamilyIndices (const core::uint32*)
        desiredVkSurfaceTransformFlagBitsKHR,			// preTransform (VkSurfaceTransformFlagBitsKHR)
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,				// compositeAlpha (VkCompositeAlphaFlagBitsKHR)
        desiredVkPresentModeKHR,						// presentMode (VkPresentModeKHR)
        VK_TRUE,										// clipped (VkBool32)
        mVkSwapchainKHR									// oldSwapchain (VkSwapchainKHR)
      };
    if (vkCreateSwapchainKHR(vkDevice, &vkSwapchainCreateInfoKHR, vulkanRhi.get_vk_allocation_callbacks(), &newVkSwapchainKHR) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to create Vulkan swap chain")
      return;
    }
    destroy_vk_swap_chain();
    mVkSwapchainKHR = newVkSwapchainKHR;
  }

  // Create depth render target
  create_depth_render_target(desiredVkExtent2D);

  // Create render pass
  mVkRenderPass = VulkanHelper::create_render_pass(context, vulkanRhi.get_vk_allocation_callbacks(), vkDevice,
                                                   desiredVkSurfaceFormatKHR.format, mDepthVkFormat,
                                                   static_cast<RHIRenderPass &>(get_render_pass()).get_vk_sample_count_flag_bits());

  // Vulkan swap chain image handling
  if (VK_NULL_HANDLE != mVkRenderPass) {
    // Get the swap chain images
    core::uint32 swapchainImageCount = 0;
    if (vkGetSwapchainImagesKHR(vkDevice, mVkSwapchainKHR, &swapchainImageCount, nullptr) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to get Vulkan swap chain images")
      return;
    }
    core::Vector<VkImage> vkImages(swapchainImageCount);
    if (vkGetSwapchainImagesKHR(vkDevice, mVkSwapchainKHR, &swapchainImageCount, vkImages.data()) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to get Vulkan swap chain images")
      return;
    }

    // Get the swap chain buffers containing the image and image view
    mSwapChainBuffer.resize(swapchainImageCount);
    const bool hasDepthStencilAttachment = (VK_FORMAT_UNDEFINED != mDepthVkFormat);
    for (core::uint32 i = 0; i < swapchainImageCount; ++i) {
      SwapChainBuffer& swapChainBuffer = mSwapChainBuffer[i];
      swapChainBuffer.vkImage = vkImages[i];

      // Create the Vulkan image view
      VulkanHelper::create_vk_image_view(vulkanRhi, swapChainBuffer.vkImage, VK_IMAGE_VIEW_TYPE_2D, 1, 1, desiredVkSurfaceFormatKHR.format, VK_IMAGE_ASPECT_COLOR_BIT, swapChainBuffer.vkImageView);

      { // Create the Vulkan framebuffer
        const core::Array<VkImageView, 2> vkImageViews = {
          swapChainBuffer.vkImageView,
          mDepthVkImageView
        };
        const VkFramebufferCreateInfo vkFramebufferCreateInfo = {
          VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,	// sType (VkStructureType)
          nullptr,									// pNext (const void*)
          0,											// flags (VkFramebufferCreateFlags)
          mVkRenderPass,								// renderPass (VkRenderPass)
          hasDepthStencilAttachment ? 2u : 1u,		// attachmentCount (core::uint32)
          vkImageViews.data(),						// pAttachments (const VkImageView*)
          desiredVkExtent2D.width,					// width (core::uint32)
          desiredVkExtent2D.height,					// height (core::uint32)
          1											// layers (core::uint32)
        };
        if (vkCreateFramebuffer(vkDevice, &vkFramebufferCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &swapChainBuffer.vkFramebuffer) != VK_SUCCESS) {
          BE_LOG(Critical, "Failed to create Vulkan framebuffer")
        }
      }
    }
  }

  { // Create the Vulkan semaphores
    static constexpr VkSemaphoreCreateInfo vkSemaphoreCreateInfo = {
      VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,	// sType (VkStructureType)
      nullptr,									// pNext (const void*)
      0											// flags (VkSemaphoreCreateFlags)
    };
    if ((vkCreateSemaphore(vkDevice, &vkSemaphoreCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &mImageAvailableVkSemaphore) != VK_SUCCESS) ||
        (vkCreateSemaphore(vkDevice, &vkSemaphoreCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &mRenderingFinishedVkSemaphore) != VK_SUCCESS)) {
      BE_LOG(Critical, "Failed to create Vulkan semaphore")
    }
  }

  // Acquire next image
  acquire_next_image(false);
}

void RHISwapChain::destroy_vk_swap_chain() {
  // Destroy Vulkan swap chain
  if (VK_NULL_HANDLE != mVkRenderPass || !mSwapChainBuffer.empty() || VK_NULL_HANDLE != mVkSwapchainKHR) {
    const RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
    const VkDevice vkDevice = vulkanRhi.get_vulkan_context().get_vk_device();
    vkDeviceWaitIdle(vkDevice);
    if (VK_NULL_HANDLE != mVkRenderPass) {
      vkDestroyRenderPass(vkDevice, mVkRenderPass, vulkanRhi.get_vk_allocation_callbacks());
      mVkRenderPass = VK_NULL_HANDLE;
    }
    if (!mSwapChainBuffer.empty()) {
      for (const SwapChainBuffer& swapChainBuffer : mSwapChainBuffer) {
        vkDestroyFramebuffer(vkDevice, swapChainBuffer.vkFramebuffer, vulkanRhi.get_vk_allocation_callbacks());
        vkDestroyImageView(vkDevice, swapChainBuffer.vkImageView, vulkanRhi.get_vk_allocation_callbacks());
      }
      mSwapChainBuffer.clear();
    }
    if (VK_NULL_HANDLE != mVkSwapchainKHR) {
      vkDestroySwapchainKHR(vkDevice, mVkSwapchainKHR, vulkanRhi.get_vk_allocation_callbacks());
      mVkSwapchainKHR = VK_NULL_HANDLE;
    }
    if (VK_NULL_HANDLE != mImageAvailableVkSemaphore) {
      vkDestroySemaphore(vulkanRhi.get_vulkan_context().get_vk_device(), mImageAvailableVkSemaphore, vulkanRhi.get_vk_allocation_callbacks());
      mImageAvailableVkSemaphore = VK_NULL_HANDLE;
    }
    if (VK_NULL_HANDLE != mRenderingFinishedVkSemaphore) {
      vkDestroySemaphore(vulkanRhi.get_vulkan_context().get_vk_device(), mRenderingFinishedVkSemaphore, vulkanRhi.get_vk_allocation_callbacks());
      mRenderingFinishedVkSemaphore = VK_NULL_HANDLE;
    }
  }

  // Destroy depth render target
  destroy_depth_render_target();
}

void RHISwapChain::acquire_next_image(bool recreateSwapChainIfNeeded) {
  const RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
  const VkResult vkResult = vkAcquireNextImageKHR(vulkanRhi.get_vulkan_context().get_vk_device(), mVkSwapchainKHR, UINT64_MAX, mImageAvailableVkSemaphore, VK_NULL_HANDLE, &mCurrentImageIndex);
  if (VK_SUCCESS != vkResult && VK_SUBOPTIMAL_KHR != vkResult) {
    if (VK_ERROR_OUT_OF_DATE_KHR == vkResult) {
      // Recreate the Vulkan swap chain
      if (recreateSwapChainIfNeeded) {
        create_vk_swap_chain();
      }
    } else {
      // Error!
      BE_LOG(Critical, "Failed to acquire next Vulkan image from swap chain")
    }
  }
}

void RHISwapChain::create_depth_render_target(const VkExtent2D &vkExtent2d) {
  if (VK_FORMAT_UNDEFINED != mDepthVkFormat) {
    const RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
    VulkanHelper::create_and_allocate_vk_image(vulkanRhi, 0, VK_IMAGE_TYPE_2D, { vkExtent2d.width, vkExtent2d.height, 1 }, 1, 1, mDepthVkFormat, static_cast<RHIRenderPass&>(get_render_pass()).get_vk_sample_count_flag_bits(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mDepthVkImage, mDepthVkDeviceMemory);
    VulkanHelper::create_vk_image_view(vulkanRhi, mDepthVkImage, VK_IMAGE_VIEW_TYPE_2D, 1, 1, mDepthVkFormat, VK_IMAGE_ASPECT_DEPTH_BIT, mDepthVkImageView);

    //Helper::transitionVkImageLayout(vulkanRhi, mDepthVkImage, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  }
}

void RHISwapChain::destroy_depth_render_target() {
  if (VK_NULL_HANDLE != mDepthVkImage) {
    BE_ASSERT(VK_NULL_HANDLE != mDepthVkDeviceMemory, "Invalid Vulkan depth device memory")
    BE_ASSERT(VK_NULL_HANDLE != mDepthVkImageView, "Invalid Vulkan depth image view")
    VulkanHelper::destroy_and_free_vk_image(static_cast<RHIDevice&>(get_rhi()), mDepthVkImage, mDepthVkDeviceMemory, mDepthVkImageView);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan