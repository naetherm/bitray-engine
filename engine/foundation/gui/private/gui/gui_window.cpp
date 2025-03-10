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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
#include "gui/gui/gui_window.h"
#include "gui/gui/gui_server.h"
#include <core/color/color4.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiWindow::GuiWindow(core::Ptr<GuiServer>& guiDevice)
: mGuiServer(guiDevice)
, mWindow(nullptr)
, mNativeWindowId(INVALID_HANDLE)
, mFrontend(nullptr)
, mSwapChain(nullptr)
, mImGuiContext(ImGui::CreateContext()) {

}

GuiWindow::~GuiWindow() {
  
}


const core::Window *GuiWindow::get_window() const {
  return mWindow;
}

core::handle GuiWindow::get_native_window_handle() const {
  return mNativeWindowId;
}

const core::FrontendServer *GuiWindow::get_frontend() const {
  return mFrontend;
}

const core::Ptr<rhi::RHISwapChain> GuiWindow::get_swap_chain() const {
  return mSwapChain;
}

core::Ptr<rhi::RHISwapChain> GuiWindow::get_swap_chain() {
  return mSwapChain;
}

const rhi::RHICommandBuffer &GuiWindow::get_command_buffer() const {
  return mCommandBuffer;
}

rhi::RHICommandBuffer &GuiWindow::get_command_buffer() {
  return mCommandBuffer;
}

void GuiWindow::initialize(core::handle windowHandle, core::Ptr<rhi::RHISwapChain> swapChain) {
  mNativeWindowId = windowHandle;

  mSwapChain = swapChain;

  // Set swapchain
  mSwapChain->add_ref();
}


void GuiWindow::draw() {
  //engine::Command::SetGraphicsRenderTarget::create(mCommandBuffer, mSwapChain);
  // Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
  //engine::Command::ClearGraphics::create(mCommandBuffer, engine::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);
  //ImGui::SetCurrentContext(mImGuiContext);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}