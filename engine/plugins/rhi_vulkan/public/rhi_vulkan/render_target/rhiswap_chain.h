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
#include <rhi/render_target/rhiswap_chain.h>
#include <core/container/vector.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;
class VulkanContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class RHISwapChain : public rhi::RHISwapChain {
  //[-------------------------------------------------------]
  //[ Public static methods                                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] static VkFormat find_color_vk_format(const rhi::RHIContext& context, VkInstance vkInstance, const VulkanContext& vulkanContext);

  [[nodiscard]] static VkFormat find_depth_vk_format(VkPhysicalDevice vkPhysicalDevice);

public:

  RHISwapChain(rhi::RHIRenderPass& renderPass, core::FrontendPlatformData windowHandle RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);
  
  ~RHISwapChain() override;


  [[nodiscard]] VkRenderPass get_vk_render_pass() const;

  [[nodiscard]] VkImage get_color_current_vk_image() const;

  [[nodiscard]] VkImage get_depth_vk_image() const;

  [[nodiscard]] VkFramebuffer get_current_vk_framebuffer() const;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIRenderTarget methods             ]
  //[-------------------------------------------------------]
public:
  void get_width_and_height(core::uint32 &width, core::uint32 &height) const override;

public:
  /**
   * @brief
   * Return the native window handle
   *
   * @return
   * Native window handle the swap chain is using as output window, can be a null handle
   */
  [[nodiscard]] core::handle get_native_window_handle() const override;

  /**
   * @brief
   * Set vertical synchronization interval
   *
   * @param[in] synchronizationInterval
   * Synchronization interval, >0 if vertical synchronization should be used, else zero
   */
  void set_vertical_synchronization_interval(core::uint32 synchronizationInterval) override;

  /**
   * @brief
   * Present the content of the current back buffer
   *
   * @note
   * - Swap of front and back buffer
   */
  void present() override;

  /**
   * @brief
   * Call this method whenever the size of the native window was changed
   */
  void resize_buffers() override;

  /**
   * @brief
   * Return the current fullscreen state
   *
   * @return
   * "true" if fullscreen, else "false"
   */
  [[nodiscard]] bool get_fullscreen_state() const override;

  /**
   * @brief
   * Set the current fullscreen state
   *
   * @param[in] fullscreen
   * "true" if fullscreen, else "false"
   */
  void set_fullscreen_state(bool fullscreen) override;

  /**
   * @brief
   * Set an render window instance
   *
   * @param[in] renderWindow
   * The render window interface instance, can be a null pointer, if valid the instance must stay valid as long as it's connected to the swap chain instance
   *
   * @remarks
   * This method can be used to override the platform specific handling for retrieving window size and doing an buffer swap on the render window (aka present).
   * An instance can be set when the user don't want that the swap chain itself tempers with the given window handle (the handle might be invalid but non zero)
   * e.g. the user uses a windowing library (e.g. SDL2) which abstracts the window handling on different windowing platforms (e.g. Win32 or Linux/Wayland) and
   * the application should run on a windowing platform which isn't supported by the swap chain implementation itself.
   */
  void set_render_window(rhi::IRenderWindow *renderWindow) override;

private:

  void create_vk_swap_chain();

  void destroy_vk_swap_chain();

  void acquire_next_image(bool recreateSwapChainIfNeeded);

  void create_depth_render_target(const VkExtent2D& vkExtent2d);

  void destroy_depth_render_target();

private:

  struct SwapChainBuffer {
    VkImage vkImage = VK_NULL_HANDLE;
    VkImageView vkImageView = VK_NULL_HANDLE;
    VkFramebuffer vkFramebuffer = VK_NULL_HANDLE;
  };

private:
  // Operating system window
  core::FrontendPlatformData mFrontendPlatformData;
  /** Native window handle window, can be a null handle */
  core::handle			mNativeWindowHandle;
  /** Render window instance, can be a null pointer, don't destroy the instance since we don't own it */
  rhi::IRenderWindow* mRenderWindow;
  // Vulkan presentation surface
  /** Vulkan presentation surface, destroy if no longer needed */
  VkSurfaceKHR mVkSurfaceKHR;	
  // Vulkan swap chain and color render target related
  /** Vulkan swap chain, destroy if no longer needed */
  VkSwapchainKHR	 mVkSwapchainKHR;
  /** Vulkan render pass, destroy if no longer needed (due to "VK_IMAGE_LAYOUT_PRESENT_SRC_KHR" we need an own Vulkan render pass instance) */
  VkRenderPass	 mVkRenderPass;
  /** Swap chain buffer for managing the color render targets */
  core::Vector<SwapChainBuffer> mSwapChainBuffer;
  /** Vulkan semaphore, destroy if no longer needed */
  VkSemaphore		 mImageAvailableVkSemaphore;
  /** Vulkan semaphore, destroy if no longer needed */
  VkSemaphore		 mRenderingFinishedVkSemaphore;
  /** The index of the current Vulkan swap chain image to render into, ~0 if invalid */
  core::uint32		 mCurrentImageIndex;
  // Depth render target related
  /** Can be "VK_FORMAT_UNDEFINED" if no depth stencil buffer is needed */
  VkFormat		mDepthVkFormat;	
  VkImage			mDepthVkImage;
  VkDeviceMemory  mDepthVkDeviceMemory;
  VkImageView		mDepthVkImageView;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan