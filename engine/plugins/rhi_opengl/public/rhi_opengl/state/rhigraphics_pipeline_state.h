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
#include "rhi_opengl/rhi_opengl.h"
#include "rhi_opengl/state/rhiblend_state.h"
#include "rhi_opengl/state/rhidepth_stencil_state.h"
#include "rhi_opengl/state/rhirasterizer_state.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * OpenGL graphics pipeline state class
 */
class RHIGraphicsPipelineState final : public rhi::RHIGraphicsPipelineState {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] graphicsPipelineState
   * Graphics pipeline state to use
   * @param[in] id
   * The unique compact graphics pipeline state ID
   */
  RHIGraphicsPipelineState(RHIDevice &openGLRhi,
                           const rhi::GraphicsPipelineStateDescriptor &graphicsPipelineState,
                           core::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIGraphicsPipelineState() override;

  /**
   * @brief
   * Return the graphics program
   * 
   * @return
   * Graphics program, always valid
   */
  [[nodiscard]] inline rhi::RHIGraphicsProgram *get_graphics_program() const {
    return mGraphicsProgram;
  }

  /**
   * @brief
   * Return the OpenGL primitive topology describing the type of primitive to render
   * 
   * @return
   * OpenGL primitive topology describing the type of primitive to render
   */
  [[nodiscard]] inline GLenum get_opengl_primitive_topology() const {
    return mOpenGLPrimitiveTopology;
  }

  /**
   * @brief
   * Return the number of vertices per patch
   * 
   * @return
   * Return the number of vertices per patch
   */
  [[nodiscard]] inline GLint get_number_of_vertices_per_patch() const {
    return mNumberOfVerticesPerPatch;
  }

  /**
   * @brief
   * Bind the graphics pipeline state
   */
  void bind_graphics_pipeline_state() const;

  //[-------------------------------------------------------]
  //[ Detail state access                                   ]
  //[-------------------------------------------------------]
  [[nodiscard]] inline const rhi::RasterizerStateDescriptor &get_rasterizer_state() const {
    return mRasterizerState.get_rasterizer_state();
  }

  [[nodiscard]] inline const rhi::DepthStencilStateDescriptor &get_depth_stencil_state() const {
    return mDepthStencilState.get_depth_stencil_state();
  }

  [[nodiscard]] inline const rhi::BlendStateDescriptor &get_blend_state() const {
    return mBlendState.get_blend_state();
  }


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override
  {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIGraphicsPipelineState(const RHIGraphicsPipelineState &source) = delete;

  RHIGraphicsPipelineState &operator=(const RHIGraphicsPipelineState &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLenum mOpenGLPrimitiveTopology;  ///< OpenGL primitive topology describing the type of primitive to render
  GLint mNumberOfVerticesPerPatch;  ///< Number of vertices per patch
  rhi::RHIGraphicsProgram *mGraphicsProgram;
  rhi::RHIRenderPass *mRenderPass;
  RHIRasterizerState mRasterizerState;
  RHIDepthStencilState mDepthStencilState;
  RHIBlendState mBlendState;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
