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
#include "gui/gui.h"
#include <core/core/server_impl.h>
#include <core/core/ptr.h>
#include <core/container/map.h>
#include <core/container/vector.h>
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
class GuiContext;
class GuiRenderer;
class GuiWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class GuiServer : public core::ServerImpl {
  
  friend class GuiRenderer;
public:
  
  static GuiServer* instance();
  
public:
  
  GuiServer();
  
  ~GuiServer() override;


  [[nodiscard]] const core::Ptr<GuiRenderer> get_renderer() const;


  GuiWindow* create_window(GuiWindow* guiWindow, core::Window* window);

public:

  void initialize(GuiContext& guiContext);

  void shutdown();

public:

  void draw();
  
private:
  /** Pointer to the gui renderer implementation */
  core::Ptr<GuiRenderer> mGuiRenderer;

  GuiContext* mGuiContext;

  core::Vector<GuiWindow*> mGuiWindows;
  core::Map<core::handle, GuiWindow*> mGuiWindowsMap;


  /** Pointer to imgui context instance, must be valid! */
  ImGuiContext*	   mImGuiContext;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}