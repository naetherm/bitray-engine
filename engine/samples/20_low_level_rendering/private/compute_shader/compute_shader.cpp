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
#include "compute_shader/compute_shader.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void ComputeShader::on_initialization()
{
	// get and check the RHI instance
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi)
	{
		// Create the buffer and texture manager
		mBufferManager = rhi->create_buffer_manager();
		mTextureManager = rhi->create_texture_manager();

		{ // Create the graphics root signature
			// TODO(naetherm) Compute shader: get rid of the OpenGL/Direct3D 11 variation here
			const core::uint32 offset = (rhi->get_name_id() == rhi::NameId::VULKAN ||
        rhi->get_name_id() == rhi::NameId::OPENGL) ? 1u : 0u;
			rhi::DescriptorRangeBuilder ranges[5];
			ranges[0].initialize(rhi::ResourceType::UNIFORM_BUFFER,	   0,			"UniformBuffer",		 rhi::ShaderVisibility::FRAGMENT);
			ranges[1].initialize(rhi::ResourceType::TEXTURE_BUFFER,	   0,			"InputTextureBuffer",	 rhi::ShaderVisibility::VERTEX);
			ranges[2].initialize(rhi::ResourceType::STRUCTURED_BUFFER, 1u + offset, "InputStructuredBuffer", rhi::ShaderVisibility::VERTEX);
			ranges[3].initialize(rhi::ResourceType::TEXTURE_2D,		   1,			"AlbedoMap",			 rhi::ShaderVisibility::FRAGMENT);
			ranges[4].initializeSampler(0, rhi::ShaderVisibility::FRAGMENT);

			rhi::RootParameterBuilder rootParameters[2];
			rootParameters[0].initializeAsDescriptorTable(4, &ranges[0]);
			rootParameters[1].initializeAsDescriptorTable(1, &ranges[4]);

			// Setup
			rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(2, rootParameters, 0, nullptr, rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mGraphicsRootSignature = rhi->create_root_signature(rootSignatureBuilder);
		}

		{ // Create the first compute root signature
			rhi::DescriptorRangeBuilder ranges[7];
			// Input
			ranges[0].initialize(rhi::ResourceType::TEXTURE_2D,		 0,	"InputTexture2D",	  rhi::ShaderVisibility::COMPUTE);
			ranges[1].initialize(rhi::ResourceType::VERTEX_BUFFER,   1,	"InputVertexBuffer",  rhi::ShaderVisibility::COMPUTE);
			ranges[2].initialize(rhi::ResourceType::INDEX_BUFFER,    2,	"InputIndexBuffer",	  rhi::ShaderVisibility::COMPUTE);
			ranges[3].initialize(rhi::ResourceType::UNIFORM_BUFFER,  0,	"InputUniformBuffer", rhi::ShaderVisibility::COMPUTE);
			// Output
			// TODO(naetherm) Compute shader: get rid of the OpenGL/Direct3D 11 variation here
			const core::uint32 offset = (rhi->get_name_id() == rhi::NameId::VULKAN ||
        rhi->get_name_id() == rhi::NameId::OPENGL) ? 4u : 0u;
			ranges[4].initialize(rhi::ResourceType::TEXTURE_2D,		 0u + offset, "OutputTexture2D",	 rhi::ShaderVisibility::COMPUTE, rhi::DescriptorRangeType::UAV);
			ranges[5].initialize(rhi::ResourceType::VERTEX_BUFFER,   1u + offset, "OutputVertexBuffer",  rhi::ShaderVisibility::COMPUTE, rhi::DescriptorRangeType::UAV);
			ranges[6].initialize(rhi::ResourceType::INDEX_BUFFER,    2u + offset, "OutputIndexBuffer",	 rhi::ShaderVisibility::COMPUTE, rhi::DescriptorRangeType::UAV);

			rhi::RootParameterBuilder rootParameters[1];
			rootParameters[0].initializeAsDescriptorTable(7, &ranges[0]);

			// Setup
			rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(1, rootParameters, 0, nullptr, rhi::RootSignatureFlags::NONE);

			// Create the instance
			mComputeRootSignature1 = rhi->create_root_signature(rootSignatureBuilder);
		}

		{ // Create the second compute root signature
			rhi::DescriptorRangeBuilder ranges[6];
			// Input
			ranges[0].initialize(rhi::ResourceType::TEXTURE_BUFFER,	   0, "InputTextureBuffer",	    rhi::ShaderVisibility::COMPUTE);
			ranges[1].initialize(rhi::ResourceType::STRUCTURED_BUFFER, 1, "InputStructuredBuffer",  rhi::ShaderVisibility::COMPUTE);
			ranges[2].initialize(rhi::ResourceType::INDIRECT_BUFFER,   2, "InputIndirectBuffer",	rhi::ShaderVisibility::COMPUTE);
			// Output
			// TODO(naetherm) Compute shader: get rid of the OpenGL/Direct3D 11 variation here
			const core::uint32 offset = (rhi->get_name_id() == rhi::NameId::VULKAN ||
        rhi->get_name_id() == rhi::NameId::OPENGL) ? 3u : 0u;
			ranges[3].initialize(rhi::ResourceType::TEXTURE_BUFFER,	   0u + offset, "OutputTextureBuffer",	  rhi::ShaderVisibility::COMPUTE, rhi::DescriptorRangeType::UAV);
			ranges[4].initialize(rhi::ResourceType::STRUCTURED_BUFFER, 1u + offset, "OutputStructuredBuffer", rhi::ShaderVisibility::COMPUTE, rhi::DescriptorRangeType::UAV);
			ranges[5].initialize(rhi::ResourceType::INDIRECT_BUFFER,   2u + offset, "OutputIndirectBuffer",   rhi::ShaderVisibility::COMPUTE, rhi::DescriptorRangeType::UAV);

			rhi::RootParameterBuilder rootParameters[1];
			rootParameters[0].initializeAsDescriptorTable(6, &ranges[0]);

			// Setup
			rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(1, rootParameters, 0, nullptr, rhi::RootSignatureFlags::NONE);

			// Create the instance
			mComputeRootSignature2 = rhi->create_root_signature(rootSignatureBuilder);
		}

		// Create sampler state and wrap it into a resource group instance
		rhi::RHIResource* samplerStateResource = nullptr;
		{
			rhi::SamplerStateDescriptor samplerState = rhi::RHISamplerState::get_default_sampler_state();
			samplerState.maxLod = 0.0f;
			samplerStateResource = rhi->create_sampler_state(samplerState);
			mGraphicsSamplerStateGroup = mGraphicsRootSignature->create_resource_group(1, 1, &samplerStateResource);
		}

		{ // Texture buffer
			static constexpr float VERTEX_POSITION_OFFSET[] =
			{								// Vertex ID	Triangle on screen
				0.5f, -0.5f, 0.0f, 0.0f,	// 0				0
				0.5f, -0.5f, 0.0f, 0.0f,	// 1			   .   .
				0.5f, -0.5f, 0.0f, 0.0f,	// 2			  2.......1
			};

			// Create the texture buffer which will be read by a compute shader
			mComputeInputTextureBuffer = mBufferManager->create_texture_buffer({sizeof(VERTEX_POSITION_OFFSET),
                                                                         VERTEX_POSITION_OFFSET});

			// Create the texture buffer which will be filled by a compute shader
			mComputeOutputTextureBuffer = mBufferManager->create_texture_buffer({sizeof(VERTEX_POSITION_OFFSET), nullptr,
                                                                          rhi::BufferFlag::UNORDERED_ACCESS |
                                                                          rhi::BufferFlag::SHADER_RESOURCE});
		}

		{ // Structured buffer
			struct Vertex final
			{
				float position[2];
				float padding[2];
			};
			static constexpr Vertex VERTICES[] =
			{									// Vertex ID	Triangle on screen
				{ -0.5f, 0.5f, 0.0f, 0.0f },	// 0				0
				{ -0.5f, 0.5f, 0.0f, 0.0f },	// 1			   .   .
				{ -0.5f, 0.5f, 0.0f, 0.0f },	// 2			  2.......1
			};

			// Create the structured buffer which will be read by a compute shader
			mComputeInputStructuredBuffer = mBufferManager->create_structured_buffer({sizeof(VERTICES), VERTICES,
                                                                               rhi::BufferFlag::SHADER_RESOURCE,
                                                                               rhi::BufferUsage::STATIC_DRAW,
                                                                               sizeof(Vertex)});

			// Create the structured buffer which will be filled by a compute shader
			mComputeOutputStructuredBuffer = mBufferManager->create_structured_buffer({sizeof(VERTICES), nullptr,
                                                                                rhi::BufferFlag::UNORDERED_ACCESS |
                                                                                rhi::BufferFlag::SHADER_RESOURCE,
                                                                                rhi::BufferUsage::STATIC_DRAW,
                                                                                sizeof(Vertex)});
		}

		{ // Indirect buffer
			{ // Create the indirect buffer which will be read by a compute shader
				const rhi::DrawIndexedArguments drawIndexedArguments =
				{
					0, // indexCountPerInstance (core::uint32)	- Filled by compute shader via atomics counting
					1, // instanceCount (core::uint32)
					0, // startIndexLocation (core::uint32)
					0, // baseVertexLocation (core::int32)
					0  // startInstanceLocation (core::uint32)
				};
				mComputeInputIndirectBuffer = mBufferManager->create_indirect_buffer({sizeof(rhi::DrawIndexedArguments),
                                                                             &drawIndexedArguments,
                                                                             rhi::IndirectBufferFlag::SHADER_RESOURCE |
                                                                             rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS});
			}

			// Create the indirect buffer which will be filled by a compute shader
			mComputeOutputIndirectBuffer = mBufferManager->create_indirect_buffer({sizeof(rhi::DrawIndexedArguments),
                                                                            nullptr,
                                                                            rhi::IndirectBufferFlag::UNORDERED_ACCESS |
                                                                            rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS});
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

		{ // Create the index buffer object (IBO)
			static constexpr core::uint16 INDICES[] =
			{
				0, 1, 2
			};
			mComputeInputIndexBuffer = mBufferManager->create_index_buffer({sizeof(INDICES), INDICES,
                                                                     rhi::BufferFlag::SHADER_RESOURCE});
			mComputeOutputIndexBuffer = mBufferManager->create_index_buffer({sizeof(INDICES), nullptr,
                                                                      rhi::BufferFlag::UNORDERED_ACCESS});
		}

		{ // Create vertex array object (VAO)
			// Create the vertex buffer object (VBO)
			// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
			static constexpr float VERTEX_POSITION[] =
			{					// Vertex ID	Triangle on screen
				 0.0f, 1.0f,	// 0				0
				 1.0f, 0.0f,	// 1			   .   .
				-0.5f, 0.0f		// 2			  2.......1
			};
			mComputeInputVertexBuffer = mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), VERTEX_POSITION,
                                                                       rhi::BufferFlag::SHADER_RESOURCE});
			mComputeOutputVertexBuffer = mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), nullptr,
                                                                        rhi::BufferFlag::UNORDERED_ACCESS});
		}

		{ // Create vertex array object (VAO)
		  // -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		  // -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		  // -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		  //    reference of the used vertex buffer objects (VBO). If the reference counter of a
		  //    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
			const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { mComputeOutputVertexBuffer };
			mVertexArray = mBufferManager->create_vertex_array({vertexAttributes, 1, vertexArrayVertexBuffers, mComputeOutputIndexBuffer});
		}

		{ // Create the uniform buffer which will be read by a compute shader
			static constexpr float RGBA_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			mComputeInputUniformBuffer = mBufferManager->create_uniform_buffer({sizeof(RGBA_COLOR), RGBA_COLOR});
		}

		{ // Resource group related
			// Create the texture instance, but without providing texture data (we use the texture as render target)
			// -> Use the "rhi::TextureFlag::RENDER_TARGET"-flag to mark this texture as a render target
			// -> Required for Vulkan, Direct3D 9, Direct3D 10, Direct3D 11 and Direct3D 12
			// -> Not required for OpenGL and OpenGL ES 3
			// -> The optimized texture clear value is a Direct3D 12 related option
			const rhi::TextureFormat::Enum textureFormat = rhi::TextureFormat::Enum::R8G8B8A8;
			rhi::RHITexture* computeInputTexture2D = mTextureManager->create_texture_2d(
        {{16, 16},
         rhi::TextureDimension::TextureDimension2D,
         textureFormat,
         rhi::TextureFlag::SHADER_RESOURCE | rhi::TextureFlag::RENDER_TARGET,
         rhi::TextureUsage::DEFAULT, 1},
        nullptr,
        reinterpret_cast<const rhi::OptimizedTextureClearValue *>(&core::Color4::GREEN));
			rhi::RHITexture* computeOutputTexture2D = mTextureManager->create_texture_2d(
        {{16, 16},
         rhi::TextureDimension::TextureDimension2D,
         textureFormat,
         rhi::TextureFlag::SHADER_RESOURCE | rhi::TextureFlag::UNORDERED_ACCESS},
         nullptr);

			{ // Create the framebuffer object (FBO) instance
				const rhi::FramebufferAttachment colorFramebufferAttachment(computeInputTexture2D);
				mFramebuffer = rhi->create_framebuffer(*rhi->create_render_pass(1, &textureFormat), &colorFramebufferAttachment);
			}

			{ // Create first compute resource group
				rhi::RHIResource* resources[7] = {
					// Input
					computeInputTexture2D, mComputeInputVertexBuffer, mComputeInputIndexBuffer, mComputeInputUniformBuffer,
					// Output
					computeOutputTexture2D, mComputeOutputVertexBuffer, mComputeOutputIndexBuffer
				};
				rhi::RHISamplerState* samplerStates[7] = {
					// Input
					static_cast<rhi::RHISamplerState*>(samplerStateResource), nullptr, nullptr, nullptr,
					// Output
					nullptr, nullptr, nullptr
				};
				mComputeResourceGroup1 = mComputeRootSignature1->create_resource_group(0,
                                                                               7, resources, samplerStates);
			}

			{ // Create second compute resource group
				rhi::RHIResource* resources[6] = {
					// Input
					mComputeInputTextureBuffer, mComputeInputStructuredBuffer, mComputeInputIndirectBuffer,
					// Output
					mComputeOutputTextureBuffer, mComputeOutputStructuredBuffer, mComputeOutputIndirectBuffer
				};
				mComputeResourceGroup2 = mComputeRootSignature2->create_resource_group(0,
                                                                               6, resources, nullptr);
			}

			{ // Create graphics resource group
				rhi::RHIResource* resources[4] = { mComputeInputUniformBuffer, mComputeOutputTextureBuffer, mComputeOutputStructuredBuffer, computeOutputTexture2D };
				rhi::RHISamplerState* samplerStates[4] = { nullptr, nullptr, nullptr, static_cast<rhi::RHISamplerState*>(samplerStateResource) };
				mGraphicsResourceGroup = mGraphicsRootSignature->create_resource_group(0,
                                                                               4, resources, samplerStates);
			}
		}

		{
			// Create the graphics program
			rhi::RHIGraphicsProgramPtr graphicsProgram;
			{
				// get the shader source code (outsourced to keep an overview)
				const char* vertexShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
				const char* computeShaderSourceCode1 = nullptr;
				const char* computeShaderSourceCode2 = nullptr;
				#include "compute_shader/shaders.inl"

				// Create the graphics program
				rhi::RHIShaderLanguage& shaderLanguage = rhi->get_default_shader_language();
				graphicsProgram = shaderLanguage.create_graphics_program(
          *mGraphicsRootSignature,
          vertexAttributes,
          shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode),
          shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));

				// Create the compute pipeline state objects (PSO)
				mComputePipelineState1 = rhi->create_compute_pipeline_state(*mComputeRootSignature1,
                                                                    *shaderLanguage.create_compute_shader_from_source_code(
																																			computeShaderSourceCode1));
				mComputePipelineState2 = rhi->create_compute_pipeline_state(*mComputeRootSignature2,
                                                                    *shaderLanguage.create_compute_shader_from_source_code(
																																			computeShaderSourceCode2));
			}

			// Create the graphics pipeline state object (PSO)
			if (nullptr != graphicsProgram)
			{
				mGraphicsPipelineState = rhi->create_graphics_pipeline_state(
          rhi::GraphicsPipelineStateDescriptorBuilder(mGraphicsRootSignature, graphicsProgram, vertexAttributes,
                                                        get_main_render_target()->get_render_pass()));
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fill_command_buffer();
	}
}

void ComputeShader::on_deinitialization()
{
	// release the used resources
	mComputeInputUniformBuffer = nullptr;
	mComputeOutputIndirectBuffer = nullptr;
	mComputeInputIndirectBuffer = nullptr;
	mComputeOutputStructuredBuffer = nullptr;
	mComputeInputStructuredBuffer = nullptr;
	mComputeOutputTextureBuffer = nullptr;
	mComputeInputTextureBuffer = nullptr;
	mVertexArray = nullptr;
	mComputeOutputVertexBuffer = nullptr;
	mComputeInputVertexBuffer = nullptr;
	mComputeOutputIndexBuffer = nullptr;
	mComputeInputIndexBuffer = nullptr;
	mComputePipelineState2 = nullptr;
	mComputePipelineState1 = nullptr;
	mGraphicsPipelineState = nullptr;
	mGraphicsSamplerStateGroup = nullptr;
	mGraphicsResourceGroup = nullptr;
	mComputeResourceGroup2 = nullptr;
	mComputeResourceGroup1 = nullptr;
	mFramebuffer = nullptr;
	mComputeRootSignature2 = nullptr;
	mComputeRootSignature1 = nullptr;
	mGraphicsRootSignature = nullptr;
	mCommandBuffer.clear();
	mTextureManager = nullptr;
	mBufferManager = nullptr;
}

void ComputeShader::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void ComputeShader::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(nullptr != get_main_render_target(), "Invalid main render target")
	BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")
	BE_ASSERT(nullptr != mGraphicsRootSignature, "Invalid graphics root signature")
	BE_ASSERT(nullptr != mComputeRootSignature1, "Invalid compute root signature 1")
	BE_ASSERT(nullptr != mComputeRootSignature2, "Invalid compute root signature 2")
	BE_ASSERT(nullptr != mFramebuffer, "Invalid framebuffer")
	BE_ASSERT(nullptr != mComputeResourceGroup1, "Invalid compute resource group 1")
	BE_ASSERT(nullptr != mComputeResourceGroup2, "Invalid compute resource group 2")
	BE_ASSERT(nullptr != mGraphicsResourceGroup, "Invalid graphics resource group")
	BE_ASSERT(nullptr != mGraphicsSamplerStateGroup, "Invalid graphics sampler state group")
	BE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
	BE_ASSERT(nullptr != mComputePipelineState1, "Invalid compute pipeline state 1")
	BE_ASSERT(nullptr != mComputePipelineState2, "Invalid compute pipeline state 2")
	BE_ASSERT(nullptr != mComputeInputVertexBuffer, "Invalid compute input vertex buffer")
	BE_ASSERT(nullptr != mComputeOutputVertexBuffer, "Invalid compute output vertex buffer")
	BE_ASSERT(nullptr != mComputeInputIndexBuffer, "Invalid compute input index buffer")
	BE_ASSERT(nullptr != mComputeOutputIndexBuffer, "Invalid compute output index buffer")
	BE_ASSERT(nullptr != mVertexArray, "Invalid vertex array")
	BE_ASSERT(nullptr != mComputeOutputTextureBuffer, "Invalid compute output texture buffer")
	BE_ASSERT(nullptr != mComputeInputTextureBuffer, "Invalid compute input texture buffer")
	BE_ASSERT(nullptr != mComputeOutputStructuredBuffer, "Invalid compute output structured buffer")
	BE_ASSERT(nullptr != mComputeInputStructuredBuffer, "Invalid compute input structured buffer")
	BE_ASSERT(nullptr != mComputeOutputIndirectBuffer, "Invalid compute output indirect buffer")
	BE_ASSERT(nullptr != mComputeInputIndirectBuffer, "Invalid compute input indirect buffer")
	BE_ASSERT(nullptr != mComputeInputUniformBuffer, "Invalid compute input uniform buffer")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	{ // Graphics: Render to texture
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

	{ // Compute: Use the graphics render to texture result for compute
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Use the render to texture result for compute")

		// Set the used compute root signature
		rhi::Command::SetComputeRootSignature::create(mCommandBuffer, mComputeRootSignature1);

		// Set the used compute pipeline state object (PSO)
		rhi::Command::SetComputePipelineState::create(mCommandBuffer, mComputePipelineState1);

		// Set compute resource groups
		rhi::Command::SetComputeResourceGroup::create(mCommandBuffer, 0, mComputeResourceGroup1);

		// Dispatch compute call
		rhi::Command::DispatchCompute::create(mCommandBuffer, 1, 1, 1);

		// Repeat everything for the second compute shader
		rhi::Command::SetComputeRootSignature::create(mCommandBuffer, mComputeRootSignature2);
		rhi::Command::SetComputePipelineState::create(mCommandBuffer, mComputePipelineState2);
		rhi::Command::SetComputeResourceGroup::create(mCommandBuffer, 0, mComputeResourceGroup2);
		rhi::Command::DispatchCompute::create(mCommandBuffer, 1, 1, 1);
	}

	{ // Graphics: Use the compute result for graphics
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Use the compute result")

		// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
		rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);

		// Set the used graphics root signature
		rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mGraphicsRootSignature);

		// Set the used graphics pipeline state object (PSO)
		rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

		// Set graphics resource groups
		rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mGraphicsResourceGroup);
		rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mGraphicsSamplerStateGroup);

		// Input assembly (IA): Set the used vertex array
		rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArray);

		// Render the specified geometric primitive, based on indexing into an array of vertices
		rhi::Command::DrawIndexedGraphics::create(mCommandBuffer, *mComputeOutputIndirectBuffer);
	}
}
