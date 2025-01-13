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
#include "rhi_opengl/shader/monolithic/rhishader_language_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhicompute_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhivertex_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhitessellation_control_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhitessellation_evaluation_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhifragment_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhigeometry_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhitask_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhimesh_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhigraphics_program_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhigraphics_program_monolithic_dsa.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/shader_compiler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIShaderLanguageMonolithic::RHIShaderLanguageMonolithic(RHIDevice& openGLRhi)
: rhi::RHIShaderLanguage(openGLRhi) {

}

rhi::RHIVertexShader* RHIShaderLanguageMonolithic::create_vertex_shader_from_bytecode(const rhi::VertexAttributes&, const rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Error!
  BE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

rhi::RHIVertexShader* RHIShaderLanguageMonolithic::create_vertex_shader_from_source_code(const rhi::VertexAttributes& vertexAttributes, const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's vertex shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  if (openGLRhi.get_extensions().isGL_ARB_vertex_shader()) {
    return re_new<RHIVertexShaderMonolithic>(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no vertex shader support!
    return nullptr;
  }
}

rhi::RHITessellationControlShader* RHIShaderLanguageMonolithic::create_tessellation_control_shader_from_bytecode(const rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Error!
  BE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

rhi::RHITessellationControlShader* RHIShaderLanguageMonolithic::create_tessellation_control_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's tessellation support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  if (openGLRhi.get_extensions().isGL_ARB_tessellation_shader()) {
    return re_new<RHITessellationControlShaderMonolithic>(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no tessellation support!
    return nullptr;
  }
}

rhi::RHITessellationEvaluationShader* RHIShaderLanguageMonolithic::create_tessellation_evaluation_shader_from_bytecode(const rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Error!
  BE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

rhi::RHITessellationEvaluationShader* RHIShaderLanguageMonolithic::create_tessellation_evaluation_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's tessellation support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  if (openGLRhi.get_extensions().isGL_ARB_tessellation_shader()) {
    return re_new<RHITessellationEvaluationShaderMonolithic>(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no tessellation support!
    return nullptr;
  }
}

rhi::RHIGeometryShader* RHIShaderLanguageMonolithic::create_geometry_shader_from_bytecode(const rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Error!
  BE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

rhi::RHIGeometryShader* RHIShaderLanguageMonolithic::create_geometry_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's geometry shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  if (openGLRhi.get_extensions().isGL_ARB_geometry_shader4()) {
    return re_new<RHIGeometryShaderMonolithic>(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no geometry shader support!
    return nullptr;
  }
}

rhi::RHIFragmentShader* RHIShaderLanguageMonolithic::create_fragment_shader_from_bytecode(const rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Error!
  BE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

rhi::RHIFragmentShader* RHIShaderLanguageMonolithic::create_fragment_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's fragment shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  if (openGLRhi.get_extensions().isGL_ARB_fragment_shader()) {
    return re_new<RHIFragmentShaderMonolithic>(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no fragment shader support!
    return nullptr;
  }
}

rhi::RHITaskShader* RHIShaderLanguageMonolithic::create_task_shader_from_bytecode(const rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Error!
  BE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

rhi::RHITaskShader* RHIShaderLanguageMonolithic::create_task_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's mesh shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  if (openGLRhi.get_extensions().isGL_NV_mesh_shader()) {
    return re_new<RHITaskShaderMonolithic>(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no task shader support!
    return nullptr;
  }
}

rhi::RHIMeshShader* RHIShaderLanguageMonolithic::create_mesh_shader_from_bytecode(const rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Error!
  BE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

rhi::RHIMeshShader* RHIShaderLanguageMonolithic::create_mesh_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's mesh shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  if (openGLRhi.get_extensions().isGL_NV_mesh_shader()) {
    return re_new<RHIMeshShaderMonolithic>(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no mesh shader support!
    return nullptr;
  }
}

rhi::RHIComputeShader* RHIShaderLanguageMonolithic::create_compute_shader_from_bytecode(const rhi::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Error!
  BE_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

rhi::RHIComputeShader* RHIShaderLanguageMonolithic::create_compute_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's compute shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  if (openGLRhi.get_extensions().isGL_ARB_compute_shader()) {
    return re_new<RHIComputeShaderMonolithic>(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no compute shader support!
    return nullptr;
  }
}

rhi::RHIGraphicsProgram* RHIShaderLanguageMonolithic::create_graphics_program(const rhi::RHIRootSignature& rootSignature, const rhi::VertexAttributes& vertexAttributes, rhi::RHIVertexShader* vertexShader, rhi::RHITessellationControlShader* tessellationControlShader, rhi::RHITessellationEvaluationShader* tessellationEvaluationShader, rhi::RHIGeometryShader* geometryShader, rhi::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  BE_ASSERT(nullptr == vertexShader || vertexShader->get_shader_language_name() == "GLSL", "OpenGL vertex shader language mismatch")
  BE_ASSERT(nullptr == tessellationControlShader || tessellationControlShader->get_shader_language_name() == "GLSL", "OpenGL tessellation control shader language mismatch")
  BE_ASSERT(nullptr == tessellationEvaluationShader || tessellationEvaluationShader->get_shader_language_name() == "GLSL", "OpenGL tessellation evaluation shader language mismatch")
  BE_ASSERT(nullptr == geometryShader || geometryShader->get_shader_language_name() == "GLSL", "OpenGL geometry shader language mismatch")
  BE_ASSERT(nullptr == fragmentShader || fragmentShader->get_shader_language_name() == "GLSL", "OpenGL fragment shader language mismatch")

  // Create the graphics program: Is "GL_EXT_direct_state_access" there?
  if (openGLRhi.get_extensions().isGL_EXT_direct_state_access() || openGLRhi.get_extensions().isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    return re_new<RHIGraphicsProgramMonolithicDsa>(openGLRhi, rootSignature, vertexAttributes, static_cast<RHIVertexShaderMonolithic*>(vertexShader), static_cast<RHITessellationControlShaderMonolithic*>(tessellationControlShader), static_cast<RHITessellationEvaluationShaderMonolithic*>(tessellationEvaluationShader), static_cast<RHIGeometryShaderMonolithic*>(geometryShader), static_cast<RHIFragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version
    return re_new<RHIGraphicsProgramMonolithic>(openGLRhi, rootSignature, vertexAttributes, static_cast<RHIVertexShaderMonolithic*>(vertexShader), static_cast<RHITessellationControlShaderMonolithic*>(tessellationControlShader), static_cast<RHITessellationEvaluationShaderMonolithic*>(tessellationEvaluationShader), static_cast<RHIGeometryShaderMonolithic*>(geometryShader), static_cast<RHIFragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

rhi::RHIGraphicsProgram* RHIShaderLanguageMonolithic::create_graphics_program(const rhi::RHIRootSignature& rootSignature, rhi::RHITaskShader* taskShader, rhi::RHIMeshShader& meshShader, rhi::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  BE_ASSERT(nullptr == taskShader || taskShader->get_shader_language_name() == "GLSL", "OpenGL task shader language mismatch")
  BE_ASSERT(meshShader.get_shader_language_name() == "GLSL", "OpenGL mesh shader language mismatch")
  BE_ASSERT(nullptr == fragmentShader || fragmentShader->get_shader_language_name() == "GLSL", "OpenGL fragment shader language mismatch")

  // Create the graphics program: Is "GL_EXT_direct_state_access" there?
  if (openGLRhi.get_extensions().isGL_EXT_direct_state_access() || openGLRhi.get_extensions().isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    return re_new<RHIGraphicsProgramMonolithicDsa>(openGLRhi, rootSignature, static_cast<RHITaskShaderMonolithic*>(taskShader), static_cast<RHIMeshShaderMonolithic&>(meshShader), static_cast<RHIFragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version
    return re_new<RHIGraphicsProgramMonolithic>(openGLRhi, rootSignature, static_cast<RHITaskShaderMonolithic*>(taskShader), static_cast<RHIMeshShaderMonolithic&>(meshShader), static_cast<RHIFragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
