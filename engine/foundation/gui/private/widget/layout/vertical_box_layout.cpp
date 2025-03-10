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
#include "gui/widget/layout/vertical_box_layout.h"
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
VerticalBoxLayout::Slot::Slot()
: BoxLayout::BoxLayoutSlot() {

}

VerticalBoxLayout::Slot::Slot(core::Ptr<Widget> widget)
: BoxLayout::BoxLayoutSlot(widget) {

}

VerticalBoxLayout::Slot::~Slot() {

}



VerticalBoxLayout::Slot& VerticalBoxLayout::Slot::operator[](core::Ptr<Widget> widget) {
  BoxLayout::BoxLayoutSlot::operator[](widget);

  return *this;
}


VerticalBoxLayout::VerticalBoxLayout() {

}

VerticalBoxLayout::~VerticalBoxLayout() {

}


void VerticalBoxLayout::construct(ConstructionArguments args) {
  for (core::uint32 i = 0; i < args.getSlots().size(); ++i) {
    mChildren.add(args.getSlotByIndex(i));
  }
}


void VerticalBoxLayout::on_update(float deltaTime) {
  for (core::uint32 i = 0; i < get_num_of_slots(); ++i) {
    mChildren[i].get_widget()->on_update(deltaTime);
  }
}

void VerticalBoxLayout::on_draw() {
  // Act, dependent on number of children
  core::uint32 numChildren = get_num_of_slots();

  if (numChildren == 1) {
    mChildren[0].get_widget()->on_draw();
  } else if (numChildren > 1) {
    for (core::uint32 i = 0; i < numChildren - 1; ++i) {
      mChildren[i].get_widget()->on_draw();
    }
    mChildren[numChildren - 1].get_widget()->on_draw();
  }
}

VerticalBoxLayout::Slot &VerticalBoxLayout::add_slot() {
  VerticalBoxLayout::Slot* slot = new VerticalBoxLayout::Slot();

  mChildren.add(slot);

  return *slot;
}

VerticalBoxLayout::Slot &VerticalBoxLayout::insert_slot(core::uint64 index) {
  if (index == INVALID_HANDLE) {
    return add_slot();
  }

  VerticalBoxLayout::Slot& slot = *(new VerticalBoxLayout::Slot());

  mChildren.insert(&slot, index);

  return slot;
}

VerticalBoxLayout::Slot& VerticalBoxLayout::make_slot(core::Ptr<Widget> widget) {
  VerticalBoxLayout::Slot* slot = new VerticalBoxLayout::Slot(widget);

  mChildren.add(slot);

  return *slot;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}