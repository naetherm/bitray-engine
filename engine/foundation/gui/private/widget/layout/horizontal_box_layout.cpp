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
#include "gui/widget/layout/horizontal_box_layout.h"
#include <imgui.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
HorizontalBoxLayout::Slot::Slot() {

}

HorizontalBoxLayout::Slot::Slot(core::Ptr<Widget> widget)
: BoxLayout::BoxLayoutSlot(widget) {

}

HorizontalBoxLayout::Slot::~Slot() {

}



HorizontalBoxLayout::Slot& HorizontalBoxLayout::Slot::operator[](core::Ptr<Widget> widget) {
  BoxLayout::BoxLayoutSlot::operator[](widget);

  return *this;
}


HorizontalBoxLayout::HorizontalBoxLayout() {

}

HorizontalBoxLayout::~HorizontalBoxLayout() {

}


void HorizontalBoxLayout::construct(ConstructionArguments args) {
  for (core::uint32 i = 0; i < args.getSlots().size(); ++i) {
    mChildren.add(args.getSlotByIndex(i));
  }
}

HorizontalBoxLayout::Slot &HorizontalBoxLayout::add_slot() {
  HorizontalBoxLayout::Slot* slot = new HorizontalBoxLayout::Slot();

  mChildren.add(slot);

  return *slot;
}

HorizontalBoxLayout::Slot &HorizontalBoxLayout::insert_slot(core::uint64 index) {
  if (index == INVALID_HANDLE) {
    return add_slot();
  }

  HorizontalBoxLayout::Slot& slot = *(new HorizontalBoxLayout::Slot());

  mChildren.insert(&slot, index);

  return slot;
}

HorizontalBoxLayout::Slot& HorizontalBoxLayout::make_slot(core::Ptr<Widget> widget) {
  HorizontalBoxLayout::Slot* slot = new HorizontalBoxLayout::Slot(widget);

  mChildren.add(slot);

  return *slot;
}


void HorizontalBoxLayout::on_update(float deltaTime) {
  for (core::uint32 i = 0; i < get_num_of_slots(); ++i) {
    mChildren[i].get_widget()->on_update(deltaTime);
  }
}

void HorizontalBoxLayout::on_draw() {
  // Act, dependent on number of children
  core::uint32 numChildren = mChildren.get_num_of_children();

  if (numChildren == 1) {
    mChildren[0].get_widget()->on_draw();
  } else if (numChildren > 1) {
    for (core::uint32 i = 0; i < numChildren - 1; ++i) {
      mChildren[i].get_widget()->on_draw();
      ImGui::SameLine();
    }
    mChildren[numChildren - 1].get_widget()->on_draw();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}