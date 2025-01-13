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


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Half
 * @brief
 * Static helper class for the half data type (16 bit floating point)
 *
 * @note
 * - This class is using information from "Fast Half Float Conversions" (ftp://ftp.fox-toolkit.org/pub/fasthalffloatconversion.pdf)
 *   written by Jeroen van der Zijp, November 2008 (Revised September 2010)
 * - OpenEXR IlmBase 1.0.2 (Modified BSD License)
 */
class Half {
public:
  /** Representation of 0.0 */
  static const uint16 Zero;
  /** Representation of 1.0 */
  static const uint16 One;

  /** Smallest positive half (5.96046448e-08f) */
  static const float32 SmallestPositive;
  /** Smallest positive normalized half (6.10351562e-05f) */
  static const float32 SmallestPositiveNormalized;
  /** Largest positive half (65504.0f) */
  static const float32 LargestPositive;
  /** Smallest positive epsilon for which 1+e!=1 (0.00097656f) */
  static const float32 Epsilon;

public:
  /**
   * @brief
   * Returns whether the given half value is zero
   *
   * @param[in] half
   * Half value to check
   *
   * @return
   * 'true' if the given half value is zero, else 'false'
   */
  static bool is_zero(uint16 half);

  /**
   * @brief
   * Returns whether the given half value is negative
   *
   * @param[in] half
   * Half value to check
   *
   * @return
   * 'true' if the given half value is negative, else 'false'
   */
  static bool is_negative(uint16 half);

  /**
   * @brief
   * Returns whether the given half value is not a number (NAN)
   *
   * @param[in] half
   * Half value to check
   *
   * @return
   * 'true' if the given half value is not a number, else 'false'
   */
  static bool is_not_a_number(uint16 half);

  /**
   * @brief
   * Returns whether the given half value is finite
   *
   * @param[in] half
   * Half value to check
   *
   * @return
   * 'true' if the given half value is finite, else 'false'
   */
  static bool is_finite(uint16 half);

  /**
   * @brief
   * Returns whether the given half value is infinity
   *
   * @param[in] half
   * Half value to check
   *
   * @return
   * 'true' if the given half value is infinity, else 'false'
   */
  static bool is_infinity(uint16 half);

  /**
   * @brief
   * Returns whether the given half value is normalized
   *
   * @param[in] half
   * Half value to check
   *
   * @return
   * 'true' if the given half value is normalized, else 'false'
   */
  static bool is_normalized(uint16 half);

  /**
   * @brief
   * Returns whether the given half value is de-normalized
   *
   * @param[in] half
   * Half value to check
   *
   * @return
   * 'true' if the given half value is de-normalized, else 'false'
   */
  static bool is_denormalized(uint16 half);

  /**
   * @brief
   * Converts a given half value into a float value
   *
   * @param[in] half
   * Half value to convert
   *
   * @return
   * The given half value as float
   */
  static float32 to_float(uint16 half);

  /**
   * @brief
   * Converts a given float value into a half value
   *
   * @param[in] f
   * Float value to convert
   *
   * @return
   * The given float value as half
   */
  static uint16 from_float(float32 f);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}