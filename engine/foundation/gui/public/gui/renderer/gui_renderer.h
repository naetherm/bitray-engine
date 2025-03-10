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
#include "gui/gui.h"
#include <core/core/refcounted.h>
#include <core/core/ptr.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace core {
class FrontendServer;
class Window;
}
namespace rhi {
class RHICommandBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiServer;
class GuiWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class GuiRenderer : public core::RefCounted {
public:
  
  GuiRenderer(core::Ptr<GuiServer> guiServer);
  
  virtual ~GuiRenderer();


  /**
   * @brief
   * Returns pointer to the dynamic rhi instance.
   *
   * @return
   * Pointer to the dynamic rhi instance.
   */
  [[nodiscard]] const rhi::RHIDevice* get_rhi_device() const;
  
  [[nodiscard]] rhi::RHIDevice* get_rhi_device();

  [[nodiscard]] rhi::RHIRenderPass* get_default_render_pass() const;

  [[nodiscard]] const rhi::RHICommandBuffer& get_command_buffer() const;

  [[nodiscard]] rhi::RHICommandBuffer& get_command_buffer();

public:

  core::Ptr<rhi::RHISwapChain> initialize_window_swapchain(core::Window* window);

public:

  void fill_graphics_command_buffer(GuiWindow* guiWindow, rhi::RHICommandBuffer& commandBuffer);

  void internal_fill_graphics_command_buffer(rhi::RHICommandBuffer& commandBuffer);

  const rhi::RHIVertexArrayPtr& fill_vertex_array_ptr();

  void start_frame(GuiWindow* mainWindow);

  void draw(GuiWindow* guiWindow);

  void end_frame(GuiWindow* guiWindow);

private:

  void create_required_rhi_resources();
  
private:
  
  core::Ptr<GuiServer> mGuiServer;
  
  rhi::RHIDevice* mRhiDynamicRhi;
  /** Pointer to the rhi context, must be valid */
  rhi::RHIContext* mRHIContext;
  
  /** The default render pass */
  rhi::RHIRenderPass* mDefaultRenderPass;
  /** The main command buffer */
  rhi::RHICommandBuffer mMainCommandBuffer;
  /** Pointer to the rhi buffer manager of the gui renderer */
  rhi::RHIBufferManagerPtr mRhiBufferManager;
  /** Pointer to the rhi texture manager of the gui renderer */
  rhi::RHITextureManagerPtr mRhiTextureManager;

  // Built in rhi configuration resources
  rhi::RHIRootSignaturePtr mRhiRootSignature;

  rhi::RHITexture2DPtr mDefaultFontTexture;

  rhi::RHIGraphicsProgramPtr mGraphicsProgram;
  rhi::RHIGraphicsPipelineStatePtr mGraphicsPipelineState;
  rhi::RHIUniformBufferPtr mVertexShaderUniformBuffer;
  core::handle mObjectSpaceToClipSpaceMatrixUniformHandle;
  rhi::RHIResourceGroupPtr mResourceGroup;
  rhi::RHIResourceGroupPtr mSamplerStateGroup;

  // Vertex and index buffers
  rhi::RHIVertexBufferPtr mVertexBuffer;
  core::uint32 mNumberOfAllocatedVertices;
  rhi::RHIIndexBufferPtr mIndexBuffer;
  core::uint32 mNumberOfAllocatedIndices;
  rhi::RHIVertexArrayPtr mVertexArray;

  core::uint64 mTime;
  
  bool mIsInitialized;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}