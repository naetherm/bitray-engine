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
#include "gui/gui/gui_server.h"
#include "gui/gui/gui_window.h"
#include "gui/renderer/gui_renderer.h"
#include <core/color/color4.h>
#include <core/frontend/window.h>
#include <core/input/types.h>
#include <gui/gui/gui_context.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiServer* GuiServer::instance() {
  static GuiServer* SInstance = new GuiServer();
  return SInstance;
}


GuiServer::GuiServer()
: mGuiContext(nullptr) {
  mGuiRenderer = core::Ptr<GuiRenderer>(new GuiRenderer(core::Ptr<GuiServer>(this)));
}

GuiServer::~GuiServer() {
  
}


const core::Ptr<GuiRenderer> GuiServer::get_renderer() const {
  return mGuiRenderer;
}

GuiWindow *GuiServer::create_window(GuiWindow *guiWindow, core::Window *window) {
  guiWindow->initialize(window->get_native_window_handle(), mGuiRenderer->initialize_window_swapchain(window));

  // Fill locale containers
  mGuiWindows.push_back(guiWindow);
  mGuiWindowsMap.emplace(window->get_native_window_handle(), guiWindow);

  return guiWindow;
}

void GuiServer::initialize(GuiContext& guiContext) {
  mImGuiContext = ImGui::CreateContext();
  mGuiContext = &guiContext;
}

void GuiServer::shutdown() {
  for (auto* window: mGuiWindows) {
    window->get_swap_chain()->release();
    window->release();

    window = nullptr;
  }

  mGuiWindows.clear();
  mGuiWindowsMap.clear();

  mGuiRenderer->release();
  mGuiRenderer = nullptr;

  mGuiContext->release();
  mGuiContext = nullptr;
}


void GuiServer::draw() {
  for (GuiWindow* window: mGuiWindows) {
    // Initialize window frame
    mGuiRenderer->start_frame(window);

    // Initialize the internal window stuff, set the render target, scissor and set clear color
    rhi::Command::SetGraphicsRenderTarget::create(window->get_command_buffer(), window->get_swap_chain().get());
    
    {
      // get the window size
      core::uint32 width  = 1;
      core::uint32 height = 1;
      window->get_swap_chain()->get_width_and_height(width, height);

      // Set the graphics viewport and scissor rectangle
      rhi::Command::SetGraphicsViewportAndScissorRectangle::create(window->get_command_buffer(), 0, 0, width, height);
    }

    // Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
    rhi::Command::ClearGraphics::create(window->get_command_buffer(), rhi::ClearFlag::COLOR_DEPTH, core::Color4::GRAY);

    // Draw the window
    window->draw();
    
    mGuiRenderer->end_frame(window);
    
    // Done, present everything

    // Push to renderer command buffer
    window->get_command_buffer().append_to_command_buffer_and_clear(mGuiRenderer->get_command_buffer());

    // Call the rendering draw method
    mGuiRenderer->draw(window);

    // Present everything
    window->get_swap_chain()->present();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}