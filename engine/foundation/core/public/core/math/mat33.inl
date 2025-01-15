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
}

template<typename TType>
Mat33<TType> Mat33<TType>::get_transposed() const {
}

template<typename TType>
bool Mat33<TType>::invert() {
}

template<typename TType>
Mat33<TType> Mat33<TType>::get_inverted() const {
}

template<typename TType>
void Mat33<TType>::set_scale_matrix(TType x, TType y, TType z) {
}

template<typename TType>
void Mat33<TType>::set_scale_matrix(const Vec3<TType>& scale) {
}

template<typename TType>
void Mat33<TType>::get_scale(TType& x, TType& y, TType& z) const {
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_scale() const {
}

template<typename TType>
void Mat33<TType>::get_scale(TType v[]) const {
}

template<typename TType>
void Mat33<TType>::from_euler_angle_x(TType x) {
}

template<typename TType>
void Mat33<TType>::from_euler_angle_y(TType y) {
}

template<typename TType>
void Mat33<TType>::from_euler_angle_z(TType z) {
}

template<typename TType>
void Mat33<TType>::to_axis_angle(TType& x, TType& y, TType& z, TType& angle) const {
}

template<typename TType>
void Mat33<TType>::from_axis_angle(TType x, TType y, TType z, TType angle) const {
}

template<typename TType>
void Mat33<TType>::from_axis_angle(const Vec3<TType>& axis, TType angle) const {
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_x_axis() const {
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_y_axis() const {
}

template<typename TType>
Vec3<TType> Mat33<TType>::get_z_axis() const {
}

template<typename TType>
void Mat33<TType>::to_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const {
}

template<typename TType>
void Mat33<TType>::from_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const {
}

template<typename TType>
Mat33<TType>& Mat33<TType>::look_at(const Vec3<TType>& eye, Vec3<TType>& at, Vec3<TType>& up) {
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}