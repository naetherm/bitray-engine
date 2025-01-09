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
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Minimalistic floating point RGBA color class
 */
class Color3 final {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
public:
  static const Color3 WHITE;  ///< White  (sRGB="1.0 1.0      1.0", Hex="#FFFFFF", alpha is one)
  static const Color3 BLACK;  ///< Black  (sRGB="0.0 0.0      0.0", Hex="#000000", alpha is one)
  static const Color3 RED;  ///< Red    (sRGB="1.0 0.0      0.0", Hex="#FF0000", alpha is one)
  static const Color3 GREEN;  ///< Green  (sRGB="0.0 1.0      0.0", Hex="#008000", alpha is one)
  static const Color3 BLUE;  ///< Blue   (sRGB="0.0 0.0      1.0", Hex="#0000FF", alpha is one)
  static const Color3 GRAY;  ///< Gray   (sRGB="0.5 0.5      0.5", Hex="#808080", alpha is one)
  static const Color3 CYAN;  ///< Cyan   (sRGB="0.0 1.0      1.0", Hex="#00FFFF", alpha is one)
  static const Color3 YELLOW;  ///< Yellow (sRGB="1.0 1.0      0.0", Hex="#FFFF00", alpha is one)
  static const Color3 ORANGE;  ///< Orange (sRGB="1.0 0.647059 0.0", Hex="#FFA500", alpha is one)


//[-------------------------------------------------------]
//[ Public data                                           ]
//[-------------------------------------------------------]
public:
  union {
    float value[3];
    struct {
      float r, g, b;
    } rgb;
  };


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
  /**
   * @brief
   * Default constructor
   * 
   * @note
   * - Sets all components to zero
   */
  inline Color3();

  /**
   * @brief
   * Copy constructor
   * 
   * @param[in] source
   * Source to copy from
   */
  inline Color3(const Color3 &source);

  inline Color3(Color3 &source);

  explicit Color3(const String &str);

  /**
   * @brief
   * Constructor
   * 
   * @param[in] r
   * Value for the red component
   * @param[in] g
   * Value for the green component
   * @param[in] b
   * Value for the blue component
   * @param[in] a
   * Value for the alpha component
   */
  inline Color3(float _r, float _g, float _b);

  /**
   * @brief
   * Destructor
   */
  inline ~Color3();

  /**
   * @brief
   * Copy operator
   * 
   * @param[in] source
   * Source to copy from
   * 
   * @return
   * Reference to this instance
   */
  inline Color3 &operator=(const Color3 &source);

  //[-------------------------------------------------------]
  //[ Assignment operators                                  ]
  //[-------------------------------------------------------]
  [[nodiscard]] inline operator float *();

  [[nodiscard]] inline operator const float *() const;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


#include "core/color/color3.inl"