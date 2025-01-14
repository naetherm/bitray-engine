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
#include "vec3.h"
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
class Quaternion {
public:
  /** 0.0, 0.0, 0.0, 0.0 */
  static const Quaternion Zero;
  /** 0.0, 0.0, 0.0, 1.0 */
  static const Quaternion Identity;

public:

  Quaternion();

  Quaternion(TType x, TType y, TType z, TType w);

  explicit Quaternion(const TType q[]);

  Quaternion(const Quaternion& q);

  Quaternion(const Vec3<TType>& axis, TType angle);

  ~Quaternion();


  Quaternion& operator=(const Quaternion& q);

  bool operator==(const Quaternion& q) const;

  bool operator!=(const Quaternion& q) const;


  Quaternion operator+(const Quaternion& q) const;

  Quaternion& operator+=(const Quaternion& q);

  Quaternion operator-() const;

  Quaternion operator-(const Quaternion& q) const;

  Quaternion& operator-=(const Quaternion& q);

  Quaternion operator*(const Quaternion& q) const;

  Quaternion& operator*=(const Quaternion& q);

  Quaternion operator*(TType s) const;

  Quaternion& operator*=(TType s);

  Quaternion operator/(TType s) const;

  Quaternion& operator/=(TType s);

  operator TType*();

  operator const TType*() const;


  /**
   *  @brief
   *    Sets the component of the quaternion
   *
   *  @param[in] x
   *    X component
   *  @param[in] y
   *    Y component
   *  @param[in] z
   *    Z component
   *  @param[in] w
   *    W component
   *
   *  @return
   *    This quaternion
   */
  Quaternion& set(TType x, TType y, TType z, TType w);

  /**
   *  @brief
   *    Sets the component of the quaternion
   *
   *  @param[in] values
   *    Array of at least 4 floats
   *
   *  @return
   *    This quaternion
   */
  Quaternion& set(const TType values[]);

  /**
   *  @brief
   *    Returns a selected axis and angle from the rotation quaternion
   *
   *  @param[out] axis
   *    Will receive the x, y, and z component of the selected axis
   *  @param[out] angle
   *    Will receive the rotation angle around the selected axis (in radian, between [0, Math::Pi])
   *
   *  @note
   *    - The quaternion must be normalized
   */
  void to_axis_angle(Vec3<TType>& axis, TType& angle) const;

  /**
   *  @brief
   *    Returns a selected axis and angle from the rotation quaternion
   *
   *  @param[out] x
   *    Will receive the x component of the selected axis
   *  @param[out] y
   *    Will receive the y component of the selected axis
   *  @param[out] z
   *    Will receive the z component of the selected axis
   *  @param[out] angle
   *    Will receive the rotation angle around the selected axis (in radian, between [0, Math::Pi])
   *
   *  @note
   *    - The quaternion must be normalized
   */
  void to_axis_angle(TType& x, TType& y, TType& z, TType& angle) const;

  /**
   *  @brief
   *    Returns a selected axis and angle from the rotation quaternion
   *
   *  @param[out] x
   *    Will receive the x component of the selected axis
   *  @param[out] y
   *    Will receive the y component of the selected axis
   *  @param[out] z
   *    Will receive the z component of the selected axis
   *  @param[out] angle
   *    Will receive the rotation angle around the selected axis (in radian, between [0, Math::Pi])
   *
   *  @note
   *    - The quaternion must be normalized
   */
  Quaternion& from_axis_angle(const Vec3<TType>& axis, TType angle);

  /**
   *  @brief
   *    Sets a rotation quaternion by using a selected axis and angle
   *
   *  @param[in] x
   *    X component of the selected axis
   *  @param[in] y
   *    Y component of the selected axis
   *  @param[in] z
   *    Z component of the selected axis
   *  @param[in] angle
   *    Rotation angle around the selected axis (in radian, between [0, Math::Pi])
   *
   *  @return
   *    This quaternion
   *
   *  @note
   *    - The given selected axis must be normalized!
   */
  Quaternion& from_axis_angle(TType x, TType y, TType z, TType angle);

  /**
   *  @brief
   *    Returns the x (left) axis
   *
   *  @return
   *    The x (left) axis, already normalized for rotation quaternions
   */
  Vec3<TType> get_x_axis() const;

  /**
   *  @brief
   *    Returns the y (up) axis
   *
   *  @return
   *    The y (up) axis, already normalized for rotation quaternions
   */
  Vec3<TType> get_y_axis() const;

  /**
   *  @brief
   *    Returns the z (forward) axis
   *
   *  @return
   *    The z (forward) axis, already normalized for rotation quaternions
   */
  Vec3<TType> get_z_axis() const;


  /**
   *  @brief
   *    Returns the length (also called magnitude) of the quaternion
   *
   *  @return
   *     The length (also called magnitude) of the quaternion
   */
  TType get_length() const;

  /**
   *  @brief
   *    Returns the squared length (norm) of the quaternion
   *
   *  @return
   *     The squared length (norm) of the quaternion
   */
  TType get_squared_length() const;

  /**
   *  @brief
   *    Returns the dot product of this quaternion and another one
   *
   *  @param[in] q
   *    Second quaternion
   *
   *  @return
   *    The dot product of the two quaternions
   */
  TType dot(const Quaternion& q) const;

  /**
   *  @brief
   *    Normalizes the quaternion
   *
   *  @return
   *    This quaternion
   */
  Quaternion& normalize();

  /**
   *  @brief
   *    Returns a normalized quaternion
   *
   *  @return
   *    Normalized quaternion
   */
  Quaternion get_normalized() const;

  /**
   *  @brief
   *    Conjugates the quaternion
   */
  void conjugate();

  /**
   *  @brief
   *    Returns the conjugated of the quaternion
   *
   *  @return
   *    The conjugated of the quaternion
   */
  Quaternion get_conjugated() const;

  /**
   *  @brief
   *    Inverts the quaternion
   */
  void invert();

  /**
   *  @brief
   *    Returns the inverted quaternion
   *
   *  @return
   *    Inverted quaternion
   */
  Quaternion get_inverted() const;

  /**
   *  @brief
   *    Calculates the exponential of the quaternion
   *
   *  @return
   *    This quaternion
   */
  Quaternion& exp();

  /**
   *  @brief
   *    Returns the exponential of the quaternion
   *
   *  @return
   *    The exponential of the quaternion
   */
  Quaternion get_exp() const;

  /**
   *  @brief
   *    Calculates the logarithm of a unit quaternion
   *
   *  @return
   *    This quaternion
   */
  Quaternion& log();

  /**
   *  @brief
   *    Returns the logarithm of a unit quaternion
   *
   *  @return
   *    The logarithm of a unit quaternion
   */
  Quaternion get_log() const;

  /**
   *  @brief
   *    Calculates the power of the quaternion
   *
   *  @param[in] power
   *    Power to calculate
   *
   *  @return
   *    This quaternion
   */
  Quaternion& power(TType power);

  /**
   *  @brief
   *    Returns the power of the quaternion
   *
   *  @param[in] power
   *    Power to calculate
   *
   *  @return
   *    The power of the quaternion
   */
  Quaternion get_power(TType power) const;

  /**
   *  @brief
   *    Computes spherical linear interpolation between qQ1 and qQ2 with time 0-1
   *
   *  @param[in] q1
   *    Start quaternion (time: 0)
   *  @param[in] q2
   *    End quaternion (time: 1)
   *  @param[in] t
   *    Time from 0-1
   */
  void slerp(const Quaternion& q1, const Quaternion& q2, TType t);

public:
  /**
   * @brief
   * Some direct quaternion element accesses
   */
  union {
    /*
     * @brief
     * >=0 = vector, 3 = scalar (w)
     */
    float q[4];

    /*
     * @brief
     * w = scalar, x&y&z = vector
     */
    struct {
      float x, y, z, w;
    };
  };
};

typedef Quaternion<core::float32> Quaternionf;
typedef Quaternion<core::float64> Quaterniond;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/quaternion.inl"