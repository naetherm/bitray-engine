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
 * Separate geometry shader class
 */
class RHIGeometryShaderSeparate final : public rhi::RHIGeometryShader {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor for creating a geometry shader from shader bytecode
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] shaderBytecode
   * Shader bytecode
   */
  RHIGeometryShaderSeparate(RHIDevice &openGLRhi,
                            const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Constructor for creating a geometry shader from shader source code
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] sourceCode
   * Shader ASCII source code, must be valid
   */
  RHIGeometryShaderSeparate(RHIDevice &openGLRhi, const char *sourceCode,
                            rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Destructor
   */
  ~RHIGeometryShaderSeparate() override;

  /**
   * @brief
   * Return the OpenGL shader program
   * 
   * @return
   * The OpenGL shader program, can be zero if no resource is allocated, do not destroy the returned resource
   */
  [[nodiscard]] inline GLuint get_opengl_shader_program() const {
    return mOpenGLShaderProgram;
  }


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIShader methods                   ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual const char *get_shader_language_name() const override
  {
    return "GLSL";
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
  explicit RHIGeometryShaderSeparate(const RHIGeometryShaderSeparate &source) = delete;

  RHIGeometryShaderSeparate &operator=(const RHIGeometryShaderSeparate &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLuint mOpenGLShaderProgram;  ///< OpenGL shader program, can be zero if no resource is allocated


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
