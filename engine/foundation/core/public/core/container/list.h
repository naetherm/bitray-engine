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
#include "core/memory/std_allocator.h"
#include "core/std/algorithm.h"
#include "core/std/compressed_pair.h"
#include "core/std/base.h"
#include "core/std/iterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


template < typename TType > class List;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
class ListNode {

  //template<typename TType>
  friend class List<TType>;

public:

  ListNode();

  ListNode(ListNode&& rhs);

  ~ListNode();


  ListNode& operator=(ListNode&& rhs);


  void add(ListNode& node);

  void insert(TType& element);

  void remove();

  const TType& get_value() const;

  TType& get_value();

private:

  void unlink();

  void update();

  bool is_unlinked() const;

public:

  ListNode* mPrev;
  ListNode* mNext;
  TType mValue;
};


template<typename TType>
class List : public ListNode<TType> {
public:

  class ConstIterator {
  public:
    typedef core::BidirectionalIteratorTag		iterator_category;
    typedef TType							value_type;
    typedef ListNode<TType>         node_type;
    typedef core::int64								difference_type;
    typedef const TType*						pointer;
    typedef const TType&						reference;

    ConstIterator& operator++();
    ConstIterator& operator--();
    const TType* operator->() const { return static_cast< const TType* >( mNode->mValue ); }
    const TType& operator*() const { return mNode->mValue; }
    bool operator==( const ConstIterator& other ) const;
    bool operator!=( const ConstIterator& other ) const;

  protected:

    ConstIterator( const List* base, const ListNode<TType>* node );

    List*			mList;
    ListNode<TType>*		mNode;

    friend class List;
  };

  class Iterator {
  public:

    typedef core::BidirectionalIteratorTag		iterator_category;
    typedef TType							value_type;
    typedef core::int64								difference_type;
    typedef TType*						pointer;
    typedef TType&						reference;

    Iterator& operator++();
    Iterator& operator--();
    TType* operator->() const { return static_cast< TType* >( mNode->mValue ); }
    TType& operator*() const { return mNode->mValue; }
    bool operator==( const Iterator& other ) const;
    bool operator!=( const Iterator& other ) const;

  protected:

    Iterator( const List* base, const ListNode<TType>* node );

    List*			mList;
    ListNode<TType>*		mNode;

    friend class List;
  };

public:

  typedef Iterator iterator;
  typedef ConstIterator const_iterator;
  typedef core::ReverseIterator<iterator> reverse_iterator;
  typedef core::ReverseIterator<const_iterator> reverse_const_iterator;
  typedef core::sizeT size_type;
public:

  List();

  List(List&& rhs);

  ~List();


  List& operator=(List&& rhs);


  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator cbegin() const;

  const_iterator cend() const;

  reverse_iterator rbegin();

  reverse_iterator rend();

  reverse_const_iterator rbegin() const;

  reverse_const_iterator rend() const;

  reverse_const_iterator crbegin() const;

  reverse_const_iterator crend() const;


  bool empty() const;

  size_type size();

  TType& front() const;

  TType& back() const;

  void push_front(TType& element);

  void push_back(TType& element);

  iterator insert(iterator iter, TType& element);

  template<typename... TArgs>
  void emplace(iterator iter, TArgs&&... args);

  template<typename... TArgs>
  void emplace_back(TArgs&&... args);

  iterator remove(iterator iter);

  void pop_back();

  void pop_front();

  void clear();

private:

  size_type mSize;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/list.inl"