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
#include "rhi_vulkan/buffer/rhivertex_array.h"
#include "rhi_vulkan/buffer/rhiindex_buffer.h"
#include "rhi_vulkan/buffer/rhivertex_buffer.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIVertexArray::RHIVertexArray(RHIDevice &vulkanRhi, rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIVertexArray(static_cast<rhi::RHIDevice &>(vulkanRhi), descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mIndexBuffer(reinterpret_cast<rhi_vulkan::RHIIndexBuffer*>(descriptor.indexBuffer))
, mNumberOfSlots(descriptor.numberOfVertexBuffers)
, mVertexVkBuffers(nullptr)
, mStrides(nullptr)
, mOffsets(nullptr)
, mVertexBuffers(nullptr) {
  // Add a reference to the given index buffer
  if (nullptr != mIndexBuffer) {
    mIndexBuffer->add_ref();
  }

  // Add a reference to the used vertex buffers
  if (mNumberOfSlots > 0) {
    const rhi::RHIContext &context = vulkanRhi.get_context();
    mVertexVkBuffers = re_typed_alloc<VkBuffer>(mNumberOfSlots);
    mStrides = re_typed_alloc<core::uint32>(mNumberOfSlots);
    mOffsets = re_typed_alloc<VkDeviceSize>(mNumberOfSlots);
    memset(mOffsets, 0, sizeof(VkDeviceSize) *
                        mNumberOfSlots);  // Vertex buffer offset is not supported by OpenGL, so our RHI implementation doesn't support it either, set everything to zero
    mVertexBuffers = re_typed_alloc<RHIVertexBuffer*>(mNumberOfSlots);

    { // Loop through all vertex buffers
      VkBuffer *currentVertexVkBuffer = mVertexVkBuffers;
      RHIVertexBuffer **currentVertexBuffer = mVertexBuffers;
      const rhi::VertexArrayVertexBuffer *vertexBufferEnd = descriptor.vertexBuffers + mNumberOfSlots;
      for (const rhi::VertexArrayVertexBuffer *vertexBuffer = descriptor.vertexBuffers;
           vertexBuffer < vertexBufferEnd; ++vertexBuffer, ++currentVertexVkBuffer, ++currentVertexBuffer) {
        // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
        *currentVertexBuffer = static_cast<RHIVertexBuffer *>(vertexBuffer->vertexBuffer);
        *currentVertexVkBuffer = (*currentVertexBuffer)->get_vk_buffer();
        (*currentVertexBuffer)->add_ref();
      }
    }

    { // Gather slot related data
      const rhi::VertexAttribute *attribute = descriptor.vertexAttributes.attributes;
      const rhi::VertexAttribute *attributesEnd = attribute + descriptor.vertexAttributes.numberOfAttributes;
      for (; attribute < attributesEnd; ++attribute) {
        mStrides[attribute->inputSlot] = attribute->strideInBytes;
      }
    }
  }
}

RHIVertexArray::~RHIVertexArray() {
  // Release the index buffer reference
  if (nullptr != mIndexBuffer) {
    mIndexBuffer->release();
  }

  // Cleanup Vulkan input slot data
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  const rhi::RHIContext &context = vulkanRhi.get_context();
  if (mNumberOfSlots > 0) {
    re_free(mVertexVkBuffers);
    re_free(mStrides);
    re_free(mOffsets);
  }

  // Release the reference to the used vertex buffers
  if (nullptr != mVertexBuffers) {
    // Release references
    RHIVertexBuffer **vertexBuffersEnd = mVertexBuffers + mNumberOfSlots;
    for (RHIVertexBuffer **vertexBuffer = mVertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer) {
      (*vertexBuffer)->release();
    }

    // Cleanup
    re_free(mVertexBuffers);
  }

  // Free the unique compact vertex array ID
  vulkanRhi.VertexArrayMakeId.destroy_id(get_id());
}

void RHIVertexArray::bind_vulkan_buffers(VkCommandBuffer vkCommandBuffer) const {
  // Set the Vulkan vertex buffers
  if (nullptr != mVertexVkBuffers) {
    vkCmdBindVertexBuffers(vkCommandBuffer, 0, mNumberOfSlots, mVertexVkBuffers, mOffsets);
  } else {
    // Do nothing since the Vulkan specification says "bindingCount must be greater than 0"
    // vkCmdBindVertexBuffers(vkCommandBuffer, 0, 0, nullptr, nullptr);
  }

  // Set the used index buffer
  // -> In case of no index buffer we don't set null indices, there's not really a point in it
  if (nullptr != mIndexBuffer) {
    vkCmdBindIndexBuffer(vkCommandBuffer, mIndexBuffer->get_vk_buffer(), 0, mIndexBuffer->get_vk_index_type());
  }
}


RHIIndexBuffer* RHIVertexArray::get_index_buffer() const {
  return mIndexBuffer;
}

void RHIVertexArray::self_destruct() override {
  delete this;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
