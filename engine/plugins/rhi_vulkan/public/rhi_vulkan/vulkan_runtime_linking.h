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
/**
 * @class
 * VulkanRuntimeLinking
 *
 * @brief
 * Helper structure fo linking against the vulkan libraries during runtime.
 */
class VulkanRuntimeLinking {
public:
  /**
   * @brief
   * Constructor.
   */
  VulkanRuntimeLinking(RHIDevice& rhiDynamicRhi, bool enableValidation);

  /**
   * @brief
   * Destructor.
   */
  ~VulkanRuntimeLinking();

public:

  [[nodiscard]] bool is_validation_enabled() const;

  [[nodiscard]] bool is_vulkan_available();

  [[nodiscard]] VkInstance get_vk_instance() const;

  [[nodiscard]] bool load_device_level_entry_points(VkDevice vkDevice, bool hasMeshShaderSupport) const;

private:

  [[nodiscard]] bool load_shared_libraries();

  [[nodiscard]] bool load_global_level_entry_points() const;

  [[nodiscard]] bool load_instance_level_entry_points() const;

  [[nodiscard]] VkResult create_vulkan_instance(bool enableValidation);

  void setup_debug_callbacks();

private:
  /** Refernce to dynamic rhi */
  RHIDevice& mRhiDynamicRhi;
  /** Pointer to shared library */
  void* mVulkanSharedLibrary;
  /** Vulkan instance */
  VkInstance mVkInstance;
  /** Vulkan debug callback */
  VkDebugReportCallbackEXT mVkDebugReportCallbackEXT;
  /** Flag for checking of validation is enabled */
  bool mEnableValidation;
  /** Flag for checkin if entry points are registered */
  bool mEntryPointsRegistered;
  /** Flag for checking of instance level functions are registered */
  bool mInstanceLevelFunctionsRegistered;
  /** Flag for checking if vulkan library was initialized */
  bool mInitialized;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
