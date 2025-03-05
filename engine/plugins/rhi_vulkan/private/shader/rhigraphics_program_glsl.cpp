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
#include "rhi_vulkan/shader/rhigraphics_program_glsl.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIGraphicsProgramGlsl::RHIGraphicsProgramGlsl(RHIDevice &vulkanRhi, RHIVertexShaderGlsl *vertexShaderGlsl, RHITessellationControlShaderGlsl *tessellationControlShaderGlsl, RHITessellationEvaluationShaderGlsl *tessellationEvaluationShaderGlsl, RHIGeometryShaderGlsl *geometryShaderGlsl, RHIFragmentShaderGlsl *fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGraphicsProgram(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
// Traditional graphics program
, mVertexShaderGlsl(vertexShaderGlsl)
, mTessellationControlShaderGlsl(tessellationControlShaderGlsl)
, mTessellationEvaluationShaderGlsl(tessellationEvaluationShaderGlsl)
, mGeometryShaderGlsl(geometryShaderGlsl)
// Both graphics programs
, mFragmentShaderGlsl(fragmentShaderGlsl)
// Task and mesh shader based graphics program
, mTaskShaderGlsl(nullptr)
, mMeshShaderGlsl(nullptr) {
  // Add references to the provided shaders
  if (nullptr != mVertexShaderGlsl) {
    mVertexShaderGlsl->add_ref();
  }
  if (nullptr != mTessellationControlShaderGlsl) {
    mTessellationControlShaderGlsl->add_ref();
  }
  if (nullptr != mTessellationEvaluationShaderGlsl) {
    mTessellationEvaluationShaderGlsl->add_ref();
  }
  if (nullptr != mGeometryShaderGlsl) {
    mGeometryShaderGlsl->add_ref();
  }
  if (nullptr != mFragmentShaderGlsl) {
    mFragmentShaderGlsl->add_ref();
  }
}

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
RHIGraphicsProgramGlsl::RHIGraphicsProgramGlsl(RHIDevice &vulkanRhi, RHITaskShaderGlsl *taskShaderGlsl, RHIMeshShaderGlsl &meshShaderGlsl, RHIFragmentShaderGlsl *fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGraphicsProgram(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
// Traditional graphics program
, mVertexShaderGlsl(nullptr)
, mTessellationControlShaderGlsl(nullptr)
, mTessellationEvaluationShaderGlsl(nullptr)
, mGeometryShaderGlsl(nullptr)
// Both graphics programs
, mFragmentShaderGlsl(fragmentShaderGlsl)
// Task and mesh shader based graphics program
, mTaskShaderGlsl(taskShaderGlsl)
, mMeshShaderGlsl(&meshShaderGlsl) {
  // Add references to the provided shaders
  if (nullptr != mTaskShaderGlsl) {
    mTaskShaderGlsl->add_ref();
  }
  mMeshShaderGlsl->add_ref();
  if (nullptr != mFragmentShaderGlsl) {
    mFragmentShaderGlsl->add_ref();
  }
}

/**
*  @brief
*    Destructor
*/
RHIGraphicsProgramGlsl::~RHIGraphicsProgramGlsl() {
// Release the shader references
  if (nullptr != mVertexShaderGlsl) {
    mVertexShaderGlsl->release();
  }
  if (nullptr != mTessellationControlShaderGlsl) {
    mTessellationControlShaderGlsl->release();
  }
  if (nullptr != mTessellationEvaluationShaderGlsl) {
    mTessellationEvaluationShaderGlsl->release();
  }
  if (nullptr != mGeometryShaderGlsl) {
    mGeometryShaderGlsl->release();
  }
  if (nullptr != mFragmentShaderGlsl) {
    mFragmentShaderGlsl->release();
  }
  if (nullptr != mTaskShaderGlsl) {
    mTaskShaderGlsl->release();
  }
  if (nullptr != mMeshShaderGlsl) {
    mMeshShaderGlsl->release();
  }
}

RHIVertexShaderGlsl* RHIGraphicsProgramGlsl::get_vertex_shader_glsl() const {
  return mVertexShaderGlsl;
}

RHITessellationControlShaderGlsl* RHIGraphicsProgramGlsl::get_tessellation_control_shader_glsl() const {
  return mTessellationControlShaderGlsl;
}

RHITessellationEvaluationShaderGlsl* RHIGraphicsProgramGlsl::get_tessellation_evaluation_shader_glsl() const {
  return mTessellationEvaluationShaderGlsl;
}

RHIGeometryShaderGlsl* RHIGraphicsProgramGlsl::get_geometry_shader_glsl() const {
  return mGeometryShaderGlsl;
}

RHIFragmentShaderGlsl* RHIGraphicsProgramGlsl::get_fragment_shader_glsl() const {
  return mFragmentShaderGlsl;
}

RHITaskShaderGlsl* RHIGraphicsProgramGlsl::get_task_shader_glsl() const {
  return mTaskShaderGlsl;
}

RHIMeshShaderGlsl* RHIGraphicsProgramGlsl::get_mesh_shader_glsl() const {
  return mMeshShaderGlsl;
}

core::handle RHIGraphicsProgramGlsl::get_uniform_handle(const char*) override {
  BE_ASSERT(false, "The Vulkan RHI graphics program GLSL implementation doesn't have legacy uniform support")
  return NULL_HANDLE;
}

void RHIGraphicsProgramGlsl::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
