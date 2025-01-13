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
#include "rhi_opengl/buffer/rhistructured_buffer_bind.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIStructuredBufferBind::RHIStructuredBufferBind(RHIDevice& openGLRhi, rhi::StructuredBufferDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIStructuredBuffer(openGLRhi, descriptor RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently bound OpenGL structured buffer
  GLint openGLStructuredBufferBackup = 0;
  glGetIntegerv(GL_SHADER_STORAGE_BUFFER_BINDING, &openGLStructuredBufferBackup);
#endif

  // Create the OpenGL structured buffer
  glGenBuffersARB(1, &mOpenGLStructuredBuffer);

  // Bind this OpenGL structured buffer and upload the data
  // -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
  glBindBufferARB(GL_SHADER_STORAGE_BUFFER, mOpenGLStructuredBuffer);
  glBufferDataARB(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptrARB>(descriptor.numberOfBytes), descriptor.data, static_cast<GLenum>(descriptor.bufferUsage));

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL structured buffer
  glBindBufferARB(GL_SHADER_STORAGE_BUFFER, static_cast<GLuint>(openGLStructuredBufferBackup));
#endif

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "SBO", 6)	// 6 = "SBO: " including terminating zero
    glObjectLabel(GL_BUFFER, mOpenGLStructuredBuffer, -1, detailedDebugName);
  }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
