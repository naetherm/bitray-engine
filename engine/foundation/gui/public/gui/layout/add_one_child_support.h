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
#include "gui/layout/children.h"
#include "gui/layout/slot.h"


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
/**
 * @class
 * AddOneChildSupport
 *
 * @tparam TSlotType
 */
template<typename TSlotType>
class AddOneChildSupport : public Children, public TSlot<TSlotType> {
public:

  /**
   * @brief
   * Default constructor.
   */
  AddOneChildSupport();

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  AddOneChildSupport(const AddOneChildSupport& rhs);

  /**
   * @brief
   * Destructor.
   */
  ~AddOneChildSupport() override;


  /**
   * @brief
   * Returns pointer to child widget at index position @p index.
   *
   * @param[in] index
   * Index position.
   *
   * @return
   * Pointer to widget.
   */
  core::Ptr<Widget> get_child_at_index(core::uint32 index) override;

  /**
   * @brief
   * Returns pointer to child widget at index position @p index.
   *
   * @param[in] index
   * Index position.
   *
   * @return
   * Pointer to widget.
   */
  const core::Ptr<Widget> get_child_at_index(core::uint32 index) const override;

  /**
   * @brief
   * Returns number of child widgets.
   *
   * @return
   * Number of child widgets.
   */
  core::int32 get_num_of_children() const override;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // gui


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "gui/layout/add_one_child_support.inl"