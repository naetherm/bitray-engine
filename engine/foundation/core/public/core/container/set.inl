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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TSortPredicate>
Set<TType, TSortPredicate>::Set() {

}

template<typename TType, typename TSortPredicate>
Set<TType, TSortPredicate>::Set(core::uint32 capacity) {
  mData.reserve(capacity);
}

template<typename TType, typename TSortPredicate>
Set<TType, TSortPredicate>::Set(const Set& rhs)
: mData(core::Forward<container_type>(rhs.mData)) {

}

template<typename TType, typename TSortPredicate>
Set<TType, TSortPredicate>::Set(Set&& rhs)
: mData(rhs.mData) {

}

template<typename TType, typename TSortPredicate>
Set<TType, TSortPredicate>::~Set() {}


template<typename TType, typename TSortPredicate>
Set<TType, TSortPredicate>& Set<TType, TSortPredicate>::operator=(const Set& rhs) {
  mData = rhs.mData;

  return *this;
}

template<typename TType, typename TSortPredicate>
Set<TType, TSortPredicate>& Set<TType, TSortPredicate>::operator=(Set&& rhs) {
  mData = core::Forward<container_type>(rhs.mData);

  return *this;
}

template<typename TType, typename TSortPredicate>
bool Set<TType, TSortPredicate>::operator==(const Set& rhs) const {
  return (mData == rhs.mData);
}

template<typename TType, typename TSortPredicate>
bool Set<TType, TSortPredicate>::operator!=(const Set& rhs) const {
  return (mData != rhs.mData);
}


template<typename TType, typename TSortPredicate>
void Set<TType, TSortPredicate>::swap(Set& rhs) {
  mData.swap(rhs.mData);
}


template<typename TType, typename TSortPredicate>
core::uint32 Set<TType, TSortPredicate>::size() const {
  return mData.size();
}

template<typename TType, typename TSortPredicate>
core::uint32 Set<TType, TSortPredicate>::data_size() const {
  return mData.data_size();
}

template<typename TType, typename TSortPredicate>
core::uint32 Set<TType, TSortPredicate>::capacity() const {
  return mData.capacity();
}

template<typename TType, typename TSortPredicate>
core::uint32 Set<TType, TSortPredicate>::data_capacity() const {
  return mData.data_capacity();
}

template<typename TType, typename TSortPredicate>
bool Set<TType, TSortPredicate>::empty() const {
  return mData.empty();
}

template<typename TType, typename TSortPredicate>
void Set<TType, TSortPredicate>::clear() {
  mData.clear();
}

template<typename TType, typename TSortPredicate>
void Set<TType, TSortPredicate>::reserve(core::uint32 capacity) {
  mData.resize(capacity);
}

template<typename TType, typename TSortPredicate>
void Set<TType, TSortPredicate>::shrink() {
  mData.shrink();
}


template<typename TType, typename TSortPredicate>
typename Set<TType, TSortPredicate>::iterator Set<TType, TSortPredicate>::begin() {
  return mData.begin();
}

template<typename TType, typename TSortPredicate>
typename Set<TType, TSortPredicate>::iterator Set<TType, TSortPredicate>::end() {
  return mData.end();
}

template<typename TType, typename TSortPredicate>
typename Set<TType, TSortPredicate>::const_iterator Set<TType, TSortPredicate>::cbegin() const {
  return mData.begin();
}

template<typename TType, typename TSortPredicate>
typename Set<TType, TSortPredicate>::const_iterator Set<TType, TSortPredicate>::cend() const {
  return mData.end();
}

template<typename TType, typename TSortPredicate>
void Set<TType, TSortPredicate>::get_elements(Vector<TType>& outElements) const {
  outElements = mData;
}

template<typename TType, typename TSortPredicate>
Vector<TType> Set<TType, TSortPredicate>::get_elements() const {
  return mData;
}


template<typename TType, typename TSortPredicate>
typename Set<TType, TSortPredicate>::container_result Set<TType, TSortPredicate>::insert(const TType& element) {
  return mData.insert(element);
}

template<typename TType, typename TSortPredicate>
typename Set<TType, TSortPredicate>::container_result Set<TType, TSortPredicate>::insert(TType&& element) {
  return mData.insert(core::Move(element));
}

template<typename TType, typename TSortPredicate>
template<typename... TArgs>
void Set<TType, TSortPredicate>::emplace(TArgs&&... args) {
  mData.emplace_back(core::Forward<TArgs>(args)...);
}

template<typename TType, typename TSortPredicate>
template<typename TOtherType>
typename Set<TType, TSortPredicate>::container_result Set<TType, TSortPredicate>::erase(const TOtherType& element) {
  typename container_type::iterator iter = mData.find(element);
  if (iter != mData.end()) {
    return mData.erase(iter);
  }
  return container_result::failure();
}

template<typename TType, typename TSortPredicate>
typename Set<TType, TSortPredicate>::container_result Set<TType, TSortPredicate>::erase(iterator iter) {
  return mData.remove(iter);
}


template<typename TType, typename TSortPredicate>
template<typename TOtherType>
typename Set<TType, TSortPredicate>::iterator Set<TType, TSortPredicate>::find(const TOtherType& element) {
  return mData.find(element);
}

template<typename TType, typename TSortPredicate>
template<typename TOtherType>
typename Set<TType, TSortPredicate>::const_iterator Set<TType, TSortPredicate>::find(const TOtherType& element) const {
  return mData.find(element);
}

template<typename TType, typename TSortPredicate>
template<typename TOtherType>
bool Set<TType, TSortPredicate>::contains(const TOtherType& element) {
  return mData.contains(element);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core