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
 * @class
 * ShaderCompiler
 *
 * @brief
 * Helper structure for collection all method for the process of shader compilation.
 */
class ShaderCompiler {
public:
  static void print_opengl_shader_information_into_log(GLuint openGLShader);

  static void print_opengl_shader_information_into_log(GLuint openGLShader, const char* sourceCode);

  static void print_opengl_program_information_into_log(GLuint openGLProgram);

  static void print_opengl_program_information_into_log(GLuint openGLProgram, const char* sourceCode);

  /**
   * @brief
   * Create and load a shader from bytecode
   * 
   * @param[in] context
   * RHI context to use
   * @param[in] shaderType
   * Shader type (for example "GL_VERTEX_SHADER_ARB")
   * @param[in] shaderBytecode
   * Shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
   * 
   * @return
   * The OpenGL shader, 0 on error, destroy the resource if you no longer need it
   */
  [[nodiscard]] static GLuint load_shader_from_bytecode(GLenum shaderType, const rhi::ShaderBytecode& shaderBytecode);

  /**
   * @brief
   * Create and load a shader program from bytecode
   * 
   * @param[in] context
   * RHI context to use
   * @param[in] shaderType
   * Shader type (for example "GL_VERTEX_SHADER_ARB")
   * @param[in] shaderBytecode
   * Shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
   * 
   * @return
   * The OpenGL shader program, 0 on error, destroy the resource if you no longer need it
   */
  [[nodiscard]] static GLuint load_shader_program_from_bytecode(GLenum shaderType, const rhi::ShaderBytecode& shaderBytecode);

  /**
   * @brief
   * Create, load and compile a shader program from source code
   * 
   * @param[in] context
   * RHI context to use
   * @param[in] shaderType
   * Shader type (for example "GL_VERTEX_SHADER_ARB")
   * @param[in] sourceCode
   * Shader ASCII source code, must be a valid pointer
   * 
   * @return
   * The OpenGL shader program, 0 on error, destroy the resource if you no longer need it
   */
  [[nodiscard]] static GLuint load_shader_program_from_source_code(GLenum shaderType, const GLchar* sourceCode);

  [[nodiscard]] static GLuint create_shader_program_object(GLuint openGLShader, const rhi::VertexAttributes& vertexAttributes);

  [[nodiscard]] static GLuint load_shader_program_from_bytecode(const rhi::VertexAttributes& vertexAttributes, GLenum shaderType, const rhi::ShaderBytecode& shaderBytecode);

  /**
   * @brief
   * Creates, loads and compiles a shader from source code
   * 
   * @param[in] context
   * RHI context to use
   * @param[in] shaderType
   * Shader type (for example "GL_VERTEX_SHADER_ARB")
   * @param[in] sourceCode
   * Shader ASCII source code, must be a valid pointer
   * 
   * @return
   * The OpenGL shader, 0 on error, destroy the resource if you no longer need it
   */
  [[nodiscard]] static GLuint load_shader_from_source_code(GLenum shaderType, const GLchar* sourceCode);

  [[nodiscard]] static GLuint load_shader_program_from_source_code(const rhi::VertexAttributes& vertexAttributes, GLenum type, const char* sourceCode);

  /**
   * @brief
   * Compile shader source code to shader bytecode
   * 
   * @param[in] context
   * RHI context to use
   * @param[in] shaderType
   * Shader type (for example "GL_VERTEX_SHADER_ARB")
   * @param[in] sourceCode
   * Shader ASCII source code, must be a valid pointer
   * @param[out] shaderBytecode
   * Receives the shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
   */
  // TODO(naetherm) Visual Studio 2017 compile settings: For some reasons I need to disable optimization for the following method or else "glslang::TShader::parse()" will output the error "ERROR: 0:1: '�' : unexpected token" (glslang (latest commit c325f4364666eedb94c20a13670df058a38a14ab - April 20, 2018), Visual Studio 2017 15.7.1)
  static void shader_source_code_to_shader_bytecode(GLenum shaderType, const GLchar* sourceCode, rhi::ShaderBytecode& shaderBytecode);

private:
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
