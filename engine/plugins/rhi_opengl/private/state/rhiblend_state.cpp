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
#include "rhi_opengl/state/rhiblend_state.h"
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
RHIBlendState::RHIBlendState(const rhi::BlendStateDescriptor& blendState)
: mBlendState(blendState)
, mOpenGLSrcBlend(Mapping::get_opengl_blend_type(mBlendState.renderTarget[0].srcBlend))
, mOpenGLDstBlend(Mapping::get_opengl_blend_type(mBlendState.renderTarget[0].destBlend)) {

}

void RHIBlendState::get_opengl_blend_states() const {
  // "GL_ARB_multisample"-extension
  if (mBlendState.alphaToCoverageEnable) {
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
  } else {
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
  }

  // TODO(naetherm) Add support for blend state per render target
  if (mBlendState.renderTarget[0].blendEnable) {
    glEnable(GL_BLEND);
    glBlendFunc(mOpenGLSrcBlend, mOpenGLDstBlend);
  } else {
    glDisable(GL_BLEND);
  }

  // TODO(naetherm) Map the rest of the blend states
  // GL_EXT_blend_func_separate
  // (GL_EXT_blend_equation_separate)
  // GL_EXT_blend_color
  // GL_EXT_blend_minmax
  // GL_EXT_blend_subtract
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
