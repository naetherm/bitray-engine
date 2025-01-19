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
#include "texture/texture.h"
#include <core/color/color4.h>
#include <stdlib.h> // For rand()


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void Texture::on_initialization()
{
	// get and check the RHI instance
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi)
	{
		// Create the buffer and texture manager
		mBufferManager = rhi->create_buffer_manager();
		mTextureManager = rhi->create_texture_manager();

		{ // Create the root signature
			rhi::DescriptorRangeBuilder ranges[4];
			ranges[0].initialize(rhi::ResourceType::TEXTURE_1D, 0, "GradientMap", rhi::ShaderVisibility::FRAGMENT);
			ranges[1].initialize(rhi::ResourceType::TEXTURE_2D, 1, "AlbedoMap",   rhi::ShaderVisibility::FRAGMENT);
			ranges[2].initializeSampler(0, rhi::ShaderVisibility::FRAGMENT);
			ranges[3].initializeSampler(1, rhi::ShaderVisibility::FRAGMENT);

			rhi::RootParameterBuilder rootParameters[2];
			rootParameters[0].initializeAsDescriptorTable(2, &ranges[0]);
			rootParameters[1].initializeAsDescriptorTable(2, &ranges[2]);

			// Setup
			rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(2, rootParameters, 0, nullptr, rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi->create_root_signature(rootSignatureBuilder);
		}

		// Create sampler state and wrap it into a resource group instance
		rhi::RHISamplerState* linearSamplerResource = nullptr;
		rhi::RHISamplerState* pointSamplerResource = nullptr;
		{
			// Create the sampler resources
			rhi::SamplerStateDescriptor samplerState = rhi::RHISamplerState::get_default_sampler_state();
			samplerState.addressU = rhi::TextureAddressMode::WRAP;
			samplerState.addressV = rhi::TextureAddressMode::WRAP;
			linearSamplerResource = rhi->create_sampler_state(samplerState);
			samplerState.filter = rhi::FilterMode::MIN_MAG_MIP_POINT;
			pointSamplerResource = rhi->create_sampler_state(samplerState);

			// Create the resource group
			rhi::RHIResource* resources[2] = { linearSamplerResource, pointSamplerResource };
			mSamplerStateGroup = mRootSignature->create_resource_group(1, 2,
                                                                 resources);
		}

		{ // Create the texture group
			rhi::RHIResource* resources[2];

			{ // Create the 1D texture
				static constexpr core::uint32 TEXTURE_WIDTH   = 256;
				static constexpr core::uint32 TEXEL_ELEMENTS  = 1;
				static constexpr core::uint32 NUMBER_OF_BYTES = TEXTURE_WIDTH;
				core::uint8 data[NUMBER_OF_BYTES];

				// Fill the texture data with a color gradient
				for (core::uint32 n = 0; n < NUMBER_OF_BYTES; n += TEXEL_ELEMENTS)
				{
					data[n] = static_cast<core::uint8>(n);
				}

				// Create the texture instance
				resources[0] = mTextureManager->create_texture_1d(
          {
            {TEXTURE_WIDTH, 0, 0},
            rhi::TextureDimension::TextureDimension1D,
            rhi::TextureFormat::R8,
            rhi::TextureFlag::GENERATE_MIPMAPS | rhi::TextureFlag::SHADER_RESOURCE
          },
          data);
			}

			{ // Create the 2D texture
				static constexpr core::uint32 TEXTURE_WIDTH   = 64;
				static constexpr core::uint32 TEXTURE_HEIGHT  = 64;
				static constexpr core::uint32 TEXEL_ELEMENTS  = 4;
				static constexpr core::uint32 NUMBER_OF_BYTES = TEXTURE_WIDTH * TEXTURE_HEIGHT * TEXEL_ELEMENTS;
				core::uint8 data[NUMBER_OF_BYTES];

				{ // Fill the texture data with a defective checkboard
					const core::uint32 rowPitch   = TEXTURE_WIDTH * TEXEL_ELEMENTS;
					const core::uint32 cellPitch  = rowPitch >> 3;		// The width of a cell in the checkboard texture
					const core::uint32 cellHeight = TEXTURE_WIDTH >> 3;	// The height of a cell in the checkerboard texture
					for (core::uint32 n = 0; n < NUMBER_OF_BYTES; n += TEXEL_ELEMENTS)
					{
						const core::uint32 x = n % rowPitch;
						const core::uint32 y = n / rowPitch;
						const core::uint32 i = x / cellPitch;
						const core::uint32 j = y / cellHeight;

						if (i % 2 == j % 2)
						{
							// Black
							data[n + 0] = 0;	// R
							data[n + 1] = 0;	// G
							data[n + 2] = 0;	// B
							data[n + 3] = 255;	// A
						}
						else
						{
							// Add some color fun instead of just boring white
							data[n + 0] = static_cast<core::uint8>(rand() % 255);	// R
							data[n + 1] = static_cast<core::uint8>(rand() % 255);	// G
							data[n + 2] = static_cast<core::uint8>(rand() % 255);	// B
							data[n + 3] = static_cast<core::uint8>(rand() % 255);	// A
						}
					}
				}

				// Create the texture instance
				resources[1] = mTextureManager->create_texture_2d(
          {{TEXTURE_WIDTH, TEXTURE_HEIGHT},
           rhi::TextureDimension::TextureDimension2D,
           rhi::TextureFormat::R8G8B8A8,
           rhi::TextureFlag::GENERATE_MIPMAPS | rhi::TextureFlag::SHADER_RESOURCE},
           data);
			}

			// Create the texture group
			rhi::RHISamplerState* samplerStates[2] = { linearSamplerResource, pointSamplerResource };
			mTextureGroup = mRootSignature->create_resource_group(0, 2,
                                                            resources, samplerStates);
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
				#include "texture/shaders.inl"

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

void Texture::on_deinitialization()
{
	// release the used resources
	mVertexArray = nullptr;
	mGraphicsPipelineState = nullptr;
	mSamplerStateGroup = nullptr;
	mTextureGroup = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mTextureManager = nullptr;
	mBufferManager = nullptr;
}

void Texture::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void Texture::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mTextureGroup, "Invalid texture group")
	BE_ASSERT(nullptr != mSamplerStateGroup, "Invalid sampler state group")
	BE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
	BE_ASSERT(nullptr != mVertexArray, "Invalid vertex array")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

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
