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
#include "rhi_opengl/shader/separate/rhishader_language_separate.h"
#include "rhi_opengl/shader/separate/rhivertex_shader_separate.h"
#include "rhi_opengl/shader/separate/rhitessellation_control_shader_separate.h"
#include "rhi_opengl/shader/separate/rhitessellation_evaluation_shader_separate.h"
#include "rhi_opengl/shader/separate/rhigeometry_shader_separate.h"
#include "rhi_opengl/shader/separate/rhifragment_shader_separate.h"
#include "rhi_opengl/shader/separate/rhicompute_shader_separate.h"
#include "rhi_opengl/shader/separate/rhitask_shader_separate.h"
#include "rhi_opengl/shader/separate/rhimesh_shader_separate.h"
#include "rhi_opengl/shader/separate/rhigraphics_program_separate.h"
#include "rhi_opengl/shader/separate/rhigraphics_program_separate_dsa.h"
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
RHIShaderLanguageSeparate::RHIShaderLanguageSeparate(RHIDevice& openGLRhi)
: rhi::RHIShaderLanguage(openGLRhi) {

}

RHIShaderLanguageSeparate::~RHIShaderLanguageSeparate() {
  // De-initialize glslang, if necessary
#ifdef RHI_OPENGL_GLSLTOSPIRV
  if (::detail::GlslangInitialized) {
    // TODO(naetherm) Fix glslang related memory leaks. See also
    //		    - "Fix a few memory leaks #916" - https://github.com/KhronosGroup/glslang/pull/916
    //		    - "FreeGlobalPools is never called in glslang::FinalizeProcess()'s path. #928" - https://github.com/KhronosGroup/glslang/issues/928
    glslang::FinalizeProcess();
    ::detail::GlslangInitialized = false;
  }
#endif
}

rhi::RHIVertexShader* RHIShaderLanguageSeparate::create_vertex_shader_from_bytecode(const rhi::VertexAttributes& vertexAttributes, const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL vertex shader bytecode is invalid")

  // Check whether or not there's vertex shader support
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_vertex_shader() && extensions.isGL_ARB_gl_spirv()) {
    return re_new<RHIVertexShaderSeparate>(openGLRhi, vertexAttributes, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no vertex shader support or no decent shader bytecode support!
    return nullptr;
  }
}

rhi::RHIVertexShader* RHIShaderLanguageSeparate::create_vertex_shader_from_source_code(const rhi::VertexAttributes& vertexAttributes, const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's vertex shader support
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_vertex_shader()) {
    return re_new<RHIVertexShaderSeparate>(openGLRhi, vertexAttributes, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no vertex shader support!
    return nullptr;
  }
}

rhi::RHITessellationControlShader* RHIShaderLanguageSeparate::create_tessellation_control_shader_from_bytecode(const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL tessellation control shader bytecode is invalid")

  // Check whether or not there's tessellation support
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_tessellation_shader() && extensions.isGL_ARB_gl_spirv()) {
    return re_new<RHITessellationControlShaderSeparate>(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no tessellation support or no decent shader bytecode support!
    return nullptr;
  }
}

rhi::RHITessellationControlShader* RHIShaderLanguageSeparate::create_tessellation_control_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's tessellation support
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_tessellation_shader()) {
    return re_new<RHITessellationControlShaderSeparate>(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no tessellation support!
    return nullptr;
  }
}

rhi::RHITessellationEvaluationShader* RHIShaderLanguageSeparate::create_tessellation_evaluation_shader_from_bytecode(const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL tessellation evaluation shader bytecode is invalid")

  // Check whether or not there's tessellation support
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_tessellation_shader() && extensions.isGL_ARB_gl_spirv()) {
    return re_new<RHITessellationEvaluationShaderSeparate>(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no tessellation support or no decent shader bytecode support!
    return nullptr;
  }
}

rhi::RHITessellationEvaluationShader* RHIShaderLanguageSeparate::create_tessellation_evaluation_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's tessellation support
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_tessellation_shader()) {
    return re_new<RHITessellationEvaluationShaderSeparate>(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no tessellation support!
    return nullptr;
  }
}

rhi::RHIGeometryShader* RHIShaderLanguageSeparate::create_geometry_shader_from_bytecode(const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL geometry shader bytecode is invalid")

  // Check whether or not there's geometry shader support
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_geometry_shader4() && extensions.isGL_ARB_gl_spirv()) {
    return re_new<RHIGeometryShaderSeparate>(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no geometry shader support or no decent shader bytecode support!
    return nullptr;
  }
}

rhi::RHIGeometryShader* RHIShaderLanguageSeparate::create_geometry_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's geometry shader support
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_geometry_shader4()) {
    return re_new<RHIGeometryShaderSeparate>(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no geometry shader support!
    return nullptr;
  }
}

rhi::RHIFragmentShader* RHIShaderLanguageSeparate::create_fragment_shader_from_bytecode(const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL fragment shader bytecode is invalid")

  // Check whether or not there's fragment shader support
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_fragment_shader() && extensions.isGL_ARB_gl_spirv()) {
    return re_new<RHIFragmentShaderSeparate>(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no fragment shader support or no decent shader bytecode support!
    return nullptr;
  }
}

rhi::RHIFragmentShader* RHIShaderLanguageSeparate::create_fragment_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's fragment shader support
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_fragment_shader()) {
    return re_new<RHIFragmentShaderSeparate>(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no fragment shader support!
    return nullptr;
  }
}

rhi::RHITaskShader* RHIShaderLanguageSeparate::create_task_shader_from_bytecode(const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL task shader bytecode is invalid")

  // Check whether or not there's task shader support
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_NV_mesh_shader() && extensions.isGL_ARB_gl_spirv()) {
    return re_new<RHITaskShaderSeparate>(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no task shader support or no decent shader bytecode support!
    return nullptr;
  }
}

rhi::RHITaskShader* RHIShaderLanguageSeparate::create_task_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's task shader support
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_NV_mesh_shader()) {
    return re_new<RHITaskShaderSeparate>(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no task shader support!
    return nullptr;
  }
}

rhi::RHIMeshShader* RHIShaderLanguageSeparate::create_mesh_shader_from_bytecode(const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL mesh shader bytecode is invalid")

  // Check whether or not there's mesh shader support
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_NV_mesh_shader() && extensions.isGL_ARB_gl_spirv()) {
    return re_new<RHIMeshShaderSeparate>(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no mesh shader support or no decent shader bytecode support!
    return nullptr;
  }
}

rhi::RHIMeshShader* RHIShaderLanguageSeparate::create_mesh_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's mesh shader support
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_NV_mesh_shader()) {
    return re_new<RHIMeshShaderSeparate>(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no mesh shader support!
    return nullptr;
  }
}

rhi::RHIComputeShader* RHIShaderLanguageSeparate::create_compute_shader_from_bytecode(const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // Sanity check
  BE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL compute shader bytecode is invalid")

  // Check whether or not there's compute shader support
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_compute_shader() && extensions.isGL_ARB_gl_spirv())
  {
    return re_new<RHIComputeShaderSeparate>(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no compute shader support or no decent shader bytecode support!
    return nullptr;
  }
}

rhi::RHIComputeShader* RHIShaderLanguageSeparate::create_compute_shader_from_source_code(const rhi::ShaderSourceCode& shaderSourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Check whether or not there's compute shader support
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());
  const Extensions& extensions = openGLRhi.get_extensions();
  if (extensions.isGL_ARB_compute_shader()) {
    return re_new<RHIComputeShaderSeparate>(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Error! There's no compute shader support!
    return nullptr;
  }
}

rhi::RHIGraphicsProgram* RHIShaderLanguageSeparate::create_graphics_program(const rhi::RHIRootSignature& rootSignature, [[maybe_unused]] const rhi::VertexAttributes& vertexAttributes, rhi::RHIVertexShader* vertexShader, rhi::RHITessellationControlShader* tessellationControlShader, rhi::RHITessellationEvaluationShader* tessellationEvaluationShader, rhi::RHIGeometryShader* geometryShader, rhi::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  if (nullptr != vertexShader && vertexShader->get_shader_language_name() != "GLSL") {
    // Error! Vertex shader language mismatch!
  }
  else if (nullptr != tessellationControlShader && tessellationControlShader->get_shader_language_name() != "GLSL") {
    // Error! Tessellation control shader language mismatch!
  }
  else if (nullptr != tessellationEvaluationShader && tessellationEvaluationShader->get_shader_language_name() != "GLSL") {
    // Error! Tessellation evaluation shader language mismatch!
  }
  else if (nullptr != geometryShader && geometryShader->get_shader_language_name() != "GLSL") {
    // Error! Geometry shader language mismatch!
  }
  else if (nullptr != fragmentShader && fragmentShader->get_shader_language_name() != "GLSL") {
    // Error! Fragment shader language mismatch!
  }

    // Is "GL_EXT_direct_state_access" there?
  if (openGLRhi.get_extensions().isGL_EXT_direct_state_access() || openGLRhi.get_extensions().isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    return re_new<RHIGraphicsProgramSeparateDsa>(openGLRhi, rootSignature, static_cast<RHIVertexShaderSeparate*>(vertexShader), static_cast<RHITessellationControlShaderSeparate*>(tessellationControlShader), static_cast<RHITessellationEvaluationShaderSeparate*>(tessellationEvaluationShader), static_cast<RHIGeometryShaderSeparate*>(geometryShader), static_cast<RHIFragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version
    return re_new<RHIGraphicsProgramSeparate>(openGLRhi, rootSignature, static_cast<RHIVertexShaderSeparate*>(vertexShader), static_cast<RHITessellationControlShaderSeparate*>(tessellationControlShader), static_cast<RHITessellationEvaluationShaderSeparate*>(tessellationEvaluationShader), static_cast<RHIGeometryShaderSeparate*>(geometryShader), static_cast<RHIFragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error! Shader language mismatch!
  // -> Ensure a correct reference counter behaviour, even in the situation of an error
  if (nullptr != vertexShader) {
    vertexShader->add_ref();
    vertexShader->release();
  }
  if (nullptr != tessellationControlShader) {
    tessellationControlShader->add_ref();
    tessellationControlShader->release();
  }
  if (nullptr != tessellationEvaluationShader) {
    tessellationEvaluationShader->add_ref();
    tessellationEvaluationShader->release();
  }
  if (nullptr != geometryShader) {
    geometryShader->add_ref();
    geometryShader->release();
  }
  if (nullptr != fragmentShader) {
    fragmentShader->add_ref();
    fragmentShader->release();
  }

  // Error!
  return nullptr;
}

rhi::RHIGraphicsProgram* RHIShaderLanguageSeparate::create_graphics_program(const rhi::RHIRootSignature& rootSignature, rhi::RHITaskShader* taskShader, rhi::RHIMeshShader& meshShader, rhi::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDevice& openGLRhi = static_cast<RHIDevice&>(get_rhi());

  // A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  if (nullptr != taskShader && taskShader->get_shader_language_name() != "GLSL") {
    // Error! Vertex shader language mismatch!
  }
  else if (meshShader.get_shader_language_name() != "GLSL") {
    // Error! Fragment shader language mismatch!
  }
  else if (nullptr != fragmentShader && fragmentShader->get_shader_language_name() != "GLSL") {
    // Error! Vertex shader language mismatch!
  }

    // Is "GL_EXT_direct_state_access" there?
  else if (openGLRhi.get_extensions().isGL_EXT_direct_state_access() || openGLRhi.get_extensions().isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    return re_new<RHIGraphicsProgramSeparateDsa>(openGLRhi, rootSignature, static_cast<RHITaskShaderSeparate*>(taskShader), static_cast<RHIMeshShaderSeparate&>(meshShader), static_cast<RHIFragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version
    return re_new<RHIGraphicsProgramSeparate>(openGLRhi, rootSignature, static_cast<RHITaskShaderSeparate*>(taskShader), static_cast<RHIMeshShaderSeparate&>(meshShader), static_cast<RHIFragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error! Shader language mismatch!
  // -> Ensure a correct reference counter behaviour, even in the situation of an error
  if (nullptr != taskShader) {
    taskShader->add_ref();
    taskShader->release();
  }
  meshShader.add_ref();
  meshShader.release();
  if (nullptr != fragmentShader) {
    fragmentShader->add_ref();
    fragmentShader->release();
  }

  // Error!
  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
