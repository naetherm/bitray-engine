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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include <atomic>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


template<typename T> using atomic = std::atomic<T>;
using atomic_bool = std::atomic<bool>;
using atomic_char = std::atomic<char>;
using atomic_uchar = std::atomic<unsigned char>;
using atomic_wchar_t = std::atomic<wchar_t>;
using atomic_short = std::atomic<short>;
using atomic_ushort = std::atomic<unsigned short>;
using atomic_int = std::atomic<int>;
using atomic_uint = std::atomic<unsigned int>;
using atomic_long = std::atomic<long>;
using atomic_ulong = std::atomic<unsigned long>;
using atomic_llong = std::atomic<long long>;
using atomic_ullong = std::atomic<unsigned long long>;
using atomic_size_t = std::atomic<size_t>;
using atomic_ptrdiff_t = std::atomic<ptrdiff_t>;
using atomic_int8 = std::atomic<int8>;
using atomic_uint8 = std::atomic<uint8>;
using atomic_int16 = std::atomic<int16>;
using atomic_uint16 = std::atomic<uint16>;
using atomic_int32 = std::atomic<int32>;
using atomic_uint32 = std::atomic<uint32>;
using atomic_int64 = std::atomic<int64>;
using atomic_uint64 = std::atomic<uint64>;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core