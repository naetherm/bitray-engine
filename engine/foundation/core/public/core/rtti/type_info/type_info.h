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
#include "core/rtti/rtti_types.h"
#include "core/string/string.h"


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
 * @class TypeInfo
 *
 * @brief
 * Type information.
 *
 * This class contains information about a type.
 */
class TypeInfo {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] name
   * The name of the type info object.
   */
  TypeInfo(const String& name);

  /**
   * @brief
   * Destructor.
   */
  virtual ~TypeInfo();


  /**
   * @brief
   * Compare the current type info object with another type info object for equality.
   *
   * @param[in] rhs
   * The type info object to compare with the current type info object.
   *
   * @return
   * If the current type info object is equal to the other type info object, return true, otherwise
   * return false.
   */
  bool operator==(const TypeInfo& rhs) const;

  /**
   * @brief
   * Compare the current type info object with another type info object for inequality.
   *
   * @param[in] rhs
   * The type info object to compare with the current type info object.
   *
   * @return
   * If the current type info object is not equal to the other type info object, return true, otherwise
   * return false.
   */
  bool operator!=(const TypeInfo& rhs) const;


  /**
   * @brief
   * Get the type info type.
   *
   * @return
   * The type info type
   */
  virtual TypeInfoType get_type_info_type() const = 0;


  /**
   * @brief
   * Get the name of the type.
   *
   * @return
   * The name of the type.
   */
  [[nodiscard]] const String& get_name() const;

protected:
  /** The name of the type */
  String mName;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}