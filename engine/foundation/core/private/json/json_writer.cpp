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
#include "core/json/json_writer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
JsonPrinter::JsonPrinter(uint64 indentationLength)
: mIndentationLength(indentationLength) {

}

JsonPrinter::~JsonPrinter() {

}


String JsonPrinter::convert_to_string(const core::JsonValue &jsonValue) {
  String result;

  // This is the root object, we typically _always_ have an object on root level
  BE_ASSERT(jsonValue.is_object(), "On Root level we expect an Object")

  uint64 numElements = jsonValue.get_object().size();
  Vector<String> keys;
  Vector<JsonValue*> values;
  jsonValue.get_object().get_keys(keys);
  jsonValue.get_object().get_values(values);
  for (uint64 i = 0; i < numElements; ++i) {
    result += keys[i] + ": " + value_to_string(*values[i], 1);
    if (i < (numElements-1)) {
      result += ",";
    }
    result += "\n";
  }

  return result;
}


String JsonPrinter::value_to_string(const JsonValue& jsonValue, uint64 level) {
  switch(jsonValue.get_type()) {
    case JsonType_Null:
      return null_to_string(jsonValue, level);
    case JsonType_Boolean:
      return boolean_to_string(jsonValue, level);
    case JsonType_Number:
      return number_to_string(jsonValue, level);
    case JsonType_Integer:
      return integer_to_string(jsonValue, level);
    case JsonType_String:
      return string_to_string(jsonValue, level);
    case JsonType_Array:
      return array_to_string(jsonValue, level);
    case JsonType_Object:
      return object_to_string(jsonValue, level);
  }
  return String();
}

String JsonPrinter::null_to_string(const JsonValue& jsonValue, uint64 level) {
  return "null";
}

String JsonPrinter::boolean_to_string(const JsonValue& jsonValue, uint64 level) {
  return jsonValue.to_bool() ? "true" : "false";
}

String JsonPrinter::number_to_string(const JsonValue& jsonValue, uint64 level) {
  return core::to_string(jsonValue.to_number());
}

String JsonPrinter::integer_to_string(const JsonValue& jsonValue, uint64 level) {
  return core::to_string(jsonValue.to_integer());
}

String JsonPrinter::string_to_string(const JsonValue& jsonValue, uint64 level) {
  if (jsonValue.is_raw_text()) {
    return "\"\"\"" + jsonValue.to_string() + "\"\"\"";
  } else {
    return "\"" + jsonValue.to_string() + "\"";
  }
}

String JsonPrinter::array_to_string(const JsonValue& jsonValue, uint64 level) {
  String result = "[\n";

  Vector<JsonValue*> array = jsonValue.get_array();

  if (array.size() > 0) {
    result += make_indent(level) + value_to_string(*array[0], level+1);

    for (uint64 i = 1; i < array.size(); ++i) {
      result += ",\n" + make_indent(level) + value_to_string(*array[i], level+1);
    }
  }

  result += "\n" + make_indent(level > 0 ? level-1 : level) + "]";
  return result;
}

String JsonPrinter::object_to_string(const JsonValue& jsonValue, uint64 level) {
  String result = "{\n";

  uint64 numElements = jsonValue.get_object().size();
  Vector<String> keys;
  Vector<JsonValue*> values;
  jsonValue.get_object().get_keys(keys);
  jsonValue.get_object().get_values(values);
  for (uint64 i = 0; i < numElements; ++i) {
    result += make_indent(level) + keys[i] + ": " + value_to_string(*values[i], level+1);
    if (i < (numElements-1)) {
      result += ",";
    }
    result += "\n";
  }

  result += make_indent(level > 0 ? level-1 : level) + "}";

  return result;
}

String JsonPrinter::make_indent(core::uint64 level) {
  String result(level * mIndentationLength, ' ');

  return result;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}