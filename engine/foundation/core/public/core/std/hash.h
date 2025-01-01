////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
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
#include "core/std/typetraits/config.h"
#include "core/std/typetraits/is_enum.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename T, bool Enabled>
struct EnablehashIf {};
template<typename T>
struct EnablehashIf<T, true> {
  sizeT operator()(const T& p) const {
    return sizeT(p);
  }
};

template<typename T>
struct hash;

template<class T>
struct hash : EnablehashIf<T, core::is_enum_v<T>> {
  typedef T argument_type;
  typedef core::sizeT result_type;

  result_type operator()(const argument_type& value) const {
    return static_cast<result_type>(value);
  }
};

template<>
struct hash<bool> {
  sizeT operator()(bool value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<uint8> {
  sizeT operator()(uint8 value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<uint16> {
  sizeT operator()(uint16 value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<uint32> {
  sizeT operator()(uint32 value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<uint64> {
  sizeT operator()(uint64 value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<int8> {
  sizeT operator()(int8 value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<int16> {
  sizeT operator()(int16 value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<int32> {
  sizeT operator()(int32 value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<int64> {
  sizeT operator()(int64 value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<float> {
  sizeT operator()(float value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<double> {
  sizeT operator()(double value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<char> {
  sizeT operator()(char value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<char16_t> {
  sizeT operator()(char16_t value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<char32_t> {
  sizeT operator()(char32_t value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<wchar_t> {
  sizeT operator()(wchar_t value) const { return static_cast<sizeT>(value); }
};
template<>
struct hash<char *> {
  sizeT operator()(const char *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct hash<wchar_t *> {
  sizeT operator()(const wchar_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct hash<char16_t *> {
  sizeT operator()(const char16_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct hash<char32_t *> {
  sizeT operator()(const char32_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct hash<const char *> {
  sizeT operator()(const char *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct hash<const wchar_t *> {
  sizeT operator()(const wchar_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct hash<const char16_t *> {
  sizeT operator()(const char16_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct hash<const char32_t *> {
  sizeT operator()(const char32_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}