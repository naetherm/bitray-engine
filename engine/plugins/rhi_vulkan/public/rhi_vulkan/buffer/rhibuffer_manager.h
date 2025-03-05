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
 *    Vulkan buffer manager interface
 */
class RHIBufferManager final : public rhi::RHIBufferManager {


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
   */
  explicit RHIBufferManager(RHIDevice &vulkanRhi);

  /**
   *  @brief
   *    Destructor
   */
  ~RHIBufferManager() override;


  //[-------------------------------------------------------]
  //[ Public rhi::RHIBufferManager methods            ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] rhi::RHIVertexBuffer *create_vertex_buffer(rhi::VertexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] rhi::RHIIndexBuffer *create_index_buffer(rhi::IndexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] rhi::RHIVertexArray *create_vertex_array(rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] rhi::RHITextureBuffer *create_texture_buffer(rhi::TextureBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] rhi::RHIStructuredBuffer *create_structured_buffer(rhi::StructuredBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] rhi::RHIIndirectBuffer *create_indirect_buffer(rhi::IndirectBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] rhi::RHIUniformBuffer *create_uniform_buffer(rhi::UniformBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  void self_destruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIBufferManager(const RHIBufferManager &source) = delete;

  RHIBufferManager &operator=(const RHIBufferManager &source) = delete;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
