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
#include "quaternion.h"
#include "core/core.h"
#include "core/math/vec2.h"
#include "core/math/vec3.h"
#include "core/math/vec4.h"


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
class Mat44 {
public:

  static const Mat44 Zero;
  static const Mat44 Identity;

public:

  Mat44();

  Mat44(const TType m[]);

  Mat44(const Mat44<TType>& other);

  Mat44(
      TType xx, TType xy, TType xz, TType xw,
      TType yx, TType yy, TType yz, TType yw,
      TType zx, TType zy, TType zz, TType zw,
      TType wx, TType wy, TType wz, TType ww);

  ~Mat44();


  Mat44<TType>& operator=(const Mat44<TType>& other);

  Mat44<TType>& operator=(const TType m[]);

  bool operator==(const Mat44<TType>& other) const;

  bool operator!=(const Mat44<TType>& other) const;

  Mat44 operator+(const Mat44<TType>& other) const;

  void operator+=(const Mat44<TType>& other);

  Mat44 operator-() const;

  Mat44 operator-(const Mat44<TType>& other) const;

  void operator-=(const Mat44<TType>& other);

  Mat44 operator*(const Mat44<TType>& other) const;

  void operator*=(const Mat44<TType>& other);

  Mat44 operator*(TType m) const;

  void operator*=(TType m);

  Mat44 operator/(TType s) const;

  void operator/=(TType s);


  operator TType*();

  operator const TType*() const;


  bool is_zero() const;

  bool is_true_zero() const;

  void set_zero();

  bool is_identity() const;

  bool is_true_identity() const;

  void set_identity();

  void set(
      TType xx, TType xy, TType xz, TType xw,
      TType yx, TType yy, TType yz, TType yw,
      TType zx, TType zy, TType zz, TType zw,
      TType wx, TType wy, TType wz, TType ww);

  Vec4<TType> get_row(core::uint8 row) const;

  void set_row(core::uint8 rowIndex, const Vec4<TType>& row);

  Vec4<TType> get_column(core::uint8 column) const;

  void set_column(core::uint8 columnIndex, const Vec4<TType>& column);

  bool is_symmetric() const;

  bool is_orthogonal() const;

  bool is_rotation_matrix() const;

  bool is_rotation_translation_matrix() const;

  TType get_trace() const;

  TType get_determinant() const;


  void transpose();

  Mat44<TType> get_transposed() const;

  bool invert();

  Mat44<TType> get_inverted() const;

  void set_scale_matrix(TType x, TType y, TType z);

  void set_scale_matrix(const Vec3<TType>& scale);

  void get_scale(TType& x, TType& y, TType& z, TType& w) const;

  Vec3<TType> get_scale() const;

  void get_scale(TType v[]) const;

  void set_translation_matrix(TType x, TType y, TType z);

  void set_translation_matrix(const Vec3<TType>& translation);

  void get_translation(TType& x, TType& y, TType& z, TType& w) const;

  Vec3<TType> get_translation() const;

  void get_translation(TType v[]) const;

  void set_translation(TType x = TType(0), TType y = TType(0), TType z = TType(0));

  void set_translation(const Vec3<TType>& translation);

  void set_translation(const TType v[]);


  Vec3<TType> get_x_axis() const;

  Vec3<TType> get_y_axis() const;

  Vec3<TType> get_z_axis() const;

  void to_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const;

  void from_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const;


  Mat44<TType>& look_at(const Vec3<TType>& eye, Vec3<TType>& at, Vec3<TType>& up);

  Mat44<TType>& view(const Quaternion<TType>& rotation, const Vec3<TType>& translation);

  Mat44<TType>& perspective(TType w, TType h, TType zNear, TType zFar);

  Mat44<TType>& perspective_fov(TType fov, TType aspect, TType zNear, TType zFar);

  Mat44<TType>& perspective_infinite(TType w, TType h, TType zNear);

  Mat44<TType>& perspective_fov_infinite(TType fov, TType aspect, TType zNear);

  Mat44<TType>& perspective_off_center(TType l, TType r, TType b, TType t, TType zNear, TType zFar);

  Mat44<TType>& ortho(TType w, TType h, TType zNear, TType zFar);

  Mat44<TType>& ortho_off_center(TType l, TType r, TType t, TType b, TType zNear, TType zFar);

public:

  union {
    TType mM[16];

    struct {
      TType xx, yx, zx, wx;
      TType xy, yy, zy, wy;
      TType xz, yz, zz, wz;
      TType xw, yw, zw, ww;
    };

    TType mM44[4][4];
  };
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/mat44.inl"