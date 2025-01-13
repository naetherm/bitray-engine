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
#include "rhi_opengl/rhidynamicrhi.h"
#include "rhi_opengl/opengl_runtime_linking.h"
#include "rhi_opengl/rhiopengl_context.h"
#include "rhi_opengl/rhiroot_signature.h"
#include "rhi_opengl/rhiresource_group.h"
#include "rhi_opengl/query/rhipipeline_statistics_query_pool.h"
#include "rhi_opengl/query/rhiocclusion_timestamp_query_pool.h"
#include "rhi_opengl/buffer/rhibuffer_manager.h"
#include "rhi_opengl/buffer/rhiindex_buffer.h"
#include "rhi_opengl/buffer/rhiindirect_buffer.h"
#include "rhi_opengl/buffer/rhistructured_buffer.h"
#include "rhi_opengl/buffer/rhitexture_buffer.h"
#include "rhi_opengl/buffer/rhiuniform_buffer.h"
#include "rhi_opengl/buffer/rhivertex_array.h"
#include "rhi_opengl/buffer/rhivertex_array_no_vao.h"
#include "rhi_opengl/buffer/rhivertex_array_vao.h"
#include "rhi_opengl/buffer/rhivertex_buffer.h"
#include "rhi_opengl/render_target/rhiframebuffer_bind.h"
#include "rhi_opengl/render_target/rhiframebuffer_dsa.h"
#include "rhi_opengl/render_target/rhirender_pass.h"
#include "rhi_opengl/render_target/rhiswap_chain.h"
#include "rhi_opengl/texture/rhitexture_manager.h"
#include "rhi_opengl/texture/rhitexture_1d.h"
#include "rhi_opengl/texture/rhitexture_1d_bind.h"
#include "rhi_opengl/texture/rhitexture_1d_dsa.h"
#include "rhi_opengl/texture/rhitexture_1d_array.h"
#include "rhi_opengl/texture/rhitexture_1d_array_bind.h"
#include "rhi_opengl/texture/rhitexture_1d_array_dsa.h"
#include "rhi_opengl/texture/rhitexture_2d.h"
#include "rhi_opengl/texture/rhitexture_2d_bind.h"
#include "rhi_opengl/texture/rhitexture_2d_dsa.h"
#include "rhi_opengl/texture/rhitexture_2d_array.h"
#include "rhi_opengl/texture/rhitexture_2d_array_bind.h"
#include "rhi_opengl/texture/rhitexture_2d_array_dsa.h"
#include "rhi_opengl/texture/rhitexture_3d.h"
#include "rhi_opengl/texture/rhitexture_3d_bind.h"
#include "rhi_opengl/texture/rhitexture_3d_dsa.h"
#include "rhi_opengl/texture/rhitexture_cube_bind.h"
#include "rhi_opengl/texture/rhitexture_cube_dsa.h"
#include "rhi_opengl/texture/rhitexture_cube_array.h"
#include "rhi_opengl/texture/rhitexture_cube_array_bind.h"
#include "rhi_opengl/texture/rhitexture_cube_array_dsa.h"
#include "rhi_opengl/shader/monolithic/rhicompute_shader_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhishader_language_monolithic.h"
#include "rhi_opengl/shader/monolithic/rhigraphics_program_monolithic_dsa.h"
#include "rhi_opengl/shader/separate/rhicompute_shader_separate.h"
#include "rhi_opengl/shader/separate/rhishader_language_separate.h"
#include "rhi_opengl/shader/separate/rhigraphics_program_separate_dsa.h"
#include "rhi_opengl/shader/separate/rhivertex_shader_separate.h"
#include "rhi_opengl/state/rhidepth_stencil_state.h"
#include "rhi_opengl/state/rhirasterizer_state.h"
#include "rhi_opengl/state/rhiblend_state.h"
#include "rhi_opengl/state/rhicompute_pipeline_state_monolithic.h"
#include "rhi_opengl/state/rhicompute_pipeline_state_separate.h"
#include "rhi_opengl/state/rhigraphics_pipeline_state.h"
#include "rhi_opengl/state/rhisampler_state.h"
#include "rhi_opengl/state/rhisampler_state_bind.h"
#include "rhi_opengl/state/rhisampler_state_dsa.h"
#include "rhi_opengl/state/rhisampler_state_so.h"
#include "rhi_opengl/extensions.h"
#include "rhi_opengl/helper.h"
#include "rhi_opengl/mapping.h"
#if defined(LINUX)
#include "rhi_opengl/linux/rhiopengl_context_linux.h"
#endif


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
namespace detail
{


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
// Implementation from "08/02/2015 Better array 'countof' implementation with C++ 11 (updated)" - https://www.g-truc.net/post-0708.html
template<typename T, std::size_t N>
[[nodiscard]] constexpr std::size_t countof(T const (&)[N]) {
  return N;
}

[[nodiscard]] bool mapBuffer(const rhi_opengl::Extensions& extensions, GLenum target, [[maybe_unused]] GLenum bindingTarget, GLuint openGLBuffer, rhi::MapType mapType, rhi::MappedSubresource& mappedSubresource) {
  // TODO(naetherm) This buffer update isn't efficient, use e.g. persistent buffer mapping

  // Is "GL_ARB_direct_state_access" there?
  if (extensions.isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    mappedSubresource.data		 = rhi_opengl::glMapNamedBuffer(openGLBuffer, rhi_opengl::Mapping::get_opengl_map_type(mapType));
    mappedSubresource.rowPitch   = 0;
    mappedSubresource.depthPitch = 0;
  }
    // Is "GL_EXT_direct_state_access" there?
  else if (extensions.isGL_EXT_direct_state_access()) {
    // Effective direct state access (DSA)
    mappedSubresource.data		 = rhi_opengl::glMapNamedBufferEXT(openGLBuffer, rhi_opengl::Mapping::get_opengl_map_type(mapType));
    mappedSubresource.rowPitch   = 0;
    mappedSubresource.depthPitch = 0;
  } else {
    // Traditional bind version

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL buffer
    GLint openGLBufferBackup = 0;
    glGetIntegerv(bindingTarget, &openGLBufferBackup);
#endif

    // Bind this OpenGL buffer
    rhi_opengl::glBindBufferARB(target, openGLBuffer);

    // Map
    mappedSubresource.data		 = rhi_opengl::glMapBufferARB(target, rhi_opengl::Mapping::get_opengl_map_type(mapType));
    mappedSubresource.rowPitch   = 0;
    mappedSubresource.depthPitch = 0;

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL buffer
    glBindBufferARB(target, static_cast<GLuint>(openGLBufferBackup));
#endif
  }

  // Done
  BE_ASSERT(nullptr != mappedSubresource.data, "Mapping of OpenGL buffer failed")
  return (nullptr != mappedSubresource.data);
}

void unmapBuffer(const rhi_opengl::Extensions& extensions, GLenum target, [[maybe_unused]] GLenum bindingTarget, GLuint openGLBuffer) {
  // Is "GL_ARB_direct_state_access" there?
  if (extensions.isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    rhi_opengl::glUnmapNamedBuffer(openGLBuffer);
  }
    // Is "GL_EXT_direct_state_access" there?
  else if (extensions.isGL_EXT_direct_state_access()) {
    // Effective direct state access (DSA)
    rhi_opengl::glUnmapNamedBufferEXT(openGLBuffer);
  } else {
    // Traditional bind version

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL buffer
    GLint openGLBufferBackup = 0;
    glGetIntegerv(bindingTarget, &openGLBufferBackup);
#endif

    // Bind this OpenGL buffer
    rhi_opengl::glBindBufferARB(target, openGLBuffer);

    // Unmap
    rhi_opengl::glUnmapBufferARB(target);

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL buffer
    glBindBufferARB(target, static_cast<GLuint>(openGLBufferBackup));
#endif
  }
}

namespace ImplementationDispatch {


//[-------------------------------------------------------]
//[ Command buffer                                        ]
//[-------------------------------------------------------]
void dispatch_command_buffer(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DispatchCommandBuffer* realData = static_cast<const rhi::Command::DispatchCommandBuffer*>(data);
  BE_ASSERT(nullptr != realData->commandBufferToDispatch, "The OpenGL command buffer to dispatch must be valid")
  static_cast<rhi_opengl::RHIDevice &>(rhi).dispatch_command_buffer_internal(*realData->commandBufferToDispatch);
}

//[-------------------------------------------------------]
//[ Graphics                                              ]
//[-------------------------------------------------------]
void set_graphics_root_signature(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetGraphicsRootSignature* realData = static_cast<const rhi::Command::SetGraphicsRootSignature*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_graphics_root_signature(realData->rootSignature);
}

void set_graphics_pipeline_state(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetGraphicsPipelineState* realData = static_cast<const rhi::Command::SetGraphicsPipelineState*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_graphics_pipeline_state(realData->graphicsPipelineState);
}

void set_graphics_resource_group(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetGraphicsResourceGroup* realData = static_cast<const rhi::Command::SetGraphicsResourceGroup*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_graphics_resource_group(realData->rootParameterIndex, realData->resourceGroup);
}

void set_graphics_vertex_array(const void* data, rhi::RHIDevice& rhi) {
  // Input-assembler (IA) stage
  const rhi::Command::SetGraphicsVertexArray* realData = static_cast<const rhi::Command::SetGraphicsVertexArray*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_graphics_vertex_array(realData->vertexArray);
}

void set_graphics_viewports(const void* data, rhi::RHIDevice& rhi) {
  // Rasterizer (RS) stage
  const rhi::Command::SetGraphicsViewports* realData = static_cast<const rhi::Command::SetGraphicsViewports*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_graphics_viewports(realData->numberOfViewports,
                                                                        (nullptr != realData->viewports)
                                                                        ? realData->viewports
                                                                        : reinterpret_cast<const rhi::Viewport *>(rhi::CommandPacketHelper::getAuxiliaryMemory(
                                                                          realData)));
}

void set_graphics_scissor_rectangles(const void* data, rhi::RHIDevice& rhi) {
  // Rasterizer (RS) stage
  const rhi::Command::SetGraphicsScissorRectangles* realData = static_cast<const rhi::Command::SetGraphicsScissorRectangles*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_graphics_scissor_rectangles(realData->numberOfScissorRectangles,
                                                                                 (nullptr !=
                                                                                  realData->scissorRectangles)
                                                                                 ? realData->scissorRectangles
                                                                                 : reinterpret_cast<const rhi::ScissorRectangle *>(rhi::CommandPacketHelper::getAuxiliaryMemory(
                                                                                   realData)));
}

void set_graphics_render_target(const void* data, rhi::RHIDevice& rhi) {
  // Output-merger (OM) stage
  const rhi::Command::SetGraphicsRenderTarget* realData = static_cast<const rhi::Command::SetGraphicsRenderTarget*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_graphics_render_target(realData->renderTarget);
}

void clear_graphics(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::ClearGraphics* realData = static_cast<const rhi::Command::ClearGraphics*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).clear_graphics(realData->clearFlags, realData->color, realData->z,
                                                                realData->stencil);
}

void draw_graphics(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DrawGraphics* realData = static_cast<const rhi::Command::DrawGraphics*>(data);
  if (nullptr != realData->indirectBuffer) {
    static_cast<rhi_opengl::RHIDevice &>(rhi).draw_graphics(*realData->indirectBuffer,
                                                                 realData->indirectBufferOffset,
                                                                 realData->numberOfDraws);
  } else {
    static_cast<rhi_opengl::RHIDevice &>(rhi).draw_graphics_emulated(
      rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void draw_indexed_graphics(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DrawIndexedGraphics* realData = static_cast<const rhi::Command::DrawIndexedGraphics*>(data);
  if (nullptr != realData->indirectBuffer) {
    static_cast<rhi_opengl::RHIDevice &>(rhi).draw_indexed_graphics(*realData->indirectBuffer,
                                                                         realData->indirectBufferOffset,
                                                                         realData->numberOfDraws);
  } else {
    static_cast<rhi_opengl::RHIDevice &>(rhi).draw_indexed_graphics_emulated(
      rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void draw_mesh_tasks(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DrawMeshTasks* realData = static_cast<const rhi::Command::DrawMeshTasks*>(data);
  if (nullptr != realData->indirectBuffer) {
    static_cast<rhi_opengl::RHIDevice &>(rhi).draw_mesh_tasks(*realData->indirectBuffer,
                                                                   realData->indirectBufferOffset,
                                                                   realData->numberOfDraws);
  } else {
    static_cast<rhi_opengl::RHIDevice &>(rhi).draw_mesh_tasks_emulated(
      rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
void set_compute_root_signature(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetComputeRootSignature* realData = static_cast<const rhi::Command::SetComputeRootSignature*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_compute_root_signature(realData->rootSignature);
}

void set_compute_pipeline_state(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetComputePipelineState* realData = static_cast<const rhi::Command::SetComputePipelineState*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_compute_pipeline_state(realData->computePipelineState);
}

void set_compute_resource_group(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetComputeResourceGroup* realData = static_cast<const rhi::Command::SetComputeResourceGroup*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).set_compute_resource_group(realData->rootParameterIndex, realData->resourceGroup);
}

void dispatch_compute(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::DispatchCompute* realData = static_cast<const rhi::Command::DispatchCompute*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).dispatch_compute(realData->groupCountX, realData->groupCountY, realData->groupCountZ);
}

//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void set_texture_minimum_maximum_mipmap_index(const void* data, [[maybe_unused]] rhi::RHIDevice& rhi) {
  const rhi::Command::SetTextureMinimumMaximumMipmapIndex* realData = static_cast<const rhi::Command::SetTextureMinimumMaximumMipmapIndex*>(data);
  BE_ASSERT(realData->texture->get_resource_type() == rhi::ResourceType::TEXTURE_2D, "Unsupported OpenGL texture resource type")
  static_cast<rhi_opengl::RHITexture2D*>(realData->texture)->setMinimumMaximumMipmapIndex(realData->minimumMipmapIndex, realData->maximumMipmapIndex);
}

void resolve_multisample_framebuffer(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::ResolveMultisampleFramebuffer* realData = static_cast<const rhi::Command::ResolveMultisampleFramebuffer*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).resolve_multisample_framebuffer(*realData->destinationRenderTarget, *realData->sourceMultisampleFramebuffer);
}

void copy_resource(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::CopyResource* realData = static_cast<const rhi::Command::CopyResource*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).copy_resource(*realData->destinationResource, *realData->sourceResource);
}

void generate_mipmaps(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::GenerateMipmaps* realData = static_cast<const rhi::Command::GenerateMipmaps*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).generate_mipmaps(*realData->resource);
}

void copy_uniform_buffer_data(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::CopyUniformBufferData* realData = static_cast<const rhi::Command::CopyUniformBufferData*>(data);
  rhi::MappedSubresource mappedSubresource;
  if (rhi.map(*realData->uniformBuffer, 0, rhi::MapType::WRITE_DISCARD, 0, mappedSubresource)) {
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
  static_cast<rhi_opengl::RHIDevice &>(rhi).reset_query_pool(*realData->queryPool, realData->firstQueryIndex, realData->numberOfQueries);
}

void begin_query(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::BeginQuery* realData = static_cast<const rhi::Command::BeginQuery*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).begin_query(*realData->queryPool, realData->queryIndex, realData->queryControlFlags);
}

void end_query(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::EndQuery* realData = static_cast<const rhi::Command::EndQuery*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).end_query(*realData->queryPool, realData->queryIndex);
}

void write_timestamp_query(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::WriteTimestampQuery* realData = static_cast<const rhi::Command::WriteTimestampQuery*>(data);
  static_cast<rhi_opengl::RHIDevice &>(rhi).write_timestamp_query(*realData->queryPool, realData->queryIndex);
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void set_debug_marker(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::SetDebugMarker* realData = static_cast<const rhi::Command::SetDebugMarker*>(data);
  static_cast<rhi_opengl::RHIDevice&>(rhi).set_debug_marker(realData->name);
}

void begin_debug_event(const void* data, rhi::RHIDevice& rhi) {
  const rhi::Command::BeginDebugEvent* realData = static_cast<const rhi::Command::BeginDebugEvent*>(data);
  static_cast<rhi_opengl::RHIDevice&>(rhi).begin_debug_event(realData->name);
}

void end_debug_event(const void*, rhi::RHIDevice& rhi) {
  static_cast<rhi_opengl::RHIDevice&>(rhi).end_debug_event();
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


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {



//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIDevice::RHIDevice()
: rhi::RHIDevice(rhi::NameId::OPENGL)
, VertexArrayMakeId()
, GraphicsPipelineStateMakeId()
, ComputePipelineStateMakeId()
, mOpenGLRuntimeLinking(nullptr)
, mOpenGLContext(nullptr)
, mExtensions(nullptr)
, mShaderLanguage(nullptr)
, mGraphicsRootSignature(nullptr)
, mComputeRootSignature(nullptr)
, mDefaultSamplerState(nullptr)
, mOpenGLCopyResourceFramebuffer(0)
, mDefaultOpenGLVertexArray(0)
  // States
, mGraphicsPipelineState(nullptr)
, mComputePipelineState(nullptr)
  // Input-assembler (IA) stage
, mVertexArray(nullptr)
, mOpenGLPrimitiveTopology(0xFFFF)	// Unknown default setting
, mNumberOfVerticesPerPatch(0)
  // Output-merger (OM) stage
, mRenderTarget(nullptr)
  // State cache to avoid making redundant OpenGL calls
, mOpenGLClipControlOrigin(GL_INVALID_ENUM)
, mOpenGLProgramPipeline(0)
, mOpenGLProgram(0)
, mOpenGLIndirectBuffer(0)
  // Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
, mOpenGLVertexProgram(0)
, mDrawIdUniformLocation(-1)
, mCurrentStartInstanceLocation(~0u) {

}

RHIDevice::~RHIDevice() {
  // Set no graphics and compute pipeline state reference, in case we have one
  if (nullptr != mGraphicsPipelineState) {
    set_graphics_pipeline_state(nullptr);
  }
  if (nullptr != mComputePipelineState) {
    set_compute_pipeline_state(nullptr);
  }

  // Set no vertex array reference, in case we have one
  if (nullptr != mVertexArray) {
    set_graphics_vertex_array(nullptr);
  }

  // release instances
  if (nullptr != mRenderTarget) {
    mRenderTarget->release();
    mRenderTarget = nullptr;
  }
  if (nullptr != mDefaultSamplerState) {
    mDefaultSamplerState->release();
    mDefaultSamplerState = nullptr;
  }

  // Destroy the OpenGL framebuffer used by "RHIDevice::RHIDevice::copy_resource()" if the "GL_ARB_copy_image"-extension isn't available
  // -> Silently ignores 0's and names that do not correspond to existing buffer objects
  glDeleteFramebuffers(1, &mOpenGLCopyResourceFramebuffer);

  // Destroy the OpenGL default vertex array
  // -> Silently ignores 0's and names that do not correspond to existing vertex array objects
  glDeleteVertexArrays(1, &mDefaultOpenGLVertexArray);

  // release the graphics and compute root signature instance, in case we have one
  if (nullptr != mGraphicsRootSignature) {
    mGraphicsRootSignature->release();
  }
  if (nullptr != mComputeRootSignature) {
    mComputeRootSignature->release();
  }

#ifdef RHI_STATISTICS
  { // For debugging: At this point there should be no resource instances left, validate this!
    // -> Are the currently any resource instances?
    const core::uint32 numberOfCurrentResources = getStatistics().getNumberOfCurrentResources();
    if (numberOfCurrentResources > 0) {
      // Error!
      if (numberOfCurrentResources > 1) {
        BE_ASSERT(false, "The OpenGL RHI implementation is going to be destroyed, but there are still %lu resource instances left (memory leak)", numberOfCurrentResources)
      } else {
        BE_ASSERT(false, "The OpenGL RHI implementation is going to be destroyed, but there is still one resource instance left (memory leak)")
      }

      // Use debug output to show the current number of resource instances
      getStatistics().debugOutputCurrentResouces(mContext);
    }
  }
#endif

  // release the shader language instance, in case we have one
  if (nullptr != mShaderLanguage) {
    mShaderLanguage->release();
  }

  // Destroy the extensions instance
  re_delete(mExtensions);

  // Destroy the OpenGL context instance
  re_delete(mOpenGLContext);

  // Destroy the OpenGL runtime linking instance
  re_delete(mOpenGLRuntimeLinking);
}

void RHIDevice::dispatch_command_buffer_internal(const rhi::RHICommandBuffer& commandBuffer) {
  // Loop through all commands
  const core::uint8* commandPacketBuffer = commandBuffer.get_command_packet_buffer();
  rhi::ConstCommandPacket constCommandPacket = commandPacketBuffer;
  while (nullptr != constCommandPacket) {
    { // Dispatch command packet
      const rhi::CommandDispatchFunctionIndex commandDispatchFunctionIndex = rhi::CommandPacketHelper::loadCommandDispatchFunctionIndex(constCommandPacket);
      const void* command = rhi::CommandPacketHelper::loadCommand(constCommandPacket);
      detail::DISPATCH_FUNCTIONS[static_cast<core::uint32>(commandDispatchFunctionIndex)](command, *this);
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

    // Sanity check
    RHI_MATCH_CHECK(*this, *rootSignature)
  }
}

void RHIDevice::set_graphics_pipeline_state(rhi::RHIGraphicsPipelineState* graphicsPipelineState) {
  if (mGraphicsPipelineState != graphicsPipelineState) {
    if (nullptr != graphicsPipelineState) {
      // Sanity check
      RHI_MATCH_CHECK(*this, *graphicsPipelineState)

      // Set new graphics pipeline state and add a reference to it
      if (nullptr != mGraphicsPipelineState) {
        mGraphicsPipelineState->release();
      }
      mGraphicsPipelineState = static_cast<RHIGraphicsPipelineState*>(graphicsPipelineState);
      mGraphicsPipelineState->add_ref();

      // Set OpenGL primitive topology
      mOpenGLPrimitiveTopology = mGraphicsPipelineState->get_opengl_primitive_topology();
      const int newNumberOfVerticesPerPatch = mGraphicsPipelineState->get_number_of_vertices_per_patch();
      if (0 != newNumberOfVerticesPerPatch && mNumberOfVerticesPerPatch != newNumberOfVerticesPerPatch) {
        mNumberOfVerticesPerPatch = newNumberOfVerticesPerPatch;
        glPatchParameteri(GL_PATCH_VERTICES, mNumberOfVerticesPerPatch);
      }

      // Set graphics pipeline state
      mGraphicsPipelineState->bind_graphics_pipeline_state();
    }
    else if (nullptr != mGraphicsPipelineState) {
      // TODO(naetherm) Handle this situation by resetting OpenGL states?
      mGraphicsPipelineState->release();
      mGraphicsPipelineState = nullptr;
    }
  }
  // Set graphics pipeline state
  else if (nullptr != mGraphicsPipelineState) {
    // Set OpenGL graphics pipeline state
    // -> This is necessary since OpenGL is using just a single current program, for graphics as well as compute
    set_opengl_graphics_program(mGraphicsPipelineState->get_graphics_program());
  }
}

void RHIDevice::set_graphics_resource_group(core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup) {
  // Security checks
#ifdef DEBUG
  {
    BE_ASSERT(nullptr != mGraphicsRootSignature, "No OpenGL RHI implementation graphics root signature set")
    const rhi::RootSignatureDescriptor& rootSignature = mGraphicsRootSignature->get_root_signature();
    BE_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The OpenGL RHI implementation root parameter index is out of bounds")
    const rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
    BE_ASSERT(rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The OpenGL RHI implementation root parameter index doesn't reference a descriptor table")
    BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The OpenGL RHI implementation descriptor ranges is a null pointer")
  }
#endif

  // Set graphics resource group
  set_resource_group(*mGraphicsRootSignature, rootParameterIndex, resourceGroup);
}

void RHIDevice::set_graphics_vertex_array(rhi::RHIVertexArray* vertexArray) {
  // Input-assembler (IA) stage

  // New vertex array?
  if (mVertexArray != vertexArray) {
    // Set a vertex array?
    if (nullptr != vertexArray) {
      // Sanity check
      RHI_MATCH_CHECK(*this, *vertexArray)

      // Unset the currently used vertex array
      unset_graphics_vertex_array();

      // Set new vertex array and add a reference to it
      mVertexArray = static_cast<RHIVertexArray*>(vertexArray);
      mVertexArray->add_ref();

      // Evaluate the internal array type of the new vertex array to set
      switch (static_cast<RHIVertexArray *>(mVertexArray)->get_internal_resource_type()) {
        case RHIVertexArray::InternalResourceType::NO_VAO:
          // Enable OpenGL vertex attribute arrays
          static_cast<RHIVertexArrayNoVao *>(mVertexArray)->enable_opengl_vertex_attrib_arrays();
          break;

        case RHIVertexArray::InternalResourceType::VAO:
          // Bind OpenGL vertex array
          glBindVertexArray(static_cast<RHIVertexArrayVao *>(mVertexArray)->get_opengl_vertex_array());
          break;
      }
    } else {
      // Unset the currently used vertex array
      unset_graphics_vertex_array();
    }
  }
}

void RHIDevice::set_graphics_viewports([[maybe_unused]] core::uint32 numberOfViewports, const rhi::Viewport* viewports) {
  // Rasterizer (RS) stage

  // Sanity check
  BE_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid OpenGL rasterizer state viewports")

  // In OpenGL, the origin of the viewport is left bottom while Direct3D is using a left top origin. To make the
  // Direct3D 11 implementation as efficient as possible the Direct3D convention is used and we have to convert in here.
  // -> This isn't influenced by the "GL_ARB_clip_control"-extension

  // get the width and height of the current render target
  core::uint32 renderTargetHeight = 1;
  if (nullptr != mRenderTarget) {
    core::uint32 renderTargetWidth = 1;
    mRenderTarget->get_width_and_height(renderTargetWidth, renderTargetHeight);
  }

  // Set the OpenGL viewport
  // TODO(naetherm) "GL_ARB_viewport_array" support ("RHIDevice::set_graphics_viewports()")
  // TODO(naetherm) Check for "numberOfViewports" out of range or are the debug events good enough?
  BE_ASSERT(numberOfViewports <= 1, "OpenGL supports only one viewport")
  glViewport(static_cast<GLint>(viewports->topLeftX), static_cast<GLint>(static_cast<float>(renderTargetHeight) - viewports->topLeftY - viewports->height), static_cast<GLsizei>(viewports->width), static_cast<GLsizei>(viewports->height));
  glDepthRange(static_cast<GLclampd>(viewports->minDepth), static_cast<GLclampd>(viewports->maxDepth));
}

void RHIDevice::set_graphics_scissor_rectangles([[maybe_unused]] core::uint32 numberOfScissorRectangles, const rhi::ScissorRectangle* scissorRectangles) {
  // Rasterizer (RS) stage

  // Sanity check
  BE_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid OpenGL rasterizer state scissor rectangles")

  // In OpenGL, the origin of the scissor rectangle is left bottom while Direct3D is using a left top origin. To make the
  // Direct3D 9 & 10 & 11 implementation as efficient as possible the Direct3D convention is used and we have to convert in here.
  // -> This isn't influenced by the "GL_ARB_clip_control"-extension

  // get the width and height of the current render target
  core::uint32 renderTargetHeight = 1;
  if (nullptr != mRenderTarget) {
    core::uint32 renderTargetWidth = 1;
    mRenderTarget->get_width_and_height(renderTargetWidth, renderTargetHeight);
  }

  // Set the OpenGL scissor rectangle
  // TODO(naetherm) "GL_ARB_viewport_array" support ("RHIDevice::set_graphics_viewports()")
  // TODO(naetherm) Check for "numberOfViewports" out of range or are the debug events good enough?
  BE_ASSERT(numberOfScissorRectangles <= 1, "OpenGL supports only one scissor rectangle")
  const GLsizei width  = scissorRectangles->bottomRightX - scissorRectangles->topLeftX;
  const GLsizei height = scissorRectangles->bottomRightY - scissorRectangles->topLeftY;
  glScissor(static_cast<GLint>(scissorRectangles->topLeftX), static_cast<GLint>(renderTargetHeight - static_cast<core::uint32>(scissorRectangles->topLeftY) - height), width, height);
}

void RHIDevice::set_graphics_render_target(rhi::RHIRenderTarget* renderTarget) {
  // Output-merger (OM) stage

  // New render target?
  if (mRenderTarget != renderTarget) {
    // Set a render target?
    if (nullptr != renderTarget) {
      // Sanity check
      RHI_MATCH_CHECK(*this, *renderTarget)

      // release the render target reference, in case we have one
      if (nullptr != mRenderTarget) {
        // Unbind OpenGL framebuffer?
        if (rhi::ResourceType::FRAMEBUFFER == mRenderTarget->get_resource_type() && rhi::ResourceType::FRAMEBUFFER != renderTarget->get_resource_type()) {
          // Do we need to disable multisample?
          if (static_cast<RHIFramebuffer *>(mRenderTarget)->is_multisample_render_target()) {
            glDisable(GL_MULTISAMPLE);
          }

          // We do not render into a OpenGL framebuffer
          glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // release
        mRenderTarget->release();
      }

      // Set new render target and add a reference to it
      mRenderTarget = renderTarget;
      mRenderTarget->add_ref();

      // Evaluate the render target type
      GLenum clipControlOrigin = GL_UPPER_LEFT;
      switch (mRenderTarget->get_resource_type()) {
        case rhi::ResourceType::SWAP_CHAIN:
        {
          static_cast<RHISwapChain *>(mRenderTarget)->get_opengl_context().make_current();
          clipControlOrigin = GL_LOWER_LEFT;	// Compensate OS window coordinate system y-flip
          break;
        }

        case rhi::ResourceType::FRAMEBUFFER:
        {
          // get the OpenGL framebuffer instance
          RHIFramebuffer* framebuffer = static_cast<RHIFramebuffer*>(mRenderTarget);

          // Bind the OpenGL framebuffer
          glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->get_opengl_framebuffer());

          // Define the OpenGL buffers to draw into, "GL_ARB_draw_buffers"-extension required
          if (mExtensions->isGL_ARB_draw_buffers()) {
            // https://www.opengl.org/registry/specs/ARB/draw_buffers.txt - "The draw buffer for output colors beyond <n> is set to NONE."
            // -> Meaning depth only rendering which has no color textures at all will work as well, no need for "glDrawBuffer(GL_NONE)"
            static constexpr GLenum OPENGL_DRAW_BUFFER[16] = {
              GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3,
              GL_COLOR_ATTACHMENT4,  GL_COLOR_ATTACHMENT5,  GL_COLOR_ATTACHMENT6,  GL_COLOR_ATTACHMENT7,
              GL_COLOR_ATTACHMENT8,  GL_COLOR_ATTACHMENT9,  GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
              GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15
            };
            glDrawBuffersARB(static_cast<GLsizei>(framebuffer->get_number_of_color_textures()), OPENGL_DRAW_BUFFER);
          }

          // Do we need to enable multisample?
          if (framebuffer->is_multisample_render_target()) {
            glEnable(GL_MULTISAMPLE);
          } else {
            glDisable(GL_MULTISAMPLE);
          }
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

      // Setup clip control
      if (mOpenGLClipControlOrigin != clipControlOrigin && mExtensions->isGL_ARB_clip_control()) {
        // OpenGL default is "GL_LOWER_LEFT" and "GL_NEGATIVE_ONE_TO_ONE", change it to match Vulkan and Direct3D
        mOpenGLClipControlOrigin = clipControlOrigin;
        glClipControl(mOpenGLClipControlOrigin, GL_ZERO_TO_ONE);
      }
    }
    else if (nullptr != mRenderTarget) {
      // Evaluate the render target type
      if (rhi::ResourceType::FRAMEBUFFER == mRenderTarget->get_resource_type()) {
        // We do not render into a OpenGL framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }

      // TODO(naetherm) Set no active render target

      // release the render target reference, in case we have one
      mRenderTarget->release();
      mRenderTarget = nullptr;
    }
  }
}

void RHIDevice::clear_graphics(core::uint32 clearFlags, const float color[4], float z, core::uint32 stencil) {
  // Sanity check
  BE_ASSERT(z >= 0.0f && z <= 1.0f, "The OpenGL clear graphics z value must be between [0, 1] (inclusive)")

  // get API flags
  core::uint32 flagsApi = 0;
  if (clearFlags & rhi::ClearFlag::COLOR) {
    flagsApi |= GL_COLOR_BUFFER_BIT;
  }
  if (clearFlags & rhi::ClearFlag::DEPTH) {
    flagsApi |= GL_DEPTH_BUFFER_BIT;
  }
  if (clearFlags & rhi::ClearFlag::STENCIL) {
    flagsApi |= GL_STENCIL_BUFFER_BIT;
  }

  // Are API flags set?
  if (0 != flagsApi) {
    // Set clear settings
    if (clearFlags & rhi::ClearFlag::COLOR) {
      glClearColor(color[0], color[1], color[2], color[3]);
    }
    if (clearFlags & rhi::ClearFlag::DEPTH) {
      glClearDepth(static_cast<GLclampd>(z));
      if (nullptr != mGraphicsPipelineState && rhi::DepthWriteMask::ALL != mGraphicsPipelineState->get_depth_stencil_state().depthWriteMask) {
        glDepthMask(GL_TRUE);
      }
    }
    if (clearFlags & rhi::ClearFlag::STENCIL) {
      glClearStencil(static_cast<GLint>(stencil));
    }

    // Unlike OpenGL, when using Direct3D 10 & 11 the scissor rectangle(s) do not affect the clear operation
    // -> We have to compensate the OpenGL behaviour in here

    // Disable OpenGL scissor test, in case it's not disabled, yet
    if (nullptr != mGraphicsPipelineState && mGraphicsPipelineState->get_rasterizer_state().scissorEnable) {
      glDisable(GL_SCISSOR_TEST);
    }

    // Clear
    glClear(flagsApi);

    // Restore the previously set OpenGL states
    if (nullptr != mGraphicsPipelineState && mGraphicsPipelineState->get_rasterizer_state().scissorEnable) {
      glEnable(GL_SCISSOR_TEST);
    }
    if ((clearFlags & rhi::ClearFlag::DEPTH) && nullptr != mGraphicsPipelineState && rhi::DepthWriteMask::ALL != mGraphicsPipelineState->get_depth_stencil_state().depthWriteMask) {
      glDepthMask(GL_FALSE);
    }
  }
}

void RHIDevice::draw_graphics(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, indirectBuffer)
  BE_ASSERT(numberOfDraws > 0, "Number of OpenGL draws must not be zero")
  BE_ASSERT(mExtensions->isGL_ARB_draw_indirect(), "The GL_ARB_draw_indirect OpenGL extension isn't supported")
  // It's possible to draw without "mVertexArray"

  // Tessellation support: "glPatchParameteri()" is called within "RHIDevice::iaSetPrimitiveTopology()"

  { // Bind indirect buffer
    const GLuint openGLIndirectBuffer = static_cast<const RHIIndirectBuffer &>(indirectBuffer).get_opengl_indirect_buffer();
    if (openGLIndirectBuffer != mOpenGLIndirectBuffer) {
      mOpenGLIndirectBuffer = openGLIndirectBuffer;
      glBindBufferARB(GL_DRAW_INDIRECT_BUFFER, mOpenGLIndirectBuffer);
    }
  }

  // Draw indirect
  if (1 == numberOfDraws) {
    glDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
  }
  else if (numberOfDraws > 1) {
    if (mExtensions->isGL_ARB_multi_draw_indirect()) {
      glMultiDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)), static_cast<GLsizei>(numberOfDraws), 0);	// 0 = tightly packed
    } else {
      // Emulate multi-draw-indirect
#ifdef DEBUG
      begin_debug_event("Multi-draw-indirect emulation");
#endif
      for (core::uint32 i = 0; i < numberOfDraws; ++i) {
        glDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
        indirectBufferOffset += sizeof(rhi::DrawArguments);
      }
#ifdef DEBUG
      end_debug_event();
#endif
    }
  }
}

void RHIDevice::draw_graphics_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  BE_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
  BE_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")
  // It's possible to draw without "mVertexArray"

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1) {
    begin_debug_event("Multi-draw-indirect emulation");
  }
#endif
  for (core::uint32 i = 0; i < numberOfDraws; ++i) {
    const rhi::DrawArguments& drawArguments = *reinterpret_cast<const rhi::DrawArguments*>(emulationData);
    updateGL_ARB_base_instanceEmulation(drawArguments.startInstanceLocation);

    // Draw and advance
    if ((drawArguments.instanceCount > 1 && mExtensions->isGL_ARB_draw_instanced()) || (drawArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance())) {
      // With instancing
      if (drawArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance()) {
        glDrawArraysInstancedBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance), static_cast<GLsizei>(drawArguments.instanceCount), drawArguments.startInstanceLocation);
      } else {
        glDrawArraysInstancedARB(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance), static_cast<GLsizei>(drawArguments.instanceCount));
      }
    } else {
      // Without instancing
      BE_ASSERT(drawArguments.instanceCount <= 1, "Invalid OpenGL instance count")
      glDrawArrays(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance));
    }
    emulationData += sizeof(rhi::DrawArguments);
  }
#ifdef DEBUG
  if (numberOfDraws > 1) {
    end_debug_event();
  }
#endif
}

void RHIDevice::draw_indexed_graphics(const rhi::RHIIndirectBuffer& indirectBuffer, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, indirectBuffer)
  BE_ASSERT(numberOfDraws > 0, "Number of OpenGL draws must not be zero")
  BE_ASSERT(nullptr != mVertexArray, "OpenGL draw indexed needs a set vertex array")
  BE_ASSERT(nullptr != mVertexArray->get_index_buffer(), "OpenGL draw indexed needs a set vertex array which contains an index buffer")
  BE_ASSERT(mExtensions->isGL_ARB_draw_indirect(), "The GL_ARB_draw_indirect OpenGL extension isn't supported")

  // Tessellation support: "glPatchParameteri()" is called within "RHIDevice::iaSetPrimitiveTopology()"

  { // Bind indirect buffer
    const GLuint openGLIndirectBuffer = static_cast<const RHIIndirectBuffer &>(indirectBuffer).get_opengl_indirect_buffer();
    if (openGLIndirectBuffer != mOpenGLIndirectBuffer) {
      mOpenGLIndirectBuffer = openGLIndirectBuffer;
      glBindBufferARB(GL_DRAW_INDIRECT_BUFFER, mOpenGLIndirectBuffer);
    }
  }

  // Draw indirect
  if (1 == numberOfDraws) {
    glDrawElementsIndirect(mOpenGLPrimitiveTopology, mVertexArray->get_index_buffer()->get_opengl_type(), reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
  }
  else if (numberOfDraws > 1) {
    if (mExtensions->isGL_ARB_multi_draw_indirect()) {
      glMultiDrawElementsIndirect(mOpenGLPrimitiveTopology, mVertexArray->get_index_buffer()->get_opengl_type(), reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)), static_cast<GLsizei>(numberOfDraws), 0);	// 0 = tightly packed
    } else {
      // Emulate multi-indexed-draw-indirect
#ifdef DEBUG
      begin_debug_event("Multi-indexed-draw-indirect emulation");
#endif
      const core::uint32 openGLType = mVertexArray->get_index_buffer()->get_opengl_type();
      for (core::uint32 i = 0; i < numberOfDraws; ++i) {
        glDrawElementsIndirect(mOpenGLPrimitiveTopology, openGLType, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
        indirectBufferOffset += sizeof(rhi::DrawIndexedArguments);
      }
#ifdef DEBUG
      end_debug_event();
#endif
    }
  }
}

void RHIDevice::draw_indexed_graphics_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  BE_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
  BE_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")
  BE_ASSERT(nullptr != mVertexArray, "OpenGL draw indexed needs a set vertex array")
  BE_ASSERT(nullptr != mVertexArray->get_index_buffer(), "OpenGL draw indexed needs a set vertex array which contains an index buffer")

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1) {
    begin_debug_event("Multi-indexed-draw-indirect emulation");
  }
#endif
  RHIIndexBuffer* indexBuffer = mVertexArray->get_index_buffer();
  for (core::uint32 i = 0; i < numberOfDraws; ++i) {
    const rhi::DrawIndexedArguments& drawIndexedArguments = *reinterpret_cast<const rhi::DrawIndexedArguments*>(emulationData);
    updateGL_ARB_base_instanceEmulation(drawIndexedArguments.startInstanceLocation);

    // Draw and advance
    if ((drawIndexedArguments.instanceCount > 1 && mExtensions->isGL_ARB_draw_instanced()) || (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance())) {
      // With instancing
      if (drawIndexedArguments.baseVertexLocation > 0) {
        // Use start instance location?
        if (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance()) {
          // Draw with base vertex location and start instance location
          glDrawElementsInstancedBaseVertexBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance),
                                                        indexBuffer->get_opengl_type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation *
              indexBuffer->get_index_size_in_bytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), static_cast<GLint>(drawIndexedArguments.baseVertexLocation), drawIndexedArguments.startInstanceLocation);
        }

        // Is the "GL_ARB_draw_elements_base_vertex" extension there?
        else if (mExtensions->isGL_ARB_draw_elements_base_vertex()) {
          // Draw with base vertex location
          glDrawElementsInstancedBaseVertex(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance),
                                            indexBuffer->get_opengl_type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation *
              indexBuffer->get_index_size_in_bytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), static_cast<GLint>(drawIndexedArguments.baseVertexLocation));
        } else {
          // Error!
          BE_ASSERT(false, "Failed to OpenGL draw indexed emulated")
        }
      }
      else if (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance()) {
        // Draw without base vertex location and with start instance location
        glDrawElementsInstancedBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance),
                                            indexBuffer->get_opengl_type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation *
            indexBuffer->get_index_size_in_bytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), drawIndexedArguments.startInstanceLocation);
      } else {
        // Draw without base vertex location
        glDrawElementsInstancedARB(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance),
                                   indexBuffer->get_opengl_type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation *
            indexBuffer->get_index_size_in_bytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount));
      }
    } else {
      // Without instancing
      BE_ASSERT(drawIndexedArguments.instanceCount <= 1, "Invalid OpenGL instance count")

      // Use base vertex location?
      if (drawIndexedArguments.baseVertexLocation > 0) {
        // Is the "GL_ARB_draw_elements_base_vertex" extension there?
        if (mExtensions->isGL_ARB_draw_elements_base_vertex()) {
          // Draw with base vertex location
          glDrawElementsBaseVertex(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance),
                                   indexBuffer->get_opengl_type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation *
              indexBuffer->get_index_size_in_bytes())), static_cast<GLint>(drawIndexedArguments.baseVertexLocation));
        } else {
          // Error!
          BE_ASSERT(false, "Failed to OpenGL draw indexed emulated")
        }
      } else {
        // Draw without base vertex location
        glDrawElements(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance),
                       indexBuffer->get_opengl_type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation *
            indexBuffer->get_index_size_in_bytes())));
      }
    }
    emulationData += sizeof(rhi::DrawIndexedArguments);
  }
#ifdef DEBUG
  if (numberOfDraws > 1) {
    end_debug_event();
  }
#endif
}

void RHIDevice::draw_mesh_tasks([[maybe_unused]] const rhi::RHIIndirectBuffer& indirectBuffer, [[maybe_unused]] core::uint32 indirectBufferOffset, [[maybe_unused]] core::uint32 numberOfDraws) {
  // Sanity checks
  BE_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")

  // TODO(naetherm) Implement me
  /*
  void MultiDrawMeshTasksIndirectNV(intptr indirect,
                  sizei drawcount,
                  sizei stride);

  void MultiDrawMeshTasksIndirectCountNV( intptr indirect,
                      intptr drawcount,
                      sizei maxdrawcount,
                      sizei stride);
  */
}

void RHIDevice::draw_mesh_tasks_emulated(const core::uint8* emulationData, core::uint32 indirectBufferOffset, core::uint32 numberOfDraws) {
  // Sanity checks
  BE_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
  BE_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1) {
    begin_debug_event("Multi-indexed-draw-indirect emulation");
  }
#endif
  for (core::uint32 i = 0; i < numberOfDraws; ++i) {
    const rhi::DrawMeshTasksArguments& drawMeshTasksArguments = *reinterpret_cast<const rhi::DrawMeshTasksArguments*>(emulationData);

    // Draw and advance
    glDrawMeshTasksNV(drawMeshTasksArguments.firstTask, drawMeshTasksArguments.numberOfTasks);
    emulationData += sizeof(rhi::DrawMeshTasksArguments);
  }
#ifdef DEBUG
  if (numberOfDraws > 1) {
    end_debug_event();
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

    // Sanity check
    RHI_MATCH_CHECK(*this, *rootSignature)
  }
}

void RHIDevice::set_compute_pipeline_state(rhi::RHIComputePipelineState* computePipelineState) {
  if (mComputePipelineState != computePipelineState) {
    if (nullptr != computePipelineState) {
      // Sanity check
      RHI_MATCH_CHECK(*this, *computePipelineState)

      // Set new compute pipeline state and add a reference to it
      if (nullptr != mComputePipelineState) {
        mComputePipelineState->release();
      }
      mComputePipelineState = static_cast<RHIComputePipelineState*>(computePipelineState);
      mComputePipelineState->add_ref();

      // Set OpenGL compute pipeline state
      set_opengl_compute_pipeline_state(mComputePipelineState);
    }
    else if (nullptr != mComputePipelineState) {
      // TODO(naetherm) Handle this situation by resetting OpenGL states?
      mComputePipelineState->release();
      mComputePipelineState = nullptr;
    }
  }

    // Set compute pipeline state
  else if (nullptr != mComputePipelineState) {
    // Set OpenGL compute pipeline state
    // -> This is necessary since OpenGL is using just a single current program, for graphics as well as compute
    set_opengl_compute_pipeline_state(mComputePipelineState);
  }
}

void RHIDevice::set_compute_resource_group(core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup) {
  // Security checks
#ifdef DEBUG
  {
    BE_ASSERT(nullptr != mComputeRootSignature, "No OpenGL RHI implementation compute root signature set")
    const rhi::RootSignatureDescriptor& rootSignature = mComputeRootSignature->get_root_signature();
    BE_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The OpenGL RHI implementation root parameter index is out of bounds")
    const rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
    BE_ASSERT(rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The OpenGL RHI implementation root parameter index doesn't reference a descriptor table")
    BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The OpenGL RHI implementation descriptor ranges is a null pointer")
  }
#endif

  // Set compute resource group
  set_resource_group(*mComputeRootSignature, rootParameterIndex, resourceGroup);
}

void RHIDevice::dispatch_compute(core::uint32 groupCountX, core::uint32 groupCountY, core::uint32 groupCountZ) {
  // "GL_ARB_compute_shader"-extension required
  if (mExtensions->isGL_ARB_compute_shader()) {
    glDispatchCompute(groupCountX, groupCountY, groupCountZ);

    // TODO(naetherm) Compute shader: Memory barrier currently fixed build in: Make sure writing to image has finished before read
    glMemoryBarrierEXT(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glMemoryBarrierEXT(GL_SHADER_STORAGE_BARRIER_BIT);
  }
}


//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void RHIDevice::resolve_multisample_framebuffer(rhi::RHIRenderTarget& destinationRenderTarget, rhi::RHIFramebuffer& sourceMultisampleFramebuffer) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, destinationRenderTarget)
  RHI_MATCH_CHECK(*this, sourceMultisampleFramebuffer)

  // Evaluate the render target type
  switch (destinationRenderTarget.get_resource_type()) {
    case rhi::ResourceType::SWAP_CHAIN:
    {
      // get the OpenGL swap chain instance
      // TODO(naetherm) Implement me, not that important in practice so not directly implemented
      // SwapChain& swapChain = static_cast<SwapChain&>(destinationRenderTarget);
      break;
    }

    case rhi::ResourceType::FRAMEBUFFER:
    {
      // get the OpenGL framebuffer instances
      const RHIFramebuffer& openGLDestinationFramebuffer = static_cast<const RHIFramebuffer&>(destinationRenderTarget);
      const RHIFramebuffer& openGLSourceMultisampleFramebuffer = static_cast<const RHIFramebuffer&>(sourceMultisampleFramebuffer);

      // get the width and height of the destination and source framebuffer
      core::uint32 destinationWidth = 1;
      core::uint32 destinationHeight = 1;
      openGLDestinationFramebuffer.get_width_and_height(destinationWidth, destinationHeight);
      core::uint32 sourceWidth = 1;
      core::uint32 sourceHeight = 1;
      openGLSourceMultisampleFramebuffer.get_width_and_height(sourceWidth, sourceHeight);

      // Resolve multisample
      glBindFramebuffer(GL_READ_FRAMEBUFFER, openGLSourceMultisampleFramebuffer.get_opengl_framebuffer());
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, openGLDestinationFramebuffer.get_opengl_framebuffer());
      glBlitFramebuffer(
        0, 0, static_cast<GLint>(sourceWidth), static_cast<GLint>(sourceHeight),			// Source
        0, 0, static_cast<GLint>(destinationWidth), static_cast<GLint>(destinationHeight),	// Destination
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
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
}

void RHIDevice::copy_resource(rhi::RHIResource& destinationResource, rhi::RHIResource& sourceResource) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, destinationResource)
  RHI_MATCH_CHECK(*this, sourceResource)

  // Evaluate the render target type
  switch (destinationResource.get_resource_type()) {
    case rhi::ResourceType::TEXTURE_2D:
      if (sourceResource.get_resource_type() == rhi::ResourceType::TEXTURE_2D) {
        // get the OpenGL texture 2D instances
        const RHITexture2D& openGlDestinationTexture2D = static_cast<const RHITexture2D&>(destinationResource);
        const RHITexture2D& openGlSourceTexture2D = static_cast<const RHITexture2D&>(sourceResource);
        BE_ASSERT(openGlDestinationTexture2D.get_width() == openGlSourceTexture2D.get_width(), "OpenGL source and destination width must be identical for resource copy")
        BE_ASSERT(openGlDestinationTexture2D.get_height() == openGlSourceTexture2D.get_height(), "OpenGL source and destination height must be identical for resource copy")

        // Copy resource, but only the top-level mipmap
        const GLsizei width = static_cast<GLsizei>(openGlDestinationTexture2D.get_width());
        const GLsizei height = static_cast<GLsizei>(openGlDestinationTexture2D.get_height());
        if (mExtensions->isGL_ARB_copy_image()) {
          glCopyImageSubData(openGlSourceTexture2D.get_opengl_texture(), GL_TEXTURE_2D, 0, 0, 0, 0,
                             openGlDestinationTexture2D.get_opengl_texture(), GL_TEXTURE_2D, 0, 0, 0, 0,
                             width, height, 1);
        } else {
#ifdef RHI_OPENGL_STATE_CLEANUP
          // Backup the currently bound OpenGL framebuffer
          GLint openGLFramebufferBackup = 0;
          glGetIntegerv(GL_FRAMEBUFFER_BINDING, &openGLFramebufferBackup);
#endif

          // Copy resource by using a framebuffer, but only the top-level mipmap
          if (0 == mOpenGLCopyResourceFramebuffer) {
            glGenFramebuffers(1, &mOpenGLCopyResourceFramebuffer);
          }
          glBindFramebuffer(GL_FRAMEBUFFER, mOpenGLCopyResourceFramebuffer);
          glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, openGlSourceTexture2D.get_opengl_texture(), 0);
          glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, openGlDestinationTexture2D.get_opengl_texture(), 0);
          static constexpr GLenum OPENGL_DRAW_BUFFER[1] = {
            GL_COLOR_ATTACHMENT1
          };
          glDrawBuffersARB(1, OPENGL_DRAW_BUFFER);	// We could use "glDrawBuffer(GL_COLOR_ATTACHMENT1);", but then we would also have to get the "glDrawBuffer()" function pointer, avoid OpenGL function overkill
          glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

#ifdef RHI_OPENGL_STATE_CLEANUP
          // Be polite and restore the previous bound OpenGL framebuffer
          glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(openGLFramebufferBackup));
#endif
        }
      } else {
        // Error!
        BE_ASSERT(false, "Failed to copy OpenGL resource")
      }
      break;

    case rhi::ResourceType::ROOT_SIGNATURE:
    case rhi::ResourceType::RESOURCE_GROUP:
    case rhi::ResourceType::GRAPHICS_PROGRAM:
    case rhi::ResourceType::VERTEX_ARRAY:
    case rhi::ResourceType::RENDER_PASS:
    case rhi::ResourceType::QUERY_POOL:
    case rhi::ResourceType::SWAP_CHAIN:
    case rhi::ResourceType::FRAMEBUFFER:
    case rhi::ResourceType::VERTEX_BUFFER:
    case rhi::ResourceType::INDEX_BUFFER:
    case rhi::ResourceType::TEXTURE_BUFFER:
    case rhi::ResourceType::STRUCTURED_BUFFER:
    case rhi::ResourceType::INDIRECT_BUFFER:
    case rhi::ResourceType::UNIFORM_BUFFER:
    case rhi::ResourceType::TEXTURE_1D:
    case rhi::ResourceType::TEXTURE_1D_ARRAY:
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
}

void RHIDevice::generate_mipmaps(rhi::RHIResource& resource) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, resource)
  BE_ASSERT(resource.get_resource_type() == rhi::ResourceType::TEXTURE_2D, "TODO(naetherm) Mipmaps can only be generated for OpenGL 2D texture resources")

  RHITexture2D& texture2D = static_cast<RHITexture2D&>(resource);

  // Is "GL_EXT_direct_state_access" there?
  if (mExtensions->isGL_ARB_direct_state_access()) {
    // Effective direct state access (DSA)
    glGenerateTextureMipmap(texture2D.get_opengl_texture());
  }
  else if (mExtensions->isGL_EXT_direct_state_access()) {
    // Effective direct state access (DSA)
    glGenerateTextureMipmapEXT(texture2D.get_opengl_texture(), GL_TEXTURE_2D);
  } else {
    // Traditional bind version

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL texture
    // TODO(naetherm) It's possible to avoid calling this multiple times
    GLint openGLTextureBackup = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLTextureBackup);
#endif

    // Generate mipmaps
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, texture2D.get_opengl_texture());
    glGenerateMipmap(GL_TEXTURE_2D);

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL texture
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLTextureBackup));
#endif
  }
}


//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void RHIDevice::reset_query_pool([[maybe_unused]] rhi::RHIQueryPool& queryPool, [[maybe_unused]] core::uint32 firstQueryIndex, [[maybe_unused]] core::uint32 numberOfQueries) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, queryPool)
  BE_ASSERT(firstQueryIndex < static_cast<const RHIQueryPool &>(queryPool).get_number_of_queries(), "OpenGL out-of-bounds query index")
  BE_ASSERT((firstQueryIndex + numberOfQueries) <= static_cast<const RHIQueryPool &>(queryPool).get_number_of_queries(), "OpenGL out-of-bounds query index")

  // Nothing to do in here for OpenGL
}

void RHIDevice::begin_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex, core::uint32 queryControlFlags) {
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Query pool type dependent processing
  const RHIQueryPool& openGLQueryPool = static_cast<const RHIQueryPool&>(queryPool);
  BE_ASSERT(queryIndex < openGLQueryPool.get_number_of_queries(), "OpenGL out-of-bounds query index")
  switch (openGLQueryPool.get_query_type()) {
    case rhi::QueryType::OCCLUSION:
      // At this point in time we know that the "GL_ARB_occlusion_query"-extension is supported
      glBeginQueryARB(GL_SAMPLES_PASSED_ARB, static_cast<const RHIOcclusionTimestampQueryPool &>(openGLQueryPool).get_opengl_queries()[queryIndex]);
      break;

    case rhi::QueryType::PIPELINE_STATISTICS:
      // At this point in time we know that the "GL_ARB_pipeline_statistics_query"-extension is supported
      static_cast<const RHIPipelineStatisticsQueryPool &>(openGLQueryPool).begin_query(queryIndex);
      break;

    case rhi::QueryType::TIMESTAMP:
      BE_ASSERT(false, "OpenGL begin query isn't allowed for timestamp queries, use \"Rhi::Command::write_timestamp_query\" instead")
      break;
  }
}

void RHIDevice::end_query(rhi::RHIQueryPool& queryPool, [[maybe_unused]] core::uint32 queryIndex) {
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Query pool type dependent processing
  const RHIQueryPool& openGLQueryPool = static_cast<const RHIQueryPool&>(queryPool);
  BE_ASSERT(queryIndex < openGLQueryPool.get_number_of_queries(), "OpenGL out-of-bounds query index")
  switch (openGLQueryPool.get_query_type()) {
    case rhi::QueryType::OCCLUSION:
      // At this point in time we know that the "GL_ARB_occlusion_query"-extension is supported
      glEndQueryARB(GL_SAMPLES_PASSED_ARB);
      break;

    case rhi::QueryType::PIPELINE_STATISTICS:
      // At this point in time we know that the "GL_ARB_pipeline_statistics_query"-extension is supported
      static_cast<const RHIPipelineStatisticsQueryPool &>(openGLQueryPool).end_query();
      break;

    case rhi::QueryType::TIMESTAMP:
      BE_ASSERT(false, "OpenGL end query isn't allowed for timestamp queries, use \"Rhi::Command::write_timestamp_query\" instead")
      break;
  }
}

void RHIDevice::write_timestamp_query(rhi::RHIQueryPool& queryPool, core::uint32 queryIndex) {
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Query pool type dependent processing
  const RHIQueryPool& openGLQueryPool = static_cast<const RHIQueryPool&>(queryPool);
  BE_ASSERT(queryIndex < openGLQueryPool.get_number_of_queries(), "OpenGL out-of-bounds query index")
  switch (openGLQueryPool.get_query_type()) {
    case rhi::QueryType::OCCLUSION:
      BE_ASSERT(false, "OpenGL write timestamp query isn't allowed for occlusion queries, use \"Rhi::Command::begin_query\" and \"Rhi::Command::end_query\" instead")
      break;

    case rhi::QueryType::PIPELINE_STATISTICS:
      BE_ASSERT(false, "OpenGL write timestamp query isn't allowed for pipeline statistics queries, use \"Rhi::Command::begin_query\" and \"Rhi::Command::end_query\" instead")
      break;

    case rhi::QueryType::TIMESTAMP:
      // At this point in time we know that the "GL_ARB_timer_query"-extension is supported
      glQueryCounter(static_cast<const RHIOcclusionTimestampQueryPool &>(openGLQueryPool).get_opengl_queries()[queryIndex], GL_TIMESTAMP);
      break;
  }
}


//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void RHIDevice::set_debug_marker(const char* name) {
  // "GL_KHR_debug"-extension required
  if (mExtensions->isGL_KHR_debug()) {
    BE_ASSERT(nullptr != name, "OpenGL debug marker names must not be a null pointer")
    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1, GL_DEBUG_SEVERITY_NOTIFICATION, -1, name);
  }
}

void RHIDevice::begin_debug_event(const char* name) {
  // "GL_KHR_debug"-extension required
  if (mExtensions->isGL_KHR_debug()) {
    BE_ASSERT(nullptr != name, "OpenGL debug event names must not be a null pointer")
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, name);
  }
}

void RHIDevice::end_debug_event() {
  // "GL_KHR_debug"-extension required
  if (mExtensions->isGL_KHR_debug()) {
    glPopDebugGroup();
  }
}
#endif


//[-------------------------------------------------------]
//[ Public virtual rhi::RHIDevice methods                      ]
//[-------------------------------------------------------]
void RHIDevice::initialize(rhi::RHIContext& rhiContext) {
  mContext = &rhiContext;
  // Is OpenGL available?
  mOpenGLRuntimeLinking = re_new<OpenGLRuntimeLinking>(*this);
  if (mOpenGLRuntimeLinking->is_opengl_available()) {
    const core::handle nativeWindowHandle = mContext->get_native_window_handle();
    const rhi::TextureFormat::Enum textureFormat = rhi::TextureFormat::Enum::R8G8B8A8;
    const RHIRenderPass renderPass(*this, 1, &textureFormat, rhi::TextureFormat::Enum::UNKNOWN, 1 RHI_RESOURCE_DEBUG_NAME("OpenGL Unknown"));
#ifdef _WIN32
    {
      // TODO(naetherm) Add external OpenGL context support
      mOpenGLContext = re_new<OpenGLContextWindows>(mOpenGLRuntimeLinking, renderPass.getDepthStencilAttachmentTextureFormat(), nativeWindowHandle);
    }
#elif defined LINUX
    mOpenGLContext = re_new<OpenGLContextLinux>(*this, mOpenGLRuntimeLinking,
                                                           renderPass.get_depth_stencil_attachment_texture_format(), nativeWindowHandle,
                                                           mContext->is_using_external_context());
#else
#error "Unsupported platform"
#endif

    // We're using "this" in here, so we are not allowed to write the following within the initializer list
    mExtensions = re_new<Extensions>(*this, *mOpenGLContext);

    // Is the OpenGL context and extensions initialized?
    if (mOpenGLContext->is_initialized() && mExtensions->initialize()) {
#ifdef DEBUG
      // "GL_ARB_debug_output"-extension available?
      if (mExtensions->isGL_ARB_debug_output()) {
        // Synchronous debug output, please
        // -> Makes it easier to find the place causing the issue
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

        // Disable severity notifications, most drivers print many things with this severity
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, false);

        // Set the debug message callback function
        glDebugMessageCallbackARB(&RHIDevice::debug_message_callback, this);
      }
#endif

      // Globally enable seamless cube map texture, e.g. Direct3D 11 has this enabled by default so do the same for OpenGL
      // -> The following is just for the sake of completeness: It's the year 2020 and OpenGL on Mac is officially dead. But if someone would still
      //    want to support it in a productive way, one has to take care of the situation that enabling seamless cube map texture can result on
      //    slow software rendering on Mac. For checking whether this is the case, see "GL_TEXTURE_CUBE_MAP_SEAMLESS on OS X" published at April 26, 2012 on http://distrustsimplicity.net/articles/gl_texture_cube_map_seamless-on-os-x/
      //    "
      //    GLint gpuVertex, gpuFragment;
      //    CGLGetParameter(CGLGetCurrentContext(), kCGLCPGPUVertexProcessing, &gpuVertex);
      //    CGLGetParameter(CGLGetCurrentContext(), kCGLCPGPUFragmentProcessing, &gpuFragment);
      //    "
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

      // Initialize the capabilities
      initialize_capabilities();

      // Create the default sampler state
      mDefaultSamplerState = create_sampler_state(rhi::RHISamplerState::get_default_sampler_state());

      // Create default OpenGL vertex array
      if (mExtensions->isGL_ARB_vertex_array_object()) {
        glGenVertexArrays(1, &mDefaultOpenGLVertexArray);
        glBindVertexArray(mDefaultOpenGLVertexArray);
      }

      // Add references to the default sampler state and set it
      if (nullptr != mDefaultSamplerState) {
        mDefaultSamplerState->add_ref();
        // TODO(naetherm) Set default sampler states
      }
    }
  }
}

const char* RHIDevice::get_name() const {
  return "OpenGL";
}

bool RHIDevice::is_initialized() const {
  // Is the OpenGL context initialized?
  return (nullptr != mOpenGLContext && mOpenGLContext->is_initialized());
}

bool RHIDevice::is_debug_enabled() {
  // OpenGL has nothing that is similar to the Direct3D 9 PIX functions (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)

  // Debug disabled
  return false;
}


//[-------------------------------------------------------]
//[ Shader language                                       ]
//[-------------------------------------------------------]
core::uint32 RHIDevice::get_number_of_shader_languages() const {
  core::uint32 numberOfShaderLanguages = 0;

  // "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
  if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects()) {
    // GLSL supported
    ++numberOfShaderLanguages;
  }

  // Done, return the number of supported shader languages
  return numberOfShaderLanguages;
}

const char* RHIDevice::get_shader_language_name(core::uint32 index) const {
  BE_ASSERT(index < get_number_of_shader_languages(), "OpenGL: Shader language index is out-of-bounds")

  // "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
  if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects()) {
    // GLSL supported
    if (0 == index) {
      return "GLSL";
    }
  }

  // Error!
  return nullptr;
}

rhi::RHIShaderLanguage* RHIDevice::get_shader_language(const char* shaderLanguageName) {
  // "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
  if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects()) {
    // In case "shaderLanguage" is a null pointer, use the default shader language
    if (nullptr != shaderLanguageName) {
      // Optimization: Check for shader language name pointer match, first
      if ("GLSL" == shaderLanguageName || !stricmp(shaderLanguageName, "GLSL")) {
        // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
        if (mExtensions->isGL_ARB_separate_shader_objects()) {
          // If required, create the separate shader language instance right now
          if (nullptr == mShaderLanguage) {
            mShaderLanguage = re_new<RHIShaderLanguageSeparate>(*this);
            mShaderLanguage->add_ref();	// Internal RHI reference
          }

          // Return the shader language instance
          return mShaderLanguage;
        }
        else if (mExtensions->isGL_ARB_shader_objects()) {
          // If required, create the monolithic shader language instance right now
          if (nullptr == mShaderLanguage) {
            mShaderLanguage = re_new<RHIShaderLanguageMonolithic>(*this);
            mShaderLanguage->add_ref();	// Internal RHI reference
          }

          // Return the shader language instance
          return mShaderLanguage;
        }
      }
    } else {
      // Return the shader language instance as default
      return get_shader_language("GLSL");
    }
  }

  // Error!
  return nullptr;
}


//[-------------------------------------------------------]
//[ Resource creation                                     ]
//[-------------------------------------------------------]
rhi::RHIRenderPass* RHIDevice::create_render_pass(core::uint32 numberOfColorAttachments, const rhi::TextureFormat::Enum* colorAttachmentTextureFormats, rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat, core::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  return re_new<RHIRenderPass>(*this, numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIQueryPool* RHIDevice::create_query_pool(rhi::QueryType queryType, core::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  BE_ASSERT(numberOfQueries > 0, "OpenGL: Number of queries mustn't be zero")
  switch (queryType) {
    case rhi::QueryType::OCCLUSION:
      if (!mExtensions->isGL_ARB_occlusion_query()) {
        BE_LOG(Critical, "OpenGL extension \"GL_ARB_occlusion_query\" isn't supported")
        return nullptr;
      }
      return re_new<RHIOcclusionTimestampQueryPool>(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);

    case rhi::QueryType::PIPELINE_STATISTICS:
      if (!mExtensions->isGL_ARB_pipeline_statistics_query()) {
        BE_LOG(Critical, "OpenGL extension \"GL_ARB_pipeline_statistics_query\" isn't supported")
        return nullptr;
      }
      return re_new<RHIPipelineStatisticsQueryPool>(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);

    case rhi::QueryType::TIMESTAMP:
      if (!mExtensions->isGL_ARB_timer_query()) {
        BE_LOG(Critical, "OpenGL extension \"GL_ARB_timer_query\" isn't supported")
        return nullptr;
      }
      return re_new<RHIOcclusionTimestampQueryPool>(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  return nullptr;
}

rhi::RHISwapChain* RHIDevice::create_swap_chain(rhi::RHIRenderPass& renderPass, core::FrontendPlatformData frontendPlatformData, bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, renderPass)
  //BE_ASSERT(NULL_HANDLE != windowHandle.nativeWindowHandle || nullptr != windowHandle.renderWindow, "OpenGL: The provided native window handle or render window must not be a null handle / null pointer")

  // Create the swap chain
  return re_new<RHISwapChain>(renderPass, frontendPlatformData, useExternalContext RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIFramebuffer* RHIDevice::create_framebuffer(rhi::RHIRenderPass& renderPass, const rhi::FramebufferAttachment* colorFramebufferAttachments, const rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Sanity check
  RHI_MATCH_CHECK(*this, renderPass)

  // "GL_ARB_framebuffer_object" required
  if (mExtensions->isGL_ARB_framebuffer_object()) {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
      // Effective direct state access (DSA)
      // -> Validation is done inside the framebuffer implementation
      return re_new<RHIFramebufferDsa>(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    } else {
      // Traditional bind version
      // -> Validation is done inside the framebuffer implementation
      return re_new<RHIFramebufferBind>(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  } else {
    // Error!
    return nullptr;
  }
}

rhi::RHIBufferManager* RHIDevice::create_buffer_manager() {
  return re_new<RHIBufferManager>(*this);
}

rhi::RHITextureManager* RHIDevice::create_texture_manager() {
  return re_new<TextureManager>(*this);
}

rhi::RHIRootSignature* RHIDevice::create_root_signature(const rhi::RootSignatureDescriptor& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  return re_new<RHIRootSignature>(*this, rootSignature RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

rhi::RHIGraphicsPipelineState* RHIDevice::create_graphics_pipeline_state(const rhi::GraphicsPipelineStateDescriptor& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Sanity checks
  BE_ASSERT(nullptr != graphicsPipelineState.rootSignature, "OpenGL: Invalid graphics pipeline state root signature")
  BE_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "OpenGL: Invalid graphics pipeline state graphics program")
  BE_ASSERT(nullptr != graphicsPipelineState.renderPass, "OpenGL: Invalid graphics pipeline state render pass")

  // Create graphics pipeline state
  core::uint16 id = 0;
  if (GraphicsPipelineStateMakeId.create_id(id)) {
    return re_new<RHIGraphicsPipelineState>(*this, graphicsPipelineState, id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
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

rhi::RHIComputePipelineState* RHIDevice::create_compute_pipeline_state(rhi::RHIRootSignature& rootSignature, rhi::RHIComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, rootSignature)
  RHI_MATCH_CHECK(*this, computeShader)

  // Create the compute pipeline state
  core::uint16 id = 0;
  if ((mExtensions->isGL_ARB_separate_shader_objects() || mExtensions->isGL_ARB_shader_objects()) && ComputePipelineStateMakeId.create_id(id)) {
    // Create the compute pipeline state
    // -> Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects()) {
      return re_new<RHIComputePipelineStateSeparate>(*this, rootSignature, static_cast<RHIComputeShaderSeparate&>(computeShader), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else if (mExtensions->isGL_ARB_shader_objects()) {
      return re_new<RHIComputePipelineStateMonolithic>(*this, rootSignature, static_cast<RHIComputeShaderMonolithic&>(computeShader), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }

  // Error: Ensure a correct reference counter behaviour
  rootSignature.add_ref();
  rootSignature.release();
  computeShader.add_ref();
  computeShader.release();
  return nullptr;
}

rhi::RHISamplerState* RHIDevice::create_sampler_state(const rhi::SamplerStateDescriptor& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  // Is "GL_ARB_sampler_objects" there?
  if (mExtensions->isGL_ARB_sampler_objects()) {
    // Effective sampler object (SO)
    return re_new<RHISamplerStateSo>(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Is "GL_EXT_direct_state_access" there?
  else if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
    // Direct state access (DSA) version to emulate a sampler object
    return re_new<RHISamplerStateDsa>(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  } else {
    // Traditional bind version to emulate a sampler object
    return re_new<RHISamplerStateBind>(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}


//[-------------------------------------------------------]
//[ Resource handling                                     ]
//[-------------------------------------------------------]
bool RHIDevice::map(rhi::RHIResource& resource, core::uint32, rhi::MapType mapType, core::uint32, rhi::MappedSubresource& mappedSubresource) {
  // Evaluate the resource type
  switch (resource.get_resource_type()) {
    case rhi::ResourceType::VERTEX_BUFFER:
      return ::detail::mapBuffer(*mExtensions, GL_ARRAY_BUFFER_ARB, GL_ARRAY_BUFFER_BINDING_ARB,
                                 static_cast<RHIVertexBuffer &>(resource).get_opengl_array_buffer(), mapType, mappedSubresource);

    case rhi::ResourceType::INDEX_BUFFER:
      return ::detail::mapBuffer(*mExtensions, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB,
                                 static_cast<RHIIndexBuffer &>(resource).get_opengl_element_array_buffer(), mapType, mappedSubresource);

    case rhi::ResourceType::TEXTURE_BUFFER:
      return ::detail::mapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB,
                                 static_cast<RHITextureBuffer &>(resource).get_opengl_texture_buffer(), mapType, mappedSubresource);

    case rhi::ResourceType::STRUCTURED_BUFFER:
      return ::detail::mapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB,
                                 static_cast<RHIStructuredBuffer &>(resource).get_opengl_structured_buffer(), mapType, mappedSubresource);

    case rhi::ResourceType::INDIRECT_BUFFER:
      return ::detail::mapBuffer(*mExtensions, GL_DRAW_INDIRECT_BUFFER, GL_DRAW_INDIRECT_BUFFER_BINDING,
                                 static_cast<RHIIndirectBuffer &>(resource).get_opengl_indirect_buffer(), mapType, mappedSubresource);

    case rhi::ResourceType::UNIFORM_BUFFER:
      return ::detail::mapBuffer(*mExtensions, GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING,
                                 static_cast<RHIUniformBuffer &>(resource).get_opengl_uniform_buffer(), mapType, mappedSubresource);

    case rhi::ResourceType::TEXTURE_1D:
    {
      glBindTexture(GL_TEXTURE_1D, static_cast<RHITexture1D &>(resource).get_opengl_texture());
      // TODO(naetherm) Implement me
      return true;
    }

    case rhi::ResourceType::TEXTURE_1D_ARRAY:
    {
      glBindTexture(GL_TEXTURE_1D_ARRAY_EXT, static_cast<RHITexture1DArray &>(resource).get_opengl_texture());
      // TODO(naetherm) Implement me
      return true;
    }

    case rhi::ResourceType::TEXTURE_2D:
    {
      bool result = true;

      glBindTexture(GL_TEXTURE_2D, static_cast<RHITexture2D &>(resource).get_opengl_texture());

      // TODO(naetherm) Implement me
      /*
      // Begin debug event
      RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

      // get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2D&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Map the Direct3D 11 resource
        result = (S_OK == mD3D11DeviceContext->Map(d3d11Resource, subresource, static_cast<D3D11_MAP>(mapType), mapFlags, reinterpret_cast<D3D11_MAPPED_SUBRESOURCE*>(&mappedSubresource)));

        // release the Direct3D 11 resource instance
        d3d11Resource->release();
      }

      // End debug event
      RHI_END_DEBUG_EVENT(this)
      */

      // Done
      return result;
    }

    case rhi::ResourceType::TEXTURE_2D_ARRAY:
    {
      bool result = true;

      glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, static_cast<RHITexture2DArray &>(resource).get_opengl_texture());

      // TODO(naetherm) Implement me
      /*
      // Begin debug event
      RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

      // get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2DArray&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Map the Direct3D 11 resource
        result = (S_OK == mD3D11DeviceContext->Map(d3d11Resource, subresource, static_cast<D3D11_MAP>(mapType), mapFlags, reinterpret_cast<D3D11_MAPPED_SUBRESOURCE*>(&mappedSubresource)));

        // release the Direct3D 11 resource instance
        d3d11Resource->release();
      }

      // End debug event
      RHI_END_DEBUG_EVENT(this)
      */

      // Done
      return result;
    }

    case rhi::ResourceType::TEXTURE_3D:
      return ::detail::mapBuffer(*mExtensions, GL_PIXEL_UNPACK_BUFFER_ARB, GL_PIXEL_UNPACK_BUFFER_BINDING_ARB,
                                 static_cast<RHITexture3D &>(resource).get_opengl_pixel_unpack_buffer(), mapType, mappedSubresource);

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

void RHIDevice::unmap(rhi::RHIResource& resource, core::uint32) {
  // Evaluate the resource type
  switch (resource.get_resource_type()) {
    case rhi::ResourceType::VERTEX_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_ARRAY_BUFFER_ARB, GL_ARRAY_BUFFER_BINDING_ARB,
                            static_cast<RHIVertexBuffer &>(resource).get_opengl_array_buffer());
      break;

    case rhi::ResourceType::INDEX_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB,
                            static_cast<RHIIndexBuffer &>(resource).get_opengl_element_array_buffer());
      break;

    case rhi::ResourceType::TEXTURE_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB,
                            static_cast<RHITextureBuffer &>(resource).get_opengl_texture_buffer());
      break;

    case rhi::ResourceType::STRUCTURED_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB,
                            static_cast<RHIStructuredBuffer &>(resource).get_opengl_structured_buffer());
      break;

    case rhi::ResourceType::INDIRECT_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_DRAW_INDIRECT_BUFFER, GL_DRAW_INDIRECT_BUFFER_BINDING,
                            static_cast<RHIIndirectBuffer &>(resource).get_opengl_indirect_buffer());
      break;

    case rhi::ResourceType::UNIFORM_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING,
                            static_cast<RHIUniformBuffer &>(resource).get_opengl_uniform_buffer());
      break;

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
      // get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2D&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Unmap the Direct3D 11 resource
        mD3D11DeviceContext->Unmap(d3d11Resource, subresource);

        // release the Direct3D 11 resource instance
        d3d11Resource->release();
      }
      */
      break;
    }

    case rhi::ResourceType::TEXTURE_2D_ARRAY:
    {
      // TODO(naetherm) Implement me
      /*
      // get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2DArray&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Unmap the Direct3D 11 resource
        mD3D11DeviceContext->Unmap(d3d11Resource, subresource);

        // release the Direct3D 11 resource instance
        d3d11Resource->release();
      }
      */
      break;
    }

    case rhi::ResourceType::TEXTURE_3D:
    {
      // Unmap pixel unpack buffer
      const RHITexture3D& texture3D = static_cast<RHITexture3D&>(resource);
      const rhi::TextureFormat::Enum textureFormat = texture3D.get_texture_format();
      const core::uint32 openGLPixelUnpackBuffer = texture3D.get_opengl_pixel_unpack_buffer();
      ::detail::unmapBuffer(*mExtensions, GL_PIXEL_UNPACK_BUFFER_ARB, GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, openGLPixelUnpackBuffer);

      // Backup the currently set alignment and currently bound OpenGL pixel unpack buffer
#ifdef RHI_OPENGL_STATE_CLEANUP
      GLint openGLAlignmentBackup = 0;
      glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
      GLint openGLUnpackBufferBackup = 0;
      glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, &openGLUnpackBufferBackup);
#endif
      glPixelStorei(GL_UNPACK_ALIGNMENT, (rhi::TextureFormat::getNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

      // Copy pixel unpack buffer to texture
      glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, openGLPixelUnpackBuffer);
      if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
        // Effective direct state access (DSA)
        if (mExtensions->isGL_ARB_direct_state_access()) {
          glTextureSubImage3D(texture3D.get_opengl_texture(), 0, 0, 0, 0, static_cast<GLsizei>(texture3D.get_width()), static_cast<GLsizei>(texture3D.get_height()), static_cast<GLsizei>(texture3D.getDepth()),
                              Mapping::get_opengl_format(textureFormat), Mapping::get_opengl_type(textureFormat), 0);
        } else {
          glTextureSubImage3DEXT(texture3D.get_opengl_texture(), GL_TEXTURE_3D, 0, 0, 0, 0, static_cast<GLsizei>(texture3D.get_width()), static_cast<GLsizei>(texture3D.get_height()), static_cast<GLsizei>(texture3D.getDepth()),
                                 Mapping::get_opengl_format(textureFormat), Mapping::get_opengl_type(textureFormat), 0);
        }
      } else {
        // Traditional bind version

        // Backup the currently bound OpenGL texture
#ifdef RHI_OPENGL_STATE_CLEANUP
        GLint openGLTextureBackup = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_3D, &openGLTextureBackup);
#endif

        // Copy pixel unpack buffer to texture
        glBindTexture(GL_TEXTURE_3D, texture3D.get_opengl_texture());
        glTexSubImage3DEXT(GL_TEXTURE_3D, 0, 0, 0, 0, static_cast<GLsizei>(texture3D.get_width()), static_cast<GLsizei>(texture3D.get_height()), static_cast<GLsizei>(texture3D.getDepth()),
                           Mapping::get_opengl_format(textureFormat), Mapping::get_opengl_type(textureFormat), 0);

        // Be polite and restore the previous bound OpenGL texture
#ifdef RHI_OPENGL_STATE_CLEANUP
        glBindTexture(GL_TEXTURE_3D, static_cast<GLuint>(openGLTextureBackup));
#endif
      }

      // Restore previous alignment and pixel unpack buffer
#ifdef RHI_OPENGL_STATE_CLEANUP
      glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
      glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, static_cast<GLuint>(openGLUnpackBufferBackup));
#else
      glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
#endif
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

bool RHIDevice::get_query_pool_results(rhi::RHIQueryPool& queryPool, [[maybe_unused]] core::uint32 numberOfDataBytes, core::uint8* data, core::uint32 firstQueryIndex, core::uint32 numberOfQueries, core::uint32 strideInBytes, core::uint32 queryResultFlags) {
  // Sanity checks
  RHI_MATCH_CHECK(*this, queryPool)
  BE_ASSERT(numberOfDataBytes >= sizeof(core::uint64), "OpenGL out-of-memory query access")
  BE_ASSERT(1 == numberOfQueries || strideInBytes > 0, "OpenGL invalid stride in bytes")
  BE_ASSERT(numberOfDataBytes >= strideInBytes * numberOfQueries, "OpenGL out-of-memory query access")
  BE_ASSERT(nullptr != data, "OpenGL out-of-memory query access")
  BE_ASSERT(numberOfQueries > 0, "OpenGL number of queries mustn't be zero")

  // Query pool type dependent processing
  bool resultAvailable = true;
  const RHIQueryPool& openGLQueryPool = static_cast<const RHIQueryPool&>(queryPool);
  BE_ASSERT(firstQueryIndex < openGLQueryPool.get_number_of_queries(), "OpenGL out-of-bounds query index")
  BE_ASSERT((firstQueryIndex + numberOfQueries) <= openGLQueryPool.get_number_of_queries(), "OpenGL out-of-bounds query index")
  const bool waitForResult = ((queryResultFlags & rhi::QueryResultFlags::WAIT) != 0);
  switch (openGLQueryPool.get_query_type()) {
    case rhi::QueryType::OCCLUSION:
    case rhi::QueryType::TIMESTAMP:	// OpenGL return the time in nanoseconds
    {
      core::uint8* currentData = data;
      const GLuint* openGLQueries = static_cast<const RHIOcclusionTimestampQueryPool &>(openGLQueryPool).get_opengl_queries();
      for (core::uint32 i = 0; i  < numberOfQueries; ++i) {
        const GLuint openGLQuery = openGLQueries[firstQueryIndex + i];
        GLuint openGLQueryResult = GL_FALSE;
        do {
          glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_AVAILABLE_ARB, &openGLQueryResult);
        }
        while (waitForResult && GL_TRUE != openGLQueryResult);
        if (GL_TRUE == openGLQueryResult) {
          glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_ARB, &openGLQueryResult);
          *reinterpret_cast<core::uint64*>(currentData) = openGLQueryResult;
        } else {
          // Result not ready
          resultAvailable = false;
          break;
        }
        currentData += strideInBytes;
      }
      break;
    }

    case rhi::QueryType::PIPELINE_STATISTICS:
      BE_ASSERT(numberOfDataBytes >= sizeof(rhi::PipelineStatisticsQueryResult), "OpenGL out-of-memory query access")
      BE_ASSERT(1 == numberOfQueries || strideInBytes >= sizeof(rhi::PipelineStatisticsQueryResult), "OpenGL out-of-memory query access")
      resultAvailable = static_cast<const RHIPipelineStatisticsQueryPool &>(openGLQueryPool).get_query_pool_results(data,
                                                                                                                    firstQueryIndex,
                                                                                                                    numberOfQueries,
                                                                                                                    strideInBytes,
                                                                                                                    waitForResult);
      break;
  }

  // Done
  return resultAvailable;
}


//[-------------------------------------------------------]
//[ Operation                                             ]
//[-------------------------------------------------------]
void RHIDevice::dispatch_command_buffer(const rhi::RHICommandBuffer& commandBuffer) {
  // Sanity check
  BE_ASSERT(!commandBuffer.is_empty(), "The OpenGL command buffer to dispatch mustn't be empty")

  // Dispatch command buffer
  dispatch_command_buffer_internal(commandBuffer);
}


//[-------------------------------------------------------]
//[ Private static methods                                ]
//[-------------------------------------------------------]
#ifdef DEBUG
void RHIDevice::debug_message_callback(core::uint32 source, core::uint32 type, core::uint32 id, core::uint32 severity, int, const char* message, const void* userParam) {
  // Source to string
  char debugSource[20 + 1]{0};	// +1 for terminating zero
  switch (source)
  {
    case GL_DEBUG_SOURCE_API_ARB:
      strncpy(debugSource, "OpenGL", 20);
      break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
      strncpy(debugSource, "Windows", 20);
      break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
      strncpy(debugSource, "Shader compiler", 20);
      break;

    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
      strncpy(debugSource, "Third party", 20);
      break;

    case GL_DEBUG_SOURCE_APPLICATION_ARB:
      strncpy(debugSource, "Application", 20);
      break;

    case GL_DEBUG_SOURCE_OTHER_ARB:
      strncpy(debugSource, "Other", 20);
      break;

    default:
      strncpy(debugSource, "?", 20);
      break;
  }

  // Debug type to string
  core::Log::LogLevel logType = core::Log::LogLevel::Critical;
  char debugType[25 + 1]{0};	// +1 for terminating zero
  switch (type)
  {
    case GL_DEBUG_TYPE_ERROR_ARB:
      strncpy(debugType, "Error", 25);
      break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
      logType = core::Log::LogLevel::Warning;
      strncpy(debugType, "Deprecated behavior", 25);
      break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
      strncpy(debugType, "Undefined behavior", 25);
      break;

    case GL_DEBUG_TYPE_PORTABILITY_ARB:
      logType = core::Log::LogLevel::Warning;
      strncpy(debugType, "Portability", 25);
      break;

    case GL_DEBUG_TYPE_PERFORMANCE_ARB:
      logType = core::Log::LogLevel::Warning;
      strncpy(debugType, "Performance", 25);
      break;

    case GL_DEBUG_TYPE_OTHER_ARB:
      strncpy(debugType, "Other", 25);
      break;

    case GL_DEBUG_TYPE_MARKER:
      strncpy(debugType, "Marker", 25);
      break;

    case GL_DEBUG_TYPE_PUSH_GROUP:
      strncpy(debugType, "Push group", 25);
      break;

    case GL_DEBUG_TYPE_POP_GROUP:
      strncpy(debugType, "Pop group", 25);
      break;

    default:
      strncpy(debugType, "?", 25);
      break;
  }

  // Debug severity to string
  char debugSeverity[20 + 1]{0};	// +1 for terminating zero
  switch (severity)
  {
    case GL_DEBUG_SEVERITY_HIGH_ARB:
      strncpy(debugSeverity, "High", 20);
      break;

    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
      strncpy(debugSeverity, "Medium", 20);
      break;

    case GL_DEBUG_SEVERITY_LOW_ARB:
      strncpy(debugSeverity, "Low", 20);
      break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
      strncpy(debugSeverity, "Notification", 20);
      break;

    default:
      strncpy(debugSeverity, "?", 20);
      break;
  }

  // Print into log
  BE_LOG(Critical, core::String(message))
  //if (static_cast<const OpenGLRhi*>(userParam)->get_context().getLog().print(logType, nullptr, __FILE__, static_cast<core::uint32>(__LINE__), "OpenGL debug message\tSource:\"%s\"\tType:\"%s\"\tID:\"%u\"\tSeverity:\"%s\"\tMessage:\"%s\"", debugSource, debugType, id, debugSeverity, message))
  {
  //	DEBUG_BREAK;
  }
}
#else
void RHIDevice::debug_message_callback(core::uint32 source, core::uint32 type, core::uint32 id, core::uint32 severity, int length, const char *message, const void *userParam) {
  // Nothing here
}
#endif


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void RHIDevice::initialize_capabilities() {
  GLint openGLValue = 0;

  { // get device name
    // -> OpenGL 4.3 Compatibility Profile Specification, section 22.2, page 627: "String queries return pointers to UTF-8 encoded, null-terminated static strings describing properties of the current GL context."
    // -> For example "AMD Radeon R9 200 Series"
    const size_t numberOfCharacters = ::detail::countof(mCapabilities.deviceName) - 1;
    strncpy(mCapabilities.deviceName, reinterpret_cast<const char*>(glGetString(GL_RENDERER)), numberOfCharacters);
    mCapabilities.deviceName[numberOfCharacters] = '\0';
  }

  // Preferred swap chain texture format
  mCapabilities.preferredSwapChainColorTextureFormat		  = rhi::TextureFormat::Enum::R8G8B8A8;
  mCapabilities.preferredSwapChainDepthStencilTextureFormat = rhi::TextureFormat::Enum::D32_FLOAT;

  // Maximum number of viewports (always at least 1)
  // TODO(naetherm) "GL_ARB_viewport_array" support ("RHIDevice::set_graphics_viewports()")
  mCapabilities.maximumNumberOfViewports = 1;	// TODO(naetherm) GL_ARB_viewport_array

  // Maximum number of simultaneous render targets (if <1 render to texture is not supported, "GL_ARB_draw_buffers" required)
  if (mExtensions->isGL_ARB_draw_buffers()) {
    glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &openGLValue);
    mCapabilities.maximumNumberOfSimultaneousRenderTargets = static_cast<core::uint32>(openGLValue);
  } else {
    // "GL_ARB_framebuffer_object"-extension for render to texture required
    mCapabilities.maximumNumberOfSimultaneousRenderTargets = static_cast<core::uint32>(mExtensions->isGL_ARB_framebuffer_object());
  }

  // Maximum texture dimension
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &openGLValue);
  mCapabilities.maximumTextureDimension = static_cast<core::uint32>(openGLValue);

  // Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
  // Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
  // Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
  if (mExtensions->isGL_EXT_texture_array()) {
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, &openGLValue);
    mCapabilities.maximumNumberOf1DTextureArraySlices = static_cast<core::uint32>(openGLValue);
    mCapabilities.maximumNumberOf2DTextureArraySlices = static_cast<core::uint32>(openGLValue);
    mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;	// TODO(naetherm) Implement me		 static_cast<core::uint32>(openGLValue);
  } else {
    mCapabilities.maximumNumberOf1DTextureArraySlices = 0;
    mCapabilities.maximumNumberOf2DTextureArraySlices = 0;
    mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;
  }

  // Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
  if (mExtensions->isGL_ARB_texture_buffer_object()) {
    glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE_EXT, &openGLValue);
    mCapabilities.maximumTextureBufferSize = static_cast<core::uint32>(openGLValue);
  } else {
    mCapabilities.maximumTextureBufferSize = 0;
  }

  // Maximum structured buffer size in bytes
  if (mExtensions->isGL_ARB_shader_storage_buffer_object()) {
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &openGLValue);
    mCapabilities.maximumStructuredBufferSize = static_cast<core::uint32>(openGLValue);
  } else {
    mCapabilities.maximumStructuredBufferSize = 0;
  }

  // Maximum indirect buffer size in bytes
  if (mExtensions->isGL_ARB_draw_indirect()) {
    // TODO(naetherm) How to get the indirect buffer maximum size? Didn't find any information about this.
    mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB
  } else {
    mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB
  }

  // Maximum uniform buffer (UBO) size in bytes (usually at least 4096 * 16 bytes, in case there's no support for uniform buffer it's 0)
  if (mExtensions->isGL_ARB_uniform_buffer_object()) {
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &openGLValue);
    mCapabilities.maximumUniformBufferSize = static_cast<core::uint32>(openGLValue);
  } else {
    mCapabilities.maximumUniformBufferSize = 0;
  }

  // Maximum number of multisamples (always at least 1, usually 8)
  if (mExtensions->isGL_ARB_texture_multisample()) {
    glGetIntegerv(GL_MAX_SAMPLES, &openGLValue);
    if (openGLValue > 8) {
      // Limit to known maximum we can test, even if e.g. GeForce 980m reports 32 here
      openGLValue = 8;
    }
    mCapabilities.maximumNumberOfMultisamples = static_cast<core::uint8>(openGLValue);
  } else {
    mCapabilities.maximumNumberOfMultisamples = 1;
  }

  // Maximum anisotropy (always at least 1, usually 16)
  // -> "GL_EXT_texture_filter_anisotropic"-extension
  glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &openGLValue);
  mCapabilities.maximumAnisotropy = static_cast<core::uint8>(openGLValue);

  // Coordinate system
  // -> If the "GL_ARB_clip_control"-extension is available: Left-handed coordinate system with clip space depth value range 0..1
  // -> If the "GL_ARB_clip_control"-extension isn't available: Right-handed coordinate system with clip space depth value range -1..1
  // -> For background theory see "Depth Precision Visualized" by Nathan Reed - https://developer.nvidia.com/content/depth-precision-visualized
  // -> For practical information see "Reversed-Z in OpenGL" by Nicolas Guillemot - https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
  // -> Shaders might want to take the following into account: "Mac computers that use OpenCL and OpenGL graphics" - https://support.apple.com/en-us/HT202823 - "iMac (Retina 5K, 27-inch, 2017)" - OpenGL 4.1
  mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = mExtensions->isGL_ARB_clip_control();

  // Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
  mCapabilities.individualUniforms = true;

  // Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex, "GL_ARB_instanced_arrays" required)
  mCapabilities.instancedArrays = mExtensions->isGL_ARB_instanced_arrays();

  // Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID, "GL_ARB_draw_instanced" required)
  mCapabilities.drawInstanced = mExtensions->isGL_ARB_draw_instanced();

  // Base vertex supported for draw calls?
  mCapabilities.baseVertex = mExtensions->isGL_ARB_draw_elements_base_vertex();

  // OpenGL has no native multithreading
  mCapabilities.nativeMultithreading = false;

  // We don't support the OpenGL program binaries since those are operation system and graphics driver version dependent, which renders them useless for pre-compiled shaders shipping
  mCapabilities.shaderBytecode = mExtensions->isGL_ARB_gl_spirv();

  // Is there support for vertex shaders (VS)?
  mCapabilities.vertexShader = mExtensions->isGL_ARB_vertex_shader();

  // Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
  if (mExtensions->isGL_ARB_tessellation_shader()) {
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &openGLValue);
    mCapabilities.maximumNumberOfPatchVertices = static_cast<core::uint32>(openGLValue);
  } else {
    mCapabilities.maximumNumberOfPatchVertices = 0;
  }

  // Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
  if (mExtensions->isGL_ARB_geometry_shader4()) {
    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB, &openGLValue);
    mCapabilities.maximumNumberOfGsOutputVertices = static_cast<core::uint32>(openGLValue);
  } else {
    mCapabilities.maximumNumberOfGsOutputVertices = 0;
  }

  // Is there support for fragment shaders (FS)?
  mCapabilities.fragmentShader = mExtensions->isGL_ARB_fragment_shader();

  // Is there support for task shaders (TS) and mesh shaders (MS)?
  mCapabilities.meshShader = mExtensions->isGL_NV_mesh_shader();

  // Is there support for compute shaders (CS)?
  mCapabilities.computeShader = mExtensions->isGL_ARB_compute_shader();
}

void RHIDevice::unset_graphics_vertex_array() {
  // release the currently used vertex array reference, in case we have one
  if (nullptr != mVertexArray) {
    // Evaluate the internal array type of the currently set vertex array
    switch (static_cast<RHIVertexArray *>(mVertexArray)->get_internal_resource_type()) {
      case RHIVertexArray::InternalResourceType::NO_VAO:
        // Disable OpenGL vertex attribute arrays
        static_cast<RHIVertexArrayNoVao *>(mVertexArray)->disable_opengl_vertex_attrib_arrays();
        break;

      case RHIVertexArray::InternalResourceType::VAO:
        // Unbind OpenGL vertex array
        // -> No need to check for "GL_ARB_vertex_array_object", in case were in here we know it must exist
        glBindVertexArray(mDefaultOpenGLVertexArray);
        break;
    }

    // release reference
    mVertexArray->release();
    mVertexArray = nullptr;
  }
}

void RHIDevice::set_resource_group(const RHIRootSignature& rootSignature, core::uint32 rootParameterIndex, rhi::RHIResourceGroup* resourceGroup) {
  if (nullptr != resourceGroup) {
    // Sanity check
    RHI_MATCH_CHECK(*this, *resourceGroup)

    // Set resource group
    const RHIResourceGroup* openGLResourceGroup = static_cast<RHIResourceGroup*>(resourceGroup);
    const core::uint32 numberOfResources = openGLResourceGroup->get_number_of_resources();
    rhi::RHIResource** resources = openGLResourceGroup->get_resources();
    const rhi::RootParameter& rootParameter = rootSignature.get_root_signature().parameters[rootParameterIndex];
    for (core::uint32 resourceIndex = 0; resourceIndex < numberOfResources; ++resourceIndex, ++resources) {
      rhi::RHIResource* resource = *resources;
      BE_ASSERT(nullptr != reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
      const rhi::DescriptorRange& descriptorRange = reinterpret_cast<const rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[resourceIndex];

      // Check the type of resource to set
      // TODO(naetherm) Some additional resource type root signature security checks in debug build?
      const rhi::ResourceType resourceType = resource->get_resource_type();
      switch (resourceType) {
        case rhi::ResourceType::TEXTURE_BUFFER:
        case rhi::ResourceType::TEXTURE_1D:
        case rhi::ResourceType::TEXTURE_1D_ARRAY:
        case rhi::ResourceType::TEXTURE_2D:
        case rhi::ResourceType::TEXTURE_2D_ARRAY:
        case rhi::ResourceType::TEXTURE_3D:
        case rhi::ResourceType::TEXTURE_CUBE:
        case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
        {
          // In OpenGL, all shaders share the same texture units (= "rhi::RootParameter::shaderVisibility" stays unused)
          switch (descriptorRange.rangeType) {
            case rhi::DescriptorRangeType::SRV:
            {
              // Is "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" there?
              if (mExtensions->isGL_ARB_direct_state_access() || mExtensions->isGL_EXT_direct_state_access()) {
                // Effective direct state access (DSA)
                const bool isArbDsa = mExtensions->isGL_ARB_direct_state_access();

                // "glBindTextureUnit()" unit parameter is zero based so we can simply use the value we received
                const GLuint unit = descriptorRange.baseShaderRegister;

                // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
                // Evaluate the texture type
                switch (resourceType) {
                  case rhi::ResourceType::TEXTURE_BUFFER:
                    if (isArbDsa) {
                      glBindTextureUnit(unit, static_cast<RHITextureBuffer *>(resource)->get_opengl_texture());
                    } else {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_BUFFER_ARB,
                                            static_cast<RHITextureBuffer *>(resource)->get_opengl_texture());
                    }
                    break;

                  case rhi::ResourceType::TEXTURE_1D:
                    if (isArbDsa) {
                      glBindTextureUnit(unit, static_cast<RHITexture1D *>(resource)->get_opengl_texture());
                    } else {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_1D,
                                            static_cast<RHITexture1D *>(resource)->get_opengl_texture());
                    }
                    break;

                  case rhi::ResourceType::TEXTURE_1D_ARRAY:
                    // No texture 1D array extension check required, if we in here we already know it must exist
                    if (isArbDsa) {
                      glBindTextureUnit(unit, static_cast<RHITexture1DArray *>(resource)->get_opengl_texture());
                    } else {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_1D_ARRAY_EXT,
                                            static_cast<RHITexture1DArray *>(resource)->get_opengl_texture());
                    }
                    break;

                  case rhi::ResourceType::TEXTURE_2D:
                    if (isArbDsa) {
                      glBindTextureUnit(unit, static_cast<RHITexture2D *>(resource)->get_opengl_texture());
                    } else {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      const RHITexture2D* texture2D = static_cast<RHITexture2D*>(resource);
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, static_cast<GLenum>((texture2D->get_number_of_multisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
                                            texture2D->get_opengl_texture());
                    }
                    break;

                  case rhi::ResourceType::TEXTURE_2D_ARRAY:
                    // No texture 2D array extension check required, if we in here we already know it must exist
                    if (isArbDsa) {
                      glBindTextureUnit(unit, static_cast<RHITexture2DArray *>(resource)->get_opengl_texture());
                    } else {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_2D_ARRAY_EXT,
                                            static_cast<RHITexture2DArray *>(resource)->get_opengl_texture());
                    }
                    break;

                  case rhi::ResourceType::TEXTURE_3D:
                    if (isArbDsa) {
                      glBindTextureUnit(unit, static_cast<RHITexture3D *>(resource)->get_opengl_texture());
                    } else {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_3D,
                                            static_cast<RHITexture3D *>(resource)->get_opengl_texture());
                    }
                    break;

                  case rhi::ResourceType::TEXTURE_CUBE:
                    if (isArbDsa) {
                      glBindTextureUnit(unit, static_cast<RHITextureCube *>(resource)->get_opengl_texture());
                    } else {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_CUBE_MAP,
                                            static_cast<RHITextureCube *>(resource)->get_opengl_texture());
                    }
                    break;

                  case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
                    // TODO(naetherm) Implement me
                    /*
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<TextureCubeArray*>(resource)->get_opengl_texture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_CUBE_MAP, static_cast<TextureCubeArray*>(resource)->get_opengl_texture());
                    }
                    */
                    break;

                  case rhi::ResourceType::ROOT_SIGNATURE:
                  case rhi::ResourceType::RESOURCE_GROUP:
                  case rhi::ResourceType::GRAPHICS_PROGRAM:
                  case rhi::ResourceType::VERTEX_ARRAY:
                  case rhi::ResourceType::RENDER_PASS:
                  case rhi::ResourceType::QUERY_POOL:
                  case rhi::ResourceType::SWAP_CHAIN:
                  case rhi::ResourceType::FRAMEBUFFER:
                  case rhi::ResourceType::VERTEX_BUFFER:
                  case rhi::ResourceType::INDEX_BUFFER:
                  case rhi::ResourceType::STRUCTURED_BUFFER:
                  case rhi::ResourceType::INDIRECT_BUFFER:
                  case rhi::ResourceType::UNIFORM_BUFFER:
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
                    BE_ASSERT(false, "Invalid OpenGL RHI implementation resource type")
                    break;
                }

                // Set the OpenGL sampler states, if required (texture buffer has no sampler state), it's valid that there's no sampler state (e.g. texel fetch instead of sampling might be used)
                if (rhi::ResourceType::TEXTURE_BUFFER != resourceType && nullptr != openGLResourceGroup->get_sampler_state()) {
                  const RHISamplerState* samplerState = static_cast<const RHISamplerState*>(openGLResourceGroup->get_sampler_state()[resourceIndex]);
                  if (nullptr != samplerState) {
                    // Is "GL_ARB_sampler_objects" there?
                    if (mExtensions->isGL_ARB_sampler_objects()) {
                      // Effective sampler object (SO)
                      glBindSampler(descriptorRange.baseShaderRegister,
                                    static_cast<const RHISamplerStateSo *>(samplerState)->get_opengl_sampler());
                    } else {
#ifdef RHI_OPENGL_STATE_CLEANUP
                      // Backup the currently active OpenGL texture
                      GLint openGLActiveTextureBackup = 0;
                      glGetIntegerv(GL_ACTIVE_TEXTURE, &openGLActiveTextureBackup);
#endif

                      // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
                      // Activate the texture unit we want to manipulate
                      // -> "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glActiveTextureARB(GL_TEXTURE0_ARB + unit);

                      // Is "GL_EXT_direct_state_access" there?
                      if (mExtensions->isGL_EXT_direct_state_access()) {
                        // Direct state access (DSA) version to emulate a sampler object
                        static_cast<const RHISamplerStateDsa *>(samplerState)->set_opengl_sampler_states();
                      } else {
                        // Traditional bind version to emulate a sampler object
                        static_cast<const RHISamplerStateBind *>(samplerState)->set_opengl_sampler_states();
                      }

#ifdef RHI_OPENGL_STATE_CLEANUP
                      // Be polite and restore the previous active OpenGL texture
                      glActiveTextureARB(static_cast<GLenum>(openGLActiveTextureBackup));
#endif
                    }
                  }
                }
              } else {
                // Traditional bind version

                // "GL_ARB_multitexture" required
                if (mExtensions->isGL_ARB_multitexture()) {
#ifdef RHI_OPENGL_STATE_CLEANUP
                  // Backup the currently active OpenGL texture
                  GLint openGLActiveTextureBackup = 0;
                  glGetIntegerv(GL_ACTIVE_TEXTURE, &openGLActiveTextureBackup);
#endif

                  // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
                  // Activate the texture unit we want to manipulate
                  // -> "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                  glActiveTextureARB(GL_TEXTURE0_ARB + descriptorRange.baseShaderRegister);

                  // Evaluate the resource type
                  switch (resourceType) {
                    case rhi::ResourceType::TEXTURE_BUFFER:
                      glBindTexture(GL_TEXTURE_BUFFER_ARB, static_cast<RHITextureBuffer *>(resource)->get_opengl_texture());
                      break;

                    case rhi::ResourceType::TEXTURE_1D:
                      glBindTexture(GL_TEXTURE_1D, static_cast<RHITexture1D *>(resource)->get_opengl_texture());
                      break;

                    case rhi::ResourceType::TEXTURE_1D_ARRAY:
                      // No extension check required, if we in here we already know it must exist
                      glBindTexture(GL_TEXTURE_1D_ARRAY_EXT,
                                    static_cast<RHITexture1DArray *>(resource)->get_opengl_texture());
                      break;

                    case rhi::ResourceType::TEXTURE_2D:
                    {
                      const RHITexture2D* texture2D = static_cast<RHITexture2D*>(resource);
                      glBindTexture(static_cast<GLenum>((texture2D->get_number_of_multisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
                                    texture2D->get_opengl_texture());
                      break;
                    }

                    case rhi::ResourceType::TEXTURE_2D_ARRAY:
                      // No extension check required, if we in here we already know it must exist
                      glBindTexture(GL_TEXTURE_2D_ARRAY_EXT,
                                    static_cast<RHITexture2DArray *>(resource)->get_opengl_texture());
                      break;

                    case rhi::ResourceType::TEXTURE_3D:
                      glBindTexture(GL_TEXTURE_3D, static_cast<RHITexture3D *>(resource)->get_opengl_texture());
                      break;

                    case rhi::ResourceType::TEXTURE_CUBE:
                      glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<RHITextureCube *>(resource)->get_opengl_texture());
                      break;

                    case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
                      // TODO(naetherm) Implement me
                      // glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<TextureCubeArray*>(resource)->get_opengl_texture());
                      break;

                    case rhi::ResourceType::ROOT_SIGNATURE:
                    case rhi::ResourceType::RESOURCE_GROUP:
                    case rhi::ResourceType::GRAPHICS_PROGRAM:
                    case rhi::ResourceType::VERTEX_ARRAY:
                    case rhi::ResourceType::RENDER_PASS:
                    case rhi::ResourceType::QUERY_POOL:
                    case rhi::ResourceType::SWAP_CHAIN:
                    case rhi::ResourceType::FRAMEBUFFER:
                    case rhi::ResourceType::VERTEX_BUFFER:
                    case rhi::ResourceType::INDEX_BUFFER:
                    case rhi::ResourceType::STRUCTURED_BUFFER:
                    case rhi::ResourceType::INDIRECT_BUFFER:
                    case rhi::ResourceType::UNIFORM_BUFFER:
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
                      BE_ASSERT(false, "Invalid OpenGL RHI implementation resource type")
                      break;
                  }

                  // Set the OpenGL sampler states, if required (texture buffer has no sampler state), it's valid that there's no sampler state (e.g. texel fetch instead of sampling might be used)
                  if (rhi::ResourceType::TEXTURE_BUFFER != resourceType) {
                    BE_ASSERT(nullptr != openGLResourceGroup->get_sampler_state(), "Invalid OpenGL sampler state")
                    const RHISamplerState* samplerState = static_cast<const RHISamplerState*>(openGLResourceGroup->get_sampler_state()[resourceIndex]);
                    if (nullptr != samplerState) {
                      // Is "GL_ARB_sampler_objects" there?
                      if (mExtensions->isGL_ARB_sampler_objects()) {
                        // Effective sampler object (SO)
                        glBindSampler(descriptorRange.baseShaderRegister,
                                      static_cast<const RHISamplerStateSo *>(samplerState)->get_opengl_sampler());
                      }
                        // Is "GL_EXT_direct_state_access" there?
                      else if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access()) {
                        // Direct state access (DSA) version to emulate a sampler object
                        static_cast<const RHISamplerStateDsa *>(samplerState)->set_opengl_sampler_states();
                      } else {
                        // Traditional bind version to emulate a sampler object
                        static_cast<const RHISamplerStateBind *>(samplerState)->set_opengl_sampler_states();
                      }
                    }
                  }

#ifdef RHI_OPENGL_STATE_CLEANUP
                  // Be polite and restore the previous active OpenGL texture
                  glActiveTextureARB(static_cast<GLenum>(openGLActiveTextureBackup));
#endif
                }
              }
              break;
            }

            case rhi::DescriptorRangeType::UAV:
            {
              // Is "GL_EXT_shader_image_load_store" there?
              if (mExtensions->isGL_EXT_shader_image_load_store()) {
                // "glBindImageTextureEXT()" unit parameter is zero based so we can simply use the value we received
                const GLuint unit = descriptorRange.baseShaderRegister;

                // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
                // Evaluate the texture type
                switch (resourceType) {
                  case rhi::ResourceType::TEXTURE_BUFFER:
                  {
                    const RHITextureBuffer* textureBuffer = static_cast<RHITextureBuffer*>(resource);
                    glBindImageTextureEXT(unit, textureBuffer->get_opengl_texture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureBuffer->get_opengl_internal_format()));
                    break;
                  }

                  case rhi::ResourceType::TEXTURE_1D:
                  {
                    const RHITexture1D* texture1D = static_cast<RHITexture1D*>(resource);
                    glBindImageTextureEXT(unit, texture1D->get_opengl_texture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture1D->get_opengl_internal_format()));
                    break;
                  }

                  case rhi::ResourceType::TEXTURE_1D_ARRAY:
                  {
                    const RHITexture1DArray* texture1DArray = static_cast<RHITexture1DArray*>(resource);
                    glBindImageTextureEXT(unit, texture1DArray->get_opengl_texture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture1DArray->get_opengl_internal_format()));
                    break;
                  }

                  case rhi::ResourceType::TEXTURE_2D:
                  {
                    const RHITexture2D* texture2D = static_cast<RHITexture2D*>(resource);
                    glBindImageTextureEXT(unit, texture2D->get_opengl_texture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture2D->get_opengl_internal_format()));
                    break;
                  }

                  case rhi::ResourceType::TEXTURE_2D_ARRAY:
                  {
                    const RHITexture2DArray* texture2DArray = static_cast<RHITexture2DArray*>(resource);
                    glBindImageTextureEXT(unit, texture2DArray->get_opengl_texture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture2DArray->get_opengl_internal_format()));
                    break;
                  }

                  case rhi::ResourceType::TEXTURE_3D:
                  {
                    const RHITexture3D* texture3D = static_cast<RHITexture3D*>(resource);
                    glBindImageTextureEXT(unit, texture3D->get_opengl_texture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture3D->get_opengl_internal_format()));
                    break;
                  }

                  case rhi::ResourceType::TEXTURE_CUBE:
                  {
                    const RHITextureCube* textureCube = static_cast<RHITextureCube*>(resource);
                    glBindImageTextureEXT(unit, textureCube->get_opengl_texture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureCube->get_opengl_internal_format()));
                    break;
                  }

                  case rhi::ResourceType::TEXTURE_CUBE_ARRAY:
                  {
                    // TODO(naetherm) Implement me
                    // const TextureCubeArray* textureCubeArray = static_cast<TextureCubeArray*>(resource);
                    // glBindImageTextureEXT(unit, textureCubeArray->get_opengl_texture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureCubeArray->get_opengl_internal_format()));
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
                  case rhi::ResourceType::VERTEX_BUFFER:
                  case rhi::ResourceType::INDEX_BUFFER:
                  case rhi::ResourceType::STRUCTURED_BUFFER:
                  case rhi::ResourceType::INDIRECT_BUFFER:
                  case rhi::ResourceType::UNIFORM_BUFFER:
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
                    BE_ASSERT(false, "Invalid OpenGL RHI implementation resource type")
                    break;
                }
              }
              break;
            }

            case rhi::DescriptorRangeType::UBV:
            case rhi::DescriptorRangeType::SAMPLER:
            case rhi::DescriptorRangeType::NUMBER_OF_RANGE_TYPES:
              BE_ASSERT(false, "Invalid OpenGL descriptor range type")
              break;
          }
          break;
        }

        case rhi::ResourceType::VERTEX_BUFFER:
        {
          BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL vertex buffer must bound at SRV or UAV descriptor range type")
          BE_ASSERT(rhi::ShaderVisibility::ALL == descriptorRange.shaderVisibility || rhi::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object()) {
            // "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
            const GLuint index = descriptorRange.baseShaderRegister;

            // Attach the buffer to the given SSBO binding point
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index,
                             static_cast<RHIVertexBuffer *>(resource)->get_opengl_array_buffer());
          }
          break;
        }

        case rhi::ResourceType::INDEX_BUFFER:
        {
          BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL index buffer must bound at SRV or UAV descriptor range type")
          BE_ASSERT(rhi::ShaderVisibility::ALL == descriptorRange.shaderVisibility || rhi::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object()) {
            // "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
            const GLuint index = descriptorRange.baseShaderRegister;

            // Attach the buffer to the given SSBO binding point
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index,
                             static_cast<RHIIndexBuffer *>(resource)->get_opengl_element_array_buffer());
          }
          break;
        }

        case rhi::ResourceType::STRUCTURED_BUFFER:
        {
          BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL structured buffer must bound at SRV or UAV descriptor range type")

          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object()) {
            // "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
            const GLuint index = descriptorRange.baseShaderRegister;

            // Attach the buffer to the given SSBO binding point
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index,
                             static_cast<RHIStructuredBuffer *>(resource)->get_opengl_structured_buffer());
          }
          break;
        }

        case rhi::ResourceType::INDIRECT_BUFFER:
        {
          BE_ASSERT(rhi::DescriptorRangeType::SRV == descriptorRange.rangeType || rhi::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL indirect buffer must bound at SRV or UAV descriptor range type")
          BE_ASSERT(rhi::ShaderVisibility::ALL == descriptorRange.shaderVisibility || rhi::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object()) {
            // "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
            const GLuint index = descriptorRange.baseShaderRegister;

            // Attach the buffer to the given SSBO binding point
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index,
                             static_cast<RHIIndirectBuffer *>(resource)->get_opengl_indirect_buffer());
          }
          break;
        }

        case rhi::ResourceType::UNIFORM_BUFFER:
          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object()) {
            // Attach the buffer to the given UBO binding point
            // -> Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
            // -> Direct3D 10 and Direct3D 11 have explicit binding points
            BE_ASSERT(rhi::DescriptorRangeType::UBV == descriptorRange.rangeType, "OpenGL uniform buffer must bound at UBV descriptor range type")
            BE_ASSERT(nullptr != openGLResourceGroup->get_resource_index_to_uniform_block_binding_index(), "Invalid OpenGL resource index to uniform block binding index")
            glBindBufferBase(GL_UNIFORM_BUFFER, openGLResourceGroup->get_resource_index_to_uniform_block_binding_index()[resourceIndex],
                             static_cast<RHIUniformBuffer *>(resource)->get_opengl_uniform_buffer());
          }
          break;

        case rhi::ResourceType::SAMPLER_STATE:
          // Unlike Direct3D >=10, OpenGL directly attaches the sampler settings to the texture (unless the sampler object extension is used)
          break;

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
        case rhi::ResourceType::VERTEX_SHADER:
        case rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
        case rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
        case rhi::ResourceType::GEOMETRY_SHADER:
        case rhi::ResourceType::FRAGMENT_SHADER:
        case rhi::ResourceType::TASK_SHADER:
        case rhi::ResourceType::MESH_SHADER:
        case rhi::ResourceType::COMPUTE_SHADER:
          BE_ASSERT(false, "Invalid OpenGL RHI implementation resource type")
          break;
      }
    }
  } else {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDevice::set_opengl_graphics_program(rhi::RHIGraphicsProgram* graphicsProgram) {
  if (nullptr != graphicsProgram) {
    // Sanity check
    RHI_MATCH_CHECK(*this, *graphicsProgram)

    // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects()) {
      // Bind the graphics program pipeline, if required
      RHIGraphicsProgramSeparate* graphicsProgramSeparate = static_cast<RHIGraphicsProgramSeparate*>(graphicsProgram);
      const core::uint32 openGLProgramPipeline = graphicsProgramSeparate->get_opengl_program_pipeline();
      if (openGLProgramPipeline != mOpenGLProgramPipeline) {
        mOpenGLProgramPipeline = openGLProgramPipeline;
        { // Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
          const RHIVertexShaderSeparate* vertexShaderSeparate = graphicsProgramSeparate->get_vertex_shader_separate();
          if (nullptr != vertexShaderSeparate) {
            mOpenGLVertexProgram = vertexShaderSeparate->get_opengl_shader_program();
            mDrawIdUniformLocation = vertexShaderSeparate->get_draw_id_uniform_location();
          } else {
            mOpenGLVertexProgram = 0;
            mDrawIdUniformLocation = -1;
          }
          mCurrentStartInstanceLocation = ~0u;
        }
        glBindProgramPipeline(mOpenGLProgramPipeline);
      }
    }
    else if (mExtensions->isGL_ARB_shader_objects()) {
      // Bind the graphics program, if required
      const RHIGraphicsProgramMonolithic* graphicsProgramMonolithic = static_cast<RHIGraphicsProgramMonolithic*>(graphicsProgram);
      const core::uint32 openGLProgram = graphicsProgramMonolithic->getOpenGLProgram();
      if (openGLProgram != mOpenGLProgram) {
        mOpenGLProgram = mOpenGLVertexProgram = openGLProgram;
        mDrawIdUniformLocation = graphicsProgramMonolithic->get_draw_id_uniform_location();
        mCurrentStartInstanceLocation = ~0u;
        glUseProgram(mOpenGLProgram);
      }
    }
  } else {
    // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects()) {
      // Unbind the program pipeline, if required
      if (0 != mOpenGLProgramPipeline) {
        glBindProgramPipeline(0);
        mOpenGLProgramPipeline = 0;
      }
    }
    else if (mExtensions->isGL_ARB_shader_objects()) {
      // Unbind the program, if required
      if (0 != mOpenGLProgram) {
        glUseProgram(0);
        mOpenGLProgram = 0;
      }
    }
    mOpenGLVertexProgram = 0;
    mDrawIdUniformLocation = -1;
    mCurrentStartInstanceLocation = ~0u;
  }
}

void RHIDevice::set_opengl_compute_pipeline_state(RHIComputePipelineState* computePipelineState) {
  if (nullptr != computePipelineState) {
    // Sanity check
    RHI_MATCH_CHECK(*this, *computePipelineState)

    // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects()) {
      // Bind the program pipeline, if required
      const core::uint32 openGLProgramPipeline = static_cast<RHIComputePipelineStateSeparate *>(computePipelineState)->get_opengl_program_pipeline();
      if (openGLProgramPipeline != mOpenGLProgramPipeline) {
        mOpenGLProgramPipeline = openGLProgramPipeline;
        glBindProgramPipeline(mOpenGLProgramPipeline);
        mOpenGLVertexProgram = 0;
        mDrawIdUniformLocation = -1;
        mCurrentStartInstanceLocation = ~0u;
      }
    }
    else if (mExtensions->isGL_ARB_shader_objects()) {
      // Bind the program, if required
      const core::uint32 openGLProgram = static_cast<RHIComputePipelineStateMonolithic *>(computePipelineState)->get_opengl_program();
      if (openGLProgram != mOpenGLProgram) {
        mOpenGLProgram = openGLProgram;
        glUseProgram(mOpenGLProgram);
        mOpenGLVertexProgram = 0;
        mDrawIdUniformLocation = -1;
        mCurrentStartInstanceLocation = ~0u;
      }
    }
  } else {
    // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects()) {
      // Unbind the program pipeline, if required
      if (0 != mOpenGLProgramPipeline) {
        glBindProgramPipeline(0);
        mOpenGLProgramPipeline = 0;
      }
    }
    else if (mExtensions->isGL_ARB_shader_objects()) {
      // Unbind the program, if required
      if (0 != mOpenGLProgram) {
        glUseProgram(0);
        mOpenGLProgram = 0;
      }
    }
    mOpenGLVertexProgram = 0;
    mDrawIdUniformLocation = -1;
    mCurrentStartInstanceLocation = ~0u;
  }
}

void RHIDevice::updateGL_ARB_base_instanceEmulation(core::uint32 startInstanceLocation) {
  if (mDrawIdUniformLocation != -1 && 0 != mOpenGLVertexProgram && mCurrentStartInstanceLocation != startInstanceLocation) {
    glProgramUniform1ui(mOpenGLVertexProgram, mDrawIdUniformLocation, startInstanceLocation);
    mCurrentStartInstanceLocation = startInstanceLocation;
  }
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl


RHI_OPENGL_API rhi::RHIDevice* create_rhi_device_instance() {
  return re_new<rhi_opengl::RHIDevice>();
}