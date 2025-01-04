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
#include "core/container/map.h"
#include "core/container/vector.h"
#include "core/json/types.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class JsonValue {

  friend class JsonParser;

public:

  JsonValue();

  JsonValue(const JsonValue& rhs);

  JsonValue(JsonValue&& rhs);

  ~JsonValue();


  JsonValue& operator=(const JsonValue& rhs);

  JsonValue& operator=(JsonValue&& rhs);

  [[nodiscard]] const JsonValue& operator()(uint32 index) const;

  [[nodiscard]] JsonValue& operator()(uint32 index);

  [[nodiscard]] const JsonValue& operator[](const String& key) const;

  [[nodiscard]] JsonValue& operator[](const String& key);


  // Testing

  [[nodiscard]] inline bool is_bool() const;

  [[nodiscard]] inline bool is_integer() const;

  [[nodiscard]] inline bool is_number() const;

  [[nodiscard]] inline bool is_string() const;

  [[nodiscard]] inline bool is_null() const;

  [[nodiscard]] inline bool is_array() const;

  [[nodiscard]] inline bool is_object() const;

  [[nodiscard]] bool has_member(const String& key) const;

  [[nodiscard]] inline bool is_raw_text() const;


  // Get

  [[nodiscard]] inline const Vector<JsonValue*>& get_array() const;

  [[nodiscard]] inline Vector<JsonValue*>& get_array();

  [[nodiscard]] inline const Map<String, JsonValue*>& get_object() const;

  [[nodiscard]] inline Map<String, JsonValue*>& get_object();

  [[nodiscard]] inline core::uint32 get_size() const;


  // To

  [[nodiscard]] EJsonType get_type() const;

  [[nodiscard]] bool to_bool() const;

  [[nodiscard]] int64 to_integer() const;

  [[nodiscard]] float to_number() const;

  [[nodiscard]] const String& to_string() const;

  [[nodiscard]] const Vector<JsonValue*>& to_array() const;

  [[nodiscard]] const Map<String, JsonValue*>& to_object() const;

  [[nodiscard]] String& to_string();

  [[nodiscard]] Vector<JsonValue*>& to_array();

  [[nodiscard]] Map<String, JsonValue*>& to_object();

private:

  void copy_from(const JsonValue& rhs);

  void copy_from(JsonValue&& rhs);

private:

  EJsonType mType;

  union {
    bool mBool;
    int64 mInteger;
    float mNumber;
  };

  //union {
    Vector<JsonValue*> mArray;
    Map<String, JsonValue*> mObject;
    String mString;
  //};

  bool mIsRawTextBlock;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/json/json_value.inl"