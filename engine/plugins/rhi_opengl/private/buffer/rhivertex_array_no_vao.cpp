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
#include "rhi_opengl/buffer/rhivertex_array_no_vao.h"
#include "rhi_opengl/buffer/rhiindex_buffer.h"
#include "rhi_opengl/buffer/rhivertex_buffer.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIVertexArrayNoVao::RHIVertexArrayNoVao(RHIDevice& openGLRhi, rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIVertexArray(openGLRhi, descriptor, InternalResourceType::NO_VAO RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mNumberOfAttributes(descriptor.vertexAttributes.numberOfAttributes)
, mAttributes(mNumberOfAttributes ? re_typed_alloc<rhi::VertexAttribute>(mNumberOfAttributes) : nullptr)
, mNumberOfVertexBuffers(descriptor.numberOfVertexBuffers)
, mVertexBuffers(descriptor.numberOfVertexBuffers ? re_typed_alloc<rhi::VertexArrayVertexBuffer>(descriptor.numberOfVertexBuffers) : nullptr)
, mIsGL_ARB_instanced_arrays(openGLRhi.get_extensions().isGL_ARB_instanced_arrays()) {
  // Copy over the data
  if (nullptr != mAttributes) {
    memcpy(mAttributes, descriptor.vertexAttributes.attributes, sizeof(rhi::VertexAttribute) * mNumberOfAttributes);
  }
  if (nullptr != mVertexBuffers) {
    memcpy(mVertexBuffers, descriptor.vertexBuffers, sizeof(rhi::VertexArrayVertexBuffer) * mNumberOfVertexBuffers);
  }

  // Add a reference to the used vertex buffers
  const rhi::VertexArrayVertexBuffer* vertexBufferEnd = mVertexBuffers + mNumberOfVertexBuffers;
  for (const rhi::VertexArrayVertexBuffer* vertexBuffer = mVertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer) {
    vertexBuffer->vertexBuffer->add_ref();
  }
}


RHIVertexArrayNoVao::~RHIVertexArrayNoVao() {
  // Destroy the vertex array attributes
  const rhi::RHIContext& context = get_rhi().get_context();
  re_free(mAttributes);

  // Destroy the vertex array vertex buffers
  if (nullptr != mVertexBuffers) {
    // release the reference to the used vertex buffers
    const rhi::VertexArrayVertexBuffer* vertexBufferEnd = mVertexBuffers + mNumberOfVertexBuffers;
    for (const rhi::VertexArrayVertexBuffer* vertexBuffer = mVertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer) {
      vertexBuffer->vertexBuffer->release();
    }

    // Cleanup
    re_free(mVertexBuffers);
  }
}

void RHIVertexArrayNoVao::enable_opengl_vertex_attrib_arrays() {
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently bound OpenGL array buffer
  // -> Using "GL_EXT_direct_state_access" this would not help in here because "glVertexAttribPointerARB" is not specified there :/
  GLint openGLArrayBufferBackup = 0;
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &openGLArrayBufferBackup);
#endif

  // Loop through all attributes
  // -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
  GLuint attributeLocation = 0;
  const rhi::VertexAttribute* attributeEnd = mAttributes + mNumberOfAttributes;
  for (const rhi::VertexAttribute* attribute = mAttributes; attribute < attributeEnd; ++attribute, ++attributeLocation) {
    // Set the OpenGL vertex attribute pointer
    // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
    const rhi::VertexArrayVertexBuffer& vertexArrayVertexBuffer = mVertexBuffers[attribute->inputSlot];
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<RHIVertexBuffer *>(vertexArrayVertexBuffer.vertexBuffer)->get_opengl_array_buffer());
    if (Mapping::is_opengl_vertex_attribute_format_integer(attribute->vertexAttributeFormat)) {
      glVertexAttribIPointer(attributeLocation,
                             Mapping::get_opengl_size(attribute->vertexAttributeFormat),
                             Mapping::get_opengl_type(attribute->vertexAttributeFormat),
                             static_cast<GLsizei>(attribute->strideInBytes),
                             reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
    } else {
      glVertexAttribPointerARB(attributeLocation,
                               Mapping::get_opengl_size(attribute->vertexAttributeFormat),
                               Mapping::get_opengl_type(attribute->vertexAttributeFormat),
                               static_cast<GLboolean>(Mapping::is_opengl_vertex_attribute_format_normalized(
                                 attribute->vertexAttributeFormat)),
                               static_cast<GLsizei>(attribute->strideInBytes),
                               reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
    }

    // Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
    if (attribute->instancesPerElement > 0 && mIsGL_ARB_instanced_arrays) {
      glVertexAttribDivisorARB(attributeLocation, attribute->instancesPerElement);
    }

    // Enable OpenGL vertex attribute array
    glEnableVertexAttribArrayARB(attributeLocation);
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL array buffer
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLArrayBufferBackup));
#endif

  // Set the used index buffer
  // -> In case of no index buffer we don't bind buffer 0, there's not really a point in it
  const RHIIndexBuffer* indexBuffer = get_index_buffer();
  if (nullptr != indexBuffer) {
    // Bind OpenGL element array buffer
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer->get_opengl_element_array_buffer());
  }
}

void RHIVertexArrayNoVao::disable_opengl_vertex_attrib_arrays() {
  // No previous bound OpenGL element array buffer restore, there's not really a point in it

  // Loop through all attributes
  // -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
  GLuint attributeLocation = 0;
  const rhi::VertexAttribute* attributeEnd = mAttributes + mNumberOfAttributes;
  for (const rhi::VertexAttribute* attribute = mAttributes; attribute < attributeEnd; ++attribute, ++attributeLocation) {
    // Disable OpenGL vertex attribute array
    glDisableVertexAttribArrayARB(attributeLocation);

    // Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
    if (attribute->instancesPerElement > 0 && mIsGL_ARB_instanced_arrays) {
      glVertexAttribDivisorARB(attributeLocation, 0);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
