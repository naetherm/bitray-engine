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
#include "rhi_vulkan/state/rhicompute_pipeline_state.h"
#include "rhi_vulkan/shader/rhicompute_shader_glsl.h"
#include "rhi_vulkan/rhiroot_signature.h"
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
RHIComputePipelineState::RHIComputePipelineState(RHIDevice &vulkanRhi, rhi::RHIRootSignature &rootSignature, rhi::RHIComputeShader &computeShader, core::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIComputePipelineState(vulkanRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mRootSignature(rootSignature)
, mComputeShader(computeShader)
, mVkPipeline(VK_NULL_HANDLE) {
  // Add a reference to the given root signature and compute shader
  rootSignature.add_ref();
  computeShader.add_ref();

  // Create the Vulkan compute pipeline
  const VkComputePipelineCreateInfo vkComputePipelineCreateInfo = {
    VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,              // sType (VkStructureType)
    nullptr,                                // pNext (const void*)
    0,                                    // flags (VkPipelineCreateFlags)
    {                                    // stage (VkPipelineShaderStageCreateInfo)
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,        // sType (VkStructureType)
      nullptr,                              // pNext (const void*)
      0,                                  // flags (VkPipelineShaderStageCreateFlags)
      VK_SHADER_STAGE_COMPUTE_BIT,                    // stage (VkShaderStageFlagBits)
      static_cast<RHIComputeShaderGlsl &>(computeShader).get_vk_shader_module(),  // module (VkShaderModule)
      "main",                                // pName (const char*)
      nullptr                                // pSpecializationInfo (const VkSpecializationInfo*)
    },
    static_cast<RHIRootSignature &>(rootSignature).get_vk_pipeline_layout(),    // layout (VkPipelineLayout)
    VK_NULL_HANDLE,                              // basePipelineHandle (VkPipeline)
    0                                    // basePipelineIndex (int32_t)
  };
  if (vkCreateComputePipelines(vulkanRhi.get_vulkan_context().get_vk_device(), VK_NULL_HANDLE, 1, &vkComputePipelineCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &mVkPipeline) == VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT) {
    RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Compute PSO", 14)	// 14 = "Compute PSO: " including terminating zero
    VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, (core::uint64)mVkPipeline, detailedDebugName);
  }
#endif
  } else {
    BE_LOG(Critical, "Failed to create the Vulkan compute pipeline")
  }
}

RHIComputePipelineState::~RHIComputePipelineState() {
  // Destroy the Vulkan compute pipeline
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  if (VK_NULL_HANDLE != mVkPipeline) {
    vkDestroyPipeline(vulkanRhi.get_vulkan_context().get_vk_device(), mVkPipeline, vulkanRhi.get_vk_allocation_callbacks());
  }

  // Release the root signature and compute shader reference
  mRootSignature.release();
  mComputeShader.release();

  // Free the unique compact compute pipeline state ID
  vulkanRhi.ComputePipelineStateMakeId.destroy_id(get_id());
}


VkPipeline RHIComputePipelineState::get_vk_pipeline() const {
  return mVkPipeline;
}

void RHIComputePipelineState::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
