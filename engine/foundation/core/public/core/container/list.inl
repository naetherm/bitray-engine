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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/memory/memory.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
ListNode<TType>::ListNode() {
  unlink();
}

template<typename TType>
ListNode<TType>::ListNode(ListNode&& rhs) {
  if (rhs.is_unlinked()) {
    unlink();
  } else {
    mPrev = rhs.mPrev;
    mNext = rhs.mNext;
    rhs.unlink();
    update();
  }

}

template<typename TType>
ListNode<TType>::~ListNode() {
  remove();
}


template<typename TType>
ListNode<TType>& ListNode<TType>::operator=(ListNode&& rhs) {
  // TODO
  return *this;
}


template<typename TType>
const TType& ListNode<TType>::get_value() const {
  return mValue;
}

template<typename TType>
TType& ListNode<TType>::get_value() {
  return mValue;
}

template<typename TType>
void ListNode<TType>::add(ListNode& node) {
  node.remove();
  mNext->mPrev = &node;
  node.mNext = mNext;
  node.mPrev = this;
  mNext = &node;
}

template<typename TType>
void ListNode<TType>::insert(TType& element) {
  ListNode<TType>* node = new ListNode();
  node->mValue = element;
  node->remove();
  mNext->mPrev = &node;
  node->mNext = mNext;
  node->mPrev = this;
  mNext = &node;
}

template<typename TType>
void ListNode<TType>::remove() {
  mPrev->mNext = mNext;
  mNext->mPrev = mPrev;
  unlink();
}

template<typename TType>
void ListNode<TType>::update() {
  mPrev->mNext = this;
  mNext->mPrev = this;
}

template<typename TType>
void ListNode<TType>::unlink() {
  mPrev = this;
  mNext = this;
}

template<typename TType>
bool ListNode<TType>::is_unlinked() const {
  return mNext == this;
}


template<typename TType>
typename List<TType>::const_iterator &List<TType>::ConstIterator::operator++() {
  if ( mNode != nullptr )
  {
    mNode = mNode->mNext;
    // did we reach the end?
    if ( mNode == mList )
    {
      mNode = nullptr;
    }
  }
  return *this;
}

template<typename TType>
typename List<TType>::const_iterator &List<TType>::ConstIterator::operator--() {
  if ( mNode == nullptr )
  {
    mNode = mList->mPrev;
  }
    // if we didn't reach the beginning yet
  else if ( mNode->mPrev != mList )
  {
    mNode = mNode->mPrev;
  }
  return *this;
}

template<typename TType>
bool List<TType>::ConstIterator::operator==(const List<TType>::ConstIterator &other) const {
  return mNode == other.mNode;
}

template<typename TType>
bool List<TType>::ConstIterator::operator!=(const List<TType>::ConstIterator &other) const {
  return !operator==(other);
}

template<typename TType>
List<TType>::ConstIterator::ConstIterator(const List *base, const ListNode<TType> *node)
: mList(const_cast<List*>(base))
, mNode(const_cast<ListNode<TType>*>(node)) {

}


template<typename TType>
typename List<TType>::iterator &List<TType>::Iterator::operator++() {
  if ( mNode != nullptr )
  {
    mNode = mNode->mNext;
    // did we reach the end?
    if ( mNode == mList )
    {
      mNode = nullptr;
    }
  }
  return *this;
}

template<typename TType>
typename List<TType>::iterator &List<TType>::Iterator::operator--() {
  if ( mNode == nullptr )
  {
    mNode = mList->mPrev;
  }
    // if we didn't reach the beginning yet
  else if ( mNode->mPrev != mList )
  {
    mNode = mNode->mPrev;
  }
  return *this;
}

template<typename TType>
bool List<TType>::Iterator::operator==(const List<TType>::Iterator &other) const {
  return mNode == other.mNode;
}

template<typename TType>
bool List<TType>::Iterator::operator!=(const List<TType>::Iterator &other) const {
  return !operator==(other);
}

template<typename TType>
List<TType>::Iterator::Iterator(const List *base, const ListNode<TType> *node)
: mList(const_cast<List*>(base))
, mNode(const_cast<ListNode<TType>*>(node)) {

}


template<typename TType>
List<TType>::List()
: ListNode<TType>()
, mSize(0) {

}

template<typename TType>
List<TType>::List(List &&rhs)
: ListNode<TType>(rhs)
, mSize(rhs.mSize) {

}

template<typename TType>
List<TType>::~List() {
  clear();
}

template<typename TType>
List<TType> &List<TType>::operator=(List &&rhs) {
  return *this;
}

template<typename TType>
typename List<TType>::iterator List<TType>::begin() {
  if (empty()) {
    return core::List<TType>::iterator(this, nullptr);
  }
  return core::List<TType>::iterator(this, this->mNext);
}

template<typename TType>
typename List<TType>::iterator List<TType>::end() {
  return core::List<TType>::iterator(this, nullptr);
}

template<typename TType>
typename List<TType>::const_iterator List<TType>::begin() const {
  if (empty()) {
    return core::List<TType>::const_iterator(this, nullptr);
  }
  return core::List<TType>::const_iterator(this, this->mNext);
}

template<typename TType>
typename List<TType>::const_iterator List<TType>::end() const {
  return core::List<TType>::const_iterator(this, nullptr);
}

template<typename TType>
typename List<TType>::const_iterator List<TType>::cbegin() const {
  if (empty()) {
    return core::List<TType>::const_iterator(this, nullptr);
  }
  return core::List<TType>::const_iterator(this, this->mNext);
}

template<typename TType>
typename List<TType>::const_iterator List<TType>::cend() const {
  return core::List<TType>::const_iterator(this, nullptr);
}

template<typename TType>
typename List<TType>::reverse_iterator List<TType>::rbegin() {
  return core::List<TType>::reverse_iterator(end());
}

template<typename TType>
typename List<TType>::reverse_iterator List<TType>::rend() {
  return core::List<TType>::reverse_iterator(begin());
}

template<typename TType>
typename List<TType>::reverse_const_iterator List<TType>::rbegin() const {
  return core::List<TType>::reverse_const_iterator(end());
}

template<typename TType>
typename List<TType>::reverse_const_iterator List<TType>::rend() const {
  return core::List<TType>::reverse_const_iterator(begin());
}

template<typename TType>
typename List<TType>::reverse_const_iterator List<TType>::crbegin() const {
  return core::List<TType>::reverse_const_iterator(cend());
}

template<typename TType>
typename List<TType>::reverse_const_iterator List<TType>::crend() const {
  return core::List<TType>::reverse_const_iterator(cbegin());
}

template<typename TType>
bool List<TType>::empty() const {
  return this->is_unlinked();
}

template<typename TType>
typename List<TType>::size_type List<TType>::size() {
  return mSize;
}

template<typename TType>
TType &List<TType>::front() const {
  return this->mNext.mValue;
}

template<typename TType>
TType &List<TType>::back() const {
  return this->mNext.mValue;
}

template<typename TType>
void List<TType>::push_front(TType &element) {
  ++mSize;
  ListNode<TType>::insert(element);
}

template<typename TType>
void List<TType>::push_back(TType &element) {
  ++mSize;
  this->mPrev->insert(element);
}

template<typename TType>
typename List<TType>::iterator List<TType>::insert(List<TType>::iterator iter, TType &element) {
  ListNode<TType> node = iter.mNode;
  if (node == nullptr) {
    node = this->mPrev;
  }
  node->insert(element);
  ++mSize;
  return core::List<TType>::iterator(this, node.mNext);
}

template<typename TType>
template<typename... TArgs>
void List<TType>::emplace(List<TType>::iterator iter, TArgs &&... args) {
  ListNode<TType> node = iter.mNode;
  if (node == nullptr) {
    node = this->mPrev;
  }
  node->insert(&(new TType(core::forward<TArgs>(args)...)));
  ++mSize;
}

template<typename TType>
template<typename... TArgs>
void List<TType>::emplace_back(TArgs &&... args) {
  ++mSize;
  this->mPrev->insert(&(new TType(core::forward<TArgs>(args)...)));
}

template<typename TType>
typename List<TType>::iterator List<TType>::remove(List<TType>::iterator iter) {
  ListNode<TType>* node = iter.mNode;
  if ( node != nullptr )
  {
    --mSize;
    ListNode<TType>* next = node->m_next;
    node->remove();
    if ( next == this )
    {
      return end();
    }
    return iterator( this, next );
  }
  return iter;
}

template<typename TType>
void List<TType>::pop_back() {
  --mSize;
  this->mPrev->remove();
}

template<typename TType>
void List<TType>::pop_front() {
  --mSize;
  this->mNext->remove();
}

template<typename TType>
void List<TType>::clear() {
  ListNode<TType>* node = this;
  do
  {
    ListNode<TType>* next = node->mNext;
    node->unlink();
    node = next;
  } while ( node != this );
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core