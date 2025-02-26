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
  private/gui/clipboard.cpp
  private/gui/clipboard_impl.cpp
  private/gui/graphics.cpp
  private/gui/graphics_impl.cpp
  private/gui/gui.cpp
  private/gui/gui_impl.cpp
  private/gui/gui_message.cpp
  private/gui/data/clipboard_data.cpp
  private/gui/event/gui_event.cpp

  # private/layout
  private/layout/layout.cpp

  # private/menu
  private/menu/menu.cpp
  private/menu/menu_item.cpp

  # private/theme
  private/theme/theme.cpp
  private/theme/theme_server.cpp

  # private/widget
  private/widget/widget.cpp
)