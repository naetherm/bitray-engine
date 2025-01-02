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
#include "core/core/config.h"
#include "core/container/vector.h"
#include "core/memory/std_allocator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TContainer = core::Vector<TType>>
class Stack {
public:

  typedef Stack<TType, TContainer> this_type;
  typedef TContainer container_type;
  typedef typename TContainer::value_type value_type;
  typedef typename TContainer::reference reference;
  typedef typename TContainer::const_reference const_reference;
  typedef typename TContainer::size_type size_type;

public:

  Stack();

  explicit Stack(const container_type& rhs);

  explicit Stack(container_type&& rhs);


  bool empty() const;

  size_type size() const;

  reference top();

  const_reference top() const;

  void push(const value_type& element);

  void push(value_type&& element);

  template<typename... TArgs>
  void emplace_back(TArgs&&... args);

  template<typename... TArgs>
  decltype(auto) emplace(TArgs&&... args);

  void pop();


  container_type & get_container();

  const container_type& get_container() const;

protected:
private:

  container_type mData;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/stack.inl"