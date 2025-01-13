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
#include "rhi_opengl/buffer/rhivertex_array_vao.h"
#include "rhi_opengl/buffer/rhivertex_buffer.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIVertexArrayVao::~RHIVertexArrayVao() {
  // Destroy the OpenGL vertex array
  // -> Silently ignores 0's and names that do not correspond to existing vertex array objects
  glDeleteVertexArrays(1, &mOpenGLVertexArray);

  // release the reference to the used vertex buffers
  if (nullptr != mVertexBuffers) {
    // release references
    RHIVertexBuffer **vertexBuffersEnd = mVertexBuffers + mNumberOfVertexBuffers;
    for (RHIVertexBuffer **vertexBuffer = mVertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer) {
      (*vertexBuffer)->release();
    }

    // Cleanup
    re_free(mVertexBuffers);
  }
}

RHIVertexArrayVao::RHIVertexArrayVao(RHIDevice &openGLRhi, rhi::VertexArrayDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: RHIVertexArray(openGLRhi, descriptor, InternalResourceType::VAO RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLVertexArray(0)
, mNumberOfVertexBuffers(descriptor.numberOfVertexBuffers)
, mVertexBuffers(nullptr) {
  // Add a reference to the used vertex buffers
  if (descriptor.numberOfVertexBuffers > 0) {
    mVertexBuffers = re_typed_alloc<RHIVertexBuffer*>(descriptor.numberOfVertexBuffers);

    // Loop through all vertex buffers
    RHIVertexBuffer **currentVertexBuffers = mVertexBuffers;
    const rhi::VertexArrayVertexBuffer *vertexBuffersEnd = descriptor.vertexBuffers + descriptor.numberOfVertexBuffers;
    for (const rhi::VertexArrayVertexBuffer *vertexBuffer = descriptor.vertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer, ++currentVertexBuffers) {
      // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
      *currentVertexBuffers = static_cast<RHIVertexBuffer *>(vertexBuffer->vertexBuffer);
      (*currentVertexBuffers)->add_ref();
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
