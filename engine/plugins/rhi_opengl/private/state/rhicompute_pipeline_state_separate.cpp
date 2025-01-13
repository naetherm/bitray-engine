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
#include "rhi_opengl/state/rhicompute_pipeline_state_separate.h"
#include "rhi_opengl/shader/separate/rhicompute_shader_separate.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/rhiroot_signature.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/mapping.h"
#include "rhi_opengl/helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIComputePipelineStateSeparate::RHIComputePipelineStateSeparate(RHIDevice& openGLRhi, const rhi::RHIRootSignature& rootSignature, RHIComputeShaderSeparate& computeShaderSeparate, core::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
RHIComputePipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLProgramPipeline(0),
mComputeShaderSeparate(computeShaderSeparate)
  {
    // Create the OpenGL program pipeline
    glGenProgramPipelines(1, &mOpenGLProgramPipeline);

  // If the "GL_ARB_direct_state_access" nor "GL_EXT_direct_state_access" extension is available, we need to change OpenGL states during resource creation (nasty thing)
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently used OpenGL program pipeline
				GLint openGLProgramPipelineBackup = 0;
#endif
  if (nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT)
  {
#ifdef RHI_OPENGL_STATE_CLEANUP
    glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);
#endif
    glBindProgramPipeline(mOpenGLProgramPipeline);
  }

  // Add reference to the provided compute shader
  computeShaderSeparate.add_ref();
  glUseProgramStages(mOpenGLProgramPipeline, GL_COMPUTE_SHADER_BIT, computeShaderSeparate.get_opengl_shader_program());

  // Validate program pipeline
  glValidateProgramPipeline(mOpenGLProgramPipeline);
  GLint validateStatus = 0;
  glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
  if (true)	// TODO(naetherm) Compute shader: Validate status always returns failure without log when using a compute shader? AMD 290X Radeon software version 18.7.1.
  //if (GL_TRUE == validateStatus)
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
    for (core::uint32 rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex)
    {
      const rhi::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
      if (rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType)
      {
        BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
        const core::uint32 numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
        for (core::uint32 descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex)
        {
          const rhi::DescriptorRange& descriptorRange = reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

          // Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
          if (rhi::DescriptorRangeType::UBV == descriptorRange.rangeType)
          {
            switch (descriptorRange.shaderVisibility)
            {
              case rhi::ShaderVisibility::ALL_GRAPHICS:
              case rhi::ShaderVisibility::VERTEX:
              case rhi::ShaderVisibility::TESSELLATION_CONTROL:
              case rhi::ShaderVisibility::TESSELLATION_EVALUATION:
              case rhi::ShaderVisibility::GEOMETRY:
              case rhi::ShaderVisibility::FRAGMENT:
              case rhi::ShaderVisibility::TASK:
              case rhi::ShaderVisibility::MESH:
                BE_ASSERT(false, "Invalid OpenGL shader visibility")
                break;

              case rhi::ShaderVisibility::ALL:
              case rhi::ShaderVisibility::COMPUTE:
                Helper::bind_uniform_block(descriptorRange, mComputeShaderSeparate.get_opengl_shader_program(),
                                           uniformBlockBindingIndex);
                break;
            }
            ++uniformBlockBindingIndex;
          }
          else if (rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType)
          {
            switch (descriptorRange.shaderVisibility)
            {
              case rhi::ShaderVisibility::ALL_GRAPHICS:
              case rhi::ShaderVisibility::VERTEX:
              case rhi::ShaderVisibility::TESSELLATION_CONTROL:
              case rhi::ShaderVisibility::TESSELLATION_EVALUATION:
              case rhi::ShaderVisibility::GEOMETRY:
              case rhi::ShaderVisibility::FRAGMENT:
              case rhi::ShaderVisibility::TASK:
              case rhi::ShaderVisibility::MESH:
                BE_ASSERT(false, "Invalid OpenGL shader visibility")
                break;

              case rhi::ShaderVisibility::ALL:
              case rhi::ShaderVisibility::COMPUTE:
                Helper::bind_uniform_location(descriptorRange, mOpenGLProgramPipeline,
                                              mComputeShaderSeparate.get_opengl_shader_program());
                break;
            }
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
    if (informationLength > 1)
    {
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
				if (nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT)
				{
					glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
				}
#endif

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.get_extensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Compute PSO", 14)	// 14 = "Compute PSO: " including terminating zero
					glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
				}
#endif
  }

RHIComputePipelineStateSeparate::~RHIComputePipelineStateSeparate()
{
  // Destroy the OpenGL program pipeline
  glDeleteProgramPipelines(1, &mOpenGLProgramPipeline);

  // release the compute shader reference
  mComputeShaderSeparate.release();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
