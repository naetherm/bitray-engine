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
#include "tessellation_shader/tessellation_shader.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void TessellationShader::on_initialization()
{
	// get and check the RHI instance
	// -> Uniform buffer object (UBO, "constant buffer" in Direct3D terminology) supported?
	// -> Tessellation control and tessellation evaluation shaders supported?
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi && rhi->get_capabilities().maximumUniformBufferSize > 0 &&
    rhi->get_capabilities().maximumNumberOfPatchVertices > 0)
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
				const char* tessellationControlShaderSourceCode = nullptr;
				const char* tessellationEvaluationShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
				#include "tessellation_shader/shaders.inl"

				// Create the graphics program
				rhi::RHIShaderLanguage& shaderLanguage = rhi->get_default_shader_language();
				graphicsProgram = shaderLanguage.create_graphics_program(
          *mRootSignature,
          vertexAttributes,
          shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode),
          shaderLanguage.create_tessellation_control_shader_from_source_code(tessellationControlShaderSourceCode),
          shaderLanguage.create_tessellation_evaluation_shader_from_source_code(tessellationEvaluationShaderSourceCode),
					shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));
			}

			// Create the graphics pipeline state object (PSO)
			if (nullptr != graphicsProgram)
			{
				rhi::GraphicsPipelineStateDescriptor graphicsPipelineState = rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgram, vertexAttributes,
                                                                                                                     get_main_render_target()->get_render_pass());
				graphicsPipelineState.primitiveTopology = rhi::PrimitiveTopology::PATCH_LIST_3;	// Patch list with 3 vertices per patch (tessellation relevant topology type) - "rhi::PrimitiveTopology::TriangleList" used for tessellation
				graphicsPipelineState.primitiveTopologyType = rhi::PrimitiveTopologyType::PATCH;
				graphicsPipelineState.rasterizerState.fillMode = rhi::FillMode::WIREFRAME;
				mGraphicsPipelineState = rhi->create_graphics_pipeline_state(graphicsPipelineState);
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fill_command_buffer();
	}
}

void TessellationShader::on_deinitialization()
{
	// release the used resources
	mVertexArray = nullptr;
	mGraphicsPipelineState = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

void TessellationShader::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void TessellationShader::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
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

	// Input assembly (IA): Set the used vertex array
	rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArray);

	// Render the specified geometric primitive, based on an array of vertices
	rhi::Command::DrawGraphics::create(mCommandBuffer, 3);
}
