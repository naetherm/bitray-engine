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
#include "gui/widget/widget.h"


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
 * Text
 *
 * @brief
 * Just a simple text without any further special functionality.
 */
class Text : public Widget {

  gui_begin_construction_args(Text)
    : mValueTooltip("")
    , mValueShowTooltip(false) {}
    gui_value(core::String, Text)
    gui_value(core::String, Tooltip)
    gui_value(bool, ShowTooltip)
  gui_end_construction_args()

public:

  /**
   * @brief
   * Default constructor.
   */
  Text();

  /**
   * @brief
   * Destructor.
   */
  ~Text() override;


  /**
   * @brief
   * Construct this widget.
   *
   * @param[in] args
   * The declaration data for this widget.
   */
  void construct(ConstructionArguments args);


  /**
   * @brief
   * Sets the text of the Text.h
   * @param test
   */
  void set_text(const core::String& text);

  /**
   * @brief
   * Returns a reference to the text of the Text.h
   *
   * @return
   * Reference to the text of the Text.h
   */
  const core::String& get_text() const;

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

  /**
   * @brief
   * Helper class for showing the tooltip information.
   */
  void show_tooltip();

protected:
  /** The text of the this basic Text */
  core::String mText;
  /** A tooltip text to show when hovered */
  core::String mTooltip;

  bool mShowTooltip;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // gui
