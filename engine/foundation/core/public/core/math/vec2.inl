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

template<typename TType> const Vec2<TType> Vec2<TType>::Zero = Vec2<TType>(TType(0), TType(0));
template<typename TType> const Vec2<TType> Vec2<TType>::One = Vec2<TType>(TType(1), TType(1));


template<typename TType>
Vec2<TType>::Vec2()
: x(0), y(0) {

}

template<typename TType>
Vec2<TType>::Vec2(const Vec2 &rhs)
: x(rhs.x), y(rhs.y) {

}

template<typename TType>
Vec2<TType>::Vec2(TType x, TType y)
: x(x), y(y) {

}

template<typename TType>
Vec2<TType>::Vec2(const TType v[])
: x(v[0]), y(v[1]) {

}

template<typename TType>
Vec2<TType>::~Vec2() {

}


template<typename TType>
Vec2<TType> &Vec2<TType>::operator=(const TType *f) {
  x = f[0];
  y = f[1];
  return *this;
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator=(const Vec2 &v) {
  x = v.x;
  y = v.y;
  return *this;
}

template<typename TType>
bool Vec2<TType>::operator==(const Vec2 &v) const {
  return ((x == v.x) && (y == v.y));
}

template<typename TType>
bool Vec2<TType>::operator!=(const Vec2 &v) const {
  return !operator==(v);
}

template<typename TType>
bool Vec2<TType>::operator<=(const Vec2 &v) const {
  return ((x <= v.x) && (y <= v.y));
}

template<typename TType>
bool Vec2<TType>::operator>=(const Vec2 &v) const {
  return ((x >= v.x) && (y >= v.y));
}

template<typename TType>
bool Vec2<TType>::operator<(const Vec2 &v) const {
  return ((x < v.x) && (y < v.y));
}

template<typename TType>
bool Vec2<TType>::operator>(const Vec2 &v) const {
  return ((x > v.x) && (y > v.y));
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator+=(const Vec2 &v) {
  x += v.x;
  y += v.y;
  return *this;
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator+=(TType t) {
  x += t;
  y += t;
  return *this;
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator+(const Vec2 &v) const {
  return Vec2(x + v.x, y + v.y);
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator+(TType t) const {
  return Vec2(x + t, y + t);
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator-() const {
  return Vec2(-x, -y);
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator-=(const Vec2 &v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator-=(TType t) {
  x -= t;
  y -= t;
  return *this;
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator-(const Vec2 &v) const {
  return Vec2(x - v.x, y - v.y);
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator-(TType t) const {
  return Vec2(x - t, y - t);
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator*(const Vec2 &v) const {
  return Vec2(x * v.x, y * v.y);
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator*(TType t) const {
  return Vec2(x * t, y * t);
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator*=(const Vec2 &v) {
  x *= v.x;
  y *= v.y;
  return *this;
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator*(TType t) {
  x *= t;
  y *= t;
  return *this;
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator/(const Vec2 &v) const {
  return Vec2(x / v.x, y / v.y);
}

template<typename TType>
Vec2<TType> Vec2<TType>::operator/(TType t) const {
  return Vec2(x / t, y / t);
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator/=(const Vec2 &v) {
  x /= v.x;
  y /= v.y;
  return *this;
}

template<typename TType>
Vec2<TType> &Vec2<TType>::operator/=(TType t) {
  x /= t;
  y /= t;
  return *this;
}

template<typename TType>
Vec2<TType>::operator TType *() {
  return &v[0];
}

template<typename TType>
Vec2<TType>::operator const TType *() const {
  return &v[0];
}

template<typename TType>
const TType& Vec2<TType>::operator[](int i) const {
  return v[i];
}

template<typename TType>
TType& Vec2<TType>::operator[](int i) {
  return v[i];
}


template<typename TType>
const TType *Vec2<TType>::get_ptr() const {
  return &v[0];
}

template<typename TType>
TType *Vec2<TType>::get_ptr() {
  return &v[0];
}


template<typename TType>
void Vec2<TType>::get_xy(TType &x, TType &y) const {
  x = this->x;
  y = this->y;
}

template<typename TType>
TType Vec2<TType>::get_x() const {
  return x;
}

template<typename TType>
TType Vec2<TType>::get_y() const {
  return y;
}

template<typename TType>
void Vec2<TType>::set_xy(TType x, TType y) {
  this->x = x;
  this->y = y;
}

template<typename TType>
void Vec2<TType>::set_xy(const TType *v) {
  this->x = v[0];
  this->y = v[1];
}

template<typename TType>
void Vec2<TType>::set_x(TType x) {
  this->x = x;
}

template<typename TType>
void Vec2<TType>::set_y(TType y) {
  this->y = y;
}

template<typename TType>
void Vec2<TType>::inc_xy(TType x, TType y) {
  this->x += x;
  this->y += y;
}

template<typename TType>
void Vec2<TType>::inc_xy(const TType *v) {
  this->x += v[0];
  this->y += v[1];
}

template<typename TType>
void Vec2<TType>::inc_x(TType x) {
  this->x += x;
}

template<typename TType>
void Vec2<TType>::inc_y(TType y) {
  this->y += y;
}

template<typename TType>
bool Vec2<TType>::is_null() const {
  return ((x == TType(0)) && (y == TType(y)));
}

template<typename TType>
void Vec2<TType>::invert() {
  x = -x;
  y = -y;
}

template<typename TType>
Vec2<TType> Vec2<TType>::get_inverted() const {
  return Vec2(-x, -y);
}

template<typename TType>
TType Vec2<TType>::get_length() const {
  return core::sqrt(get_squared_length());
}

template<typename TType>
TType Vec2<TType>::get_squared_length() const {
  return x * x + y * y;
}

template<typename TType>
void Vec2<TType>::set_length(TType length) {
  TType u = x * x + y * y;
  if (u) {
    u = core::sqrt(u);
    if (u) {
      const TType s = length/u;

      x *= s;
      y *= s;
    }
  }
}

template<typename TType>
Vec2<TType> &Vec2<TType>::normalize() {
  TType u = x * x + y * y;
  if (u) {
    u = core::sqrt(u);
    if (u) {
      const TType s = TType(1)/u;

      x *= s;
      y *= s;
    }
  }
  return *this;
}

template<typename TType>
Vec2<TType> Vec2<TType>::get_normalized() const {
  TType u = x * x + y * y;
  if (u) {
    u = core::sqrt(u);
    if (u) {
      const TType s = TType(1)/u;

      return Vec2(x * s, y * s);
    }
  }
  return Vec2<TType>::Zero;
}

template<typename TType>
TType Vec2<TType>::get_distance(const Vec2 &o) const {
  return core::sqrt(get_squared_distance(o));
}

template<typename TType>
TType Vec2<TType>::get_squared_distance(const Vec2 &o) const {
  const TType dx = o.x - x;
  const TType dy = o.y - y;
  return dx * dx + dy * dy;
}

template<typename TType>
TType Vec2<TType>::dot(const Vec2 &o) const {
  return x * o.x + y * o.y;
}

template<typename TType>
TType Vec2<TType>::get_angle(const Vec2 &o) const {
  return core::acos(dot(o) / (get_length() * o.get_length()));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core