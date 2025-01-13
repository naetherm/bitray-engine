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
#include "rhi_opengl/shader/separate/rhigeometry_shader_separate.h"
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
RHIGeometryShaderSeparate::RHIGeometryShaderSeparate(RHIDevice& openGLRhi, const rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGeometryShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLShaderProgram(ShaderCompiler::load_shader_program_from_bytecode(GL_GEOMETRY_SHADER_ARB, shaderBytecode)) {
  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLShaderProgram && openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
    glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
  }
#endif
}

RHIGeometryShaderSeparate::RHIGeometryShaderSeparate(RHIDevice& openGLRhi, const char* sourceCode, rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGeometryShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLShaderProgram(ShaderCompiler::load_shader_program_from_source_code(GL_GEOMETRY_SHADER_ARB, sourceCode)) {
  // Return shader bytecode, if requested do to so
  if (nullptr != shaderBytecode) {
    ShaderCompiler::shader_source_code_to_shader_bytecode(GL_GEOMETRY_SHADER_ARB, sourceCode, *shaderBytecode);
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLShaderProgram && openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
    glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
  }
#endif
}

RHIGeometryShaderSeparate::~RHIGeometryShaderSeparate() {
  // Destroy the OpenGL shader program
  // -> Silently ignores 0's and names that do not correspond to existing buffer objects
  glDeleteProgram(mOpenGLShaderProgram);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
