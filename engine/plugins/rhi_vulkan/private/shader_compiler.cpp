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
#include "rhi_vulkan/shader_compiler.h"
#include <glslang/Public/ShaderLang.h>
#include <smolv.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {

bool ShaderCompiler::GlslangInitialized = false;

const TBuiltInResource ShaderCompiler::DefaultTBuiltInResource = {
  32,		/// MaxLights
  6,		/// MaxClipPlanes
  32,		/// MaxTextureUnits
  32,		/// MaxTextureCoords
  64,		/// MaxVertexAttribs
  4096,	/// MaxVertexUniformComponents
  64,		/// MaxVaryingFloats
  32,		/// MaxVertexTextureImageUnits
  80,		/// MaxCombinedTextureImageUnits
  32,		/// MaxTextureImageUnits
  4096,	/// MaxFragmentUniformComponents
  32,		/// MaxDrawBuffers
  128,	/// MaxVertexUniformVectors
  8,		/// MaxVaryingVectors
  16,		/// MaxFragmentUniformVectors
  16,		/// MaxVertexOutputVectors
  15,		/// MaxFragmentInputVectors
  -8,		/// MinProgramTexelOffset
  7,		/// MaxProgramTexelOffset
  8,		/// MaxClipDistances
  65535,	/// MaxComputeWorkGroupCountX
  65535,	/// MaxComputeWorkGroupCountY
  65535,	/// MaxComputeWorkGroupCountZ
  1024,	/// MaxComputeWorkGroupSizeX
  1024,	/// MaxComputeWorkGroupSizeY
  64,		/// MaxComputeWorkGroupSizeZ
  1024,	/// MaxComputeUniformComponents
  16,		/// MaxComputeTextureImageUnits
  8,		/// MaxComputeImageUniforms
  8,		/// MaxComputeAtomicCounters
  1,		/// MaxComputeAtomicCounterBuffers
  60,		/// MaxVaryingComponents
  64,		/// MaxVertexOutputComponents
  64,		/// MaxGeometryInputComponents
  128,	/// MaxGeometryOutputComponents
  128,	/// MaxFragmentInputComponents
  8,		/// MaxImageUnits
  8,		/// MaxCombinedImageUnitsAndFragmentOutputs
  8,		/// MaxCombinedShaderOutputResources
  0,		/// MaxImageSamples
  0,		/// MaxVertexImageUniforms
  0,		/// MaxTessControlImageUniforms
  0,		/// MaxTessEvaluationImageUniforms
  0,		/// MaxGeometryImageUniforms
  8,		/// MaxFragmentImageUniforms
  8,		/// MaxCombinedImageUniforms
  16,		/// MaxGeometryTextureImageUnits
  256,	/// MaxGeometryOutputVertices
  1024,	/// MaxGeometryTotalOutputComponents
  1024,	/// MaxGeometryUniformComponents
  64,		/// MaxGeometryVaryingComponents
  128,	/// MaxTessControlInputComponents
  128,	/// MaxTessControlOutputComponents
  16,		/// MaxTessControlTextureImageUnits
  1024,	/// MaxTessControlUniformComponents
  4096,	/// MaxTessControlTotalOutputComponents
  128,	/// MaxTessEvaluationInputComponents
  128,	/// MaxTessEvaluationOutputComponents
  16,		/// MaxTessEvaluationTextureImageUnits
  1024,	/// MaxTessEvaluationUniformComponents
  120,	/// MaxTessPatchComponents
  32,		/// MaxPatchVertices
  64,		/// MaxTessGenLevel
  16,		/// MaxViewports
  0,		/// MaxVertexAtomicCounters
  0,		/// MaxTessControlAtomicCounters
  0,		/// MaxTessEvaluationAtomicCounters
  0,		/// MaxGeometryAtomicCounters
  8,		/// MaxFragmentAtomicCounters
  8,		/// MaxCombinedAtomicCounters
  1,		/// MaxAtomicCounterBindings
  0,		/// MaxVertexAtomicCounterBuffers
  0,		/// MaxTessControlAtomicCounterBuffers
  0,		/// MaxTessEvaluationAtomicCounterBuffers
  0,		/// MaxGeometryAtomicCounterBuffers
  1,		/// MaxFragmentAtomicCounterBuffers
  1,		/// MaxCombinedAtomicCounterBuffers
  16384,	/// MaxAtomicCounterBufferSize
  4,		/// MaxTransformFeedbackBuffers
  64,		/// MaxTransformFeedbackInterleavedComponents
  8,		/// MaxCullDistances
  8,		/// MaxCombinedClipAndCullDistances
  4,		/// MaxSamples
  256,	/// MaxMeshOutputVerticesNV
  512,	/// MaxMeshOutputPrimitivesNV
  32,		/// MaxMeshWorkGroupSizeX_NV
  1,		/// MaxMeshWorkGroupSizeY_NV
  1,		/// MaxMeshWorkGroupSizeZ_NV
  32,		/// MaxTaskWorkGroupSizeX_NV
  1,		/// MaxTaskWorkGroupSizeY_NV
  1,		/// MaxTaskWorkGroupSizeZ_NV
  4,		/// MaxMeshViewCountNV
  0,		/// maxMeshOutputVerticesEXT
  0,		/// maxMeshOutputPrimitivesEXT
  0,		/// maxMeshWorkGroupSizeX_EXT
  0,		/// maxMeshWorkGroupSizeY_EXT
  0,		/// maxMeshWorkGroupSizeZ_EXT
  0,		/// maxTaskWorkGroupSizeX_EXT
  0,		/// maxTaskWorkGroupSizeY_EXT
  0,		/// maxTaskWorkGroupSizeZ_EXT
  0,		/// maxMeshViewCountEXT
  0,		/// maxDualSourceDrawBuffersEXT
  {		/// limits
    1,	/// nonInductiveForLoops
    1,	/// whileLoops
    1,	/// doWhileLoops
    1,	/// generalUniformIndexing
    1,	/// generalAttributeMatrixVectorIndexing
    1,	/// generalVaryingIndexing
    1,	/// generalSamplerIndexing
    1,	/// generalVariableIndexing
    1,	/// generalConstantMatrixVectorIndexing
  }
};


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
VkShaderModule ShaderCompiler::create_vk_shader_module_from_bytecode(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, const rhi::ShaderBytecode& shaderBytecode) {
  // Decode from SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
  // -> https://github.com/aras-p/smol-v
  // -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
  const size_t spirvOutputBufferSize = smolv::GetDecodedBufferSize(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
  // TODO(naetherm) Try to avoid new/delete by trying to use the C-runtime stack if there aren't too many bytes
  core::uint8* spirvOutputBuffer = re_typed_alloc<core::uint8>(spirvOutputBufferSize);
  smolv::Decode(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes(), spirvOutputBuffer, spirvOutputBufferSize);

  // Create the Vulkan shader module
  const VkShaderModuleCreateInfo vkShaderModuleCreateInfo =  {
    VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,			// sType (VkStructureType)
    nullptr,												// pNext (const void*)
    0,														// flags (VkShaderModuleCreateFlags)
    spirvOutputBufferSize,									// codeSize (size_t)
    reinterpret_cast<const core::uint32*>(spirvOutputBuffer)	// pCode (const core::uint32*)
  };
  VkShaderModule vkShaderModule = VK_NULL_HANDLE;
  if (vkCreateShaderModule(vkDevice, &vkShaderModuleCreateInfo, vkAllocationCallbacks, &vkShaderModule) != VK_SUCCESS) {
    BE_LOG(Critical, "Failed to create the Vulkan shader module")
  }

  // Done
  re_free(spirvOutputBuffer);
  return vkShaderModule;
}

VkShaderModule ShaderCompiler::create_vk_shader_module_from_source_code(const rhi::RHIContext& context, const VkAllocationCallbacks* vkAllocationCallbacks, VkDevice vkDevice, VkShaderStageFlagBits vkShaderStageFlagBits, const char* sourceCode, rhi::ShaderBytecode* shaderBytecode) {

  // Initialize glslang, if necessary
  if (!GlslangInitialized) {
    glslang::InitializeProcess();
    GlslangInitialized = true;
  }
  // GLSL to intermediate
  // -> OpenGL 4.5
  static constexpr int glslVersion = 450;
  EShLanguage shLanguage = EShLangCount;
  if ((vkShaderStageFlagBits & VK_SHADER_STAGE_VERTEX_BIT) != 0) {
    shLanguage = EShLangVertex;
  } else if ((vkShaderStageFlagBits & VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT) != 0) {
    shLanguage = EShLangTessControl;
  } else if ((vkShaderStageFlagBits & VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) != 0) {
    shLanguage = EShLangTessEvaluation;
  } else if ((vkShaderStageFlagBits & VK_SHADER_STAGE_GEOMETRY_BIT) != 0) {
    shLanguage = EShLangGeometry;
  } else if ((vkShaderStageFlagBits & VK_SHADER_STAGE_FRAGMENT_BIT) != 0) {
    shLanguage = EShLangFragment;
  } else if ((vkShaderStageFlagBits & VK_SHADER_STAGE_TASK_BIT_NV) != 0) {
    shLanguage = EShLangTaskNV;
  } else if ((vkShaderStageFlagBits & VK_SHADER_STAGE_MESH_BIT_NV) != 0) {
    shLanguage = EShLangMeshNV;
  } else if ((vkShaderStageFlagBits & VK_SHADER_STAGE_COMPUTE_BIT) != 0) {
    shLanguage = EShLangCompute;
  } else {
    BE_ASSERT(false, "Invalid Vulkan shader stage flag bits")
  }
  glslang::TShader shader(shLanguage);
  shader.setEnvClient(glslang::EShClient::EShClientVulkan, glslang::EShTargetVulkan_1_0);
  shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetLanguageVersion::EShTargetSpv_1_6);
  shader.setEnvInput(glslang::EShSourceGlsl, shLanguage, glslang::EShClientVulkan, glslVersion);
  shader.setEntryPoint("main");
  {
    const char* sourcePointers[] = { sourceCode };
    shader.setStrings(sourcePointers, 1);
  }
  static constexpr EShMessages shMessages = static_cast<EShMessages>(EShMsgDefault | EShMsgSpvRules | EShMsgVulkanRules);
  if (shader.parse(&DefaultTBuiltInResource, glslVersion, false, shMessages)) {
    glslang::TProgram program;
    program.addShader(&shader);
    if (program.link(shMessages)) {
      // Intermediate to SPIR-V
      const glslang::TIntermediate* intermediate = program.getIntermediate(shLanguage);
      if (nullptr != intermediate) {
        // TODO(naetherm) "glslang::GlslangToSpv()" is using the default allocator
        std::vector<unsigned int> spirv;
        glslang::GlslangToSpv(*intermediate, spirv);

        // Optional shader bytecode output
        if (nullptr != shaderBytecode) {
          // Encode to SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
          // -> https://github.com/aras-p/smol-v
          // -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
          // -> Don't apply "spv::spirvbin_t::remap()" or the SMOL-V result will be bigger
          smolv::ByteArray byteArray;
          smolv::Encode(spirv.data(), sizeof(unsigned int) * spirv.size(), byteArray, smolv::kEncodeFlagStripDebugInfo);

          // Done
          shaderBytecode->setBytecodeCopy(static_cast<core::uint32>(byteArray.size()), reinterpret_cast<core::uint8*>(byteArray.data()));
        }

        // Create the Vulkan shader module
        const VkShaderModuleCreateInfo vkShaderModuleCreateInfo = {
          VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	// sType (VkStructureType)
          nullptr,										// pNext (const void*)
          0,												// flags (VkShaderModuleCreateFlags)
          sizeof(unsigned int) * spirv.size(),			// codeSize (size_t)
          spirv.data()									// pCode (const core::uint32*)
        };
        VkShaderModule vkShaderModule = VK_NULL_HANDLE;
        if (vkCreateShaderModule(vkDevice, &vkShaderModuleCreateInfo, vkAllocationCallbacks, &vkShaderModule) != VK_SUCCESS) {
          BE_LOG(Critical, "Failed to create the Vulkan shader module")
        }
        return vkShaderModule;
      }
    } else {
      // Failed to link the program
      BE_LOG(Critical, "Failed to link the GLSL program: " + core::String(program.getInfoLog()))
      {
        DEBUG_BREAK;
      }
    }
  } else {
    // Failed to parse the shader source code
    BE_LOG(Critical, "Failed to parse the GLSL shader source code: " + core::String(shader.getInfoLog()))
    {
      DEBUG_BREAK;
    }
  }
  
  return VK_NULL_HANDLE;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan