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
#include "gui/widget/layout/layout.h"
#include "gui/layout/slot.h"
#include "gui/layout/panel_children.h"


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
 * BoxLayout
 *
 * @brief
 */
class BoxLayout : public Layout {

public:

  /**
   * @class
   * BoxLayoutSlot
   *
   * @brief
   * Generic box layout slot definition.
   */
  class BoxLayoutSlot : public TSlot<BoxLayoutSlot> {
  public:

    /**
     * @brief
     * Default constructor.
     */
    BoxLayoutSlot();

    /**
     * @brief
     * constructor.
     *
     * @param[in] widget
     * Pointer to the widget to assign to this slot.
     */
    BoxLayoutSlot(core::Ptr<Widget> widget);

    /**
     * @brief
     * Destructor.
     */
    ~BoxLayoutSlot() override;
  };

public:

  /**
   * @brief
   * Constructor.
   */
  BoxLayout();

  /**
   * @brief
   * Destructor.
   */
  ~BoxLayout() override;

  /**
   * @brief
   * Returns the number of slots within the layout.
   *
   * @return
   * The number of slots within the layout.
   */
  core::uint32 get_num_of_slots() const;

  void clear();

protected:

  /** List of all children of this box layout */
  PanelChildren<BoxLayoutSlot> mChildren;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}