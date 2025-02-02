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
#include "core/rtti/type_info/type_info.h"


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
 * ReferenceTypeInfo
 *
 * @brief
 * Type information for reference types.
 *
 * Type information for reference types.
 */
class ReferenceTypeInfo : public TypeInfo {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] referenceType
   * The type info object associated with the referenced type.
   *
   * @param[in] isConst
   * If true, the referenced type is a constant type, otherwise it is a non-constant type.
   */
  ReferenceTypeInfo(TypeInfo* referenceType, bool isConst);

  /**
   * @brief
   * Destructor.
   */
  ~ReferenceTypeInfo() override;


  /**
   * @brief
   * Compare the current type info object with another type info object for equality.
   *
   * @param[in] other
   * The type info object to compare with the current type info object.
   *
   * @return
   * If the current type info object is equal to the other type info object, return true, otherwise
   * return false.
   */
  bool operator==(const TypeInfo& other) const;


  /**
   * @brief
   * Retrieve the type info object associated with the referenced type.
   *
   * @return
   * The type info object associated with the referenced type.
   */
  TypeInfo* get_reference_type();

  /**
   * @brief
   * Query if the referenced type is a constant type.
   *
   * @return
   * True if the referenced type is a constant type, otherwise false.
   */
  bool is_const() const;


  /**
   * @brief
   * Get the type info type.
   *
   * @return
   * The type info type
   */
  TypeInfoType get_type_info_type() const override;;

private:

  /** The type info object associated with the referenced type. */
  TypeInfo* mReferenceType;

  /** Flag indicating if the referenced type is a constant type. */
  bool mIsConst;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}