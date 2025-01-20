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
#include "rhi_vulkan/rhi_vulkan.h"
#include "rhi_vulkan/shader/rhicompute_shader_glsl.h"
#include "rhi_vulkan/shader/rhivertex_shader_glsl.h"
#include "rhi_vulkan/shader/rhifragment_shader_glsl.h"
#include "rhi_vulkan/shader/rhimesh_shader_glsl.h"
#include "rhi_vulkan/shader/rhitask_shader_glsl.h"
#include "rhi_vulkan/shader/rhitessellation_control_shader_glsl.h"
#include "rhi_vulkan/shader/rhitessellation_evaluation_shader_glsl.h"
#include "rhi_vulkan/shader/rhigraphics_program_glsl.h"
#include "rhi_vulkan/shader/rhigeometry_shader_glsl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 *  @brief
 *    GLSL graphics program class
 */
class RHIGraphicsProgramGlsl final : public rhi::RHIGraphicsProgram
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   *  @brief
   *    Constructor for traditional graphics program
   *
   *  @param[in] vulkanRhi
   *    Owner Vulkan RHI instance
   *  @param[in] vertexAttributes
   *    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
   *  @param[in] vertexShaderGlsl
   *    Vertex shader the graphics program is using, can be a null pointer
   *  @param[in] tessellationControlShaderGlsl
   *    Tessellation control shader the graphics program is using, can be a null pointer
   *  @param[in] tessellationEvaluationShaderGlsl
   *    Tessellation evaluation shader the graphics program is using, can be a null pointer
   *  @param[in] geometryShaderGlsl
   *    Geometry shader the graphics program is using, can be a null pointer
   *  @param[in] fragmentShaderGlsl
   *    Fragment shader the graphics program is using, can be a null pointer
   *
   *  @note
   *    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
   */
  RHIGraphicsProgramGlsl(RHIDevice& vulkanRhi, RHIVertexShaderGlsl* vertexShaderGlsl, RHITessellationControlShaderGlsl* tessellationControlShaderGlsl, RHITessellationEvaluationShaderGlsl* tessellationEvaluationShaderGlsl, RHIGeometryShaderGlsl* geometryShaderGlsl, RHIFragmentShaderGlsl* fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
   *  @brief
   *    Constructor for task and mesh shader based graphics program
   *
   *  @param[in] vulkanRhi
   *    Owner Vulkan RHI instance
   *  @param[in] taskShaderGlsl
   *    Task shader the graphics program is using, can be a null pointer
   *  @param[in] meshShaderGlsl
   *    Mesh shader the graphics program is using
   *  @param[in] fragmentShaderGlsl
   *    Fragment shader the graphics program is using, can be a null pointer
   *
   *  @note
   *    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
   */
  RHIGraphicsProgramGlsl(RHIDevice& vulkanRhi, RHITaskShaderGlsl* taskShaderGlsl, RHIMeshShaderGlsl& meshShaderGlsl, RHIFragmentShaderGlsl* fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
   *  @brief
   *    Destructor
   */
  ~RHIGraphicsProgramGlsl() override;

  //[-------------------------------------------------------]
  //[ Traditional graphics program                          ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    Return the GLSL vertex shader the graphics program is using
   *
   *  @return
   *    The GLSL vertex shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] RHIVertexShaderGlsl* get_vertex_shader_glsl() const;

  /**
   *  @brief
   *    Return the GLSL tessellation control shader the graphics program is using
   *
   *  @return
   *    The GLSL tessellation control shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] RHITessellationControlShaderGlsl* get_tessellation_control_shader_glsl() const;

  /**
   *  @brief
   *    Return the GLSL tessellation evaluation shader the graphics program is using
   *
   *  @return
   *    The GLSL tessellation evaluation shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] RHITessellationEvaluationShaderGlsl* get_tessellation_evaluation_shader_glsl() const;

  /**
   *  @brief
   *    Return the GLSL geometry shader the graphics program is using
   *
   *  @return
   *    The GLSL geometry shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] RHIGeometryShaderGlsl* get_geometry_shader_glsl() const;

  //[-------------------------------------------------------]
  //[ Both graphics programs                                ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    Return the GLSL fragment shader the graphics program is using
   *
   *  @return
   *    The GLSL fragment shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] RHIFragmentShaderGlsl* get_fragment_shader_glsl() const;

  //[-------------------------------------------------------]
  //[ Task and mesh shader based graphics program           ]
  //[-------------------------------------------------------]
  /**
   *  @brief
   *    Return the GLSL task shader the graphics program is using
   *
   *  @return
   *    The GLSL task shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] RHITaskShaderGlsl* get_task_shader_glsl() const;

  /**
   *  @brief
   *    Return the GLSL mesh shader the graphics program is using
   *
   *  @return
   *    The GLSL mesh shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
   */
  [[nodiscard]] RHIMeshShaderGlsl* get_mesh_shader_glsl() const;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIGraphicsProgram methods          ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] core::handle get_uniform_handle(const char*) override;


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  void self_destruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIGraphicsProgramGlsl(const RHIGraphicsProgramGlsl& source) = delete;
  RHIGraphicsProgramGlsl& operator =(const RHIGraphicsProgramGlsl& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Traditional graphics program
  /** Vertex shader the graphics program is using (we keep a reference to it), can be a null pointer */
  RHIVertexShaderGlsl*				  mVertexShaderGlsl;
  /** Tessellation control shader the graphics program is using (we keep a reference to it), can be a null pointer */
  RHITessellationControlShaderGlsl*	  mTessellationControlShaderGlsl;
  /** Tessellation evaluation shader the graphics program is using (we keep a reference to it), can be a null pointer */
  RHITessellationEvaluationShaderGlsl* mTessellationEvaluationShaderGlsl;
  /** Geometry shader the graphics program is using (we keep a reference to it), can be a null pointer */
  RHIGeometryShaderGlsl*				  mGeometryShaderGlsl;					
  // Both graphics programs
  /** Fragment shader the graphics program is using (we keep a reference to it), can be a null pointer */
  RHIFragmentShaderGlsl* mFragmentShaderGlsl;	
  // Task and mesh shader based graphics program
  /** Task shader the graphics program is using (we keep a reference to it), can be a null pointer */
  RHITaskShaderGlsl* mTaskShaderGlsl;
  /** Mesh shader the graphics program is using (we keep a reference to it), can be a null pointer */
  RHIMeshShaderGlsl* mMeshShaderGlsl;	
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
