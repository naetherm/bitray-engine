////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2023 RacoonStudios
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
#include "gui/widget/button/button.h"
#include "gui/helper/imgui_helper.h"
#include <imgui_internal.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Button::Button() {
  SignalClicked.connect(&SlotOnClicked);
  SignalHovered.connect(&SlotOnFocused);
}

Button::~Button() {

}


void Button::construct(Button::ConstructionArguments args) {
  mLabel = args.getText();
  mSize = args.getSize();
  mDisabled = args.getDisabled();
  mTooltip = args.getTooltip();
  mShowTooltip = args.getShowTooltip();
  SlotOnClicked = args.mEventSlotOnClicked;
  SlotOnFocused = args.mEventSlotOnFocused;
}


void Button::on_update(float deltaTime) {
  // Nothing to do here
}

void Button::on_draw() {

  ImGui::BeginDisabled(mDisabled);
  if (ImGui::ButtonEx(mLabel, ImGuiHelper::to_imvec2(mSize))) {
    SignalClicked();
  }

  if (mIsHovered = ImGui::IsItemHovered()) {
    SignalHovered();

    if (mShowTooltip) {
      show_tooltip();
    }
  }

  if (mDisabled) {
    ImGui::EndDisabled();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // gui
