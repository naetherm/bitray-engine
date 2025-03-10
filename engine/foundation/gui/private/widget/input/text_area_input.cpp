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
#include "gui/widget/input/text_area_input.h"
#include "gui/helper/imgui_helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
TextAreaInput::TextAreaInput() {
  SignalContentChanged.connect(&SlotOnContentChanged);
}

TextAreaInput::~TextAreaInput() {

}


void TextAreaInput::construct(ConstructionArguments args) {
  mContent = args.getContent();
  mSize = args.getSize();
  SlotOnContentChanged = args.mEventSlotOnContentChanged;
}

void TextAreaInput::on_update(float deltaTime) {

}

void TextAreaInput::on_draw() {
  ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

  char* tempContent = const_cast<char*>(mContent.c_str());
  static char text[1024 * 16] =
    "/*\n"
    " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
    " the hexadecimal encoding of one offending instruction,\n"
    " more formally, the invalid operand with locked CMPXCHG8B\n"
    " instruction bug, is a design flaw in the majority of\n"
    " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
    " processors (all in the P5 microarchitecture).\n"
    "*/\n\n"
    "label:\n"
    "\tlock cmpxchg8b eax\n";

  if (ImGui::InputTextMultiline(
    "##source",
    text, // mContent
    1024 * 16, ImVec2(mSize.get_x(), ImGui::GetTextLineHeight() * mSize.get_y()),
    flags)) {
    printf("When called?");
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // gui
