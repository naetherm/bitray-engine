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
#include "rhi_opengl/buffer/rhibuffer_manager.h"
#include "rhi_opengl/buffer/rhiindex_buffer_bind.h"
#include "rhi_opengl/buffer/rhiindex_buffer_dsa.h"
#include "rhi_opengl/buffer/rhiindirect_buffer_bind.h"
#include "rhi_opengl/buffer/rhiindirect_buffer_dsa.h"
#include "rhi_opengl/buffer/rhistructured_buffer_bind.h"
#include "rhi_opengl/buffer/rhistructured_buffer_dsa.h"
#include "rhi_opengl/buffer/rhitexture_buffer_bind.h"
#include "rhi_opengl/buffer/rhitexture_buffer_dsa.h"
#include "rhi_opengl/buffer/rhiuniform_buffer_bind.h"
#include "rhi_opengl/buffer/rhiuniform_buffer_dsa.h"
#include "rhi_opengl/buffer/rhivertex_array_no_vao.h"
#include "rhi_opengl/buffer/rhivertex_array_vao_bind.h"
#include "rhi_opengl/buffer/rhivertex_array_vao_dsa.h"
#include "rhi_opengl/buffer/rhivertex_buffer_bind.h"
#include "rhi_opengl/buffer/rhivertex_buffer_dsa.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIBufferManager::RHIBufferManager(RHIDevice& openGLRhi)
: rhi::RHIBufferManager(openGLRhi)
, mExtensions(&openGLRhi.get_extensions()) {
  
}

rhi::RHIVertexBuffer* RHIBufferManager::create_vertex_buffer(rhi::VertexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // "GL_ARB_vertex_buffer_object" required
  if (mExtensions->isGL_ARB_vertex_buffer_object()) {
    // Is "GL_EXT_direct_state_access" there?
    RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      return re_new<RHIVertexBufferDsa>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      return re_new<RHIVertexBufferBind>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    // Error!
    return nullptr;
  }
}

rhi::RHIIndexBuffer* RHIBufferManager::create_index_buffer(rhi::IndexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // "GL_ARB_vertex_buffer_object" required
  if (mExtensions->isGL_ARB_vertex_buffer_object()) {
    // Is "GL_EXT_direct_state_access" there?
    RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      return re_new<RHIIndexBufferDsa>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      return re_new<RHIIndexBufferBind>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    // Error!
    return nullptr;
  }
}

rhi::RHIVertexArray* RHIBufferManager::create_vertex_array(rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity checks
#ifdef DEBUG
  {
    const rhi::VertexArrayVertexBuffer* vertexBufferEnd = descriptor.vertexBuffers + descriptor.numberOfVertexBuffers;
    for (const rhi::VertexArrayVertexBuffer* vertexBuffer = descriptor.vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer) {
      BE_ASSERT(&openGLRhi == &vertexBuffer->vertexBuffer->get_rhi(), "OpenGL error: The given vertex buffer resource is owned by another RHI instance")
    }
  }
#endif
  BE_ASSERT(nullptr == descriptor.indexBuffer || &openGLRhi == &descriptor.indexBuffer->get_rhi(), "OpenGL error: The given index buffer resource is owned by another RHI instance")

  // Create vertex array
  if (openGLRhi.VertexArrayMakeId.create_id(descriptor.id)) {
    // Is "GL_ARB_vertex_array_object" there?
    if (mExtensions->isGL_ARB_vertex_array_object()) {
      // Effective vertex array object (VAO)

      // Is "GL_EXT_direct_state_access" there?
      if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
        // Effective direct state access (DSA)
        return re_new<RHIVertexArrayVaoDsa>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
      } else {
        // Traditional bind version
        return re_new<RHIVertexArrayVaoBind>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
      }
    } else {
      // Traditional version
      return re_new<RHIVertexArrayNoVao>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
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
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT((descriptor.numberOfBytes % rhi::TextureFormat::getNumberOfBytesPerElement(descriptor.textureFormat)) == 0, "The OpenGL texture buffer size must be a multiple of the selected texture format bytes per texel")

  // "GL_ARB_texture_buffer_object" required
  if (mExtensions->isGL_ARB_texture_buffer_object()) {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      return re_new<RHITextureBufferDsa>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      return re_new<RHITextureBufferBind>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    // Error!
    return nullptr;
  }
}

rhi::RHIStructuredBuffer* RHIBufferManager::create_structured_buffer(rhi::StructuredBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity checks
  BE_ASSERT((descriptor.numberOfBytes % descriptor.numberOfStructuredBytes) == 0, "The OpenGL structured buffer size must be a multiple of the given number of structure bytes")
  BE_ASSERT((descriptor.numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The OpenGL structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

  // "GL_ARB_shader_storage_buffer_object" required
  if (mExtensions->isGL_ARB_shader_storage_buffer_object()) {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      return re_new<RHIStructuredBufferDsa>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      return re_new<RHIStructuredBufferBind>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    // Error!
    return nullptr;
  }
}

rhi::RHIIndirectBuffer* RHIBufferManager::create_indirect_buffer(rhi::IndirectBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity checks
  BE_ASSERT((descriptor.flags & rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 || (descriptor.flags & rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0, "Invalid OpenGL flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" is missing")
  BE_ASSERT(!((descriptor.flags & rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 && (descriptor.flags & rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0), "Invalid OpenGL flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" must be set, but not both at one and the same time")
  BE_ASSERT((descriptor.flags & rhi::IndirectBufferFlag::DRAW_ARGUMENTS) == 0 || (descriptor.numberOfBytes % sizeof(rhi::DrawArguments)) == 0, "OpenGL indirect buffer element type flags specification is \"DRAW_ARGUMENTS\" but the given number of bytes don't align to this")
  BE_ASSERT((descriptor.flags & rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) == 0 || (descriptor.numberOfBytes % sizeof(rhi::DrawIndexedArguments)) == 0, "OpenGL indirect buffer element type flags specification is \"DRAW_INDEXED_ARGUMENTS\" but the given number of bytes don't align to this")

  // "GL_ARB_draw_indirect" required
  if (mExtensions->isGL_ARB_draw_indirect()) {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      return re_new<RHIIndirectBufferDsa>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      return re_new<RHIIndirectBufferBind>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    // Error!
    return nullptr;
  }
}

rhi::RHIUniformBuffer* RHIBufferManager::create_uniform_buffer(rhi::UniformBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // "GL_ARB_uniform_buffer_object" required
  if (mExtensions->isGL_ARB_uniform_buffer_object()) {
    RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

    // Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
    // -> Inside GLSL "layout(binding, std140) writeonly uniform OutputUniformBuffer" will result in the GLSL compiler error "Failed to parse the GLSL shader source code: ERROR: 0:85: 'assign' :  l-value required "anon@6" (can't modify a uniform)"
    // -> Inside GLSL "layout(binding, std430) writeonly buffer  OutputUniformBuffer" will work in OpenGL but will fail in Vulkan with "Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT" Object: "0" Location: "0" Message code: "13" Layer prefix: "Validation" Message: "Object: VK_NULL_HANDLE (Type) | Type mismatch on descriptor slot 0.0 (used as type `ptr to uniform struct of (vec4 of float32)`) but descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER""
    // BE_ASSERT((bufferFlags & rhi::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid OpenGL buffer flags, uniform buffer can't be used for unordered access")
    // BE_ASSERT((bufferFlags & rhi::BufferFlag::SHADER_RESOURCE) != 0, "Invalid OpenGL buffer flags, uniform buffer must be used as shader resource")

    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      return re_new<RHIUniformBufferDsa>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      return re_new<RHIUniformBufferBind>(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    // Error!
    return nullptr;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
