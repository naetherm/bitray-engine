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
#include "rhi_vulkan/shader/rhishader_language_glsl.h"
#include "rhi_vulkan/shader/rhicompute_shader_glsl.h"
#include "rhi_vulkan/shader/rhifragment_shader_glsl.h"
#include "rhi_vulkan/shader/rhigeometry_shader_glsl.h"
#include "rhi_vulkan/shader/rhigraphics_program_glsl.h"
#include "rhi_vulkan/shader/rhimesh_shader_glsl.h"
#include "rhi_vulkan/shader/rhitask_shader_glsl.h"
#include "rhi_vulkan/shader/rhitessellation_evaluation_shader_glsl.h"
#include "rhi_vulkan/shader/rhitessellation_control_shader_glsl.h"
#include "rhi_vulkan/shader/rhivertex_shader_glsl.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/shader_compiler.h"
#include "rhi_vulkan/vulkan_helper.h"
#include <glslang/Public/ShaderLang.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIShaderLanguageGlsl::RHIShaderLanguageGlsl(RHIDevice &vulkanRhi)
: rhi::RHIShaderLanguage(vulkanRhi) {
  
}

RHIShaderLanguageGlsl::~RHIShaderLanguageGlsl() {
  // De-initialize glslang, if necessary
  if (ShaderCompiler::GlslangInitialized) {
    // TODO(naetherm) Fix glslang related memory leaks. See also
    //		    - "Fix a few memory leaks #916" - https://github.com/KhronosGroup/glslang/pull/916
    //		    - "FreeGlobalPools is never called in glslang::FinalizeProcess()'s path. #928" - https://github.com/KhronosGroup/glslang/issues/928
    glslang::FinalizeProcess();
    ShaderCompiler::GlslangInitialized = false;
  }
}

const char *RHIShaderLanguageGlsl::get_shader_language_name() const {
  return VulkanHelper::GLSL_NAME;
}

rhi::RHIVertexShader * RHIShaderLanguageGlsl::create_vertex_shader_from_bytecode([[maybe_unused]] const rhi::VertexAttributes &vertexAttributes, const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan vertex shader bytecode is invalid")

  // Create shader instance
  return new RHIVertexShaderGlsl(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIVertexShader * RHIShaderLanguageGlsl::create_vertex_shader_from_source_code([[maybe_unused]] const rhi::VertexAttributes &vertexAttributes, const rhi::ShaderSourceCode &shaderSourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  return new RHIVertexShaderGlsl(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITessellationControlShader * RHIShaderLanguageGlsl::create_tessellation_control_shader_from_bytecode(const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan tessellation control shader bytecode is invalid")

  // Create shader instance
  return new RHITessellationControlShaderGlsl(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITessellationControlShader * RHIShaderLanguageGlsl::create_tessellation_control_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  return new RHITessellationControlShaderGlsl(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITessellationEvaluationShader * RHIShaderLanguageGlsl::create_tessellation_evaluation_shader_from_bytecode(const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan tessellation evaluation shader bytecode is invalid")

  // Create shader instance
  return new RHITessellationEvaluationShaderGlsl(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITessellationEvaluationShader * RHIShaderLanguageGlsl::create_tessellation_evaluation_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  return new RHITessellationEvaluationShaderGlsl(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIGeometryShader * RHIShaderLanguageGlsl::create_geometry_shader_from_bytecode(const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan geometry shader bytecode is invalid")

  // Create shader instance
  return new RHIGeometryShaderGlsl(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIGeometryShader * RHIShaderLanguageGlsl::create_geometry_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  return new RHIGeometryShaderGlsl(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIFragmentShader * RHIShaderLanguageGlsl::create_fragment_shader_from_bytecode(const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan fragment shader bytecode is invalid")

  // Create shader instance
  return new RHIFragmentShaderGlsl(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIFragmentShader * RHIShaderLanguageGlsl::create_fragment_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  return new RHIFragmentShaderGlsl(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITaskShader *RHIShaderLanguageGlsl::create_task_shader_from_bytecode(const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan task shader bytecode is invalid")

  // Create shader instance
  return new RHITaskShaderGlsl(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHITaskShader * RHIShaderLanguageGlsl::create_task_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  return new RHITaskShaderGlsl(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIMeshShader *RHIShaderLanguageGlsl::create_mesh_shader_from_bytecode(const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan mesh shader bytecode is invalid")

  // Create shader instance
  return new RHIMeshShaderGlsl(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIMeshShader * RHIShaderLanguageGlsl::create_mesh_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  return new RHIMeshShaderGlsl(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIComputeShader *RHIShaderLanguageGlsl::create_compute_shader_from_bytecode(const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Vulkan compute shader bytecode is invalid")

  // Create shader instance
  return new RHIComputeShaderGlsl(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIComputeShader * RHIShaderLanguageGlsl::create_compute_shader_from_source_code(const rhi::ShaderSourceCode &shaderSourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  return new RHIComputeShaderGlsl(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIGraphicsProgram * RHIShaderLanguageGlsl::create_graphics_program([[maybe_unused]] const rhi::RHIRootSignature &rootSignature, [[maybe_unused]] const rhi::VertexAttributes &vertexAttributes, rhi::RHIVertexShader *vertexShader, rhi::RHITessellationControlShader *tessellationControlShader, rhi::RHITessellationEvaluationShader *tessellationEvaluationShader, rhi::RHIGeometryShader *geometryShader, rhi::RHIFragmentShader *fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  BE_ASSERT(nullptr == vertexShader || vertexShader->get_shader_language_name() == VulkanHelper::GLSL_NAME, "Vulkan vertex shader language mismatch")
  BE_ASSERT(nullptr == tessellationControlShader || tessellationControlShader->get_shader_language_name() == VulkanHelper::GLSL_NAME, "Vulkan tessellation control shader language mismatch")
  BE_ASSERT(nullptr == tessellationEvaluationShader || tessellationEvaluationShader->get_shader_language_name() == VulkanHelper::GLSL_NAME, "Vulkan tessellation evaluation shader language mismatch")
  BE_ASSERT(nullptr == geometryShader || geometryShader->get_shader_language_name() == VulkanHelper::GLSL_NAME, "Vulkan geometry shader language mismatch")
  BE_ASSERT(nullptr == fragmentShader || fragmentShader->get_shader_language_name() == VulkanHelper::GLSL_NAME, "Vulkan fragment shader language mismatch")

  // Create the graphics program
  return new RHIGraphicsProgramGlsl(vulkanRhi, static_cast<RHIVertexShaderGlsl *>(vertexShader),
                                                              static_cast<RHITessellationControlShaderGlsl *>(tessellationControlShader),
                                                              static_cast<RHITessellationEvaluationShaderGlsl *>(tessellationEvaluationShader),
                                                              static_cast<RHIGeometryShaderGlsl *>(geometryShader),
                                                              static_cast<RHIFragmentShaderGlsl *>(fragmentShader)
                                                              RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIGraphicsProgram * RHIShaderLanguageGlsl::create_graphics_program([[maybe_unused]] const rhi::RHIRootSignature &rootSignature, rhi::RHITaskShader *taskShader, rhi::RHIMeshShader &meshShader, rhi::RHIFragmentShader *fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  BE_ASSERT(nullptr == taskShader || taskShader->get_shader_language_name() == VulkanHelper::GLSL_NAME, "Vulkan task shader language mismatch")
  BE_ASSERT(meshShader.get_shader_language_name() == VulkanHelper::GLSL_NAME, "Vulkan mesh shader language mismatch")
  BE_ASSERT(nullptr == fragmentShader || fragmentShader->get_shader_language_name() == VulkanHelper::GLSL_NAME, "Vulkan fragment shader language mismatch")

  // Create the graphics program
  return new RHIGraphicsProgramGlsl(vulkanRhi, static_cast<RHITaskShaderGlsl *>(taskShader),
                                                              static_cast<RHIMeshShaderGlsl &>(meshShader),
                                                              static_cast<RHIFragmentShaderGlsl *>(fragmentShader)
                                                              RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


void RHIShaderLanguageGlsl::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
