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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
#include "core/io/media_type.h"
#include "core/container/vector.h"
#include "core/string/string_splitter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
MediaType::MediaType() {

}

MediaType::MediaType(const String &str) {
  set(str);
}

MediaType::MediaType(const String &type, const String &subType)
: mType(type)
, mSubType(subType) {

}

MediaType::MediaType(const MediaType &rhs)
: mType(rhs.mType)
, mSubType(rhs.mSubType) {

}

MediaType &MediaType::operator=(const MediaType &rhs) {
  mType = rhs.mType;
  mSubType = rhs.mSubType;

  return *this;
}

bool MediaType::operator==(const MediaType &rhs) const {
  return ((mType == rhs.mSubType) && (mSubType == rhs.mSubType));
}

bool MediaType::operator!=(const MediaType &rhs) const {
  return !operator==(rhs);
}

bool MediaType::is_valid() const {
  return !mType.empty() && !mSubType.empty();
}

void MediaType::clear() {
  mType.clear();
  mSubType.clear();
}

void MediaType::set(const String &str) {
  Vector<String> tokens = StringSplitter::split(str, "/");
  mType = tokens[0];
  mSubType = tokens[1];
}

void MediaType::set(const String &type, const String &subType) {
  mType = type;
  mSubType = subType;
}

String MediaType::as_string() const {
  return mType + "/" + mSubType;
}

const String &MediaType::get_type() const {
  return mType;
}

const String &MediaType::get_sub_type() const {
  return mSubType;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}