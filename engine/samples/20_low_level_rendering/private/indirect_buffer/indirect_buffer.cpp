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
#include "indirect_buffer/indirect_buffer.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void IndirectBuffer::on_initialization()
{
	// Call the base implementation
  Triangle::on_initialization();

	{ // Create the indirect buffer
		const rhi::DrawArguments drawArguments =
		{
			3,	// vertexCountPerInstance (core::uint32)
			1,	// instanceCount (core::uint32)
			0,	// startVertexLocation (core::uint32)
			0	// startInstanceLocation (core::uint32)
		};
		mIndirectBuffer = mBufferManager->create_indirect_buffer({sizeof(rhi::DrawArguments), &drawArguments,
                                                             rhi::IndirectBufferFlag::DRAW_ARGUMENTS});
	}

	// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	mCommandBuffer.clear();	// Throw away "Triangle"-stuff
	fill_command_buffer();
}

void IndirectBuffer::on_deinitialization()
{
	// release the used resources
	mIndirectBuffer = nullptr;

	// Call the base implementation
  Triangle::on_deinitialization();
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void IndirectBuffer::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "Command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
	BE_ASSERT(nullptr != mVertexArray, "Invalid vertex array")
	BE_ASSERT(nullptr != mIndirectBuffer, "Invalid indirect buffer")

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

	// Set debug marker
	// -> Debug methods: When using Direct3D <11.1, these methods map to the Direct3D 9 PIX functions
	//    (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)
	COMMAND_SET_DEBUG_MARKER(mCommandBuffer, "Everyone ready for the upcoming triangle?")

	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Drawing the fancy triangle")

		// Render the specified geometric primitive, based on an array of vertices
		rhi::Command::DrawGraphics::create(mCommandBuffer, *mIndirectBuffer);
	}
}
