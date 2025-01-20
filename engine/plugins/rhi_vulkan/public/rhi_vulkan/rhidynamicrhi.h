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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "rhi_vulkan/rhi_vulkan.h"
#include <rhi/rhidynamicrhi.h>
#include <core/container/array.h>

extern "C"  RHI_VULKAN_API rhi::RHIDevice* create_rhi_device_instance();


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIRootSignature;
class RHIVertexArray;
class VulkanContext;
class VulkanRuntimeLinking;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class RHIDevice : public rhi::RHIDevice {

  //[-------------------------------------------------------]
  //[ Public data                                           ]
  //[-------------------------------------------------------]
public:

  core::MakeIdUInt16 VertexArrayMakeId;
  core::MakeIdUInt16 GraphicsPipelineStateMakeId;
  core::MakeIdUInt16 ComputePipelineStateMakeId;

public:

  RHIDevice();

  ~RHIDevice() override;


  [[nodiscard]] const VkAllocationCallbacks* get_vk_allocation_callbacks() const;

  [[nodiscard]] const VulkanRuntimeLinking& get_vulkan_runtime_linking() const;

  [[nodiscard]] const VulkanContext& get_vulkan_context() const;

public:

  void dispatch_command_buffer_internal(const rhi::RHICommandBuffer& commandBuffer);

  //[-------------------------------------------------------]
  //[ Graphics                                              ]
  //[-------------------------------------------------------]
  void set_graphics_root_signature(rhi::RHIRootSignature* rootSignature);
  void set_graphics_pipeline_state(rhi::RHIGraphicsPipelineState* graphicsPipelineState);
  void set_graphics_resource_group(core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup);
  void set_graphics_vertex_array(rhi::RHIVertexArray* vertexArray);															// Input-assembler (IA) stage
  void set_graphics_viewports(core::uint32 numberOfViewports, const rhi::Viewport* viewports);									// Rasterizer (RS) stage
  void set_graphics_scissor_rectangles(core::uint32 numberOfScissorRectangles, const rhi::ScissorRectangle* scissorRectangles);	// Rasterizer (RS) stage
  void set_graphics_render_target(rhi::RHIRenderTarget* renderTarget);															// Output-merger (OM) stage
  void clear_graphics(core::uint32 clearFlags, const float color[4], float z, core::uint32 stencil);
  void draw_graphics(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_graphics_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_indexed_graphics(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_indexed_graphics_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_mesh_tasks(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  void draw_mesh_tasks_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1);
  //[-------------------------------------------------------]
  //[ Compute                                               ]
  //[-------------------------------------------------------]
  void set_compute_root_signature(rhi::RHIRootSignature* rootSignature);
  void set_compute_pipeline_state(rhi::RHIComputePipelineState* computePipelineState);
  void set_compute_resource_group(core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup);
  void dispatch_compute(core::uint32 groupCountX, core::uint32 groupCountY, core::uint32 groupCountZ);
  //[-------------------------------------------------------]
  //[ Resource                                              ]
  //[-------------------------------------------------------]
  void resolve_multisample_framebuffer(rhi::RHIRenderTarget& destinationRenderTarget, rhi::RHIFramebuffer& sourceMultisampleFramebuffer);
  void copy_resource(rhi::RHIResource& destinationResource, rhi::RHIResource& sourceResource);
  void generate_mipmaps(rhi::RHIResource& resource);
  //[-------------------------------------------------------]
  //[ Query                                                 ]
  //[-------------------------------------------------------]
  void reset_query_pool(rhi::RHIQueryPool& queryPool, core::uint32 firstQueryIndex, core::uint32 numberOfQueries);
  void begin_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex, core::uint32 queryControlFlags);
  void end_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex);
  void write_timestamp_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex);
  //[-------------------------------------------------------]
  //[ Debug                                                 ]
  //[-------------------------------------------------------]
#ifdef DEBUG
  void set_debug_marker(const char* name);
  void begin_debug_event(const char* name);
  void end_debug_event();
#endif

  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIDevice methods           ]
  //[-------------------------------------------------------]
public:

  void initialize(rhi::RHIContext& rhiContext) override;

  [[nodiscard]] virtual const char* get_name() const override;
  [[nodiscard]] virtual bool is_initialized() const override;
  [[nodiscard]] virtual bool is_debug_enabled() override;
  //[-------------------------------------------------------]
  //[ Shader language                                       ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual core::uint32 get_number_of_shader_languages() const override;
  [[nodiscard]] virtual const char* get_shader_language_name(core::uint32 index) const override;
  [[nodiscard]] virtual rhi::RHIShaderLanguage* get_shader_language(const char* shaderLanguageName = nullptr) override;
  //[-------------------------------------------------------]
  //[ Resource creation                                     ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual rhi::RHIRenderPass* create_render_pass(core::uint32 numberOfColorAttachments, const rhi::TextureFormat::Enum* colorAttachmentTextureFormats, rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat = rhi::TextureFormat::UNKNOWN, core::uint8 numberOfMultisamples = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIQueryPool* create_query_pool(rhi::QueryType queryType, core::uint32 numberOfQueries = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHISwapChain* create_swap_chain(rhi::RHIRenderPass& renderPass, core::FrontendPlatformData frontendPlatformData, bool useExternalContext = false RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIFramebuffer* create_framebuffer(rhi::RHIRenderPass& renderPass, const rhi::FramebufferAttachment* colorFramebufferAttachments, const rhi::FramebufferAttachment* depthStencilFramebufferAttachment = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIBufferManager* create_buffer_manager() override;
  [[nodiscard]] virtual rhi::RHITextureManager* create_texture_manager() override;
  [[nodiscard]] virtual rhi::RHIRootSignature* create_root_signature(const rhi::RootSignatureDescriptor& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIGraphicsPipelineState* create_graphics_pipeline_state(const rhi::GraphicsPipelineStateDescriptor& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHIComputePipelineState* create_compute_pipeline_state(rhi::RHIRootSignature& rootSignature, rhi::RHIComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual rhi::RHISamplerState* create_sampler_state(const rhi::SamplerStateDescriptor& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  //[-------------------------------------------------------]
  //[ Resource handling                                     ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual bool map(rhi::RHIResource& resource, core::uint32 subresource, rhi::MapType mapType, core::uint32 mapFlags, rhi::MappedSubresource& mappedSubresource) override;
  virtual void unmap(rhi::RHIResource& resource, core::uint32 subresource) override;
  [[nodiscard]] virtual bool get_query_pool_results(rhi::RHIQueryPool& queryPool, core::uint32 numberOfDataBytes, core::uint8* data, core::uint32 firstQueryIndex = 0, core::uint32 numberOfQueries = 1, core::uint32 strideInBytes = 0, core::uint32 queryResultFlags = 0) override;
  //[-------------------------------------------------------]
  //[ Operation                                             ]
  //[-------------------------------------------------------]
  virtual void dispatch_command_buffer(const rhi::RHICommandBuffer& commandBuffer) override;

private:

  void initialize_capabilities();

  void unset_graphics_vertex_array();

  void begin_vulkan_render_pass();

private:

  VkAllocationCallbacks mVkAllocationCallbacks;

  VulkanRuntimeLinking* mVulkanRuntimeLinking;
  VulkanContext* mVulkanContext;

  rhi::RHIContext* mRhiContext;

  /** GLSL shader language instance (we keep a reference to it), can be a null pointer */
  rhi::RHIShaderLanguage* mShaderLanguageGlsl;
  /** Currently set graphics root signature (we keep a reference to it), can be a null pointer */
  RHIRootSignature*		  mGraphicsRootSignature;
  /** Currently set compute root signature (we keep a reference to it), can be a null pointer */
  RHIRootSignature*		  mComputeRootSignature;	


  rhi::RHISamplerState* mDefaultSamplerState;

  bool mInsideVulkanRenderPass;

  core::Array<VkClearValue, 9> mVkClearValues;
  //[-------------------------------------------------------]
  //[ Input-assembler (IA) stage                            ]
  //[-------------------------------------------------------]
  /** Currently set vertex array (we keep a reference to it), can be a null pointer */
  RHIVertexArray* mVertexArray;	
  //[-------------------------------------------------------]
  //[ Output-merger (OM) stage                              ]
  //[-------------------------------------------------------]
  rhi::RHIRenderTarget* mRenderTarget;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan