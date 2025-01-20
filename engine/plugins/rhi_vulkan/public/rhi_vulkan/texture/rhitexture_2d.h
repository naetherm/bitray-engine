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
 *  @brief
 *    OpenVR-support: Data required for passing Vulkan textures to IVRCompositor::Submit; Be sure to call OpenVR_Shutdown before destroying these resources
 *
 *  @note
 *    - From OpenVR SDK 1.0.7 "openvr.h"-header
 */
struct VRVulkanTextureData_t
{
  VkImage			 m_nImage;
  VkDevice		 m_pDevice;
  VkPhysicalDevice m_pPhysicalDevice;
  VkInstance		 m_pInstance;
  VkQueue			 m_pQueue;
  core::uint32		 m_nQueueFamilyIndex;
  core::uint32		 m_nWidth;
  core::uint32		 m_nHeight;
  VkFormat		 m_nFormat;
  core::uint32		 m_nSampleCount;
};

/**
 *  @brief
 *    Vulkan 2D texture interface
 */
class RHITexture2D final : public rhi::RHITexture2D
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   *  @brief
   *    Constructor
   *
   *  @param[in] vulkanRhi
   *    Owner Vulkan RHI instance
   *  @param[in] width
   *    Texture width, must be >0
   *  @param[in] height
   *    Texture height, must be >0
   *  @param[in] textureFormat
   *    Texture format
   *  @param[in] data
   *    Texture data, can be a null pointer
   *  @param[in] textureFlags
   *    Texture flags, see "rhi::TextureFlag::Enum"
   *  @param[in] numberOfMultisamples
   *    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
   */
  RHITexture2D(RHIDevice& vulkanRhi, rhi::TextureDescriptor descriptor, const void* data RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   *  @brief
   *    Destructor
   */
  ~RHITexture2D() override;

  /**
   *  @brief
   *    Return the Vulkan image view
   *
   *  @return
   *    The Vulkan image view
   */
  [[nodiscard]] VkImageView get_vk_image_view() const;

  /**
   *  @brief
   *    Return the Vulkan image layout
   *
   *  @return
   *    The Vulkan image layout
   */
  [[nodiscard]] VkImageLayout get_vk_image_layout() const;

  /**
   *  @brief
   *    Return the Vulkan format
   *
   *  @return
   *    The Vulkan format
   */
  [[nodiscard]] VkFormat get_vk_format() const;

  /**
   *  @brief
   *    Set minimum maximum mipmap index
   *
   *  @param[in] minimumMipmapIndex
   *    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
   *  @param[in] maximumMipmapIndex
   *    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
   */
  void set_minimum_maximum_mipmap_index([[maybe_unused]] core::uint32 minimumMipmapIndex, [[maybe_unused]] core::uint32 maximumMipmapIndex);


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIResource methods                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] void* get_internal_resource_handle() const override;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void self_destruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHITexture2D(const RHITexture2D& source) = delete;
  RHITexture2D& operator =(const RHITexture2D& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  VRVulkanTextureData_t mVrVulkanTextureData;
  VkImageLayout		  mVkImageLayout;
  VkDeviceMemory		  mVkDeviceMemory;
  VkImageView			  mVkImageView;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
