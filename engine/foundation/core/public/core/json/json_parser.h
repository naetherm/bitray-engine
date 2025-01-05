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
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class JsonValue;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * JsonParser
 *
 * @brief
 * A class responsible for parsing JSON data from a string input.
 *
 * The `JsonParser` class provides methods to parse a JSON string into a structured
 * `JsonValue` representation. It supports parsing various JSON data types, such as
 * objects, arrays, strings, numbers, and boolean values.
 */
class JsonParser {
public:
  /**
   * @brief
   * Static method to parse a JSON string into a `JsonValue`.
   *
   * This static method acts as a convenient entry point to parse a JSON string
   * into a `JsonValue` object. It parses the entire string and returns the parsed value.
   *
   * @param[in] inputString
   * The input string containing the JSON data to be parsed.
   *
   * @return
   * The parsed `JsonValue` representing the JSON data.
   */
  static JsonValue parse(const String& inputString);

public:
  /**
   * @brief
   * Constructor for the `JsonParser` class.
   *
   * This constructor initializes the parser with the provided JSON string.
   *
   * @param[in] inputString
   * The input string containing the JSON data to be parsed.
   */
  explicit JsonParser(const String& inputString);

  /**
   * @brief
   * Destructor for the `JsonParser` class.
   *
   * Cleans up resources used by the parser.
   */
  ~JsonParser();

  /**
   * @brief
   * Parses a JSON value from the input string.
   *
   * This method is responsible for parsing any JSON value (object, array,
   * string, number, etc.) and storing the result in the provided `JsonValue` object.
   *
   * @param[in] value
   * The `JsonValue` to store the parsed data.
   *
   * @return
   * `true` if the value was successfully parsed, `false` otherwise.
   */
  bool parse_value(JsonValue* value);

  /**
   * @brief
   * Parses a `null` value in the JSON string.
   *
   * This method parses the `null` keyword and sets the `JsonValue` to represent
   * a null value.
   *
   * @param[in] value
   * The `JsonValue` to store the parsed `null` value.
   *
   * @return
   * `true` if the null value was successfully parsed, `false` otherwise.
   */
  bool parse_null(JsonValue* value);

  /**
   * @brief
   * Parses a `false` value in the JSON string.
   *
   * This method parses the `false` keyword and sets the `JsonValue` to represent
   * a false boolean value.
   *
   * @param[in] value
   * The `JsonValue` to store the parsed `false` value.
   *
   * @return
   * `true` if the false value was successfully parsed, `false` otherwise.
   */
  bool parse_false(JsonValue* value);

  /**
   * @brief
   * Parses a `true` value in the JSON string.
   *
   * This method parses the `true` keyword and sets the `JsonValue` to represent
   * a true boolean value.
   *
   * @param[in] value
   * The `JsonValue` to store the parsed `true` value.
   *
   * @return
   * `true` if the true value was successfully parsed, `false` otherwise.
   */
  bool parse_true(JsonValue* value);

  /**
   * @brief
   * Parses a numeric value from the JSON string.
   *
   * This method parses a number (integer or floating point) and sets the
   * `JsonValue` to represent the parsed number.
   *
   * @param[in] value
   * The `JsonValue` to store the parsed number.
   *
   * @return
   * `true` if the number was successfully parsed, `false` otherwise.
   */
  bool parse_number(JsonValue* value);

  /**
   * @brief
   * Parses a JSON object from the input string.
   *
   * This method parses a JSON object and stores the result in the provided `JsonValue` object.
   * Optionally, it can treat the object as the root object of the JSON string.
   *
   * @param[in] value
   * The `JsonValue` to store the parsed object.
   * @param[in] isRootObject
   * A flag indicating whether the parsed object is the root object. Default is `false`.
   *
   * @return
   * `true` if the object was successfully parsed, `false` otherwise.
   */
  bool parse_object(JsonValue* value, bool isRootObject = false);

  /**
   * @brief
   * Parses a JSON array from the input string.
   *
   * This method parses a JSON array and stores the result in the provided `JsonValue` object.
   *
   * @param[in] value
   * The `JsonValue` to store the parsed array.
   *
   * @return
   * `true` if the array was successfully parsed, `false` otherwise.
   */
  bool parse_array(JsonValue* value);

  /**
   * @brief
   * Parses a JSON string value from the input string.
   *
   * This method parses a string enclosed in double quotes and stores the result in the
   * provided `JsonValue` object.
   *
   * @param[in] value
   * The `JsonValue` to store the parsed string.
   *
   * @return
   * `true` if the string was successfully parsed, `false` otherwise.
   */
  bool parse_string(JsonValue* value);

private:
  /**
   * @brief
   * Retrieves the next character in the input string.
   *
   * @return
   * The next character in the input string.
   */
  char next();

  /**
   * @brief
   * Retrieves the current character in the input string.
   *
   * @return
   * The current character in the input string.
   */
  char current();

  /**
   * @brief
   * Checks if a string matches a specific value.
   *
   * @param[in] str
   * The string to compare against.
   *
   * @return
   * `true` if the current character matches the string, `false` otherwise.
   */
  [[nodiscard]] bool is_string(const String& str) const;

  /**
   * @brief
   * Parses a key from the input string.
   *
   * @return
   * The parsed key as a string.
   */
  [[nodiscard]] String parse_key();

  /**
   * @brief
   * Checks if a character is whitespace.
   *
   * @param c
   * The character to check.
   *
   * @return
   * `true` if the character is whitespace, `false` otherwise.
   */
  [[nodiscard]] static bool is_whitespace(char c);

  /**
   * @brief
   * Skips over whitespace characters in the input string.
   */
  void skip_whitespace();

  /**
   * @brief
   * Checks if the current string is a multiline string.
   *
   * @return
   * `true` if the string is a multiline string, `false` otherwise.
   */
  bool is_multiline_string_quotes();

  /**
   * @brief
   * Parses a multiline string.
   *
   * @return
   * The parsed multiline string.
   */
  String parse_multiline_string();

  /**
   * @brief
   * Parses an internal string value from the input string.
   *
   * @param[in] value
   * Optional `JsonValue` to store the parsed string.
   *
   * @return
   * The parsed string.
   */
  String parse_string_internal(JsonValue* value = nullptr);

private:
  /** The input JSON string to be parsed. */
  String mInputString;
  /** The current position while parsing the string. */
  uint64 mCurrentPosition;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}