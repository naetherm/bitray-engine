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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Efficiently removes an element from a container by swapping it with the last element.
 *
 * This template function is designed for containers like `core::Vector` where removing an element
 * can be made efficient by swapping it with the last element before removing it. This avoids the
 * need to shift subsequent elements, which can be costly in terms of performance.
 *
 * @tparam TType
 * The type of the container, expected to provide `begin()`, `end()`, and `pop_back()` methods,
 * such as `core::Vector`, `std::vector`.
 *
 * @param container
 * The container from which the element is to be removed.
 *
 * @param iter
 * An iterator pointing to the element to be removed.
 *
 * @return
 * An iterator pointing to the position of the element that replaced the removed element.
 *
 * @note
 * This function modifies the order of elements in the container, as it swaps the target element
 * with the last element. Use this only when element order does not matter.
 */
template<typename TType>
typename TType::iterator swizzle_vector_for_element_removal(TType& container, typename TType::iterator& iter) {
  const int index = static_cast<int>(iter - container.begin());
  *iter = container.back();
  container.pop_back();
  return container.begin() + index;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}