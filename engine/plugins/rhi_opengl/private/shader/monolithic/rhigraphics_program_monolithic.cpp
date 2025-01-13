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
#include "rhi_opengl/shader/monolithic/rhigraphics_program_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhivertex_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhitessellation_control_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhitessellation_evaluation_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhifragment_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhigeometry_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhitask_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhimesh_shader_monolithic.h"
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/rhiroot_signature.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/shader_compiler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIGraphicsProgramMonolithic::RHIGraphicsProgramMonolithic(RHIDevice& openGLRhi, const rhi::RHIRootSignature& rootSignature, const rhi::VertexAttributes& vertexAttributes, RHIVertexShaderMonolithic* vertexShaderMonolithic, RHITessellationControlShaderMonolithic* tessellationControlShaderMonolithic, RHITessellationEvaluationShaderMonolithic* tessellationEvaluationShaderMonolithic, RHIGeometryShaderMonolithic* geometryShaderMonolithic, RHIFragmentShaderMonolithic* fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLProgram(glCreateProgram())
, mDrawIdUniformLocation(-1) {
  // Attach the shaders to the program
  // -> We don't need to keep a reference to the shader, to add and release at once to ensure a nice behaviour
  if (nullptr != vertexShaderMonolithic) {
    vertexShaderMonolithic->add_ref();
    glAttachShader(mOpenGLProgram, vertexShaderMonolithic->getOpenGLShader());
    vertexShaderMonolithic->release();
  }
  if (nullptr != tessellationControlShaderMonolithic) {
    tessellationControlShaderMonolithic->add_ref();
    glAttachShader(mOpenGLProgram, tessellationControlShaderMonolithic->getOpenGLShader());
    tessellationControlShaderMonolithic->release();
  }
  if (nullptr != tessellationEvaluationShaderMonolithic) {
    tessellationEvaluationShaderMonolithic->add_ref();
    glAttachShader(mOpenGLProgram, tessellationEvaluationShaderMonolithic->getOpenGLShader());
    tessellationEvaluationShaderMonolithic->release();
  }
  if (nullptr != geometryShaderMonolithic) {
    // Add a reference to the shader
    geometryShaderMonolithic->add_ref();

    // Attach the monolithic shader to the monolithic program
    glAttachShader(mOpenGLProgram, geometryShaderMonolithic->getOpenGLShader());

    // release the shader
    geometryShaderMonolithic->release();
  }
  if (nullptr != fragmentShaderMonolithic) {
    fragmentShaderMonolithic->add_ref();
    glAttachShader(mOpenGLProgram, fragmentShaderMonolithic->getOpenGLShader());
    fragmentShaderMonolithic->release();
  }

  { // Define the vertex array attribute binding locations ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 & 12 terminology)
    const core::uint32 numberOfVertexAttributes = vertexAttributes.numberOfAttributes;
    for (core::uint32 vertexAttribute = 0; vertexAttribute < numberOfVertexAttributes; ++vertexAttribute) {
      glBindAttribLocation(mOpenGLProgram, vertexAttribute, vertexAttributes.attributes[vertexAttribute].name);
    }
  }

  // Link the program
  linkProgram(openGLRhi, rootSignature);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLProgram && openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
    glObjectLabel(GL_PROGRAM, mOpenGLProgram, -1, detailedDebugName);
  }
#endif
}

RHIGraphicsProgramMonolithic::RHIGraphicsProgramMonolithic(RHIDevice& openGLRhi, const rhi::RHIRootSignature& rootSignature, RHITaskShaderMonolithic* taskShaderMonolithic, RHIMeshShaderMonolithic& meshShaderMonolithic, RHIFragmentShaderMonolithic* fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mOpenGLProgram(glCreateProgram())
, mDrawIdUniformLocation(-1) {
  // Attach the shaders to the program
  // -> We don't need to keep a reference to the shader, to add and release at once to ensure a nice behaviour
  if (nullptr != taskShaderMonolithic) {
    taskShaderMonolithic->add_ref();
    glAttachShader(mOpenGLProgram, taskShaderMonolithic->getOpenGLShader());
    taskShaderMonolithic->release();
  }
  meshShaderMonolithic.add_ref();
  glAttachShader(mOpenGLProgram, meshShaderMonolithic.getOpenGLShader());
  meshShaderMonolithic.release();
  if (nullptr != fragmentShaderMonolithic) {
    fragmentShaderMonolithic->add_ref();
    glAttachShader(mOpenGLProgram, fragmentShaderMonolithic->getOpenGLShader());
    fragmentShaderMonolithic->release();
  }

  // Link the program
  linkProgram(openGLRhi, rootSignature);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLProgram && openGLRhi.get_extensions().isGL_KHR_debug()) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
    glObjectLabel(GL_PROGRAM, mOpenGLProgram, -1, detailedDebugName);
  }
#endif
}

RHIGraphicsProgramMonolithic::~RHIGraphicsProgramMonolithic() {
  // Destroy the OpenGL program
  // -> A value of 0 for program will be silently ignored
  glDeleteShader(mOpenGLProgram);
}

void RHIGraphicsProgramMonolithic::linkProgram(RHIDevice& openGLRhi, const rhi::RHIRootSignature& rootSignature) {
  // Link the program
  glLinkProgram(mOpenGLProgram);

  // Check the link status
  GLint linked = GL_FALSE;
  glGetProgramiv(mOpenGLProgram, GL_LINK_STATUS, &linked);
  if (GL_TRUE == linked) {
    // We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
    // -> Use modern GLSL:
    //    "layout(location = 0) out vec4 ColorOutput0;"
    //    "layout(location = 1) out vec4 ColorOutput1;"
    // -> Use legacy GLSL if necessary:
    //    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
    //    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

    // get draw ID uniform location
    if (!openGLRhi.get_extensions().isGL_ARB_base_instance()) {
      mDrawIdUniformLocation = glGetUniformLocation(mOpenGLProgram, "drawIdUniform");
    }

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
            // Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension,
            // for backward compatibility, ask for the uniform block index
            const GLuint uniformBlockIndex = glGetUniformBlockIndex(mOpenGLProgram, descriptorRange.baseShaderRegisterName);
            if (GL_INVALID_INDEX != uniformBlockIndex)
            {
              // Associate the uniform block with the given binding point
              glUniformBlockBinding(mOpenGLProgram, uniformBlockIndex, uniformBlockBindingIndex);
              ++uniformBlockBindingIndex;
            }
          }
          else if (rhi::DescriptorRangeType::SAMPLER != descriptorRange.rangeType) {
            const GLint uniformLocation = glGetUniformLocation(mOpenGLProgram, descriptorRange.baseShaderRegisterName);
            if (uniformLocation >= 0) {
              // OpenGL/GLSL is not automatically assigning texture units to samplers, so, we have to take over this job
              // -> When using OpenGL or OpenGL ES 3 this is required
              // -> OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension supports explicit binding points ("layout(binding = 0)"
              //    in GLSL shader) , for backward compatibility we don't use it in here
              // -> When using Direct3D 9, 10, 11 or 12, the texture unit
              //    to use is usually defined directly within the shader by using the "register"-keyword
              // -> Use the "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" extension if possible to not change OpenGL states
              if (nullptr != glProgramUniform1i) {
                glProgramUniform1i(mOpenGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
              }
              else if (nullptr != glProgramUniform1iEXT) {
                glProgramUniform1iEXT(mOpenGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
              } else {
                // TODO(naetherm) There's room for binding API call related optimization in here (will certainly be no huge overall efficiency gain)
#ifdef RHI_OPENGL_STATE_CLEANUP
                // Backup the currently used OpenGL program
											GLint openGLProgramBackup = 0;
											glGetIntegerv(GL_CURRENT_PROGRAM, &openGLProgramBackup);
											if (static_cast<GLuint>(openGLProgramBackup) == mOpenGLProgram)
											{
												// Set uniform, please note that for this our program must be the currently used one
												glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
											}
											else
											{
												// Set uniform, please note that for this our program must be the currently used one
												glUseProgram(mOpenGLProgram);
												glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));

												// Be polite and restore the previous used OpenGL program
												glUseProgram(static_cast<GLhandleARB>(openGLProgramBackup));
											}
#else
                glUseProgram(mOpenGLProgram);
                glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
#endif
              }
            }
          }
        }
      }
    }
  }
  else
  {
    // Error, program link failed!

    // get the length of the information (including a null termination)
    GLint informationLength = 0;
    glGetProgramiv(mOpenGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
    if (informationLength > 1) {
      // Allocate memory for the information
      const rhi::RHIContext& context = openGLRhi.get_context();
      char* informationLog = re_typed_alloc<char>(informationLength);

      // get the information
      glGetProgramInfoLog(mOpenGLProgram, informationLength, nullptr, informationLog);

      // Output the debug string
      BE_LOG(Critical, informationLog)

      // Cleanup information memory
      re_free(informationLog);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
