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
#include "empty/empty.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void Empty::on_initialization()
{
  // get and check the RHI instance
  rhi::RHIDevicePtr rhi(get_rhi());
  if (nullptr != rhi)
  {
    // Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
    fill_command_buffer();
  }
}

void Empty::on_deinitialization()
{
  // release the used resources
}

void Empty::on_update() {
  ExampleBase::on_update();
}

void Empty::on_draw(rhi::RHICommandBuffer& commandBuffer)
{
  // Dispatch pre-recorded command buffer
  rhi::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void Empty::fill_command_buffer()
{
  // Sanity checks
  BE_ASSERT(nullptr != get_rhi(), "Invalid RHI instance")
  BE_ASSERT(mCommandBuffer.is_empty(), "The command buffer is already filled")

  // Scoped debug event
  COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

  // Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
  rhi::Command::ClearGraphics::create(mCommandBuffer, rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);
}
