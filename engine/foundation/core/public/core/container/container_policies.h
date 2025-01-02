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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/memory/memory.h"
#include "core/std/typetraits/conditional.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
struct TrivialConstructorExecutor {
  static void exec(TType* ptr) {
    core::Memory::set(ptr, 0, sizeof(TType));
  }

  static void exec(TType* ptr, core::uint32 numElements) {
    core::Memory::set(ptr, 0, sizeof(TType) * numElements);
  }
};

template<typename TType>
struct NonTrivialConstructorExecutor {
  static void exec(TType* ptr) {
    ::new(ptr) TType;
  }

  static void exec(TType* ptr, core::uint32 numElements) {
    while (numElements-- > 0) {
      ::new(ptr++) TType;
    }
  }
};

template<typename TType>
struct ConstructorExecutorPolicy {
  typedef typename core::conditional<
    core::is_trivially_constructible<TType>::value,
    TrivialConstructorExecutor<TType>,
    NonTrivialConstructorExecutor<TType>>::type type;
};

template<typename TType>
struct TrivialDestructorExecutor {
  static void exec(TType* ptr) {

  }

  static void exec(TType* ptr, core::uint32 numElements) {

  }
};

template<typename TType>
struct NonTrivialDestructorExecutor {
  static void exec(TType* ptr) {
    ptr->~TType();
  }

  static void exec(TType* ptr, core::uint32 numElements) {
    while (numElements-- > 0) {
      (ptr++)->~TType();
    }
  }
};

template<typename TType>
struct DestructorExecutorPolicy {
  typedef typename core::conditional<
    core::is_trivially_destructible<TType>::value,
    TrivialDestructorExecutor<TType>,
    NonTrivialDestructorExecutor<TType>>::type type;
};

template<typename TType>
struct TrivialCopyConstructorExecutor {
  static void exec(TType* ptr, const TType* src) {
    core::Memory::copy(ptr, src, sizeof(TType));
  }
  static void exec(TType* ptr, const TType* src, core::uint32 numElements) {
    core::Memory::copy(ptr, src, numElements * sizeof(TType));
  }
  static void exec(TType* ptr, const TType& src, core::uint32 numElements) {
    core::Memory::copy(ptr, &src, numElements * sizeof(TType));
  }
};

template<typename TType>
struct NonTrivialCopyConstructorExecutor {
  static void exec(TType* ptr, const TType* src) {
    ::new(ptr) TType(*src);
  }
  static void exec(TType* ptr, const TType* src, core::uint32 numElements) {
    while (numElements--) {
      ::new(ptr++) TType(*(src++));
    }
  }
  static void exec(TType* ptr, const TType& src, core::uint32 numElements) {
    while (numElements--) {
      ::new(ptr++) TType(src);
    }
  }
};
template<typename TType>
struct CopyConstructorExecutorPolicy {
  typedef typename core::conditional<
    core::is_trivially_copy_constructible<TType>::value,
      TrivialCopyConstructorExecutor<TType>,
      NonTrivialCopyConstructorExecutor<TType>>::type type;
};

template<typename TType>
struct TrivialMoveConstructorExecutor {
  static void exec(TType* ptr, const TType* src) {
    core::Memory::move(ptr, src, sizeof(TType));
  }
  static void exec(TType* ptr, const TType* src, core::uint32 numElements) {
    core::Memory::move(ptr, src, numElements * sizeof(TType));
  }
};

template<typename TType>
struct NonTrivialMoveConstructorExecutor {
  static void exec(TType* ptr, const TType* src) {
    ::new(ptr) TType(core::move(*src));
  }
  static void exec(TType* ptr, const TType* src, core::uint32 numElements) {
    while (numElements--) {
      ::new(ptr++) TType(core::move(*(src++)));
    }
  }
};
template<typename TType>
struct MoveConstructorExecutorPolicy {
  typedef typename core::conditional<
    core::is_trivially_move_constructible<TType>::value,
      TrivialMoveConstructorExecutor<TType>,
      NonTrivialMoveConstructorExecutor<TType>>::type type;
};

template<typename TType>
struct TrivialMoveAssignExecutor {
  static void exec(TType* ptr, const TType* src) {
    core::Memory::move(ptr, src, sizeof(TType));
  }
  static void exec(TType* ptr, const TType* src, core::uint32 numElements) {
    core::Memory::move(ptr, src, numElements * sizeof(TType));
  }
};

template<typename TType>
struct NonTrivialMoveAssignExecutor {
  static void exec(TType* ptr, const TType* src) {
    *ptr = core::move(*src);
  }
  static void exec(TType* ptr, const TType* src, core::uint32 numElements) {
    if ( ptr > src && ( ptr - src ) < numElements )
    {
      ptr += ( numElements - 1 );
      src += ( numElements - 1 );
      while ( numElements-- )
      {
        *( ptr-- ) = core::move( *( src-- ) );
      }
    }
    else
    {
      while ( numElements-- )
      {
        *( ptr++ ) = core::move( *( src++ ) );
      }
    }
  }
};
template<typename TType>
struct MoveAssignExecutorPolicy {
  typedef typename core::conditional<
    core::is_trivially_move_assignable<TType>::value,
    TrivialMoveAssignExecutor<TType>,
    NonTrivialMoveAssignExecutor<TType>>::type type;
};

template<typename TType>
struct HashPolicy {

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core