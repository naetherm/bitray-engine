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
#include "core/math/mat44.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
Mat33<TType>::Mat33()
: xx(TType(1)), yx(TType(0)), zx(TType(0))
, xy(TType(0)), yy(TType(1)), zy(TType(0))
, xz(TType(0)), yz(TType(0)), zz(TType(1)) {
}

template<typename TType>
Mat33<TType>::Mat33(const TType fS[]) {
  Memory::copy(mM, fS, sizeof(TType) * 9);
}

template<typename TType>
Mat33<TType>::Mat33(const Mat33& other) {
  Memory::copy(mM, other.mM, sizeof(TType) * 9);
}

template<typename TType>
Mat33<TType>::Mat33(const Mat44<TType>& other) {
}

template<typename TType>
Mat33<TType>::Mat33(TType xx, TType xy, TType xz, TType yx, TType yy, TType yz, TType zx, TType zy, TType zz)
: xx(xx), yx(yx), zx(zx)
, xy(xy), yy(yy), zy(zy)
, xz(xz), yz(yz), zz(zz) {
}

template<typename TType>
Mat33<TType>::~Mat33() {
}

template<typename TType>
Mat33<TType>& Mat33<TType>::operator=(const Mat33<TType>& other) {
  Memory::copy(mM, other.mM, sizeof(TType) * 9);
}

template<typename TType>
Mat33<TType>& Mat33<TType>::operator=(const TType m[]) {
  Memory::copy(mM, m, sizeof(TType) * 9);
}

template<typename TType>
bool Mat33<TType>::operator==(const Mat33<TType>& other) const {
  for (sizeT i = 0; i < 9; i++) {
    if (!Math::near(mM[i], other.mM[i])) {
      return false;
    }
  }
  return true;
}

template<typename TType>
bool Mat33<TType>::operator!=(const Mat33<TType>& other) const {
  return !(*this == other);
}

template<typename TType>
Mat33<TType> Mat33<TType>::operator+(const Mat33<TType>& other) const {
  return Mat44<TType>(
    xx + other.xx, xy + other.xy, xz + other.xz,
    yx + other.yx, yy + other.yy, yz + other.yz,
    zx + other.zx, zy + other.zy, zz + other.zz
  );
}

template<typename TType>
void Mat33<TType>::operator+=(const Mat33<TType>& other) {
  for (sizeT i = 0; i < 9; i++) {
    mM[i] += other.mM[i];
  }
}

template<typename TType>
Mat33<TType> Mat33<TType>::operator-() const {
  return get_inverted();
}

template<typename TType>
Mat33<TType> Mat33<TType>::operator-(const Mat33<TType>& other) const {
  return Mat44<TType>(
    xx - other.xx, xy - other.xy, xz - other.xz,
    yx - other.yx, yy - other.yy, yz - other.yz,
    zx - other.zx, zy - other.zy, zz - other.zz
  );
}

template<typename TType>
void Mat33<TType>::operator-=(const Mat33<TType>& other) {
  for (sizeT i = 0; i < 9; i++) {
    mM[i] -= other.mM[i];
  }
}

template<typename TType>
Mat33<TType> Mat33<TType>::operator*(const Mat33<TType>& other) const {
}

template<typename TType>
void Mat33<TType>::operator*=(const Mat33<TType>& other) {
	*this = *this * other;
}

template<typename TType>
Mat33<TType> Mat33<TType>::operator*(TType m) const {
  return Mat44<TType>(
    xx * m, xy * m, xz * m,
    yx * m, yy * m, yz * m,
    zx * m, zy * m, zz * m
  );
}

template<typename TType>
void Mat33<TType>::operator*=(TType m) {
  for (sizeT i = 0; i < 9; i++) {
    mM[i] *= m;
  }
}

template<typename TType>
Mat33<TType> Mat33<TType>::operator/(TType s) const {
  // Sanity check
  BE_ASSERT(s != TType(0), "Divide by zero")

  return Mat44<TType>(
    xx / s, xy / s, xz / s,
    yx / s, yy / s, yz / s,
    zx / s, zy / s, zz / s
  );
}

template<typename TType>
void Mat33<TType>::operator/=(TType s) {
  // Sanity check
  BE_ASSERT(s != TType(0), "Divide by zero")

  for (sizeT i = 0; i < 9; i++) {
    mM[i] /= s;
  }
}

template<typename TType>
TType Mat33<TType>::operator[](uint8 index) const {
  // Sanity check
  BE_ASSERT(index < 9, "Index out of range")

  return mM[index];
}

template<typename TType>
TType& Mat33<TType>::operator[](uint8 index) {
  // Sanity check
  BE_ASSERT(index < 9, "Index out of range")

  return mM[index];
}

template<typename TType>
TType Mat33<TType>::operator()(uint8 row, uint8 column) const {
  // Sanity check
  BE_ASSERT(row < 3, "Row index out of range")
  BE_ASSERT(column < 3, "Column index out of range")

  return mM33[column][row];
}

template<typename TType>
TType& Mat33<TType>::operator()(uint8 row, uint8 column) {
  // Sanity check
  BE_ASSERT(row < 3, "Row index out of range")
  BE_ASSERT(column < 3, "Column index out of range")

  return mM33[column][row];
}

template<typename TType>
Mat33<TType>::operator TType*() {
  return mM;
}

template<typename TType>
Mat33<TType>::operator const TType*() const {
  return mM;
}

template<typename TType>
bool Mat33<TType>::is_zero() const {
  return (Math::near(xx, TType(0)) && Math::near(xy, TType(0)) && Math::near(xz, TType(0)) &&
          Math::near(yx, TType(0)) && Math::near(yy, TType(0)) && Math::near(yz, TType(0)) &&
          Math::near(zx, TType(0)) && Math::near(zy, TType(0)) && Math::near(zz, TType(0)));
}

template<typename TType>
bool Mat33<TType>::is_true_zero() const {
  return Memory::compare(mM, Zero.mM, sizeof(TType) * 9) == 0;
}

template<typename TType>
void Mat33<TType>::set_zero() {
  Memory::set(mM, TType(0), sizeof(TType) * 9);
}

template<typename TType>
bool Mat33<TType>::is_identity() const {
  return (Math::near(xx, TType(1)) && Math::near(xy, TType(0)) && Math::near(xz, TType(0)) &&
          Math::near(yx, TType(0)) && Math::near(yy, TType(1)) && Math::near(yz, TType(0)) &&
          Math::near(zx, TType(0)) && Math::near(zy, TType(0)) && Math::near(zz, TType(1)));
}

template<typename TType>
bool Mat33<TType>::is_true_identity() const {
  return Memory::compare(mM, Identity.mM, sizeof(TType) * 9) == 0;
}

template<typename TType>
void Mat33<TType>::set_identity() {
  Memory::set(mM, Identity.mM, sizeof(TType) * 9);
}

template<typename TType>
void Mat33<TType>::set(TType xx, TType xy, TType xz, TType yx, TType yy, TType yz, TType zx, TType zy, TType zz) {
  this->xx = xx; this->xy = xy; this->xz = xz;
  this->yx = yx; this->yy = yy; this->yz = yx;
  this->zx = zx; this->zy = zy; this->zz = zz;
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_row(core::uint8 row) const {
  // Sanity check
  BE_ASSERT(row < 3, "Invalid row index");

  return Vec4<TType>(mM[row + 3*0], mM[row + 3*1], mM[row + 3*2]);
}

template<typename TType>
void Mat33<TType>::set_row(core::uint8 rowIndex, const Vec3<TType>& row) {
  // Sanity check
  BE_ASSERT(rowIndex < 3, "Invalid row index");

  mM[rowIndex + 3*0] = row.x;
  mM[rowIndex + 3*1] = row.y;
  mM[rowIndex + 3*2] = row.z;
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_column(core::uint8 column) const {
  // Sanity check
  BE_ASSERT(column < 3, "Invalid column index");

  return Vec3<TType>(mM[column*3 + 0], mM[column*3 + 1], mM[column*3 + 2]);
}

template<typename TType>
void Mat33<TType>::set_column(core::uint8 columnIndex, const Vec3<TType>& column) {
  // Sanity check
  BE_ASSERT(column < 3, "Invalid column index")

  mM[columnIndex*3 + 0] = column.x;
  mM[columnIndex*3 + 1] = column.y;
  mM[columnIndex*3 + 2] = column.z;
}

template<typename TType>
bool Mat33<TType>::is_symmetric() const {
  return (Math::near(xy, yx) && Math::near(xz, zx) && Math::near(yz, zy));
}

template<typename TType>
bool Mat33<TType>::is_orthogonal() const {
  return get_transposed() == get_inverted();
}

template<typename TType>
bool Mat33<TType>::is_rotation_matrix() const {
  return is_orthogonal() && Math::near(get_determinant(), TType(1));
}

template<typename TType>
TType Mat33<TType>::get_trace() const {
  return xx+yy+zz;
}

template<typename TType>
TType Mat33<TType>::get_determinant() const {
}

template<typename TType>
void Mat33<TType>::transpose() {
  // xy <-> yx
  TType fT = xy;
  xy = yx;
  yx = fT;

  // xz <-> zx
  fT = xz;
  xz = zx;
  zx = fT;

  // yz <-> zy
  fT = yz;
  yz = zy;
  zy = fT;
}

template<typename TType>
Mat33<TType> Mat33<TType>::get_transposed() const {
  return Mat33(xx, yx, zx,
           xy, yy, zy,
           xz, yz, zz);
}

template<typename TType>
bool Mat33<TType>::invert() {
  // First, calculate the determinant of the matrix
  TType fDet = get_determinant();

  // Check for null to avoid division by null
  if (fDet) {
    // Calculate the inverse of the matrix using Cramers rule. Same as matrix3x4,
    // but we ignore the translation.
    fDet = TType(1)/fDet;
    Set(fDet*(yy*zz + zy*-yz), fDet*(zy*xz + xy*-zz), fDet*(xy*yz + yy*-xz),
      fDet*(yz*zx + zz*-yx), fDet*(zz*xx + xz*-zx), fDet*(xz*yx + yz*-xx),
      fDet*(yx*zy - zx* yy), fDet*(zx*xy - xx* zy), fDet*(xx*yy - yx* xy));
  } else {
    // For sure, set identity matrix
    set_identity();

    // Error!
    return false;
  }
  //	}

  // Done
  return true;
}

template<typename TType>
Mat33<TType> Mat33<TType>::get_inverted() const {
  // First, calculate the determinant of the matrix
  TType fDet = get_determinant();

  // Check for null to avoid division by null
  if (fDet) {
    // Calculate the inverse of the matrix using Cramers rule. Same as matrix3x4,
    // but we ignore the translation.
    fDet = TType(1)/fDet;
    return Matrix3x3(fDet*(yy*zz + zy*-yz), fDet*(zy*xz + xy*-zz), fDet*(xy*yz + yy*-xz),
             fDet*(yz*zx + zz*-yx), fDet*(zz*xx + xz*-zx), fDet*(xz*yx + yz*-xx),
             fDet*(yx*zy - zx* yy), fDet*(zx*xy - xx* zy), fDet*(xx*yy - yx* xy));
  }

  return Identity;
}

template<typename TType>
void Mat33<TType>::set_scale_matrix(TType x, TType y, TType z) {
  xx =    x; xy = TType(0); xz = TType(0);
  yx = TType(0); yy =    y; yz = TType(0);
  zx = TType(0); zy = TType(0); zz =    z;
}

template<typename TType>
void Mat33<TType>::set_scale_matrix(const Vec3<TType>& scale) {
  xx = scale.x; xy = TType(0); xz = TType(0);
  yx = TType(0); yy = scale.y; yz = TType(0);
  zx = TType(0); zy = TType(0); zz = scale.z;
}

template<typename TType>
void Mat33<TType>::get_scale(TType& x, TType& y, TType& z) const {
  // Optimized version
  x = Math::sqrt(xx*xx + yx*yx + zx*zx);
  y = Math::sqrt(xy*xy + yy*yy + zy*zy);
  z = Math::sqrt(xz*xz + yz*yz + zz*zz);
  if (get_determinant() < TType(0)) {
    x = -x;
    y = -y;
    z = -z;
  }
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_scale() const {
}

template<typename TType>
void Mat33<TType>::get_scale(TType v[]) const {
	get_scale(v[0], v[1], v[2]);
}

template<typename TType>
void Mat33<TType>::from_euler_angle_x(TType x) {
  TType fSin = Math::sin(x);
  TType fCos = Math::cos(x);
  xx = TType(1); xy = TType(0); xz =  TType(0);
  yx = TType(0); yy = fCos; yz = -fSin;
  zx = TType(0); zy = fSin; zz =  fCos;
}

template<typename TType>
void Mat33<TType>::from_euler_angle_y(TType y) {
  TType fSin = Math::sin(y);
  TType fCos = Math::cos(y);
  xx =  fCos; xy = TType(0); xz = fSin;
  yx =  TType(0); yy = TType(1); yz = TType(0);
  zx = -fSin; zy = TType(0); zz = fCos;
}

template<typename TType>
void Mat33<TType>::from_euler_angle_z(TType z) {
  TType fSin = Math::sin(z);
  TType fCos = Math::cos(z);
  xx = fCos; xy = -fSin; xz = TType(0);
  yx = fSin; yy =  fCos; yz = TType(0);
  zx = TType(0); zy =  TType(0); zz = TType(1);
}

template<typename TType>
void Mat33<TType>::to_axis_angle(TType& x, TType& y, TType& z, TType& angle) const {
  TType fTrace = xx + yy + zz;
  TType fCos   = TType(0.5)*(fTrace-TType(1));
  angle = Math::acos(fCos);  // In [0, Math::Pi]

  if (angle > TType(0)) {
    if (angle < Math::Pi) {
      x = zy-yz;
      y = xz-zx;
      z = yx-xy;

      // Normalize the axis
      // Avoid division through zero...
      TType fU = x*x + y*y + z*z;
      if (fU) {
        fU = Math::sqrt(fU);
        if (fU) {
          // Scale
          float fScale = TType(1)/fU;
          x *= fScale;
          y *= fScale;
          z *= fScale;
        }
      }
    } else {
      // Angle is Math::Pi
      if (xx >= yy) {
        // 00 >= 11
        if (xx >= zz) {
          // 00 is maximum diagonal term
        x = TType(0.5)*Math::sqrt(xx - yy - zz + TType(1));
          float fHalfInverse = TType(0.5)/x;
          y = fHalfInverse*xy;
          z = fHalfInverse*xz;
        } else {
          // 22 is maximum diagonal term
          z = TType(0.5)*Math::sqrt(zz - xx - yy + TType(1));
          float fHalfInverse = TType(0.5)/z;
          x = fHalfInverse*xz;
          y = fHalfInverse*yz;
        }
      } else {
        // 11 > 00
        if (yy >= zz) {
          // 11 is maximum diagonal term
          y = TType(0.5)*Math::sqrt(yy - xx - zz + TType(1));
          float fHalfInverse  = TType(0.5)/y;
          x = fHalfInverse*xy;
          z = fHalfInverse*xz;
        } else {
          // 22 is maximum diagonal term
          z = TType(0.5)*Math::sqrt(zz - xx - yy + TType(1));
          float fHalfInverse = TType(0.5)/z;
          x = fHalfInverse*xz;
          y = fHalfInverse*yz;
        }
      }
    }
  } else {
    // Angle is 0 and the matrix is the identity. So, we can choose any axis...
    x = TType(0);
    y = TType(0);
    z = TType(1);
  }
}

template<typename TType>
void Mat33<TType>::from_axis_angle(TType x, TType y, TType z, TType angle) const {
  TType fRSin = Math::sin(angle);
  TType fRCos = Math::cos(angle);
  xx =     fRCos + x*x*(1-fRCos); xy = -z*fRSin + x*y*(1-fRCos); xz =  y*fRSin + x*z*(1-fRCos);
  yx =  z*fRSin + y*x*(1-fRCos); yy =     fRCos + y*y*(1-fRCos); yz = -x*fRSin + y*z*(1-fRCos);
  zx = -y*fRSin + z*x*(1-fRCos); zy =  x*fRSin + z*y*(1-fRCos); zz =     fRCos + z*z*(1-fRCos);
}

template<typename TType>
void Mat33<TType>::from_axis_angle(const Vec3<TType>& axis, TType angle) const {
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_x_axis() const {
	return Vec3<TType>(xx, yx, zx);
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_y_axis() const {
	return Vec3<TType>(xy, yy, zy);
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_z_axis() const {
	return Vec3<TType>(xz, yz, zz);
}

template<typename TType>
void Mat33<TType>::to_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const {
  x.x = xx; y.x = xy; z.x = xz;
  x.y = yx; y.y = yy; z.y = yz;
  x.z = zx; y.z = zy; z.z = zz;
}

template<typename TType>
void Mat33<TType>::from_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const {
  xx = x.x; xy = y.x; xz = z.x;
  yx = x.y; yy = y.y; yz = z.y;
  zx = x.z; zy = y.z; zz = z.z;
}

template<typename TType>
Mat33<TType>& Mat33<TType>::look_at(const Vec3<TType>& eye, Vec3<TType>& at, Vec3<TType>& up) {
  Vec3<TType> vZAxis = (eye - at).get_normalized();
  Vec3<TType> vXAxis = up.cross(vZAxis).get_normalized();
  Vec3<TType> vYAxis = vZAxis.cross(vXAxis);

  // Setup matrix
  xx = vXAxis.x; xy = vXAxis.y; xz = vXAxis.z;
  yx = vYAxis.x; yy = vYAxis.y; yz = vYAxis.z;
  zx = vZAxis.x; zy = vZAxis.y; zz = vZAxis.z;

  // Return this matrix
  return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}