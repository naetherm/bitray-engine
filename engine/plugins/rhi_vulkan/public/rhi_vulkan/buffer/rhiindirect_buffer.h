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
 *    Vulkan indirect buffer object interface
 */
class RHIIndirectBuffer final : public rhi::RHIIndirectBuffer
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
   *  @param[in] numberOfBytes
   *    Number of bytes within the indirect buffer, must be valid
   *  @param[in] data
   *    Indirect buffer data, can be a null pointer (empty buffer)
   *  @param[in] indirectBufferFlags
   *    Indirect buffer flags, see "rhi::IndirectBufferFlag"
   *  @param[in] bufferUsage
   *    Indication of the buffer usage
   */
  RHIIndirectBuffer(RHIDevice& vulkanRhi, rhi::IndirectBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   *  @brief
   *    Destructor
   */
  ~RHIIndirectBuffer() override;

  /**
   *  @brief
   *    Return the Vulkan indirect buffer
   *
   *  @return
   *    The Vulkan indirect buffer
   */
  [[nodiscard]] VkBuffer get_vk_buffer() const;

  /**
   *  @brief
   *    Return the Vulkan device memory
   *
   *  @return
   *    The Vulkan device memory
   */
  [[nodiscard]] VkDeviceMemory get_vk_device_memory() const;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIIndirectBuffer methods           ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] const core::uint8* get_emulation_data() const override;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  void self_destruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIIndirectBuffer(const RHIIndirectBuffer& source) = delete;
  RHIIndirectBuffer& operator =(const RHIIndirectBuffer& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** Vulkan indirect buffer */
  VkBuffer	   mVkBuffer;
  /** Vulkan indirect memory */
  VkDeviceMemory mVkDeviceMemory;	
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
