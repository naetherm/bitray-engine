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
#include "core/container/vector.h"
#include "core/std/any.h"


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
class ArgumentList {
public:
  /**
   * @brief
   * Default constructor
   */
  ArgumentList() {}

  /**
   * @brief
   * Default destructor
   */
  virtual ~ArgumentList() {}


  /**
   * @brief
   * Returns the number of arguments
   *
   * @return
   * Number of arguments
   */
  [[nodiscard]] sizeT get_size() const;

  /**
   * @brief
   * Adds an argument
   *
   * @param[in] argument
   * Argument to add
   */
  template<typename TType>
  void add_argument(TType&& argument);

  /**
   * @brief
   * Returns an argument
   *
   * @param[in] index
   * Index of the argument
   *
   * @return
   * Argument
   */
  template<typename TType>
  TType get_argument(sizeT index) const;

protected:
  /** List of arguments */
  Vector<Any> mArguments;
};




//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}