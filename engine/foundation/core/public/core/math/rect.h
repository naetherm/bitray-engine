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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template<typename TType> class Vec2;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Rect
 *
 * @brief
 * A generic rectangle class representing a 2D rectangular area, defined by
 * its position(x, y), width(w),and height(h). Provides utility functions
 * for rectangle properties and comparisons.
 *
 * @tparam TType The type of the coordinates and dimensions(e.g., int, float).
 */
template<typename TType>
class Rect {
public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes the rectangle with default values (typically zero).
   */
  Rect();

  /**
   * @brief
   * Copy constructor.
   *
   * Creates a new rectangle as a copy of the given rectangle.
   *
   * @param[in] rhs The rectangle to copy.
   */
  Rect(const Rect<TType>& rhs);

  /**
   * @brief
   * Parameterized constructor.
   *
   * Initializes the rectangle with specified position and dimensions.
   *
   * @param[in] x The x-coordinate of the rectangle's top-left corner.
   * @param[in] y The y-coordinate of the rectangle's top-left corner.
   * @param[in] w The width of the rectangle.
   * @param[in] h The height of the rectangle.
   */
  Rect(TType x, TType y, TType w, TType h);

  /**
   * @brief
   * Destructor.
   *
   * Cleans up any resources used by the rectangle (if applicable).
   */
  ~Rect();

  /**
   * @brief
   * Copy assignment operator.
   *
   * Assigns the values of another rectangle to this rectangle.
   *
   * @param[in] rhs The rectangle to copy values from.
   *
   * @return A reference to this rectangle.
   */
  Rect<TType>& operator=(const Rect<TType>& rhs);

  /**
   * @brief
   * Equality operator.
   *
   * Checks if two rectangles are equal in position and dimensions.
   *
   * @param[in] rhs The rectangle to compare against.
   *
   * @return True if the rectangles are equal, false otherwise.
   */
  bool operator==(const Rect& rhs) const;

  /**
   * @brief
   * Inequality operator.
   *
   * Checks if two rectangles are not equal in position or dimensions.
   *
   * @param[in] rhs The rectangle to compare against.
   *
   * @return True if the rectangles are not equal, false otherwise.
   */
  bool operator!=(const Rect& rhs) const;

  /**
   * @brief
   * Gets the center point of the rectangle.
   *
   * Calculates the center of the rectangle based on its position and dimensions.
   *
   * @return A Vec2 object representing the center point.
   */
  Vec2<TType> get_center() const;

  /**
   * @brief
   * Gets the x-coordinate of the rectangle's top-left corner.
   *
   * @return The x-coordinate.
   */
  TType get_x() const;

  /**
   * @brief
   * Gets the y-coordinate of the rectangle's top-left corner.
   *
   * @return The y-coordinate.
   */
  TType get_y() const;

  /**
   * @brief
   * Gets the width of the rectangle.
   *
   * @return The width.
   */
  TType get_width() const;

  /**
   * @brief
   * Gets the height of the rectangle.
   *
   * @return The height.
   */
  TType get_height() const;

  /**
   * @brief
   * Calculates the surface area of the rectangle.
   *
   * @return The surface area (width * height).
   */
  TType calculate_surface() const;

public:

  TType x, y, w, h;
};

typedef Rect<core::int32> Recti;
typedef Rect<core::float32> Rectf;
typedef Rect<core::float64> Rectd;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/rect.inl"