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
#include "core/json/json_parser.h"
#include "core/json/json_value.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


static String SNull = "null";
static String SFalse = "false";
static String STrue = "true";


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
JsonValue JsonParser::parse(const String& inputString) {

  JsonValue result; // = new JsonValue();
  JsonParser parser(inputString);
  parser.parse_object(&result, true);

  return result;
}


JsonParser::JsonParser(const String& inputString)
: mInputString(inputString)
, mCurrentPosition(0) {

}

JsonParser::~JsonParser() {
  // Nothing to do, do not delete the JsonValue as we will need it outside
}


bool JsonParser::parse_null(JsonValue* value) {
  if (is_string(SNull)) {

    value->mType = EJsonType::JsonType_Null;
    mCurrentPosition += SNull.length();

    return true;
  }

  return false;
}

bool JsonParser::parse_value(JsonValue *value) {
  skip_whitespace();
  char ch = current();

  switch (ch) {
    case '{': return parse_object(value, false);
    case '[': return parse_array(value);
    case '"': return parse_string(value);
    case '-': return parse_number(value);
    case 'f': return parse_false(value);
    case 't': return parse_true(value);
    case 'n': return parse_null(value);
    default: return ch >= '0' && ch <= '9' && parse_number(value);
  }
}

bool JsonParser::parse_false(JsonValue *value) {
  if (is_string(SFalse)) {
    value->mType = JsonType_Boolean;
    value->mBool = false;
    mCurrentPosition += SFalse.length();
    return true;
  }

  return false;
}

bool JsonParser::parse_true(JsonValue *value) {
  if (is_string(STrue)) {
    value->mType = JsonType_Boolean;
    value->mBool = true;
    mCurrentPosition += STrue.length();
    return true;
  }

  return false;
}

bool JsonParser::parse_number(JsonValue *value) {
  bool isFloat = false;
  uint64 start = mCurrentPosition;

  if (current() == '-')
    next();

  while (current() >= '0' && current() <= '9')
    next();

  if (current() == '.') {
    isFloat = true;
    next();

    while (current() >= '0' && current() <= '9')
      next();
  }

  if (current() == 'e' || current() == 'E') {
    isFloat = true;
    next();

    if (current() == '-' || current() == '+')
      next();

    while (current() >= '0' && current() <= '9')
      next();
  }

  // Get string
  String result = mInputString.substr(start, (mCurrentPosition-start));

  if (isFloat)
  {
    value->mType = JsonType_Number;
    value->mNumber = (float)strtod(result.c_str(), nullptr);
  }
  else
  {
    value->mType = JsonType_Integer;
    value->mInteger = (int)strtol(result.c_str(), nullptr, 10);
  }

  return true;
}

bool JsonParser::parse_array(JsonValue *value) {
  if (current() != '[')
    return false;

  value->mType = JsonType_Array;

  next();

  // Empty array.
  if (current() == ']') {
    return true;
  }

  while (current()) {
    skip_whitespace();

    auto* val = new JsonValue();
    if (!parse_value(val))
      return false;

    value->mArray.push_back(val);

    skip_whitespace();

    if (current() == ']') {
      next();
      break;
    }
  }

  return true;
}

bool JsonParser::parse_string(JsonValue *value) {
  String str = parse_string_internal(value);

  if (str.empty())
    return false;

  value->mType = JsonType_String;
  //auto tmp = new String(str);;
  value->mString = str;
  return true;
}

bool JsonParser::parse_object(JsonValue* value, bool isRootObject) {
  if (current() == '{') {
    next();
  } else if (!isRootObject) {
    // There is something wrong!
    return false;
  }

  // The current json value is a table
  value->mType = JsonType_Object;
  // Create the corresponding table
  //value->mObject = new Map<String, JsonValue*>();
  skip_whitespace();

  // Check if just an empty object
  if (current() == '}') {
    return true;
  }

  // Now loop through all entries
  while (current()) {
    skip_whitespace();
    // The next one is definitively a key name
    String key = parse_key();
    skip_whitespace();

    if (key.empty() || current() != ':')
      return false;

    next();

    JsonValue* newValue = new JsonValue();
    if (!parse_value(newValue))
      return false;

    value->mObject.insert(key, newValue);
    //(*value->mObject)[key] = newValue;


    skip_whitespace();

    if (current() == '}') {
      next();
      break;
    }
  }

  return true;
}


char JsonParser::next() {
  return mInputString[mCurrentPosition++];
}

char JsonParser::current() {
  return mInputString[mCurrentPosition];
}

bool JsonParser::is_string(const String &str) const {
  return mInputString.compare(mCurrentPosition, str.size(), str) == 0;
}

String JsonParser::parse_key() {
  String result;
  if (current() == '"')
    return parse_string_internal();

  uint64 start = mCurrentPosition;

  while (current())
  {
    uint64 cur_wo_whitespace = mCurrentPosition;
    if (is_whitespace(current()))
      skip_whitespace();

    if (current() == ':')
      return mInputString.substr(start, cur_wo_whitespace - start);

    next();
  }

  return result;
}

bool JsonParser::is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void JsonParser::skip_whitespace() {
  while (current()) {
    while (current() && (current() <= ' ' || current() == ','))
      next();

    // Skip comment.
    if (current() == '#') {
      while (current() && current() != '\n')
        next();
    }
    else
      break;
  }
}

bool JsonParser::is_multiline_string_quotes() {
  return mInputString[mCurrentPosition] == '"' && mInputString[mCurrentPosition+1] == '"' && mInputString[mCurrentPosition+2] == '"';
}

String JsonParser::parse_multiline_string() {
  String result;
  if (!is_multiline_string_quotes())
    return result;

  mCurrentPosition += 3;
  uint64 start = mCurrentPosition;

  while (current())
  {

    if (is_multiline_string_quotes())
    {
      uint64 end = mCurrentPosition;
      String new_result = mInputString.substr(start, end - start);
      result = new_result;
      mCurrentPosition += 3;
      return result;
    }

    next();
  }

  return result;
}

String JsonParser::parse_string_internal(JsonValue* value) {
  String result;
  if (current() != '"')
    return result;

  if (is_multiline_string_quotes()) {
    if (value) {
      value->mIsRawTextBlock = true;
    }
    return parse_multiline_string();
  }

  next();
  uint64 start = mCurrentPosition;

  while (current())
  {
    if (current() == '"')
    {
      uint64 end = mCurrentPosition;
      next();
      return mInputString.substr(start, (end-start));
      break;
    }

    next();
  }

  return result;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}