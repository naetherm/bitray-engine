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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Vec2
 *
 * @brief
 * A template class representing a 2D vector with various utility functions
 * for vector operations such as addition, subtraction, normalization,
 * and distance calculations.
 *
 * @tparam TType The type of the vector components (e.g., int, float).
 */
template<typename TType>
class Vec2 {
public:

  /** Constant representing a zero vector (0, 0). */
  static const Vec2 Zero;
  /** Constant representing a unit vector (1, 1). */
  static const Vec2 One;

public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes the vector to (0, 0).
   */
  Vec2();

  /**
   * @brief
   * Copy constructor.
   *
   * Creates a new vector as a copy of another vector.
   *
   * @param[in] rhs The vector to copy.
   */
  Vec2(const Vec2& rhs);

  /**
   * @brief
   * Parameterized constructor.
   *
   * Initializes the vector with specified x and y components.
   *
   * @param[in] x The x-component of the vector.
   * @param[in] y The y-component of the vector.
   */
  Vec2(TType x, TType y);

  /**
   * @brief
   * Constructor from an array.
   *
   * Initializes the vector with values from a given array.
   *
   * @param[in] v An array containing the x and y components.
   */
  explicit Vec2(const TType v[]);

  /**
   * @brief
   * Destructor.
   *
   * Cleans up the vector (if applicable).
   */
  ~Vec2();


  Vec2& operator=(const TType f[]);

  Vec2& operator=(const Vec2& v);

  bool operator==(const Vec2& v) const;
  bool operator!=(const Vec2& v) const;
  bool operator<=(const Vec2& v) const;
  bool operator>=(const Vec2& v) const;
  bool operator<(const Vec2& v) const;
  bool operator>(const Vec2& v) const;

  Vec2& operator+=(const Vec2& v);
  Vec2& operator+=(TType t);
  Vec2 operator+(const Vec2& v) const;
  Vec2 operator+(TType t) const;
  Vec2 operator-() const;
  Vec2& operator-=(const Vec2& v);
  Vec2& operator-=(TType t);
  Vec2 operator-(const Vec2& v) const;
  Vec2 operator-(TType t) const;

  Vec2 operator*(const Vec2& v) const;
  Vec2 operator*(TType t) const;
  Vec2& operator*=(const Vec2& v);
  Vec2& operator*(TType t);
  Vec2 operator/(const Vec2& v) const;
  Vec2 operator/(TType t) const;
  Vec2& operator/=(const Vec2& v);
  Vec2& operator/=(TType t);

  operator TType*();
  operator const TType*() const;

  const TType& operator[](int i) const;
  TType& operator[](int i);


  [[nodiscard]] const TType* get_ptr() const;

  [[nodiscard]] TType* get_ptr();

  void get_xy(TType& x, TType& y) const;

  [[nodiscard]] TType get_x() const;

  [[nodiscard]] TType get_y() const;

  void set_xy(TType x, TType y);

  void set_xy(const TType v[]);

  void set_x(TType x);

  void set_y(TType y);

  void inc_xy(TType x, TType y);

  void inc_xy(const TType v[]);

  void inc_x(TType x);

  void inc_y(TType y);


  [[nodiscard]] bool is_null() const;

  void invert();

  [[nodiscard]] Vec2 get_inverted() const;

  [[nodiscard]] TType get_length() const;

  [[nodiscard]] TType get_squared_length() const;

  void set_length(TType length = TType(1));

  Vec2& normalize();

  [[nodiscard]] Vec2 get_normalized() const;

  [[nodiscard]] TType get_distance(const Vec2& o) const;

  [[nodiscard]] TType get_squared_distance(const Vec2& o) const;

  [[nodiscard]] TType dot(const Vec2& o) const;

  [[nodiscard]] TType get_angle(const Vec2& o) const;

public:

  /**
   * @union
   * Represents the vector as an array or as individual x and y components.
   */
  union {
    TType v[2]; ///< Array representation of the vector components.

    struct {
      TType x, y; ///< Individual components of the vector.
    };
  };

};

typedef Vec2<core::int32> Vec2i;
typedef Vec2<core::uint32> Vec2ui;
typedef Vec2<float> Vec2f;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/vec2.inl"