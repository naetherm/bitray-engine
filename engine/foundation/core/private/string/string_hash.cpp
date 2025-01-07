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
#include "core/string/string_hash.h"
#include "core/core/assert_impl.h"
#include "core/utility/fnv1a.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

[[nodiscard]] uint32 StringHash::calculate_fnv(const char *string) {
  // Sanity check
  BE_ASSERT(nullptr != string, "The string must be valid to be able to calculate a FNV1a32 hash")

  // 32-bit FNV-1a implementation basing on http://www.isthe.com/chongo/tech/comp/fnv/
  uint32 hash = FNV1a_INITIAL_HASH_32;
  for (; '\0' != *string; ++string) {
    hash = (hash ^ *string) * FNV1a_MAGIC_PRIME_32;
  }
  return hash;
}


StringHash::ConstCharWrapper::ConstCharWrapper(const char *string)
: mString(string) {
  // Nothing here
}

StringHash::StringHash()
: mId(MAXIMUM_UINT32_T_VALUE) {
  // Nothing here
}

template<uint32 N>
StringHash::StringHash(const char (&string)[N]) noexcept
: mId(compile_time_fnv(string)) {
  // It's a trap!
#ifdef _WIN32
  static_assert(false, "Use the \"STRING_ID()\" macro to mark compile string IDs");
#endif
  STRING_HASH_SET_SOURCE(string);
}

StringHash::StringHash(const ConstCharWrapper &string)
: mId(calculate_fnv(string.mString)) {
  STRING_HASH_SET_SOURCE(string.mString);
}

StringHash::StringHash(uint32 id)
: mId(id) {
  // Nothing do to in here
}

StringHash::StringHash(const StringHash &stringId)
: mId(stringId.mId) {
  // Nothing here
  STRING_HASH_SET_SOURCE(stringId.mSourceString);
}

[[nodiscard]] uint32 StringHash::get_id() const {
  return mId;
}

[[nodiscard]] StringHash::operator uint32() const {
  return mId;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core