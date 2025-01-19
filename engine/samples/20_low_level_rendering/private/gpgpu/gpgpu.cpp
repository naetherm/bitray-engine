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
#include "gpgpu/gpgpu.h"
#include <core/color/color4.h>
#include <core/log/log.h>
#include <core/core/default_assert.h>
#include <core/memory/memory.h>


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Gpgpu::Gpgpu(Application& cApplication, const core::String& name) :
	ExampleBase(cApplication, name)
{
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void Gpgpu::on_initialization()
{
	// Create the buffer and texture manager
	mBufferManager = get_rhi()->create_buffer_manager();
	mTextureManager = get_rhi()->create_texture_manager();

	{ // Create the root signature
		rhi::DescriptorRangeBuilder ranges[2];
		ranges[0].initialize(rhi::ResourceType::TEXTURE_2D, 0, "ContentMap", rhi::ShaderVisibility::FRAGMENT);
		ranges[1].initializeSampler(0, rhi::ShaderVisibility::FRAGMENT);

		rhi::RootParameterBuilder rootParameters[2];
		rootParameters[0].initializeAsDescriptorTable(1, &ranges[0]);
		rootParameters[1].initializeAsDescriptorTable(1, &ranges[1]);

		// Setup
		rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
		rootSignatureBuilder.initialize(2, rootParameters, 0, nullptr, rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		mRootSignature = get_rhi()->create_root_signature(rootSignatureBuilder);
	}

	// Create the 2D texture and framebuffer object (FBO) instances
	const rhi::TextureFormat::Enum textureFormat = rhi::TextureFormat::Enum::R8G8B8A8;
	rhi::RHIRenderPass* renderPass = get_rhi()->create_render_pass(1, &textureFormat);
	for (int i = 0; i < 2; ++i)
	{
		// Create the texture instance, but without providing texture data (we use the texture as render target)
		// -> Use the "rhi::TextureFlag::RENDER_TARGET"-flag to mark this texture as a render target
		// -> Required for Vulkan, Direct3D 9, Direct3D 10, Direct3D 11 and Direct3D 12
		// -> Not required for OpenGL and OpenGL ES 3
		// -> The optimized texture clear value is a Direct3D 12 related option
		rhi::RHITexture* texture2D = mTexture2D[i] = mTextureManager->create_texture_2d(
      {{64, 64},
       rhi::TextureDimension::TextureDimension2D,
       textureFormat,
       rhi::TextureFlag::SHADER_RESOURCE | rhi::TextureFlag::RENDER_TARGET,
       rhi::TextureUsage::DEFAULT,
       1},
       nullptr,
       reinterpret_cast<const rhi::OptimizedTextureClearValue *>(&core::Color4::BLUE));

		// Create the framebuffer object (FBO) instance
		const rhi::FramebufferAttachment colorFramebufferAttachment(texture2D);
		mFramebuffer[i] = get_rhi()->create_framebuffer(*renderPass, &colorFramebufferAttachment);
	}

	// Create sampler state and wrap it into a resource group instance: We don't use mipmaps
	rhi::RHIResource* samplerStateResource = nullptr;
	{
		rhi::SamplerStateDescriptor samplerState = rhi::RHISamplerState::get_default_sampler_state();
		samplerState.maxLod = 0.0f;
		samplerStateResource = get_rhi()->create_sampler_state(samplerState);
		mSamplerStateGroup = mRootSignature->create_resource_group(1, 1, &samplerStateResource);
	}

	{ // Create texture group
		rhi::RHIResource* resource = mTexture2D[0];
		rhi::RHISamplerState* samplerState = static_cast<rhi::RHISamplerState*>(samplerStateResource);
		mTextureGroup = mRootSignature->create_resource_group(0, 1, &resource, &samplerState);
	}

	// Vertex input layout
	static constexpr rhi::VertexAttribute vertexAttributesLayout[] =
	{
		{ // Attribute 0
			// Data destination
			rhi::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
			"Position",								// name[32] (char)
			"POSITION",								// semanticName[32] (char)
			0,										// semanticIndex (core::uint32)
			// Data source
			0,										// inputSlot (core::uint32)
			0,										// alignedByteOffset (core::uint32)
			sizeof(float) * 2,						// strideInBytes (core::uint32)
			0										// instancesPerElement (core::uint32)
		}
	};
	const rhi::VertexAttributes vertexAttributes(1, vertexAttributesLayout);

	{ // Create vertex array object (VAO) for content generation
		// Create the vertex buffer object (VBO)
		// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
		static constexpr float VERTEX_POSITION[] =
		{					// Vertex ID	Triangle on screen
			 0.0f, 1.0f,	// 0				0
			 1.0f, 0.0f,	// 1			   .   .
			-0.5f, 0.0f		// 2			  2.......1
		};
		rhi::RHIVertexBufferPtr vertexBuffer(
      mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), VERTEX_POSITION}));

		// Create vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
		const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
		mVertexArrayContentGeneration = mBufferManager->create_vertex_array({vertexAttributes,
                                                                        1,
                                                                        vertexArrayVertexBuffers});
	}

	{ // Create vertex array object (VAO) for content processing
		// Create the vertex buffer object (VBO)
		// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
		static constexpr float VERTEX_POSITION[] =
		{					// Vertex ID	Triangle strip on screen
			-1.0f, -1.0f,	// 0			  1.......3
			-1.0f,  1.0f,	// 1			  .   .   .
			 1.0f, -1.0f,	// 2			  0.......2
			 1.0f,  1.0f	// 3
		};
		rhi::RHIVertexBufferPtr vertexBuffer(
      mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), VERTEX_POSITION}));

		// Create vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
		const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
		mVertexArrayContentProcessing = mBufferManager->create_vertex_array({vertexAttributes,
                                                                        1,
                                                                        vertexArrayVertexBuffers});
	}

	{
		// Create the graphics programs
		rhi::RHIGraphicsProgramPtr graphicsProgramContentGeneration;
		rhi::RHIGraphicsProgramPtr graphicsProgramContentProcessing;
		{
			// get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode_ContentGeneration = nullptr;
			const char* fragmentShaderSourceCode_ContentProcessing = nullptr;
			#include "gpgpu/shaders.inl"

			// In order to keep this example simple and to show that it's possible, we use the same vertex shader for both graphics programs
			// -> Depending on the used graphics API and whether or not the shader compiler & linker is clever,
			//    the unused texture coordinate might get optimized out
			// -> In a real world application you shouldn't rely on shader compiler & linker behaviour assumptions
			rhi::RHIShaderLanguage& shaderLanguage = get_rhi()->get_default_shader_language();
			rhi::RHIVertexShaderPtr vertexShader(
        shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode));
			graphicsProgramContentGeneration = shaderLanguage.create_graphics_program(*mRootSignature, vertexAttributes,
                                                                                vertexShader,
																																								shaderLanguage.create_fragment_shader_from_source_code(
																																									fragmentShaderSourceCode_ContentGeneration));
			graphicsProgramContentProcessing = shaderLanguage.create_graphics_program(*mRootSignature, vertexAttributes,
                                                                                vertexShader,
																																								shaderLanguage.create_fragment_shader_from_source_code(
																																									fragmentShaderSourceCode_ContentProcessing));
		}

		// Create the graphics pipeline state objects (PSO)
		if (nullptr != graphicsProgramContentGeneration && nullptr != graphicsProgramContentProcessing)
		{
			{ // Content generation
				rhi::GraphicsPipelineStateDescriptor graphicsPipelineState = rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgramContentGeneration, vertexAttributes,
                                                                                                                     mFramebuffer[0]->get_render_pass());
				graphicsPipelineState.depthStencilState.depthEnable = false;
				mGraphicsPipelineStateContentGeneration = get_rhi()->create_graphics_pipeline_state(graphicsPipelineState);
			}
			{ // Content processing
				rhi::GraphicsPipelineStateDescriptor graphicsPipelineState = rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgramContentProcessing, vertexAttributes,
                                                                                                                     mFramebuffer[0]->get_render_pass());
				graphicsPipelineState.primitiveTopology = rhi::PrimitiveTopology::TRIANGLE_STRIP;
				graphicsPipelineState.depthStencilState.depthEnable = false;
				mGraphicsPipelineStateContentProcessing = get_rhi()->create_graphics_pipeline_state(graphicsPipelineState);
			}
		}
	}

	// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	fillCommandBufferContentGeneration();
	fillCommandBufferContentProcessing();
}

void Gpgpu::on_deinitialization()
{
	// release the used resources
	mCommandBufferContentGeneration.clear();
	mCommandBufferContentProcessing.clear();
	mVertexArrayContentProcessing = nullptr;
	mGraphicsPipelineStateContentProcessing = nullptr;
	mVertexArrayContentGeneration = nullptr;
	mGraphicsPipelineStateContentGeneration = nullptr;
	mSamplerStateGroup = nullptr;
	mTextureGroup = nullptr;
	mRootSignature = nullptr;
	for (int i = 0; i < 2; ++i)
	{
		mFramebuffer[i] = nullptr;
		mTexture2D[i] = nullptr;
	}
	mBufferManager = nullptr;
	mTextureManager = nullptr;
}

void Gpgpu::fillCommandBufferContentGeneration()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(nullptr != mFramebuffer[0], "Invalid framebuffer at index 0")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mGraphicsPipelineStateContentGeneration, "Invalid graphics pipeline state content generation")
	BE_ASSERT(nullptr != mVertexArrayContentGeneration, "Invalid vertex array content generation")
	BE_ASSERT(mCommandBufferContentGeneration.is_empty(), "Command buffer content generation is already filled")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT(mCommandBufferContentGeneration, "Generate the content of the 2D texture to process later on")

	// Set the graphics render target to render into
	rhi::Command::SetGraphicsRenderTarget::create(mCommandBufferContentGeneration, mFramebuffer[0]);

	// Clear the graphics color buffer of the current render target with blue
	rhi::Command::ClearGraphics::create(mCommandBufferContentGeneration, rhi::ClearFlag::COLOR, core::Color4::BLUE);

	// Set the used graphics root signature
	rhi::Command::SetGraphicsRootSignature::create(mCommandBufferContentGeneration, mRootSignature);

	{ // Set the viewport and scissor rectangle
		// get the render target with and height
		core::uint32 width  = 1;
		core::uint32 height = 1;
		rhi::RHIRenderTarget* renderTarget = mFramebuffer[0];
		if (nullptr != renderTarget)
		{
      renderTarget->get_width_and_height(width, height);
		}

		// Set the graphics viewport and scissor rectangle
		rhi::Command::SetGraphicsViewportAndScissorRectangle::create(mCommandBufferContentGeneration, 0, 0, width, height);
	}

	// Set the used graphics pipeline state object (PSO)
	rhi::Command::SetGraphicsPipelineState::create(mCommandBufferContentGeneration, mGraphicsPipelineStateContentGeneration);

	// Input assembly (IA): Set the used vertex array
	rhi::Command::SetGraphicsVertexArray::create(mCommandBufferContentGeneration, mVertexArrayContentGeneration);

	// Render the specified geometric primitive, based on indexing into an array of vertices
	rhi::Command::DrawGraphics::create(mCommandBufferContentGeneration, 3);
}

void Gpgpu::fillCommandBufferContentProcessing()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(nullptr != mFramebuffer[1], "Invalid framebuffer at index 1")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mGraphicsPipelineStateContentProcessing, "Invalid graphics pipeline state content processing")
	BE_ASSERT(nullptr != mTextureGroup, "Invalid texture group")
	BE_ASSERT(nullptr != mSamplerStateGroup, "Invalid sampler state group")
	BE_ASSERT(nullptr != mTexture2D[0], "Invalid texture 2D at index 0")
	BE_ASSERT(mCommandBufferContentProcessing.is_empty(), "Command buffer content processing is already filled")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT(mCommandBufferContentProcessing, "Content processing")

	// Set the graphics render target to render into
	rhi::Command::SetGraphicsRenderTarget::create(mCommandBufferContentProcessing, mFramebuffer[1]);

	// We don't need to clear the current render target because our fullscreen quad covers the full screen

	// Set the used graphics root signature
	rhi::Command::SetGraphicsRootSignature::create(mCommandBufferContentProcessing, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	rhi::Command::SetGraphicsPipelineState::create(mCommandBufferContentProcessing, mGraphicsPipelineStateContentProcessing);

	// Set graphics resource groups
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBufferContentProcessing, 0, mTextureGroup);
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBufferContentProcessing, 1, mSamplerStateGroup);

	// Input assembly (IA): Set the used vertex array
	rhi::Command::SetGraphicsVertexArray::create(mCommandBufferContentProcessing, mVertexArrayContentProcessing);

	// Render the specified geometric primitive, based on indexing into an array of vertices
	rhi::Command::DrawGraphics::create(mCommandBufferContentProcessing, 4);
}

void Gpgpu::onDoJob()
{
	// Generate the content of the 2D texture to process later on
	// -> After this step, "mTexture2D[0]" holds the content we want to process later on
  mCommandBufferContentGeneration.dispatch_to_rhi(*get_rhi());

	// Content processing
	// -> After this step, "mTexture2D[1]" holds the processed content
  mCommandBufferContentProcessing.dispatch_to_rhi(*get_rhi());

	// TODO(naetherm) "rhi::RHIrhi::map()"/"rhi::RHIrhi::unmap()" are currently under construction
	// Map the texture holding the processed content
	rhi::MappedSubresource mappedSubresource;
	if (get_rhi()->map(*mTexture2D[1], 0, rhi::MapType::READ, 0, mappedSubresource))
	{
		// get the processed content pointer
//		const core::uint8* data = static_cast<core::uint8*>(mappedSubresource.data);

		// TODO(naetherm) Write it out as image?

		// Unmap the texture holding the processed content
    get_rhi()->unmap(*mTexture2D[1], 0);
	}
}
