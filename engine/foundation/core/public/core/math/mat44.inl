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
#include "core/memory/memory.h"
#include "core/math/vec4.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
Mat44<TType>::Mat44() {
}

template<typename TType>
Mat44<TType>::Mat44(const TType m[]) {
}

template<typename TType>
Mat44<TType>::Mat44(const Mat44<TType>& other) {
}

template<typename TType>
Mat44<TType>::Mat44(TType xx, TType xy, TType xz, TType xw, TType yx, TType yy, TType yz, TType yw, TType zx, TType zy,
  TType zz, TType zw, TType wx, TType wy, TType wz, TType ww) {
}

template<typename TType>
Mat44<TType>::~Mat44() {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::operator=(const Mat44<TType>& other) {
  Memory::copy(mM, other.mM, sizeof(TType) * 16);
}

template<typename TType>
Mat44<TType>& Mat44<TType>::operator=(const TType m[]) {
  Memory::copy(mM, m, sizeof(TType) * 16);
}

template<typename TType>
bool Mat44<TType>::operator==(const Mat44<TType>& other) const {
  for (sizeT i = 0; i < 16; i++) {
    if (!Math::near(mM[i], other.mM[i])) {
      return false;
    }
  }
  return true;
}

template<typename TType>
bool Mat44<TType>::operator!=(const Mat44<TType>& other) const {
  return !(*this == other);
}

template<typename TType>
Mat44<TType> Mat44<TType>::operator+(const Mat44<TType>& other) const {
  return Mat44<TType>(
    xx + other.xx, xy + other.xy, xz + other.xz, xw + other.xw,
    yx + other.yx, yy + other.yy, yz + other.yz, yw + other.yw,
    zx + other.zx, zy + other.zy, zz + other.zz, zw + other.zw,
    wx + other.wx, wy + other.wy, wz + other.wz, ww + other.ww
  );
}

template<typename TType>
void Mat44<TType>::operator+=(const Mat44<TType>& other) {
  for (sizeT i = 0; i < 16; i++) {
    mM[i] += other.mM[i];
  }
}

template<typename TType>
Mat44<TType> Mat44<TType>::operator-() const {
  return get_inverted();
}

template<typename TType>
Mat44<TType> Mat44<TType>::operator-(const Mat44<TType>& other) const {
  return Mat44<TType>(
    xx - other.xx, xy - other.xy, xz - other.xz, xw - other.xw,
    yx - other.yx, yy - other.yy, yz - other.yz, yw - other.yw,
    zx - other.zx, zy - other.zy, zz - other.zz, zw - other.zw,
    wx - other.wx, wy - other.wy, wz - other.wz, ww - other.ww
  );
}

template<typename TType>
void Mat44<TType>::operator-=(const Mat44<TType>& other) {
  for (sizeT i = 0; i < 16; i++) {
    mM[i] -= other.mM[i];
  }
}

template<typename TType>
Mat44<TType> Mat44<TType>::operator*(const Mat44<TType>& other) const {
  return Mat44<TType>(xx*mM.xx + xy*mM.yx + xz*mM.zx + xw*mM.wx, xx*mM.xy + xy*mM.yy + xz*mM.zy + xw*mM.wy, xx*mM.xz + xy*mM.yz + xz*mM.zz + xw*mM.wz, xx*mM.xw + xy*mM.yw + xz*mM.zw + xw*mM.ww,
                      yx*mM.xx + yy*mM.yx + yz*mM.zx + yw*mM.wx, yx*mM.xy + yy*mM.yy + yz*mM.zy + yw*mM.wy, yx*mM.xz + yy*mM.yz + yz*mM.zz + yw*mM.wz, yx*mM.xw + yy*mM.yw + yz*mM.zw + yw*mM.ww,
                      zx*mM.xx + zy*mM.yx + zz*mM.zx + zw*mM.wx, zx*mM.xy + zy*mM.yy + zz*mM.zy + zw*mM.wy, zx*mM.xz + zy*mM.yz + zz*mM.zz + zw*mM.wz, zx*mM.xw + zy*mM.yw + zz*mM.zw + zw*mM.ww,
                      wx*mM.xx + wy*mM.yx + wz*mM.zx + ww*mM.wx, wx*mM.xy + wy*mM.yy + wz*mM.zy + ww*mM.wy, wx*mM.xz + wy*mM.yz + wz*mM.zz + ww*mM.wz, wx*mM.xw + wy*mM.yw + wz*mM.zw + ww*mM.ww);
}

template<typename TType>
void Mat44<TType>::operator*=(const Mat44<TType>& other) {
}

template<typename TType>
Mat44<TType> Mat44<TType>::operator*(TType m) const {
  return Mat44<TType>(
    xx * m, xy * m, xz * m, xw * m,
    yx * m, yy * m, yz * m, yw * m,
    zx * m, zy * m, zz * m, zw * m,
    wx * m, wy * m, wz * m, ww * m
  );
}

template<typename TType>
void Mat44<TType>::operator*=(TType m) {
  for (sizeT i = 0; i < 16; i++) {
    mM[i] *= m;
  }
}

template<typename TType>
Mat44<TType> Mat44<TType>::operator/(TType s) const {
  // Sanity check
  BE_ASSERT(s != TType(0), "Divide by zero")

  return Mat44<TType>(
    xx / s, xy / s, xz / s, xw / s,
    yx / s, yy / s, yz / s, yw / s,
    zx / s, zy / s, zz / s, zw / s,
    wx / s, wy / s, wz / s, ww / s
  );
}

template<typename TType>
void Mat44<TType>::operator/=(TType s) {
  // Sanity check
  BE_ASSERT(s != TType(0), "Divide by zero")

  for (sizeT i = 0; i < 16; i++) {
    mM[i] /= s;
  }
}

template<typename TType>
TType Mat44<TType>::operator[](uint8 index) const {
  // Sanity check
  BE_ASSERT(index < 16, "Index out of range")

  return mM[index];
}

template<typename TType>
TType& Mat44<TType>::operator[](uint8 index) {
  // Sanity check
  BE_ASSERT(index < 16, "Index out of range")

  return mM[index];
}

template<typename TType>
TType Mat44<TType>::operator()(uint8 row, uint8 column) const {
  // Sanity check
  BE_ASSERT(row < 4, "Row index out of range")
  BE_ASSERT(column < 4, "Column index out of range")

  return mM44[row][column];
}

template<typename TType>
TType& Mat44<TType>::operator()(uint8 row, uint8 column) {
  // Sanity check
  BE_ASSERT(row < 4, "Row index out of range")
  BE_ASSERT(column < 4, "Column index out of range")

  return mM44[row][column];
}

template<typename TType>
Mat44<TType>::operator TType*() {
  return mM;
}

template<typename TType>
Mat44<TType>::operator const TType*() const {
  return mM;
}

template<typename TType>
bool Mat44<TType>::is_zero() const {
  return (Math::near(xx, TType(0)) && Math::near(xy, TType(0)) && Math::near(xz, TType(0)) && Math::near(xw, TType(0)) &&
          Math::near(yx, TType(0)) && Math::near(yy, TType(0)) && Math::near(yz, TType(0)) && Math::near(yw, TType(0)) &&
          Math::near(zx, TType(0)) && Math::near(zy, TType(0)) && Math::near(zz, TType(0)) && Math::near(zw, TType(0)) &&
          Math::near(wx, TType(0)) && Math::near(wy, TType(0)) && Math::near(wz, TType(0)) && Math::near(ww, TType(0)));
}

template<typename TType>
bool Mat44<TType>::is_true_zero() const {
  return Memory::compare(mM, Zero.mM, sizeof(TType) * 16) == 0;
}

template<typename TType>
void Mat44<TType>::set_zero() {
  Memory::set(mM, TType(0), sizeof(TType) * 16);
}

template<typename TType>
bool Mat44<TType>::is_identity() const {
  return (Math::near(xx, TType(1)) && Math::near(xy, TType(0)) && Math::near(xz, TType(0)) && Math::near(xw, TType(0)) &&
          Math::near(yx, TType(0)) && Math::near(yy, TType(1)) && Math::near(yz, TType(0)) && Math::near(yw, TType(0)) &&
          Math::near(zx, TType(0)) && Math::near(zy, TType(0)) && Math::near(zz, TType(1)) && Math::near(zw, TType(0)) &&
          Math::near(wx, TType(0)) && Math::near(wy, TType(0)) && Math::near(wz, TType(0)) && Math::near(ww, TType(1)));
}

template<typename TType>
bool Mat44<TType>::is_true_identity() const {
  return Memory::compare(mM, Identity.mM, sizeof(TType) * 16) == 0;
}

template<typename TType>
void Mat44<TType>::set_identity() {
  Memory::set(mM, Identity.mM, sizeof(TType) * 16);
}

template<typename TType>
void Mat44<TType>::set(TType xx, TType xy, TType xz, TType xw, TType yx, TType yy, TType yz, TType yw, TType zx,
TType zy, TType zz, TType zw, TType wx, TType wy, TType wz, TType ww) {
  this->xx = xx; this->xy = xy; this->xz = xz; this->xw = xw;
  this->yx = yx; this->yy = yy; this->yz = yx; this->yw = yw;
  this->zx = zx; this->zy = zy; this->zz = zz; this->zw = zw;
  this->wx = wx; this->wy = wy; this->wz = wz; this->ww = ww;
}

template<typename TType>
Vec4<TType> Mat44<TType>::get_row(core::uint8 row) const {
  // Sanity check
  BE_ASSERT(row < 4, "Invalid row index");

  return Vec4<TType>(mM[row + 4*0], mM[row + 4*1], mM[row + 4*2], mM[row + 4*3]);
}

template<typename TType>
void Mat44<TType>::set_row(core::uint8 rowIndex, const Vec4<TType>& row) {
  // Sanity check
  BE_ASSERT(rowIndex < 4, "Invalid row index");

  mM[rowIndex + 4*0] = row.x;
  mM[rowIndex + 4*1] = row.y;
  mM[rowIndex + 4*2] = row.z;
  mM[rowIndex + 4*3] = row.w;
}

template<typename TType>
Vec4<TType> Mat44<TType>::get_column(core::uint8 column) const {
  // Sanity check
  BE_ASSERT(column < 4, "Invalid column index");

  return Vec4<TType>(mM[column*4 + 0], mM[column*4 + 1], mM[column*4 + 2], mM[column*4 + 3]);
}

template<typename TType>
void Mat44<TType>::set_column(core::uint8 columnIndex, const Vec4<TType>& column) {
  // Sanity check
  BE_ASSERT(column < 4, "Invalid column index")

  mM[columnIndex*4 + 0] = column.x;
  mM[columnIndex*4 + 1] = column.y;
  mM[columnIndex*4 + 2] = column.z;
  mM[columnIndex*4 + 3] = column.w;
}

template<typename TType>
bool Mat44<TType>::is_symmetric() const {
  return (Math::near(xy, yx) && Math::near(xz, zx) && Math::near(xw, wx) &&
          Math::near(yz, zy) && Math::near(yw, wy) && Math::near(zw, wz));
}

template<typename TType>
bool Mat44<TType>::is_orthogonal() const {
  return get_transposed() == get_inverted();
}

template<typename TType>
bool Mat44<TType>::is_rotation_matrix() const {
  return is_orthogonal() && Math::near(get_determinant(), TType(1));
}

template<typename TType>
bool Mat44<TType>::is_rotation_translation_matrix() const {
	return (is_rotation_matrix() && !Math::near(xw, 0.0f) && !Math::near(yw, 0.0f) && !Math::near(zw, 0.0f));
}

template<typename TType>
TType Mat44<TType>::get_trace() const {
  return xx+yy+zz+ww;
}

template<typename TType>
TType Mat44<TType>::get_determinant() const {
  return (xx*yy - yx*xy)*(zz*ww - wz*zw) - (xx*zy - zx*xy)*(yz*ww - wz*yw) +
         (xx*wy - wx*xy)*(yz*zw - zz*yw) + (yx*zy - zx*yy)*(xz*ww - wz*xw) -
         (yx*wy - wx*yy)*(xz*zw - zz*xw) + (zx*wy - wx*zy)*(xz*yw - yz*xw);
}

template<typename TType>
void Mat44<TType>::transpose() {
  // xy <-> yx
  TType fT = xy;
  xy = yx;
  yx = fT;

  // xz <-> zx
  fT = xz;
  xz = zx;
  zx = fT;

  // xw <-> wx
  fT = xw;
  xw = wx;
  wx = fT;

  // yz <-> zy
  fT = yz;
  yz = zy;
  zy = fT;

  // yw <-> wy
  fT = yw;
  yw = wy;
  wy = fT;

  // zw <-> wz
  fT = zw;
  zw = wz;
  wz = fT;
}

template<typename TType>
Mat44<TType> Mat44<TType>::get_transposed() const {
  return Mat44<TType>(xx, yx, zx, wx,
                      xy, yy, zy, wy,
                      xz, yz, zz, wz,
                      xw, yw, zw, ww);
}

template<typename TType>
bool Mat44<TType>::invert() {
  // First, calculate the determinant of the matrix
  TType fDet = get_determinant();

  if (fDet) {
    // Calculate the inverse of the matrix using Cramers rule
    fDet = TType(1) / fDet;
    set(fDet*(yy*(zz*ww - wz*zw) + zy*(wz*yw - yz*ww) + wy*(yz*zw - zz*yw)), fDet*(zy*(xz*ww - wz*xw) + wy*(zz*xw - xz*zw) + xy*(wz*zw - zz*ww)), fDet*(wy*(xz*yw - yz*xw) + xy*(yz*ww - wz*yw) + yy*(wz*xw - xz*ww)), fDet*(xy*(zz*yw - yz*zw) + yy*(xz*zw - zz*xw) + zy*(yz*xw - xz*yw)),
        fDet*(yz*(zx*ww - wx*zw) + zz*(wx*yw - yx*ww) + wz*(yx*zw - zx*yw)), fDet*(zz*(xx*ww - wx*xw) + wz*(zx*xw - xx*zw) + xz*(wx*zw - zx*ww)), fDet*(wz*(xx*yw - yx*xw) + xz*(yx*ww - wx*yw) + yz*(wx*xw - xx*ww)), fDet*(xz*(zx*yw - yx*zw) + yz*(xx*zw - zx*xw) + zz*(yx*xw - xx*yw)),
        fDet*(yw*(zx*wy - wx*zy) + zw*(wx*yy - yx*wy) + ww*(yx*zy - zx*yy)), fDet*(zw*(xx*wy - wx*xy) + ww*(zx*xy - xx*zy) + xw*(wx*zy - zx*wy)), fDet*(ww*(xx*yy - yx*xy) + xw*(yx*wy - wx*yy) + yw*(wx*xy - xx*wy)), fDet*(xw*(zx*yy - yx*zy) + yw*(xx*zy - zx*xy) + zw*(yx*xy - xx*yy)),
        fDet*(yx*(wy*zz - zy*wz) + zx*(yy*wz - wy*yz) + wx*(zy*yz - yy*zz)), fDet*(zx*(wy*xz - xy*wz) + wx*(xy*zz - zy*xz) + xx*(zy*wz - wy*zz)), fDet*(wx*(yy*xz - xy*yz) + xx*(wy*yz - yy*wz) + yx*(xy*wz - wy*xz)), fDet*(xx*(yy*zz - zy*yz) + yx*(zy*xz - xy*zz) + zx*(xy*yz - yy*xz)));
  } else {
    // For sure, set identity matrix
    set_identity();

    // Error!
    return false;
  }

  // Done
  return true;
}

template<typename TType>
Mat44<TType> Mat44<TType>::get_inverted() const {
  // First, calculate the determinant of the matrix
  TType fDet = get_determinant();

  if (fDet) {
    // Calculate the inverse of the matrix using Cramers rule
    fDet = TType(1) / fDet;
    return Mat44<TType>(fDet*(yy*(zz*ww - wz*zw) + zy*(wz*yw - yz*ww) + wy*(yz*zw - zz*yw)), fDet*(zy*(xz*ww - wz*xw) + wy*(zz*xw - xz*zw) + xy*(wz*zw - zz*ww)), fDet*(wy*(xz*yw - yz*xw) + xy*(yz*ww - wz*yw) + yy*(wz*xw - xz*ww)), fDet*(xy*(zz*yw - yz*zw) + yy*(xz*zw - zz*xw) + zy*(yz*xw - xz*yw)),
                        fDet*(yz*(zx*ww - wx*zw) + zz*(wx*yw - yx*ww) + wz*(yx*zw - zx*yw)), fDet*(zz*(xx*ww - wx*xw) + wz*(zx*xw - xx*zw) + xz*(wx*zw - zx*ww)), fDet*(wz*(xx*yw - yx*xw) + xz*(yx*ww - wx*yw) + yz*(wx*xw - xx*ww)), fDet*(xz*(zx*yw - yx*zw) + yz*(xx*zw - zx*xw) + zz*(yx*xw - xx*yw)),
                        fDet*(yw*(zx*wy - wx*zy) + zw*(wx*yy - yx*wy) + ww*(yx*zy - zx*yy)), fDet*(zw*(xx*wy - wx*xy) + ww*(zx*xy - xx*zy) + xw*(wx*zy - zx*wy)), fDet*(ww*(xx*yy - yx*xy) + xw*(yx*wy - wx*yy) + yw*(wx*xy - xx*wy)), fDet*(xw*(zx*yy - yx*zy) + yw*(xx*zy - zx*xy) + zw*(yx*xy - xx*yy)),
                        fDet*(yx*(wy*zz - zy*wz) + zx*(yy*wz - wy*yz) + wx*(zy*yz - yy*zz)), fDet*(zx*(wy*xz - xy*wz) + wx*(xy*zz - zy*xz) + xx*(zy*wz - wy*zz)), fDet*(wx*(yy*xz - xy*yz) + xx*(wy*yz - yy*wz) + yx*(xy*wz - wy*xz)), fDet*(xx*(yy*zz - zy*yz) + yx*(zy*xz - xy*zz) + zx*(xy*yz - yy*xz)));
  } else return Mat44<TType>::Identity;
}

template<typename TType>
void Mat44<TType>::set_scale_matrix(TType x, TType y, TType z) {
  xx =    x; xy = 0.0f; xz = 0.0f; xw = 0.0f;
  yx = 0.0f; yy =    y; yz = 0.0f; yw = 0.0f;
  zx = 0.0f; zy = 0.0f; zz =    z; zw = 0.0f;
  wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

template<typename TType>
void Mat44<TType>::set_scale_matrix(const Vec3<TType>& scale) {
  xx = scale.x; xy = 0.0f; xz = 0.0f; xw = 0.0f;
  yx = 0.0f; yy = scale.y; yz = 0.0f; yw = 0.0f;
  zx = 0.0f; zy = 0.0f; zz = scale.z; zw = 0.0f;
  wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

template<typename TType>
void Mat44<TType>::get_scale(TType& x, TType& y, TType& z) const {
  // Optimized version
  x = Math::sqrt(xx*xx + yx*yx + zx*zx);
  y = Math::sqrt(xy*xy + yy*yy + zy*zy);
  z = Math::sqrt(xz*xz + yz*yz + zz*zz);
  if (get_determinant() < 0.0f) {
    x = -x;
    y = -y;
    z = -z;
  }
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_scale() const {
  Vec3<TType> scale;
  get_scale(scale.x, scale.y, scale.z);
  return scale;
}

template<typename TType>
void Mat44<TType>::get_scale(TType v[]) const {
  get_scale(v[0], v[1], v[2], v[3]);
}

template<typename TType>
void Mat44<TType>::set_translation_matrix(TType x, TType y, TType z) {
  xx = 1.0f; xy = 0.0f; xz = 0.0f; xw =    x;
  yx = 0.0f; yy = 1.0f; yz = 0.0f; yw =    y;
  zx = 0.0f; zy = 0.0f; zz = 1.0f; zw =    z;
  wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

template<typename TType>
void Mat44<TType>::set_translation_matrix(const Vec3<TType>& translation) {
  xx = 1.0f; xy = 0.0f; xz = 0.0f; xw = translation.x;
  yx = 0.0f; yy = 1.0f; yz = 0.0f; yw = translation.y;
  zx = 0.0f; zy = 0.0f; zz = 1.0f; zw = translation.z;
  wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

template<typename TType>
void Mat44<TType>::get_translation(TType& x, TType& y, TType& z, TType& w) const {
  x = xw;
  y = yw;
  z = zw;
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_translation() const {
  return Vec3<TType>(xw, yw, zw);
}

template<typename TType>
void Mat44<TType>::get_translation(TType v[]) const {
  v[0] = xw;
  v[1] = yw;
  v[2] = zw;
}

template<typename TType>
void Mat44<TType>::set_translation(TType x, TType y, TType z) {
  xw = x;
  yw = y;
  zw = z;
}

template<typename TType>
void Mat44<TType>::set_translation(const Vec3<TType>& translation) {
  set_translation(translation.x, translation.y, translation.z);
}

template<typename TType>
void Mat44<TType>::set_translation(const TType v[]) {
  set_translation(v[0], v[1], v[2]);
}

template<typename TType>
void Mat44<TType>::from_euler_angle_x(TType x) {
}

template<typename TType>
void Mat44<TType>::from_euler_angle_y(TType y) {
}

template<typename TType>
void Mat44<TType>::from_euler_angle_z(TType z) {
}

template<typename TType>
void Mat44<TType>::to_axis_angle(TType& x, TType& y, TType& z, TType& angle) const {
}

template<typename TType>
void Mat44<TType>::from_axis_angle(TType x, TType y, TType z, TType angle) const {
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_x_axis() const {
  return Vec3<TType>(xx, yx, zx);
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_y_axis() const {
  return Vec3<TType>(xy, yy, zy);
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_z_axis() const {
  return Vec3<TType>(xz, yz, zz);
}

template<typename TType>
void Mat44<TType>::to_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const {
}

template<typename TType>
void Mat44<TType>::from_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::look_at(const Vec3<TType>& eye, Vec3<TType>& at, Vec3<TType>& up) {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::view(const Quaternion<TType>& rotation, const Vec3<TType>& translation) {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective(TType w, TType h, TType zNear, TType zFar) {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective_fov(TType fov, TType aspect, TType zNear, TType zFar) {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective_infinite(TType w, TType h, TType zNear) {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective_fov_infinite(TType fov, TType aspect, TType zNear) {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective_off_center(TType l, TType r, TType b, TType t, TType zNear, TType zFar) {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::ortho(TType w, TType h, TType zNear, TType zFar) {
}

template<typename TType>
Mat44<TType>& Mat44<TType>::ortho_off_center(TType l, TType r, TType t, TType b, TType zNear, TType zFar) {
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}