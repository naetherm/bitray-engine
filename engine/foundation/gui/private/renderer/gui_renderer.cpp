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
#include "gui/renderer/gui_renderer.h"
#include "gui/gui/gui_context.h"
#include "gui/gui/gui_server.h"
#include "gui/gui/gui_window.h"
#include <core/frontend/window.h>
#include <imgui.h>
#include <imgui_internal.h>
#if defined(LINUX)
#include <core/linux/xcb_backend.h>
#include <sys/time.h>
#endif


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {

namespace detail
{


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
//static const core::AssetId IMGUI_GLYPH_MAP_2D(ASSET_ID("racoon_engine/texture/dynamic_by_code/ImGuiGlyphMap2D"));

// Vertex input layout
static constexpr rhi::VertexAttribute VertexAttributesLayout[] =
  {
    { // Attribute 0
      // Data destination
      rhi::VertexAttributeFormat::FLOAT_4,		// vertexAttributeFormat (engine::VertexAttributeFormat)
      "Position",									// name[32] (char)
      "POSITION",									// semanticName[32] (char)
      0,											// semanticIndex (core::uint32)
      // Data source
      0,											// inputSlot (core::uint32)
      0,											// alignedByteOffset (core::uint32)
      sizeof(float) * 4 + sizeof(core::uint8) * 4,	// strideInBytes (core::uint32)
      0											// instancesPerElement (core::uint32)
    },
    { // Attribute 1
      // Data destination
      rhi::VertexAttributeFormat::R8G8B8A8_UNORM,	// vertexAttributeFormat (engine::VertexAttributeFormat)
      "Color",									// name[32] (char)
      "COLOR",									// semanticName[32] (char)
      0,											// semanticIndex (core::uint32)
      // Data source
      0,											// inputSlot (core::uint32)
      sizeof(float) * 4,							// alignedByteOffset (core::uint32)
      sizeof(float) * 4 + sizeof(core::uint8) * 4,	// strideInBytes (core::uint32)
      0											// instancesPerElement (core::uint32)
    }
  };
const rhi::VertexAttributes VertexAttributes(2, VertexAttributesLayout);


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
} // detail



//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiRenderer::GuiRenderer(core::Ptr<GuiServer> guiServer)
: mGuiServer(guiServer)
, mRHIContext(nullptr)
, mDefaultRenderPass(nullptr)
, mRhiRootSignature(nullptr)
, mIsInitialized(false) {
  
}

GuiRenderer::~GuiRenderer() {
  
}


const rhi::RHIDevice* GuiRenderer::get_rhi_device() const {
  return mRhiDynamicRhi;
}

rhi::RHIDevice* GuiRenderer::get_rhi_device() {
  return mRhiDynamicRhi;
}

[[nodiscard]] rhi::RHIRenderPass* GuiRenderer::get_default_render_pass() const {
  return mDefaultRenderPass;
}

[[nodiscard]] const rhi::RHICommandBuffer& GuiRenderer::get_command_buffer() const {
  return mMainCommandBuffer;
}

[[nodiscard]] rhi::RHICommandBuffer& GuiRenderer::get_command_buffer() {
  return mMainCommandBuffer;
}


core::Ptr<rhi::RHISwapChain> GuiRenderer::initialize_window_swapchain(core::Window* window) {
  core::FrontendPlatformData platformData;
#if defined(LINUX)
  platformData.os_linux.connection = core::XcbBackend::instance().get_xcb_connection();
  platformData.os_linux.window = window->get_native_window_handle();
#endif
  
  {
    if (mRHIContext == nullptr && !mIsInitialized) {
#if defined(LINUX)
      mRHIContext = new rhi::RHIContext(platformData);
#endif
      typedef rhi::RHIDevice *(*RHI_INSTANCER)();
      
      RHI_INSTANCER _Creator = reinterpret_cast<RHI_INSTANCER>(mGuiServer->mGuiContext->get_rhi_library().get_symbol("create_rhi_device_instance"));
      
      if (_Creator) {
        
        mRhiDynamicRhi = _Creator();
        mRhiDynamicRhi->initialize(*mRHIContext);
        
        if (mRhiDynamicRhi) {
          mRhiDynamicRhi->add_ref();
        }
        
        mRhiBufferManager = mRhiDynamicRhi->create_buffer_manager();
        mRhiBufferManager->add_ref();
        mRhiTextureManager = mRhiDynamicRhi->create_texture_manager();
        mRhiTextureManager->add_ref();
      }
      
      const rhi::Capabilities& cCapabilities = mRhiDynamicRhi->get_capabilities();
      mDefaultRenderPass = mRhiDynamicRhi->create_render_pass(
        1,
        &cCapabilities.preferredSwapChainColorTextureFormat,
        cCapabilities.preferredSwapChainDepthStencilTextureFormat,
        1);

      // TODO

      // Create default font
      {
        ImFontConfig config;
        config.MergeMode = true;
        config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
        // TODO(naetherm): Add fontawesone support!

        ImGui::GetIO().Fonts->AddFontDefault();

        unsigned char *pixels = nullptr;
        int width = 0;
        int height = 0;
        //ImGui::GetIO().Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
        ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        
        // Upload texture to RHI
        mDefaultFontTexture = mRhiTextureManager->create_texture_2d(
            {
              {static_cast<core::uint32>(width), static_cast<core::uint32>(height), 0},
              rhi::TextureDimension2D,
              rhi::TextureFormat::R8G8B8A8,
              rhi::TextureFlag::GENERATE_MIPMAPS | rhi::TextureFlag::SHADER_RESOURCE,
              rhi::TextureUsage::DEFAULT,
              1,
            },
            pixels, 
            nullptr RHI_RESOURCE_DEBUG_NAME("Debug 2D GUI glyph texture atlas"));
       
      }
      
      mIsInitialized = true;
    }
  }
  
  rhi::RHISwapChain* swapChain;
  
  // Create and initialize swap chain
  swapChain = mRhiDynamicRhi->create_swap_chain(*mDefaultRenderPass, window->get_platform_data());
  
  return core::Ptr<rhi::RHISwapChain>(swapChain);
}

void GuiRenderer::fill_graphics_command_buffer(GuiWindow* guiWindow, rhi::RHICommandBuffer& commandBuffer) {
  // Do we have to build the internal resources first
  if (mRhiRootSignature == nullptr)  {
    create_required_rhi_resources();
  }

  // Proceed

  // Set up the orthographic projection matrix within the vertex shader uniform buffer
  {
    // We require the size of the window we are drawing to
    core::uint32 x, y;
    guiWindow->get_swap_chain()->get_width_and_height(x, y);
    //printf("Display Size: %d x %d\n", x, y);
    core::Vec2f texelOffset(0.0f, 0.0f);
    if (mRhiDynamicRhi->get_name_id() == rhi::NameId::DIRECT3D9) {
      texelOffset += 0.5f;
    }

    const float l = texelOffset.x;
    const float r = x + texelOffset.x;
    const float t = texelOffset.y;
    const float b = y + texelOffset.y;
    const float objectSpaceToClipSpaceMatrix[4][4] =
      {
        { 2.0f / (r - l),	 0.0f,        0.0f, 0.0f },
        { 0.0f,				 2.0f/(t-b),  0.0f, 0.0f },
        { 0.0f,				 0.0f,        0.5f, 0.0f },
        { (r + l) / (l - r), (t+b)/(b-t), 0.5f, 1.0f }
      };

    if (nullptr != mVertexShaderUniformBuffer)
    {
      // Copy data into the uniform buffer
      rhi::Command::CopyUniformBufferData::create(commandBuffer, *mVertexShaderUniformBuffer, objectSpaceToClipSpaceMatrix, sizeof(objectSpaceToClipSpaceMatrix));
    }
    else
    {
      // Set legacy uniforms
      rhi::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToClipSpaceMatrixUniformHandle, &objectSpaceToClipSpaceMatrix[0][0]);
    }

  }

  { // RHI configuration
    // Set the used graphics root signature
    rhi::Command::SetGraphicsRootSignature::create(commandBuffer, mRhiRootSignature);

    // Set the used graphics pipeline state object (PSO)
    rhi::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

    // Set graphics resource groups
    rhi::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, mResourceGroup);
    rhi::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
  }

  // Setup input assembly (IA): Set the used vertex array
  rhi::Command::SetGraphicsVertexArray::create(commandBuffer, fill_vertex_array_ptr());

  // Render command lists
  internal_fill_graphics_command_buffer(commandBuffer);
}

void GuiRenderer::internal_fill_graphics_command_buffer(rhi::RHICommandBuffer& commandBuffer) {
  if (GImGui->Initialized)
  {
    // No combined scoped profiler CPU and GPU sample as well as renderer debug event command by intent, this is something the caller has to take care of
    // RE_SCOPED_PROFILER_EVENT(mRenderer.get_context(), commandBuffer, "Debug GUI")

    // Render command lists
    // -> There's no need to try to gather draw calls and batch them into multi-draw-indirect buffers, ImGui does already a pretty good job
    int vertexOffset = 0;
    int indexOffset = 0;
    const ImDrawData* imDrawData = ImGui::GetDrawData();
    for (int commandListIndex = 0; commandListIndex < imDrawData->CmdListsCount; ++commandListIndex)
    {
      const ImDrawList* imDrawList = imDrawData->CmdLists[commandListIndex];
      for (int commandIndex = 0; commandIndex < imDrawList->CmdBuffer.size(); ++commandIndex)
      {
        const ImDrawCmd* pcmd = &imDrawList->CmdBuffer[commandIndex];
        if (nullptr != pcmd->UserCallback)
        {
          //BE_LOG(Info, "Callback method")
          pcmd->UserCallback(imDrawList, pcmd);
        }
        else
        {
          /*
          if (pcmd->TextureId) {
            { // Setup orthographic projection matrix into our vertex shader uniform buffer
              const ImVec2& displaySize = ImGui::GetIO().DisplaySize;
              ImVec2 texelOffset(0.0f, 0.0f);
              if (get_rhi()->get_name_id() == engine::NameId::DIRECT3D9)
              {
                // Take care of the Direct3D 9 half-pixel/half-texel offset
                // -> See "Directly Mapping Texels to Pixels (Direct3D 9)" at https://docs.microsoft.com/en-gb/windows/win32/direct3d9/directly-mapping-texels-to-pixels?redirectedfrom=MSDN
                texelOffset.x += 0.5f;
                texelOffset.y += 0.5f;
              }
              const float l = texelOffset.x;
              const float r = displaySize.x + texelOffset.x;
              const float t = texelOffset.y;
              const float b = displaySize.y + texelOffset.y;
              const float objectSpaceToClipSpaceMatrix[4][4] =
                {
                  { 2.0f / (r - l),	 0.0f,        0.0f, 0.0f },
                  { 0.0f,				 2.0f/(t-b),  0.0f, 0.0f },
                  { 0.0f,				 0.0f,        0.5f, 0.0f },
                  { (r + l) / (l - r), (t+b)/(b-t), 0.5f, 1.0f }
                };
              if (nullptr != mVertexShaderUniformBuffer)
              {
                // Copy data into the uniform buffer
                engine::Command::CopyUniformBufferData::create(commandBuffer, *mVertexShaderUniformBuffer, objectSpaceToClipSpaceMatrix, sizeof(objectSpaceToClipSpaceMatrix));
              }
              else
              {
                // Set legacy uniforms
                engine::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToClipSpaceMatrixUniformHandle, &objectSpaceToClipSpaceMatrix[0][0]);
              }
            }
            engine::RHITexture2D *tex = reinterpret_cast<engine::RHITexture2D *>(pcmd->TextureId);

            //engine::Command::set_graphics_root_signature::create(commandBuffer, mRootSignature);

            // Set the used graphics pipeline state object (PSO)
            //engine::Command::set_graphics_pipeline_state::create(commandBuffer, mGraphicsPipelineState);

            // Set graphics resource groups
            engine::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, this->get_resource_group_by_texture(tex));
            engine::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
            //engine::Command::set_graphics_resource_group::create(commandBuffer, 0, tex);
          } else {

            //engine::Command::set_graphics_root_signature::create(commandBuffer, mRootSignature);

            // Set the used graphics pipeline state object (PSO)
            //engine::Command::set_graphics_pipeline_state::create(commandBuffer, mGraphicsPipelineState);

            // Set graphics resource groups
            engine::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, this->get_resource_group_by_texture(mDefaultFontTexture));
            engine::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
          }
          */

          // Set graphics scissor rectangle
          rhi::Command::SetGraphicsScissorRectangles::create(commandBuffer, static_cast<long>(pcmd->ClipRect.x), static_cast<long>(pcmd->ClipRect.y), static_cast<long>(pcmd->ClipRect.z), static_cast<long>(pcmd->ClipRect.w));

          // Draw graphics
          rhi::Command::DrawIndexedGraphics::create(commandBuffer, static_cast<core::uint32>(pcmd->ElemCount), 1, static_cast<core::uint32>(indexOffset), static_cast<core::int32>(vertexOffset));
        }
        indexOffset += pcmd->ElemCount;
      }
      vertexOffset += imDrawList->VtxBuffer.size();
    }
  }
}

const rhi::RHIVertexArrayPtr& GuiRenderer::fill_vertex_array_ptr() {
  // Only proceed if imgui was initialized
  if (GImGui->Initialized) {
    ImGui::Render();

    const ImDrawData* drawData = ImGui::GetDrawData();
    // Get additional data
    rhi::RHIDevice* rhi = get_rhi_device();

    // Create vertex and index buffers
    {
      // Create and/or grow buffers if needed
      if (mVertexBuffer == nullptr || mNumberOfAllocatedVertices < drawData->TotalVtxCount) {
        mNumberOfAllocatedVertices = static_cast<core::uint32>(drawData->TotalVtxCount + 5000);	// Add some reserve to reduce reallocations
        mVertexBuffer = mRhiBufferManager->create_vertex_buffer({mNumberOfAllocatedVertices * sizeof(ImDrawVert), nullptr, 0,
                                                                 rhi::BufferUsage::DYNAMIC_DRAW}
          RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
        mVertexArray = nullptr;
      }

      if (mIndexBuffer == nullptr || mNumberOfAllocatedIndices < drawData->TotalIdxCount) {
        mNumberOfAllocatedIndices = static_cast<core::uint32>(drawData->TotalIdxCount + 10000);	// Add some reserve to reduce reallocations
        mIndexBuffer = mRhiBufferManager->create_index_buffer({mNumberOfAllocatedIndices * sizeof(ImDrawIdx), nullptr, 0,
                                                               rhi::BufferUsage::DYNAMIC_DRAW,
                                                               rhi::IndexBufferFormat::UNSIGNED_SHORT}
          RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
        mVertexArray = nullptr;
      }

      // Check for vertex array
      {
        if (mVertexArray == nullptr) {
          BE_ASSERT(nullptr != mVertexBuffer, "Invalid vertex buffer")
          BE_ASSERT(nullptr != mIndexBuffer, "Invalid index buffer")

          // Create vertex array object (VAO)
          const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { mVertexBuffer };
          mVertexArray = mRhiBufferManager->create_vertex_array(
            {
              gui::detail::VertexAttributes,
              1,
              vertexArrayVertexBuffers,
              mIndexBuffer
            } RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
        }
      }

      // Copy and convert all vertices and indices into a single contiguous buffer
      {
        rhi::MappedSubresource vertexBufferMappedSubresource;
        if (rhi->map(*mVertexBuffer, 0, rhi::MapType::WRITE_DISCARD, 0, vertexBufferMappedSubresource))
        {
          rhi::MappedSubresource indexBufferMappedSubresource;
          if (rhi->map(*mIndexBuffer, 0, rhi::MapType::WRITE_DISCARD, 0, indexBufferMappedSubresource))
          {
            ImDrawVert* imDrawVert = static_cast<ImDrawVert*>(vertexBufferMappedSubresource.data);
            ImDrawIdx* imDrawIdx = static_cast<ImDrawIdx*>(indexBufferMappedSubresource.data);
            for (int i = 0; i < drawData->CmdListsCount; ++i)
            {
              const ImDrawList* imDrawList = drawData->CmdLists[i];
              memcpy(imDrawVert, &imDrawList->VtxBuffer[0], imDrawList->VtxBuffer.size() * sizeof(ImDrawVert));
              memcpy(imDrawIdx, &imDrawList->IdxBuffer[0], imDrawList->IdxBuffer.size() * sizeof(ImDrawIdx));
              imDrawVert += imDrawList->VtxBuffer.size();
              imDrawIdx += imDrawList->IdxBuffer.size();
            }

            // Unmap the index buffer
            rhi->unmap(*mIndexBuffer, 0);
          }

          // Unmap the vertex buffer
          rhi->unmap(*mVertexBuffer, 0);
        }
      }
    }
  }
  
  return mVertexArray;
}


void GuiRenderer::start_frame(GuiWindow* guiWindow) {
  {
    ImGuiIO& imGuiIo = ImGui::GetIO();

    { // Setup display size (every frame to accommodate for render target resizing)
      core::uint32 width = 0;
      core::uint32 height = 0;
      guiWindow->get_swap_chain()->get_width_and_height(width, height);
      imGuiIo.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
    }

    { // Setup time step
#if defined(LINUX)
      timeval currentTimeValue;
      gettimeofday(&currentTimeValue, nullptr);
      const core::uint64 currentTime = currentTimeValue.tv_sec * 1000000 + currentTimeValue.tv_usec;
      imGuiIo.DeltaTime = static_cast<float>(currentTime - mTime) / 1000000.0f;
      mTime = currentTime;
#endif
    }
  }

  ImGui::NewFrame();
}

void GuiRenderer::draw(GuiWindow* guiWindow) {
  fill_graphics_command_buffer(guiWindow, mMainCommandBuffer);

  get_command_buffer().dispatch_to_rhi_and_clear(*get_rhi_device());
}

void GuiRenderer::end_frame(GuiWindow* guiWindow) {
  
}


void GuiRenderer::create_required_rhi_resources() {
  printf("create_required_rhi_resources\n");
  // Create the root signature instance
  {
    // Create the root signature
    rhi::DescriptorRangeBuilder ranges[4];
    ranges[0].initialize(rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicVs", rhi::ShaderVisibility::VERTEX);
    ranges[1].initialize(rhi::ResourceType::TEXTURE_2D,		0, "GlyphMap",				rhi::ShaderVisibility::FRAGMENT);
    ranges[2].initializeSampler(0, rhi::ShaderVisibility::FRAGMENT);

    rhi::RootParameterBuilder rootParameters[2];
    rootParameters[0].initializeAsDescriptorTable(2, &ranges[0]);
    rootParameters[1].initializeAsDescriptorTable(1, &ranges[2]);

    // Setup
    rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
    rootSignatureBuilder.initialize(2, rootParameters, 0, nullptr, rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Create the instance
    mRhiRootSignature = mRhiDynamicRhi->create_root_signature(rootSignatureBuilder RHI_RESOURCE_DEBUG_NAME("GuiRenderer"));
  }

  // Create the graphics pipeline state instance
  {
    // Create the graphics program
    {
      // get the shader source code, this is outsourced for now, but later we will use the shader system
      const char* vertexShaderSourceCode = nullptr;
      const char* fragmentShaderSourceCode = nullptr;
#include "gui/renderer/shaders/gui_glsl_450.h"	// For Vulkan
#include "gui/renderer/shaders/gui_glsl_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "gui/renderer/shaders/gui_null.h"

      // Create the graphics program
      rhi::RHIShaderLanguage& shaderLanguage = mRhiDynamicRhi->get_default_shader_language();
      mGraphicsProgram = shaderLanguage.create_graphics_program(
        *mRhiRootSignature,
        gui::detail::VertexAttributes,
        shaderLanguage.create_vertex_shader_from_source_code(gui::detail::VertexAttributes, vertexShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("GuiRenderer")),
      shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("GuiRenderer"))
      RHI_RESOURCE_DEBUG_NAME("GUI"));
    }

    // Now create the graphics pipeline state object
    {
      rhi::GraphicsPipelineStateDescriptor graphicsPipelineState = rhi::GraphicsPipelineStateDescriptorBuilder(
        mRhiRootSignature,
        mGraphicsProgram,
        gui::detail::VertexAttributes,
        *get_default_render_pass());
      graphicsPipelineState.rasterizerState.cullMode				 = rhi::CullMode::NONE;
      graphicsPipelineState.rasterizerState.scissorEnable			 = 1;
      graphicsPipelineState.depthStencilState.depthEnable			 = false;
      graphicsPipelineState.depthStencilState.depthWriteMask		 = rhi::DepthWriteMask::ZERO;
      graphicsPipelineState.blendState.renderTarget[0].blendEnable = true;
      graphicsPipelineState.blendState.renderTarget[0].srcBlend	 = rhi::Blend::SRC_ALPHA;
      graphicsPipelineState.blendState.renderTarget[0].destBlend	 = rhi::Blend::INV_SRC_ALPHA;
      mGraphicsPipelineState = mRhiDynamicRhi->create_graphics_pipeline_state(graphicsPipelineState RHI_RESOURCE_DEBUG_NAME("GUI"));
    }
  }


  // Create vertex uniform buffer instance
  {
    if (mRhiDynamicRhi->get_capabilities().maximumUniformBufferSize > 0)
    {
      mVertexShaderUniformBuffer = mRhiBufferManager->create_uniform_buffer(
        {sizeof(float) * 4 * 4,
         nullptr,
         rhi::BufferUsage::DYNAMIC_DRAW} RHI_RESOURCE_DEBUG_NAME("GUI"));
    }
    else if (nullptr != mGraphicsProgram)
    {
      mObjectSpaceToClipSpaceMatrixUniformHandle = mGraphicsProgram->get_uniform_handle("ObjectSpaceToClipSpaceMatrix");
    }

    // Create sampler state instance and wrap it into a resource group instance
    rhi::RHIResource* samplerStateResource = nullptr;
    {
      rhi::SamplerStateDescriptor samplerState = rhi::RHISamplerState::get_default_sampler_state();
      samplerState.addressU = rhi::TextureAddressMode::WRAP;
      samplerState.addressV = rhi::TextureAddressMode::WRAP;
      samplerStateResource = mRhiDynamicRhi->create_sampler_state(samplerState RHI_RESOURCE_DEBUG_NAME("GuiRenderer"));
      mSamplerStateGroup = mRhiRootSignature->create_resource_group(1, 1, &samplerStateResource, nullptr RHI_RESOURCE_DEBUG_NAME("GuiRenderer"));
    }

    { // Create resource group
      rhi::RHIResource *resources[2] = {mVertexShaderUniformBuffer, mDefaultFontTexture};
      rhi::RHISamplerState *samplerStates[2] = {nullptr, static_cast<rhi::RHISamplerState *>(samplerStateResource)};
      mResourceGroup = mRhiRootSignature->create_resource_group(
        0,
        2,
        resources,
        samplerStates RHI_RESOURCE_DEBUG_NAME("GuiRenderer"));
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}