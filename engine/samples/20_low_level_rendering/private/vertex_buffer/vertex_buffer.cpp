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
#include "vertex_buffer/vertex_buffer.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void VertexBuffer::on_initialization()
{
	// get and check the RHI instance
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi)
	{
		// Create the buffer manager
		mBufferManager = rhi->create_buffer_manager();

		{ // Create the root signature
			// Setup
			rhi::RootSignatureDescriptorBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(0, nullptr, 0, nullptr, rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi->create_root_signature(rootSignatureBuilder);
		}

		// Vertex input layout
		static constexpr rhi::VertexAttribute vertexAttributesLayoutVBO[] =
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
				sizeof(float) * 5,						// strideInBytes (core::uint32)
				0										// instancesPerElement (core::uint32)
			},
			{ // Attribute 1
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"Color",								// name[32] (char)
				"COLOR",								// semanticName[32] (char)
				0,										// semanticIndex (core::uint32)
				// Data source
				0,										// inputSlot (core::uint32)
				sizeof(float) * 2,						// alignedByteOffset (core::uint32)
				sizeof(float) * 5,						// strideInBytes (core::uint32)
				0										// instancesPerElement (core::uint32)
			}
		};
		const rhi::VertexAttributes vertexAttributesVBO(2, vertexAttributesLayoutVBO);
		static constexpr rhi::VertexAttribute vertexAttributesLayoutVBOs[] =
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
			},
			{ // Attribute 1
				// Data destination
				rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
				"Color",								// name[32] (char)
				"COLOR",								// semanticName[32] (char)
				0,										// semanticIndex (core::uint32)
				// Data source
				1,										// inputSlot (core::uint32)
				0,										// alignedByteOffset (core::uint32)
				sizeof(float) * 3,						// strideInBytes (core::uint32)
				0										// instancesPerElement (core::uint32)
			}
		};
		const rhi::VertexAttributes vertexAttributesVBOs(2, vertexAttributesLayoutVBOs);

		// Vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.

		{ // Create vertex array object (VAO)
			// Create the vertex buffer object (VBO) holding position and color data
			// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
			// -> Traditional normalized RGB vertex colors
			static constexpr float VERTEX_POSITION_COLOR[] =
			{	 // Position     Color				// Vertex ID	Triangle on screen
				 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,	// 0				0
				 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	// 1			   .   .
				-0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// 2			  2.......1
			};
			rhi::RHIVertexBufferPtr vertexBufferPositionColor(
        mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION_COLOR), VERTEX_POSITION_COLOR}));

			// Create vertex array object (VAO)
			const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBufferPositionColor };
			mVertexArrayVbo = mBufferManager->create_vertex_array({vertexAttributesVBO,
                                                            1, vertexArrayVertexBuffers});
		}

		{ // Create vertex array object (VAO) using multiple vertex buffer object (VBO)
			// Create the vertex buffer object (VBO) holding color data
			// -> Traditional normalized RGB vertex colors
			static constexpr float VERTEX_COLOR[] =
			{					// Vertex ID	Triangle on screen
				1.0f, 0.0f, 0.0f,	// 0			  0.......1
				0.0f, 1.0f, 0.0f,	// 1			   .   .
				0.0f, 0.0f, 1.0f	// 2			  	2
			};
			rhi::RHIVertexBufferPtr vertexBufferColor(
        mBufferManager->create_vertex_buffer({sizeof(VERTEX_COLOR), VERTEX_COLOR}));

			// Create the vertex buffer object (VBO) holding position data
			// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
			static constexpr float VERTEX_POSITION[] =
			{					// Vertex ID	Triangle on screen
				-0.5f,  0.0f,	// 0			  0.......1
				 1.0f,  0.0f,	// 1			   .   .
				 0.0f, -1.0f	// 2			  	2
			};
			rhi::RHIVertexBufferPtr vertexBufferPosition(
        mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), VERTEX_POSITION}));

			// Create vertex array object (VAO)
			const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBufferPosition, vertexBufferColor };
			mVertexArrayVbos = mBufferManager->create_vertex_array({vertexAttributesVBOs,
                                                             2, vertexArrayVertexBuffers});
		}

		{
			// get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode = nullptr;
			#include "vertex_buffer/shaders.inl"
			rhi::RHIShaderLanguage& shaderLanguage = rhi->get_default_shader_language();

			{ // Create pipeline state objects (PSO) using one vertex buffer object (VBO)
				// Create the graphics program
				rhi::RHIGraphicsProgramPtr graphicsProgram;
				graphicsProgram = shaderLanguage.create_graphics_program(
          *mRootSignature,
          vertexAttributesVBO,
          shaderLanguage.create_vertex_shader_from_source_code(vertexAttributesVBO, vertexShaderSourceCode),
					shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));

				// Create the graphics pipeline state objects (PSO)
				if (nullptr != graphicsProgram)
				{
					mGraphicsPipelineStateVbo = rhi->create_graphics_pipeline_state(
            rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgram, vertexAttributesVBO,
                                                          get_main_render_target()->get_render_pass()));
				}
			}

			{ // Create graphics pipeline state objects (PSO) using multiple vertex buffer object (VBO)
				// Create the graphics program
				rhi::RHIGraphicsProgramPtr graphicsProgram;
				graphicsProgram = shaderLanguage.create_graphics_program(
          *mRootSignature,
          vertexAttributesVBOs,
          shaderLanguage.create_vertex_shader_from_source_code(vertexAttributesVBOs, vertexShaderSourceCode),
					shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));

				// Create the graphics pipeline state objects (PSO)
				if (nullptr != graphicsProgram)
				{
					mGraphicsPipelineStateVbos = rhi->create_graphics_pipeline_state(
            rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgram, vertexAttributesVBOs,
                                                          get_main_render_target()->get_render_pass()));
				}
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fill_command_buffer();
	}
}

void VertexBuffer::on_deinitialization()
{
	// release the used resources
	mGraphicsPipelineStateVbos = nullptr;
	mVertexArrayVbos = nullptr;
	mVertexArrayVbo = nullptr;
	mGraphicsPipelineStateVbo = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

void VertexBuffer::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void VertexBuffer::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mGraphicsPipelineStateVbo, "Invalid graphics pipeline state VBO")
	BE_ASSERT(nullptr != mVertexArrayVbo, "Invalid vertex array VBO")
	BE_ASSERT(nullptr != mGraphicsPipelineStateVbos, "Invalid graphics pipeline state VBOs")
	BE_ASSERT(nullptr != mVertexArrayVbos, "Invalid vertex array VBOs")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);

	// Set the used graphics root signature
	rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// First lower triangle using one vertex buffer object (VBO)
	if (nullptr != mGraphicsPipelineStateVbo)
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Draw using one VBO")

		// Set the used graphics pipeline state object (PSO)
		rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineStateVbo);

		// Input assembly (IA): Set the used vertex array
		rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArrayVbo);

		// Render the specified geometric primitive, based on an array of vertices
		rhi::Command::DrawGraphics::create(mCommandBuffer, 3);
	}

	// Second upper triangle using multiple vertex buffer object (VBO)
	if (nullptr != mGraphicsPipelineStateVbos)
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Draw using multiple VBOs")

		// Set the used graphics pipeline state object (PSO)
		rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineStateVbos);

		// Input assembly (IA): Set the used vertex array
		rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArrayVbos);

		// Render the specified geometric primitive, based on an array of vertices
		rhi::Command::DrawGraphics::create(mCommandBuffer, 3);
	}
}
