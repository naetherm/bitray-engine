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
#include "core/json/json_value.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * JsonPrinter
 *
 * @brief
 * A class that serializes a `JsonValue` into a JSON formatted string.
 *
 * The `JsonPrinter` class is used to convert a `JsonValue` object into a string representation
 * that adheres to the JSON format. The class supports indentation for pretty-printing and
 * offers methods to convert various JSON value types to their corresponding string representations.
 */
class JsonPrinter {
public:

  /**
   * @brief
   * Constructor.
   *
   * Initializes a `JsonPrinter` instance with the specified indentation length.
   *
   * @param[in] indentationLength
   * The number of spaces used for indentation when formatting the JSON. The default is 2 spaces.
   */
  JsonPrinter(uint64 indentationLength = 2);

  /**
   * @brief
   * Destructor.
   *
   * Cleans up any resources used by the `JsonPrinter`.
   */
  ~JsonPrinter();

  /**
   * @brief
   * Converts a `JsonValue` to a formatted JSON string.
   *
   * Serializes the provided `JsonValue` to its JSON string representation.
   *
   * @param[in] jsonValue
   * The `JsonValue` object to convert to a string.
   *
   * @return
   * A string containing the formatted JSON representation of the input value.
   */
  String convert_to_string(const JsonValue& jsonValue);

private:

  /**
   * @brief
   * Converts a `JsonValue` to its string representation (recursive).
   *
   * This is a helper method that converts any `JsonValue` to its string form depending on its type.
   * It is called recursively for nested arrays and objects.
   *
   * @param[in] jsonValue
   * The `JsonValue` object to convert to a string.
   * @param[in] level
   * The current level of indentation for nested structures.
   *
   * @return
   * A string representation of the `JsonValue`.
   */
  String value_to_string(const JsonValue& jsonValue, uint64 level = 0);

  /**
   * @brief
   * Converts a `null` value to its string representation.
   *
   * @param[in] jsonValue
   * The `JsonValue` representing a null value.
   * @param[in] level
   * The current level of indentation.
   *
   * @return
   * The string `"null"`.
   */
  String null_to_string(const JsonValue& jsonValue, uint64 level = 0);

  /**
   * @brief
   * Converts a boolean value to its string representation.
   *
   * @param[in] jsonValue
   * The `JsonValue` representing a boolean value.
   * @param[in] level
   * The current level of indentation.
   *
   * @return
   * The string `"true"` or `"false"`.
   */
  String boolean_to_string(const JsonValue& jsonValue, uint64 level = 0);

  /**
   * @brief
   * Converts a numeric value to its string representation.
   *
   * @param[in] jsonValue
   * The `JsonValue` representing a numeric value (either integer or float).
   * @param[in] level
   * The current level of indentation.
   *
   * @return
   * A string representation of the number.
   */
  String number_to_string(const JsonValue& jsonValue, uint64 level = 0);

  /**
   * @brief
   * Converts an integer value to its string representation.
   *
   * @param[in] jsonValue
   * The `JsonValue` representing an integer.
   * @param[in] level
   * The current level of indentation.
   *
   * @return
   * A string representation of the integer.
   */
  String integer_to_string(const JsonValue& jsonValue, uint64 level = 0);

  /**
   * @brief
   * Converts a string value to its string representation.
   *
   * @param[in] jsonValue
   * The `JsonValue` representing a string.
   * @param[in] level
   * The current level of indentation.
   *
   * @return
   * A string representation of the string (with appropriate escaping).
   */
  String string_to_string(const JsonValue& jsonValue, uint64 level = 0);

  /**
   * @brief
   * Converts an array value to its string representation.
   *
   * @param[in] jsonValue
   * The `JsonValue` representing an array.
   * @param[in] level
   * The current level of indentation.
   *
   * @return
   * A string representation of the array, with elements serialized in JSON format.
   */
  String array_to_string(const JsonValue& jsonValue, uint64 level = 0);

  /**
   * @brief
   * Converts an object value to its string representation.
   *
   * @param[in] jsonValue
   * The `JsonValue` representing an object.
   * @param[in] level
   * The current level of indentation.
   *
   * @return
   * A string representation of the object, with key-value pairs serialized in JSON format.
   */
  String object_to_string(const JsonValue& jsonValue, uint64 level = 0);

  /**
   * @brief
   * Creates indentation for nested structures.
   *
   * Generates a string consisting of spaces for indentation at the specified level.
   *
   * @param[in] level
   * The level of indentation.
   *
   * @return
   * A string with spaces for indentation.
   */
  String make_indent(uint64 level);

private:

  /** The number of spaces used for indentation (default is 2). */
  uint32 mIndentationLength;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}