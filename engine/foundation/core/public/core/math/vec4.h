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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
class Vec4 {
public:

  /** Constant representing a zero vector (0, 0, 0, 0). */
  static const Vec4 Zero;
  /** Constant representing a unit vector (1, 1, 1, 1). */
  static const Vec4 One;

public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes the vector to (0, 0).
   */
  Vec4();

  /**
   * @brief
   * Copy constructor.
   *
   * Creates a new vector as a copy of another vector.
   *
   * @param[in] rhs The vector to copy.
   */
  Vec4(const Vec4& rhs);

  /**
   * @brief
   * Parameterized constructor.
   *
   * Initializes the vector uniformly with a specific value.
   *
   * @param[in] value
   * The value to use for all entries.
   */
  Vec4(TType value);

  /**
   * @brief
   * Parameterized constructor.
   *
   * Initializes the vector with specified x and y components.
   *
   * @param[in] x The x-component of the vector.
   * @param[in] y The y-component of the vector.
   * @param[in] z The z-component of the vector.
   * @param[in] w The w-component of the vector.
   */
  Vec4(TType x, TType y, TType z, TType w = TType(1));

  /**
   * @brief
   * Constructor from an array.
   *
   * Initializes the vector with values from a given array.
   *
   * @param[in] v An array containing the x and y components.
   */
  explicit Vec4(const TType v[]);

  /**
   * @brief
   * Destructor.
   *
   * Cleans up the vector (if applicable).
   */
  ~Vec4();


  Vec4& operator=(const TType f[]);

  Vec4& operator=(const Vec4& v);

  bool operator==(const Vec4& v) const;
  bool operator!=(const Vec4& v) const;
  bool operator<=(const Vec4& v) const;
  bool operator>=(const Vec4& v) const;
  bool operator<(const Vec4& v) const;
  bool operator>(const Vec4& v) const;

  Vec4& operator+=(const Vec4& v);
  Vec4& operator+=(TType t);
  Vec4 operator+(const Vec4& v) const;
  Vec4 operator+(TType t) const;
  Vec4 operator-() const;
  Vec4& operator-=(const Vec4& v);
  Vec4& operator-=(TType t);
  Vec4 operator-(const Vec4& v) const;
  Vec4 operator-(TType t) const;

  Vec4 operator*(const Vec4& v) const;
  Vec4 operator*(TType t) const;
  Vec4& operator*=(const Vec4& v);
  Vec4& operator*(TType t);
  Vec4 operator/(const Vec4& v) const;
  Vec4 operator/(TType t) const;
  Vec4& operator/=(const Vec4& v);
  Vec4& operator/=(TType t);

  operator TType*();
  operator const TType*() const;

  const TType& operator[](int i) const;
  TType& operator[](int i);


  [[nodiscard]] const TType* get_ptr() const;

  [[nodiscard]] TType* get_ptr();

  void get_xyz(TType& x, TType& y, TType& z) const;

  void get_xyzw(TType& x, TType& y, TType& z, TType& w) const;

  [[nodiscard]] TType get_x() const;

  [[nodiscard]] TType get_y() const;

  [[nodiscard]] TType get_z() const;

  [[nodiscard]] TType get_w() const;

  void set_xyz(TType x, TType y, TType z);

  void set_xyzw(TType x, TType y, TType z, TType w = TType(1));

  void set_xyz(const TType v[]);

  void set_xyzw(const TType v[]);

  void set_x(TType x);

  void set_y(TType y);

  void set_z(TType z);

  void set_w(TType w);

  void inc_xyz(TType x, TType y, TType z);

  void inc_xyzw(TType x, TType y, TType z, TType w = TType(1));

  void inc_xyz(const TType v[]);

  void inc_xyzw(const TType v[]);

  void inc_x(TType x);

  void inc_y(TType y);

  void inc_z(TType z);

  void inc_w(TType w);


  [[nodiscard]] bool is_null() const;

  void invert();

  [[nodiscard]] Vec4 get_inverted() const;

  [[nodiscard]] TType get_length() const;

  [[nodiscard]] TType get_squared_length() const;

  void set_length(TType length = TType(1));

  Vec4& normalize();

  [[nodiscard]] Vec4 get_normalized() const;

  [[nodiscard]] TType get_distance(const Vec4& o) const;

  [[nodiscard]] TType get_squared_distance(const Vec4& o) const;

  [[nodiscard]] TType dot(const Vec4& o) const;

  [[nodiscard]] TType get_angle(const Vec4& o) const;
  
public:

  /**
   * @union
   * Represents the vector as an array or as individual x, y, z, and w components.
   */
  union {
    TType v[4]; ///< Array representation of the vector components.

    struct {
      TType x, y, z, w; ///< Individual components of the vector.
    };
  };

};

typedef Vec4<core::int32> Vec4i;
typedef Vec4<core::uint32> Vec4ui;
typedef Vec4<core::float32> Vec4f;
typedef Vec4<core::float64> Vec4d;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/vec4.inl"