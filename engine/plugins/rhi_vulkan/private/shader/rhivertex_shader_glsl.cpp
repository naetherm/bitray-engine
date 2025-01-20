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
#include "rhi_vulkan/shader/rhivertex_shader_glsl.h"
#include "rhi_vulkan/shader_compiler.h"
#include "rhi_vulkan/vulkan_context.h"
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
RHIVertexShaderGlsl::RHIVertexShaderGlsl(RHIDevice &vulkanRhi, const rhi::ShaderBytecode &shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIVertexShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVkShaderModule(ShaderCompiler::create_vk_shader_module_from_bytecode(vulkanRhi.get_context(), vulkanRhi.get_vk_allocation_callbacks(), vulkanRhi.get_vulkan_context().get_vk_device(), shaderBytecode)) {
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VS", 5)	// 5 = "VS: " including terminating zero
    VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (core::uint64)mVkShaderModule, detailedDebugName);
  }
#endif
}

RHIVertexShaderGlsl::RHIVertexShaderGlsl(RHIDevice &vulkanRhi, const char *sourceCode, rhi::ShaderBytecode *shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIVertexShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mVkShaderModule(ShaderCompiler::create_vk_shader_module_from_source_code(vulkanRhi.get_context(), vulkanRhi.get_vk_allocation_callbacks(), vulkanRhi.get_vulkan_context().get_vk_device(), VK_SHADER_STAGE_VERTEX_BIT, sourceCode, shaderBytecode)) {
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VS", 5)	// 5 = "VS: " including terminating zero
    VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (core::uint64)mVkShaderModule, detailedDebugName);
  }
#endif
}

RHIVertexShaderGlsl::~RHIVertexShaderGlsl() {
  if (VK_NULL_HANDLE != mVkShaderModule) {
    const RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
    vkDestroyShaderModule(vulkanRhi.get_vulkan_context().get_vk_device(), mVkShaderModule, vulkanRhi.get_vk_allocation_callbacks());
  }
}

VkShaderModule RHIVertexShaderGlsl::get_vk_shader_module() const {
  return mVkShaderModule;
}

const char* RHIVertexShaderGlsl::get_shader_language_name() const override {
  return VulkanHelper::GLSL_NAME;
}

void RHIVertexShaderGlsl::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
