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
#include "gui/gui/data/clipboard_data.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ClipboardData::ClipboardData()
: mDataType(EDataType::DataEmpty) {
}

ClipboardData::ClipboardData(const core::String& data)
: mDataType(EDataType::DataString)
, mString(data){
}

ClipboardData::ClipboardData(const ClipboardData& other)
: mDataType(other.mDataType) {
}

ClipboardData::~ClipboardData() {
}

ClipboardData& ClipboardData::operator=(const ClipboardData& other) {
  mDataType = other.mDataType;

  return *this;
}

bool ClipboardData::operator==(const ClipboardData& other) const {
  if (mDataType != other.mDataType)
    return false;

  switch (mDataType) {
    case EDataType::DataString:
      return mString == other.mString;
    default:
      return false;
  }
}

EDataType ClipboardData::get_data_type() const {
  return mDataType;
}

core::String ClipboardData::get_string() const {
  // Sanity check
  BE_ASSERT(mDataType == EDataType::DataString, "ClipboardData::get_string() called on a non-string data type");

  return mString;
}

void ClipboardData::set_string(const core::String& data) {
  clear();

  mDataType = EDataType::DataString;
  mString = data;
}

void ClipboardData::clear() {
  if (mDataType == EDataType::DataString)
    mString.clear();
  mDataType = EDataType::DataEmpty;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}