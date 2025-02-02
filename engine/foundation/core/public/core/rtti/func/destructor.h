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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Destructor
 *
 * @brief
 * A destructor is used to delete a class instance of a given type.
 */
class Destructor {
public:

  /**
   * @brief
   * Creates a new destructor that destroys a class instance with the given type.
   *
   * @tparam TClass
   * The type of the class which is destroyed.
   *
   * @tparam TPointerToPointer
   * A boolean flag which indicates whether the pointer to be destroyed is a pointer
   * to a pointer (i.e. a pointer to a pointer to the class instance).
   *
   * @return
   * A new instance of the destructor.
   */
  template<typename TClass, bool TPointerToPointer = false>
  static Destructor create();

public:

  /**
   * @brief
   * Creates a new destructor with no associated destruction function.
   */
  Destructor();


  /**
   * @brief
   * Destroys an object of the type that is associated with this destructor.
   *
   * @param ptr
   * A pointer to the object which will be destroyed.
   */
  void destroy(void* ptr) const;

private:

  /**
   * @brief
   * A stub that calls the destructor of a class.
   *
   * @param ptr
   * A pointer to the object which will be destroyed.
   */
  template<typename TClass, bool TPointerToPointer>
  static void destruct_stub(void* ptr);

private:

  /**
   * @brief
   * A function pointer that points to a destructor function.
   */
  typedef void (*DestructorStub)(void*);

private:

  /**
   * @brief
   * A function pointer that points to a destructor function.
   *
   * This pointer is set to a valid destructor function when the object is
   * created using the create() function. Otherwise, it is set to
   * nullptr when the object is default constructed.
   */
  DestructorStub mDestructorStub;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/func/destructor.inl"