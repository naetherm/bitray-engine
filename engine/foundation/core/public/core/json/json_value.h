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
/**
 * @class
 * JsonValue
 *
 * @brief
 * A class that represents a JSON value in a type-safe manner.
 *
 * The `JsonValue` class is a container for different JSON data types, such as booleans,
 * integers, strings, arrays, objects, and null. It provides methods to access and manipulate
 * the data, check the type of the value, and convert it to other types.
 */
class JsonValue {

  friend class JsonParser; ///< The JsonParser class is a friend class, giving it access to private members.

public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes the `JsonValue` as a null value.
   */
  JsonValue();

  /**
   * @brief
   * Copy constructor.
   *
   * Creates a new `JsonValue` by copying the data from another `JsonValue`.
   *
   * @param[in] rhs
   * The `JsonValue` to copy.
   */
  JsonValue(const JsonValue& rhs);

  /**
   * @brief
   * Move constructor.
   *
   * Moves the data from another `JsonValue` into this one.
   *
   * @param[in] rhs
   * The `JsonValue` to move from.
   */
  JsonValue(JsonValue&& rhs);

  /**
   * @brief
   * Destructor.
   *
   * Cleans up the resources used by the `JsonValue`.
   */
  ~JsonValue();

  /**
   * @brief
   * Copy assignment operator.
   *
   * Assigns the data from another `JsonValue` to this one.
   *
   * @param[in] rhs
   * The `JsonValue` to copy from.
   *
   * @return
   * A reference to this `JsonValue`.
   */
  JsonValue& operator=(const JsonValue& rhs);

  /**
   * @brief
   * Move assignment operator.
   *
   * Moves the data from another `JsonValue` to this one.
   *
   * @param[in] rhs
   * The `JsonValue` to move from.
   *
   * @return
   * A reference to this `JsonValue`.
   */
  JsonValue& operator=(JsonValue&& rhs);

  /**
   * @brief
   * Accessor for array elements by index (const).
   *
   * Returns a reference to the `JsonValue` at the given index in the array.
   *
   * @param[in] index
   * The index of the array element.
   *
   * @return
   * A reference to the `JsonValue` at the specified index.
   */
  [[nodiscard]] const JsonValue& operator()(uint32 index) const;

  /**
   * @brief
   * Accessor for array elements by index.
   *
   * Returns a reference to the `JsonValue` at the given index in the array.
   *
   * @param[in] index
   * The index of the array element.
   *
   * @return
   * A reference to the `JsonValue` at the specified index.
   */
  [[nodiscard]] JsonValue& operator()(uint32 index);

  /**
   * @brief
   * Accessor for object members by key (const).
   *
   * Returns a reference to the `JsonValue` associated with the given key in the object.
   *
   * @param[in] key
   * The key of the object member.
   *
   * @return
   * A reference to the `JsonValue` associated with the key.
   */
  [[nodiscard]] const JsonValue& operator[](const String& key) const;

  /**
   * @brief
   * Accessor for object members by key.
   *
   * Returns a reference to the `JsonValue` associated with the given key in the object.
   *
   * @param[in] key
   * The key of the object member.
   *
   * @return
   * A reference to the `JsonValue` associated with the key.
   */
  [[nodiscard]] JsonValue& operator[](const String& key);

  // Testing

  /**
   * @brief
   * Checks if the value is a boolean.
   *
   * @return
   * `true` if the value is a boolean, `false` otherwise.
   */
  [[nodiscard]] inline bool is_bool() const;

  /**
   * @brief
   * Checks if the value is an integer.
   *
   * @return
   * `true` if the value is an integer, `false` otherwise.
   */
  [[nodiscard]] inline bool is_integer() const;

  /**
   * @brief
   * Checks if the value is a number (integer or floating-point).
   *
   * @return
   * `true` if the value is a number, `false` otherwise.
   */
  [[nodiscard]] inline bool is_number() const;

  /**
   * @brief
   * Checks if the value is a string.
   *
   * @return
   * `true` if the value is a string, `false` otherwise.
   */
  [[nodiscard]] inline bool is_string() const;

  /**
   * @brief
   * Checks if the value is null.
   *
   * @return
   * `true` if the value is null, `false` otherwise.
   */
  [[nodiscard]] inline bool is_null() const;

  /**
   * @brief
   * Checks if the value is an array.
   *
   * @return
   * `true` if the value is an array, `false` otherwise.
   */
  [[nodiscard]] inline bool is_array() const;

  /**
   * @brief
   * Checks if the value is an object.
   *
   * @return
   * `true` if the value is an object, `false` otherwise.
   */
  [[nodiscard]] inline bool is_object() const;

  /**
   * @brief
   * Checks if the object contains a member with the given key.
   *
   * @param[in] key
   * The key to check for in the object.
   *
   * @return
   * `true` if the object has the specified key, `false` otherwise.
   */
  [[nodiscard]] bool has_member(const String& key) const;

  /**
   * @brief
   * Checks if the value represents a raw text block.
   *
   * @return
   * `true` if the value is raw text, `false` otherwise.
   */
  [[nodiscard]] inline bool is_raw_text() const;

  // Get

  /**
   * @brief
   * Gets the array of values (const).
   *
   * Returns the array that holds the values if the type is an array.
   *
   * @return
   * The array of `JsonValue` elements.
   */
  [[nodiscard]] inline const Vector<JsonValue*>& get_array() const;

  /**
   * @brief
   * Gets the array of values.
   *
   * Returns the array that holds the values if the type is an array.
   *
   * @return
   * The array of `JsonValue` elements.
   */
  [[nodiscard]] inline Vector<JsonValue*>& get_array();

  /**
   * @brief
   * Gets the object of key-value pairs (const).
   *
   * Returns the object that holds the key-value pairs if the type is an object.
   *
   * @return
   * The object of key-value pairs.
   */
  [[nodiscard]] inline const Map<String, JsonValue*>& get_object() const;

  /**
   * @brief
   * Gets the object of key-value pairs.
   *
   * Returns the object that holds the key-value pairs if the type is an object.
   *
   * @return
   * The object of key-value pairs.
   */
  [[nodiscard]] inline Map<String, JsonValue*>& get_object();

  /**
   * @brief
   * Gets the size of the array or object.
   *
   * @return
   * The size of the array or object.
   */
  [[nodiscard]] inline core::uint32 get_size() const;

  // To

  /**
   * @brief
   * Gets the type of the JSON value.
   *
   * @return
   * The type of the value as an `EJsonType`.
   */
  [[nodiscard]] EJsonType get_type() const;

  /**
   * @brief
   * Converts the value to a boolean.
   *
   * @return
   * The boolean value.
   */
  [[nodiscard]] bool to_bool() const;

  /**
   * @brief
   * Converts the value to an integer.
   *
   * @return
   * The integer value.
   */
  [[nodiscard]] int64 to_integer() const;

  /**
   * @brief
   * Converts the value to a number (float).
   *
   * @return
   * The number value.
   */
  [[nodiscard]] float to_number() const;

  /**
   * @brief
   * Converts the value to a string (const).
   *
   * @return
   * The string value.
   */
  [[nodiscard]] const String& to_string() const;

  /**
   * @brief
   * Converts the value to an array (const).
   *
   * @return
   * The array value.
   */
  [[nodiscard]] const Vector<JsonValue*>& to_array() const;

  /**
   * @brief
   * Converts the value to an object (const).
   *
   * @return
   * The object value.
   */
  [[nodiscard]] const Map<String, JsonValue*>& to_object() const;

  /**
   * @brief
   * Converts the value to a string (mutable).
   *
   * @return
   * The string value.
   */
  [[nodiscard]] String& to_string();

  /**
   * @brief
   * Converts the value to an array (mutable).
   *
   * @return
   * The array value.
   */
  [[nodiscard]] Vector<JsonValue*>& to_array();

  /**
   * @brief
   * Converts the value to an object (mutable).
   *
   * @return
   * The object value.
   */
  [[nodiscard]] Map<String, JsonValue*>& to_object();

private:
  /**
   * @brief
   * Copy data from another `JsonValue`.
   *
   * This method handles both copying data and ensuring proper resource management.
   *
   * @param[in] rhs
   * The `JsonValue` to copy from.
   */
  void copy_from(const JsonValue& rhs);

  /**
   * @brief
   * Move data from another `JsonValue`.
   *
   * This method handles moving data from the source `JsonValue`.
   *
   * @param[in] rhs
   * The `JsonValue` to move from.
   */
  void copy_from(JsonValue&& rhs);

private:
  /** The type of the JSON value (boolean, integer, etc.). */
  EJsonType mType;

  union {
    /** Stores a boolean value. */
    bool mBool;
    /** Stores an integer value. */
    int64 mInteger;
    /** Stores a number (float). */
    float mNumber;
  };

  union {
    /** Stores an array of `JsonValue` objects. */
    Vector<JsonValue*> mArray;
    /** Stores an object of key-value pairs. */
    Map<String, JsonValue*> mObject;
    /** Stores a string value. */
    String mString;
  };

  /** Flag to indicate if the value is raw text. */
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