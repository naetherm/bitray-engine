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
#include "instancing/instancing.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void Instancing::on_initialization()
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

		{
			// There are two instancing approaches available
			// - Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			// - Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			//
			// In general, instanced arrays will probably run on the most systems:
			// -> Direct3D 10, Direct3D 11 and Direct3D 12 support both approaches
			// -> Direct3D 9 has support for instanced arrays, but does not support draw instanced
			// -> OpenGL 3.1 introduced draw instance ("GL_ARB_draw_instanced"-extension)
			// -> OpenGL 3.3 introduced instance array ("GL_ARB_instanced_arrays"-extension)
			// -> OpenGL ES 3.0 support both approaches
			rhi::RHIShaderLanguage& shaderLanguage = rhi->get_default_shader_language();

			// Left side (green): Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			if (rhi->get_capabilities().instancedArrays)
			{
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
					},
					{ // Attribute 1
						// Data destination
						rhi::VertexAttributeFormat::FLOAT_1,	// vertexAttributeFormat (rhi::VertexAttributeFormat)
						"InstanceID",							// name[32] (char)
						"TEXCOORD",								// semanticName[32] (char)
						0,										// semanticIndex (core::uint32)
						// Data source
						1,										// inputSlot (core::uint32)
						0,										// alignedByteOffset (core::uint32)
						sizeof(float),							// strideInBytes (core::uint32)
						1										// instancesPerElement (core::uint32)
					}
				};
				const rhi::VertexAttributes vertexAttributes(2, vertexAttributesLayout);

				{ // Create vertex array object (VAO)
					// Create the vertex buffer object (VBO)
					// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
					static constexpr float VERTEX_POSITION[] =
					{					// Vertex ID	Triangle on screen
						 0.0f, 1.0f,	// 0					 .0
						 0.0f, 0.0f,	// 1				 .    .
						-1.0f, 0.0f		// 2			  2.......1
					};
					rhi::RHIVertexBufferPtr vertexBufferPosition(
            mBufferManager->create_vertex_buffer({sizeof(VERTEX_POSITION), VERTEX_POSITION}));

					// Create the per-instance-data vertex buffer object (VBO)
					// -> Simple instance ID in order to keep it similar to the "draw instanced" version on the right side (blue)
					static constexpr float INSTANCE_ID[] =
					{
						0.0f, 1.0f
					};
					rhi::RHIVertexBufferPtr vertexBufferInstanceId(
            mBufferManager->create_vertex_buffer({sizeof(INSTANCE_ID), INSTANCE_ID}));

					// Create the index buffer object (IBO)
					// -> In this example, we only draw a simple triangle and therefore usually do not need an index buffer
					// -> In Direct3D 9, instanced arrays with hardware support is only possible when drawing indexed primitives, see
					//    "Efficiently Drawing Multiple Instances of Geometry (Direct3D 9)"-article at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173349%28v=vs.85%29.aspx#Drawing_Non_Indexed_Geometry
					static constexpr core::uint16 INDICES[] =
					{
						0, 1, 2
					};
					rhi::RHIIndexBufferPtr indexBufferInstancedArrays(
            mBufferManager->create_index_buffer({sizeof(INDICES), INDICES}));

					// Create vertex array object (VAO)
					// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
					// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
					// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
					//    reference of the used vertex buffer objects (VBO). If the reference counter of a
					//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
					const rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBufferPosition, vertexBufferInstanceId, };
					mVertexArrayInstancedArrays = mBufferManager->create_vertex_array({vertexAttributes,
                                                                            2,
                                                                            vertexArrayVertexBuffers,
                                                                            indexBufferInstancedArrays});
				}

				// Create the graphics program
				rhi::RHIGraphicsProgramPtr graphicsProgram;
				{
					// get the shader source code (outsourced to keep an overview)
					const char* vertexShaderSourceCode = nullptr;
					const char* fragmentShaderSourceCode = nullptr;
					#include "instancing/instanced_arrays_shaders.inl"

					// Create the graphics program
					graphicsProgram = shaderLanguage.create_graphics_program(
            *mRootSignature,
            vertexAttributes,
            shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode),
						shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));
				}

				// Create the graphics pipeline state object (PSO)
				if (nullptr != graphicsProgram)
				{
					mGraphicsPipelineStateInstancedArrays = rhi->create_graphics_pipeline_state(
            rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgram, vertexAttributes,
                                                          get_main_render_target()->get_render_pass()));
				}
			}

			// Right side (blue): Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			if (rhi->get_capabilities().drawInstanced)
			{
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
					{				// Vertex ID	Triangle on screen
						0.0f, 1.0f,	// 0			  0.	
						1.0f, 0.0f,	// 1			  .    .
						0.0f, 0.0f	// 2			  2.......1
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
					mVertexArrayDrawInstanced = mBufferManager->create_vertex_array({vertexAttributes,
                                                                          1,
                                                                          vertexArrayVertexBuffers});
				}

				// Create the graphics program
				rhi::RHIGraphicsProgramPtr graphicsProgram;
				{
					// get the shader source code (outsourced to keep an overview)
					const char* vertexShaderSourceCode = nullptr;
					const char* fragmentShaderSourceCode = nullptr;
					#include "instancing/draw_instanced_shaders.inl"

					// Create the graphics program
					graphicsProgram = shaderLanguage.create_graphics_program(
            *mRootSignature,
            vertexAttributes,
            shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode),
						shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));
				}

				// Create the graphics pipeline state object (PSO)
				if (nullptr != graphicsProgram)
				{
					mGraphicsPipelineStateDrawInstanced = rhi->create_graphics_pipeline_state(
            rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgram, vertexAttributes,
                                                          get_main_render_target()->get_render_pass()));
				}
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fill_command_buffer();
	}
}

void Instancing::on_deinitialization()
{
	// release the used resources
	mVertexArrayDrawInstanced = nullptr;
	mGraphicsPipelineStateDrawInstanced = nullptr;
	mVertexArrayInstancedArrays = nullptr;
	mGraphicsPipelineStateInstancedArrays = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

void Instancing::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void Instancing::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mGraphicsPipelineStateInstancedArrays, "Invalid graphics pipeline state instanced arrays")
	BE_ASSERT(nullptr != mVertexArrayInstancedArrays, "Invalid vertex array instanced arrays")
	BE_ASSERT(!get_rhi()->get_capabilities().drawInstanced || nullptr != mGraphicsPipelineStateDrawInstanced, "Invalid graphics pipeline state draw instanced")
	BE_ASSERT(!get_rhi()->get_capabilities().drawInstanced || nullptr != mVertexArrayDrawInstanced, "Invalid vertex array draw instanced")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);

	// Set the used graphics root signature
	rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Left side (green): Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
	if (get_rhi()->get_capabilities().instancedArrays)
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Draw using instanced arrays")

		// Set the used graphics pipeline state object (PSO)
		rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineStateInstancedArrays);

		// Input assembly (IA): Set the used vertex array
		rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArrayInstancedArrays);

		// Render the specified geometric primitive, based on an array of vertices
		// -> In this example, we only draw a simple triangle and therefore usually do not need an index buffer
		// -> In Direct3D 9, instanced arrays with hardware support is only possible when drawing indexed primitives, see
		//    "Efficiently Drawing Multiple Instances of Geometry (Direct3D 9)"-article at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173349%28v=vs.85%29.aspx#Drawing_Non_Indexed_Geometry
		rhi::Command::DrawIndexedGraphics::create(mCommandBuffer, 3, 2);
	}

	// Right side (blue): Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
	if (get_rhi()->get_capabilities().drawInstanced)
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Draw instanced")

		// Set the used graphics pipeline state object (PSO)
		rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineStateDrawInstanced);

		// Input assembly (IA): Set the used vertex array
		rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArrayDrawInstanced);

		// Render the specified geometric primitive, based on an array of vertices
		rhi::Command::DrawGraphics::create(mCommandBuffer, 3, 2);
	}
}
