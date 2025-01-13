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
#include "rhi_opengl/shader_compiler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
void ShaderCompiler::print_opengl_shader_information_into_log(GLuint openGLShader) {
  // get the length of the information (including a null termination)
  GLint informationLength = 0;
  glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
  if (informationLength > 1) {
    // Allocate memory for the information
    char *informationLog = re_typed_alloc<char>(informationLength);

    // get the information
    glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

    // Output the debug string
    BE_LOG(Critical, informationLog)

    // Cleanup information memory
    re_free(informationLog);
  }
}

void ShaderCompiler::print_opengl_shader_information_into_log(GLuint openGLShader,
                                                              const char *sourceCode) {
  // get the length of the information (including a null termination)
  GLint informationLength = 0;
  glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
  if (informationLength > 1) {
    // Allocate memory for the information
    char *informationLog = re_typed_alloc<char>(informationLength);

    // get the information
    glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

    // Output the debug string
    BE_LOG(Critical, core::String(sourceCode))
    //if (context.getLog().print(core::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<core::uint32>(__LINE__), informationLog))
    {
      //	DEBUG_BREAK;
    }

    // Cleanup information memory
    re_free(informationLog);
  }
}

void ShaderCompiler::print_opengl_program_information_into_log(GLuint openGLProgram) {
  // get the length of the information (including a null termination)
  GLint informationLength = 0;
  glGetProgramiv(openGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
  if (informationLength > 1) {
    // Allocate memory for the information
    char *informationLog = re_typed_alloc<char>(informationLength);

    // get the information
    glGetProgramInfoLog(openGLProgram, informationLength, nullptr, informationLog);

    // Output the debug string
    BE_LOG(Critical, informationLog)

    // Cleanup information memory
    re_free(informationLog);
  }
}

void ShaderCompiler::print_opengl_program_information_into_log(GLuint openGLProgram,
                                                               const char *sourceCode) {
  // get the length of the information (including a null termination)
  GLint informationLength = 0;
  glGetProgramiv(openGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
  if (informationLength > 1) {
    // Allocate memory for the information
    char *informationLog = re_typed_alloc<char>(informationLength);

    // get the information
    glGetProgramInfoLog(openGLProgram, informationLength, nullptr, informationLog);

    // Output the debug string
    BE_LOG(Critical, core::String(sourceCode))
    //if (context.getLog().print(core::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<core::uint32>(__LINE__), informationLog))
    {
      //	DEBUG_BREAK;
    }

    // Cleanup information memory
    re_free(informationLog);
  }
}

GLuint ShaderCompiler::load_shader_from_bytecode(GLenum shaderType,
                                                 const rhi::ShaderBytecode &shaderBytecode) {
  // Create the shader object
  const GLuint openGLShader = glCreateShader(shaderType);

  // Load the SPIR-V module into the shader object
  // -> "glShaderBinary" is OpenGL 4.1
  {
    // Decode from SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
    // -> https://github.com/aras-p/smol-v
    // -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
    const size_t spirvOutputBufferSize = smolv::GetDecodedBufferSize(shaderBytecode.getBytecode(),
                                                                     shaderBytecode.getNumberOfBytes());
    core::uint8 *spirvOutputBuffer = re_typed_alloc<core::uint8>(spirvOutputBufferSize);
    smolv::Decode(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes(), spirvOutputBuffer,
                  spirvOutputBufferSize);
    glShaderBinary(1, &openGLShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, spirvOutputBuffer,
                   static_cast<GLsizei>(spirvOutputBufferSize));
    re_free(spirvOutputBuffer);
  }

  // Done
  return openGLShader;
}

GLuint ShaderCompiler::load_shader_program_from_bytecode(GLenum shaderType,
                                                         const rhi::ShaderBytecode &shaderBytecode) {
  // Create and load the shader object
  const GLuint openGLShader = load_shader_from_bytecode(shaderType, shaderBytecode);

  // Specialize the shader
  // -> Before this shader the isn't compiled, after this shader is supposed to be compiled
  glSpecializeShaderARB(openGLShader, "main", 0, nullptr, nullptr);

  // Check compile status
  GLint compiled = GL_FALSE;
  glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  if (GL_TRUE == compiled) {
    // All went fine, create and return the program
    const GLuint openGLProgram = glCreateProgram();
    glProgramParameteri(openGLProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glAttachShader(openGLProgram, openGLShader);
    glLinkProgram(openGLProgram);
    glDetachShader(openGLProgram, openGLShader);
    glDeleteShader(openGLShader);

    // Check the link status
    GLint linked = GL_FALSE;
    glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
    if (GL_TRUE != linked) {
      // Error, program link failed!
      print_opengl_program_information_into_log(openGLProgram, nullptr);
    }

    // Done
    return openGLProgram;
  } else {
    // Error, failed to compile the shader!
    print_opengl_shader_information_into_log(openGLShader, nullptr);

    // Destroy the OpenGL shader
    // -> A value of 0 for shader will be silently ignored
    glDeleteShader(openGLShader);

    // Error!
    return 0u;
  }
}

GLuint
ShaderCompiler::load_shader_program_from_source_code(GLenum shaderType, const GLchar *sourceCode) {
  // Create the shader program
  const GLuint openGLProgram = glCreateShaderProgramv(shaderType, 1, &sourceCode);

  // Check the link status
  GLint linked = GL_FALSE;
  glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
  if (GL_TRUE == linked) {
    // All went fine, return the program
    return openGLProgram;
  } else {
    // Error, failed to compile the shader!
    print_opengl_program_information_into_log(openGLProgram, sourceCode);

    // Destroy the program
    // -> A value of 0 for shader will be silently ignored
    glDeleteProgram(openGLProgram);

    // Error!
    return 0u;
  }
}

GLuint ShaderCompiler::create_shader_program_object(GLuint openGLShader,
                                                    const rhi::VertexAttributes &vertexAttributes) {
  if (openGLShader > 0) {
    // Create the OpenGL program
    const GLuint openGLProgram = glCreateProgram();
    if (openGLProgram > 0) {
      glProgramParameteri(openGLProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

      // Attach the shader to the program
      glAttachShader(openGLProgram, openGLShader);

      // Define the vertex array attribute binding locations ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 & 12 terminology)
      // -> Crucial code that glCreateShaderProgram doesn't do
      {
        const core::uint32 numberOfVertexAttributes = vertexAttributes.numberOfAttributes;
        for (core::uint32 vertexAttribute = 0; vertexAttribute < numberOfVertexAttributes; ++vertexAttribute) {
          glBindAttribLocation(openGLProgram, vertexAttribute, vertexAttributes.attributes[vertexAttribute].name);
        }
      }

      // Link the program
      glLinkProgram(openGLProgram);

      // Detach the shader from the program
      glDetachShader(openGLProgram, openGLShader);
    }

    // Destroy the OpenGL shader
    glDeleteShader(openGLShader);

    // Check the link status
    if (openGLProgram > 0) {
      GLint linked = GL_FALSE;
      glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
      if (GL_TRUE == linked) {
        // Done
        return openGLProgram;
      } else {
        // Error, program link failed!
        print_opengl_program_information_into_log(openGLProgram);
      }
    }
  }

  // Error!
  return 0;
}

GLuint
ShaderCompiler::load_shader_program_from_bytecode(const rhi::VertexAttributes &vertexAttributes,
                                                  GLenum shaderType, const rhi::ShaderBytecode &shaderBytecode) {
  // Create and load the shader object
  const GLuint openGLShader = load_shader_from_bytecode(shaderType, shaderBytecode);

  // Specialize the shader
  // -> Before this shader the isn't compiled, after this shader is supposed to be compiled
  glSpecializeShaderARB(openGLShader, "main", 0, nullptr, nullptr);

  // Check compile status
  GLint compiled = GL_FALSE;
  glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  if (GL_TRUE == compiled) {
    // All went fine, create and return the program
    return create_shader_program_object(openGLShader, vertexAttributes);
  } else {
    // Error, failed to compile the shader!
    print_opengl_shader_information_into_log(openGLShader);

    // Destroy the OpenGL shader
    // -> A value of 0 for shader will be silently ignored
    glDeleteShader(openGLShader);

    // Error!
    return 0;
  }
}

GLuint ShaderCompiler::load_shader_from_source_code(GLenum shaderType, const GLchar *sourceCode) {
  // Create the shader object
  const GLuint openGLShader = glCreateShader(shaderType);

  // Load the shader source
  glShaderSource(openGLShader, 1, &sourceCode, nullptr);

  // Compile the shader
  glCompileShader(openGLShader);

  // Check compile status
  GLint compiled = GL_FALSE;
  glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  if (GL_TRUE == compiled) {
    // All went fine, return the shader
    return openGLShader;
  } else {
    // Error, failed to compile the shader!

    { // get the length of the information
      GLint informationLength = 0;
      glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
      if (informationLength > 1) {
        // Allocate memory for the information
        GLchar *informationLog = re_typed_alloc<GLchar>(informationLength);

        // get the information
        glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

        // Output the debug string
        BE_LOG(Critical, core::String(sourceCode))
        //if (context.getLog().print(core::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<core::uint32>(__LINE__), informationLog))
        {
          //	DEBUG_BREAK;
        }

        // Cleanup information memory
        re_free(informationLog);
      }
    }

    // Destroy the shader
    // -> A value of 0 for shader will be silently ignored
    glDeleteShader(openGLShader);

    // Error!
    return 0u;
  }
}

GLuint ShaderCompiler::load_shader_program_from_source_code(
  const rhi::VertexAttributes &vertexAttributes, GLenum type,
  const char *sourceCode) {
  return create_shader_program_object(load_shader_from_source_code(type, sourceCode),
                                      vertexAttributes);
}

#ifdef _MSC_VER
#pragma optimize("", off)
#endif

void
ShaderCompiler::shader_source_code_to_shader_bytecode(GLenum shaderType, const GLchar *sourceCode,
                                                      rhi::ShaderBytecode &shaderBytecode) {
#ifdef RHI_OPENGL_GLSLTOSPIRV
  // Initialize glslang, if necessary
        if (!GlslangInitialized)
        {
          glslang::InitializeProcess();
          GlslangInitialized = true;
        }

        // GLSL to intermediate
        // -> OpenGL 4.1 (the best OpenGL version Mac OS X 10.11 supports, so lowest version we have to support)
        // TODO(naetherm) OpenGL GLSL 430 instead of 410 for e.g. "GL_ARB_shader_image_load_store" build in support. Apply dropped OpenGL support so we can probably drop Apple support. If at once point RacoonEngine should run on Apple hardware, we probably will use MoltenVK for Vulkan (yet another RHI API Metal just for Apple hardware is probably to much work for a spare time project).
        // const int glslVersion = 410;
        const int glslVersion = 430;
        EShLanguage shLanguage = EShLangCount;
        switch (shaderType)
        {
          case GL_VERTEX_SHADER_ARB:
            shLanguage = EShLangVertex;
            break;

          case GL_TESS_CONTROL_SHADER:
            shLanguage = EShLangTessControl;
            break;

          case GL_TESS_EVALUATION_SHADER:
            shLanguage = EShLangTessEvaluation;
            break;

          case GL_GEOMETRY_SHADER_ARB:
            shLanguage = EShLangGeometry;
            break;

          case GL_FRAGMENT_SHADER_ARB:
            shLanguage = EShLangFragment;
            break;

          case GL_COMPUTE_SHADER:
            shLanguage = EShLangCompute;
            break;
        }
        glslang::TShader shader(shLanguage);
        shader.setEnvInput(glslang::EShSourceGlsl, shLanguage, glslang::EShClientOpenGL, glslVersion);
        shader.setEntryPoint("main");
        {
          const char* sourcePointers[] = { sourceCode };
          shader.setStrings(sourcePointers, 1);
        }
        const EShMessages shMessages = static_cast<EShMessages>(EShMsgDefault);
        if (shader.parse(&DefaultTBuiltInResource, glslVersion, false, shMessages))
        {
          glslang::TProgram program;
          program.addShader(&shader);
          if (program.link(shMessages))
          {
            // Intermediate to SPIR-V
            const glslang::TIntermediate* intermediate = program.getIntermediate(shLanguage);
            if (nullptr != intermediate)
            {
              std::vector<unsigned int> spirv;
              glslang::GlslangToSpv(*intermediate, spirv);

              // Encode to SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
              // -> https://github.com/aras-p/smol-v
              // -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
              // -> Don't apply "spv::spirvbin_t::remap()" or the SMOL-V result will be bigger
              smolv::ByteArray byteArray;
              smolv::Encode(spirv.data(), sizeof(unsigned int) * spirv.size(), byteArray, smolv::kEncodeFlagStripDebugInfo);

              // Done
              shaderBytecode.setBytecodeCopy(static_cast<core::uint32>(byteArray.size()), reinterpret_cast<core::uint8*>(byteArray.data()));
            }
          }
          else
          {
            // Failed to link the program
            BE_LOG(Critical, core::String(sourceCode))
            //if (context.getLog().print(core::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<core::uint32>(__LINE__), "Failed to link the GLSL program: %s", program.getInfoLog()))
            {
            //	DEBUG_BREAK;
            }
          }
        }
        else
        {
          // Failed to parse the shader source code
          BE_LOG(Critical, core::String(sourceCode))
          //if (context.getLog().print(core::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<core::uint32>(__LINE__), "Failed to parse the GLSL shader source code: %s", shader.getInfoLog()))
          {
          //	DEBUG_BREAK;
          }
        }
#endif
}

#ifdef _MSC_VER
#pragma optimize("", on)
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
