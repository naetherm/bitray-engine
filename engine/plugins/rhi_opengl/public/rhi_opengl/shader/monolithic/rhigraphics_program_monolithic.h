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

class RHIVertexShaderMonolithic;

class RHITessellationControlShaderMonolithic;

class RHITessellationEvaluationShaderMonolithic;

class RHIFragmentShaderMonolithic;

class RHIGeometryShaderMonolithic;

class RHITaskShaderMonolithic;

class RHIMeshShaderMonolithic;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Monolithic graphics program class
 */
class RHIGraphicsProgramMonolithic : public rhi::RHIGraphicsProgram {


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
   * @param[in] vertexAttributes
   * Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
   * @param[in] vertexShaderMonolithic
   * Vertex shader the graphics program is using, can be a null pointer
   * @param[in] tessellationControlShaderMonolithic
   * Tessellation control shader the graphics program is using, can be a null pointer
   * @param[in] tessellationEvaluationShaderMonolithic
   * Tessellation evaluation shader the graphics program is using, can be a null pointer
   * @param[in] geometryShaderMonolithic
   * Geometry shader the graphics program is using, can be a null pointer
   * @param[in] fragmentShaderMonolithic
   * Fragment shader the graphics program is using, can be a null pointer
   * 
   * @note
   * - The graphics program keeps a reference to the provided shaders and releases it when no longer required
   */
  RHIGraphicsProgramMonolithic(RHIDevice &openGLRhi, const rhi::RHIRootSignature &rootSignature,
                               const rhi::VertexAttributes &vertexAttributes,
                               RHIVertexShaderMonolithic *vertexShaderMonolithic,
                               RHITessellationControlShaderMonolithic *tessellationControlShaderMonolithic,
                               RHITessellationEvaluationShaderMonolithic *tessellationEvaluationShaderMonolithic,
                               RHIGeometryShaderMonolithic *geometryShaderMonolithic,
                               RHIFragmentShaderMonolithic *fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Constructor for task and mesh shader based graphics program
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] rootSignature
   * Root signature
   * @param[in] taskShaderMonolithic
   * Task shader the graphics program is using, can be a null pointer
   * @param[in] meshShaderMonolithic
   * Mesh shader the graphics program is using
   * @param[in] fragmentShaderMonolithic
   * Fragment shader the graphics program is using, can be a null pointer
   * 
   * @note
   * - The graphics program keeps a reference to the provided shaders and releases it when no longer required
   */
  RHIGraphicsProgramMonolithic(RHIDevice &openGLRhi, const rhi::RHIRootSignature &rootSignature,
                               RHITaskShaderMonolithic *taskShaderMonolithic,
                               RHIMeshShaderMonolithic &meshShaderMonolithic,
                               RHIFragmentShaderMonolithic *fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Destructor
   */
  ~RHIGraphicsProgramMonolithic() override;

  /**
   * @brief
   * Return the OpenGL program
   * 
   * @return
   * The OpenGL program, can be zero if no resource is allocated, do not destroy the returned resource
   */
  [[nodiscard]] inline GLuint getOpenGLProgram() const {
    return mOpenGLProgram;
  }

  /**
   * @brief
   * Return the draw ID uniform location
   * 
   * @return
   * Draw ID uniform location, -1 if there's no such uniform
   */
  [[nodiscard]] inline GLint get_draw_id_uniform_location() const {
    return mDrawIdUniformLocation;
  }


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIGraphicsProgram methods          ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual core::handle get_uniform_handle(const char *) override
  {
    BE_ASSERT(false, "The OpenGL RHI graphics program monolithic implementation doesn't have legacy uniform support")
    return NULL_HANDLE;
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
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint mOpenGLProgram;  ///< OpenGL program, can be zero if no resource is allocated


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIGraphicsProgramMonolithic(const RHIGraphicsProgramMonolithic &source) = delete;

  RHIGraphicsProgramMonolithic &operator=(const RHIGraphicsProgramMonolithic &source) = delete;

  void linkProgram(RHIDevice &openGLRhi, const rhi::RHIRootSignature &rootSignature);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLint mDrawIdUniformLocation;  ///< Draw ID uniform location, used for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
