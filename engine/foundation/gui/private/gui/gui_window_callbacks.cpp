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
#include "gui/gui/gui_window_callback.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiWindowCallbacks::GuiWindowCallbacks()
: mImGuiIO(ImGui::GetIO()) {

}

GuiWindowCallbacks::~GuiWindowCallbacks() {

}

void GuiWindowCallbacks::on_tick(float deltaTime) {

}

void GuiWindowCallbacks::on_draw() {

}

void GuiWindowCallbacks::on_size(int x, int y) {
  mWindowSize.x = x;
  mWindowSize.y = y;
  WindowCallbacks::on_size(x, y);
}

void GuiWindowCallbacks::on_move(int x, int y) {

}


void GuiWindowCallbacks::on_leave() {

}

void GuiWindowCallbacks::on_enter() {

}

void GuiWindowCallbacks::on_mouse_position(int x, int y) {
  mImGuiIO.MousePos.x = static_cast<float>(x) * (mImGuiIO.DisplaySize.x / (float)mWindowSize.x);
  mImGuiIO.MousePos.y = static_cast<float>(y) * (mImGuiIO.DisplaySize.y / (float)mWindowSize.y);
}

void GuiWindowCallbacks::on_mouse_move(int x, int y) {
  mImGuiIO.MousePos.x = static_cast<float>(x) * (mImGuiIO.DisplaySize.x / (float)mWindowSize.x);
  mImGuiIO.MousePos.y = static_cast<float>(y) * (mImGuiIO.DisplaySize.y / (float)mWindowSize.y);
}

void GuiWindowCallbacks::on_mouse_wheel(float x) {
  mImGuiIO.MouseWheel += x;
}

void GuiWindowCallbacks::on_mouse_button_down(core::InputMouseItem inputMouseItem) {
  mImGuiIO.MouseDown[inputMouseItem] = true;
}

void GuiWindowCallbacks::on_mouse_button_up(core::InputMouseItem inputMouseItem) {
  mImGuiIO.MouseDown[inputMouseItem] = false;
}

void GuiWindowCallbacks::on_key_press(core::InputKeyboardItem key) {
  mImGuiIO.KeysDown[key] = true;
  mImGuiIO.AddInputCharacter(key);
}

void GuiWindowCallbacks::on_key_release(core::InputKeyboardItem key) {
  mImGuiIO.KeysDown[key] = false;

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}