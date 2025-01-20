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
#include "rhi_vulkan/state/rhigraphics_pipeline_state.h"
#include "rhi_vulkan/shader/rhigraphics_program_glsl.h"
#include "rhi_vulkan/render_target/rhirender_pass.h"
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
RHIGraphicsPipelineState::RHIGraphicsPipelineState(RHIDevice &vulkanRhi, const rhi::GraphicsPipelineStateDescriptor &graphicsPipelineState, core::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
: rhi::RHIGraphicsPipelineState(vulkanRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
, mRootSignature(graphicsPipelineState.rootSignature)
, mGraphicsProgram(graphicsPipelineState.graphicsProgram)
, mRenderPass(graphicsPipelineState.renderPass)
, mVkPipeline(VK_NULL_HANDLE) {
  // Add a reference to the referenced RHI resources
  mRootSignature->add_ref();
  mGraphicsProgram->add_ref();
  mRenderPass->add_ref();

  // Our pipeline state needs to be independent of concrete render targets, so we're using dynamic viewport ("VK_DYNAMIC_STATE_VIEWPORT") and scissor ("VK_DYNAMIC_STATE_SCISSOR") states
  static constexpr core::uint32 WIDTH = 42;
  static constexpr core::uint32 HEIGHT = 42;

  // Shaders
  RHIGraphicsProgramGlsl *graphicsProgramGlsl = static_cast<RHIGraphicsProgramGlsl *>(mGraphicsProgram);
  const bool hasMeshShader = (nullptr != graphicsProgramGlsl->get_mesh_shader_glsl());
  core::uint32 stageCount = 0;
  core::Array<VkPipelineShaderStageCreateInfo, 5> vkPipelineShaderStageCreateInfos;
  {
    // Define helper macro
#define SHADER_STAGE(vkShaderStageFlagBits, shaderGlsl) \
    if (nullptr != shaderGlsl) { \
      VulkanHelper::add_vk_pipeline_shader_stage_create_info(vkShaderStageFlagBits, shaderGlsl->get_vk_shader_module(), vkPipelineShaderStageCreateInfos, stageCount); \
      ++stageCount; \
    }

    // Shader stages
    SHADER_STAGE(VK_SHADER_STAGE_TASK_BIT_NV, graphicsProgramGlsl->get_task_shader_glsl())
    SHADER_STAGE(VK_SHADER_STAGE_MESH_BIT_NV, graphicsProgramGlsl->get_mesh_shader_glsl())
    SHADER_STAGE(VK_SHADER_STAGE_VERTEX_BIT, graphicsProgramGlsl->get_vertex_shader_glsl())
    SHADER_STAGE(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, graphicsProgramGlsl->get_tessellation_control_shader_glsl())
    SHADER_STAGE(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, graphicsProgramGlsl->get_tessellation_evaluation_shader_glsl())
    SHADER_STAGE(VK_SHADER_STAGE_GEOMETRY_BIT, graphicsProgramGlsl->get_geometry_shader_glsl())
    SHADER_STAGE(VK_SHADER_STAGE_FRAGMENT_BIT, graphicsProgramGlsl->get_fragment_shader_glsl())

    // Undefine helper macro
#undef SHADER_STAGE
  }

  // Vertex attributes
  const core::uint32 numberOfAttributes = graphicsPipelineState.vertexAttributes.numberOfAttributes;
  core::Vector<VkVertexInputBindingDescription> vkVertexInputBindingDescriptions;
  core::Vector<VkVertexInputAttributeDescription> vkVertexInputAttributeDescriptions(numberOfAttributes);
  for (core::uint32 attribute = 0; attribute < numberOfAttributes; ++attribute) {
    const rhi::VertexAttribute *attributes = &graphicsPipelineState.vertexAttributes.attributes[attribute];
    const core::uint32 inputSlot = attributes->inputSlot;

    {
      // Map to Vulkan vertex input binding description
      if (vkVertexInputBindingDescriptions.size() <= inputSlot) {
        vkVertexInputBindingDescriptions.resize(inputSlot + 1);
      }
      VkVertexInputBindingDescription &vkVertexInputBindingDescription = vkVertexInputBindingDescriptions[inputSlot];
      vkVertexInputBindingDescription.binding = inputSlot;
      vkVertexInputBindingDescription.stride = attributes->strideInBytes;
      vkVertexInputBindingDescription.inputRate = (attributes->instancesPerElement > 0) ? VK_VERTEX_INPUT_RATE_INSTANCE
                                                                                        : VK_VERTEX_INPUT_RATE_VERTEX;
    }

    { // Map to Vulkan vertex input attribute description
      VkVertexInputAttributeDescription &vkVertexInputAttributeDescription = vkVertexInputAttributeDescriptions[attribute];
      vkVertexInputAttributeDescription.location = attribute;
      vkVertexInputAttributeDescription.binding = inputSlot;
      vkVertexInputAttributeDescription.format = Mapping::get_vulkan_format(attributes->vertexAttributeFormat);
      vkVertexInputAttributeDescription.offset = attributes->alignedByteOffset;
    }
  }

  // Create the Vulkan graphics pipeline
  // TODO(naetherm) Implement the rest of the value mappings
  const VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,      // sType (VkStructureType)
    nullptr,                              // pNext (const void*)
    0,                                  // flags (VkPipelineVertexInputStateCreateFlags)
    static_cast<core::uint32>(vkVertexInputBindingDescriptions.size()),    // vertexBindingDescriptionCount (core::uint32)
    vkVertexInputBindingDescriptions.data(),              // pVertexBindingDescriptions (const VkVertexInputBindingDescription*)
    static_cast<core::uint32>(vkVertexInputAttributeDescriptions.size()),  // vertexAttributeDescriptionCount (core::uint32)
    vkVertexInputAttributeDescriptions.data()              // pVertexAttributeDescriptions (const VkVertexInputAttributeDescription*)
  };
  const VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,    // sType (VkStructureType)
    nullptr,                              // pNext (const void*)
    0,                                  // flags (VkPipelineInputAssemblyStateCreateFlags)
    Mapping::get_vulkan_type(graphicsPipelineState.primitiveTopology),  // topology (VkPrimitiveTopology)
    VK_FALSE                              // primitiveRestartEnable (VkBool32)
  };
  const VkViewport vkViewport = {
    0.0f,            // x (float)
    0.0f,            // y (float)
    static_cast<float>(WIDTH),  // width (float)
    static_cast<float>(HEIGHT),  // height (float)
    0.0f,            // minDepth (float)
    1.0f            // maxDepth (float)
  };
  const VkRect2D scissorVkRect2D = {
    { // offset (VkOffset2D)
      0,  // x (core::int32)
      0  // y (core::int32)
    },
    { // extent (VkExtent2D)
      WIDTH,  // width (core::uint32)
      HEIGHT  // height (core::uint32)
    }
  };
  const VkPipelineTessellationStateCreateInfo vkPipelineTessellationStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,                                                                                  // sType (VkStructureType)
    nullptr,                                                                                                          // pNext (const void*)
    0,                                                                                                              // flags (VkPipelineTessellationStateCreateFlags)
    (graphicsPipelineState.primitiveTopology >= rhi::PrimitiveTopology::PATCH_LIST_1) ?
    static_cast<core::uint32>(graphicsPipelineState.primitiveTopology) -
    static_cast<core::uint32>(rhi::PrimitiveTopology::PATCH_LIST_1) + 1 : 1  // patchControlPoints (core::uint32)
  };
  const VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,  // sType (VkStructureType)
    nullptr,                        // pNext (const void*)
    0,                            // flags (VkPipelineViewportStateCreateFlags)
    1,                            // viewportCount (core::uint32)
    &vkViewport,                      // pViewports (const VkViewport*)
    1,                            // scissorCount (core::uint32)
    &scissorVkRect2D                    // pScissors (const VkRect2D*)
  };
  const float depthBias = static_cast<float>(graphicsPipelineState.rasterizerState.depthBias);
  const float depthBiasClamp = graphicsPipelineState.rasterizerState.depthBiasClamp;
  const float slopeScaledDepthBias = graphicsPipelineState.rasterizerState.slopeScaledDepthBias;
  const VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,                                    // sType (VkStructureType)
    nullptr,                                                            // pNext (const void*)
    0,                                                                // flags (VkPipelineRasterizationStateCreateFlags)
    static_cast<VkBool32>(graphicsPipelineState.rasterizerState.depthClipEnable),                          // depthClampEnable (VkBool32)
    VK_FALSE,                                                            // rasterizerDiscardEnable (VkBool32)
    (rhi::FillMode::WIREFRAME == graphicsPipelineState.rasterizerState.fillMode) ? VK_POLYGON_MODE_LINE
                                                                                   : VK_POLYGON_MODE_FILL,    // polygonMode (VkPolygonMode)
    static_cast<VkCullModeFlags>(static_cast<int>(graphicsPipelineState.rasterizerState.cullMode) -
                                 1),                // cullMode (VkCullModeFlags)
    (1 == graphicsPipelineState.rasterizerState.frontCounterClockwise) ? VK_FRONT_FACE_COUNTER_CLOCKWISE
                                                                       : VK_FRONT_FACE_CLOCKWISE,  // frontFace (VkFrontFace)
    static_cast<VkBool32>(0.0f != depthBias || 0.0f != depthBiasClamp ||
                          0.0f != slopeScaledDepthBias),                // depthBiasEnable (VkBool32)
    depthBias,                                                            // depthBiasConstantFactor (float)
    depthBiasClamp,                                                          // depthBiasClamp (float)
    slopeScaledDepthBias,                                                      // depthBiasSlopeFactor (float)
    1.0f                                                              // lineWidth (float)
  };
  const RHIRenderPass *renderPass = static_cast<const RHIRenderPass *>(mRenderPass);
  const VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,  // sType (VkStructureType)
    nullptr,                          // pNext (const void*)
    0,                              // flags (VkPipelineMultisampleStateCreateFlags)
    renderPass->get_vk_sample_count_flag_bits(),            // rasterizationSamples (VkSampleCountFlagBits)
    VK_FALSE,                          // sampleShadingEnable (VkBool32)
    0.0f,                            // minSampleShading (float)
    nullptr,                          // pSampleMask (const VkSampleMask*)
    VK_FALSE,                          // alphaToCoverageEnable (VkBool32)
    VK_FALSE                          // alphaToOneEnable (VkBool32)
  };
  const VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,                          // sType (VkStructureType)
    nullptr,                                                  // pNext (const void*)
    0,                                                      // flags (VkPipelineDepthStencilStateCreateFlags)
    static_cast<VkBool32>(0 != graphicsPipelineState.depthStencilState.depthEnable),              // depthTestEnable (VkBool32)
    static_cast<VkBool32>(rhi::DepthWriteMask::ALL == graphicsPipelineState.depthStencilState.depthWriteMask),  // depthWriteEnable (VkBool32)
    Mapping::get_vulkan_comparison_func(graphicsPipelineState.depthStencilState.depthFunc),            // depthCompareOp (VkCompareOp)
    VK_FALSE,                                                  // depthBoundsTestEnable (VkBool32)
    static_cast<VkBool32>(0 != graphicsPipelineState.depthStencilState.stencilEnable),              // stencilTestEnable (VkBool32)
    { // front (VkStencilOpState)
      VK_STENCIL_OP_KEEP,                                            // failOp (VkStencilOp)
      VK_STENCIL_OP_KEEP,                                            // passOp (VkStencilOp)
      VK_STENCIL_OP_KEEP,                                            // depthFailOp (VkStencilOp)
      VK_COMPARE_OP_NEVER,                                          // compareOp (VkCompareOp)
      0,                                                    // compareMask (core::uint32)
      0,                                                    // writeMask (core::uint32)
      0                                                    // reference (core::uint32)
    },
    { // back (VkStencilOpState)
      VK_STENCIL_OP_KEEP,                                            // failOp (VkStencilOp)
      VK_STENCIL_OP_KEEP,                                            // passOp (VkStencilOp)
      VK_STENCIL_OP_KEEP,                                            // depthFailOp (VkStencilOp)
      VK_COMPARE_OP_NEVER,                                          // compareOp (VkCompareOp)
      0,                                                    // compareMask (core::uint32)
      0,                                                    // writeMask (core::uint32)
      0                                                    // reference (core::uint32)
    },
    0.0f,                                                    // minDepthBounds (float)
    1.0f                                                    // maxDepthBounds (float)
  };
  const core::uint32 numberOfColorAttachments = renderPass->get_number_of_color_attachments();
  BE_ASSERT(numberOfColorAttachments < 8, "Invalid number of Vulkan color attachments")
  BE_ASSERT(numberOfColorAttachments == graphicsPipelineState.numberOfRenderTargets,
            "Invalid number of Vulkan color attachments")
  core::Array<VkPipelineColorBlendAttachmentState, 8> vkPipelineColorBlendAttachmentStates;
  for (core::uint8 i = 0; i < numberOfColorAttachments; ++i) {
    const rhi::RenderTargetBlendDesc &renderTargetBlendDesc = graphicsPipelineState.blendState.renderTarget[i];
    VkPipelineColorBlendAttachmentState &vkPipelineColorBlendAttachmentState = vkPipelineColorBlendAttachmentStates[i];
    vkPipelineColorBlendAttachmentState.blendEnable = static_cast<VkBool32>(renderTargetBlendDesc.blendEnable);        // blendEnable (VkBool32)
    vkPipelineColorBlendAttachmentState.srcColorBlendFactor = Mapping::get_vulkan_blend_factor(renderTargetBlendDesc.srcBlend);    // srcColorBlendFactor (VkBlendFactor)
    vkPipelineColorBlendAttachmentState.dstColorBlendFactor = Mapping::get_vulkan_blend_factor(renderTargetBlendDesc.destBlend);    // dstColorBlendFactor (VkBlendFactor)
    vkPipelineColorBlendAttachmentState.colorBlendOp = Mapping::get_vulkan_blend_op(renderTargetBlendDesc.blendOp);        // colorBlendOp (VkBlendOp)
    vkPipelineColorBlendAttachmentState.srcAlphaBlendFactor = Mapping::get_vulkan_blend_factor(renderTargetBlendDesc.srcBlendAlpha);  // srcAlphaBlendFactor (VkBlendFactor)
    vkPipelineColorBlendAttachmentState.dstAlphaBlendFactor = Mapping::get_vulkan_blend_factor(renderTargetBlendDesc.destBlendAlpha);  // dstAlphaBlendFactor (VkBlendFactor)
    vkPipelineColorBlendAttachmentState.alphaBlendOp = Mapping::get_vulkan_blend_op(renderTargetBlendDesc.blendOpAlpha);    // alphaBlendOp (VkBlendOp)
    vkPipelineColorBlendAttachmentState.colorWriteMask = renderTargetBlendDesc.renderTargetWriteMask;              // colorWriteMask (VkColorComponentFlags)
  }
  const VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,  // sType (VkStructureType)
    nullptr,                          // pNext (const void*)
    0,                              // flags (VkPipelineColorBlendStateCreateFlags)
    VK_FALSE,                          // logicOpEnable (VkBool32)
    VK_LOGIC_OP_COPY,                      // logicOp (VkLogicOp)
    numberOfColorAttachments,                  // attachmentCount (core::uint32)
    vkPipelineColorBlendAttachmentStates.data(),        // pAttachments (const VkPipelineColorBlendAttachmentState*)
    {0.0f, 0.0f, 0.0f, 0.0f}                  // blendConstants[4] (float)
  };
  static constexpr core::Array<VkDynamicState, 2> vkDynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  static const VkPipelineDynamicStateCreateInfo vkPipelineDynamicStateCreateInfo = {
    VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,  // sType (VkStructureType)
    nullptr,                        // pNext (const void*)
    0,                            // flags (VkPipelineDynamicStateCreateFlags)
    static_cast<core::uint32>(vkDynamicStates.size()),      // dynamicStateCount (core::uint32)
    vkDynamicStates.data()                  // pDynamicStates (const VkDynamicState*)
  };
  const VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo = {
    VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,          // sType (VkStructureType)
    nullptr,                              // pNext (const void*)
    0,                                  // flags (VkPipelineCreateFlags)
    stageCount,                              // stageCount (core::uint32)
    vkPipelineShaderStageCreateInfos.data(),              // pStages (const VkPipelineShaderStageCreateInfo*)
    &vkPipelineVertexInputStateCreateInfo,                // pVertexInputState (const VkPipelineVertexInputStateCreateInfo*)
    hasMeshShader ? nullptr : &vkPipelineInputAssemblyStateCreateInfo,  // pInputAssemblyState (const VkPipelineInputAssemblyStateCreateInfo*)
    hasMeshShader ? nullptr : &vkPipelineTessellationStateCreateInfo,  // pTessellationState (const VkPipelineTessellationStateCreateInfo*)
    &vkPipelineViewportStateCreateInfo,                  // pViewportState (const VkPipelineViewportStateCreateInfo*)
    &vkPipelineRasterizationStateCreateInfo,              // pRasterizationState (const VkPipelineRasterizationStateCreateInfo*)
    &vkPipelineMultisampleStateCreateInfo,                // pMultisampleState (const VkPipelineMultisampleStateCreateInfo*)
    &vkPipelineDepthStencilStateCreateInfo,                // pDepthStencilState (const VkPipelineDepthStencilStateCreateInfo*)
    &vkPipelineColorBlendStateCreateInfo,                // pColorBlendState (const VkPipelineColorBlendStateCreateInfo*)
    &vkPipelineDynamicStateCreateInfo,                  // pDynamicState (const VkPipelineDynamicStateCreateInfo*)
    static_cast<RHIRootSignature *>(mRootSignature)->get_vk_pipeline_layout(),  // layout (VkPipelineLayout)
    renderPass->get_vk_render_pass(),                    // renderPass (VkRenderPass)
    0,                                  // subpass (core::uint32)
    VK_NULL_HANDLE,                            // basePipelineHandle (VkPipeline)
    0                                  // basePipelineIndex (core::int32)
  };
  if (vkCreateGraphicsPipelines(vulkanRhi.get_vulkan_context().get_vk_device(), VK_NULL_HANDLE, 1, &vkGraphicsPipelineCreateInfo, vulkanRhi.get_vk_allocation_callbacks(), &mVkPipeline) == VK_SUCCESS) {
// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT) {
      RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics PSO", 15)	// 15 = "Graphics PSO: " including terminating zero
      VulkanHelper::set_debug_object_name(vulkanRhi.get_vulkan_context().get_vk_device(), VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, (core::uint64)mVkPipeline, detailedDebugName);
    }
#endif
  } else {
    BE_LOG(Critical, "Failed to create the Vulkan graphics pipeline")
  }
}

RHIGraphicsPipelineState::~RHIGraphicsPipelineState() {
  // Destroy the Vulkan graphics pipeline
  RHIDevice &vulkanRhi = static_cast<RHIDevice &>(get_rhi());
  if (VK_NULL_HANDLE != mVkPipeline) {
    vkDestroyPipeline(vulkanRhi.get_vulkan_context().get_vk_device(), mVkPipeline, vulkanRhi.get_vk_allocation_callbacks());
  }

  // Release referenced RHI resources
  mRootSignature->release();
  mGraphicsProgram->release();
  mRenderPass->release();

  // Free the unique compact graphics pipeline state ID
  vulkanRhi.GraphicsPipelineStateMakeId.destroy_id(get_id());
}

VkPipeline RHIGraphicsPipelineState::get_vk_pipeline() const {
  return mVkPipeline;
}

void RHIGraphicsPipelineState::self_destruct() override {
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan
