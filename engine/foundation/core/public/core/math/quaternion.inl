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
#include "core/math/mat33.h"
#include "core/math/mat44.h"
#include "core/math/vec3.h"
#include "core/math/vec4.h"


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
  from_axis(axis, angle);
}

template<typename TType>
Quaternion<TType>::Quaternion(const Mat33<TType>& rotation) {
  from_rotation_matrix(rotation);
}

template<typename TType>
Quaternion<TType>::Quaternion(const Mat44<TType>& rotation) {
  from_rotation_matrix(rotation);
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
  return Quaternion(
w*q.x + x*q.w + y*q.z - z*q.y,
w*q.y + y*q.w + z*q.x - x*q.z,
w*q.z + z*q.w + x*q.y - y*q.x,
w*q.w - x*q.x - y*q.y - z*q.z);
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::operator*=(const Quaternion<TType>& q) {
  const TType qw = w, qx = x, qy = y, qz = z;
  w = qw*q.w - qx*q.x - qy*q.y - qz*q.z;
  x = qw*q.x + qx*q.w + qy*q.z - qz*q.y;
  y = qw*q.y + qy*q.w + qz*q.x - qx*q.z;
  z = qw*q.z + qz*q.w + qx*q.y - qy*q.x;
  return *this;
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
Vec3<TType> Quaternion<TType>::operator*(const Vec3<TType>& v) const {
  const TType x2 = x*x;
  const TType y2 = y*y;
  const TType z2 = z*z;
  const TType w2 = w*w;
  const TType xa = x*v.x;
  const TType yb = y*v.y;
  const TType zc = z*v.z;
  return Vec3<TType>(
    v.x*( x2 - y2 - z2 + w2) + 2*(w*(y*v.z - z*v.y) + x*(yb + zc)),
    v.y*(-x2 + y2 - z2 + w2) + 2*(w*(z*v.x - x*v.z) + y*(xa + zc)),
    v.z*(-x2 - y2 + z2 + w2) + 2*(w*(x*v.y - y*v.x) + z*(xa + yb)));
}

template<typename TType>
Vec4<TType> Quaternion<TType>::operator*(const Vec4<TType>& v) const {
  const TType x2 = x*x;
  const TType y2 = y*y;
  const TType z2 = z*z;
  const TType w2 = w*w;
  const TType xa = x*v.x;
  const TType yb = y*v.y;
  const TType zc = z*v.z;
  return Vec4<TType>(
    v.x*( x2 - y2 - z2 + w2) + 2*(w*(y*v.z - z*v.y) + x*(yb + zc)),
    v.y*(-x2 + y2 - z2 + w2) + 2*(w*(z*v.y - x*v.z) + y*(xa + zc)),
    v.z*(-x2 - y2 + z2 + w2) + 2*(w*(x*v.y - y*v.x) + z*(xa + yb)),
    TType(1));
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
    this->w = TType(1);
    this->x = TType(0);
    this->y = TType(0);
    this->z = TType(0);
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
    this->w = TType(1);
    this->x = TType(0);
    this->y = TType(0);
    this->z = TType(0);
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
  TType fTy  = TType(2)*y;
  TType fTz  = TType(2)*z;
  TType fTwy = fTy*w;
  TType fTwz = fTz*w;
  TType fTxy = fTy*x;
  TType fTxz = fTz*x;
  TType fTyy = fTy*y;
  TType fTzz = fTz*z;
  return Vector3(TType(1)-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
}

template<typename TType>
Vec3<TType> Quaternion<TType>::get_y_axis() const {
  TType fTx  = TType(2)*x;
  TType fTy  = TType(2)*y;
  TType fTz  = TType(2)*z;
  TType fTwx = fTx*w;
  TType fTwz = fTz*w;
  TType fTxx = fTx*x;
  TType fTxy = fTy*x;
  TType fTyz = fTz*y;
  TType fTzz = fTz*z;
  return Vector3(fTxy-fTwz, TType(1)-(fTxx+fTzz), fTyz+fTwx);
}

template<typename TType>
Vec3<TType> Quaternion<TType>::get_z_axis() const {
  TType fTx  = TType(2)*x;
  TType fTy  = TType(2)*y;
  TType fTz  = TType(2)*z;
  TType fTwx = fTx*w;
  TType fTwy = fTy*w;
  TType fTxx = fTx*x;
  TType fTxz = fTz*x;
  TType fTyy = fTy*y;
  TType fTyz = fTz*y;
  return Vector3(fTxz+fTwy, fTyz-fTwx, TType(1)-(fTxx+fTyy));
}


template<typename TType>
void Quaternion<TType>::to_rotation_matrix(Mat33<TType>& matrix) const {
  TType fTx  = TType(2)*x;
  TType fTy  = TType(2)*y;
  TType fTz  = TType(2)*z;
  TType fTwx = fTx*w;
  TType fTwy = fTy*w;
  TType fTwz = fTz*w;
  TType fTxx = fTx*x;
  TType fTxy = fTy*x;
  TType fTxz = fTz*x;
  TType fTyy = fTy*y;
  TType fTyz = fTz*y;
  TType fTzz = fTz*z;
  matrix.xx = TType(1)-(fTyy+fTzz); matrix.xy =       fTxy-fTwz;      matrix.xz =       fTxz+fTwy;
  matrix.yx =       fTxy+fTwz;      matrix.yy = TType(1)-(fTxx+fTzz); matrix.yz =       fTyz-fTwx;
  matrix.zx =       fTxz-fTwy;      matrix.zy =       fTyz+fTwx;      matrix.zz = TType(1)-(fTxx+fTyy);
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::from_rotation_matrix(const Mat33<TType>& matrix) {
  // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
  // article "Quaternion Calculus and Fast Animation"
  TType fTrace = matrix.xx+matrix.yy+matrix.zz;
  if (fTrace > TType(0)) {
    // |w| > 1/2, may as well choose w > 1/2
    TType fRoot = Math::sqrt(fTrace + TType(1));  // 2w
    w           = TType(0.5)*fRoot;
    fRoot       = TType(0.5)/fRoot;  // 1/(4w)
    x           = (matrix.zy-matrix.yz)*fRoot;
    y           = (matrix.xz-matrix.zx)*fRoot;
    z           = (matrix.yx-matrix.xy)*fRoot;
  } else {
    // |w| <= 1/2
    static size_t nNext[3] = { 1, 2, 0 };
    size_t i = 0;
    if (matrix.yy > matrix.xx)
      i = 1;
    if (matrix.zz > matrix.fM33[i][i])
      i = 2;
    size_t j = nNext[i];
    size_t k = nNext[j];

    TType fRoot = Math::sqrt(matrix.fM33[i][i]-matrix.fM33[j][j]-matrix.fM33[k][k] + TType(1));
    TType *apkQuat[3] = { &x, &y, &z };
    *apkQuat[i] = TType(0.5)*fRoot;
    fRoot = TType(0.5)/fRoot;
    w = (matrix.fM33[j][k]-matrix.fM33[k][j])*fRoot;
    *apkQuat[j] = (matrix.fM33[i][j]+matrix.fM33[j][i])*fRoot;
    *apkQuat[k] = (matrix.fM33[i][k]+matrix.fM33[k][i])*fRoot;
  }

  return *this;
}

template<typename TType>
void Quaternion<TType>::to_rotation_matrix(Mat44<TType>& matrix) const {
  TType fTx  = x+x;
  TType fTy  = y+y;
  TType fTz  = z+z;
  TType fTwx = fTx*w;
  TType fTwy = fTy*w;
  TType fTwz = fTz*w;
  TType fTxx = fTx*x;
  TType fTxy = fTy*x;
  TType fTxz = fTz*x;
  TType fTyy = fTy*y;
  TType fTyz = fTz*y;
  TType fTzz = fTz*z;
  matrix.xx = TType(1)-(fTyy+fTzz); matrix.xy =       fTxy-fTwz;      matrix.xz =       fTxz+fTwy;       matrix.xw = TType(0);
  matrix.yx =       fTxy+fTwz;      matrix.yy = TType(1)-(fTxx+fTzz); matrix.yz =       fTyz-fTwx;       matrix.yw = TType(0);
  matrix.zx =       fTxz-fTwy;      matrix.zy =       fTyz+fTwx;      matrix.zz = TType(1)-(fTxx+fTyy);  matrix.zw = TType(0);
}

template<typename TType>
Quaternion<TType>& Quaternion<TType>::from_rotation_matrix(const Mat44<TType>& matrix) {
  // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
  // article "Quaternion Calculus and Fast Animation"
  TType fTrace = matrix.xx+matrix.yy+matrix.zz;
  if (fTrace > TType(0)) {
    // |w| > 1/2, may as well choose w > 1/2
    TType fRoot = Math::sqrt(fTrace + TType(1));  // 2w
    w           = TType(0.5)*fRoot;
    fRoot       = TType(0.5)/fRoot;  // 1/(4w)
    x           = (matrix.zy-matrix.yz)*fRoot;
    y           = (matrix.xz-matrix.zx)*fRoot;
    z           = (matrix.yx-matrix.xy)*fRoot;
  } else {
    // |w| <= 1/2
    static size_t nNext[3] = { 1, 2, 0 };
    size_t i = 0;
    if (matrix.yy > matrix.xx)
      i = 1;
    if (matrix.zz > matrix.fM43[i][i])
      i = 2;
    size_t j = nNext[i];
    size_t k = nNext[j];

    TType fRoot = Math::sqrt(matrix.fM43[i][i]-matrix.fM43[j][j]-matrix.fM43[k][k] + TType(1));
    TType *apkQuat[3] = { &x, &y, &z };
    *apkQuat[i] = TType(0.5)*fRoot;
    fRoot = TType(0.5)/fRoot;
    w = (matrix.fM43[j][k]-matrix.fM43[k][j])*fRoot;
    *apkQuat[j] = (matrix.fM43[i][j]+matrix.fM43[j][i])*fRoot;
    *apkQuat[k] = (matrix.fM43[i][k]+matrix.fM43[k][i])*fRoot;
  }

  return *this;
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
  // TODO(naetherm): Not the best one of dealing with that here
#define W 3
#define X 0
#define Y 1
#define Z 2
  // From
  TType fFrom[4] = { q1.q[W], q1.q[X], q1.q[Y], q1.q[Z] };

  // To
  TType fTo[4] = { q2.q[W], q2.q[X], q2.q[Y], q2.q[Z] };

  // Calc cosine
  TType fCosom = fFrom[W]*fTo[W] + fFrom[X]*fTo[X] + fFrom[Y]*fTo[Y] + fFrom[Z]*fTo[Z];

  // Adjust signs (if necessary)
  TType fTo1[4];
  if (fCosom < 0.0f) {
    fCosom  = -fCosom;
    fTo1[W] = -fTo[W];
    fTo1[X] = -fTo[X];
    fTo1[Y] = -fTo[Y];
    fTo1[Z] = -fTo[Z];
  } else {
    fTo1[W] = fTo[W];
    fTo1[X] = fTo[X];
    fTo1[Y] = fTo[Y];
    fTo1[Z] = fTo[Z];
  }

  // Calculate coefficients
  TType fScale0, fScale1;
  if ((TType(1)-fCosom) > Math::BE_EPSILON) {
    // Standard case (slerp)
    TType fOmega = Math::acos(fCosom);
    TType fSinom = Math::sin(fOmega);
    fScale0 = Math::sin((TType(1) - t)*fOmega)/fSinom;
    fScale1 = Math::sin(t*fOmega)/fSinom;
  } else {
    // "fFrom" and "fTo" quaternions are very close
    //  ... so we can do a linear interpolation:
    fScale0 = TType(1) - t;
    fScale1 = t;
  }

  // Calculate final values
  w = fScale0*fFrom[W] + fScale1*fTo1[W];
  x = fScale0*fFrom[X] + fScale1*fTo1[X];
  y = fScale0*fFrom[Y] + fScale1*fTo1[Y];
  z = fScale0*fFrom[Z] + fScale1*fTo1[Z];

#undef W
#undef X
#undef Y
#undef Z
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}