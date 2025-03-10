#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2025 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////


set(FILES
  private/gui.cpp

  # private/application
  private/application/gui_application.cpp

  # private/gui
  private/gui/gui_context.cpp
  private/gui/gui_server.cpp
  private/gui/gui_window.cpp
  private/gui/gui_window_callbacks.cpp

  # private/helper
  private/helper/imgui_helper.cpp

  # private/layout
  private/layout/basic_slot.cpp
  private/layout/children.cpp
  private/layout/multi_slot.cpp
  private/layout/no_children.cpp
  private/layout/slot.cpp

  # private/renderer
  private/renderer/gui_renderer.cpp
  private/renderer/gui_texture.cpp
  private/renderer/gui_window_render_data.cpp

  # private/ui
  private/ui/ui.cpp

  # private/widget
  private/widget/widget.cpp
  private/widget/button/abstract_button.cpp
  private/widget/button/button.cpp
  private/widget/button/small_button.cpp
  private/widget/container/compound.cpp
  private/widget/container/null_widget.cpp
  private/widget/input/text_area_input.cpp
  private/widget/layout/box_layout.cpp
  private/widget/layout/horizontal_box_layout.cpp
  private/widget/layout/layout.cpp
  private/widget/layout/vertical_box_layout.cpp
  private/widget/panel/panel.cpp
  private/widget/text/text.cpp
)
