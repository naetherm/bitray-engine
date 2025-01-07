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
#include "core/core.h"
#include "core/math/rect.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Display
 *
 * @brief
 * Display representation
 */
class Display {
public:
  /**
   * @brief
   * Constructor.
   */
  Display() = default;

  /**
   * @brief
   * Constructor.
   *
   * @param[in] shape
   * The shape of the display
   * @param[in] dpiFactor
   * The dpi factor.
   */
  inline explicit Display(const Recti& shape, float dpiFactor = 96);

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  inline Display(const Display& rhs);

  /**
   * @brief
   * Destructor.
   */
  ~Display() = default;


  /**
   * @brief
   * Compare operators.
   * @param rhs
   * @return
   */
  [[nodiscard]] inline bool operator==(const Display& rhs) const;
  [[nodiscard]] inline bool operator!=(const Display& rhs) const;

  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * Object to copy.
   *
   * @return
   * Reference to this instance.
   */
  inline Display& operator=(const Display& rhs);


  /**
   * @brief
   * Returns the shape.
   *
   * @return
   * The shape
   */
  [[nodiscard]] inline const Recti& get_shape() const;

  /**
   * @brief
   * Returns the dpi factor.
   *
   * @return
   * The dpi factor.
   */
  [[nodiscard]] inline float get_dpi_factor() const;

  /**
   * @brief
   * Sets the shape.
   * @param[in] shape
   * The shape.
   */
  inline void set_shape(const Recti& shape);

  /**
   * @brief
   * Sets the dpi factor.
   *
   * @param[in] dpiFactor
   * The dpi factor.
   */
  inline void set_dpi_factor(float dpiFactor);

protected:
  /** The shape of the display */
  Recti mShape;
  /** DPI factor of the display */
  float mDpiFactor;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/frontend/display.inl"