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
#include "core/container/hash_map.h"
#include "core/string/string.h"
#include "core/rtti/tools/dynamic_object.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


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
 * RttiMember
 *
 * @brief
 * The base class for all members of a class in the RTTI system.
 *
 * This class is the base class for all members of a class in the RTTI system.
 * It provides a common interface for all members of a class, such as fields,
 * methods and properties.
 */
class RttiMember {

  template<typename TClass> friend class ClassBuilder;
  template<typename TClass> friend class EnumBuilder;

public:

  /**
   * @brief
   * Default constructor for the RttiMember class.
   */
  RttiMember();

  /**
   * @brief
   * The destructor for `RttiMember`.
   *
   * Cleans up any dynamically allocated memory used by the `RttiMember`.
   */
  virtual ~RttiMember();


  /**
   * @brief
   * Returns true if the RttiMember has a tag with the given name.
   *
   * @param name The name of the tag to search for.
   *
   * @return true if the RttiMember has a tag with the given name, and false otherwise.
   */
  [[nodiscard]] bool has_tag(const String& name) const;

  /**
   * @brief
   * Retrieves the value of the tag with the given name.
   *
   * @param name The name of the tag to retrieve.
   *
   * @return A reference to the DynamicObject associated with the tag.
   */
  [[nodiscard]] const DynamicObject& get_tag(const String& name) const;

protected:

  /**
   * @brief
   * Adds a tag to the RttiMember.
   *
   * @param name The name of the tag.
   * @param value The value of the tag.
   */
  void add_tag(const String& name, const DynamicObject& value);

protected:

  /** A map of string to dynamic object that stores all the tags that have been added to this RttiMember. */
  core::hash_map<String, DynamicObject> mTagsMap;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}