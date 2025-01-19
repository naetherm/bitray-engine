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
#include "cube_texture/cube_texture.h"
#include <core/color/color4.h>
#include <core/math/mat44.h>
#include <stdlib.h> // For rand()


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void CubeTexture::on_initialization()
{
	// get and check the RHI instance
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi)
	{
		const bool textureCubeArraySupported = (rhi->get_capabilities().maximumNumberOfCubeTextureArraySlices > 0);

		// Create the buffer and texture manager
		mBufferManager = rhi->create_buffer_manager();
		mTextureManager = rhi->create_texture_manager();

		{ // Create the root signature
			rhi::DescriptorRangeBuilder ranges[4];
			ranges[0].initialize(rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicVs",	rhi::ShaderVisibility::VERTEX);
			if (textureCubeArraySupported)
			{
				ranges[1].initialize(rhi::ResourceType::TEXTURE_CUBE_ARRAY, 0, "CubeMapArray", rhi::ShaderVisibility::FRAGMENT);
			}
			else
			{
				ranges[1].initialize(rhi::ResourceType::TEXTURE_CUBE, 0, "CubeMap", rhi::ShaderVisibility::FRAGMENT);
			}
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

		// Create uniform buffer
		// -> Direct3D 9 does not support uniform buffers
		// -> Direct3D 10, 11 and 12 do not support individual uniforms
		// -> The RHI is just a light weight abstraction layer, so we need to handle the differences
		// -> Allocate enough memory for one 4x4 floating point matrix
		if (0 != rhi->get_capabilities().maximumUniformBufferSize)
		{
			mUniformBuffer = mBufferManager->create_uniform_buffer({4 * 4 * sizeof(float), nullptr,
                                                             rhi::BufferUsage::DYNAMIC_DRAW});
		}

		// Create sampler state and wrap it into a resource group instance
		rhi::RHISamplerState* pointSamplerResource = nullptr;
		{
			// Create the sampler resources
			rhi::SamplerStateDescriptor samplerState = rhi::RHISamplerState::get_default_sampler_state();
			samplerState.addressU = rhi::TextureAddressMode::WRAP;
			samplerState.addressV = rhi::TextureAddressMode::WRAP;
			samplerState.addressW = rhi::TextureAddressMode::WRAP;
			samplerState.filter = rhi::FilterMode::MIN_MAG_MIP_POINT;
			pointSamplerResource = rhi->create_sampler_state(samplerState);

			// Create the resource group
			rhi::RHIResource* resources[1] = { pointSamplerResource };
			mSamplerStateGroup = mRootSignature->create_resource_group(1, 1,
                                                                 resources);
		}

		{ // Create the texture group
			rhi::RHIResource* resources[2];
			resources[0] = mUniformBuffer;

			{ // Create the cube texture
				static constexpr core::uint32 TEXTURE_WIDTH = 1;
				static constexpr core::uint32 TEXEL_ELEMENTS = 4;
				static constexpr core::uint32 NUMBER_OF_BYTES_PER_SLICE = TEXTURE_WIDTH * TEXTURE_WIDTH * TEXEL_ELEMENTS * 6;
				static constexpr core::uint32 NUMBER_OF_BYTES = NUMBER_OF_BYTES_PER_SLICE * 2;
				core::uint8 data[NUMBER_OF_BYTES] = {};
				core::uint8* currentData = data + NUMBER_OF_BYTES_PER_SLICE;	// The first cube slice will be black

				// Face 0 = positive X = red
				currentData[0] = 255;
				currentData[1] = 0;
				currentData[2] = 0;
				currentData[3] = 0;
				currentData += TEXEL_ELEMENTS;

				// Face 1 = negative X = dark red
				currentData[0] = 127;
				currentData[1] = 0;
				currentData[2] = 0;
				currentData[3] = 0;
				currentData += TEXEL_ELEMENTS;

				// Face 2 = positive Y = green
				currentData[0] = 0;
				currentData[1] = 255;
				currentData[2] = 0;
				currentData[3] = 0;
				currentData += TEXEL_ELEMENTS;

				// Face 3 = negative Y = dark green
				currentData[0] = 0;
				currentData[1] = 127;
				currentData[2] = 0;
				currentData[3] = 0;
				currentData += TEXEL_ELEMENTS;

				// Face 4 = positive Z = blue
				currentData[0] = 0;
				currentData[1] = 0;
				currentData[2] = 255;
				currentData[3] = 0;
				currentData += TEXEL_ELEMENTS;

				// Face 5 = negative Z = dark blue
				currentData[0] = 0;
				currentData[1] = 0;
				currentData[2] = 127;
				currentData[3] = 0;

				// Create the texture instance
				if (textureCubeArraySupported)
				{
					resources[1] = mTextureManager->create_texture_cube_array(
            {{TEXTURE_WIDTH, 2},
             rhi::TextureDimension::TextureDimension3D,
             rhi::TextureFormat::R8G8B8A8,
             rhi::TextureFlag::SHADER_RESOURCE},
             data);
				}
				else
				{
					resources[1] = mTextureManager->create_texture_cube(
            {{TEXTURE_WIDTH},
             rhi::TextureDimension::TextureDimension2D,
             rhi::TextureFormat::R8G8B8A8,
             rhi::TextureFlag::SHADER_RESOURCE},
             data + NUMBER_OF_BYTES_PER_SLICE);
				}
			}

			// Create the texture group
			rhi::RHISamplerState* samplerStates[2] = { nullptr, pointSamplerResource };
			mTextureGroup = mRootSignature->create_resource_group(0, 2,
                                                            resources, samplerStates);
		}

		// Vertex input layout
		static constexpr rhi::VertexAttribute vertexAttributesLayout[] =
		{
			{ // Attribute 0
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"Position",								// name[32] (char)
				"POSITION",								// semanticName[32] (char)
				0,										// semanticIndex (core::uint32)
				// Data source
				0,										// inputSlot (core::uint32)
				0,										// alignedByteOffset (core::uint32)
				sizeof(float) * 3,						// strideInBytes (core::uint32)
				0										// instancesPerElement (core::uint32)
			}
		};
		const rhi::VertexAttributes vertexAttributes(1, vertexAttributesLayout);

		{ // Create vertex array object (VAO)
			// Our cube is constructed like this
			/*
					3+------+2  y
					/|     /|   |
				   / |    / |   |
				  / 0+---/--+1  *---x
				7+------+6 /   /
				 | /    | /   z
				 |/     |/
				4+------+5
			*/

			// Create the vertex buffer object (VBO)
			// -> Object space vertex positions
			static constexpr float VERTEX_POSITION[] =
			{
				-0.5f, -0.5f, -0.5f,	// 0
				 0.5f, -0.5f, -0.5f,	// 1
				 0.5f,  0.5f, -0.5f,	// 2
				-0.5f,  0.5f, -0.5f,	// 3
				-0.5f, -0.5f,  0.5f,	// 4
				 0.5f, -0.5f,  0.5f,	// 5
				 0.5f,  0.5f,  0.5f,	// 6
				-0.5f,  0.5f,  0.5f,	// 7
			};
			rhi::RHIVertexBufferPtr vertexBuffer(
        mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), VERTEX_POSITION, 0,
                                             rhi::BufferUsage::STATIC_DRAW}));

			// Create the index buffer object (IBO)
			static constexpr core::uint16 INDICES[] =
			{
				// Back		Triangle
				0, 3, 2,	// 0
				2, 1, 0,	// 1
				// Front
				5, 6, 7,	// 0
				7, 4, 5,	// 1
				// Left
				4, 7, 3,	// 0
				3, 0, 4,	// 1
				// Right
				1, 2, 6,	// 0
				6, 5, 1,	// 1
				// Top
				6, 2, 3,	// 0
				3, 7, 6,	// 1
				// Bottom
				5, 4, 0,	// 0
				0, 1, 5		// 1
			};
			rhi::RHIIndexBufferPtr indexBuffer(
        mBufferManager->create_index_buffer({sizeof(INDICES), INDICES, 0, rhi::BufferUsage::STATIC_DRAW,
                                            rhi::IndexBufferFormat::UNSIGNED_SHORT}));

			// Create vertex array object (VAO)
			// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
			// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
			// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
			//    reference of the used vertex buffer objects (VBO). If the reference counter of a
			//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
			const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
			mVertexArray = mBufferManager->create_vertex_array({vertexAttributes, 1, vertexArrayVertexBuffers, indexBuffer});
		}

		{ // Create the graphics program
			// get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode = nullptr;
			#include "cube_texture/shaders.inl"

			// Create the graphics program
			rhi::RHIShaderLanguage& shaderLanguage = rhi->get_default_shader_language();
			mGraphicsProgram = shaderLanguage.create_graphics_program(
        *mRootSignature,
        vertexAttributes,
        shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode),
				shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));
		}

		// Create the graphics pipeline state object (PSO)
		if (nullptr != mGraphicsProgram)
		{
			mGraphicsPipelineState = rhi->create_graphics_pipeline_state(
        rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, mGraphicsProgram, vertexAttributes,
                                                      get_main_render_target()->get_render_pass()));

			// Optimization: Cached data to not bother the RHI too much
			if (nullptr == mUniformBuffer)
			{
				mObjectSpaceToClipSpaceMatrixUniformHandle = mGraphicsProgram->get_uniform_handle(
          "ObjectSpaceToClipSpaceMatrix");
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fill_command_buffer();
	}
}

void CubeTexture::on_deinitialization()
{
	// release the used resources
	mObjectSpaceToClipSpaceMatrixUniformHandle = NULL_HANDLE;
	mVertexArray = nullptr;
	mGraphicsProgram = nullptr;
	mGraphicsPipelineState = nullptr;
	mSamplerStateGroup = nullptr;
	mTextureGroup = nullptr;
	mUniformBuffer = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mTextureManager = nullptr;
	mBufferManager = nullptr;
}

void CubeTexture::on_update()
{
	// stop the stopwatch
	mStopwatch.stop();

	// Update the global timer (FPS independent movement)
	mGlobalTimer += mStopwatch.get_milliseconds() * 0.0005f;

	// start the stopwatch
	mStopwatch.start();
}

void CubeTexture::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	{ // Set uniform
		// get the aspect ratio
		float aspectRatio = 4.0f / 3.0f;
		float w, h;
		{
			// get the render target with and height
			const rhi::RHIRenderTarget* renderTarget = get_main_render_target();
			if (nullptr != renderTarget)
			{
				core::uint32 width  = 1;
				core::uint32 height = 1;
        renderTarget->get_width_and_height(width, height);

				w = width;
				h = height;
				// get the aspect ratio
				aspectRatio = static_cast<float>(width) / static_cast<float>(height);
			}
		}

		// Calculate the object space to clip space matrix
    const core::Mat44f viewSpaceToClipSpace	= core::Mat44f::create_perspective(45.0f, aspectRatio, 100.0f, 0.1f);	// Near and far flipped due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
    const core::Mat44f viewTranslate			= core::Mat44f::create_translation(core::Vec3f(0.0f, 0.0f, 3.0f));
    core::Mat44f worldSpaceToViewSpace	= viewTranslate.get_rotated(core::Vec3f(1.0f, 1.0f, 0.0f), mGlobalTimer);
    const core::Mat44f objectSpaceToWorldSpace	= core::Mat44f::create_scale(core::Vec3f(0.5f));
    core::Mat44f objectSpaceToViewSpace  = worldSpaceToViewSpace * objectSpaceToWorldSpace;
    const core::Mat44f objectSpaceToClipSpace	= viewSpaceToClipSpace * objectSpaceToViewSpace;

		// Upload the uniform data
		// -> Two versions: One using an uniform buffer and one setting an individual uniform
		if (nullptr != mUniformBuffer)
		{
			// Copy data into the uniform buffer
			rhi::Command::CopyUniformBufferData::create(commandBuffer, *mUniformBuffer, (objectSpaceToClipSpace), sizeof(float) * 4 * 4);
		}
		else
		{
			// Set legacy uniforms
			rhi::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToClipSpaceMatrixUniformHandle, (objectSpaceToClipSpace));
		}
	}

	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void CubeTexture::fill_command_buffer()
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
	rhi::Command::DrawIndexedGraphics::create(mCommandBuffer, 36);
}
