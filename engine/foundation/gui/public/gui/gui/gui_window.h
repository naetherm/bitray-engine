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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
#include "gui/gui.h"
#include <core/core/refcounted.h>
#include <core/core/ptr.h>
#include <imgui.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace core {
class FrontendServer;
class Window;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiServer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class GuiWindow : public core::RefCounted {
public:

  GuiWindow(core::Ptr<GuiServer>& guiDevice);

  virtual ~GuiWindow();

  [[nodiscard]] const core::Window* get_window() const;

  [[nodiscard]] core::handle get_native_window_handle() const;

  [[nodiscard]] const core::FrontendServer* get_frontend() const;

  [[nodiscard]] const core::Ptr<rhi::RHISwapChain> get_swap_chain() const;

  [[nodiscard]] core::Ptr<rhi::RHISwapChain> get_swap_chain();

  [[nodiscard]] const rhi::RHICommandBuffer& get_command_buffer() const;

  [[nodiscard]] rhi::RHICommandBuffer& get_command_buffer();

  // Debugging
public:

public:

  void initialize(core::handle windowHandle, core::Ptr<rhi::RHISwapChain> swapChain);

public:

  virtual void draw();

public:

  ImGuiContext* mImGuiContext;

protected:
  /** Pointer to the global, owning gui device instance */
  core::Ptr<GuiServer> mGuiServer;
  /** Pointer to the native window */
  core::Window* mWindow;
  /** The native window id */
  core::handle mNativeWindowId;
  /** Pointer to the frontend */
  core::FrontendServer* mFrontend;
  /** Pointer to the swapchain used by this window for drawing */
  core::Ptr<rhi::RHISwapChain> mSwapChain;
  /** The command buffer for this window */
  rhi::RHICommandBuffer mCommandBuffer;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}