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
#define VOLK_IMPLEMENTATION
#include "rhi_vulkan/volk.h"
#include "rhi_vulkan/rhidynamicrhi.h"
#include "rhi_vulkan/rhiresource_group.h"
#include "rhi_vulkan/rhiroot_signature.h"
#include "rhi_vulkan/vulkan_context.h"
#include "rhi_vulkan/vulkan_helper.h"
#include "rhi_vulkan/vulkan_runtime_linking.h"
#include "rhi_vulkan/buffer/rhibuffer_manager.h"
#include "rhi_vulkan/buffer/rhiindex_buffer.h"
#include "rhi_vulkan/buffer/rhiindirect_buffer.h"
#include "rhi_vulkan/buffer/rhistructured_buffer.h"
#include "rhi_vulkan/buffer/rhitexture_buffer.h"
#include "rhi_vulkan/buffer/rhiuniform_buffer.h"
#include "rhi_vulkan/buffer/rhivertex_array.h"
#include "rhi_vulkan/buffer/rhivertex_buffer.h"
#include "rhi_vulkan/query/rhiquery_pool.h"
#include "rhi_vulkan/render_target/rhiframebuffer.h"
#include "rhi_vulkan/render_target/rhirender_pass.h"
#include "rhi_vulkan/render_target/rhiswap_chain.h"
#include "rhi_vulkan/shader/rhishader_language_glsl.h"
#include "rhi_vulkan/state/rhicompute_pipeline_state.h"
#include "rhi_vulkan/state/rhigraphics_pipeline_state.h"
#include "rhi_vulkan/state/rhisampler_state.h"
#include "rhi_vulkan/texture/rhitexture_2d.h"
#include "rhi_vulkan/texture/rhitexture_manager.h"
#include <core/memory/memory.h>


namespace internal {

[[nodiscard]] VKAPI_ATTR void* VKAPI_CALL vkAllocationFunction(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope) {
  return core::Memory::reallocate(nullptr, 0, size, alignment);
}

[[nodiscard]] VKAPI_ATTR void* VKAPI_CALL vkReallocationFunction(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope) {
  return core::Memory::reallocate(pOriginal, 0, size, alignment);
}

VKAPI_ATTR void VKAPI_CALL vkFreeFunction(void* pUserData, void* pMemory) {
  core::Memory::reallocate(pMemory, 0, 0, 1);
}


namespace ImplementationDispatch {


//[-------------------------------------------------------]
//[ Command buffer                                        ]
//[-------------------------------------------------------]
void dispatch_command_buffer(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DispatchCommandBuffer* realData = static_cast<const rhi::Command::DispatchCommandBuffer*>(data);
  BE_ASSERT(nullptr != realData->commandBufferToDispatch, "The OpenGL command buffer to dispatch must be valid")
  static_cast<rhi_vulkan::RHIDevice &>(rhi).dispatch_command_buffer_internal(*realData->commandBufferToDispatch);
}

//[-------------------------------------------------------]
//[ Graphics                                              ]
//[-------------------------------------------------------]
void set_graphics_root_signature(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetGraphicsRootSignature* realData = static_cast<const rhi::Command::SetGraphicsRootSignature*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_graphics_root_signature(realData->rootSignature);
}

void set_graphics_pipeline_state(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetGraphicsPipelineState* realData = static_cast<const rhi::Command::SetGraphicsPipelineState*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_graphics_pipeline_state(realData->graphicsPipelineState);
}

void set_graphics_resource_group(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetGraphicsResourceGroup* realData = static_cast<const rhi::Command::SetGraphicsResourceGroup*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_graphics_resource_group(realData->rootParameterIndex,
                                                                             realData->resourceGroup);
}

void set_graphics_vertex_array(const void* data, rhi::RHIDevice& rhi) {
  // Input-assembler (IA) stage
  const rhi::Command::SetGraphicsVertexArray* realData = static_cast<const rhi::Command::SetGraphicsVertexArray*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_graphics_vertex_array(realData->vertexArray);
}

void set_graphics_viewports(const void* data, rhi::RHIDevice& rhi) {
  // Rasterizer (RS) stage
  const rhi::Command::SetGraphicsViewports* realData = static_cast<const rhi::Command::SetGraphicsViewports*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_graphics_viewports(realData->numberOfViewports,
                                                                        (nullptr != realData->viewports)
                                                                        ? realData->viewports
                                                                        : reinterpret_cast<const rhi::Viewport *>(rhi::CommandPacketHelper::getAuxiliaryMemory(
                                                                          realData)));
}

void set_graphics_scissor_rectangles(const void* data, rhi::RHIDevice& rhi) {
  // Rasterizer (RS) stage
  const rhi::Command::SetGraphicsScissorRectangles* realData = static_cast<const rhi::Command::SetGraphicsScissorRectangles*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_graphics_scissor_rectangles(realData->numberOfScissorRectangles,
                                                                                 (nullptr !=
                                                                                  realData->scissorRectangles)
                                                                                 ? realData->scissorRectangles
                                                                                 : reinterpret_cast<const rhi::ScissorRectangle *>(rhi::CommandPacketHelper::getAuxiliaryMemory(
                                                                                   realData)));
}

void set_graphics_render_target(const void* data, rhi::RHIDevice& rhi) {
  // Output-merger (OM) stage
  const rhi::Command::SetGraphicsRenderTarget* realData = static_cast<const rhi::Command::SetGraphicsRenderTarget*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_graphics_render_target(realData->renderTarget);
}

void clear_graphics(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::ClearGraphics* realData = static_cast<const rhi::Command::ClearGraphics*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).clear_graphics(realData->clearFlags, realData->color, realData->z, realData->stencil);
}

void draw_graphics(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DrawGraphics* realData = static_cast<const rhi::Command::DrawGraphics*>(data);
  if (nullptr != realData->indirectBuffer) {
    static_cast<rhi_vulkan::RHIDevice &>(rhi).draw_graphics(*realData->indirectBuffer,
                                                                 realData->indirectBufferOffset,
                                                                 realData->numberOfDraws);
  } else {
    static_cast<rhi_vulkan::RHIDevice &>(rhi).draw_graphics_emulated(
      rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void draw_indexed_graphics(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DrawIndexedGraphics* realData = static_cast<const rhi::Command::DrawIndexedGraphics*>(data);
  if (nullptr != realData->indirectBuffer) {
    static_cast<rhi_vulkan::RHIDevice &>(rhi).draw_indexed_graphics(*realData->indirectBuffer,
                                                                         realData->indirectBufferOffset,
                                                                         realData->numberOfDraws);
  } else {
    static_cast<rhi_vulkan::RHIDevice &>(rhi).draw_indexed_graphics_emulated(
      rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void draw_mesh_tasks(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DrawMeshTasks* realData = static_cast<const rhi::Command::DrawMeshTasks*>(data);
  if (nullptr != realData->indirectBuffer) {
    static_cast<rhi_vulkan::RHIDevice &>(rhi).draw_mesh_tasks(*realData->indirectBuffer,
                                                                   realData->indirectBufferOffset,
                                                                   realData->numberOfDraws);
  } else {
    static_cast<rhi_vulkan::RHIDevice &>(rhi).draw_mesh_tasks_emulated(
      rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
void set_compute_root_signature(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetComputeRootSignature* realData = static_cast<const rhi::Command::SetComputeRootSignature*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_compute_root_signature(realData->rootSignature);
}

void set_compute_pipeline_state(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetComputePipelineState* realData = static_cast<const rhi::Command::SetComputePipelineState*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_compute_pipeline_state(realData->computePipelineState);
}

void set_compute_resource_group(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetComputeResourceGroup* realData = static_cast<const rhi::Command::SetComputeResourceGroup*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).set_compute_resource_group(realData->rootParameterIndex,
                                                                            realData->resourceGroup);
}

void dispatch_compute(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DispatchCompute* realData = static_cast<const rhi::Command::DispatchCompute*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).dispatch_compute(realData->groupCountX, realData->groupCountY,
                                                                  realData->groupCountZ);
}

//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void set_texture_minimum_maximum_mipmap_index(const void* data, [[maybe_unused]] rhi::RHIDevice& rhi) {
  const rhi::Command::SetTextureMinimumMaximumMipmapIndex* realData = static_cast<const rhi::Command::SetTextureMinimumMaximumMipmapIndex*>(data);
  BE_ASSERT(realData->texture->get_resource_type() == rhi::ResourceType::TEXTURE_2D, "Unsupported OpenGL texture resource type")
  static_cast<rhi_vulkan::RHITexture2D*>(realData->texture)->set_minimum_maximum_mipmap_index(realData->minimumMipmapIndex, realData->maximumMipmapIndex);
}

void resolve_multisample_framebuffer(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::ResolveMultisampleFramebuffer* realData = static_cast<const rhi::Command::ResolveMultisampleFramebuffer*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).resolve_multisample_framebuffer(*realData->destinationRenderTarget,
                                                                                 *realData->sourceMultisampleFramebuffer);
}

void copy_resource(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::CopyResource* realData = static_cast<const rhi::Command::CopyResource*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).copy_resource(*realData->destinationResource,
                                                               *realData->sourceResource);
}

void generate_mipmaps(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::GenerateMipmaps* realData = static_cast<const rhi::Command::GenerateMipmaps*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).generate_mipmaps(*realData->resource);
}

void copy_uniform_buffer_data(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::CopyUniformBufferData* realData = static_cast<const rhi::Command::CopyUniformBufferData*>(data);
  rhi::MappedSubresource mappedSubresource;
  if (rhi.map(*realData->uniformBuffer, 0, rhi::MapType::WRITE_DISCARD, 0, mappedSubresource))
  {
    memcpy(mappedSubresource.data, rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->numberOfBytes);
    rhi.unmap(*realData->uniformBuffer, 0);
  }
}

void set_uniform(const void*, [[maybe_unused]] rhi::RHIDevice& rhi) {
  BE_ASSERT(false, "The set uniform command isn't supported by the OpenGL RHI implementation")
}

//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void reset_query_pool(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::ResetQueryPool* realData = static_cast<const rhi::Command::ResetQueryPool*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).reset_query_pool(*realData->queryPool, realData->firstQueryIndex,
                                                                  realData->numberOfQueries);
}

void begin_query(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::BeginQuery* realData = static_cast<const rhi::Command::BeginQuery*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).begin_query(*realData->queryPool, realData->queryIndex,
                                                             realData->queryControlFlags);
}

void end_query(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::EndQuery* realData = static_cast<const rhi::Command::EndQuery*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).end_query(*realData->queryPool, realData->queryIndex);
}

void write_timestamp_query(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::WriteTimestampQuery* realData = static_cast<const rhi::Command::WriteTimestampQuery*>(data);
  static_cast<rhi_vulkan::RHIDevice &>(rhi).write_timestamp_query(*realData->queryPool, realData->queryIndex);
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void set_debug_marker(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetDebugMarker* realData = static_cast<const rhi::Command::SetDebugMarker*>(data);
  static_cast<rhi_vulkan::RHIDevice&>(rhi).set_debug_marker(realData->name);
}

void begin_debug_event(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::BeginDebugEvent* realData = static_cast<const rhi::Command::BeginDebugEvent*>(data);
  static_cast<rhi_vulkan::RHIDevice&>(rhi).begin_debug_event(realData->name);
}

void end_debug_event(const void*, rhi::RHIDevice& rhi) {
  static_cast<rhi_vulkan::RHIDevice&>(rhi).end_debug_event();
}
#else
void set_debug_marker(const void*, rhi::RHIDevice&) {
  // Nothing here
}

void begin_debug_event(const void*, rhi::RHIDevice&) {
  // Nothing here
}

void end_debug_event(const void*, rhi::RHIDevice&) {
  // Nothing here
}
#endif

}


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
static constexpr rhi::ImplementationDispatchFunction DISPATCH_FUNCTIONS[static_cast<core::uint8>(rhi::CommandDispatchFunctionIndex::NUMBER_OF_FUNCTIONS)] = {
  // Command buffer
  &ImplementationDispatch::dispatch_command_buffer,
  // Graphics
  &ImplementationDispatch::set_graphics_root_signature,
  &ImplementationDispatch::set_graphics_pipeline_state,
  &ImplementationDispatch::set_graphics_resource_group,
  &ImplementationDispatch::set_graphics_vertex_array,		// Input-assembler (IA) stage
  &ImplementationDispatch::set_graphics_viewports,			// Rasterizer (RS) stage
  &ImplementationDispatch::set_graphics_scissor_rectangles,	// Rasterizer (RS) stage
  &ImplementationDispatch::set_graphics_render_target,		// Output-merger (OM) stage
  &ImplementationDispatch::clear_graphics,
  &ImplementationDispatch::draw_graphics,
  &ImplementationDispatch::draw_indexed_graphics,
  &ImplementationDispatch::draw_mesh_tasks,
  // Compute
  &ImplementationDispatch::set_compute_root_signature,
  &ImplementationDispatch::set_compute_pipeline_state,
  &ImplementationDispatch::set_compute_resource_group,
  &ImplementationDispatch::dispatch_compute,
  // Resource
  &ImplementationDispatch::set_texture_minimum_maximum_mipmap_index,
  &ImplementationDispatch::resolve_multisample_framebuffer,
  &ImplementationDispatch::copy_resource,
  &ImplementationDispatch::generate_mipmaps,
  &ImplementationDispatch::copy_uniform_buffer_data,
  &ImplementationDispatch::set_uniform,
  // Query
  &ImplementationDispatch::reset_query_pool,
  &ImplementationDispatch::begin_query,
  &ImplementationDispatch::end_query,
  &ImplementationDispatch::write_timestamp_query,
  // Debug
  &ImplementationDispatch::set_debug_marker,
  &ImplementationDispatch::begin_debug_event,
  &ImplementationDispatch::end_debug_event
};

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_vulkan {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIDevice::RHIDevice()
: rhi::RHIDevice(rhi::NameId::VULKAN)
, mVkAllocationCallbacks{nullptr, &::internal::vkAllocationFunction, &::internal::vkReallocationFunction, &::internal::vkFreeFunction, nullptr, nullptr}
, mVulkanRuntimeLinking(nullptr)
, mVulkanContext(nullptr)
, mRhiContext(nullptr)
, mShaderLanguageGlsl(nullptr)
, mGraphicsRootSignature(nullptr)
, mComputeRootSignature(nullptr)
, mDefaultSamplerState(nullptr)
, mInsideVulkanRenderPass(false)
, mVkClearValues{}
, mVertexArray(nullptr)
, mRenderTarget(nullptr) {
  
}

RHIDevice::~RHIDevice() {
  delete mVulkanContext;

  delete mVulkanRuntimeLinking;
}


const VkAllocationCallbacks *RHIDevice::get_vk_allocation_callbacks() const {
  return &mVkAllocationCallbacks;
}

const VulkanRuntimeLinking& RHIDevice::get_vulkan_runtime_linking() const {
  return *mVulkanRuntimeLinking;
}

const VulkanContext& RHIDevice::get_vulkan_context() const {
  return *mVulkanContext;
}


void RHIDevice::dispatch_command_buffer_internal(const rhi::RHICommandBuffer& commandBuffer) {
  // Loop through all commands
  const core::uint8* commandPacketBuffer = commandBuffer.get_command_packet_buffer();
  rhi::ConstCommandPacket constCommandPacket = commandPacketBuffer;
  while (nullptr != constCommandPacket) {
    { // Dispatch command packet
      const rhi::CommandDispatchFunctionIndex commandDispatchFunctionIndex = rhi::CommandPacketHelper::loadCommandDispatchFunctionIndex(constCommandPacket);
      const void* command = rhi::CommandPacketHelper::loadCommand(constCommandPacket);
      internal::DISPATCH_FUNCTIONS[static_cast<core::uint32>(commandDispatchFunctionIndex)](command, *this);
    }

    { // Next command
      const core::uint32 nextCommandPacketByteIndex = rhi::CommandPacketHelper::getNextCommandPacketByteIndex(constCommandPacket);
      constCommandPacket = (~0u != nextCommandPacketByteIndex) ? &commandPacketBuffer[nextCommandPacketByteIndex] : nullptr;
    }
  }
}

//[-------------------------------------------------------]
//[ Graphics                                              ]
//[-------------------------------------------------------]
void RHIDevice::set_graphics_root_signature(rhi::RHIRootSignature* rootSignature) {
  if (nullptr != mGraphicsRootSignature) {
    mGraphicsRootSignature->release();
  }
  mGraphicsRootSignature = static_cast<RHIRootSignature*>(rootSignature);
  if (nullptr != mGraphicsRootSignature) {
    mGraphicsRootSignature->add_ref();
  }
}

void RHIDevice::set_graphics_pipeline_state(rhi::RHIGraphicsPipelineState* graphicsPipelineState) {
  if (nullptr != graphicsPipelineState) {
    // Bind Vulkan graphics pipeline
    vkCmdBindPipeline(get_vulkan_context().get_vk_command_buffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<RHIGraphicsPipelineState*>(graphicsPipelineState)->get_vk_pipeline());
  } else {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDevice::set_graphics_resource_group(core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup) {
  // Security checks
#ifdef RHI_DEBUG
  {
    BE_ASSERT(nullptr != mGraphicsRootSignature, "No Vulkan RHI implementation graphics root signature set")
    const rhi::RootSignature& rootSignature = mGraphicsRootSignature->getRootSignature();
    BE_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The Vulkan RHI implementation root parameter index is out of bounds")
    const rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
    BE_ASSERT(rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The Vulkan RHI implementation root parameter index doesn't reference a descriptor table")
    BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The Vulkan RHI implementation descriptor ranges is a null pointer")
  }
#endif

  if (nullptr != resourceGroup) {
    // Bind Vulkan descriptor set
    const VkDescriptorSet vkDescriptorSet = static_cast<RHIResourceGroup*>(resourceGroup)->get_vk_descriptor_set();
    if (VK_NULL_HANDLE != vkDescriptorSet) {
      vkCmdBindDescriptorSets(get_vulkan_context().get_vk_command_buffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsRootSignature->get_vk_pipeline_layout(), rootParameterIndex, 1, &vkDescriptorSet, 0, nullptr);
    }
  } else {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDevice::set_graphics_vertex_array(rhi::RHIVertexArray* vertexArray) {
  // Input-assembler (IA) stage

  // New vertex array?
  if (mVertexArray != vertexArray) {
    // Set a vertex array?
    if (nullptr != vertexArray) {
      // Unset the currently used vertex array
      unset_graphics_vertex_array();

      // Set new vertex array and add a reference to it
      mVertexArray = static_cast<RHIVertexArray*>(vertexArray);
      mVertexArray->add_ref();

      // Bind Vulkan buffers
      mVertexArray->bind_vulkan_buffers(get_vulkan_context().get_vk_command_buffer());
    } else {
      // Unset the currently used vertex array
      unset_graphics_vertex_array();
    }
  }
}

void RHIDevice::set_graphics_viewports(core::uint32 numberOfViewports, const rhi::Viewport* viewports) {
  // Rasterizer (RS) stage

  // Sanity check
  BE_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid Vulkan rasterizer state viewports")

  // Set Vulkan viewport
  // -> We're using the "VK_KHR_maintenance1"-extension ("VK_KHR_MAINTENANCE1_EXTENSION_NAME"-definition) to be able to specify a negative viewport height,
  //    this way we don't have to apply "<output position>.y = -<output position>.y" inside vertex shaders to compensate for the Vulkan coordinate system
  // TODO(naetherm) Add support for multiple viewports
  VkViewport vkViewport = reinterpret_cast<const VkViewport*>(viewports)[0];
  vkViewport.y += vkViewport.height;
  vkViewport.height = -vkViewport.height;
  vkCmdSetViewport(get_vulkan_context().get_vk_command_buffer(), 0, 1, &vkViewport);
}

void RHIDevice::set_graphics_scissor_rectangles(core::uint32 numberOfScissorRectangles, const rhi::ScissorRectangle* scissorRectangles) {
  // Rasterizer (RS) stage

  // Sanity check
  BE_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid Vulkan rasterizer state scissor rectangles")

  // Set Vulkan scissor
  // TODO(naetherm) Add support for multiple scissor rectangles. Change "rhi::ScissorRectangle" to Vulkan style to make it the primary API on the long run?
  const VkRect2D vkRect2D = {
    { static_cast<int32_t>(scissorRectangles[0].topLeftX), static_cast<int32_t>(scissorRectangles[0].topLeftY) },
    { static_cast<core::uint32>(scissorRectangles[0].bottomRightX - scissorRectangles[0].topLeftX), static_cast<core::uint32>(scissorRectangles[0].bottomRightY - scissorRectangles[0].topLeftY) }
  };
  vkCmdSetScissor(get_vulkan_context().get_vk_command_buffer(), 0, 1, &vkRect2D);
}

void RHIDevice::set_graphics_render_target(rhi::RHIRenderTarget* renderTarget) {
  // Output-merger (OM) stage

  // New render target?
  if (mRenderTarget != renderTarget) {
    // Release the render target reference, in case we have one
    if (nullptr != mRenderTarget) {
      // Start Vulkan render pass, if necessary (for e.g. clearing)
      if (!mInsideVulkanRenderPass && ((mRenderTarget->get_resource_type() == rhi::ResourceType::SWAP_CHAIN && nullptr == renderTarget) || mRenderTarget->get_resource_type() == rhi::ResourceType::FRAMEBUFFER)) {
        begin_vulkan_render_pass();
      }

      // End Vulkan render pass, if necessary
      if (mInsideVulkanRenderPass) {
        vkCmdEndRenderPass(get_vulkan_context().get_vk_command_buffer());
        mInsideVulkanRenderPass = false;
      }

      // Release
      mRenderTarget->release();
      mRenderTarget = nullptr;
    }

    // Set a render target?
    if (nullptr != renderTarget) {
      // Set new render target and add a reference to it
      mRenderTarget = renderTarget;
      mRenderTarget->add_ref();

      // Set clear color and clear depth stencil values
      const core::uint32 numberOfColorAttachments = static_cast<const RHIRenderPass&>(mRenderTarget->get_render_pass()).get_number_of_color_attachments();
      BE_ASSERT(numberOfColorAttachments < 8, "Vulkan only supports 7 render pass color attachments")
      for (core::uint32 i = 0; i < numberOfColorAttachments; ++i) {
        mVkClearValues[i] = VkClearValue{ { { 0.0f, 0.0f, 0.0f, 1.0f } } };
      }
      mVkClearValues[numberOfColorAttachments] = VkClearValue{ { { 1.0f, 0 } } };
    }
  }
}

void RHIDevice::clear_graphics(core::uint32 clearFlags, const float color[4], float z, core::uint32 stencil) {
  // Sanity checks
  BE_ASSERT(nullptr != mRenderTarget, "Can't execute Vulkan clear command without a render target set")
  BE_ASSERT(!mInsideVulkanRenderPass, "Can't execute clear command inside a Vulkan render pass")
  BE_ASSERT(z >= 0.0f && z <= 1.0f, "The Vulkan clear graphics z value must be between [0, 1] (inclusive)")

  // Clear color
  const core::uint32 numberOfColorAttachments = 1;// TODO(naetherm): static_cast<const RHIRenderPass &>(mRenderTarget->get_render_pass()).get_number_of_color_attachments();
  BE_ASSERT(numberOfColorAttachments < 8, "Vulkan only supports 7 render pass color attachments")
  if (clearFlags & rhi::ClearFlag::COLOR) {
    for (core::uint32 i = 0; i < numberOfColorAttachments; ++i) {
      memcpy(mVkClearValues[i].color.float32, &color[0], sizeof(float) * 4);
    }
  }

  // Clear depth stencil
  if ((clearFlags & rhi::ClearFlag::DEPTH) || (clearFlags & rhi::ClearFlag::STENCIL)) {
    mVkClearValues[numberOfColorAttachments].depthStencil.depth = z;
    mVkClearValues[numberOfColorAttachments].depthStencil.stencil = stencil;
  }
}

void RHIDevice::draw_graphics(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  BE_ASSERT(numberOfDraws > 0, "Number of Vulkan draws must not be zero")
  // It's possible to draw without "mVertexArray"

  // Start Vulkan render pass, if necessary
  if (!mInsideVulkanRenderPass) {
    begin_vulkan_render_pass();
  }

  // Vulkan draw indirect command
  vkCmdDrawIndirect(get_vulkan_context().get_vk_command_buffer(), static_cast<const RHIIndirectBuffer&>(indirectBuffer).get_vk_buffer(), indirectBufferOffset, numberOfDraws, sizeof(VkDrawIndirectCommand));
}

void RHIDevice::draw_graphics_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  BE_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
  BE_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")
  // It's possible to draw without "mVertexArray"

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Start Vulkan render pass, if necessary
  if (!mInsideVulkanRenderPass) {
    begin_vulkan_render_pass();
  }

  // Emit the draw calls
#ifdef RHI_DEBUG
  if (numberOfDraws > 1) {
    beginDebugEvent("Multi-draw-indirect emulation");
  }
#endif
  const VkCommandBuffer vkCommandBuffer = get_vulkan_context().get_vk_command_buffer();
  for (core::uint32 i = 0; i < numberOfDraws; ++i) {
    // Draw and advance
    const rhi::DrawArguments& drawArguments = *reinterpret_cast<const rhi::DrawArguments*>(emulationData);
    vkCmdDraw(vkCommandBuffer, drawArguments.vertexCountPerInstance, drawArguments.instanceCount, drawArguments.startVertexLocation, drawArguments.startInstanceLocation);
    emulationData += sizeof(rhi::DrawArguments);
  }
#ifdef RHI_DEBUG
  if (numberOfDraws > 1) {
    endDebugEvent();
  }
#endif
}

void RHIDevice::draw_indexed_graphics(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  BE_ASSERT(numberOfDraws > 0, "Number of Vulkan draws must not be zero")
  BE_ASSERT(nullptr != mVertexArray, "Vulkan draw indexed needs a set vertex array")
  BE_ASSERT(nullptr != mVertexArray->get_index_buffer(), "Vulkan draw indexed needs a set vertex array which contains an index buffer")

  // Start Vulkan render pass, if necessary
  if (!mInsideVulkanRenderPass) {
    begin_vulkan_render_pass();
  }

  // Vulkan draw indexed indirect command
  vkCmdDrawIndexedIndirect(get_vulkan_context().get_vk_command_buffer(), static_cast<const RHIIndirectBuffer&>(indirectBuffer).get_vk_buffer(), indirectBufferOffset, numberOfDraws, sizeof(VkDrawIndexedIndirectCommand));
}

void RHIDevice::draw_indexed_graphics_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  BE_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
  BE_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")
  BE_ASSERT(nullptr != mVertexArray, "Vulkan draw indexed needs a set vertex array")
  BE_ASSERT(nullptr != mVertexArray->get_index_buffer(), "Vulkan draw indexed needs a set vertex array which contains an index buffer")

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Start Vulkan render pass, if necessary
  if (!mInsideVulkanRenderPass) {
    begin_vulkan_render_pass();
  }

  // Emit the draw calls
#ifdef RHI_DEBUG
  if (numberOfDraws > 1) {
    beginDebugEvent("Multi-indexed-draw-indirect emulation");
  }
#endif
  const VkCommandBuffer vkCommandBuffer = get_vulkan_context().get_vk_command_buffer();
  for (core::uint32 i = 0; i < numberOfDraws; ++i) {
    // Draw and advance
    const rhi::DrawIndexedArguments& drawIndexedArguments = *reinterpret_cast<const rhi::DrawIndexedArguments*>(emulationData);
    vkCmdDrawIndexed(vkCommandBuffer, drawIndexedArguments.indexCountPerInstance, drawIndexedArguments.instanceCount, drawIndexedArguments.startIndexLocation, drawIndexedArguments.baseVertexLocation, drawIndexedArguments.startInstanceLocation);
    emulationData += sizeof(rhi::DrawIndexedArguments);
  }
#ifdef RHI_DEBUG
  if (numberOfDraws > 1) {
    endDebugEvent();
  }
#endif
}

void RHIDevice::draw_mesh_tasks(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  BE_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")

  // TODO(naetherm) Implement me
  // vkCmdDrawMeshTasksIndirectNV
  // vkCmdDrawMeshTasksIndirectCountNV
}

void RHIDevice::draw_mesh_tasks_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  BE_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
  BE_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Emit the draw calls
#ifdef RHI_DEBUG
  if (numberOfDraws > 1) {
    beginDebugEvent("Multi-indexed-draw-indirect emulation");
  }
#endif
  const VkCommandBuffer vkCommandBuffer = get_vulkan_context().get_vk_command_buffer();
  for (core::uint32 i = 0; i < numberOfDraws; ++i) {
    const rhi::DrawMeshTasksArguments& drawMeshTasksArguments = *reinterpret_cast<const rhi::DrawMeshTasksArguments*>(emulationData);

    // Draw and advance
    vkCmdDrawMeshTasksNV(vkCommandBuffer, drawMeshTasksArguments.numberOfTasks, drawMeshTasksArguments.firstTask);
    emulationData += sizeof(rhi::DrawMeshTasksArguments);
  }
#ifdef RHI_DEBUG
  if (numberOfDraws > 1) {
    endDebugEvent();
  }
#endif
}

//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
void RHIDevice::set_compute_root_signature(rhi::RHIRootSignature* rootSignature) {
  if (nullptr != mComputeRootSignature) {
    mComputeRootSignature->release();
  }
  mComputeRootSignature = static_cast<RHIRootSignature*>(rootSignature);
  if (nullptr != mComputeRootSignature) {
    mComputeRootSignature->add_ref();
  }
}

void RHIDevice::set_compute_pipeline_state(rhi::RHIComputePipelineState* computePipelineState) {
  if (nullptr != computePipelineState) {
    // Bind Vulkan compute pipeline
    vkCmdBindPipeline(get_vulkan_context().get_vk_command_buffer(), VK_PIPELINE_BIND_POINT_COMPUTE, static_cast<RHIComputePipelineState*>(computePipelineState)->get_vk_pipeline());
  } else {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDevice::set_compute_resource_group(core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup) {
  // Security checks
#ifdef RHI_DEBUG
  {
    BE_ASSERT(nullptr != mComputeRootSignature, "No Vulkan RHI implementation compute root signature set")
    const rhi::RootSignature& rootSignature = mComputeRootSignature->getRootSignature();
    BE_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The Vulkan RHI implementation root parameter index is out of bounds")
    const rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
    BE_ASSERT(rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The Vulkan RHI implementation root parameter index doesn't reference a descriptor table")
    BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The Vulkan RHI implementation descriptor ranges is a null pointer")
  }
#endif

  if (nullptr != resourceGroup) {
    // Bind Vulkan descriptor set
    const VkDescriptorSet vkDescriptorSet = static_cast<RHIResourceGroup*>(resourceGroup)->get_vk_descriptor_set();
    if (VK_NULL_HANDLE != vkDescriptorSet) {
      vkCmdBindDescriptorSets(get_vulkan_context().get_vk_command_buffer(), VK_PIPELINE_BIND_POINT_COMPUTE, mComputeRootSignature->get_vk_pipeline_layout(), rootParameterIndex, 1, &vkDescriptorSet, 0, nullptr);
    }
  } else {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDevice::dispatch_compute(core::uint32 groupCountX, core::uint32 groupCountY, core::uint32 groupCountZ) {

}

//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void RHIDevice::resolve_multisample_framebuffer(rhi::RHIRenderTarget& destinationRenderTarget, rhi::RHIFramebuffer& sourceMultisampleFramebuffer) {

}

void RHIDevice::copy_resource(rhi::RHIResource& destinationResource, rhi::RHIResource& sourceResource) {

}

void RHIDevice::generate_mipmaps(rhi::RHIResource& resource) {

}

//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void RHIDevice::reset_query_pool(rhi::RHIQueryPool& queryPool, core::uint32 firstQueryIndex, core::uint32 numberOfQueries) {
  // Reset Vulkan query pool
  vkCmdResetQueryPool(get_vulkan_context().get_vk_command_buffer(), static_cast<const RHIQueryPool&>(queryPool).get_vk_query_pool(), firstQueryIndex, numberOfQueries);
}

void RHIDevice::begin_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex, core::uint32 queryControlFlags) {

  // Begin Vulkan query
  vkCmdBeginQuery(get_vulkan_context().get_vk_command_buffer(), static_cast<const RHIQueryPool&>(queryPool).get_vk_query_pool(), queryIndex, ((queryControlFlags & rhi::QueryControlFlags::PRECISE) != 0) ? VK_QUERY_CONTROL_PRECISE_BIT : 0u);
}

void RHIDevice::end_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex) {

  // End Vulkan query
  vkCmdEndQuery(get_vulkan_context().get_vk_command_buffer(), static_cast<const RHIQueryPool&>(queryPool).get_vk_query_pool(), queryIndex);
}

void RHIDevice::write_timestamp_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex) {
  // Write Vulkan timestamp query
  vkCmdWriteTimestamp(get_vulkan_context().get_vk_command_buffer(), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, static_cast<const RHIQueryPool&>(queryPool).get_vk_query_pool(), queryIndex);
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void RHIDevice::set_debug_marker(const char* name) {

}

void RHIDevice::begin_debug_event(const char* name) {

}

void RHIDevice::end_debug_event() {

}

#endif



void RHIDevice::initialize(rhi::RHIContext& rhiContext) override {
  // First, assign the rhi context
  mRhiContext = &rhiContext;
  mContext = &rhiContext;

#if DEBUG
  const bool enableValidation = true;
#else
  const bool enableValidation = false;
#endif

  // Create the vulkan runtime linking instance and check if vulkan is available
  mVulkanRuntimeLinking = new VulkanRuntimeLinking(*this, enableValidation);

  // TODO init volk
  VkResult result = volkInitialize();

  if (result == VK_SUCCESS && mVulkanRuntimeLinking->is_vulkan_available()) {
    // Vulkan is available, proceed with creating the vulkan context
    mVulkanContext = new VulkanContext(*this);

    if (mVulkanContext->is_initialized()) {
      // Initialize the capabilities of the current renderer
      initialize_capabilities();

      // TODO(naetherm): Create the default sampler state
      mDefaultSamplerState = create_sampler_state(rhi::RHISamplerState::get_default_sampler_state());
    }
  } else {
    BE_LOG(Critical, "Eeeeehm ... nope!")
  }
}

const char* RHIDevice::get_name() const override {
  return "Vulkan";
}

bool RHIDevice::is_initialized() const override {
  return mVulkanContext && mVulkanContext->is_initialized();
}

bool RHIDevice::is_debug_enabled() override {
  // Check for any "VK_EXT_debug_marker"-extension function pointer
  return (nullptr != vkDebugMarkerSetObjectTagEXT || nullptr != vkDebugMarkerSetObjectNameEXT || nullptr != vkCmdDebugMarkerBeginEXT || nullptr != vkCmdDebugMarkerEndEXT || nullptr != vkCmdDebugMarkerInsertEXT);
}

//[-------------------------------------------------------]
//[ Shader language                                       ]
//[-------------------------------------------------------]
core::uint32 RHIDevice::get_number_of_shader_languages() const override {
  return 1;
}

const char* RHIDevice::get_shader_language_name(core::uint32 index) const override {
  return VulkanHelper::GLSL_NAME;
}

rhi::RHIShaderLanguage* RHIDevice::get_shader_language(const char* shaderLanguageName) override {
  // In case "shaderLanguage" is a null pointer, use the default shader language
  if (nullptr != shaderLanguageName) {
    // Optimization: Check for shader language name pointer match, first
    // -> "ShaderLanguageSeparate::NAME" has the same value
    if (VulkanHelper::GLSL_NAME == shaderLanguageName || !stricmp(shaderLanguageName, VulkanHelper::GLSL_NAME)) {
      // If required, create the GLSL shader language instance right now
      if (nullptr == mShaderLanguageGlsl) {
        mShaderLanguageGlsl = new RHIShaderLanguageGlsl(*this);
        mShaderLanguageGlsl->add_ref();	// Internal RHI reference
      }
      return mShaderLanguageGlsl;
    }
  } else {
    // Return the shader language instance as default
    return get_shader_language(VulkanHelper::GLSL_NAME);
  }

  // Error!
  return nullptr;
}

//[-------------------------------------------------------]
//[ Resource creation                                     ]
//[-------------------------------------------------------]
rhi::RHIRenderPass* RHIDevice::create_render_pass(core::uint32 numberOfColorAttachments, const rhi::TextureFormat::Enum* colorAttachmentTextureFormats, rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, core::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) override {
  return new RHIRenderPass(*this, numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples);
}

rhi::RHIQueryPool* RHIDevice::create_query_pool(rhi::QueryType queryType, core::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) override {
  BE_ASSERT(numberOfQueries > 0, "Vulkan: Number of queries mustn't be zero")
  return new RHIQueryPool(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHISwapChain* RHIDevice::create_swap_chain(rhi::RHIRenderPass& renderPass, core::FrontendPlatformData frontendPlatformData, bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) override {
  //BE_ASSERT(NULL_HANDLE != windowHandle.nativeWindowHandle || nullptr != windowHandle.renderWindow, "Vulkan: The provided native window handle or render window must not be a null handle / null pointer")

  // Create the swap chain
  /// TODO(naetherm): Implement me!
  return new RHISwapChain(renderPass, frontendPlatformData RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIFramebuffer* RHIDevice::create_framebuffer(rhi::RHIRenderPass& renderPass, const rhi::FramebufferAttachment* colorFramebufferAttachments, const rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) override {
  // Create the framebuffer
  return new RHIFramebuffer(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIBufferManager* RHIDevice::create_buffer_manager() override {
  return new RHIBufferManager(*this);
}

rhi::RHITextureManager* RHIDevice::create_texture_manager() override {
  return new RHITextureManager(*this);
}

rhi::RHIRootSignature* RHIDevice::create_root_signature(const rhi::RootSignatureDescriptor& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) override {
  return new RHIRootSignature(*this, rootSignature RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIGraphicsPipelineState* RHIDevice::create_graphics_pipeline_state(const rhi::GraphicsPipelineStateDescriptor& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) override {
  // Sanity checks
  BE_ASSERT(nullptr != graphicsPipelineState.rootSignature, "Vulkan: Invalid graphics pipeline state root signature")
  BE_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "Vulkan: Invalid graphics pipeline state graphics program")
  BE_ASSERT(nullptr != graphicsPipelineState.renderPass, "Vulkan: Invalid graphics pipeline state render pass")

  // Create graphics pipeline state
  core::uint16 id = 0;
  if (GraphicsPipelineStateMakeId.create_id(id)) {
    return new RHIGraphicsPipelineState(*this, graphicsPipelineState, id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error: Ensure a correct reference counter behaviour
  graphicsPipelineState.rootSignature->add_ref();
  graphicsPipelineState.rootSignature->release();
  graphicsPipelineState.graphicsProgram->add_ref();
  graphicsPipelineState.graphicsProgram->release();
  graphicsPipelineState.renderPass->add_ref();
  graphicsPipelineState.renderPass->release();
  return nullptr;
}

rhi::RHIComputePipelineState* RHIDevice::create_compute_pipeline_state(rhi::RHIRootSignature& rootSignature, rhi::RHIComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) override {
  // Create the compute pipeline state
  core::uint16 id = 0;
  if (ComputePipelineStateMakeId.create_id(id)) {
    return new RHIComputePipelineState(*this, rootSignature, computeShader, id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error: Ensure a correct reference counter behaviour
  rootSignature.add_ref();
  rootSignature.release();
  computeShader.add_ref();
  computeShader.release();
  return nullptr;
}

rhi::RHISamplerState* RHIDevice::create_sampler_state(const rhi::SamplerStateDescriptor& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) override {
  return new RHISamplerState(*this, samplerState);
}

//[-------------------------------------------------------]
//[ Resource handling                                     ]
//[-------------------------------------------------------]
bool RHIDevice::map(rhi::RHIResource& resource, core::uint32 subresource, rhi::MapType mapType, core::uint32 mapFlags, rhi::MappedSubresource& mappedSubresource) override {
  // Evaluate the resource type
  switch (resource.get_resource_type()) {
    case rhi::ResourceType::VERTEX_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIVertexBuffer&>(resource).get_vk_device_memory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case rhi::ResourceType::INDEX_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIIndexBuffer&>(resource).get_vk_device_memory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case rhi::ResourceType::TEXTURE_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(get_vulkan_context().get_vk_device(), static_cast<RHITextureBuffer&>(resource).get_vk_device_memory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case rhi::ResourceType::STRUCTURED_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIStructuredBuffer&>(resource).get_vk_device_memory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case rhi::ResourceType::INDIRECT_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIIndirectBuffer&>(resource).get_vk_device_memory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case rhi::ResourceType::UNIFORM_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIUniformBuffer&>(resource).get_vk_device_memory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case rhi::ResourceType::TEXTURE_1D:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case rhi::ResourceType::TEXTURE_1D_ARRAY:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case rhi::ResourceType::TEXTURE_2D:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case rhi::ResourceType::TEXTURE_2D_ARRAY:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case rhi::ResourceType::TEXTURE_3D:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case rhi::ResourceType::TEXTURE_CUBE:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case rhi::ResourceType::ROOT_SIGNATURE:
    case rhi::ResourceType::RESOURCE_GROUP:
    case rhi::ResourceType::GRAPHICS_PROGRAM:
    case rhi::ResourceType::VERTEX_ARRAY:
    case rhi::ResourceType::RENDER_PASS:
    case rhi::ResourceType::QUERY_POOL:
    case rhi::ResourceType::SWAP_CHAIN:
    case rhi::ResourceType::FRAMEBUFFER:
    case rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
    case rhi::ResourceType::COMPUTE_PIPELINE_STATE:
    case rhi::ResourceType::SAMPLER_STATE:
    case rhi::ResourceType::VERTEX_SHADER:
    case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
    case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
    case rhi::ResourceType::GEOMETRY_SHADER:
    case rhi::ResourceType::FRAGMENT_SHADER:
    case rhi::ResourceType::TASK_SHADER:
    case rhi::ResourceType::MESH_SHADER:
    case rhi::ResourceType::COMPUTE_SHADER:
    default:
      // Nothing we can map, set known return values
      mappedSubresource.data		 = nullptr;
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;

      // Error!
      return false;
  }
}

void RHIDevice::unmap(rhi::RHIResource& resource, core::uint32 subresource) override {
  // Evaluate the resource type
  switch (resource.get_resource_type()) {
    case rhi::ResourceType::VERTEX_BUFFER:
    {
      vkUnmapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIVertexBuffer&>(resource).get_vk_device_memory());
      break;
    }

    case rhi::ResourceType::INDEX_BUFFER:
    {
      vkUnmapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIIndexBuffer&>(resource).get_vk_device_memory());
      break;
    }

    case rhi::ResourceType::TEXTURE_BUFFER:
    {
      vkUnmapMemory(get_vulkan_context().get_vk_device(), static_cast<RHITextureBuffer&>(resource).get_vk_device_memory());
      break;
    }

    case rhi::ResourceType::STRUCTURED_BUFFER:
    {
      vkUnmapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIStructuredBuffer&>(resource).get_vk_device_memory());
      break;
    }

    case rhi::ResourceType::INDIRECT_BUFFER:
    {
      vkUnmapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIIndirectBuffer&>(resource).get_vk_device_memory());
      break;
    }

    case rhi::ResourceType::UNIFORM_BUFFER:
    {
      vkUnmapMemory(get_vulkan_context().get_vk_device(), static_cast<RHIUniformBuffer&>(resource).get_vk_device_memory());
      break;
    }

    case rhi::ResourceType::TEXTURE_1D:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case rhi::ResourceType::TEXTURE_1D_ARRAY:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case rhi::ResourceType::TEXTURE_2D:
    {
      // TODO(naetherm) Implement me
      /*
      // Get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2D&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Unmap the Direct3D 11 resource
        mD3D11DeviceContext->Unmap(d3d11Resource, subresource);

        // Release the Direct3D 11 resource instance
        d3d11Resource->Release();
      }
      */
      break;
    }

    case rhi::ResourceType::TEXTURE_2D_ARRAY:
    {
      // TODO(naetherm) Implement me
      /*
      // Get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2DArray&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Unmap the Direct3D 11 resource
        mD3D11DeviceContext->Unmap(d3d11Resource, subresource);

        // Release the Direct3D 11 resource instance
        d3d11Resource->Release();
      }
      */
      break;
    }

    case rhi::ResourceType::TEXTURE_3D:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case rhi::ResourceType::TEXTURE_CUBE:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case rhi::ResourceType::ROOT_SIGNATURE:
    case rhi::ResourceType::RESOURCE_GROUP:
    case rhi::ResourceType::GRAPHICS_PROGRAM:
    case rhi::ResourceType::VERTEX_ARRAY:
    case rhi::ResourceType::RENDER_PASS:
    case rhi::ResourceType::QUERY_POOL:
    case rhi::ResourceType::SWAP_CHAIN:
    case rhi::ResourceType::FRAMEBUFFER:
    case rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
    case rhi::ResourceType::COMPUTE_PIPELINE_STATE:
    case rhi::ResourceType::SAMPLER_STATE:
    case rhi::ResourceType::VERTEX_SHADER:
    case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
    case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
    case rhi::ResourceType::GEOMETRY_SHADER:
    case rhi::ResourceType::FRAGMENT_SHADER:
    case rhi::ResourceType::TASK_SHADER:
    case rhi::ResourceType::MESH_SHADER:
    case rhi::ResourceType::COMPUTE_SHADER:
    default:
      // Nothing we can unmap
      break;
  }
}

bool RHIDevice::get_query_pool_results(rhi::RHIQueryPool& queryPool, core::uint32 numberOfDataBytes, core::uint8* data, core::uint32 firstQueryIndex, core::uint32 numberOfQueries, core::uint32 strideInBytes, core::uint32 queryResultFlags) override {

  // Query pool type dependent processing
  const RHIQueryPool& vulkanQueryPool = static_cast<const RHIQueryPool&>(queryPool);
  switch (vulkanQueryPool.get_query_type()) {
    case rhi::QueryType::OCCLUSION:
    case rhi::QueryType::TIMESTAMP:	// TODO(naetherm) Convert time to nanoseconds, see e.g. http://vulkan-spec-chunked.ahcox.com/ch16s05.html - VkPhysicalDeviceLimits::timestampPeriod - The number of nanoseconds it takes for a timestamp value to be incremented by 1
    {
      // Get Vulkan query pool results
      const VkQueryResultFlags vkQueryResultFlags = 0u;
      // const VkQueryResultFlags vkQueryResultFlags = ((queryResultFlags & rhi::QueryResultFlags::WAIT) != 0) ? VK_QUERY_RESULT_WAIT_BIT : 0u;	// TODO(naetherm)
      return (vkGetQueryPoolResults(get_vulkan_context().get_vk_device(), vulkanQueryPool.get_vk_query_pool(), firstQueryIndex, numberOfQueries, numberOfDataBytes, data, strideInBytes, VK_QUERY_RESULT_64_BIT | vkQueryResultFlags) == VK_SUCCESS);
    }

    case rhi::QueryType::PIPELINE_STATISTICS:
    {
      // Our setup results in the same structure layout as used by "D3D11_QUERY_DATA_PIPELINE_STATISTICS" which we use for "rhi::PipelineStatisticsQueryResult"
      const VkQueryResultFlags vkQueryResultFlags = 0u;
      // const VkQueryResultFlags vkQueryResultFlags = ((queryResultFlags & rhi::QueryResultFlags::WAIT) != 0) ? VK_QUERY_RESULT_WAIT_BIT : 0u;	// TODO(naetherm)
      return (vkGetQueryPoolResults(get_vulkan_context().get_vk_device(), vulkanQueryPool.get_vk_query_pool(), firstQueryIndex, numberOfQueries, numberOfDataBytes, data, strideInBytes, VK_QUERY_RESULT_64_BIT | vkQueryResultFlags) == VK_SUCCESS);
    }
  }

  // Result not ready
  return false;
}

//[-------------------------------------------------------]
//[ Operation                                             ]
//[-------------------------------------------------------]
void RHIDevice::dispatch_command_buffer(const rhi::RHICommandBuffer& commandBuffer) override {
  // Sanity check
  BE_ASSERT(!commandBuffer.is_empty(), "The Vulkan command buffer to dispatch mustn't be empty")

  // Begin Vulkan command buffer
  // -> This automatically resets the Vulkan command buffer in case it was previously already recorded
  static constexpr VkCommandBufferBeginInfo vkCommandBufferBeginInfo = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	// sType (VkStructureType)
    nullptr,										// pNext (const void*)
    0,												// flags (VkCommandBufferUsageFlags)
    nullptr											// pInheritanceInfo (const VkCommandBufferInheritanceInfo*)
  };
  if (vkBeginCommandBuffer(get_vulkan_context().get_vk_command_buffer(), &vkCommandBufferBeginInfo) == VK_SUCCESS) {
    // Dispatch command buffer
    dispatch_command_buffer_internal(commandBuffer);

    // We need to forget about the currently set render target
    // -> "Critical: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT" Object: "54336828" Location: "0" Message code: "0" Layer prefix: "Validation" Message: " [ VUID-vkEndCommandBuffer-commandBuffer-00060 ] Object: 0x33d1d3c (Type = 6) | vkEndCommandBuffer(): It is invalid to issue this call inside an active render pass (0x20). The Vulkan spec states: If commandBuffer is a primary command buffer, there must not be an active render pass instance (https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-vkEndCommandBuffer-commandBuffer-00060)" "
    set_graphics_render_target(nullptr);

    // We need to forget about the currently set vertex array
    // -> "Critical: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT" Object: "217049444" Location: "0" Message code: "0" Layer prefix: "Validation" Message: " [ UNASSIGNED-CoreValidation-DrawState-VtxIndexOutOfBounds ] Object: 0xcefe964 (Type = 6) | The Pipeline State Object (0x1b) expects that this Command Buffer's vertex binding Index 0 should be set via vkCmdBindVertexBuffers. This is because VkVertexInputBindingDescription struct at index 0 of pVertexBindingDescriptions has a binding value of 0." "
    unset_graphics_vertex_array();

    // End Vulkan command buffer
    if (vkEndCommandBuffer(get_vulkan_context().get_vk_command_buffer()) != VK_SUCCESS) {
      // Error!
      BE_LOG(Critical, "Failed to end Vulkan command buffer instance")
    }
  } else {
    // Error!
    BE_LOG(Critical, "Failed to begin Vulkan command buffer instance")
  }
}


void RHIDevice::initialize_capabilities() {
  { // Get device name
    VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
    vkGetPhysicalDeviceProperties(mVulkanContext->get_vk_physical_device(), &vkPhysicalDeviceProperties);
    const size_t numberOfCharacters = strlen(vkPhysicalDeviceProperties.deviceName) - 1;
    strncpy(mCapabilities.deviceName, vkPhysicalDeviceProperties.deviceName, numberOfCharacters);
    mCapabilities.deviceName[numberOfCharacters] = '\0';
  }

  // Preferred swap chain texture format
  mCapabilities.preferredSwapChainColorTextureFormat = (RHISwapChain::find_color_vk_format(get_context(),
                                                                                        mVulkanRuntimeLinking->get_vk_instance(),
                                                                                        *mVulkanContext) == VK_FORMAT_R8G8B8A8_UNORM) ? rhi::TextureFormat::Enum::R8G8B8A8 : rhi::TextureFormat::Enum::B8G8R8A8;

  { // Preferred swap chain depth stencil texture format
    const VkFormat depthVkFormat = RHISwapChain::find_depth_vk_format(mVulkanContext->get_vk_physical_device());
    if (VK_FORMAT_D32_SFLOAT == depthVkFormat) {
      mCapabilities.preferredSwapChainDepthStencilTextureFormat = rhi::TextureFormat::Enum::D32_FLOAT;
    } else {
      // TODO(naetherm) Add support for "VK_FORMAT_D32_SFLOAT_S8_UINT" and "VK_FORMAT_D24_UNORM_S8_UINT"
      mCapabilities.preferredSwapChainDepthStencilTextureFormat = rhi::TextureFormat::Enum::D32_FLOAT;
    }
  }

  // TODO(naetherm) Implement me, this in here is just a placeholder implementation

  { // "D3D_FEATURE_LEVEL_11_0"
    // Maximum number of viewports (always at least 1)
    mCapabilities.maximumNumberOfViewports = 16;

    // Maximum number of simultaneous render targets (if <1 render to texture is not supported)
    mCapabilities.maximumNumberOfSimultaneousRenderTargets = 8;

    // Maximum texture dimension
    mCapabilities.maximumTextureDimension = 16384;

    // Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
    mCapabilities.maximumNumberOf1DTextureArraySlices = 512;

    // Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
    mCapabilities.maximumNumberOf2DTextureArraySlices = 512;

    // Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
    mCapabilities.maximumNumberOfCubeTextureArraySlices = 512;

    // Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
    mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 128 * 1024 * 1024;	// TODO(naetherm) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention the texture buffer? Currently the OpenGL 3 minimum is used: 128 MiB.

    // Maximum indirect buffer size in bytes
    mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

    // Maximum number of multisamples (always at least 1, usually 8)
    mCapabilities.maximumNumberOfMultisamples = 1;	// TODO(naetherm) Add multisample support, when setting "VkPhysicalDeviceFeatures" -> "sampleRateShading" -> VK_TRUE don't forget to check whether or not the device sup pots the feature

    // Maximum anisotropy (always at least 1, usually 16)
    mCapabilities.maximumAnisotropy = 16;

    // Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
    mCapabilities.instancedArrays = true;

    // Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
    mCapabilities.drawInstanced = true;

    // Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
    mCapabilities.maximumNumberOfPatchVertices = 32;

    // Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
    mCapabilities.maximumNumberOfGsOutputVertices = 1024;	// TODO(naetherm) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention it, so I assume it's 1024
  }

  // The rest is the same for all feature levels

  // Maximum uniform buffer (UBO) size in bytes (usually at least 4096 * 16 bytes, in case there's no support for uniform buffer it's 0)
  // -> See https://msdn.microsoft.com/en-us/library/windows/desktop/ff819065(v=vs.85).aspx - "Resource Limits (Direct3D 11)" - "Number of elements in a constant buffer D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096)"
  // -> One element = float4 = 16 bytes
  mCapabilities.maximumUniformBufferSize = 4096 * 16;

  // Left-handed coordinate system with clip space depth value range 0..1
  mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = true;

  // Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
  mCapabilities.individualUniforms = false;

  // Base vertex supported for draw calls?
  mCapabilities.baseVertex = true;

  // Vulkan has native multithreading
  mCapabilities.nativeMultithreading = false;	// TODO(naetherm) Enable native multithreading when done

  // Vulkan has shader bytecode support
  mCapabilities.shaderBytecode = false;	// TODO(naetherm) Vulkan has shader bytecode support, set "mCapabilities.shaderBytecode" to true later on

  // Is there support for vertex shaders (VS)?
  mCapabilities.vertexShader = true;

  // Is there support for fragment shaders (FS)?
  mCapabilities.fragmentShader = true;

  // Is there support for task shaders (TS) and mesh shaders (MS)?
  mCapabilities.meshShader = (nullptr != vkCmdDrawMeshTasksNV);

  // Is there support for compute shaders (CS)?
  mCapabilities.computeShader = true;
}

void RHIDevice::unset_graphics_vertex_array()
{
  // Release the currently used vertex array reference, in case we have one
  if (nullptr != mVertexArray) {
    // Do nothing since the Vulkan specification says "bindingCount must be greater than 0"
    // vkCmdBindVertexBuffers(get_vulkan_context().get_vk_command_buffer(), 0, 0, nullptr, nullptr);

    // Release reference
    mVertexArray->release();
    mVertexArray = nullptr;
  }
}

void RHIDevice::begin_vulkan_render_pass()
{
  // Sanity checks
  BE_ASSERT(!mInsideVulkanRenderPass, "We're already inside a Vulkan render pass")
  BE_ASSERT(nullptr != mRenderTarget, "Can't begin a Vulkan render pass without a render target set")

  // Start Vulkan render pass
  const core::uint32 numberOfAttachments = static_cast<const RHIRenderPass&>(mRenderTarget->get_render_pass()).get_number_of_attachments();
  BE_ASSERT(numberOfAttachments < 9, "Vulkan only supports 8 render pass attachments")
  switch (mRenderTarget->get_resource_type()) {
    case rhi::ResourceType::SWAP_CHAIN:
    {
      const RHISwapChain* swapChain = static_cast<RHISwapChain*>(mRenderTarget);
      VulkanHelper::begin_vulkan_render_pass(*mRenderTarget, swapChain->get_vk_render_pass(), swapChain->get_current_vk_framebuffer(), numberOfAttachments, mVkClearValues, get_vulkan_context().get_vk_command_buffer());
      break;
    }

    case rhi::ResourceType::FRAMEBUFFER:
    {
      const RHIFramebuffer* framebuffer = static_cast<RHIFramebuffer*>(mRenderTarget);
      VulkanHelper::begin_vulkan_render_pass(*mRenderTarget, framebuffer->get_vk_render_pass(), framebuffer->get_vk_framebuffer(), numberOfAttachments, mVkClearValues, get_vulkan_context().get_vk_command_buffer());
      break;
    }

    case rhi::ResourceType::ROOT_SIGNATURE:
    case rhi::ResourceType::RESOURCE_GROUP:
    case rhi::ResourceType::GRAPHICS_PROGRAM:
    case rhi::ResourceType::VERTEX_ARRAY:
    case rhi::ResourceType::RENDER_PASS:
    case rhi::ResourceType::QUERY_POOL:
    case rhi::ResourceType::VERTEX_BUFFER:
    case rhi::ResourceType::INDEX_BUFFER:
    case rhi::ResourceType::TEXTURE_BUFFER:
    case rhi::ResourceType::STRUCTURED_BUFFER:
    case rhi::ResourceType::INDIRECT_BUFFER:
    case rhi::ResourceType::UNIFORM_BUFFER:
    case rhi::ResourceType::TEXTURE_1D:
    case rhi::ResourceType::TEXTURE_1D_ARRAY:
    case rhi::ResourceType::TEXTURE_2D:
    case rhi::ResourceType::TEXTURE_2D_ARRAY:
    case rhi::ResourceType::TEXTURE_3D:
    case rhi::ResourceType::TEXTURE_CUBE:
    case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
    case rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
    case rhi::ResourceType::COMPUTE_PIPELINE_STATE:
    case rhi::ResourceType::SAMPLER_STATE:
    case rhi::ResourceType::VERTEX_SHADER:
    case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
    case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
    case rhi::ResourceType::GEOMETRY_SHADER:
    case rhi::ResourceType::FRAGMENT_SHADER:
    case rhi::ResourceType::TASK_SHADER:
    case rhi::ResourceType::MESH_SHADER:
    case rhi::ResourceType::COMPUTE_SHADER:
    default:
      // Not handled in here
      break;
  }
  mInsideVulkanRenderPass = true;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_vulkan


RHI_VULKAN_API rhi::RHIDevice* create_rhi_device_instance() {
  return new rhi_vulkan::RHIDevice();
}