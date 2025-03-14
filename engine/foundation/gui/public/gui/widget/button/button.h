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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "gui/gui.h"
#include "gui/widget/button/abstract_button.h"
#include <core/math/vec2.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]


/**
 * @class
 * Button
 *
 * @brief
 * Widget that adds a newline.
 */
class Button : public AbstractButton {

  gui_begin_construction_args(Button)
    : mValueSize(core::Vec2i(0, 0))
    , mValueDisabled(false)
    , mValueTooltip("")
    , mValueShowTooltip(false) {}
    gui_value(core::String, Text)
    gui_value(core::Vec2i, Size)
    gui_value(bool, Disabled)
    gui_value(core::String, Tooltip)
    gui_value(bool, ShowTooltip)
    gui_event(core::Slot<>, SlotOnClicked)
    gui_event(core::Slot<>, SlotOnFocused)
  gui_end_construction_args()

public:

  /**
   * @brief
   * Default constructor.
   */
  Button();

  /**
   * @brief
   * Destructor.
   */
  ~Button() override;


  /**
   * @brief
   * Construct this widget.
   *
   * @param[in] args
   * The declaration data for this widget.
   */
  void construct(ConstructionArguments args);

public:

  /**
   * @brief
   * Called when the widget is updated.
   *
   * @param[in] deltaTime
   * The time between the this and the last update in seconds.
   */
  void on_update(float deltaTime) override;

  /**
   * @brief
   * Called in the drawing process.
   */
  void on_draw() override;

protected:
  /** The label of the button */
  core::String mLabel;
  /** The size of the button */
  core::Vec2i mSize;
  /** True if the button is currently disabled */
  bool mDisabled;
};
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // gui
