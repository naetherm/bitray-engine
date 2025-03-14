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
#include "rhi/rhi.h"
#include "rhi/buffer/rhibuffer_types.h"
#include "rhi/buffer/rhiindex_buffer_types.h"
#include "rhi/buffer/rhiindirect_buffer_types.h"
#include "rhi/buffer/rhivertex_array_types.h"
#include <core/core/refcounted.h>
#include <core/core/ptr.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class RHIDevice;
class RHIVertexBuffer;
class RHIVertexArray;
class RHIIndexBuffer;
class RHITextureBuffer;
class RHIStructuredBuffer;
class RHIIndirectBuffer;
class RHIUniformBuffer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract buffer manager interface
 *
 * @remarks
 * The buffer manager is responsible for managing fine granular instances of
 * - Vertex buffer object ("rhi::RHIVertexBuffer")
 * - Index buffer object ("rhi::RHIIndexBuffer")
 * - Vertex array object ("rhi::RHIVertexArray")
 * - Texture buffer object ("rhi::RHITextureBuffer")
 * - Structured buffer object ("rhi::RHIStructuredBuffer")
 * - Indirect buffer object ("rhi::RHIIndirectBuffer")
 * - Uniform buffer object ("rhi::RHIUniformBuffer")
 *
 * Implementations are free to implement a naive 1:1 mapping of a resource to an RHI implementation resource.
 * For AZDO ("Almost Zero Driver Overhead") implementations might allocate a few big RHI implementation resources
 * and manage the granular instances internally. Modern RHI implementations like Vulkan or DirectX 12 are build
 * around the concept that the user just allocates a few big memory heaps and then managing them by their own.
 */
class RHIBufferManager : public core::RefCounted {

  // Public methods
public:
  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHIBufferManager() override = default;

  /**
   * @brief
   * Return the owner RHI instance
   *
   * @return
   * The owner RHI instance, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] inline RHIDevice &get_rhi() const {
    return mRhi;
  }

  // Public virtual rhi::RHIBufferManager methods
public:
  //[-------------------------------------------------------]
  //[ Resource creation                                     ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Create a vertex buffer object (VBO, "array buffer" in OpenGL terminology) instance
   *
   * @param[in] numberOfBytes
   * Number of bytes within the vertex buffer, must be valid
   * @param[in] data
   * Vertex buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] bufferFlags
   * Buffer flags, see "rhi::BufferFlag"
   * @param[in] bufferUsage
   * Indication of the buffer usage
   *
   * @return
   * The created VBO instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHIVertexBuffer* create_vertex_buffer(VertexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create an index buffer object (IBO, "element array buffer" in OpenGL terminology) instance
   *
   * @param[in] numberOfBytes
   * Number of bytes within the index buffer, must be valid
   * @param[in] data
   * Index buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] bufferFlags
   * Buffer flags, see "rhi::BufferFlag"
   * @param[in] bufferUsage
   * Indication of the buffer usage
   * @param[in] indexBufferFormat
   * Index buffer data format
   *
   * @return
   * The created IBO instance, null pointer on error. release the returned instance if you no longer need it.
   */
  [[nodiscard]] virtual RHIIndexBuffer* create_index_buffer(rhi::IndexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create a vertex array instance
   *
   * @param[in] vertexAttributes
   * Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
   * @param[in] numberOfVertexBuffers
   * Number of vertex buffers, having zero vertex buffers is valid
   * @param[in] vertexBuffers
   * At least "numberOfVertexBuffers" instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] indexBuffer
   * Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
   *
   * @return
   * The created vertex array instance, null pointer on error. release the returned instance if you no longer need it.
   *
   * @note
   * - The created vertex array instance keeps a reference to the vertex buffers used by the vertex array attributes
   * - It's valid that a vertex array implementation is adding a reference and releasing it again at once
   * (this means that in the case of not having any more references, a vertex buffer might get destroyed when calling this method)
   */
  [[nodiscard]] virtual RHIVertexArray* create_vertex_array(rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create an texture buffer object (TBO) instance
   *
   * @param[in] numberOfBytes
   * Number of bytes within the texture buffer, must be valid
   * @param[in] data
   * Texture buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] bufferFlags
   * Buffer flags, see "rhi::BufferFlag"
   * @param[in] bufferUsage
   * Indication of the buffer usage
   * @param[in] textureFormat
   * Texture buffer data format
   *
   * @return
   * The created TBO instance, null pointer on error. release the returned instance if you no longer need it.
   *
   * @note
   * - Only supported if "rhi::Capabilities::maximumTextureBufferSize" is not 0
   */
  [[nodiscard]] virtual RHITextureBuffer* create_texture_buffer(rhi::TextureBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create an structured buffer object (SBO) instance
   *
   * @param[in] numberOfBytes
   * Number of bytes within the structured buffer, must be valid
   * @param[in] data
   * Structured buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] bufferFlags
   * Buffer flags, see "rhi::BufferFlag"
   * @param[in] bufferUsage
   * Indication of the buffer usage
   * @param[in] numberOfStructureBytes
   * Number of structure bytes
   *
   * @return
   * The created structured buffer instance, null pointer on error. release the returned instance if you no longer need it.
   *
   * @note
   * - Only supported if "rhi::Capabilities::maximumStructuredBufferSize" is not 0
   */
  [[nodiscard]] virtual RHIStructuredBuffer* create_structured_buffer(rhi::StructuredBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create an indirect buffer object instance
   *
   * @param[in] numberOfBytes
   * Number of bytes within the indirect buffer, must be valid
   * @param[in] data
   * Indirect buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] indirectBufferFlags
   * Indirect buffer flags, see "rhi::IndirectBufferFlag"
   * @param[in] bufferUsage
   * Indication of the buffer usage
   *
   * @return
   * The created indirect buffer instance, null pointer on error. release the returned instance if you no longer need it.
   *
   * @note
   * - Only supported if "rhi::Capabilities::maximumIndirectBufferSize" is >0
   */
  [[nodiscard]] virtual RHIIndirectBuffer* create_indirect_buffer(rhi::IndirectBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
   * @brief
   * Create an uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instance
   *
   * @param[in] numberOfBytes
   * Number of bytes within the uniform buffer, must be valid
   * @param[in] data
   * Uniform buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
   * @param[in] bufferUsage
   * Indication of the buffer usage
   *
   * @return
   * The created UBO instance, null pointer on error. release the returned instance if you no longer need it.
   *
   * @note
   * - Only supported if "rhi::Capabilities::maximumUniformBufferSize" is >0
   * - There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
   */
  [[nodiscard]] virtual RHIUniformBuffer* create_uniform_buffer(rhi::UniformBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  // Protected methods
protected:
  /**
   * @brief
   * Constructor
   *
   * @param[in] rhi
   * Owner RHI instance
   */
  inline explicit RHIBufferManager(RHIDevice &rhi)
  : mRhi(rhi) {

  }

  explicit RHIBufferManager(const RHIBufferManager &source) = delete;

  RHIBufferManager &operator=(const RHIBufferManager &source) = delete;

  // Private data
private:
  /** The owner RHI instance */
  RHIDevice &mRhi;

};

typedef core::Ptr<RHIBufferManager> RHIBufferManagerPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
