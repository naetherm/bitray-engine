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
#include "rhi/rhi.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Index buffer data format
 */
struct IndexBufferFormat final {
  enum Enum {
    /// One byte per element, core::uint8 (may not be supported by each RHI implementation, primarily for mobile devices)
    UNSIGNED_CHAR = 0,
    /// Two bytes per element, core::uint16 (best support across multiple RHI implementations)
    UNSIGNED_SHORT = 1,
    /// Four bytes per element, core::uint32 (may not be supported by each RHI implementation)
    UNSIGNED_INT = 2
  };

  /**
  *  @brief
  *    "rhi::IndexBufferFormat" to number of bytes per element
  *
  *  @param[in] indexFormat
  *    "rhi::IndexBufferFormat" to map
  *
  *  @return
  *    Number of bytes per element
  */
  [[nodiscard]] static inline core::uint32 getNumberOfBytesPerElement(Enum indexFormat) {
    static constexpr core::uint32 MAPPING[] =
      {
        1,  // One byte per element, core::uint8 (may not be supported by each RHI implementation, primarily for mobile devices)
        2,  // Two bytes per element, core::uint16 (best support across multiple RHI implementations)
        4  // Four bytes per element, core::uint32 (may not be supported by each RHI implementation)
      };
    return MAPPING[indexFormat];
  }
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
