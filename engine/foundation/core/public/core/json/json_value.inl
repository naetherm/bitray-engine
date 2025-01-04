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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
inline const Vector<JsonValue*>& JsonValue::get_array() const {
  return mArray;
}

inline Vector<JsonValue*>& JsonValue::get_array() {
  return mArray;
}

inline const Map<String, JsonValue*>& JsonValue::get_object() const {
  return mObject;
}

inline Map<String, JsonValue*>& JsonValue::get_object() {
  return mObject;
}


inline bool JsonValue::is_bool() const {
  return mType == EJsonType::JsonType_Boolean;
}

inline bool JsonValue::is_integer() const {
  return mType == EJsonType::JsonType_Integer;
}

inline bool JsonValue::is_number() const {
  return mType == EJsonType::JsonType_Number;
}

inline bool JsonValue::is_string() const {
  return mType == EJsonType::JsonType_String;
}

inline bool JsonValue::is_null() const {
  return mType == EJsonType::JsonType_Null;
}

inline bool JsonValue::is_array() const {
  return mType == EJsonType::JsonType_Array;
}

inline bool JsonValue::is_object() const {
  return mType == EJsonType::JsonType_Object;
}

inline bool JsonValue::is_raw_text() const {
  return mIsRawTextBlock;
}

inline core::uint32 JsonValue::get_size() const {
  BE_ASSERT(is_array() || is_object(), "Try to get he size of value that does not support size")

  if (is_array()) {
    return mArray.size();
  } else if (is_object()) {
    return mObject.size();
  }

  // Error
  return 0;
}



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}