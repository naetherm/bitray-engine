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
#include "render_to_texture/render_to_texture.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void RenderToTexture::on_initialization()
{
	// get and check the RHI instance
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi)
	{
		// Create the buffer and texture manager
		mBufferManager = rhi->create_buffer_manager();
		mTextureManager = rhi->create_texture_manager();

		{ // Create the root signature
			rhi::DescriptorRangeBuilder ranges[2];
			ranges[0].initialize(rhi::ResourceType::TEXTURE_2D, 0, "AlbedoMap", rhi::ShaderVisibility::FRAGMENT);
			ranges[1].initializeSampler(0, rhi::ShaderVisibility::FRAGMENT);

			rhi::RootParameterBuilder rootParameters[2];
			rootParameters[0].initializeAsDescriptorTable(1, &ranges[0]);
			rootParameters[1].initializeAsDescriptorTable(1, &ranges[1]);

			// Setup
			rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(2, rootParameters, 0, nullptr, rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi->create_root_signature(rootSignatureBuilder);
		}

		// Create sampler state and wrap it into a resource group instance
		rhi::RHIResource* samplerStateResource = nullptr;
		{
			rhi::SamplerStateDescriptor samplerState = rhi::RHISamplerState::get_default_sampler_state();
			samplerState.maxLod = 0.0f;
			samplerStateResource = rhi->create_sampler_state(samplerState);
			mSamplerStateGroup = mRootSignature->create_resource_group(1, 1, &samplerStateResource);
		}

		{ // Texture resource related
			// Create the texture instance, but without providing texture data (we use the texture as render target)
			// -> Use the "rhi::TextureFlag::RENDER_TARGET"-flag to mark this texture as a render target
			// -> Required for Vulkan, Direct3D 9, Direct3D 10, Direct3D 11 and Direct3D 12
			// -> Not required for OpenGL and OpenGL ES 3
			// -> The optimized texture clear value is a Direct3D 12 related option
			const rhi::TextureFormat::Enum textureFormat = rhi::TextureFormat::Enum::R8G8B8A8;
			rhi::RHITexture* texture2D = mTextureManager->create_texture_2d(
        {{16, 16},
         rhi::TextureDimension::TextureDimension2D,
         textureFormat,
         rhi::TextureFlag::SHADER_RESOURCE | rhi::TextureFlag::RENDER_TARGET,
         rhi::TextureUsage::DEFAULT,
         1}, nullptr,
         reinterpret_cast<const rhi::OptimizedTextureClearValue *>(&core::Color4::GREEN));

			{ // Create texture group
				rhi::RHIResource* resource = texture2D;
				rhi::RHISamplerState* samplerState = static_cast<rhi::RHISamplerState*>(samplerStateResource);
				mTextureGroup = mRootSignature->create_resource_group(0, 1, &resource, &samplerState);
			}

			{ // Create the framebuffer object (FBO) instance
				const rhi::FramebufferAttachment colorFramebufferAttachment(texture2D);
				mFramebuffer = rhi->create_framebuffer(*rhi->create_render_pass(1, &textureFormat), &colorFramebufferAttachment);
			}
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

		{ // Create vertex array object (VAO)
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
			mVertexArray = mBufferManager->create_vertex_array({vertexAttributes, 1, vertexArrayVertexBuffers});
		}

		{
			// Create the graphics program
			rhi::RHIGraphicsProgramPtr graphicsProgram;
			{
				// get the shader source code (outsourced to keep an overview)
				const char* vertexShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
				#include "render_to_texture/shaders.inl"

				// Create the graphics program
				rhi::RHIShaderLanguage& shaderLanguage = rhi->get_default_shader_language();
				graphicsProgram = shaderLanguage.create_graphics_program(
          *mRootSignature,
          vertexAttributes,
          shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode),
					shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));
			}

			// Create the graphics pipeline state object (PSO)
			if (nullptr != graphicsProgram)
			{
				mGraphicsPipelineState = rhi->create_graphics_pipeline_state(
          rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgram, vertexAttributes,
                                                        get_main_render_target()->get_render_pass()));
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fill_command_buffer();
	}
}

void RenderToTexture::on_deinitialization()
{
	// release the used resources
	mVertexArray = nullptr;
	mGraphicsPipelineState = nullptr;
	mSamplerStateGroup = nullptr;
	mTextureGroup = nullptr;
	mFramebuffer = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mTextureManager = nullptr;
	mBufferManager = nullptr;
}

void RenderToTexture::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void RenderToTexture::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(nullptr != get_main_render_target(), "Invalid main render target")
	BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mFramebuffer, "Invalid framebuffer")
	BE_ASSERT(nullptr != mTextureGroup, "Invalid texture group")
	BE_ASSERT(nullptr != mSamplerStateGroup, "Invalid sampler state group")
	BE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
	BE_ASSERT(nullptr != mVertexArray, "Invalid vertex array")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	{ // Render to texture
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Render to texture")

		// This in here is of course just an example. In a real application
		// there would be no point in constantly updating texture content
		// without having any real change.

		// Set the graphics render target to render into
		rhi::Command::SetGraphicsRenderTarget::create(mCommandBuffer, mFramebuffer);

		// Clear the graphics color buffer of the current render target with green
		rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR, core::Color4::GREEN);

		// Restore graphics main swap chain as current render target
		rhi::Command::SetGraphicsRenderTarget::create(mCommandBuffer, get_main_render_target());
	}

	{ // Use the render to texture result
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Use the render to texture result")

		// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
		rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);

		// Set the used graphics root signature
		rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

		// Set the used graphics pipeline state object (PSO)
		rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

		// Set graphics resource groups
		rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mTextureGroup);
		rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mSamplerStateGroup);

		// Input assembly (IA): Set the used vertex array
		rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArray);

		// Render the specified geometric primitive, based on an array of vertices
		rhi::Command::DrawGraphics::create(mCommandBuffer, 3);
	}
}
