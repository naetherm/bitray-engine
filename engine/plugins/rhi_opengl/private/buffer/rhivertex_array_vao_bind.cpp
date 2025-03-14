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
#include "rhi_opengl/buffer/rhivertex_array_vao_bind.h"
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
RHIVertexArrayVaoBind::RHIVertexArrayVaoBind(RHIDevice& openGLRhi, rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIVertexArrayVao(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
  // Vertex buffer reference handling is done within the base class "VertexArrayVao"

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently bound OpenGL array buffer
  GLint openGLArrayBufferBackup = 0;
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &openGLArrayBufferBackup);

  // Backup the currently bound OpenGL element array buffer
  GLint openGLElementArrayBufferBackup = 0;
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, &openGLElementArrayBufferBackup);

  // Backup the currently bound OpenGL vertex array
  GLint openGLVertexArrayBackup = 0;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &openGLVertexArrayBackup);
#endif

  // Create the OpenGL vertex array
  glGenVertexArrays(1, &mOpenGLVertexArray);

  // Bind this OpenGL vertex array
  glBindVertexArray(mOpenGLVertexArray);

  // Loop through all attributes
  // -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
  GLuint attributeLocation = 0;
  const rhi::VertexAttribute* attributeEnd = descriptor.vertexAttributes.attributes + descriptor.vertexAttributes.numberOfAttributes;
  for (const rhi::VertexAttribute* attribute = descriptor.vertexAttributes.attributes; attribute < attributeEnd; ++attribute, ++attributeLocation) {
    // Set the OpenGL vertex attribute pointer
    // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
    const rhi::VertexArrayVertexBuffer& vertexArrayVertexBuffer = descriptor.vertexBuffers[attribute->inputSlot];
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
      static_cast<GLboolean>(Mapping::is_opengl_vertex_attribute_format_normalized(attribute->vertexAttributeFormat)),
      static_cast<GLsizei>(attribute->strideInBytes),
      reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
    }

    // Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
    if (attribute->instancesPerElement > 0 && openGLRhi.get_extensions().isGL_ARB_instanced_arrays()) {
      glVertexAttribDivisorARB(attributeLocation, attribute->instancesPerElement);
    }

    // Enable OpenGL vertex attribute array
    glEnableVertexAttribArrayARB(attributeLocation);
  }

  // Check the used index buffer
  // -> In case of no index buffer we don't bind buffer 0, there's not really a point in it
  if (nullptr != descriptor.indexBuffer) {
    // Bind OpenGL element array buffer
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, get_index_buffer()->get_opengl_element_array_buffer());
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL vertex array
  glBindVertexArray(static_cast<GLuint>(openGLVertexArrayBackup));

  // Be polite and restore the previous bound OpenGL element array buffer
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLElementArrayBufferBackup));

  // Be polite and restore the previous bound OpenGL array buffer
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLArrayBufferBackup));
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VAO", 6)	// 6 = "VAO: " including terminating zero
    glObjectLabel(GL_VERTEX_ARRAY, mOpenGLVertexArray, -1, detailedDebugName);
  }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
