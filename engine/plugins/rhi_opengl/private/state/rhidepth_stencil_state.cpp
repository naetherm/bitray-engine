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
#include "rhi_opengl/state/rhidepth_stencil_state.h"
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
RHIDepthStencilState::RHIDepthStencilState(const rhi::DepthStencilStateDescriptor& depthStencilState)
: mDepthStencilState(depthStencilState)
,  mOpenGLDepthMaskEnabled(static_cast<GLboolean>((rhi::DepthWriteMask::ALL == mDepthStencilState.depthWriteMask) ? GL_TRUE : GL_FALSE))
, mOpenGLDepthFunc(Mapping::get_opengl_comparison_func(depthStencilState.depthFunc)) {

}

void RHIDepthStencilState::get_opengl_depth_stencil_states() const {
  // rhi::DepthStencilState::depthEnable
  if (mDepthStencilState.depthEnable) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }

  // rhi::DepthStencilState::depthWriteMask
  glDepthMask(mOpenGLDepthMaskEnabled);

  // rhi::DepthStencilState::depthFunc
  glDepthFunc(static_cast<GLenum>(mOpenGLDepthFunc));

  // TODO(naetherm) Map the rest of the depth stencil states
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
