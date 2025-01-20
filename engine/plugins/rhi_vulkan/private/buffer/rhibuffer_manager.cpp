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
#include "rhi_vulkan/buffer/rhibuffer_manager.h"
#include "rhi_vulkan/buffer/rhiindex_buffer.h"
#include "rhi_vulkan/buffer/rhiindirect_buffer.h"
#include "rhi_vulkan/buffer/rhistructured_buffer.h"
#include "rhi_vulkan/buffer/rhitexture_buffer.h"
#include "rhi_vulkan/buffer/rhiuniform_buffer.h"
#include "rhi_vulkan/buffer/rhivertex_array.h"
#include "rhi_vulkan/buffer/rhivertex_buffer.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIBufferManager::RHIBufferManager(RHIDevice& vulkanRhi) :
rhi::RHIBufferManager(vulkanRhi) {
  
}

RHIBufferManager::~RHIBufferManager() {
  
}


rhi::RHIVertexBuffer* RHIBufferManager::create_vertex_buffer(rhi::VertexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
  return new RHIVertexBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIIndexBuffer* RHIBufferManager::create_index_buffer(rhi::IndexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
  return new RHIIndexBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIVertexArray* RHIBufferManager::create_vertex_array(rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity checks
#ifdef DEBUG
  {
    const rhi::VertexArrayVertexBuffer* vertexBufferEnd = descriptor.vertexBuffers + descriptor.numberOfVertexBuffers;
    for (const rhi::VertexArrayVertexBuffer* vertexBuffer = descriptor.vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer) {
      BE_ASSERT(&vulkanRhi == &vertexBuffer->vertexBuffer->get_rhi(), "Vulkan error: The given vertex buffer resource is owned by another RHI instance")
    }
  }
#endif
  BE_ASSERT(nullptr == descriptor.indexBuffer || &vulkanRhi == &descriptor.indexBuffer->get_rhi(), "Vulkan error: The given index buffer resource is owned by another RHI instance")

  // Create vertex array
  core::uint16 id;
  if (vulkanRhi.VertexArrayMakeId.create_id(id)) {
    return new RHIVertexArray(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error: Ensure a correct reference counter behaviour
  const rhi::VertexArrayVertexBuffer* vertexBufferEnd = descriptor.vertexBuffers + descriptor.numberOfVertexBuffers;
  for (const rhi::VertexArrayVertexBuffer* vertexBuffer = descriptor.vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer) {
    vertexBuffer->vertexBuffer->add_ref();
    vertexBuffer->vertexBuffer->release();
  }
  if (nullptr != descriptor.indexBuffer) {
    descriptor.indexBuffer->add_ref();
    descriptor.indexBuffer->release();
  }
  return nullptr;
}

rhi::RHITextureBuffer* RHIBufferManager::create_texture_buffer(rhi::TextureBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
  return new RHITextureBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIStructuredBuffer* RHIBufferManager::create_structured_buffer(rhi::StructuredBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
  return new RHIStructuredBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIIndirectBuffer* RHIBufferManager::create_indirect_buffer(rhi::IndirectBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());
  return new RHIIndirectBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIUniformBuffer* RHIBufferManager::create_uniform_buffer(rhi::UniformBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& vulkanRhi = static_cast<RHIDevice&>(get_rhi());

  // Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
  // -> Inside GLSL "layout(binding, std140) writeonly uniform OutputUniformBuffer" will result in the GLSL compiler error "Failed to parse the GLSL shader source code: ERROR: 0:85: 'assign' :  l-value required "anon@6" (can't modify a uniform)"
  // -> Inside GLSL "layout(binding, std430) writeonly buffer  OutputUniformBuffer" will work in OpenGL but will fail in Vulkan with "Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT" Object: "0" Location: "0" Message code: "13" Layer prefix: "Validation" Message: "Object: VK_NULL_HANDLE (Type) | Type mismatch on descriptor slot 0.0 (used as type `ptr to uniform struct of (vec4 of float32)`) but descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER""
  // BE_ASSERT((bufferFlags & rhi::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid Vulkan buffer flags, uniform buffer can't be used for unordered access")
  // BE_ASSERT((bufferFlags & rhi::BufferFlag::SHADER_RESOURCE) != 0, "Invalid Vulkan buffer flags, uniform buffer must be used as shader resource")

  // Create the uniform buffer
  return new RHIUniformBuffer(vulkanRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

void RHIBufferManager::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
