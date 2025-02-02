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
#include "core/std/typetraits/remove_const.h"


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
template <typename T>
struct TypeStorage {

  typedef T OriginalType;
  typedef T StorageType;

  /**
   * @brief
   * Stores the original type as-is.
   *
   * @param cValue The value to be stored.
   *
   * @return The stored value.
   */
  static StorageType store(OriginalType cValue) {
    return cValue;
  }

  /**
   * @brief
   * Restores the original type from the stored type.
   *
   * @param cValue The stored value to be converted back to the original type.
   *
   * @return The original value.
   */
  static OriginalType restore(StorageType cValue) {
    return cValue;
  }

  /**
   * @brief
   * Restores the original type from the stored type as a reference.
   *
   * @param cValue The stored value to be converted back to the original type as a reference.
   *
   * @return The original value as a reference.
   */
  static OriginalType &restore_ref(StorageType &cValue) {
    return cValue;
  }

  /**
   * @brief
   * Restores the original type from the stored type as a const reference.
   *
   * @param cValue The stored value to be converted back to the original type as a const reference.
   *
   * @return The original value as a const reference.
   */
  static const OriginalType &restore_ref(const StorageType &cValue) {
    return cValue;
  }
};

template <typename T>
struct TypeStorage<T&> {

  typedef T& OriginalType;
  typedef typename TypeStorage<T*>::StorageType StorageType;

  /**
   * @brief
   * Stores the original type as a pointer to the value.
   *
   * This function takes a reference to a value and stores it as a pointer to that value.
   * This is useful for storing references into a type that is not capable of storing references.
   *
   * @param cValue The reference to be stored.
   *
   * @return The pointer to the stored value.
   */
  static StorageType store(OriginalType cValue) {
    return TypeStorage<T*>::store(&cValue);
  }

  /**
   * @brief
   * Restores the original type from the stored type.
   *
   * This function retrieves the original reference type from a pointer
   * that was stored using the `store` function.
   *
   * @param cValue The stored pointer value to be converted back to the original reference type.
   *
   * @return The reference to the original type.
   */
  static OriginalType restore(StorageType cValue) {
    return *TypeStorage<T*>::restore(cValue);
  }

  /**
   * @brief
   * Restores a reference to the original type from the stored type.
   *
   * This function retrieves the original reference type from a pointer
   * that was stored using the `store` function.
   *
   * @param cValue The stored pointer value to be converted back to the original reference type.
   *
   * @return The reference to the original type.
   */
  static OriginalType &restore_ref(StorageType &cValue) {
    return *TypeStorage<T*>::restore(cValue);
  }

  /**
   * @brief
   * Restores the original type from the stored type.
   *
   * This function retrieves the original reference type from a pointer
   * that was stored using the `store` function.
   *
   * @param cValue The stored pointer value to be converted back to the original reference type.
   *
   * @return The reference to the original type.
   */
  static const OriginalType &restore_ref(const StorageType &cValue) {
    return *TypeStorage<T*>::restore(cValue);
  }
};

template <typename T>
struct TypeStorage<const T> {

  typedef const T OriginalType;
  typedef typename core::remove_const<T>::type StorageType;

  /**
   * @brief
   * Converts a value of the original type to the stored type.
   *
   * @param cValue The value to convert.
   *
   * @return The converted value.
   */
  static StorageType store(OriginalType cValue) {
    return static_cast<StorageType>(cValue);
  }

  /**
   * @brief
   * Converts a stored value back to the original type.
   *
   * @param cValue The stored value to convert.
   *
   * @return The original value.
   */
  static OriginalType restore(StorageType cValue) {
    return cValue;
  }

  /**
   * @brief
   * Converts a stored reference value back to the original reference type.
   *
   * @param cValue The stored reference value to convert.
   *
   * @return The original reference value.
   */
  static OriginalType &restore_ref(StorageType &cValue) {
    return cValue;
  }

  /**
   * @brief
   * Converts a stored const reference value back to the original const reference type.
   *
   * @param cValue The stored const reference value to convert.
   *
   * @return The original const reference value.
   */
  static const OriginalType &restore_ref(const StorageType &cValue) {
    return cValue;
  }
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}