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
RHIVertexBufferDsa::RHIVertexBufferDsa(RHIDevice &openGLRhi, rhi::VertexBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIVertexBuffer(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
  if (openGLRhi.get_extensions().isGL_ARB_direct_state_access()) {
    // Create the OpenGL array buffer
    glCreateBuffers(1, &mOpenGLArrayBuffer);

    // Upload the data
    // -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
    glNamedBufferData(mOpenGLArrayBuffer, static_cast<GLsizeiptr>(descriptor.numberOfBytes), descriptor.data, static_cast<GLenum>(descriptor.bufferUsage));
  } else {
    // Create the OpenGL array buffer
    glGenBuffersARB(1, &mOpenGLArrayBuffer);

    // Upload the data
    // -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
    glNamedBufferDataEXT(mOpenGLArrayBuffer, static_cast<GLsizeiptr>(descriptor.numberOfBytes), descriptor.data, static_cast<GLenum>(descriptor.bufferUsage));
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VBO", 6)	// 6 = "VBO: " including terminating zero
    glObjectLabel(GL_BUFFER, mOpenGLArrayBuffer, -1, detailedDebugName);
  }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
