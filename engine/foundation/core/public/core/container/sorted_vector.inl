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
#include "core/std/sort.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TSortPredicate>
SortedVector<TType, TSortPredicate>::SortedVector() {}

template<typename TType, typename TSortPredicate>
SortedVector<TType, TSortPredicate>::SortedVector(const SortedVector& rhs) {}

template<typename TType, typename TSortPredicate>
SortedVector<TType, TSortPredicate>::SortedVector(SortedVector&& rhs) {}

template<typename TType, typename TSortPredicate>
SortedVector<TType, TSortPredicate>::SortedVector(core::uint32 size) {}


template<typename TType, typename TSortPredicate>
SortedVector<TType, TSortPredicate>& SortedVector<TType, TSortPredicate>::operator=(const SortedVector& rhs) {}

template<typename TType, typename TSortPredicate>
SortedVector<TType, TSortPredicate>& SortedVector<TType, TSortPredicate>::operator=(SortedVector&& rh) {}

template<typename TType, typename TSortPredicate>
bool SortedVector<TType, TSortPredicate>::operator==(const SortedVector& rhs) const {
  make_clean();
  rhs.make_clean();
  return base_type::operator==(rhs);
}

template<typename TType, typename TSortPredicate>
bool SortedVector<TType, TSortPredicate>::operator!=(const SortedVector& rhs) const {
  make_clean();
  rhs.make_clean();
  return base_type::operator!=(rhs);
}

template<typename TType, typename TSortPredicate>
TType& SortedVector<TType, TSortPredicate>::operator[](core::uint32 index) {
  make_clean();
  return base_type::operator[](index);
}

template<typename TType, typename TSortPredicate>
const TType& SortedVector<TType, TSortPredicate>::operator[](core::uint32 index) const {
  make_clean();
  return base_type::operator[](index);
}


template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::swap(SortedVector& rhs) {}


template<typename TType, typename TSortPredicate>
const TType* SortedVector<TType, TSortPredicate>::data() const {
  make_clean();
  return base_type::data();
}

template<typename TType, typename TSortPredicate>
TType* SortedVector<TType, TSortPredicate>::data() {
  make_clean();
  return base_type::data();
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::clear() {
  mIsDirty = false;
  base_type::clear();
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::resize(core::uint32 size) {
  if (this->size() > 0) {
    if (size > this->size()) {
      set_dirty();
    }
  }
  base_type::resize(size);
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::resize(core::uint32 size, const TType& element) {
  if (this->size() > 0) {
    if (size > this->size()) {
      set_dirty();
    }
  }
  base_type::resize(size, element);
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::iterator SortedVector<TType, TSortPredicate>::begin() {
  make_clean();
  return base_type::begin();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::iterator SortedVector<TType, TSortPredicate>::end() {
  make_clean();
  return base_type::end();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::const_iterator SortedVector<TType, TSortPredicate>::begin() const {
  make_clean();
  return base_type::begin();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::const_iterator SortedVector<TType, TSortPredicate>::end() const {
  make_clean();
  return base_type::end();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::const_iterator SortedVector<TType, TSortPredicate>::cbegin() const {
  make_clean();
  return base_type::cbegin();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::const_iterator SortedVector<TType, TSortPredicate>::cend() const {
  make_clean();
  return base_type::cend();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::reverse_iterator SortedVector<TType, TSortPredicate>::rbegin() {
  make_clean();
  return base_type::rbegin();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::reverse_iterator SortedVector<TType, TSortPredicate>::rend() {
  make_clean();
  return base_type::rend();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::const_reverse_iterator SortedVector<TType, TSortPredicate>::crbegin() const {
  make_clean();
  return base_type::crbegin();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::const_reverse_iterator SortedVector<TType, TSortPredicate>::crend() const {
  make_clean();
  return base_type::crend();
}

template<typename TType, typename TSortPredicate>
const TType& SortedVector<TType, TSortPredicate>::front() const {
  make_clean();
  return base_type::front();
}

template<typename TType, typename TSortPredicate>
TType& SortedVector<TType, TSortPredicate>::front() {
  make_clean();
  return base_type::front();
}

template<typename TType, typename TSortPredicate>
const TType& SortedVector<TType, TSortPredicate>::back() const {

  make_clean();
  return base_type::back();
}

template<typename TType, typename TSortPredicate>
TType& SortedVector<TType, TSortPredicate>::back() {
  make_clean();
  return base_type::back();
}


template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::push_back(const TType& element) {
  base_type::push_back(element);
  set_dirty();
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::push_back(TType& element) {
  base_type::push_back(core::Forward<TType>(element));
  set_dirty();
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::push_back(const Vector<TType>& vec) {
  base_type::push_back(vec);
  set_dirty();
}

template<typename TType, typename TSortPredicate>
TType SortedVector<TType, TSortPredicate>::pop_back() {
  make_clean();
  return base_type::pop_back();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::insert(const TType& element) {
  container_result result = base_type::insert(element);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::insert(TType&& element) {
  container_result result = base_type::insert(element);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::insert(const_iterator iter, const TType& element) {
  container_result result = base_type::insert(iter, element);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::insert(const_iterator iter, TType&& element) {
  container_result result = base_type::insert(iter, element);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::insert_at(core::uint32 index, const TType& element) {
  container_result result = base_type::insert_at(index, element);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::insert_at(core::uint32 index, TType& element) {
  container_result result = base_type::insert_at(index, element);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
template<typename... TArgs>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::emplace(const_iterator iterator, TArgs&&... args) {
  container_result result = base_type::emplace(iterator, core::Forward<TArgs>(args)...);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
template<typename... TArgs>
TType& SortedVector<TType, TSortPredicate>::emplace_back(TArgs&&... args) {
  TType& element = base_type::emplace_back(core::Forward<TArgs>(args)...);
  set_dirty();
  return element;
}

template<typename TType, typename TSortPredicate>
template<typename... TArgs>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::emplace_at(const core::uint32 index, TArgs&&... args) {
  container_result result = base_type::emplace_at(index, core::Forward<TArgs>(args)...);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::erase(const TType& element) {
  make_clean();
  iterator iter = std::lower_bound(base_type::begin(), base_type::end(), element, mPredicate);
  if (iter != base_type::end() && !mPredicate(element, *iter)) {
    return base_type ::erase(iter);
  }
  return container_result::failure();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::erase_at(const core::uint32 index) {
  container_result result = base_type::erase_at(index);
  set_dirty();
  return result;
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::container_result SortedVector<TType, TSortPredicate>::erase_at(const core::uint32 first, core::uint32 last) {
  container_result result = base_type::erase_at(first, last);
  set_dirty();
  return result;
}


template<typename TType, typename TSortPredicate>
core::int32 SortedVector<TType, TSortPredicate>::get_index(const TType& element) const {
  make_clean();
  return base_type::get_index(element);
}

template<typename TType, typename TSortPredicate>
bool SortedVector<TType, TSortPredicate>::contains(const TType& element) {
  make_clean();
  return base_type::contains(element);
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::iterator SortedVector<TType, TSortPredicate>::find(const TType& element) {
  make_clean();
}

template<typename TType, typename TSortPredicate>
typename SortedVector<TType, TSortPredicate>::const_iterator SortedVector<TType, TSortPredicate>::find(const TType& element) const {
  make_clean();
}


template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::grow(core::uint32 numOfElements) {
  if (this->size() > 0 && numOfElements > 0) {
    set_dirty();
  }
  base_type::grow(numOfElements);
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::sort() {
  // TODO(naetherm): Implement this!
  core::sort(base_type::begin(), base_type::end(), mPredicate);
  mIsDirty = false;
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::insertion_sort() {
  // TODO(naetherm): Implement this!
  core::stable_sort(base_type::begin(), base_type::end(), mPredicate);
  mIsDirty = false;
}

template<typename TType, typename TSortPredicate>
bool SortedVector<TType, TSortPredicate>::is_sorted() const {
  return core::is_sorted(base_type::begin(), base_type::end(), mPredicate);
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::make_clean() {
  if (is_dirty()) {
    insertion_sort();
  }
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::make_unique() {
  // TODO(naetherm): Implement this!
  const auto begin = this->begin();
  const auto end = this->end();
  if (begin != end) {
    core::stable_sort(begin, end);
    auto iter = std::unique(begin, end);
    erase(iter, end);
  }
  mIsDirty = false;
}

template<typename TType, typename TSortPredicate>
void SortedVector<TType, TSortPredicate>::set_dirty() {
  mIsDirty = true;
}
template<typename TType, typename TSortPredicate>
bool SortedVector<TType, TSortPredicate>::is_dirty() const {
  return mIsDirty;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core