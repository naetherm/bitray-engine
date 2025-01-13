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
#include "rhi_opengl/helper.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/extensions.h"
#include <rhi/texture/rhitexture_1d.h>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace {
namespace detail {
#ifdef RHI_OPENGL_GLSLTOSPIRV
static bool GlslangInitialized = false;

			// Settings from "glslang/StandAlone/resourceLimits.cpp"
			static constexpr TBuiltInResource DefaultTBuiltInResource =
			{
				32,		///< MaxLights
				6,		///< MaxClipPlanes
				32,		///< MaxTextureUnits
				32,		///< MaxTextureCoords
				64,		///< MaxVertexAttribs
				4096,	///< MaxVertexUniformComponents
				64,		///< MaxVaryingFloats
				32,		///< MaxVertexTextureImageUnits
				80,		///< MaxCombinedTextureImageUnits
				32,		///< MaxTextureImageUnits
				4096,	///< MaxFragmentUniformComponents
				32,		///< MaxDrawBuffers
				128,	///< MaxVertexUniformVectors
				8,		///< MaxVaryingVectors
				16,		///< MaxFragmentUniformVectors
				16,		///< MaxVertexOutputVectors
				15,		///< MaxFragmentInputVectors
				-8,		///< MinProgramTexelOffset
				7,		///< MaxProgramTexelOffset
				8,		///< MaxClipDistances
				65535,	///< MaxComputeWorkGroupCountX
				65535,	///< MaxComputeWorkGroupCountY
				65535,	///< MaxComputeWorkGroupCountZ
				1024,	///< MaxComputeWorkGroupSizeX
				1024,	///< MaxComputeWorkGroupSizeY
				64,		///< MaxComputeWorkGroupSizeZ
				1024,	///< MaxComputeUniformComponents
				16,		///< MaxComputeTextureImageUnits
				8,		///< MaxComputeImageUniforms
				8,		///< MaxComputeAtomicCounters
				1,		///< MaxComputeAtomicCounterBuffers
				60,		///< MaxVaryingComponents
				64,		///< MaxVertexOutputComponents
				64,		///< MaxGeometryInputComponents
				128,	///< MaxGeometryOutputComponents
				128,	///< MaxFragmentInputComponents
				8,		///< MaxImageUnits
				8,		///< MaxCombinedImageUnitsAndFragmentOutputs
				8,		///< MaxCombinedShaderOutputResources
				0,		///< MaxImageSamples
				0,		///< MaxVertexImageUniforms
				0,		///< MaxTessControlImageUniforms
				0,		///< MaxTessEvaluationImageUniforms
				0,		///< MaxGeometryImageUniforms
				8,		///< MaxFragmentImageUniforms
				8,		///< MaxCombinedImageUniforms
				16,		///< MaxGeometryTextureImageUnits
				256,	///< MaxGeometryOutputVertices
				1024,	///< MaxGeometryTotalOutputComponents
				1024,	///< MaxGeometryUniformComponents
				64,		///< MaxGeometryVaryingComponents
				128,	///< MaxTessControlInputComponents
				128,	///< MaxTessControlOutputComponents
				16,		///< MaxTessControlTextureImageUnits
				1024,	///< MaxTessControlUniformComponents
				4096,	///< MaxTessControlTotalOutputComponents
				128,	///< MaxTessEvaluationInputComponents
				128,	///< MaxTessEvaluationOutputComponents
				16,		///< MaxTessEvaluationTextureImageUnits
				1024,	///< MaxTessEvaluationUniformComponents
				120,	///< MaxTessPatchComponents
				32,		///< MaxPatchVertices
				64,		///< MaxTessGenLevel
				16,		///< MaxViewports
				0,		///< MaxVertexAtomicCounters
				0,		///< MaxTessControlAtomicCounters
				0,		///< MaxTessEvaluationAtomicCounters
				0,		///< MaxGeometryAtomicCounters
				8,		///< MaxFragmentAtomicCounters
				8,		///< MaxCombinedAtomicCounters
				1,		///< MaxAtomicCounterBindings
				0,		///< MaxVertexAtomicCounterBuffers
				0,		///< MaxTessControlAtomicCounterBuffers
				0,		///< MaxTessEvaluationAtomicCounterBuffers
				0,		///< MaxGeometryAtomicCounterBuffers
				1,		///< MaxFragmentAtomicCounterBuffers
				1,		///< MaxCombinedAtomicCounterBuffers
				16384,	///< MaxAtomicCounterBufferSize
				4,		///< MaxTransformFeedbackBuffers
				64,		///< MaxTransformFeedbackInterleavedComponents
				8,		///< MaxCullDistances
				8,		///< MaxCombinedClipAndCullDistances
				4,		///< MaxSamples
				256,	///< MaxMeshOutputVerticesNV
				512,	///< MaxMeshOutputPrimitivesNV
				32,		///< MaxMeshWorkGroupSizeX_NV
				1,		///< MaxMeshWorkGroupSizeY_NV
				1,		///< MaxMeshWorkGroupSizeZ_NV
				32,		///< MaxTaskWorkGroupSizeX_NV
				1,		///< MaxTaskWorkGroupSizeY_NV
				1,		///< MaxTaskWorkGroupSizeZ_NV
				4,		///< MaxMeshViewCountNV
        35068,
				{		///< limits
					1,	///< nonInductiveForLoops
					1,	///< whileLoops
					1,	///< doWhileLoops
					1,	///< generalUniformIndexing
					1,	///< generalAttributeMatrixVectorIndexing
					1,	///< generalVaryingIndexing
					1,	///< generalSamplerIndexing
					1,	///< generalVariableIndexing
					1,	///< generalConstantMatrixVectorIndexing
				}
			};
#endif
}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
void Helper::update_width_height(core::uint32 mipmapIndex, core::uint32 textureWidth, core::uint32 textureHeight, core::uint32 &width, core::uint32 &height) {
  rhi::RHITexture::get_mipmap_size(mipmapIndex, textureWidth, textureHeight);
  if (width > textureWidth) {
    width = textureWidth;
  }
  if (height > textureHeight) {
    height = textureHeight;
  }
}


void Helper::bind_uniform_block(const rhi::DescriptorRange &descriptorRange, core::uint32 openGLProgram, core::uint32 uniformBlockBindingIndex) {
  // Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension,
  // for backward compatibility, ask for the uniform block index
  const GLuint uniformBlockIndex = glGetUniformBlockIndex(openGLProgram, descriptorRange.baseShaderRegisterName);
  if (GL_INVALID_INDEX != uniformBlockIndex) {
    // Associate the uniform block with the given binding point
    glUniformBlockBinding(openGLProgram, uniformBlockIndex, uniformBlockBindingIndex);
  }
}

void Helper::bind_uniform_location(const rhi::DescriptorRange &descriptorRange, core::uint32 openGLProgramPipeline, core::uint32 openGLProgram) {
  const GLint uniformLocation = glGetUniformLocation(openGLProgram, descriptorRange.baseShaderRegisterName);
  if (uniformLocation >= 0) {
    // OpenGL/GLSL is not automatically assigning texture units to samplers, so, we have to take over this job
    // -> When using OpenGL or OpenGL ES 3 this is required
    // -> OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension supports explicit binding points ("layout(binding = 0)"
    //    in GLSL shader) , for backward compatibility we don't use it in here
    // -> When using Direct3D 9, 10, 11 or 12, the texture unit
    //    to use is usually defined directly within the shader by using the "register"-keyword
    // -> Use the "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" extension if possible to not change OpenGL states
    if (nullptr != glProgramUniform1i) {
      glProgramUniform1i(openGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
    }
    else if (nullptr != glProgramUniform1iEXT) {
      glProgramUniform1iEXT(openGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
    } else {
      // TODO(naetherm) There's room for binding API call related optimization in here (will certainly be no huge overall efficiency gain)
#ifdef RHI_OPENGL_STATE_CLEANUP
      // Backup the currently used OpenGL program
      GLint openGLProgramBackup = 0;
      glGetProgramPipelineiv(openGLProgramPipeline, GL_ACTIVE_PROGRAM, &openGLProgramBackup);
      if (static_cast<core::uint32>(openGLProgramBackup) == openGLProgram)
      {
        // Set uniform, please note that for this our program must be the currently used one
        glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
      }
      else
      {
        // Set uniform, please note that for this our program must be the currently used one
        glActiveShaderProgram(openGLProgramPipeline, openGLProgram);
        glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));

        // Be polite and restore the previous used OpenGL program
        glActiveShaderProgram(openGLProgramPipeline, static_cast<GLuint>(openGLProgramBackup));
      }
#else
      glActiveShaderProgram(openGLProgramPipeline, openGLProgram);
      glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
#endif
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
