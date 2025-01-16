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
#include "core/core.h"


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
template<typename TType> class Mat44;
template<typename TType> class Vec2;
template<typename TType> class Vec3;
template<typename TType> class Vec4;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Represents a column-major, left-handed 4x4 matrix implementation.
 *
 * This template class provides functionality for working with 3x3 transformation matrices.
 * It supports operations such as addition, multiplication, inversion, and specialized transformations
 * like scaling, translation, and projection. Designed for left-handed coordinate systems,
 * it is compatible with graphics programming conventions.
 *
 * @tparam TType
 * The underlying data type for the matrix elements, typically `float` or `double`.
 */
template<typename TType>
class Mat33 {
public:

  /** Zero matrix */
  static const Mat33 Zero;
  /** Identity matrix */
  static const Mat33 Identity;

public:
  
  Mat33();

  Mat33(const TType fS[]);
  Mat33(const Mat33& other);
  Mat33(const Mat44<TType>& other);
  Mat33(TType xx, TType xy, TType xz,
        TType yx, TType yy, TType yz,
        TType zx, TType zy, TType zz);

  ~Mat33();


  // Assignment Operators
  Mat33<TType>& operator=(const Mat33<TType>& other);

  Mat33<TType>& operator=(const TType m[]);

  // Comparison Operators
  bool operator==(const Mat33<TType>& other) const;

  bool operator!=(const Mat33<TType>& other) const;

  // Arithmetic Operators
  Mat33 operator+(const Mat33<TType>& other) const;

  void operator+=(const Mat33<TType>& other);

  Mat33 operator-() const;

  Mat33 operator-(const Mat33<TType>& other) const;

  void operator-=(const Mat33<TType>& other);

  Mat33 operator*(const Mat33<TType>& other) const;

  void operator*=(const Mat33<TType>& other);

  Mat33 operator*(TType m) const;

  void operator*=(TType m);

  Mat33 operator/(TType s) const;

  void operator/=(TType s);

  Vec2<TType> operator*(const Vec2<TType>& v) const;

  Vec3<TType> operator*(const Vec3<TType>& v) const;

  Vec4<TType> operator*(const Vec4<TType>& v) const;

  // Element Access Operators
  TType operator [](uint8 index) const;
  TType& operator [](uint8 index);
  TType operator ()(uint8 row = 0, uint8 column = 0) const;
  TType& operator ()(uint8 row = 0, uint8 column = 0);

  // Type Conversion Operators
  operator TType*();

  operator const TType*() const;

  /**
   * @brief
   * Returns whether this matrix is the zero matrix using an epsilon environment
   *
   * @return
   * 'true' if this matrix is the zero matrix, else 'false'
   */
  bool is_zero() const;

  /**
   * @brief
   * Returns whether this matrix is truly the zero matrix
   *
   * @remarks
   * All components MUST be exactly 0. Floating point inaccuracy
   * is not taken into account.
   *
   * @return
   * 'true' if this matrix is truly the zero matrix, else 'false'
   */
  bool is_true_zero() const;

  /**
   * @brief
   * Sets a zero matrix
   *
   * @remarks
   *   @code
   *    | 0 0 0 0 |
   *    | 0 0 0 0 |
   *    | 0 0 0 0 |
   *    | 0 0 0 0 |
   *   @endcode
   */
  void set_zero();

  /**
   * @brief
   * Returns whether this matrix is the identity matrix using an epsilon environment
   *
   * @return
   * 'true' if this matrix is the identity matrix, else 'false'
   */
  bool is_identity() const;

  /**
   * @brief
   * Returns whether this matrix is truly the identity matrix
   *
   * @remarks
   * All components MUST be exactly either 0 or 1. Floating point inaccuracy
   * is not taken into account.
   *
   * @return
   * 'true' if this matrix is truly the identity matrix, else 'false'
   */
  bool is_true_identity() const;

  /**
   * @brief
   * Sets an identity matrix
   *
   * @remarks
   *   @code
   *    | 1 0 0 0 |
   *    | 0 1 0 0 |
   *    | 0 0 1 0 |
   *    | 0 0 0 1 |
   *   @endcode
   */
  void set_identity();

  /**
   * @brief
   * Sets the elements of the matrix
   */
  void set(
      TType xx, TType xy, TType xz,
      TType yx, TType yy, TType yz,
      TType zx, TType zy, TType zz);

  /**
   * @brief
   * Returns a requested row
   *
   * @param[in] row
   * Index of the row to return (0-3)
   *
   * @return
   * The requested row (null vector on error)
   *
   * @remarks
   *   @code
   *    | x y z w | <- Row 0
   *    | 0 0 0 0 |
   *    | 0 0 0 0 |
   *    | 0 0 0 0 |
   *   @endcode
   */
  Vec3<TType> get_row(core::uint8 row) const;

  /**
   * @brief
   * Sets a row
   *
   * @param[in] rowIndex
   * Index of the row to set (0-3)
   * @param[in] row
   * Row vector
   *
   * @see
   * - get_row()
   */
  void set_row(core::uint8 rowIndex, const Vec3<TType>& row);

  /**
   * @brief
   * Returns a requested column
   *
   * @param[in] column
   * Index of the column to return (0-3)
   *
   * @return
   * The requested column (null vector on error)
   *
   * @remarks
   *   @code
   *   | x 0 0 0 |
   *   | y 0 0 0 |
   *   | z 0 0 0 |
   *   | w 0 0 0 |
   *     ^
   *     |
   *     Column 0
   *   @endcode
   */
  Vec3<TType> get_column(core::uint8 column) const;

  /**
   * @brief
   * Sets a column
   *
   * @param[in] columnIndex
   * Index of the column to set (0-3)
   * @param[in] column
   * Column vector
   *
   * @see
   * - get_column()
   */
  void set_column(core::uint8 columnIndex, const Vec3<TType>& column);

  /**
   * @brief
   * Returns true if the matrix is symmetric
   *
   * @return
   * 'true' if the matrix is symmetric, else 'false'
   *
   * @remarks
   * A matrix is symmetric if it is equal to it's transposed matrix.\n
   * A = A^T  ->  a(i, j) = a(j, i)
   */
  bool is_symmetric() const;

  /**
   * @brief
   * Returns true if this matrix is orthogonal
   *
   * @return
   * 'true' if the matrix is orthogonal, else 'false'
   *
   * @remarks
   * A matrix is orthogonal if it's transposed matrix is equal to its unversed matrix.\n
   * A^T = A^-1  or  A*A^T = A^T*A = I
   *
   * @note
   * - An orthogonal matrix is always non-singular (invertible) and it's inverse is equal to it's transposed
   * - The transpose and inverse of the matrix is orthogonal, too
   * - Products of orthogonal matrices are orthogonal, too
   * - The determinant of an orthogonal matrix is +/- 1
   * - The row and column vectors of an orthogonal matrix form an orthonormal basis,
   *   that is, these vectors are unit-length and they are mutually perpendicular
   */
  bool is_orthogonal() const;

  /**
   * @brief
   * Returns true if this matrix is a rotation matrix
   *
   * @return
   * 'true' if this matrix is a rotation matrix, else 'false'
   *
   * @remarks
   * A rotation matrix is orthogonal, and it's determinant is 1 to rule out reflections.
   *
   * @see
   * - is_orthogonal()
   */
  bool is_rotation_matrix() const;

  /**
  *  @brief
  *    Returns the trace of the matrix
  *
  *  @return
  *    The trace of the matrix
  *
  *  @remarks
  *    The trace of the matrix is the sum of the main diagonal elements:\n
  *      xx+yy+zz+ww
  */
  TType get_trace() const;

  /**
  *  @brief
  *    Returns the determinant of the matrix
  *
  *  @return
  *    Determinant of the matrix
  *
  *  @note
  *    - If the determinant is non-zero, then an inverse matrix exists
  *    - If the determinant is 0, the matrix is called singular, else nonsingular (invertible) matrix
  *    - If the determinant is 1, the inverse matrix is equal to the transpose of the matrix
  */
  TType get_determinant() const;


  /**
  *  @brief
  *    Transpose this matrix
  *
  *  @remarks
  *    The transpose of matrix is the matrix generated when every element in
  *    the matrix is swapped with the opposite relative to the major diagonal
  *    This can be expressed as the mathematical operation:
  *    @code
  *      M'   = M
  *        ij    ji
  *
  *      | xx xy xz xw |                    | xx yx zx wx |
  *      | yx yy yz yw |  the transpose is  | xy yy zy wy |
  *      | zx zy zz zw |                    | xz yz zz wz |
  *      | wx wy wz ww |                    | xw yw zw ww |
  *    @endcode
  *
  *  @note
  *    - If the matrix is a rotation matrix (= isotropic matrix = determinant is 1),
  *      then the transpose is guaranteed to be the inverse of the matrix
  */
  void transpose();

  /**
  *  @brief
  *    Returns the transposed matrix
  *
  *  @return
  *    Transposed matrix
  *
  *  @see
  *    - Transpose()
  */
  Mat33<TType> get_transposed() const;

  /**
  *  @brief
  *    Inverts the matrix
  *
  *  @return
  *    'true' if all went fine, else 'false' (maybe the determinant is null?)
  *
  *  @note
  *    - If the determinant is 1, the inversed matrix is equal to the transposed one
  */
  bool invert();

  /**
  *  @brief
  *    Returns the inverse of the matrix
  *
  *  @return
  *    Inverse of the matrix, if the determinant is null, an identity matrix is returned
  */
  Mat33<TType> get_inverted() const;
  /**
      *  @brief
      *    Sets a scale matrix
      *
      *  @param[in] x
      *    X scale
      *  @param[in] y
      *    Y scale
      *  @param[in] z
      *    Z scale
      *
      *  @remarks
      *    @code
      *    | x 0 0 0 |
      *    | 0 y 0 0 |
      *    | 0 0 z 0 |
      *    | 0 0 0 1 |
      *    @endcode
      */
  void set_scale_matrix(TType x, TType y, TType z);

  void set_scale_matrix(const Vec3<TType>& scale);

  /**
  *  @brief
  *    Extracts the scale vector from the matrix as good as possible
  *
  *  @param[out] x
  *    Receives the x scale
  *  @param[out] y
  *    Receives the y scale
  *  @param[out] z
  *    Receives the z scale
  *
  *  @note
  *    - This function will not work correctly if one or two components are negative while
  *      another is/are not (we can't figure out WHICH axis are negative!)
  */
  void get_scale(TType& x, TType& y, TType& z) const;

  Vec3<TType> get_scale() const;

  void get_scale(TType v[]) const;


  /**
  *  @brief
  *    Sets an x axis rotation matrix by using one given Euler angle
  *
  *  @param[in] x
  *    Rotation angle around the x axis (in radian, between [0, Math::Pi2])
  *
  *  @remarks
  *    @code
  *         |    1       0       0    0 |
  *    RX = |    0     cos(a) -sin(a) 0 |
  *         |    0     sin(a)  cos(a) 0 |
  *         |    0       0       0    1 |
  *    @endcode
  *    where a > 0 indicates a counterclockwise rotation in the yz-plane (if you look along -x)
  */
  void from_euler_angle_x(TType x);

  /**
  *  @brief
  *    Sets an y axis rotation matrix by using one given Euler angle
  *
  *  @param[in] y
  *    Rotation angle around the y axis (in radian, between [0, Math::Pi2])
  *
  *  @remarks
  *    @code
  *         |  cos(a)    0     sin(a) 0 |
  *    RY = |    0       1       0    0 |
  *         | -sin(a)    0     cos(a) 0 |
  *         |    0       0       0    1 |
  *    @endcode
  *    where a > 0 indicates a counterclockwise rotation in the zx-plane (if you look along -y)
  */
  void from_euler_angle_y(TType y);

  /**
  *  @brief
  *    Sets an z axis rotation matrix by using one given Euler angle
  *
  *  @param[in] z
  *    Rotation angle around the z axis (in radian, between [0, Math::Pi2])
  *
  *  @remarks
  *    @code
  *         |  cos(a) -sin(a)    0    0 |
  *    RZ = |  sin(a)  cos(a)    0    0 |
  *         |    0       0       1    0 |
  *         |    0       0       0    1 |
  *    @endcode
  *    where a > 0 indicates a counterclockwise rotation in the xy-plane (if you look along -z)
  */
  void from_euler_angle_z(TType z);

  /**
  *  @brief
  *    Returns a rotation matrix as a selected axis and angle
  *
  *  @param[out] x
  *    Will receive the x component of the selected axis
  *  @param[out] y
  *    Will receive the y component of the selected axis
  *  @param[out] z
  *    Will receive the z component of the selected axis
  *  @param[out] angle
  *    Will receive the rotation angle around the selected axis (in radian, between [0, Math::Pi])
  */
  void to_axis_angle(TType& x, TType& y, TType& z, TType& angle) const;

  /**
  *  @brief
  *    Sets a rotation matrix by using a selected axis and angle
  *
  *  @param[in] x
  *    X component of the selected axis
  *  @param[in] y
  *    Y component of the selected axis
  *  @param[in] z
  *    Z component of the selected axis
  *  @param[in] angle
  *    Rotation angle around the selected axis (in radian, between [0, Math::Pi])
  *
  *  @note
  *    - The given selected axis must be normalized!
  */
  void from_axis_angle(TType x, TType y, TType z, TType angle);

  void from_axis_angle(const Vec3<TType>& axis, TType angle);


  /**
  *  @brief
  *    Returns the x (left) axis
  *
  *  @return
  *    The x (left) axis
  *
  *  @remarks
  *    @code
  *    | x 0 0 0 |
  *    | y 0 0 0 |
  *    | z 0 0 0 |
  *    | 0 0 0 1 |
  *    @endcode
  *
  *  @note
  *    - It's possible that the axis vector is not normalized because for instance
  *      the matrix was scaled
  */
  Vec3<TType> get_x_axis() const;

  /**
  *  @brief
  *    Returns the y (up) axis
  *
  *  @return
  *    The y (up) axis
  *
  *  @remarks
  *    @code
  *    | 0 x 0 0 |
  *    | 0 y 0 0 |
  *    | 0 z 0 0 |
  *    | 0 0 0 1 |
  *    @endcode
  *
  *  @see
  *    - GetXAxis()
  */
  Vec3<TType> get_y_axis() const;

  /**
  *  @brief
  *    Returns the z (forward) axis
  *
  *  @return
  *    The z (forward) axis
  *
  *  @remarks
  *    @code
  *    | 0 0 x 0 |
  *    | 0 0 y 0 |
  *    | 0 0 z 0 |
  *    | 0 0 0 1 |
  *    @endcode
  *
  *  @see
  *    - GetXAxis()
  */
  Vec3<TType> get_z_axis() const;

  /**
  *  @brief
  *    Returns the three axis of a rotation matrix (not normalized)
  *
  *  @param[out] x
  *    Will receive the x axis
  *  @param[out] y
  *    Will receive the y axis
  *  @param[out] z
  *    Will receive the z axis
  *
  *  @remarks
  *    @code
  *    | x.x y.x z.x 0 |
  *    | x.y y.y z.y 0 |
  *    | x.z y.z z.z 0 |
  *    | 0   0   0   1 |
  *    @endcode
  */
  void to_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const;

  /**
  *  @brief
  *    Sets a rotation matrix by using three given axis
  *
  *  @param[in] x
  *    X axis
  *  @param[in] y
  *    Y axis
  *  @param[in] z
  *    Z axis
  *
  *  @see
  *    - ToAxis()
  */
  void from_axis(Vec3<TType>& x, Vec3<TType>& y, Vec3<TType>& z) const;


  /**
  *  @brief
  *    Builds a look-at matrix
  *
  *  @param[in] eye
  *    Eye position
  *  @param[in] at
  *    Camera look-at target
  *  @param[in] up
  *    Current world's up, usually [0, 1, 0]
  *
  *  @return
  *    This instance
  */
  Mat33<TType>& look_at(const Vec3<TType>& eye, Vec3<TType>& at, Vec3<TType>& up);

public:
  /**
   * @brief
   * Some direct matrix accesses
   */
  union {
    /**
     * @brief
     * One dimensional array representation
     *
     * @remarks
     * The entry of the matrix in row r (0 <= r <= 2) and column c (0 <= c <= 2) is
     * stored at index i = r+3*c (0 <= i <= 8).\n
     * Indices:
     * @code
     *   | 0 3 6 |
     *   | 1 4 7 |
     *   | 2 5 8 |
     * @endcode
     */
    TType mM[9];

    /**
     * @brief
     * Direct element representation
     *
     * @remarks
     * Indices: (row/column)
     * @code
     *   | xx xy xz |
     *   | yx yy yz |
     *   | zx zy zz |
     * @endcode
     * It's recommended to use this way to access the elements.
     */
    struct {
      TType xx, yx, zx;
      TType xy, yy, zy;
      TType xz, yz, zz;
    };

    /**
     * @brief
     * Two dimensional array representation
     *
     * @remarks
     * fM33[i][j] -> i=column, j=row\n
     * Try to avoid this access mode. This can be a problem on a platform/console that
     * chooses to store the data in column-major rather than row-major format.
     */
    struct {
      TType mM33[3][3];
    };
  };
};

typedef Mat33<float32> Mat33f;
typedef Mat33<float64> Mat33d;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/mat33.inl"