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
#include "core/math/mat33.h"
#include "core/math/vec2.h"
#include "core/math/vec3.h"
#include "core/math/vec4.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {

template<typename TType>
const Mat44<TType> Mat44<TType>::Zero    (
  TType(0), TType(0), TType(0), TType(0),
  TType(0), TType(0), TType(0), TType(0),
  TType(0), TType(0), TType(0), TType(0),
  TType(0), TType(0), TType(0), TType(0)
);
template<typename TType>
const Mat44<TType> Mat44<TType>::Identity(
  TType(1), TType(0), TType(0), TType(0),
  TType(0), TType(1), TType(0), TType(0),
  TType(0), TType(0), TType(1), TType(0),
  TType(0), TType(0), TType(0), TType(1)
);


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
Mat44<TType> Mat44<TType>::create_perspective_fov(TType fov, TType aspect, TType zNear, TType zFar) {
  Mat44<TType> mat;
  mat.perspective_fov(fov, aspect, zNear, zFar);
  return mat;
}

template<typename TType>
Mat44<TType> Mat44<TType>::create_translation(TType x, TType y, TType z) {
  Mat44<TType> mat;
  mat.set_translation(x, y, z);
  return mat;
}

template<typename TType>
Mat44<TType> Mat44<TType>::create_translation(const Vec3<TType>& translation) {
  Mat44<TType> mat;
  mat.set_translation(translation.x, translation.y, translation.z);
  return mat;
}

template<typename TType>
Mat44<TType> Mat44<TType>::create_scale(TType x, TType y, TType z) {
  Mat44<TType> mat;
  mat.set_scale_matrix(x, y, z);
  return mat;
}

template<typename TType>
Mat44<TType> Mat44<TType>::create_scale(const Vec3<TType>& scale) {
  Mat44<TType> mat;
  mat.set_scale_matrix(scale);
  return mat;
}

template<typename TType>
Mat44<TType> Mat44<TType>::create_rotation(TType x, TType y, TType z, TType angle) {
  Mat44<TType> mat;
  mat.from_axis_angle(x, y, z, angle);
  return mat;
}

template<typename TType>
Mat44<TType> Mat44<TType>::create_rotation(const Vec3<TType>& axis, TType angle) {
  Mat44<TType> mat;
  mat.from_axis_angle(axis, angle);
  return mat;
}



template<typename TType>
Mat44<TType>::Mat44()
: xx(TType(1)), yx(TType(0)), zx(TType(0)), wx(TType(0))
, xy(TType(0)), yy(TType(1)), zy(TType(0)), wy(TType(0))
, xz(TType(0)), yz(TType(0)), zz(TType(1)), wz(TType(0))
, xw(TType(0)), yw(TType(0)), zw(TType(0)), ww(TType(1)) {
}

template<typename TType>
Mat44<TType>::Mat44(const TType m[]) {
  Memory::copy(mM, m, sizeof(TType) * 16);
}

template<typename TType>
Mat44<TType>::Mat44(const Mat44<TType>& other) {
  Memory::copy(mM, other.mM, sizeof(TType) * 16);
}

template<typename TType>
Mat44<TType>::Mat44(TType xx, TType xy, TType xz, TType xw, TType yx, TType yy, TType yz, TType yw, TType zx, TType zy,
  TType zz, TType zw, TType wx, TType wy, TType wz, TType ww)
: xx(xx), yx(yx), zx(zx), wx(wx)
, xy(xy), yy(yy), zy(zy), wy(wy)
, xz(xz), yz(yz), zz(zz), wz(wz)
, xw(xw), yw(yw), zw(zw), ww(ww) {
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
  return Mat44<TType>(xx*other.xx + xy*other.yx + xz*other.zx + xw*other.wx, xx*other.xy + xy*other.yy + xz*other.zy + xw*other.wy, xx*other.xz + xy*other.yz + xz*other.zz + xw*other.wz, xx*other.xw + xy*other.yw + xz*other.zw + xw*other.ww,
                      yx*other.xx + yy*other.yx + yz*other.zx + yw*other.wx, yx*other.xy + yy*other.yy + yz*other.zy + yw*other.wy, yx*other.xz + yy*other.yz + yz*other.zz + yw*other.wz, yx*other.xw + yy*other.yw + yz*other.zw + yw*other.ww,
                      zx*other.xx + zy*other.yx + zz*other.zx + zw*other.wx, zx*other.xy + zy*other.yy + zz*other.zy + zw*other.wy, zx*other.xz + zy*other.yz + zz*other.zz + zw*other.wz, zx*other.xw + zy*other.yw + zz*other.zw + zw*other.ww,
                      wx*other.xx + wy*other.yx + wz*other.zx + ww*other.wx, wx*other.xy + wy*other.yy + wz*other.zy + ww*other.wy, wx*other.xz + wy*other.yz + wz*other.zz + ww*other.wz, wx*other.xw + wy*other.yw + wz*other.zw + ww*other.ww);
}

template<typename TType>
void Mat44<TType>::operator*=(const Mat44<TType>& other) {
	*this = *this * other;
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
Vec2<TType> Mat44<TType>::operator*(const Vec2<TType>& v) const {
  const TType x = v.x, y = v.y;
  const TType fInvW = TType(1)/(wx*x + wy*y + ww);
  return Vec2<TType>(
    (xx*x + xy*y + xw)*fInvW,
    (yx*x + yy*y + yw)*fInvW);
}

template<typename TType>
Vec3<TType> Mat44<TType>::operator*(const Vec3<TType>& v) const {
  const TType x = v.x, y = v.y, z = v.z;
  const TType fInvW = TType(1)/(wx*x + wy*y + wz*z + ww);
  return Vec3<TType>(
    (xx*x + xy*y + xz*z + xw)*fInvW,
    (yx*x + yy*y + yz*z + yw)*fInvW,
    (zx*x + zy*y + zz*z + zw)*fInvW);
}

template<typename TType>
Vec4<TType> Mat44<TType>::operator*(const Vec4<TType>& v) const {
  const TType x = v.x, y = v.y, z = v.z, w = v.w;
  return Vec4<TType>(
    xx*x + xy*y + xz*z + xw*w,
    yx*x + yy*y + yz*z + yw*w,
    zx*x + zy*y + zz*z + zw*w,
    wx*x + wy*y + wz*z + ww*w);
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
	return (is_rotation_matrix() && !Math::near(xw, TType(0)) && !Math::near(yw, TType(0)) && !Math::near(zw, TType(0)));
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
  TType t = xy;
  xy = yx;
  yx = t;

  // xz <-> zx
  t = xz;
  xz = zx;
  zx = t;

  // xw <-> wx
  t = xw;
  xw = wx;
  wx = t;

  // yz <-> zy
  t = yz;
  yz = zy;
  zy = t;

  // yw <-> wy
  t = yw;
  yw = wy;
  wy = t;

  // zw <-> wz
  t = zw;
  zw = wz;
  wz = t;
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
  xx =    x;     xy = TType(0); xz = TType(0); xw = TType(0);
  yx = TType(0); yy =    y;     yz = TType(0); yw = TType(0);
  zx = TType(0); zy = TType(0); zz =    z;     zw = TType(0);
  wx = TType(0); wy = TType(0); wz = TType(0); ww = TType(1);
}

template<typename TType>
void Mat44<TType>::set_scale_matrix(const Vec3<TType>& scale) {
  xx = scale.x;  xy = TType(0); xz = TType(0); xw = TType(0);
  yx = TType(0); yy = scale.y;  yz = TType(0); yw = TType(0);
  zx = TType(0); zy = TType(0); zz = scale.z;  zw = TType(0);
  wx = TType(0); wy = TType(0); wz = TType(0); ww = TType(1);
}

template<typename TType>
void Mat44<TType>::get_scale(TType& x, TType& y, TType& z) const {
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
  xx = TType(1); xy = TType(0); xz = TType(0); xw =    x;
  yx = TType(0); yy = TType(1); yz = TType(0); yw =    y;
  zx = TType(0); zy = TType(0); zz = TType(1); zw =    z;
  wx = TType(0); wy = TType(0); wz = TType(0); ww = TType(1);
}

template<typename TType>
void Mat44<TType>::set_translation_matrix(const Vec3<TType>& translation) {
  xx = TType(1); xy = TType(0); xz = TType(0); xw = translation.x;
  yx = TType(0); yy = TType(1); yz = TType(0); yw = translation.y;
  zx = TType(0); zy = TType(0); zz = TType(1); zw = translation.z;
  wx = TType(0); wy = TType(0); wz = TType(0); ww = TType(1);
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
  TType fSin = Math::sin(x);
  TType fCos = Math::cos(x);
  xx = TType(1); xy = TType(0); xz =  TType(0); xw = TType(0);
  yx = TType(0); yy = fCos;     yz = -fSin;     yw = TType(0);
  zx = TType(0); zy = fSin;     zz =  fCos;     zw = TType(0);
  wx = TType(0); wy = TType(0); wz =  TType(0); ww = TType(1);
}

template<typename TType>
void Mat44<TType>::from_euler_angle_y(TType y) {
  TType fSin = Math::sin(y);
  TType fCos = Math::cos(y);
  xx =  fCos;     xy = TType(0); xz = fSin;     xw = TType(0);
  yx =  TType(0); yy = TType(1); yz = TType(0); yw = TType(0);
  zx = -fSin;     zy = TType(0); zz = fCos;     zw = TType(0);
  wx =  TType(0); wy = TType(0); wz = TType(0); ww = TType(1);
}

template<typename TType>
void Mat44<TType>::from_euler_angle_z(TType z) {
  TType fSin = Math::sin(z);
  TType fCos = Math::cos(z);
  xx = fCos;     xy = -fSin;     xz = TType(0); xw = TType(0);
  yx = fSin;     yy =  fCos;     yz = TType(0); yw = TType(0);
  zx = TType(0); zy =  TType(0); zz = TType(1); zw = TType(0);
  wx = TType(0); wy =  TType(0); wz = TType(0); ww = TType(1);
}

template<typename TType>
void Mat44<TType>::to_axis_angle(TType& x, TType& y, TType& z, TType& angle) const {
  TType trace = xx + yy + zz;
  TType fCos   = TType(0.5)*(trace-TType(1));
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
          TType fScale = TType(1)/fU;
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
          TType fHalfInverse = TType(0.5)/x;
          y = fHalfInverse*xy;
          z = fHalfInverse*xz;
        } else {
          // 22 is maximum diagonal term
          z = TType(0.5)*Math::sqrt(zz - xx - yy + TType(1));
          TType fHalfInverse = TType(0.5)/z;
          x = fHalfInverse*xz;
          y = fHalfInverse*yz;
        }
      } else {
        // 11 > 00
        if (yy >= zz) {
          // 11 is maximum diagonal term
          y = TType(0.5)*Math::sqrt(yy - xx - zz + TType(1));
          TType fHalfInverse  = TType(0.5)/y;
          x = fHalfInverse*xy;
          z = fHalfInverse*xz;
        } else {
          // 22 is maximum diagonal term
          z = TType(0.5)*Math::sqrt(zz - xx - yy + TType(1));
          TType fHalfInverse = TType(0.5)/z;
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
void Mat44<TType>::from_axis_angle(TType x, TType y, TType z, TType angle) const {
  TType fRSin = Math::sin(angle);
  TType fRCos = Math::cos(angle);
  xx =    fRCos + x*x*(1-fRCos);    xy = -z*fRSin + x*y*(1-fRCos);    xz =  y*fRSin + x*z*(1-fRCos);    xw = TType(0);
  yx =  z*fRSin + y*x*(1-fRCos);    yy =    fRCos + y*y*(1-fRCos);    yz = -x*fRSin + y*z*(1-fRCos);    yw = TType(0);
  zx = -y*fRSin + z*x*(1-fRCos);    zy =  x*fRSin + z*y*(1-fRCos);    zz =    fRCos + z*z*(1-fRCos);    zw = TType(0);
  wx = TType(0);                    wy = TType(0);                    wz = TType(0);                    ww = TType(1);
}

template<typename TType>
void Mat44<TType>::from_axis_angle(const Vec3<TType>& axis, TType angle) const {
  from_axis_angle(axis.x, axis.y, axis.z, angle);
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
  x.x = xx; y.x = xy; z.x = xz;
  x.y = yx; y.y = yy; z.y = yz;
  x.z = zx; y.z = zy; z.z = zz;
}

template<typename TType>
void Mat44<TType>::from_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const {
  xx = x.x; xy = y.x; xz = z.x; xw = TType(0);
  yx = x.y; yy = y.y; yz = z.y; yw = TType(0);
  zx = x.z; zy = y.z; zz = z.z; zw = TType(0);
  wx = TType(0); wy = TType(0); wz = TType(0); ww = TType(1);
}

template<typename TType>
Mat44<TType>& Mat44<TType>::look_at(const Vec3<TType>& eye, Vec3<TType>& at, Vec3<TType>& up) {
  Vec3<TType> vZAxis = (eye - at).get_normalized();
  Vec3<TType> vXAxis = up.CrossProduct(vZAxis).get_normalized();
  Vec3<TType> vYAxis = vZAxis.CrossProduct(vXAxis);

  // Setup matrix
  xx = vXAxis.x;     xy = vXAxis.y;     xz = vXAxis.z;     xw = -vXAxis.DotProduct(eye);
  yx = vYAxis.x;     yy = vYAxis.y;     yz = vYAxis.z;     yw = -vYAxis.DotProduct(eye);
  zx = vZAxis.x;     zy = vZAxis.y;     zz = vZAxis.z;     zw = -vZAxis.DotProduct(eye);
  wx = TType(0);     wy = TType(0);     wz = TType(0);     ww = TType(1);

  // Return this matrix
  return *this;
}

template<typename TType>
Mat44<TType>& Mat44<TType>::view(const Quaternion<TType>& rotation, const Vec3<TType>& translation) {
  // Calculate view matrix:
  Mat33<TType> mRot;
  rotation.ToRotationMatrix(mRot);
  mRot.transpose();
  Vec3<TType> vPos = -(mRot*translation);

  // Now create the view matrix
  xx = mRot.xx;     xy = mRot.xy;     xz = mRot.xz;     xw = vPos.x;
  yx = mRot.yx;     yy = mRot.yy;     yz = mRot.yz;     yw = vPos.y;
  zx = mRot.zx;     zy = mRot.zy;     zz = mRot.zz;     zw = vPos.z;
  wx = TType(0);    wy = TType(0);    wz = TType(0);    ww = TType(1);

  // Return this matrix
  return *this;
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective(TType w, TType h, TType zNear, TType zFar) {
  // Setup the matrix
  xx = 2*zNear/w;           xy = TType(0);             xz = TType(0);                 xw = TType(0);
  yx = TType(0);            yy = 2*zNear/h;            yz = TType(0);                 yw = TType(0);
  zx = TType(0);            zy = TType(0);             zz = zFar/(zNear-zFar);        zw = zNear*zFar/(zNear-zFar);
  wx = TType(0);            wy = TType(0);             wz = -TType(1);                ww = TType(0);

  // Return this matrix
  return *this;
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective_fov(TType fov, TType aspect, TType zNear, TType zFar) {
  float e = TType(1)/Math::tan(fov*0.5f); // Focal length

  // Setup the matrix
  xx = e/aspect;      xy = TType(0); xz = TType(0);                          xw = TType(0);
  yx = TType(0);      yy = e;        yz = TType(0);                          yw = TType(0);
  zx = TType(0);      zy = TType(0); zz = (zFar+zNear)/(zNear-zFar);         zw = (2*zNear*zFar)/(zNear-zFar);
  wx = TType(0);      wy = TType(0); wz = -TType(1);                         ww = TType(0);

  // Return this matrix
  return *this;
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective_infinite(TType w, TType h, TType zNear) {
  // Setup the matrix
  xx = 2*zNear/w;           xy = TType(0);             xz = TType(0);  xw = TType(0);
  yx = TType(0);            yy = 2*zNear/h;            yz = TType(0);  yw = TType(0);
  zx = TType(0);            zy = TType(0);             zz = -TType(1); zw = -2.0f*zNear;
  wx = TType(0);            wy = TType(0);             wz = -TType(1); ww = TType(0);

  // Return this matrix
  return *this;
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective_fov_infinite(TType fov, TType aspect, TType zNear) {
  float e = TType(1)/Math::tan(fov*0.5f); // Focal length

  // Setup the matrix
  xx = e/aspect;      xy = TType(0); xz = TType(0);  xw = TType(0);
  yx = TType(0);      yy = e;        yz = TType(0);  yw = TType(0);
  zx = TType(0);      zy = TType(0); zz = -TType(1); zw = -2.0f*zNear;
  wx = TType(0);      wy = TType(0); wz = -TType(1); ww = TType(0);

  // Return this matrix
  return *this;
}

template<typename TType>
Mat44<TType>& Mat44<TType>::perspective_off_center(TType l, TType r, TType b, TType t, TType zNear, TType zFar) {
  // Setup the matrix
  xx = 2*zNear/(r-l);        xy = TType(0);             xz = (l+r)/(r-l);                  xw = TType(0);
  yx = TType(0);             yy = 2*zNear/(t-b);        yz = (t+b)/(t-b);                  yw = TType(0);
  zx = TType(0);             zy = TType(0);             zz = -((zFar+zNear)/(zFar-zNear)); zw = -((2*zFar*zNear)/(zFar-zNear));
  wx = TType(0);             wy = TType(0);             wz = -TType(1);                    ww = TType(0);

  // Return this matrix
  return *this;
}

template<typename TType>
Mat44<TType>& Mat44<TType>::ortho(TType w, TType h, TType zNear, TType zFar) {
  // Setup the matrix
  xx = 2/w;          xy = TType(0);      xz = TType(0);                xw = TType(0);
  yx = TType(0);     yy = 2/h;           yz = TType(0);                yw = TType(0);
  zx = TType(0);     zy = TType(0);      zz = TType(1)/(zNear-zFar);   zw = zNear/(zNear-zFar);
  wx = TType(0);     wy = TType(0);      wz = TType(0);                ww = TType(1);

  // Return this matrix
  return *this;
}

template<typename TType>
Mat44<TType>& Mat44<TType>::ortho_off_center(TType l, TType r, TType t, TType b, TType zNear, TType zFar) {
  // Setup the matrix
  xx = 2/(r-l);       xy = TType(0);      xz = TType(0);                xw = (l+r)/(l-r);
  yx = TType(0);      yy = 2/(t-b);       yz = TType(0);                yw = (t+b)/(b-t);
  zx = TType(0);      zy = TType(0);      zz = TType(1)/(zNear-zFar);   zw = zNear/(zNear-zFar);
  wx = TType(0);      wy = TType(0);      wz = TType(0);                ww = TType(1);

  // Return this matrix
  return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}