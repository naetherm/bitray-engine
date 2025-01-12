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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/math/math.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {

template<typename TType> const Vec4<TType> Vec4<TType>::Zero = Vec4<TType>(TType(0), TType(0), TType(0), TType(0));
template<typename TType> const Vec4<TType> Vec4<TType>::One = Vec4<TType>(TType(1), TType(1), TType(1), TType(1));


template<typename TType>
Vec4<TType>::Vec4()
: x(0), y(0), z(0), w(TType(1)) {

}

template<typename TType>
Vec4<TType>::Vec4(const Vec4 &rhs)
: x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {

}

template<typename TType>
Vec4<TType>::Vec4(TType x, TType y, TType z, TType w)
: x(x), y(y), z(z), w(w) {

}

template<typename TType>
Vec4<TType>::Vec4(const TType v[])
: x(v[0]), y(v[1]), z(v[2]), w(v[3]) {

}

template<typename TType>
Vec4<TType>::~Vec4() {

}


template<typename TType>
Vec4<TType> &Vec4<TType>::operator=(const TType *f) {
  x = f[0];
  y = f[1];
  z = f[2];
  w = f[3];
  return *this;
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator=(const Vec4 &v) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = v.w;
  return *this;
}

template<typename TType>
bool Vec4<TType>::operator==(const Vec4 &v) const {
  return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

template<typename TType>
bool Vec4<TType>::operator!=(const Vec4 &v) const {
  return !operator==(v);
}

template<typename TType>
bool Vec4<TType>::operator<=(const Vec4 &v) const {
  return ((x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w));
}

template<typename TType>
bool Vec4<TType>::operator>=(const Vec4 &v) const {
  return ((x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w));
}

template<typename TType>
bool Vec4<TType>::operator<(const Vec4 &v) const {
  return ((x < v.x) && (y < v.y) && (z < v.z) && (w < v.w));
}

template<typename TType>
bool Vec4<TType>::operator>(const Vec4 &v) const {
  return ((x > v.x) && (y > v.y) && (z > v.z) && (w > v.w));
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator+=(const Vec4 &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  w += v.w;
  return *this;
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator+=(TType t) {
  x += t;
  y += t;
  z += t;
  w += t;
  return *this;
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator+(const Vec4 &v) const {
  return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator+(TType t) const {
  return Vec4(x + t, y + t, z + t, w + t);
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator-() const {
  return Vec4(-x, -y, -z, -w);
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator-=(const Vec4 &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  w -= v.w;
  return *this;
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator-=(TType t) {
  x -= t;
  y -= t;
  z -= t;
  w -= t;
  return *this;
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator-(const Vec4 &v) const {
  return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator-(TType t) const {
  return Vec4(x - t, y - t, z - t, w - t);
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator*(const Vec4 &v) const {
  return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator*(TType t) const {
  return Vec4(x * t, y * t, z * t, w * t);
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator*=(const Vec4 &v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  w *= v.w;
  return *this;
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator*(TType t) {
  x *= t;
  y *= t;
  z *= t;
  w *= t;
  return *this;
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator/(const Vec4 &v) const {
  return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);
}

template<typename TType>
Vec4<TType> Vec4<TType>::operator/(TType t) const {
  return Vec4(x / t, y / t, z / t, w / t);
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator/=(const Vec4 &v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  w /= v.w;
  return *this;
}

template<typename TType>
Vec4<TType> &Vec4<TType>::operator/=(TType t) {
  x /= t;
  y /= t;
  z /= t;
  w /= t;
  return *this;
}

template<typename TType>
Vec4<TType>::operator TType *() {
  return &v[0];
}

template<typename TType>
Vec4<TType>::operator const TType *() const {
  return &v[0];
}

template<typename TType>
const TType& Vec4<TType>::operator[](int i) const {
  return v[i];
}

template<typename TType>
TType& Vec4<TType>::operator[](int i) {
  return v[i];
}


template<typename TType>
const TType *Vec4<TType>::get_ptr() const {
  return &v[0];
}

template<typename TType>
TType *Vec4<TType>::get_ptr() {
  return &v[0];
}


template<typename TType>
void Vec4<TType>::get_xyz(TType &x, TType &y, TType& z) const {
  x = this->x;
  y = this->y;
  z = this->z;
}

template<typename TType>
void Vec4<TType>::get_xyzw(TType &x, TType &y, TType& z, TType& w) const {
  x = this->x;
  y = this->y;
  z = this->z;
  w = this->w;
}

template<typename TType>
TType Vec4<TType>::get_x() const {
  return x;
}

template<typename TType>
TType Vec4<TType>::get_y() const {
  return y;
}

template<typename TType>
TType Vec4<TType>::get_z() const {
  return z;
}

template<typename TType>
TType Vec4<TType>::get_w() const {
  return w;
}

template<typename TType>
void Vec4<TType>::set_xyz(TType x, TType y, TType z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

template<typename TType>
void Vec4<TType>::set_xyzw(TType x, TType y, TType z, TType w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

template<typename TType>
void Vec4<TType>::set_xyz(const TType *v) {
  this->x = v[0];
  this->y = v[1];
  this->z = v[2];
}

template<typename TType>
void Vec4<TType>::set_xyzw(const TType *v) {
  this->x = v[0];
  this->y = v[1];
  this->z = v[2];
  this->w = v[3];
}

template<typename TType>
void Vec4<TType>::set_x(TType x) {
  this->x = x;
}

template<typename TType>
void Vec4<TType>::set_y(TType y) {
  this->y = y;
}

template<typename TType>
void Vec4<TType>::set_z(TType z) {
  this->z = z;
}

template<typename TType>
void Vec4<TType>::set_w(TType w) {
  this->w = w;
}

template<typename TType>
void Vec4<TType>::inc_xyz(TType x, TType y, TType z) {
  this->x += x;
  this->y += y;
  this->z += z;
}

template<typename TType>
void Vec4<TType>::inc_xyzw(TType x, TType y, TType z, TType w) {
  this->x += x;
  this->y += y;
  this->z += z;
  this->w += w;
}

template<typename TType>
void Vec4<TType>::inc_xyz(const TType *v) {
  this->x += v[0];
  this->y += v[1];
  this->z += v[2];
}

template<typename TType>
void Vec4<TType>::inc_xyzw(const TType *v) {
  this->x += v[0];
  this->y += v[1];
  this->z += v[2];
  this->w += v[3];
}

template<typename TType>
void Vec4<TType>::inc_x(TType x) {
  this->x += x;
}

template<typename TType>
void Vec4<TType>::inc_y(TType y) {
  this->y += y;
}

template<typename TType>
void Vec4<TType>::inc_z(TType z) {
  this->z += z;
}

template<typename TType>
void Vec4<TType>::inc_w(TType w) {
  this->w += w;
}

template<typename TType>
bool Vec4<TType>::is_null() const {
  return ((x == TType(0)) && (y == TType(0)) && (z == TType(0)) && (w == TType(0)));
}

template<typename TType>
void Vec4<TType>::invert() {
  x = -x;
  y = -y;
  z = -z;
  w = -w;
}

template<typename TType>
Vec4<TType> Vec4<TType>::get_inverted() const {
  return Vec4(-x, -y, -z, -w);
}

template<typename TType>
TType Vec4<TType>::get_length() const {
  return core::sqrt(get_squared_length());
}

template<typename TType>
TType Vec4<TType>::get_squared_length() const {
  return x * x + y * y + z * z + w * w;
}

template<typename TType>
void Vec4<TType>::set_length(TType length) {
  TType u = x * x + y * y + z * z + w * w;
  if (u) {
    u = core::sqrt(u);
    if (u) {
      const TType s = length/u;

      x *= s;
      y *= s;
      z *= s;
      w *= s;
    }
  }
}

template<typename TType>
Vec4<TType> &Vec4<TType>::normalize() {
  TType u = x * x + y * y + z * z + w * w;
  if (u) {
    u = core::sqrt(u);
    if (u) {
      const TType s = TType(1)/u;

      x *= s;
      y *= s;
      z *= s;
      w *= s;
    }
  }
  return *this;
}

template<typename TType>
Vec4<TType> Vec4<TType>::get_normalized() const {
  TType u = x * x + y * y + z * z + w * w;
  if (u) {
    u = core::sqrt(u);
    if (u) {
      const TType s = TType(1)/u;

      return Vec4(x * s, y * s, z * s, w * s);
    }
  }
  return Vec4<TType>::Zero;
}

template<typename TType>
TType Vec4<TType>::get_distance(const Vec4 &o) const {
  return core::sqrt(get_squared_distance(o));
}

template<typename TType>
TType Vec4<TType>::get_squared_distance(const Vec4 &o) const {
  const TType dx = o.x - x;
  const TType dy = o.y - y;
  const TType dz = o.z - z;
  const TType dw = o.w - w;
  return dx * dx + dy * dy + dz * dz + dw * dw;
}

template<typename TType>
TType Vec4<TType>::dot(const Vec4 &o) const {
  return x * o.x + y * o.y + z * o.z + w * o.w;
}

template<typename TType>
TType Vec4<TType>::get_angle(const Vec4 &o) const {
  return core::acos(dot(o) / (get_length() * o.get_length()));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core