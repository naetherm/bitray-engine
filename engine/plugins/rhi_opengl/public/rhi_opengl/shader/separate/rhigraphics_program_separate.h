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

class RHIVertexShaderSeparate;

class RHITessellationControlShaderSeparate;

class RHITessellationEvaluationShaderSeparate;

class RHIGeometryShaderSeparate;

class RHIFragmentShaderSeparate;

class RHIComputeShaderSeparate;

class RHITaskShaderSeparate;

class RHIMeshShaderSeparate;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Separate graphics program class
 */
class RHIGraphicsProgramSeparate : public rhi::RHIGraphicsProgram {


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
  RHIGraphicsProgramSeparate(RHIDevice &openGLRhi, const rhi::RHIRootSignature &rootSignature,
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
  RHIGraphicsProgramSeparate(RHIDevice &openGLRhi, const rhi::RHIRootSignature &rootSignature,
                             RHITaskShaderSeparate *taskShaderSeparate, RHIMeshShaderSeparate &meshShaderSeparate,
                             RHIFragmentShaderSeparate *fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
   * @brief
   * Destructor
   */
  ~RHIGraphicsProgramSeparate() override;

  /**
   * @brief
   * Return the OpenGL program pipeline
   * 
   * @return
   * The OpenGL program pipeline, can be zero if no resource is allocated, do not destroy the returned resource
   */
  [[nodiscard]] inline GLuint get_opengl_program_pipeline() const {
    return mOpenGLProgramPipeline;
  }

  /**
   * @brief
   * Return the vertex shader the program is using
   * 
   * @return
   * Vertex shader the program is using, can be a null pointer, don't destroy the instance
   */
  [[nodiscard]] inline RHIVertexShaderSeparate *get_vertex_shader_separate() const {
    return mVertexShaderSeparate;
  }


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIGraphicsProgram methods          ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual core::handle get_uniform_handle(const char *) override
  {
    BE_ASSERT(false, "The OpenGL RHI graphics program separate implementation doesn't have legacy uniform support")
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
  GLuint mOpenGLProgramPipeline;  ///< OpenGL program pipeline ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
  // Traditional graphics program
  RHIVertexShaderSeparate *mVertexShaderSeparate;          ///< Vertex shader the program is using (we keep a reference to it), can be a null pointer
  RHITessellationControlShaderSeparate *mTessellationControlShaderSeparate;    ///< Tessellation control shader the program is using (we keep a reference to it), can be a null pointer
  RHITessellationEvaluationShaderSeparate *mTessellationEvaluationShaderSeparate;  ///< Tessellation evaluation shader the program is using (we keep a reference to it), can be a null pointer
  RHIGeometryShaderSeparate *mGeometryShaderSeparate;          ///< Geometry shader the program is using (we keep a reference to it), can be a null pointer
  // Both graphics programs
  RHIFragmentShaderSeparate *mFragmentShaderSeparate;  ///< Fragment shader the program is using (we keep a reference to it), can be a null pointer
  // Task and mesh shader based graphics program
  RHITaskShaderSeparate *mTaskShaderSeparate;  ///< Task shader the program is using (we keep a reference to it), can be a null pointer
  RHIMeshShaderSeparate *mMeshShaderSeparate;  ///< Mesh shader the program is using (we keep a reference to it), can be a null pointer


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIGraphicsProgramSeparate(const RHIGraphicsProgramSeparate &source) = delete;

  RHIGraphicsProgramSeparate &operator=(const RHIGraphicsProgramSeparate &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
