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
#include "instanced_cubes/instanced_cubes.h"
#include "instanced_cubes/cube_renderer_draw_instanced/cube_renderer_draw_instanced.h"
#include "instanced_cubes/cube_renderer_instanced_arrays/cube_renderer_instanced_arrays.h"
#include "application.h"
#include <core/color/color4.h>
#include <core/input/input_server.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
InstancedCubes::InstancedCubes(Application& cApplication, const core::String& name)
: ExampleBase(cApplication, name),
	mInputManager(&core::InputServer::instance()),
	mCubeRenderer(nullptr),
	mNumberOfCubeInstances(1000),
	mGlobalTimer(0.0f),
	mGlobalScale(1.0f),
	mDisplayStatistics(true),
	mFPSUpdateTimer(0.0f),
	mFramesSinceCheck(0),
	mFramesPerSecond(0.0f)
{
	// Nothing here
}

InstancedCubes::~InstancedCubes()
{
	delete mInputManager;

	// The resources are released within "on_deinitialization()"
}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void InstancedCubes::on_initialization()
{
	// get and check the RHI instance
	rhi::RHIDevicePtr rhi(get_rhi());
	if (nullptr != rhi)
	{
		// Create the cube renderer instance
		// -> Evaluate the feature set of the used RHI
		// TODO(naetherm) This example doesn't support texture buffer emulation, which for OpenGL ES 3 is currently used
		const rhi::Capabilities& capabilities = rhi->get_capabilities();
		if (capabilities.drawInstanced && capabilities.maximumNumberOf2DTextureArraySlices > 0 && capabilities.maximumTextureBufferSize > 0 &&
      rhi->get_name_id() != rhi::NameId::OPENGLES3)
		{
			// Render cubes by using draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			mCubeRenderer = new CubeRendererDrawInstanced(*rhi, get_main_render_target()->get_render_pass(), NUMBER_OF_TEXTURES, SCENE_RADIUS);
		}
		else if (capabilities.instancedArrays)
		{
			// Render cubes by using instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			mCubeRenderer = new CubeRendererInstancedArrays(*rhi, get_main_render_target()->get_render_pass(), NUMBER_OF_TEXTURES, SCENE_RADIUS);
		}

		// Tell the cube renderer about the number of cubes
		if (nullptr != mCubeRenderer)
		{
			mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
		}
	}
}

void InstancedCubes::on_deinitialization()
{
	// Destroy the cube renderer, in case there's one
	if (nullptr != mCubeRenderer)
	{
		delete mCubeRenderer;
		mCubeRenderer = nullptr;
	}
}

void InstancedCubes::on_update()
{
	// stop the stopwatch
  mStopwatch.stop();

	// Update the global timer (FPS independent movement)
  const float timeDifference = mStopwatch.get_milliseconds();
	mGlobalTimer += timeDifference;

	// Calculate the current FPS
	++mFramesSinceCheck;
	mFPSUpdateTimer += timeDifference;
	if (mFPSUpdateTimer > 1000.0f)
	{
		mFramesPerSecond   = static_cast<float>(mFramesSinceCheck) / (mFPSUpdateTimer / 1000.0f);
		mFPSUpdateTimer   -= 1000.0f;
		mFramesSinceCheck  = 0;

    printf(">> FPS: %f\n", mFramesPerSecond);
	}

	// start the stopwatch
  mStopwatch.start();

	{ // Input
    core::Keyboard* keyboard = mInputManager->get_keyboard();
		if (keyboard->has_changed())
		{
			// Add a fixed number of cubes
			if (keyboard->NumpadAdd.check_hit_and_release() || keyboard->Add.check_hit_and_release())
			{
				// Upper limit, just in case someone tries something nasty
				if (mNumberOfCubeInstances < UINT_MAX - NUMBER_OF_CHANGED_CUBES)
				{
					// Update the number of cubes
					mNumberOfCubeInstances += NUMBER_OF_CHANGED_CUBES;

					// Tell the cube renderer about the number of cubes
					if (nullptr != mCubeRenderer)
					{
						printf(":: Created %d cube instances\n", mNumberOfCubeInstances);
						mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
					}
				}
			}

			// Subtract a fixed number of cubes
			if (keyboard->NumpadSubtract.check_hit_and_release() || keyboard->Subtract.check_hit_and_release())
			{
				// Lower limit
				if (mNumberOfCubeInstances > 1)
				{
					// Update the number of cubes
					if (mNumberOfCubeInstances > NUMBER_OF_CHANGED_CUBES)
					{
						mNumberOfCubeInstances -= NUMBER_OF_CHANGED_CUBES;
					}
					else
					{
						mNumberOfCubeInstances = 1;
					}

					// Tell the cube renderer about the number of cubes
					if (nullptr != mCubeRenderer)
					{
						mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
					}
				}
			}

			// Scale cubes up (change the size of all cubes at the same time)
			if (keyboard->Up.check_hit_and_release())
			{
				mGlobalScale += 0.1f;
			}

			// Scale cubes down (change the size of all cubes at the same time)
			if (keyboard->Down.check_hit_and_release())
			{
				mGlobalScale -= 0.1f;	// No need to check for negative values, results in entertaining inversed backface culling
			}

			// Show/hide statistics
			if (keyboard->Space.check_hit_and_release())
			{
				// Toggle display of statistics
				mDisplayStatistics = !mDisplayStatistics;
			}
		}
		mInputManager->update();
	}
}

void InstancedCubes::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);

	// Draw the cubes
	if (nullptr != mCubeRenderer)
	{
		mCubeRenderer->fillCommandBuffer(mGlobalTimer, mGlobalScale, sin(mGlobalTimer * 0.001f) * SCENE_RADIUS, sin(mGlobalTimer * 0.0005f) * SCENE_RADIUS, cos(mGlobalTimer * 0.0008f) * SCENE_RADIUS, mCommandBuffer);
	}

	// Append command buffer to the given command buffer
  mCommandBuffer.append_to_command_buffer_and_clear(commandBuffer);
}
