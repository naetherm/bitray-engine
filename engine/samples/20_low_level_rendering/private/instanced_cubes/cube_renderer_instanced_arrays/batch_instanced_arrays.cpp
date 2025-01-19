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
#include "instanced_cubes/cube_renderer_instanced_arrays/batch_instanced_arrays.h"
#include <stdlib.h> // For rand()
#include <random>
#include <core/math/quaternion.h>
#include <core/math/euler_angles.h>


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
void BatchInstancedArrays::initialize(rhi::RHIBufferManager& bufferManager, rhi::RHIRootSignature& rootSignature, const rhi::VertexAttributes& vertexAttributes, rhi::RHIVertexBuffer& vertexBuffer, rhi::RHIIndexBuffer& indexBuffer, rhi::RHIGraphicsProgram& graphicsProgram, rhi::RHIRenderPass& renderPass, core::uint32 numberOfCubeInstances, bool alphaBlending, core::uint32 numberOfTextures, core::uint32 sceneRadius)
{
	// Set owner RHI instance
	mRhi = &graphicsProgram.get_rhi();

	// release previous data if required
	mVertexArray = nullptr;

	// Set the number of cube instance
	mNumberOfCubeInstances = numberOfCubeInstances;

	{ // Create the texture buffer instance
		// Allocate the local per instance data
		const core::uint32 numberOfElements = mNumberOfCubeInstances * 2 * 4;
		float* data = new float[numberOfElements];
		float* RESTRICT dataCurrent = data;
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_int_distribution<> randomDistribution(0, 65536);

		// Set data
		// -> Layout: [Position][Rotation][Position][Rotation]...
		//    - Position: xyz=Position, w=Slice of the 2D texture array to use
		//    - Rotation: Rotation quaternion (xyz) and scale (w)
		//      -> We don't need to store the w component of the quaternion. It's normalized and storing
		//         three components while recomputing the fourths component is be sufficient.
    core::Quaternionf rotation(1.0f, 0.0f, 0.0f, 0.0f);	// Identity rotation quaternion
		for (core::uint32 i = 0; i < mNumberOfCubeInstances; ++i)
		{
			{ // Position
				// r=x
				*dataCurrent = -static_cast<float>(sceneRadius) + 2.0f * static_cast<float>(sceneRadius) * static_cast<float>(rand() % 65536) / 65536.0f;
				++dataCurrent;
				// g=y
				*dataCurrent = -static_cast<float>(sceneRadius) + 2.0f * static_cast<float>(sceneRadius) * static_cast<float>(rand() % 65536) / 65536.0f;
				++dataCurrent;
				// b=z
				*dataCurrent = -static_cast<float>(sceneRadius) + 2.0f * static_cast<float>(sceneRadius) * static_cast<float>(rand() % 65536) / 65536.0f;
				++dataCurrent;
				// a=Slice of the 2D texture array to use
				*dataCurrent = static_cast<float>(rand() % numberOfTextures); // Choose a random texture
				++dataCurrent;
			}

			{ // Rotation
        rotation = core::EulerAnglesf::to_quaternion(static_cast<float>(randomDistribution(randomGenerator)) / 65536.0f, static_cast<float>(randomDistribution(randomGenerator)) / 65536.0f * 2.0f, static_cast<float>(randomDistribution(randomGenerator)) / 65536.0f * 3.0f);

        // r=x
				*dataCurrent = rotation.x;
				++dataCurrent;
				// g=y
				*dataCurrent = rotation.y;
				++dataCurrent;
				// b=z
				*dataCurrent = rotation.z;
				++dataCurrent;
				// a=scale
				*dataCurrent = 2.0f * static_cast<float>(rand() % 65536) / 65536.0f;
				++dataCurrent;
			}
		}

		// Create the vertex buffer object (VBO) instance containing the per-instance-data
		rhi::RHIVertexBuffer* vertexBufferPerInstanceData = bufferManager.create_vertex_buffer(
      {sizeof(float) * numberOfElements, data});

		{ // Create vertex array object (VAO)
			// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
			// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
			// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
			//    reference of the used vertex buffer objects (VBO). If the reference counter of a
			//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
			const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { &vertexBuffer, vertexBufferPerInstanceData };
			mVertexArray = bufferManager.create_vertex_array({vertexAttributes, 2, vertexArrayVertexBuffers, &indexBuffer});
		}

		// Free local per instance data
		delete [] data;
	}

	{ // Create the graphics pipeline state object (PSO)
		rhi::GraphicsPipelineStateDescriptor graphicsPipelineState = rhi::GraphicsPipelineStateDescriptorBuilder(&rootSignature, &graphicsProgram, vertexAttributes, renderPass);
		graphicsPipelineState.blendState.renderTarget[0].blendEnable = alphaBlending;
		graphicsPipelineState.blendState.renderTarget[0].srcBlend    = rhi::Blend::SRC_ALPHA;
		graphicsPipelineState.blendState.renderTarget[0].destBlend   = rhi::Blend::ONE;
		mGraphicsPipelineState = mRhi->create_graphics_pipeline_state(graphicsPipelineState);
	}
}

void BatchInstancedArrays::fillCommandBuffer(rhi::RHICommandBuffer& commandBuffer) const
{
	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer)

	// Set the used graphics pipeline state object (PSO)
	rhi::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

	// Setup input assembly (IA): Set the used vertex array
	rhi::Command::SetGraphicsVertexArray::create(commandBuffer, mVertexArray);

	// Use instancing in order to draw multiple cubes with just a single draw call
	// -> Draw calls are one of the most expensive rendering, avoid them if possible
	rhi::Command::DrawIndexedGraphics::create(commandBuffer, 36, mNumberOfCubeInstances);
}
