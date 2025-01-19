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
#include "queries/queries.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void Queries::on_initialization()
{
	// Call the base implementation
  Triangle::on_initialization();

	// get and check the RHI instance
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi)
	{
		// Create the queries
		mOcclusionQueryPool = rhi->create_query_pool(rhi::QueryType::OCCLUSION);
		mPipelineStatisticsQueryPool = rhi->create_query_pool(rhi::QueryType::PIPELINE_STATISTICS);
		mTimestampQueryPool = rhi->create_query_pool(rhi::QueryType::TIMESTAMP, 2);

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		mCommandBuffer.clear();	// Throw away "Triangle"-stuff
		fill_command_buffer();
	}
}

void Queries::on_deinitialization()
{
	// release the used resources
	mOcclusionQueryPool = nullptr;
	mPipelineStatisticsQueryPool = nullptr;
	mTimestampQueryPool = nullptr;

	// Call the base implementation
  Triangle::on_deinitialization();
}

void Queries::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Call the base implementation
  Triangle::on_draw(commandBuffer);

	// get query results
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi)
	{
		// Sanity checks
		BE_ASSERT(nullptr != mOcclusionQueryPool, "Invalid occlusion query pool")
		BE_ASSERT(nullptr != mPipelineStatisticsQueryPool, "Invalid pipeline statistics query pool")
		BE_ASSERT(nullptr != mTimestampQueryPool, "Invalid timestamp query pool")

		{ // Occlusion query pool
			core::uint64 numberOfSamples = 0;
			if (rhi->get_query_pool_results(*mOcclusionQueryPool, sizeof(core::uint64),
                                      reinterpret_cast<core::uint8 *>(&numberOfSamples)))
			{
				NOP;	// TODO(naetherm) Process result
			}
		}

		{ // Pipeline statistics query pool
			rhi::PipelineStatisticsQueryResult pipelineStatisticsQueryResult = {};
			if (rhi->get_query_pool_results(*mPipelineStatisticsQueryPool, sizeof(rhi::PipelineStatisticsQueryResult),
                                      reinterpret_cast<core::uint8 *>(&pipelineStatisticsQueryResult)))
			{
				NOP;	// TODO(naetherm) Process result
			}
		}

		{ // Timestamp query pool
			core::uint64 timestamp[2] = {};
			if (rhi->get_query_pool_results(*mTimestampQueryPool, sizeof(core::uint64) * 2,
                                      reinterpret_cast<core::uint8 *>(&timestamp), 0, 2, sizeof(core::uint64)))
			{
				NOP;	// TODO(naetherm) Process result
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void Queries::fill_command_buffer()
{
	// Sanity checks
	BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
	BE_ASSERT(mCommandBuffer.is_empty(), "Command buffer is already filled")
	BE_ASSERT(nullptr != mRootSignature, "Invalid root signature")
	BE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
	BE_ASSERT(nullptr != mVertexArray, "Invalid vertex array")
	BE_ASSERT(nullptr != mOcclusionQueryPool, "Invalid occlusion query pool")
	BE_ASSERT(nullptr != mPipelineStatisticsQueryPool, "Invalid pipeline statistics query pool")
	BE_ASSERT(nullptr != mTimestampQueryPool, "Invalid timestamp query pool")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// reset and begin queries
	rhi::Command::ResetQueryPool::create(mCommandBuffer, *mTimestampQueryPool, 0, 2);
	rhi::Command::WriteTimestampQuery::create(mCommandBuffer, *mTimestampQueryPool, 0);
	rhi::Command::ResetAndBeginQuery::create(mCommandBuffer, *mOcclusionQueryPool);
	rhi::Command::ResetAndBeginQuery::create(mCommandBuffer, *mPipelineStatisticsQueryPool, 0, rhi::QueryControlFlags::PRECISE);

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
		rhi::Command::DrawGraphics::create(mCommandBuffer, 3);
	}

	// End queries
	rhi::Command::EndQuery::create(mCommandBuffer, *mOcclusionQueryPool);
	rhi::Command::EndQuery::create(mCommandBuffer, *mPipelineStatisticsQueryPool);
	rhi::Command::WriteTimestampQuery::create(mCommandBuffer, *mTimestampQueryPool, 1);
}
