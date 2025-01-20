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
//#define VULKAN_DEFINE_RUNTIMELINKING
#include "rhi_vulkan/vulkan_runtime_linking.h"
//#undef VULKAN_DEFINE_RUNTIMELINKING
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include <core/container/vector.h>
#if defined(LINUX)
#include <link.h>
#include <dlfcn.h>
#endif
#include <sstream>


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
static constexpr const char* VALIDATION_LAYER_NAMES[] = {
  "VK_LAYER_LUNARG_standard_validation"
};
#endif


[[nodiscard]] const char* vk_debug_report_object_type_to_string(VkDebugReportObjectTypeEXT vkDebugReportObjectTypeEXT) {
  // Define helper macro
#define VALUE(value) case value: return #value;

  // Evaluate
  switch (vkDebugReportObjectTypeEXT) {
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR_EXT)
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV_EXT)
    //	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT)	not possible
    //	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT)	not possible
    //	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_KHR_EXT)	not possible
    //	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR_EXT)	not possible
    //	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_BEGIN_RANGE_EXT)	not possible
    //	VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_END_RANGE_EXT)	not possible
    VALUE(VK_DEBUG_REPORT_OBJECT_TYPE_MAX_ENUM_EXT)
  }

  // Undefine helper macro
#undef VALUE

  // Error!
  return nullptr;
}

[[nodiscard]] VKAPI_ATTR VkBool32 VKAPI_CALL debug_report_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, core::uint64 object, size_t location, core::int32 messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData) {
  const rhi::RHIContext* context = static_cast<const rhi::RHIContext*>(pUserData);

  // TODO(naetherm) Inside e.g. the "InstancedCubes"-example the log gets currently flooded with
  //          "Warning: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT" Object: "120" Location: "5460" Message code: "0" Layer prefix: "DS" Message: "DescriptorSet 0x78 previously bound as set #0 is incompatible with set 0xc82f498 newly bound as set #0 so set #1 and any subsequent sets were disturbed by newly bound pipelineLayout (0x8b)" ".
  //          It's a known Vulkan API issue regarding validation. See https://github.com/KhronosGroup/Vulkan-Docs/issues/305 - "vkCmdBindDescriptorSets should be able to take NULL sets. #305".
  //          Currently I see no other way then ignoring this message.
  if (VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT == objectType && 5460 == location && 0 == messageCode) {
    // The Vulkan call should not be aborted to have the same behavior with and without validation layers enabled
    return VK_FALSE;
  }

  // TODO(naetherm) File "racoonengine\source\rhi\private\vulkanrhi\vulkanrhi.cpp" | Line 1029 | Critical: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT" Object: "4963848" Location: "0" Message code: "0" Layer prefix: "Loader Message" Message: "loader_create_device_chain: Failed to find 'vkGetInstanceProcAddr' in layer C:\Program Files (x86)\Steam\.\SteamOverlayVulkanLayer.dll.  Skipping layer."
  if (VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT == objectType && object && 0 == location && 0 == messageCode && nullptr != strstr(pMessage, "SteamOverlayVulkanLayer.dll")) {
    return VK_FALSE;
  }

  // Get log message type
  // -> Vulkan is using a flags combination, map it to our log message type enumeration
  /// TODO(naetherm): Reimplement this
  /*
  core::ILog::Type type = core::ILog::Type::TRACE;
  if ((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0)
  {
    type = core::ILog::Type::CRITICAL;
  }
  else if ((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0)
  {
    type = core::ILog::Type::WARNING;
  }
  else if ((flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0)
  {
    type = core::ILog::Type::PERFORMANCE_WARNING;
  }
  else if ((flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0)
  {
    type = core::ILog::Type::INFORMATION;
  }
  else if ((flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0)
  {
    type = core::ILog::Type::DEBUG;
  }
 */

  // Construct the log message
  std::stringstream message;
  message << "Vulkan debug report callback: ";
  message << "Object type: \"" << vk_debug_report_object_type_to_string(objectType) << "\" ";
  message << "Object: \"" << object << "\" ";
  message << "Location: \"" << location << "\" ";
  message << "Message code: \"" << messageCode << "\" ";
  message << "Layer prefix: \"" << pLayerPrefix << "\" ";
  message << "Message: \"" << pMessage << "\" ";

  // Print log message
  BE_LOG(Critical, message.str().c_str())
  //if (context->getLog().print(type, nullptr, __FILE__, static_cast<core::uint32>(__LINE__), message.str().c_str()))
  {
    //	DEBUG_BREAK;
  }

  // The Vulkan call should not be aborted to have the same behavior with and without validation layers enabled
  return VK_FALSE;
}

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


VulkanRuntimeLinking::VulkanRuntimeLinking(RHIDevice &rhiDynamicRhi, bool enableValidation)
: mRhiDynamicRhi(rhiDynamicRhi)
, mVulkanSharedLibrary(nullptr)
, mVkInstance(VK_NULL_HANDLE)
, mVkDebugReportCallbackEXT(VK_NULL_HANDLE)
, mEnableValidation(enableValidation)
, mEntryPointsRegistered(false)
, mInstanceLevelFunctionsRegistered(false)
, mInitialized(false) {

}

VulkanRuntimeLinking::~VulkanRuntimeLinking() {
  // Destroy the Vulkan debug report callback
  if (VK_NULL_HANDLE != mVkDebugReportCallbackEXT) {
    vkDestroyDebugReportCallbackEXT(mVkInstance, mVkDebugReportCallbackEXT, mRhiDynamicRhi.get_vk_allocation_callbacks());
  }

  // Destroy the Vulkan instance
  if (VK_NULL_HANDLE != mVkInstance) {
    vkDestroyInstance(mVkInstance, mRhiDynamicRhi.get_vk_allocation_callbacks());
  }

  // Destroy the shared library instances
#ifdef _WIN32
  if (nullptr != mVulkanSharedLibrary) {
    ::FreeLibrary(static_cast<HMODULE>(mVulkanSharedLibrary));
  }
#elif defined LINUX
  if (nullptr != mVulkanSharedLibrary) {
    ::dlclose(mVulkanSharedLibrary);
  }
#else
#error "Unsupported platform"
#endif
}


bool VulkanRuntimeLinking::is_validation_enabled() const {
  return mEnableValidation;
}

bool VulkanRuntimeLinking::is_vulkan_available() {
  if (!mInitialized) {
    // We are on it ...
    mInitialized = true;

    const VkResult result = create_vulkan_instance(mEntryPointsRegistered);
    /*
    // First load the shared library
    if (load_shared_libraries()) {
      // Now, load the global entry points
      mEntryPointsRegistered = load_global_level_entry_points();
      if (mEntryPointsRegistered) {
        const VkResult result = create_vulkan_instance(mEntryPointsRegistered);

        if (result == VK_SUCCESS) {
          mInstanceLevelFunctionsRegistered = load_instance_level_entry_points();

          if (mInstanceLevelFunctionsRegistered && mEnableValidation) {
            setup_debug_callbacks();
          }
        } else {
          BE_LOG(Critical, "Failed to create a valid vulkan instance")
        }
      }
    }
     */
  }

  return mInitialized; // (mEntryPointsRegistered && mInstanceLevelFunctionsRegistered && (mVkInstance != VK_NULL_HANDLE));
}

VkInstance VulkanRuntimeLinking::get_vk_instance() const {
  return mVkInstance;
}

bool VulkanRuntimeLinking::load_device_level_entry_points(VkDevice vkDevice, bool hasMeshShaderSupport) const {

#define IMPORT_FUNC(funcName)																										\
				if (result)																														\
				{																																\
					funcName = reinterpret_cast<PFN_##funcName>(vkGetDeviceProcAddr(vkDevice, #funcName));										\
					if (nullptr == funcName)																									\
					{																															\
						BE_LOG(Critical, core::String("Failed to load instance based Vulkan function pointer ") + #funcName)	\
						result = false;																											\
					}																															\
				}

  bool result = true;

  // Load the Vulkan device level function entry points
  IMPORT_FUNC(vkDestroyDevice)
  IMPORT_FUNC(vkCreateShaderModule)
  IMPORT_FUNC(vkDestroyShaderModule)
  IMPORT_FUNC(vkCreateBuffer)
  IMPORT_FUNC(vkDestroyBuffer)
  IMPORT_FUNC(vkMapMemory)
  IMPORT_FUNC(vkUnmapMemory)
  IMPORT_FUNC(vkCreateBufferView)
  IMPORT_FUNC(vkDestroyBufferView)
  IMPORT_FUNC(vkAllocateMemory)
  IMPORT_FUNC(vkFreeMemory)
  IMPORT_FUNC(vkGetBufferMemoryRequirements)
  IMPORT_FUNC(vkBindBufferMemory)
  IMPORT_FUNC(vkCreateRenderPass)
  IMPORT_FUNC(vkDestroyRenderPass)
  IMPORT_FUNC(vkCreateImage)
  IMPORT_FUNC(vkDestroyImage)
  IMPORT_FUNC(vkGetImageSubresourceLayout)
  IMPORT_FUNC(vkGetImageMemoryRequirements)
  IMPORT_FUNC(vkBindImageMemory)
  IMPORT_FUNC(vkCreateImageView)
  IMPORT_FUNC(vkDestroyImageView)
  IMPORT_FUNC(vkCreateSampler)
  IMPORT_FUNC(vkDestroySampler)
  IMPORT_FUNC(vkCreateSemaphore)
  IMPORT_FUNC(vkDestroySemaphore)
  IMPORT_FUNC(vkCreateFence)
  IMPORT_FUNC(vkDestroyFence)
  IMPORT_FUNC(vkWaitForFences)
  IMPORT_FUNC(vkCreateCommandPool)
  IMPORT_FUNC(vkDestroyCommandPool)
  IMPORT_FUNC(vkAllocateCommandBuffers)
  IMPORT_FUNC(vkFreeCommandBuffers)
  IMPORT_FUNC(vkBeginCommandBuffer)
  IMPORT_FUNC(vkEndCommandBuffer)
  IMPORT_FUNC(vkGetDeviceQueue)
  IMPORT_FUNC(vkQueueSubmit)
  IMPORT_FUNC(vkQueueWaitIdle)
  IMPORT_FUNC(vkDeviceWaitIdle)
  IMPORT_FUNC(vkCreateFramebuffer)
  IMPORT_FUNC(vkDestroyFramebuffer)
  IMPORT_FUNC(vkCreatePipelineCache)
  IMPORT_FUNC(vkDestroyPipelineCache)
  IMPORT_FUNC(vkCreatePipelineLayout)
  IMPORT_FUNC(vkDestroyPipelineLayout)
  IMPORT_FUNC(vkCreateGraphicsPipelines)
  IMPORT_FUNC(vkCreateComputePipelines)
  IMPORT_FUNC(vkDestroyPipeline)
  IMPORT_FUNC(vkCreateDescriptorPool)
  IMPORT_FUNC(vkDestroyDescriptorPool)
  IMPORT_FUNC(vkCreateDescriptorSetLayout)
  IMPORT_FUNC(vkDestroyDescriptorSetLayout)
  IMPORT_FUNC(vkAllocateDescriptorSets)
  IMPORT_FUNC(vkFreeDescriptorSets)
  IMPORT_FUNC(vkUpdateDescriptorSets)
  IMPORT_FUNC(vkCreateQueryPool)
  IMPORT_FUNC(vkDestroyQueryPool)
  IMPORT_FUNC(vkGetQueryPoolResults)
  IMPORT_FUNC(vkCmdBeginQuery)
  IMPORT_FUNC(vkCmdEndQuery)
  IMPORT_FUNC(vkCmdResetQueryPool)
  IMPORT_FUNC(vkCmdWriteTimestamp)
  IMPORT_FUNC(vkCmdCopyQueryPoolResults)
  IMPORT_FUNC(vkCmdPipelineBarrier)
  IMPORT_FUNC(vkCmdBeginRenderPass)
  IMPORT_FUNC(vkCmdEndRenderPass)
  IMPORT_FUNC(vkCmdExecuteCommands)
  IMPORT_FUNC(vkCmdCopyImage)
  IMPORT_FUNC(vkCmdBlitImage)
  IMPORT_FUNC(vkCmdCopyBufferToImage)
  IMPORT_FUNC(vkCmdClearAttachments)
  IMPORT_FUNC(vkCmdCopyBuffer)
  IMPORT_FUNC(vkCmdBindDescriptorSets)
  IMPORT_FUNC(vkCmdBindPipeline)
  IMPORT_FUNC(vkCmdSetViewport)
  IMPORT_FUNC(vkCmdSetScissor)
  IMPORT_FUNC(vkCmdSetLineWidth)
  IMPORT_FUNC(vkCmdSetDepthBias)
  IMPORT_FUNC(vkCmdPushConstants)
  IMPORT_FUNC(vkCmdBindIndexBuffer)
  IMPORT_FUNC(vkCmdBindVertexBuffers)
  IMPORT_FUNC(vkCmdDraw)
  IMPORT_FUNC(vkCmdDrawIndexed)
  IMPORT_FUNC(vkCmdDrawIndirect)
  IMPORT_FUNC(vkCmdDrawIndexedIndirect)
  IMPORT_FUNC(vkCmdDispatch)
  IMPORT_FUNC(vkCmdClearColorImage)
  IMPORT_FUNC(vkCmdClearDepthStencilImage)
  // "VK_KHR_swapchain"-extension
  IMPORT_FUNC(vkCreateSwapchainKHR)
  IMPORT_FUNC(vkDestroySwapchainKHR)
  IMPORT_FUNC(vkGetSwapchainImagesKHR)
  IMPORT_FUNC(vkAcquireNextImageKHR)
  IMPORT_FUNC(vkQueuePresentKHR)
  // "VK_NV_mesh_shader"-extension
  if (hasMeshShaderSupport)
  {
    IMPORT_FUNC(vkCmdDrawMeshTasksNV)
  }

#undef IMPORT_FUNC

  return result;
}


bool VulkanRuntimeLinking::load_shared_libraries() {
#if defined(LINUX)
  mVulkanSharedLibrary = ::dlopen("libvulkan.so", RTLD_NOW);
  if (mVulkanSharedLibrary == nullptr) {
    BE_LOG(Critical, "Failed to load the shared library libvulkan.so")
  }
#else
#error "Unsupported platform!"
#endif
  return (mVulkanSharedLibrary != nullptr);
}

bool VulkanRuntimeLinking::load_global_level_entry_points() const {

#if defined LINUX
#define IMPORT_FUNC(funcName)																																		\
					if (result)																																						\
					{																																								\
						void* symbol = ::dlsym(mVulkanSharedLibrary, #funcName);																									\
						if (nullptr != symbol)																																		\
						{																																							\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																										\
						}																																							\
						else																																						\
						{																																							\
							link_map *linkMap = nullptr;																															\
							const char* libraryName = "unknown";																													\
							if (dlinfo(mVulkanSharedLibrary, RTLD_DI_LINKMAP, &linkMap))																							\
							{																																						\
								libraryName = linkMap->l_name;																														\
							}																																						\
							BE_LOG(Critical, core::String("Failed to locate the following entry point: ") + #funcName)	\
							result = false;																																			\
						}																																							\
					}
#else
#error "Unsupported platform"
#endif

  bool result = true;

  // Load the Vulkan global level function entry points
  IMPORT_FUNC(vkGetInstanceProcAddr)
  IMPORT_FUNC(vkGetDeviceProcAddr)
  IMPORT_FUNC(vkEnumerateInstanceExtensionProperties)
  IMPORT_FUNC(vkEnumerateInstanceLayerProperties)
  IMPORT_FUNC(vkCreateInstance)

  // Undefine the helper macro
#undef IMPORT_FUNC

  return result;
}

bool VulkanRuntimeLinking::load_instance_level_entry_points() const {

#define IMPORT_FUNC(funcName)																										\
				if (result)																														\
				{																																\
					funcName = reinterpret_cast<PFN_##funcName>(vkGetInstanceProcAddr(mVkInstance, #funcName));									\
					if (nullptr == funcName)																									\
					{																															\
						BE_LOG(Critical, core::String("Failed to load instance based Vulkan function pointer ") + #funcName)	\
						result = false;																											\
					}																															\
				}

  bool result = true;

  // Load the Vulkan instance level function entry points
  IMPORT_FUNC(vkDestroyInstance)
  IMPORT_FUNC(vkEnumeratePhysicalDevices)
  IMPORT_FUNC(vkEnumerateDeviceLayerProperties)
  IMPORT_FUNC(vkEnumerateDeviceExtensionProperties)
  IMPORT_FUNC(vkGetPhysicalDeviceQueueFamilyProperties)
  IMPORT_FUNC(vkGetPhysicalDeviceFeatures)
  IMPORT_FUNC(vkGetPhysicalDeviceFormatProperties)
  IMPORT_FUNC(vkGetPhysicalDeviceMemoryProperties)
  IMPORT_FUNC(vkGetPhysicalDeviceProperties)
  IMPORT_FUNC(vkCreateDevice)
  if (mEnableValidation)
  {
    // "VK_EXT_debug_report"-extension
    IMPORT_FUNC(vkCreateDebugReportCallbackEXT)
    IMPORT_FUNC(vkDestroyDebugReportCallbackEXT)
  }
  // "VK_KHR_surface"-extension
  IMPORT_FUNC(vkDestroySurfaceKHR)
  IMPORT_FUNC(vkGetPhysicalDeviceSurfaceSupportKHR)
  IMPORT_FUNC(vkGetPhysicalDeviceSurfaceFormatsKHR)
  IMPORT_FUNC(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
  IMPORT_FUNC(vkGetPhysicalDeviceSurfacePresentModesKHR)
#if defined VK_USE_PLATFORM_XCB_KHR
  // "VK_KHR_xlib_surface"-extension
  //IMPORT_FUNC(vkCreateXcbSurfaceKHR)
#endif

#undef IMPORT_FUNC
  return result;
}

VkResult VulkanRuntimeLinking::create_vulkan_instance(bool enableValidation) {
  core::Vector<const char*> enabledExtensions;
  enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME); //= { VK_KHR_SURFACE_EXTENSION_NAME };
#if defined(VK_USE_PLATFORM_XCB_KHR)
  //#warning "TODO(naetherm) Not tested"
  enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#else
#error "Unsupported platform"
#endif
  // Also push in debug extension if in debug mode
  if (enableValidation) {
    enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  { // Ensure the extensions we need are supported
    core::uint32 propertyCount = 0;
    if ((vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr) != VK_SUCCESS) || (0 == propertyCount)) {
      BE_LOG(Critical, "Failed to enumerate Vulkan instance extension properties")
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
    ::internal::VkExtensionPropertiesVector vkExtensionPropertiesVector(propertyCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, &vkExtensionPropertiesVector[0]) != VK_SUCCESS) {
      BE_LOG(Critical, "Failed to enumerate Vulkan instance extension properties")
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
    for (const char* enabledExtension : enabledExtensions) {
      if (!VulkanHelper::is_extension_available(enabledExtension, vkExtensionPropertiesVector)) {
        BE_LOG(Critical, core::String("Couldn't find Vulkan instance extension named ") + enabledExtension)
        return VK_ERROR_EXTENSION_NOT_PRESENT;
      }
    }
  }

  // Create application information
  static constexpr VkApplicationInfo vkApplicationInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "RacoonEngine Application",
    .pEngineName = "RacoonEngine",
    .apiVersion = VK_API_VERSION_1_3};

  /*
const VkInstanceCreateInfo vkInstanceCreateInfo = {
  .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
  .pNext = nullptr,
  .pApplicationInfo = &vkApplicationInfo,
  .enabledExtensionCount = enabledExtensions.size(),
  .ppEnabledExtensionNames = enabledExtensions.data()
};
 */
  const VkInstanceCreateInfo vkInstanceCreateInfo = {
    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,							// sType (VkStructureType)
    nullptr,														// pNext (const void*)
    0,																// flags (VkInstanceCreateFlags)
    &vkApplicationInfo,												// pApplicationInfo (const VkApplicationInfo*)
    enableValidation ? ::internal::NUMBER_OF_VALIDATION_LAYERS : 0,	// enabledLayerCount (core::uint32)
    enableValidation ? ::internal::VALIDATION_LAYER_NAMES : nullptr,	// ppEnabledLayerNames (const char* const*)
    static_cast<core::uint32>(enabledExtensions.size()),				// enabledExtensionCount (core::uint32)
    enabledExtensions.data()										// ppEnabledExtensionNames (const char* const*)
  };

  //VkResult vkResult = vkCreateInstance(&vkInstanceCreateInfo, mRhiDynamicRhi.get_vk_allocation_callbacks(), &mVkInstance);
  VkResult vkResult = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &mVkInstance);

  if (VK_ERROR_LAYER_NOT_PRESENT == vkResult && enableValidation) {
    // Error! Since the show must go on, try creating a Vulkan instance without validation enabled...
    BE_LOG(Warning, "Failed to create the Vulkan instance with validation enabled, layer is not present. Install e.g. the LunarG Vulkan SDK and see e.g. https://vulkan.lunarg.com/doc/view/1.3.275.0/windows/layers.html .")
    mEnableValidation = false;
    vkResult = create_vulkan_instance(mEnableValidation);
  }

  volkLoadInstance(mVkInstance);

  // Done
  return vkResult;
}

void VulkanRuntimeLinking::setup_debug_callbacks() {
  // Sanity check
  BE_ASSERT(mEnableValidation, "Do only call this Vulkan method if validation is enabled")

  // The report flags determine what type of messages for the layers will be displayed
  // -> Use "VK_DEBUG_REPORT_FLAG_BITS_MAX_ENUM_EXT" to get everything, quite verbose
  static constexpr VkDebugReportFlagsEXT vkDebugReportFlagsEXT = (VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT);

  // Setup debug callback
  const VkDebugReportCallbackCreateInfoEXT vkDebugReportCallbackCreateInfoEXT = {
    VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,	// sType (VkStructureType)
    nullptr,													// pNext (const void*)
    vkDebugReportFlagsEXT,										// flags (VkDebugReportFlagsEXT)
    ::internal::debug_report_callback,								// pfnCallback (PFN_vkDebugReportCallbackEXT)
    nullptr			// pUserData (void*)
  };
  if (vkCreateDebugReportCallbackEXT(mVkInstance, &vkDebugReportCallbackCreateInfoEXT, mRhiDynamicRhi.get_vk_allocation_callbacks(), &mVkDebugReportCallbackEXT) != VK_SUCCESS) {
    BE_LOG(Warning, "Failed to create the Vulkan debug report callback")
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan