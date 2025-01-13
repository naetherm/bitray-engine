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
 * Separate shader language class
 */
class RHIShaderLanguageSeparate final : public rhi::RHIShaderLanguage {


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
   */
  explicit RHIShaderLanguageSeparate(RHIDevice &openGLRhi);

  ~RHIShaderLanguageSeparate() override;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIShaderLanguage methods           ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual const char *get_shader_language_name() const override
  {
    return "GLSL";
  }

  [[nodiscard]] virtual rhi::RHIVertexShader *
  create_vertex_shader_from_bytecode(const rhi::VertexAttributes &vertexAttributes,
                                     const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIVertexShader *
  create_vertex_shader_from_source_code(const rhi::VertexAttributes &vertexAttributes,
                                        const rhi::ShaderSourceCode &shaderSourceCode,
                                        rhi::ShaderBytecode *shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITessellationControlShader *create_tessellation_control_shader_from_bytecode(
    const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITessellationControlShader *
  create_tessellation_control_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode,
                                                      rhi::ShaderBytecode *shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITessellationEvaluationShader *create_tessellation_evaluation_shader_from_bytecode(
    const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITessellationEvaluationShader *
  create_tessellation_evaluation_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode,
                                                         rhi::ShaderBytecode *shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIGeometryShader *create_geometry_shader_from_bytecode(
    const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIGeometryShader *
  create_geometry_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode,
                                          rhi::ShaderBytecode *shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIFragmentShader *create_fragment_shader_from_bytecode(
    const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIFragmentShader *
  create_fragment_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode,
                                          rhi::ShaderBytecode *shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITaskShader *create_task_shader_from_bytecode(
    const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITaskShader *
  create_task_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode,
                                      rhi::ShaderBytecode *shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIMeshShader *create_mesh_shader_from_bytecode(
    const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIMeshShader *
  create_mesh_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode,
                                      rhi::ShaderBytecode *shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIComputeShader *create_compute_shader_from_bytecode(
    const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIComputeShader *
  create_compute_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode,
                                         rhi::ShaderBytecode *shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIGraphicsProgram *create_graphics_program(const rhi::RHIRootSignature &rootSignature,
                                                                           [[maybe_unused]] const rhi::VertexAttributes &vertexAttributes,
                                                                           rhi::RHIVertexShader *vertexShader,
                                                                           rhi::RHITessellationControlShader *tessellationControlShader,
                                                                           rhi::RHITessellationEvaluationShader *tessellationEvaluationShader,
                                                                           rhi::RHIGeometryShader *geometryShader,
                                                                           rhi::RHIFragmentShader *fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHIGraphicsProgram *
  create_graphics_program(const rhi::RHIRootSignature &rootSignature, rhi::RHITaskShader *taskShader,
                          rhi::RHIMeshShader &meshShader,
                          rhi::RHIFragmentShader *fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


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
  explicit RHIShaderLanguageSeparate(const RHIShaderLanguageSeparate &source) = delete;

  RHIShaderLanguageSeparate &operator=(const RHIShaderLanguageSeparate &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
