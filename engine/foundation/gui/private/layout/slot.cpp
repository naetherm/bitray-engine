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
#include "gui/layout/slot.h"
#include "gui/widget/container/null_widget.h"
#include "gui/widget/widget.h"


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
Slot::Slot()
: mWidget(NullWidget::instance()) {

}

Slot::Slot(core::Ptr<Widget> widget)
: mWidget(widget) {

}

Slot::Slot(const Slot& rhs)
: mWidget(rhs.mWidget) {

}

Slot::~Slot() {

}


Slot& Slot::operator=(const Slot& rhs) {
  mWidget = rhs.mWidget;
  return *this;
}

bool Slot::operator==(const Slot &rhs) const {
  return (mWidget == rhs.mWidget);
}

bool Slot::operator!=(const Slot &rhs) const {
  return !operator==(rhs);
}

core::Ptr<Widget> Slot::attach_widget(core::Ptr<Widget> widget) {
  mWidget = widget;

  return this->mWidget;
}

core::Ptr<Widget> Slot::detach_widget() {
  mWidget = NullWidget::instance();

  return this->mWidget;
}

core::Ptr<Widget> Slot::get_widget() const {
  return mWidget;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // gui
