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
#include "instanced_cubes/cube_renderer_instanced_arrays/cube_renderer_instanced_arrays.h"
#include "instanced_cubes/cube_renderer_instanced_arrays/batch_instanced_arrays.h"
#include <core/log/log.h>
#include <math.h>
#include <float.h>	// For "FLT_MAX"
#include <stdlib.h>	// For "rand()"


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		// Vertex input layout
		static constexpr rhi::VertexAttribute CubeRendererInstancedArraysVertexAttributesLayout[] =
		{
			// Mesh data
			{ // Attribute 0
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"Position",								// name[32] (char)
				"POSITION",								// semanticName[32] (char)
				0,										// semanticIndex (core::uint32)
				// Data source
				0,										// inputSlot (core::uint32)
				0,										// alignedByteOffset (core::uint32)
				sizeof(float) * 8,						// strideInBytes (core::uint32)
				0										// instancesPerElement (core::uint32)
			},
			{ // Attribute 1
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"TexCoord",								// name[32] (char)
				"TEXCOORD",								// semanticName[32] (char)
				0,										// semanticIndex (core::uint32)
				// Data source
				0,										// inputSlot (core::uint32)
				sizeof(float) * 3,						// alignedByteOffset (core::uint32)
				sizeof(float) * 8,						// strideInBytes (core::uint32)
				0										// instancesPerElement (core::uint32)
			},
			{ // Attribute 2
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"Normal",								// name[32] (char)
				"NORMAL",								// semanticName[32] (char)
				0,										// semanticIndex (core::uint32)
				// Data source
				0,										// inputSlot (core::uint32)
				sizeof(float) * 5,						// alignedByteOffset (core::uint32)
				sizeof(float) * 8,						// strideInBytes (core::uint32)
				0										// instancesPerElement (core::uint32)
			},

			// Per-instance data
			{ // Attribute 3
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_4,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"PerInstancePositionTexture",			// name[32] (char)
				"TEXCOORD",								// semanticName[32] (char)
				1,										// semanticIndex (core::uint32)
				// Data source
				1,										// inputSlot (core::uint32)
				0,										// alignedByteOffset (core::uint32)
				sizeof(float) * 8,						// strideInBytes (core::uint32)
				1										// instancesPerElement (core::uint32)
			},
			{ // Attribute 4
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_4,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"PerInstanceRotationScale",				// name[32] (char)
				"TEXCOORD",								// semanticName[32] (char)
				2,										// semanticIndex (core::uint32)
				// Data source
				1,										// inputSlot (core::uint32)
				sizeof(float) * 4,						// alignedByteOffset (core::uint32)
				sizeof(float) * 8,						// strideInBytes (core::uint32)
				1										// instancesPerElement (core::uint32)
			}
		};
		const rhi::VertexAttributes CubeRendererInstancedArraysVertexAttributes(5, CubeRendererInstancedArraysVertexAttributesLayout);


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
CubeRendererInstancedArrays::CubeRendererInstancedArrays(rhi::RHIDevice& rhi, rhi::RHIRenderPass& renderPass, core::uint32 numberOfTextures, core::uint32 sceneRadius) :
	mRhi(&rhi),
	mRenderPass(renderPass),
	mNumberOfTextures(numberOfTextures),
	mSceneRadius(sceneRadius),
	mMaximumNumberOfInstancesPerBatch(0),
	mNumberOfBatches(0),
	mBatches(nullptr)
{
	// Create the buffer and texture manager
	mBufferManager = mRhi->create_buffer_manager();
	mTextureManager = mRhi->create_texture_manager();

	// Check number of textures (limit of this implementation and RHI limit)
	if (mNumberOfTextures > MAXIMUM_NUMBER_OF_TEXTURES)
	{
		mNumberOfTextures = MAXIMUM_NUMBER_OF_TEXTURES;
	}

	// get the maximum number of instances per batch
	// -> When using instanced arrays, the limit is the available memory for a vertex buffer
	// -> To be on the safe side and not bumping into a limitation of less capable cards we set a decent maximum number of instances per batch
	mMaximumNumberOfInstancesPerBatch = 65536;

	{ // Create the root signature
		rhi::DescriptorRangeBuilder ranges[5];
		ranges[0].initialize(rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockStaticVs",  rhi::ShaderVisibility::VERTEX);
		ranges[1].initialize(rhi::ResourceType::UNIFORM_BUFFER, 1, "UniformBlockDynamicVs", rhi::ShaderVisibility::VERTEX);
		ranges[2].initialize(rhi::ResourceType::TEXTURE_2D,		0, "AlbedoMap",				rhi::ShaderVisibility::FRAGMENT);
		ranges[3].initialize(rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicFs", rhi::ShaderVisibility::FRAGMENT);
		ranges[4].initializeSampler(0, rhi::ShaderVisibility::FRAGMENT);

		rhi::RootParameterBuilder rootParameters[3];
		rootParameters[0].initializeAsDescriptorTable(2, &ranges[0]);
		rootParameters[1].initializeAsDescriptorTable(2, &ranges[2]);
		rootParameters[2].initializeAsDescriptorTable(1, &ranges[4]);

		// Setup
		rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
		rootSignatureBuilder.initialize(4, rootParameters, 0, nullptr, rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		mRootSignature = mRhi->create_root_signature(rootSignatureBuilder);
	}

	{ // Create the textures
		static constexpr core::uint32 TEXTURE_WIDTH   = 128;
		static constexpr core::uint32 TEXTURE_HEIGHT  = 128;
		static constexpr core::uint32 NUMBER_OF_BYTES = TEXTURE_WIDTH * TEXTURE_HEIGHT * 4;

		// Allocate memory for the 2D texture
		core::uint8* data = new core::uint8[NUMBER_OF_BYTES * mNumberOfTextures];

		{ // Fill the texture content
			// TODO(naetherm) Be a little bit more creative while filling the texture data
			core::uint8* RESTRICT dataCurrent = data;
			const float colors[][MAXIMUM_NUMBER_OF_TEXTURES] =
			{
				{ 1.0f, 0.0f, 0.0f},
				{ 0.0f, 0.1f, 0.0f},
				{ 0.0f, 0.0f, 0.1f},
				{ 0.5f, 0.5f, 0.5f},
				{ 1.0f, 1.0f, 1.0f},
				{ 0.1f, 0.2f, 0.2f},
				{ 0.2f, 0.5f, 0.5f},
				{ 0.1f, 0.8f, 0.2f}
			};
			for (core::uint32 j = 0; j < mNumberOfTextures; ++j)
			{
				// Random content
				for (core::uint32 i = 0; i < TEXTURE_WIDTH * TEXTURE_HEIGHT; ++i)
				{
					*dataCurrent = static_cast<core::uint8>(static_cast<float>(rand() % 255) * colors[j][0]);
					++dataCurrent;
					*dataCurrent = static_cast<core::uint8>(static_cast<float>(rand() % 255) * colors[j][1]);
					++dataCurrent;
					*dataCurrent = static_cast<core::uint8>(static_cast<float>(rand() % 255) * colors[j][2]);
					++dataCurrent;
					*dataCurrent = 255;
					++dataCurrent;
				}
			}
		}

		// Create the texture instance
		// -> This implementation has to support Direct3D 9 which has no 2D array textures
		// -> We simply create a single simple 2D texture atlas with the textures aligned along the vertical axis
		mTexture2D = mTextureManager->create_texture_2d(
      {{TEXTURE_WIDTH, TEXTURE_HEIGHT * mNumberOfTextures},
       rhi::TextureDimension::TextureDimension2D,
       rhi::TextureFormat::R8G8B8A8,
       rhi::TextureFlag::GENERATE_MIPMAPS | rhi::TextureFlag::SHADER_RESOURCE},
       data);

		// Free texture memory
		delete [] data;
	}

	// Create sampler state instance and wrap it into a resource group instance
	rhi::RHIResource* samplerStateResource = mRhi->create_sampler_state(
		rhi::RHISamplerState::get_default_sampler_state());
	mSamplerStateGroup = mRootSignature->create_resource_group(1, 1, &samplerStateResource);

	// Uniform buffer object (UBO, "constant buffer" in Direct3D terminology) supported?
	// -> If they are there, we really want to use them (performance and ease of use)
	if (mRhi->get_capabilities().maximumUniformBufferSize > 0)
	{
		{ // Create and set constant graphics program uniform buffer at once
			// TODO(naetherm) Ugly fixed hacked in model-view-projection matrix
			// TODO(naetherm) OpenGL matrix, Direct3D has minor differences within the projection matrix we have to compensate
			static constexpr float MVP[] =
			{
				 1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
				 0.0f,			 1.9776078f,	-0.57472473f,	-0.573576453f,
				-1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
				 0.0f,			 0.0f,			 9.8198195f,	 10.0f
			};
			mUniformBufferStaticVs = mBufferManager->create_uniform_buffer({sizeof(MVP), MVP});
		}

		// Create dynamic uniform buffers
		mUniformBufferDynamicVs = mBufferManager->create_uniform_buffer({sizeof(float) * 2, nullptr,
                                                                    rhi::BufferUsage::DYNAMIC_DRAW});
		mUniformBufferDynamicFs = mBufferManager->create_uniform_buffer({sizeof(float) * 3, nullptr,
                                                                    rhi::BufferUsage::DYNAMIC_DRAW});
	}

	{ // Create resource group with vertex shader visibility
		rhi::RHIResource* resources[2] = { mUniformBufferStaticVs, mUniformBufferDynamicVs };
		mResourceGroupVs = mRootSignature->create_resource_group(0, 2,
                                                             resources);
	}

	{ // Create resource group with fragment shader visibility
		rhi::RHIResource* resources[2] = { mTexture2D, mUniformBufferDynamicFs };
		rhi::RHISamplerState* samplerStates[2] = { static_cast<rhi::RHISamplerState*>(samplerStateResource), nullptr };
		mResourceGroupFs = mRootSignature->create_resource_group(1, 2,
                                                             resources, samplerStates);
	}

	{
		// get the shader source code (outsourced to keep an overview)
		const char* vertexShaderSourceCode = nullptr;
		const char* fragmentShaderSourceCode = nullptr;
		#include "instanced_cubes/cube_renderer_instanced_arrays/shaders.inl"

		// Create the graphics program
		rhi::RHIShaderLanguage& shaderLanguage = mRhi->get_default_shader_language();
		mGraphicsProgram = shaderLanguage.create_graphics_program(
      *mRootSignature,
      detail::CubeRendererInstancedArraysVertexAttributes,
      shaderLanguage.create_vertex_shader_from_source_code(detail::CubeRendererInstancedArraysVertexAttributes,
                                                           vertexShaderSourceCode),
			shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));
	}

	{ // Create the vertex buffer object (VBO)
		static constexpr float VERTEX_POSITION[] =
		{
			// Front face
			// Position					TexCoord		 Normal				// Vertex ID
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 0.0f, 0.0f, 1.0f,	// 0
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,	// 1
			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		 0.0f, 0.0f, 1.0f,	// 2
			-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,		 0.0f, 0.0f, 1.0f,	// 3
			// Back face
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		 0.0f, 0.0f,-1.0f,	// 4
			-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 0.0f, 0.0f,-1.0f,	// 5
			 0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		 0.0f, 0.0f,-1.0f, 	// 6
			 0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		 0.0f, 0.0f,-1.0f,	// 7
			// Top face
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		 0.0f, 1.0f, 0.0f,	// 8
			-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,		 0.0f, 1.0f, 0.0f,	// 9
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		 0.0f, 1.0f, 0.0f,	// 10
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 0.0f, 1.0f, 0.0f,	// 11
			// Bottom face
			-0.5f, -0.5f, -0.5f,		1.0f, 1.0f,		 0.0f,-1.0f, 0.0f,	// 12
			 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		 0.0f,-1.0f, 0.0f,	// 13
			 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 0.0f,-1.0f, 0.0f,	// 14
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		 0.0f,-1.0f, 0.0f,	// 15
			// Right face
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		 1.0f, 0.0f, 0.0f,	// 16
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 1.0f, 0.0f, 0.0f,	// 17
			 0.5f,  0.5f,  0.5f,		0.0f, 1.0f,		 1.0f, 0.0f, 0.0f,	// 18
			 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 1.0f, 0.0f, 0.0f,	// 19
			// Left face
			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	// 20
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	// 21
			-0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	// 22
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f	// 23
		};
		mVertexBuffer = mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), VERTEX_POSITION});
	}

	{ // Create the index buffer object (IBO)
		static constexpr core::uint16 INDICES[] =
		{
			// Front face	Triangle ID
			 1,  0,  2,		// 0
			 3,  2,  0,		// 1
			// Back face
			 6,  5,  4,		// 2
			 4,  7,  6,		// 3
			// Top face
			 9,  8, 10,		// 4
			11, 10,  8,		// 5
			// Bottom face
			13, 12, 14,		// 6
			15, 14, 12,		// 7
			// Right face
			17, 16, 18,		// 8
			19, 18, 16,		// 9
			// Left face
			21, 20, 22,		// 10
			23, 22, 20		// 11
		};
		mIndexBuffer = mBufferManager->create_index_buffer({sizeof(INDICES), INDICES});
	}
}

CubeRendererInstancedArrays::~CubeRendererInstancedArrays()
{
	// The RHI resource pointers are released automatically

	// Destroy the batches, if needed
	delete [] mBatches;
}


//[-------------------------------------------------------]
//[ Public virtual ICubeRenderer methods                  ]
//[-------------------------------------------------------]
void CubeRendererInstancedArrays::setNumberOfCubes(core::uint32 numberOfCubes)
{
	// Destroy previous batches, in case there are any
	if (nullptr != mBatches)
	{
		delete [] mBatches;
		mNumberOfBatches = 0;
		mBatches = nullptr;
	}

	// A third of the cubes should be rendered using alpha blending
	const core::uint32 numberOfTransparentCubes = numberOfCubes / 3;
	const core::uint32 numberOfSolidCubes       = numberOfCubes - numberOfTransparentCubes;

	// There's a limitation how many instances can be created per draw call
	// -> If required, create multiple batches
	const core::uint32 numberOfSolidBatches       = static_cast<core::uint32>(ceil(static_cast<float>(numberOfSolidCubes)       / static_cast<float>(mMaximumNumberOfInstancesPerBatch)));
	const core::uint32 numberOfTransparentBatches = static_cast<core::uint32>(ceil(static_cast<float>(numberOfTransparentCubes) / static_cast<float>(mMaximumNumberOfInstancesPerBatch)));

	// Create a batch instances
	mNumberOfBatches = numberOfSolidBatches + numberOfTransparentBatches;
	mBatches = new BatchInstancedArrays[mNumberOfBatches];

	// Initialize the solid batches
	BatchInstancedArrays* batch     = mBatches;
	BatchInstancedArrays* lastBatch = mBatches + numberOfSolidBatches;
	for (int remaningNumberOfCubes = static_cast<int>(numberOfSolidCubes); batch < lastBatch; ++batch, remaningNumberOfCubes -= mMaximumNumberOfInstancesPerBatch)
	{
		const core::uint32 currentNumberOfCubes = (remaningNumberOfCubes > static_cast<int>(mMaximumNumberOfInstancesPerBatch)) ? mMaximumNumberOfInstancesPerBatch : remaningNumberOfCubes;
		batch->initialize(*mBufferManager, *mRootSignature, detail::CubeRendererInstancedArraysVertexAttributes, *mVertexBuffer, *mIndexBuffer, *mGraphicsProgram, mRenderPass, currentNumberOfCubes, false, mNumberOfTextures, mSceneRadius);
	}

	// Initialize the transparent batches
	// -> TODO(naetherm) For correct alpha blending, the transparent instances should be sorted from back to front.
	lastBatch = batch + numberOfTransparentBatches;
	for (int remaningNumberOfCubes = static_cast<int>(numberOfTransparentCubes); batch < lastBatch; ++batch, remaningNumberOfCubes -= mMaximumNumberOfInstancesPerBatch)
	{
		const core::uint32 currentNumberOfCubes = (remaningNumberOfCubes > static_cast<int>(mMaximumNumberOfInstancesPerBatch)) ? mMaximumNumberOfInstancesPerBatch : remaningNumberOfCubes;
		batch->initialize(*mBufferManager, *mRootSignature, detail::CubeRendererInstancedArraysVertexAttributes, *mVertexBuffer, *mIndexBuffer, *mGraphicsProgram, mRenderPass, currentNumberOfCubes, true, mNumberOfTextures, mSceneRadius);
	}

	// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	mCommandBuffer.clear();
	fillReusableCommandBuffer();
}

void CubeRendererInstancedArrays::fillCommandBuffer(float globalTimer, float globalScale, float lightPositionX, float lightPositionY, float lightPositionZ, rhi::RHICommandBuffer& commandBuffer)
{
	// Sanity check
	BE_ASSERT(nullptr != mGraphicsProgram, "Invalid graphics program")

	{ // Update graphics program uniform data
		// Some counting timer, we don't want to touch the buffers on the GPU
		const float timerAndGlobalScale[] = { globalTimer, globalScale };

		// Animate the point light world space position
		const float lightPosition[] = { lightPositionX, lightPositionY, lightPositionZ };

		// Use uniform buffer?
		if (nullptr != mUniformBufferDynamicVs)
		{
			// Copy data into the uniform buffer
			rhi::Command::CopyUniformBufferData::create(commandBuffer, *mUniformBufferDynamicVs, timerAndGlobalScale, sizeof(timerAndGlobalScale));
			if (nullptr != mUniformBufferDynamicFs)
			{
				rhi::Command::CopyUniformBufferData::create(commandBuffer, *mUniformBufferDynamicFs, lightPosition, sizeof(lightPosition));
			}
		}
		else
		{
			// Set individual graphics program uniforms
			// -> Using uniform buffers (aka constant buffers in Direct3D) would be more efficient, but Direct3D 9 doesn't support it (neither does e.g. OpenGL ES 3.0)
			// -> To keep it simple in here, I just use a less performant string to identity the uniform (does not really hurt in here)
			rhi::Command::SetUniform::create2fv(commandBuffer, *mGraphicsProgram,
                                            mGraphicsProgram->get_uniform_handle("TimerAndGlobalScale"), timerAndGlobalScale);
			rhi::Command::SetUniform::create3fv(commandBuffer, *mGraphicsProgram,
                                            mGraphicsProgram->get_uniform_handle("LightPosition"), lightPosition);
		}
	}

	// Set constant graphics program uniform
	if (nullptr == mUniformBufferStaticVs)
	{
		// TODO(naetherm) Ugly fixed hacked in model-view-projection matrix
		// TODO(naetherm) OpenGL matrix, Direct3D has minor differences within the projection matrix we have to compensate
		static constexpr float MVP[] =
		{
			 1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
			 0.0f,			 1.9776078f,	-0.57472473f,	-0.573576453f,
			-1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
			 0.0f,			 0.0f,			 9.8198195f,	 10.0f
		};

		// There's no uniform buffer: We have to set individual uniforms
		rhi::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram,
                                                mGraphicsProgram->get_uniform_handle("MVP"), MVP);
	}

	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void CubeRendererInstancedArrays::fillReusableCommandBuffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != mRhi, "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mTexture2D, "Invalid texture 2D")
	BE_ASSERT(0 == mRhi->get_capabilities().maximumUniformBufferSize || nullptr != mUniformBufferStaticVs, "Invalid uniform buffer static VS")
	BE_ASSERT(0 == mRhi->get_capabilities().maximumUniformBufferSize || nullptr != mUniformBufferDynamicVs, "Invalid uniform buffer dynamic VS")
	BE_ASSERT(0 == mRhi->get_capabilities().maximumUniformBufferSize || nullptr != mUniformBufferDynamicFs, "Invalid uniform buffer dynamic FS")
	BE_ASSERT(nullptr != mResourceGroupVs && nullptr != mResourceGroupFs, "Invalid resource group")
	BE_ASSERT(nullptr != mSamplerStateGroup, "Invalid sampler state group")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Set the used graphics root signature
	rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set resource groups
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mResourceGroupVs);
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mResourceGroupFs);
	rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 2, mSamplerStateGroup);

	// Draw the batches
	if (nullptr != mBatches)
	{
		// Loop though all batches
		BatchInstancedArrays* batch     = mBatches;
		BatchInstancedArrays* lastBatch = mBatches + mNumberOfBatches;
		for (; batch < lastBatch; ++batch)
		{
			// Draw this batch
			batch->fillCommandBuffer(mCommandBuffer);
		}
	}
}
