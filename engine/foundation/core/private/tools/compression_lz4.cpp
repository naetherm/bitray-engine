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
#include "core/tools/compression_lz4.h"
#include <lz4hc.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


core::sizeT CompressionLZ4::compress(core::uint8 *dst, const core::uint8 *src, core::sizeT srcSize, core::sizeT dstCapacity) const {
  return LZ4_compress_HC(
    reinterpret_cast<const char*>(src),
    reinterpret_cast<char *>(dst),
    static_cast<int>(srcSize),
    dstCapacity,
    LZ4HC_CLEVEL_MAX);
}

core::sizeT CompressionLZ4::decompress(core::uint8 *dst, const core::uint8 *src, core::sizeT srcSize, core::sizeT dstSize) const {
  return LZ4_decompress_safe(
    reinterpret_cast<const char*>(src),
    reinterpret_cast<char*>(dst),
    static_cast<int>(srcSize),
    static_cast<int>(dstSize));;
}

core::sizeT CompressionLZ4::compress_bound(core::uint32 srcSize) const {
  return LZ4_compressBound(srcSize);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
