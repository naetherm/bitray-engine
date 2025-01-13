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
#include "rhi_opengl/shader/separate/rhigraphics_program_separate.h"
#include "rhi_opengl/shader/separate/rhivertex_shader_separate.h"
#include "rhi_opengl/shader/separate/rhitessellation_control_shader_separate.h"
#include "rhi_opengl/shader/separate/rhitessellation_evaluation_shader_separate.h"
#include "rhi_opengl/shader/separate/rhigeometry_shader_separate.h"
#include "rhi_opengl/shader/separate/rhifragment_shader_separate.h"
#include "rhi_opengl/shader/separate/rhicompute_shader_separate.h"
#include "rhi_opengl/shader/separate/rhitask_shader_separate.h"
#include "rhi_opengl/shader/separate/rhimesh_shader_separate.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/rhiroot_signature.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIGraphicsProgramSeparate::RHIGraphicsProgramSeparate(RHIDevice& openGLRhi, const rhi::RHIRootSignature& rootSignature, RHIVertexShaderSeparate* vertexShaderSeparate, RHITessellationControlShaderSeparate* tessellationControlShaderSeparate, RHITessellationEvaluationShaderSeparate* tessellationEvaluationShaderSeparate, RHIGeometryShaderSeparate* geometryShaderSeparate, RHIFragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLProgramPipeline(0)
// Traditional graphics program
, mVertexShaderSeparate(vertexShaderSeparate)
, mTessellationControlShaderSeparate(tessellationControlShaderSeparate)
, mTessellationEvaluationShaderSeparate(tessellationEvaluationShaderSeparate)
, mGeometryShaderSeparate(geometryShaderSeparate)
// Both graphics programs
, mFragmentShaderSeparate(fragmentShaderSeparate)
// Task and mesh shader based graphics program
, mTaskShaderSeparate(nullptr)
, mMeshShaderSeparate(nullptr) {
  // Create the OpenGL program pipeline
  glGenProgramPipelines(1, &mOpenGLProgramPipeline);

  // If the "GL_ARB_direct_state_access" nor "GL_EXT_direct_state_access" extension is available, we need to change OpenGL states during resource creation (nasty thing)
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently used OpenGL program pipeline
  GLint openGLProgramPipelineBackup = 0;
#endif
  if (nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT) {
#ifdef RHI_OPENGL_STATE_CLEANUP
    glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);
#endif
    glBindProgramPipeline(mOpenGLProgramPipeline);
  }

    // Add references to the provided shaders
#define USE_PROGRAM_STAGES(ShaderBit, ShaderSeparate) if (nullptr != ShaderSeparate) { ShaderSeparate->add_ref(); glUseProgramStages(mOpenGLProgramPipeline, ShaderBit, ShaderSeparate->get_opengl_shader_program()); }
  USE_PROGRAM_STAGES(GL_VERTEX_SHADER_BIT, mVertexShaderSeparate)
  USE_PROGRAM_STAGES(GL_TESS_CONTROL_SHADER_BIT, mTessellationControlShaderSeparate)
  USE_PROGRAM_STAGES(GL_TESS_EVALUATION_SHADER_BIT, mTessellationEvaluationShaderSeparate)
  USE_PROGRAM_STAGES(GL_GEOMETRY_SHADER_BIT, mGeometryShaderSeparate)
  USE_PROGRAM_STAGES(GL_FRAGMENT_SHADER_BIT, mFragmentShaderSeparate)
#undef USE_PROGRAM_STAGES

  // Validate program pipeline
  glValidateProgramPipeline(mOpenGLProgramPipeline);
  GLint validateStatus = 0;
  glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
  if (GL_TRUE == validateStatus)
  {
    // We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
    // -> Use modern GLSL:
    //    "layout(location = 0) out vec4 ColorOutput0;"
    //    "layout(location = 1) out vec4 ColorOutput1;"
    // -> Use legacy GLSL if necessary:
    //    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
    //    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

    // The actual locations assigned to uniform variables are not known until the program object is linked successfully
    // -> So we have to build a root signature parameter index -> uniform location mapping here
    const rhi::RootSignatureDescriptor& rootSignatureData = static_cast<const RHIRootSignature &>(rootSignature).get_root_signature();
    const core::uint32 numberOfRootParameters = rootSignatureData.numberOfParameters;
    core::uint32 uniformBlockBindingIndex = 0;
    for (core::uint32 rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex) {
      const rhi::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
      if (rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
        BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
        const core::uint32 numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
        for (core::uint32 descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex) {
          const rhi::DescriptorRange& descriptorRange = reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

          // Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
          if (rhi::DescriptorRangeType::UBV == descriptorRange.rangeType) {
#define BIND_UNIFORM_BLOCK(ShaderSeparate) if (nullptr != ShaderSeparate) Helper::bind_uniform_block(descriptorRange, ShaderSeparate->get_opengl_shader_program(), uniformBlockBindingIndex);
            switch (descriptorRange.shaderVisibility) {
              case rhi::ShaderVisibility::ALL:
              case rhi::ShaderVisibility::ALL_GRAPHICS:
                BIND_UNIFORM_BLOCK(mVertexShaderSeparate)
                BIND_UNIFORM_BLOCK(mTessellationControlShaderSeparate)
                BIND_UNIFORM_BLOCK(mTessellationEvaluationShaderSeparate)
                BIND_UNIFORM_BLOCK(mGeometryShaderSeparate)
                BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
                break;

              case rhi::ShaderVisibility::VERTEX:
                BIND_UNIFORM_BLOCK(mVertexShaderSeparate)
                break;

              case rhi::ShaderVisibility::TESSELLATION_CONTROL:
                BIND_UNIFORM_BLOCK(mTessellationControlShaderSeparate)
                break;

              case rhi::ShaderVisibility::TESSELLATION_EVALUATION:
                BIND_UNIFORM_BLOCK(mTessellationEvaluationShaderSeparate)
                break;

              case rhi::ShaderVisibility::GEOMETRY:
                BIND_UNIFORM_BLOCK(mGeometryShaderSeparate)
                break;

              case rhi::ShaderVisibility::FRAGMENT:
                BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
                break;

              case rhi::ShaderVisibility::TASK:
              case rhi::ShaderVisibility::MESH:
              case rhi::ShaderVisibility::COMPUTE:
                BE_ASSERT(false, "Invalid OpenGL shader visibility")
                break;
            }
#undef BIND_UNIFORM_BLOCK
            ++uniformBlockBindingIndex;
          }
          else if (rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType) {
#define BIND_UNIFORM_LOCATION(ShaderSeparate) if (nullptr != ShaderSeparate) Helper::bind_uniform_location(descriptorRange, mOpenGLProgramPipeline, ShaderSeparate->get_opengl_shader_program());
            switch (descriptorRange.shaderVisibility) {
              case rhi::ShaderVisibility::ALL:
              case rhi::ShaderVisibility::ALL_GRAPHICS:
                BIND_UNIFORM_LOCATION(mVertexShaderSeparate)
                BIND_UNIFORM_LOCATION(mTessellationControlShaderSeparate)
                BIND_UNIFORM_LOCATION(mTessellationEvaluationShaderSeparate)
                BIND_UNIFORM_LOCATION(mGeometryShaderSeparate)
                BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
                break;

              case rhi::ShaderVisibility::VERTEX:
                BIND_UNIFORM_LOCATION(mVertexShaderSeparate)
                break;

              case rhi::ShaderVisibility::TESSELLATION_CONTROL:
                BIND_UNIFORM_LOCATION(mTessellationControlShaderSeparate)
                break;

              case rhi::ShaderVisibility::TESSELLATION_EVALUATION:
                BIND_UNIFORM_LOCATION(mTessellationEvaluationShaderSeparate)
                break;

              case rhi::ShaderVisibility::GEOMETRY:
                BIND_UNIFORM_LOCATION(mGeometryShaderSeparate)
                break;

              case rhi::ShaderVisibility::FRAGMENT:
                BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
                break;

              case rhi::ShaderVisibility::TASK:
              case rhi::ShaderVisibility::MESH:
              case rhi::ShaderVisibility::COMPUTE:
                BE_ASSERT(false, "Invalid OpenGL shader visibility")
                break;
            }
#undef BIND_UNIFORM_LOCATION
          }
        }
      }
    }
  }
  else
  {
    // Error, program pipeline validation failed!

    // get the length of the information (including a null termination)
    GLint informationLength = 0;
    glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_INFO_LOG_LENGTH, &informationLength);
    if (informationLength > 1) {
      // Allocate memory for the information
      const rhi::RHIContext& context = openGLRhi.get_context();
      char* informationLog = re_typed_alloc<char>(informationLength);

      // get the information
      glGetProgramPipelineInfoLog(mOpenGLProgramPipeline, informationLength, nullptr, informationLog);

      // Output the debug string
      BE_LOG(Critical, informationLog)

      // Cleanup information memory
      re_free(informationLog);
    }
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous used OpenGL program pipeline
  if (nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT) {
    glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
  }
#endif

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLProgramPipeline && openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
    glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
  }
#endif
}

RHIGraphicsProgramSeparate::RHIGraphicsProgramSeparate(RHIDevice& openGLRhi, const rhi::RHIRootSignature& rootSignature, RHITaskShaderSeparate* taskShaderSeparate, RHIMeshShaderSeparate& meshShaderSeparate, RHIFragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLProgramPipeline(0)
// Traditional graphics program
, mVertexShaderSeparate(nullptr)
, mTessellationControlShaderSeparate(nullptr)
, mTessellationEvaluationShaderSeparate(nullptr)
, mGeometryShaderSeparate(nullptr)
// Both graphics programs
, mFragmentShaderSeparate(fragmentShaderSeparate)
// Task and mesh shader based graphics program
, mTaskShaderSeparate(taskShaderSeparate)
, mMeshShaderSeparate(&meshShaderSeparate) {
  // Create the OpenGL program pipeline
  glGenProgramPipelines(1, &mOpenGLProgramPipeline);

  // If the "GL_ARB_direct_state_access" nor "GL_EXT_direct_state_access" extension is available, we need to change OpenGL states during resource creation (nasty thing)
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently used OpenGL program pipeline
  GLint openGLProgramPipelineBackup = 0;
#endif
  if (nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT) {
#ifdef RHI_OPENGL_STATE_CLEANUP
    glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);
#endif
    glBindProgramPipeline(mOpenGLProgramPipeline);
  }

    // Add references to the provided shaders
#define USE_PROGRAM_STAGES(ShaderBit, ShaderSeparate) if (nullptr != ShaderSeparate) { ShaderSeparate->add_ref(); glUseProgramStages(mOpenGLProgramPipeline, ShaderBit, ShaderSeparate->get_opengl_shader_program()); }
  USE_PROGRAM_STAGES(GL_TASK_SHADER_BIT_NV, mTaskShaderSeparate)
  USE_PROGRAM_STAGES(GL_MESH_SHADER_BIT_NV, mMeshShaderSeparate)
  USE_PROGRAM_STAGES(GL_FRAGMENT_SHADER_BIT, mFragmentShaderSeparate)
#undef USE_PROGRAM_STAGES

  // Validate program pipeline
  glValidateProgramPipeline(mOpenGLProgramPipeline);
  GLint validateStatus = 0;
  glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
  if (GL_TRUE == validateStatus) {
    // We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
    // -> Use modern GLSL:
    //    "layout(location = 0) out vec4 ColorOutput0;"
    //    "layout(location = 1) out vec4 ColorOutput1;"
    // -> Use legacy GLSL if necessary:
    //    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
    //    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

    // The actual locations assigned to uniform variables are not known until the program object is linked successfully
    // -> So we have to build a root signature parameter index -> uniform location mapping here
    const rhi::RootSignatureDescriptor& rootSignatureData = static_cast<const RHIRootSignature &>(rootSignature).get_root_signature();
    const core::uint32 numberOfRootParameters = rootSignatureData.numberOfParameters;
    core::uint32 uniformBlockBindingIndex = 0;
    for (core::uint32 rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex) {
      const rhi::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
      if (rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
        BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
        const core::uint32 numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
        for (core::uint32 descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex) {
          const rhi::DescriptorRange& descriptorRange = reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

          // Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
          if (rhi::DescriptorRangeType::UBV == descriptorRange.rangeType) {
#define BIND_UNIFORM_BLOCK(ShaderSeparate) if (nullptr != ShaderSeparate) Helper::bind_uniform_block(descriptorRange, ShaderSeparate->get_opengl_shader_program(), uniformBlockBindingIndex);
            switch (descriptorRange.shaderVisibility) {
              case rhi::ShaderVisibility::ALL:
              case rhi::ShaderVisibility::ALL_GRAPHICS:
                BIND_UNIFORM_BLOCK(mTaskShaderSeparate)
                BIND_UNIFORM_BLOCK(mMeshShaderSeparate)
                break;

              case rhi::ShaderVisibility::FRAGMENT:
                BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
                break;

              case rhi::ShaderVisibility::TASK:
                BIND_UNIFORM_BLOCK(mTaskShaderSeparate)
                break;

              case rhi::ShaderVisibility::MESH:
                BIND_UNIFORM_BLOCK(mMeshShaderSeparate)
                break;

              case rhi::ShaderVisibility::VERTEX:
              case rhi::ShaderVisibility::TESSELLATION_CONTROL:
              case rhi::ShaderVisibility::TESSELLATION_EVALUATION:
              case rhi::ShaderVisibility::GEOMETRY:
              case rhi::ShaderVisibility::COMPUTE:
                BE_ASSERT(false, "Invalid OpenGL shader visibility")
                break;
            }
#undef BIND_UNIFORM_BLOCK
            ++uniformBlockBindingIndex;
          }
          else if (rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType) {
#define BIND_UNIFORM_LOCATION(ShaderSeparate) if (nullptr != ShaderSeparate) Helper::bind_uniform_location(descriptorRange, mOpenGLProgramPipeline, ShaderSeparate->get_opengl_shader_program());
            switch (descriptorRange.shaderVisibility) {
              case rhi::ShaderVisibility::ALL:
              case rhi::ShaderVisibility::ALL_GRAPHICS:
                BIND_UNIFORM_LOCATION(mTaskShaderSeparate)
                BIND_UNIFORM_LOCATION(mMeshShaderSeparate)
                break;

              case rhi::ShaderVisibility::FRAGMENT:
                BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
                break;

              case rhi::ShaderVisibility::TASK:
                BIND_UNIFORM_LOCATION(mTaskShaderSeparate)
                break;

              case rhi::ShaderVisibility::MESH:
                BIND_UNIFORM_LOCATION(mMeshShaderSeparate)
                break;

              case rhi::ShaderVisibility::VERTEX:
              case rhi::ShaderVisibility::TESSELLATION_CONTROL:
              case rhi::ShaderVisibility::TESSELLATION_EVALUATION:
              case rhi::ShaderVisibility::GEOMETRY:
              case rhi::ShaderVisibility::COMPUTE:
                BE_ASSERT(false, "Invalid OpenGL shader visibility")
                break;
            }
#undef BIND_UNIFORM_LOCATION
          }
        }
      }
    }
  } else {
    // Error, program pipeline validation failed!

    // get the length of the information (including a null termination)
    GLint informationLength = 0;
    glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_INFO_LOG_LENGTH, &informationLength);
    if (informationLength > 1) {
      // Allocate memory for the information
      const rhi::RHIContext& context = openGLRhi.get_context();
      char* informationLog = re_typed_alloc<char>(informationLength);

      // get the information
      glGetProgramPipelineInfoLog(mOpenGLProgramPipeline, informationLength, nullptr, informationLog);

      // Output the debug string
      BE_LOG(Critical, informationLog)

      // Cleanup information memory
      re_free(informationLog);
    }
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous used OpenGL program pipeline
  if (nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT) {
    glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
  }
#endif

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLProgramPipeline && openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
    glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
  }
#endif
}

RHIGraphicsProgramSeparate::~RHIGraphicsProgramSeparate() {
  // Destroy the OpenGL program pipeline
  glDeleteProgramPipelines(1, &mOpenGLProgramPipeline);

  // release the shader references
  if (nullptr != mVertexShaderSeparate) {
    mVertexShaderSeparate->release();
  }
  if (nullptr != mTessellationControlShaderSeparate) {
    mTessellationControlShaderSeparate->release();
  }
  if (nullptr != mTessellationEvaluationShaderSeparate) {
    mTessellationEvaluationShaderSeparate->release();
  }
  if (nullptr != mGeometryShaderSeparate) {
    mGeometryShaderSeparate->release();
  }
  if (nullptr != mFragmentShaderSeparate) {
    mFragmentShaderSeparate->release();
  }
  if (nullptr != mTaskShaderSeparate) {
    mTaskShaderSeparate->release();
  }
  if (nullptr != mMeshShaderSeparate) {
    mMeshShaderSeparate->release();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
