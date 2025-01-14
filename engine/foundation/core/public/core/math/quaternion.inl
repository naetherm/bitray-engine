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


template<typename TType>
const Quaternion<TType> Quaternion<TType>::Zero = (TType(0), TType(0), TType(0), TType(0));
template<typename TType>
const Quaternion<TType> Quaternion<TType>::Identity = (TType(0), TType(0), TType(0), TType(1));


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
Quaternion<TType>::Quaternion()
: x(TType(0)), y(TType(0)), z(TType(0)), w(TType(1)) {

}

template<typename TType>
Quaternion<TType>::Quaternion(TType x, TType y, TType z, TType w)
: x(x), y(y), z(z), w(w) {

}

template<typename TType>
Quaternion<TType>::Quaternion(const TType* q)
: x(q[0]), y(q[1]), z(q[2]), w(q[3]) {

}

template<typename TType>
Quaternion<TType>::Quaternion(const Quaternion<TType>& q)
: x(q.x), y(q.y), z(q.z), w(q.w) {

}

template<typename TType>
Quaternion<TType>::Quaternion(const Vec3<TType>& axis, TType angle) {

}

template<typename TType>
Quaternion<TType>::~Quaternion() {

}


template<typename TType>
Quaternion<TType>& Quaternion<TType>::operator=(const Quaternion<TType>& q) {
  x = q.x;
  y = q.y;
  z = q.z;
  w = q.w;
  return *this;
}

template<typename TType>
bool Quaternion<TType>::operator==(const Quaternion<TType>& q) const {
  return (x == q.x && y == q.y && z == q.z && w == q.w);
}

template<typename TType>
bool Quaternion<TType>::operator!=(const Quaternion<TType>& q) const {
  return !(*this == q);
}


template<typename TType>
Quaternion<TType> Quaternion<TType>::operator+(const Quaternion<TType>& q) const {
  return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::operator+=(const Quaternion<TType>& q) {
  x += q.x;
  y += q.y;
  z += q.z;
  w += q.w;
  return *this;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::operator-() const {
  TType norm = x*x + y*y + z*z + w*w;
  if (norm > TType(0)) {
    TType invNorm = 1 / norm;
    return Quaternion<TType>(-x * invNorm, -y * invNorm, -z * invNorm, w * invNorm);
  }
  return Quaternion<TType>::Zero;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::operator-(const Quaternion<TType>& q) const {
  return Quaternion<TType>(x - q.x, y - q.y, z - q.z, w - q.w);
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::operator-=(const Quaternion<TType>& q) {
  x -= q.x;
  y -= q.y;
  z -= q.z;
  w -= q.w;
  return *this;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::operator*(const Quaternion<TType>& q) const {

}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::operator*=(const Quaternion<TType>& q) {

}

template<typename TType>
Quaternion<TType> Quaternion<TType>::operator*(TType s) const {
  return Quaternion<TType>(x * s, y * s, z * s, w * s);
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::operator*=(TType s) {
  x *= s;
  y *= s;
  z *= s;
  w *= s;
  return *this;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::operator/(TType s) const {
  return Quaternion<TType>(x / s, y / s, z / s, w / s);
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::operator/=(TType s) {
  x /= s;
  y /= s;
  z /= s;
  w /= s;
  return *this;
}

template<typename TType>
Quaternion<TType>::operator TType*() {
  return q;
}

template<typename TType>
Quaternion<TType>::operator const TType*() const {
  return q;
}


template<typename TType>
Quaternion<TType>& Quaternion<TType>::set(TType x, TType y, TType z, TType w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
  return *this;
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::set(const TType values[]) {
  x = values[0];
  y = values[1];
  z = values[2];
  w = values[3];
  return *this;
}

template<typename TType>
void Quaternion<TType>::to_axis_angle(Vec3<TType>& axis, TType& angle) const {
  // Avoid division through zero...
  TType fSqrLength = x*x + y*y + z*z;
  if (fSqrLength) {
    fSqrLength = Math::sqrt(fSqrLength);
    if (fSqrLength) {
      TType fInvLength = TType(1) / fSqrLength;
      axis.x     = x*fInvLength;
      axis.y     = y*fInvLength;
      axis.z     = z*fInvLength;
      angle = TType(2) * Math::acos(w);

      // Done
      return;
    }
  }

  // Fallback...
  axis.x     = TType(0);
  axis.y     = TType(0);
  axis.z     = TType(1);
  angle = TType(0);
}

template<typename TType>
void Quaternion<TType>::to_axis_angle(TType& x, TType& y, TType& z, TType& angle) const {
  // Avoid division through zero...
  TType fSqrLength = this->x * this->x + this->y * this->y + this->z * this->z;
  if (fSqrLength) {
    fSqrLength = Math::sqrt(fSqrLength);
    if (fSqrLength) {
      TType fInvLength = TType(1) / fSqrLength;
      x     = this->x * fInvLength;
      y     = this->y * fInvLength;
      z     = this->z * fInvLength;
      angle = TType(2) * Math::acos(w);

      // Done
      return;
    }
  }

  // Fallback...
  x     = TType(0);
  y     = TType(0);
  z     = TType(1);
  angle = TType(0);
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::from_axis_angle(const Vec3<TType>& axis, TType angle) {
  // Check whether the angle is 0, in that case we do not need to calculate sin/cos stuff...
  if (Math::abs(angle) < Math::BE_EPSILON) {
    this->w = 1.0f;
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
  } else {
    TType fSin = Math::sin(angle * TType(0.5));
    this->w = Math::cos(angle * TType(0.5));
    this->x = axis.x * fSin;
    this->y = axis.y * fSin;
    this->z = axis.z * fSin;
  }

  return *this;
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::from_axis_angle(TType x, TType y, TType z, TType angle) {
  // Check whether the angle is 0, in that case we do not need to calculate sin/cos stuff...
  if (Math::abs(angle) < Math::BE_EPSILON) {
    this->w = 1.0f;
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
  } else {
    TType fSin = Math::sin(angle * TType(0.5));
    this->w = Math::cos(angle * TType(0.5));
    this->x = x * fSin;
    this->y = y * fSin;
    this->z = z * fSin;
  }

  return *this;
}

template<typename TType>
Vec3<TType> Quaternion<TType>::get_x_axis() const {

}

template<typename TType>
Vec3<TType> Quaternion<TType>::get_y_axis() const {

}

template<typename TType>
Vec3<TType> Quaternion<TType>::get_z_axis() const {

}


template<typename TType>
TType Quaternion<TType>::get_length() const {
  return Math::sqrt(get_squared_length());
}

template<typename TType>
TType Quaternion<TType>::get_squared_length() const {
  return x*x + y*y + z*z + w*w;
}

template<typename TType>
TType Quaternion<TType>::dot(const Quaternion<TType>& q) const {
  return x * q.x + y * q.y + z * q.z + w * q.w;
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::normalize() {
  TType length = get_length();
  if (length > TType(0)) {
    length = Math::sqrt(length);
    if (length) {
      const TType invLength = 1 / length;
      x *= invLength;
      y *= invLength;
      z *= invLength;
      w *= invLength;
    }
  }
  return *this;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::get_normalized() const {
  TType length = get_length();
  if (length > TType(0)) {
    length = Math::sqrt(length);
    if (length) {
      const TType invLength = 1 / length;
      return Quaternion<TType>(x * invLength, y * invLength, z * invLength, w * invLength);
    }
  }
  return Quaternion<TType>::Zero;
}

template<typename TType>
void Quaternion<TType>::conjugate() {
  x = -x;
  y = -y;
  z = -z;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::get_conjugated() const {
  return Quaternion<TType>(-x, -y, -z, w);
}

template<typename TType>
void Quaternion<TType>::invert() {
  const TType length = get_length();
  if (length > TType(0)) {
    const TType invLength = 1 / length;
    x = -x * invLength;
    y = -y * invLength;
    z = -z * invLength;
    w *= invLength;
  } else {
    x = y = z = w = TType(0);
  }
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::get_inverted() const {
  const TType length = get_length();
  if (length > TType(0)) {
    const TType invLength = 1 / length;
    return Quaternion<TType>(-x * invLength, -y * invLength, -z * invLength, w * invLength);
  }
  return Quaternion<TType>::Zero;
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::exp() {
  TType om = x * x + y * y + z * z;
  if (om) {
    om = Math::sqrt(om);
    if (om) {
      const TType sinOm = Math::sin(om) / om;
      x *= sinOm;
      y *= sinOm;
      z *= sinOm;
      w = Math::cos(om);
    }
  }
  return *this;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::get_exp() const {
  TType om = x * x + y * y + z * z;
  if (om) {
    om = Math::sqrt(om);
    if (om) {
      const TType sinOm = Math::sin(om) / om;
      return Quaternion<TType>(x * sinOm, y * sinOm, z * sinOm, Math::cos(om));
    }
  }
  // Fallback
  return Quaternion<TType>(x, y, z, Math::cos(om));
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::log() {
  const TType theta = Math::acos(w);
  const TType sinTheta = Math::sin(theta);
  if (Math::abs(sinTheta) > TType(0)) {
    x /= sinTheta * theta;
    y /= sinTheta * theta;
    z /= sinTheta * theta;
    w = TType(0);
  }
  return *this;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::get_log() const {
  const TType theta = Math::acos(w);
  const TType sinTheta = Math::sin(theta);
  if (Math::abs(sinTheta) > TType(0)) {
    return Quaternion<TType>(x / sinTheta * theta, y / sinTheta * theta, z / sinTheta * theta, Math::cos(theta));
  }
  return Quaternion<TType>(x, y, z, TType(0));
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::power(TType power) {
  log();
  x *= power;
  y *= power;
  z *= power;
  w *= power;
  exp();
  return *this;
}

template<typename TType>
Quaternion<TType> Quaternion<TType>::get_power(TType power) const {
  return (get_log() * power).get_exp();
}

template<typename TType>
void Quaternion<TType>::slerp(const Quaternion<TType>& q1, const Quaternion<TType>& q2, TType t) {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}