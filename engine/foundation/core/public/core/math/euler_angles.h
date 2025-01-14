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
#include "core/math/vec3.h"
#include "core/math/vec4.h"


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
template <typename TType> class Quaternion;
template <typename TType> class Mat33;
template <typename TType> class Mat44;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * EulerAngles
 *
 * @brief
 * Static Euler angles conversion tool class
 *
 * @remarks
 * The implementation of this class is basing on the 'Euler Angle Conversion' gem from Ken Shoemake (1993)
 * published in the 'Graphics Gems IV' book. The original code is available here:
 * http://www.graphicsgems.org/gemsiv/euler_angle/
 *
 * @note
 * - Within Bitray, only 'XYZs' Euler angles are used
 * - When dealing with Euler angles keep care of 'gimbal lock', at http://www.sjbaker.org/steve/omniv/eulers_are_evil.html
 *   you will find some good information about this topic
 */
template<typename TType>
class EulerAngles {
public:
  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Euler angles order
   */
  enum EOrder {
    // Static axes (takes axes from initial static frame)
    XYZs = 0,	/**< XYZ static axis */
    XYXs = 2,	/**< XYX static axis */
    XZYs = 4,	/**< XZY static axis */
    XZXs = 6,	/**< XZX static axis */
    YZXs = 8,	/**< YZX static axis */
    YZYs = 10,	/**< YZY static axis */
    YXZs = 12,	/**< YXZ static axis */
    YXYs = 14,	/**< YXY static axis */
    ZXYs = 16,	/**< ZXY static axis */
    ZXZs = 18,	/**< ZXZ static axis */
    ZYXs = 20,	/**< ZYX static axis */
    ZYZs = 22,	/**< ZYZ static axis */
    // Rotating axes
    ZYXr = 1,	/**< ZYX rotating axis */
    XYXr = 3,	/**< XYX rotating axis */
    YZXr = 5,	/**< YZX rotating axis */
    XZXr = 7,	/**< XZX rotating axis */
    XZYr = 9,	/**< XZY rotating axis */
    YZYr = 11,	/**< YZY rotating axis */
    ZXYr = 13,	/**< ZXY rotating axis */
    YXYr = 15,	/**< YXY rotating axis */
    YXZr = 17,	/**< YXZ rotating axis */
    ZXZr = 19,	/**< ZXZ rotating axis */
    XYZr = 21,	/**< XYZ rotating axis */
    ZYZr = 23	/**< ZYZ rotating axis */
  };

public:
  /**
   * @brief
   * Sets a rotation quaternion by using three given Euler angles
   *
   * @param[in]  z
   * Rotation angle around the x axis (in radian)
   * where fAngleX > 0 indicates a counterclockwise rotation in the yz-plane (if you look along -x)
   * @param[in]  y
   * Rotation angle around the y axis (in radian)
   * where fAngleY > 0 indicates a counterclockwise rotation in the zx-plane (if you look along -y)
   * @param[in]  z
   * Rotation angle around the z axis (in radian)
   * where fAngleZ > 0 indicates a counterclockwise rotation in the xy-plane (if you look along -z)
   * @param[in]  order
   * Order of the Euler angles
   *
   * @return
   * Resulting rotation quaternion
   */
  static Quaternion<TType> to_quaternion(TType x, TType y, TType z, EOrder order = XYZs);

  /**
   * @brief
   * Sets a rotation quaternion by using three given Euler angles
   *
   * @param[in]  angles
   * Vector containing all rotation angles.
   * where fAngleZ > 0 indicates a counterclockwise rotation in the xy-plane (if you look along -z)
   * @param[in]  order
   * Order of the Euler angles
   *
   * @return
   * Resulting rotation quaternion
   */
  static Quaternion<TType> from_quaternion(Vec3<TType> angles, EOrder order = XYZs);
};

typedef EulerAngles<core::float32> EulerAnglesf;
typedef EulerAngles<core::float64> EulerAnglesd;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/math/euler_angles.inl"