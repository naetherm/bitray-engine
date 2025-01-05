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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * MediaType
 *
 * @brief
 * Represents a media type (MIME type) consisting of a type and a subtype.
 */
class MediaType {
public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes an invalid media type.
   */
  MediaType();

  /**
   * @brief
   * Constructs a MediaType from a string representation.
   *
   * @param[in] str
   * The media type string in the format "type/subtype".
   */
  explicit MediaType(const String& str);

  /**
   * @brief
   * Constructs a MediaType with the specified type and subtype.
   *
   * @param[in] type
   * The primary type (e.g., "text").
   *
   * @param[in] subType
   * The subtype (e.g., "html").
   */
  MediaType(const String& type, const String& subType);

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * The MediaType instance to copy.
   */
  explicit MediaType(const MediaType& rhs);

  /**
   * @brief
   * Assignment operator.
   *
   * @param[in] rhs
   * The MediaType instance to assign from.
   *
   * @return
   * A reference to this instance.
   */
  MediaType& operator=(const MediaType& rhs);

  /**
   * @brief
   * Equality operator.
   *
   * @param[in] rhs
   * The MediaType instance to compare with.
   *
   * @return
   * `true` if the type and subtype match, otherwise `false`.
   */
  bool operator==(const MediaType& rhs) const;

  /**
   * @brief
   * Inequality operator.
   *
   * @param[in] rhs
   * The MediaType instance to compare with.
   *
   * @return
   * `true` if the type or subtype differs, otherwise `false`.
   */
  bool operator!=(const MediaType& rhs) const;

  /**
   * @brief
   * Checks if the media type is valid.
   *
   * @return
   * `true` if both the type and subtype are non-empty, otherwise `false`.
   */
  bool is_valid() const;

  /**
   * @brief
   * Clears the media type, making it invalid.
   */
  void clear();

  /**
   * @brief
   * Sets the media type using a string representation.
   *
   * @param[in] str
   * The media type string in the format "type/subtype".
   */
  void set(const String& str);

  /**
   * @brief
   * Sets the media type using the specified type and subtype.
   *
   * @param[in] type
   * The primary type (e.g., "application").
   *
   * @param[in] subType
   * The subtype (e.g., "json").
   */
  void set(const String& type, const String& subType);

  /**
   * @brief
   * Converts the media type to its string representation.
   *
   * @return
   * A string in the format "type/subtype".
   */
  String as_string() const;

  /**
   * @brief
   * Retrieves the primary type.
   *
   * @return
   * A reference to the primary type string.
   */
  const String& get_type() const;

  /**
   * @brief
   * Retrieves the subtype.
   *
   * @return
   * A reference to the subtype string.
   */
  const String& get_sub_type() const;

private:
  /** The primary type (e.g., "image"). */
  String mType;

  /** The subtype (e.g., "png"). */
  String mSubType;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}