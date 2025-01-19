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
#include "geometry_shader/geometry_shader.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void GeometryShader::on_initialization()
{
	// get and check the RHI instance
	// -> Geometry shaders supported?
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi && rhi->get_capabilities().maximumNumberOfGsOutputVertices)
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
		const rhi::VertexAttributes vertexAttributes(0, nullptr);

		{
			// Create the graphics program
			rhi::RHIGraphicsProgramPtr graphicsProgram;
			{
				// get the shader source code (outsourced to keep an overview)
				const char* vertexShaderSourceCode = nullptr;
				const char* geometryShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
				#include "geometry_shader/shaders.inl"

				// Create the graphics program
				rhi::RHIShaderLanguage& shaderLanguage = rhi->get_default_shader_language();
				graphicsProgram = shaderLanguage.create_graphics_program(
          *mRootSignature,
          vertexAttributes,
          shaderLanguage.create_vertex_shader_from_source_code(vertexAttributes, vertexShaderSourceCode),
          shaderLanguage.create_geometry_shader_from_source_code(geometryShaderSourceCode),
					shaderLanguage.create_fragment_shader_from_source_code(fragmentShaderSourceCode));
			}

			// Create the graphics pipeline state object (PSO)
			if (nullptr != graphicsProgram)
			{
				rhi::GraphicsPipelineStateDescriptor graphicsPipelineState = rhi::GraphicsPipelineStateDescriptorBuilder(mRootSignature, graphicsProgram, vertexAttributes,
                                                                                                                     get_main_render_target()->get_render_pass());
				graphicsPipelineState.primitiveTopology = rhi::PrimitiveTopology::POINT_LIST;
				graphicsPipelineState.primitiveTopologyType = rhi::PrimitiveTopologyType::POINT;
				mGraphicsPipelineState = rhi->create_graphics_pipeline_state(graphicsPipelineState);
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fill_command_buffer();
	}
}

void GeometryShader::on_deinitialization()
{
	// release the used resources
	mGraphicsPipelineState = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

void GeometryShader::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Dispatch pre-recorded command buffer
	rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void GeometryShader::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "Command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);

	// Set the used graphics root signature
	rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

	// Render the specified geometric primitive, based on an array of vertices
	// -> Emit a single point in order to generate a draw call, the geometry shader does the rest
	// -> Attribute-less rendering (aka "drawing without data")
	rhi::Command::DrawGraphics::create(mCommandBuffer, 1);
}
