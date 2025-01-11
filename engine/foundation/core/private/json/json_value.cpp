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
#include "core/json/json_value.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
JsonValue::JsonValue()
: mType(EJsonType::JsonType_Null)
, mBool(false)
//, mArray(nullptr)
, mIsRawTextBlock(false) {

}

JsonValue::JsonValue(const JsonValue& rhs)
: mType(rhs.mType)
, mIsRawTextBlock(rhs.mIsRawTextBlock) {
  copy_from(rhs);
}

JsonValue::JsonValue(JsonValue&& rhs)
: mType(rhs.mType)
, mIsRawTextBlock(rhs.mIsRawTextBlock) {
  copy_from(rhs);
}

JsonValue::~JsonValue() {
  // Based on type, call the correct cleanup
  if (mType == JsonType_Object) {
    Vector<JsonValue*> values;
    mObject.get_values(values);
    for (auto& value: values) {
      //delete value;
    }
    mObject.clear();
    //delete mObject;
  }
  else if (mType == JsonType_Array) {
    for (auto& value: mArray) {
      //delete value;
    }
    mArray.clear();
    //delete mArray;
  }
  else if (mType == JsonType_String) {
    mString.clear();
    //delete mString;
  }
}


JsonValue& JsonValue::operator=(const core::JsonValue &rhs) {
  if (this != &rhs) {
    mType = rhs.mType;
    mIsRawTextBlock = rhs.mIsRawTextBlock;
    copy_from(rhs);
  }

  return *this;
}

JsonValue& JsonValue::operator=(core::JsonValue &&rhs) {
  if (this != &rhs) {
    mType = rhs.mType;
    mIsRawTextBlock = rhs.mIsRawTextBlock;
    copy_from(rhs);
  }

  return *this;
}

const JsonValue &JsonValue::operator()(uint32 index) const {
  BE_ASSERT(is_array(), "Cannot interpret as array, is of other type")
  //auto iter = mArray->at(index);
  //return iter;
  return *mArray.at(index);
}

JsonValue &JsonValue::operator()(uint32 index) {
  BE_ASSERT(is_array(), "Cannot interpret as array, is of other type")
  //auto iter = mArray->at(index);
  //return iter;
  return *mArray.at(index);
}

const JsonValue &JsonValue::operator[](const String &key) const {
  BE_ASSERT(is_object(), "Cannot interpret as object, is of other type")
  return *(mObject)[key]; // *mObject->find(key).value();
}

JsonValue &JsonValue::operator[](const String &key) {
  BE_ASSERT(is_object(), "Cannot interpret as object, is of other type")
  return *(mObject)[key]; // ->find(key).value();
}


bool JsonValue::has_member(const core::String &key) const {
  if (mType == JsonType_Object) {
    return mObject.contains(key);
  }

  // Error
  return false;
}

EJsonType JsonValue::get_type() const {
  return mType;
}

bool JsonValue::to_bool() const {
  BE_ASSERT(is_bool(), "Cannot interpret as bool, is of other type")
  return mBool;
}

int64 JsonValue::to_integer() const {
  BE_ASSERT(is_integer(), "Cannot interpret as integer, is of other type")
  return mInteger;
}

float JsonValue::to_number() const {
  BE_ASSERT(is_number(), "Cannot interpret as number, is of other type")
  return mNumber;
}

const String& JsonValue::to_string() const {
  BE_ASSERT(is_string(), "Cannot interpret as string, is of other type")
  return mString;
}

const Vector<JsonValue*> &JsonValue::to_array() const {
  BE_ASSERT(is_array(), "Cannot interpret as array, is of other type")
  return mArray;
}

const Map<String, JsonValue*> &JsonValue::to_object() const {
  BE_ASSERT(is_object(), "Cannot interpret as object, is of other type")
  return mObject;
}

String& JsonValue::to_string() {
  BE_ASSERT(is_string(), "Cannot interpret as string, is of other type")
  return mString;
}

Vector<JsonValue*> &JsonValue::to_array() {
  BE_ASSERT(is_array(), "Cannot interpret as array, is of other type")
  return mArray;
}

Map<String, JsonValue*> &JsonValue::to_object() {
  BE_ASSERT(is_object(), "Cannot interpret as object, is of other type")
  return mObject;
}


void JsonValue::copy_from(const core::JsonValue &rhs) {
  switch(rhs.mType) {

    case JsonType_Null:
      break;
    case JsonType_Boolean:
      mBool = rhs.mBool;
      break;
    case JsonType_Number:
      mNumber = rhs.mNumber;
      break;
    case JsonType_Integer:
      mInteger = rhs.mInteger;
      break;
    case JsonType_String:
      mString = rhs.mString; // new String(*rhs.mString);
      break;
    case JsonType_Array:
      mArray = rhs.mArray; // new Vector<JsonValue*>(*rhs.mArray);
      break;
    case JsonType_Object:
      mObject = rhs.mObject; // new Map<String, JsonValue*>(*rhs.mObject);
      break;
  }
}

void JsonValue::copy_from(core::JsonValue &&rhs) {
  switch(rhs.mType) {

    case JsonType_Null:
      break;
    case JsonType_Boolean:
      mBool = rhs.mBool;
      break;
    case JsonType_Number:
      mNumber = rhs.mNumber;
      break;
    case JsonType_Integer:
      mInteger = rhs.mInteger;
      break;
    case JsonType_String:
      mString = rhs.mString; // new String(*rhs.mString);
      break;
    case JsonType_Array:
      mArray = rhs.mArray; // new Vector<JsonValue*>(*rhs.mArray);
      break;
    case JsonType_Object:
      mObject = rhs.mObject; // new Map<String, JsonValue*>(*rhs.mObject);
      break;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}