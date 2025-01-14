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
}

template<typename TType>
Mat44<TType>& Mat44<TType>::operator=(const TType m[]) {
}

template<typename TType>
bool Mat44<TType>::operator==(const Mat44<TType>& other) const {
}

template<typename TType>
bool Mat44<TType>::operator!=(const Mat44<TType>& other) const {
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
  return Mat44<TType>(
    xx / s, xy / s, xz / s, xw / s,
    yx / s, yy / s, yz / s, yw / s,
    zx / s, zy / s, zz / s, zw / s,
    wx / s, wy / s, wz / s, ww / s
  );
}

template<typename TType>
void Mat44<TType>::operator/=(TType s) {
  for (sizeT i = 0; i < 16; i++) {
    mM[i] /= s;
  }
}

template<typename TType>
TType Mat44<TType>::operator[](uint8 index) const {
  return mM[index];
}

template<typename TType>
TType& Mat44<TType>::operator[](uint8 index) {
  return mM[index];
}

template<typename TType>
TType Mat44<TType>::operator()(uint8 row, uint8 column) const {
  return mM44[row][column];
}

template<typename TType>
TType& Mat44<TType>::operator()(uint8 row, uint8 column) {
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
}

template<typename TType>
bool Mat44<TType>::is_true_zero() const {
}

template<typename TType>
void Mat44<TType>::set_zero() {
}

template<typename TType>
bool Mat44<TType>::is_identity() const {
}

template<typename TType>
bool Mat44<TType>::is_true_identity() const {
}

template<typename TType>
void Mat44<TType>::set_identity() {
}

template<typename TType>
void Mat44<TType>::set(TType xx, TType xy, TType xz, TType xw, TType yx, TType yy, TType yz, TType yw, TType zx,
  TType zy, TType zz, TType zw, TType wx, TType wy, TType wz, TType ww) {
}

template<typename TType>
Vec4<TType> Mat44<TType>::get_row(core::uint8 row) const {
}

template<typename TType>
void Mat44<TType>::set_row(core::uint8 rowIndex, const Vec4<TType>& row) {
}

template<typename TType>
Vec4<TType> Mat44<TType>::get_column(core::uint8 column) const {
}

template<typename TType>
void Mat44<TType>::set_column(core::uint8 columnIndex, const Vec4<TType>& column) {
}

template<typename TType>
bool Mat44<TType>::is_symmetric() const {
}

template<typename TType>
bool Mat44<TType>::is_orthogonal() const {
}

template<typename TType>
bool Mat44<TType>::is_rotation_matrix() const {
}

template<typename TType>
bool Mat44<TType>::is_rotation_translation_matrix() const {
}

template<typename TType>
TType Mat44<TType>::get_trace() const {
}

template<typename TType>
TType Mat44<TType>::get_determinant() const {
}

template<typename TType>
void Mat44<TType>::transpose() {
}

template<typename TType>
Mat44<TType> Mat44<TType>::get_transposed() const {
}

template<typename TType>
bool Mat44<TType>::invert() {
}

template<typename TType>
Mat44<TType> Mat44<TType>::get_inverted() const {
}

template<typename TType>
void Mat44<TType>::set_scale_matrix(TType x, TType y, TType z) {
}

template<typename TType>
void Mat44<TType>::set_scale_matrix(const Vec3<TType>& scale) {
}

template<typename TType>
void Mat44<TType>::get_scale(TType& x, TType& y, TType& z, TType& w) const {
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_scale() const {
}

template<typename TType>
void Mat44<TType>::get_scale(TType v[]) const {
}

template<typename TType>
void Mat44<TType>::set_translation_matrix(TType x, TType y, TType z) {
}

template<typename TType>
void Mat44<TType>::set_translation_matrix(const Vec3<TType>& translation) {
}

template<typename TType>
void Mat44<TType>::get_translation(TType& x, TType& y, TType& z, TType& w) const {
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_translation() const {
}

template<typename TType>
void Mat44<TType>::get_translation(TType v[]) const {
}

template<typename TType>
void Mat44<TType>::set_translation(TType x, TType y, TType z) {
}

template<typename TType>
void Mat44<TType>::set_translation(const Vec3<TType>& translation) {
}

template<typename TType>
void Mat44<TType>::set_translation(const TType v[]) {
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
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_y_axis() const {
}

template<typename TType>
Vec3<TType> Mat44<TType>::get_z_axis() const {
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