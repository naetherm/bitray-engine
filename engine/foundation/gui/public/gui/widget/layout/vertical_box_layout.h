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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "gui/gui.h"
#include "gui/widget/layout/box_layout.h"
#include "gui/layout/slot.h"


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
class Gui;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * VerticalBoxLayout
 *
 * @brief
 * Layout wrapper for boxes that are ordered in horizontal direction.
 */
class VerticalBoxLayout : public BoxLayout {
public:

  /**
   * @class
   * Slot
   *
   * @brief
   * Slot system for horizontal boxes.
   */
  class Slot : public BoxLayout::BoxLayoutSlot {
  public:

    /**
     * @brief
     * Default constructor.
     */
    Slot();

    /**
     * @brief
     * Constructor.
     *
     * @param[in] widget
     * The widget to assign to this slot.
     */
    Slot(core::Ptr<Widget> widget);

    /**
     * @brief
     * Destructor.
     */
    ~Slot() override;

    /**
     * @brief
     * Assignment operator.
     *
     * @param[in] widget
     * Adds the widget to this slot.
     *
     * @return
     * Reference to this slot.
     */
    Slot& operator[](core::Ptr<Widget> widget);
  };

public:

  gui_begin_construction_args(VerticalBoxLayout)
    {}
    gui_slots(Slot)
  gui_end_construction_args()

public:

  /**
   * @brief
   * Constructor.
   */
  VerticalBoxLayout();

  /**
   * @brief
   * Destructor.
   */
  ~VerticalBoxLayout() override;


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
   * Adds a new slot empty slot and returns it as reference.
   *
   * @return
   * Reference to newly created slot.
   */
  Slot& add_slot();

  /**
   * @brief
   * Inserts a new empty slot as the given index position @p index.
   *
   * @param[in] index
   * The position where the new slot should be added.
   *
   * @return
   * Reference to newly created slot.
   */
  Slot& insert_slot(core::uint64 index = INVALID_HANDLE);

  /**
   * @brief
   * Creates a new slot and add a widget to that slot.
   *
   * @param[in] widget
   * Pointer to the widget to assign to this slot.
   *
   * @return
   * Reference to the widget.
   */
  Slot& make_slot(core::Ptr<Widget> widget);

public:

  /**
   * @brief
   * Called when the widget is updated.
   *
   * @param[in] deltaTime
   * The time between this and the last update in seconds.
   */
  void on_update(float deltaTime) override;

  /**
   * @brief
   * Called in the drawing process.
   */
  void on_draw() override;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}