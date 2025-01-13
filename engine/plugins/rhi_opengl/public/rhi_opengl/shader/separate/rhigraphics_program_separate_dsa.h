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
#include "rhi_opengl/shader/separate/rhigraphics_program_separate.h"


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
 * Separate graphics program class, effective direct state access (DSA)
 */
class RHIGraphicsProgramSeparateDsa final : public RHIGraphicsProgramSeparate {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor for traditional graphics program
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] rootSignature
   * Root signature
   * @param[in] vertexShaderSeparate
   * Vertex shader the graphics program is using, can be a null pointer
   * @param[in] tessellationControlShaderSeparate
   * Tessellation control shader the graphics program is using, can be a null pointer
   * @param[in] tessellationEvaluationShaderSeparate
   * Tessellation evaluation shader the graphics program is using, can be a null pointer
   * @param[in] geometryShaderSeparate
   * Geometry shader the graphics program is using, can be a null pointer
   * @param[in] fragmentShaderSeparate
   * Fragment shader the graphics program is using, can be a null pointer
   * 
   * @note
   * - The graphics program keeps a reference to the provided shaders and releases it when no longer required
   */
  RHIGraphicsProgramSeparateDsa(RHIDevice &openGLRhi, const rhi::RHIRootSignature &rootSignature,
                                RHIVertexShaderSeparate *vertexShaderSeparate,
                                RHITessellationControlShaderSeparate *tessellationControlShaderSeparate,
                                RHITessellationEvaluationShaderSeparate *tessellationEvaluationShaderSeparate,
                                RHIGeometryShaderSeparate *geometryShaderSeparate,
                                RHIFragmentShaderSeparate *fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Constructor for task and mesh shader based graphics program
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] rootSignature
   * Root signature
   * @param[in] taskShaderSeparate
   * Task shader the graphics program is using, can be a null pointer
   * @param[in] meshShaderSeparate
   * Mesh shader the graphics program is using
   * @param[in] fragmentShaderSeparate
   * Fragment shader the graphics program is using, can be a null pointer
   * 
   * @note
   * - The graphics program keeps a reference to the provided shaders and releases it when no longer required
   */
  RHIGraphicsProgramSeparateDsa(RHIDevice &openGLRhi, const rhi::RHIRootSignature &rootSignature,
                                RHITaskShaderSeparate *taskShaderSeparate, RHIMeshShaderSeparate &meshShaderSeparate,
                                RHIFragmentShaderSeparate *fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Destructor
   */
  ~RHIGraphicsProgramSeparateDsa() override = default;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIGraphicsProgramSeparateDsa(const RHIGraphicsProgramSeparateDsa &source) = delete;

  RHIGraphicsProgramSeparateDsa &operator=(const RHIGraphicsProgramSeparateDsa &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
