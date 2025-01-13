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
#include "rhi_opengl/state/rhigraphics_pipeline_state.h"
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
RHIGraphicsPipelineState::RHIGraphicsPipelineState(RHIDevice &openGLRhi, const rhi::GraphicsPipelineStateDescriptor &graphicsPipelineState, core::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGraphicsPipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLPrimitiveTopology(0xFFFF)  // Unknown default setting
, mNumberOfVerticesPerPatch(0)
, mGraphicsProgram(graphicsPipelineState.graphicsProgram)
, mRenderPass(graphicsPipelineState.renderPass)
, mRasterizerState(graphicsPipelineState.rasterizerState)
, mDepthStencilState(graphicsPipelineState.depthStencilState)
, mBlendState(graphicsPipelineState.blendState) {
  // Tessellation support: Up to 32 vertices per patch are supported "rhi::PrimitiveTopology::PATCH_LIST_1" ... "rhi::PrimitiveTopology::PATCH_LIST_32"
  if (graphicsPipelineState.primitiveTopology >= rhi::PrimitiveTopology::PATCH_LIST_1) {
    // Use tessellation

    // get number of vertices that will be used to make up a single patch primitive
    // -> There's no need to check for the "GL_ARB_tessellation_shader" extension, it's there if "rhi::Capabilities::maximumNumberOfPatchVertices" is not 0
    const int numberOfVerticesPerPatch = static_cast<int>(graphicsPipelineState.primitiveTopology) - static_cast<int>(rhi::PrimitiveTopology::PATCH_LIST_1) + 1;
    if (numberOfVerticesPerPatch <= static_cast<int>(openGLRhi.get_capabilities().maximumNumberOfPatchVertices)) {
      // Set number of vertices that will be used to make up a single patch primitive
      mNumberOfVerticesPerPatch = numberOfVerticesPerPatch;

      // Set OpenGL primitive topology
      mOpenGLPrimitiveTopology = GL_PATCHES;
    } else {
      // Error!
      BE_ASSERT(false, "Invalid number of OpenGL vertices per patch")
    }
  } else {
    // Do not use tessellation

    // Map and backup the set OpenGL primitive topology
    mOpenGLPrimitiveTopology = Mapping::get_opengl_type(graphicsPipelineState.primitiveTopology);
  }

    // Ensure a correct reference counter behaviour
  graphicsPipelineState.rootSignature->add_ref();
  graphicsPipelineState.rootSignature->release();

    // Add a reference to the referenced RHI resources
  mGraphicsProgram->add_ref();
  mRenderPass->add_ref();
}

RHIGraphicsPipelineState::~RHIGraphicsPipelineState() {
  // release referenced RHI resources
  mGraphicsProgram->release();
  mRenderPass->release();

  // Free the unique compact graphics pipeline state ID
  static_cast<RHIDevice &>(get_rhi()).GraphicsPipelineStateMakeId.destroy_id(get_id());
}

void RHIGraphicsPipelineState::bind_graphics_pipeline_state() const {
  static_cast<RHIDevice &>(get_rhi()).set_opengl_graphics_program(mGraphicsProgram);

  // Set the OpenGL rasterizer state
  mRasterizerState.set_opengl_rasterizer_states();

  // Set OpenGL depth stencil state
  mDepthStencilState.get_opengl_depth_stencil_states();

  // Set OpenGL blend state
  mBlendState.get_opengl_blend_states();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
