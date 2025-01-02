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
#include "core/std/algorithm.h"
#include "core/std/utils.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::Iterator::Iterator()
: mMap(nullptr)
, mIndex(0) {

}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::Iterator::Iterator(const Map<TKey, TValue, TSortPredicate>::Iterator &rhs)
: mMap(rhs.mMap)
, mIndex(rhs.mIndex) {

}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::Iterator &Map<TKey, TValue, TSortPredicate>::Iterator::operator=(const Map<TKey, TValue, TSortPredicate>::Iterator &rhs) {
  mMap = rhs.mMap;
  mIndex = rhs.mIndex;

  return *this;
}


template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::Iterator::operator==(const Map<TKey, TValue, TSortPredicate>::Iterator &rhs) const {
  return ((mIndex == rhs.mIndex) && (mMap == rhs.mMap));
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::Iterator::operator!=(const Map<TKey, TValue, TSortPredicate>::Iterator &rhs) const {
  return !operator==(rhs);
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::Iterator::operator<=(const Map<TKey, TValue, TSortPredicate>::Iterator &rhs) const {
  return (mIndex <= rhs.mIndex);
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::Iterator::operator<(const Map<TKey, TValue, TSortPredicate>::Iterator &rhs) const {
  return (mIndex < rhs.mIndex);
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::Iterator::operator>=(const Map<TKey, TValue, TSortPredicate>::Iterator &rhs) const {
  return (mIndex >= rhs.mIndex);
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::Iterator::operator>(const Map<TKey, TValue, TSortPredicate>::Iterator &rhs) const {
  return (mIndex > rhs.mIndex);
}


template<typename TKey, typename TValue, typename TSortPredicate>
TKey &Map<TKey, TValue, TSortPredicate>::Iterator::key() {
  return mMap->mKeys[mIndex];
}

template<typename TKey, typename TValue, typename TSortPredicate>
TValue &Map<TKey, TValue, TSortPredicate>::Iterator::value() {
  return mMap->mValues[mIndex];
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::ElementType Map<TKey, TValue, TSortPredicate>::Iterator::operator*() const {
  return ElementType(mMap->mKeys[mIndex], mMap->mValues[mIndex]);
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::Iterator &Map<TKey, TValue, TSortPredicate>::Iterator::operator++() {
  mIndex++;

  return *this;
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::Iterator Map<TKey, TValue, TSortPredicate>::Iterator::operator+(Map::Iterator::difference_type count) const {
  return Map::Iterator(mMap, mIndex + count);
}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::Iterator::Iterator(const Map *map, core::uint32 index)
: mMap(const_cast<Map*>(map))
, mIndex(index) {

}

//
template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::ConstIterator::ConstIterator()
  : mMap(nullptr)
  , mIndex(0) {

}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::ConstIterator::ConstIterator(const Map<TKey, TValue, TSortPredicate>::ConstIterator &rhs)
  : mMap(rhs.mMap)
  , mIndex(rhs.mIndex) {

}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::ConstIterator &Map<TKey, TValue, TSortPredicate>::ConstIterator::operator=(const Map<TKey, TValue, TSortPredicate>::ConstIterator &rhs) {
  mMap = rhs.mMap;
  mIndex = rhs.mIndex;

  return *this;
}


template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::ConstIterator::operator==(const Map<TKey, TValue, TSortPredicate>::ConstIterator &rhs) const {
  return ((mIndex == rhs.mIndex) && (mMap == rhs.mMap));
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::ConstIterator::operator!=(const Map<TKey, TValue, TSortPredicate>::ConstIterator &rhs) const {
  return !operator==(rhs);
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::ConstIterator::operator<=(const Map<TKey, TValue, TSortPredicate>::ConstIterator &rhs) const {
  return (mIndex <= rhs.mIndex);
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::ConstIterator::operator<(const Map<TKey, TValue, TSortPredicate>::ConstIterator &rhs) const {
  return (mIndex < rhs.mIndex);
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::ConstIterator::operator>=(const Map<TKey, TValue, TSortPredicate>::ConstIterator &rhs) const {
  return (mIndex >= rhs.mIndex);
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::ConstIterator::operator>(const Map<TKey, TValue, TSortPredicate>::ConstIterator &rhs) const {
  return (mIndex > rhs.mIndex);
}


template<typename TKey, typename TValue, typename TSortPredicate>
const TKey &Map<TKey, TValue, TSortPredicate>::ConstIterator::key() const {
  return mMap->mKeys[mIndex];
}

template<typename TKey, typename TValue, typename TSortPredicate>
const TValue &Map<TKey, TValue, TSortPredicate>::ConstIterator::value() const {
  return mMap->mValues[mIndex];
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::ElementType Map<TKey, TValue, TSortPredicate>::ConstIterator::operator*() const {
  return ElementType(key(), value());
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::ConstIterator &Map<TKey, TValue, TSortPredicate>::ConstIterator::operator++() {
  mIndex++;

  return *this;
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::const_iterator Map<TKey, TValue, TSortPredicate>::ConstIterator::operator+(Map::ConstIterator::difference_type count) const {
  return const_iterator(mMap, mIndex + count);
}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::ConstIterator::ConstIterator(const Map *map, core::uint32 index)
  : mMap(const_cast<Map*>(map))
  , mIndex(index) {

}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::Map() {

}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::Map(core::uint32 initialCapacity)
: mKeys(initialCapacity)
, mValues(initialCapacity) {
  mKeys.reserve(initialCapacity);
  mValues.reserve(initialCapacity);
}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::Map(const Map<TKey, TValue, TSortPredicate> &rhs)
: mKeys(rhs.mKeys)
, mValues(rhs.mValues) {

}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate>::~Map() {
  // Nothing to do here, the Vectors take care of their data themselves
}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate> &Map<TKey, TValue, TSortPredicate>::operator=(const Map<TKey, TValue, TSortPredicate> &rhs) {
  if (this != &rhs) {
    mKeys = rhs.mKeys;
    mValues = rhs.mValues;
  }

  return *this;
}

template<typename TKey, typename TValue, typename TSortPredicate>
Map<TKey, TValue, TSortPredicate> &Map<TKey, TValue, TSortPredicate>::operator=(Map<TKey, TValue, TSortPredicate> &&rhs) {
  if (this != &rhs) {
    mKeys = rhs.mKeys;
    mValues = rhs.mValues;
  }


  return *this;
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::operator==(const Map<TKey, TValue, TSortPredicate> &rhs) const {
  return ((mKeys == rhs.mKeys) && (mValues == rhs.mValues));
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::operator!=(const Map<TKey, TValue, TSortPredicate> &rhs) const {
  return !operator==(rhs);
}

template<typename TKey, typename TValue, typename TSortPredicate>
TValue &Map<TKey, TValue, TSortPredicate>::operator[](const TKey &key) {
  return mValues[mKeys.get_index(key)];
}

template<typename TKey, typename TValue, typename TSortPredicate>
const TValue &Map<TKey, TValue, TSortPredicate>::operator[](const TKey &key) const {
  return mValues[mKeys.get_index(key)];
}

template<typename TKey, typename TValue, typename TSortPredicate>
void Map<TKey, TValue, TSortPredicate>::swap(Map<TKey, TValue, TSortPredicate> &rhs) {

}

template<typename TKey, typename TValue, typename TSortPredicate>
core::uint32 Map<TKey, TValue, TSortPredicate>::size() const {
  return mKeys.size();
}

template<typename TKey, typename TValue, typename TSortPredicate>
core::uint32 Map<TKey, TValue, TSortPredicate>::data_size() const {
  return (sizeof(TKey) + sizeof(TValue)) * size();
}

template<typename TKey, typename TValue, typename TSortPredicate>
core::uint32 Map<TKey, TValue, TSortPredicate>::capacity() const {
  return mKeys.capacity();
}

template<typename TKey, typename TValue, typename TSortPredicate>
core::uint32 Map<TKey, TValue, TSortPredicate>::data_capacity() const {
  return (sizeof(TKey) + sizeof(TValue)) * capacity();
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::empty() const {
  return mKeys.empty();
}

template<typename TKey, typename TValue, typename TSortPredicate>
void Map<TKey, TValue, TSortPredicate>::clear() {
  mKeys.clear();
  mValues.clear();
}

template<typename TKey, typename TValue, typename TSortPredicate>
void Map<TKey, TValue, TSortPredicate>::reserve(core::uint32 newCapacity) {
  mKeys.reserve(newCapacity);
  mValues.reserve(newCapacity);
}

template<typename TKey, typename TValue, typename TSortPredicate>
void Map<TKey, TValue, TSortPredicate>::shrink() {
  mKeys.shrink();
  mValues.shrink();
}

template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::contains(const TKey& key) const {
  return find(key) != end();
}

template<typename TKey, typename TValue, typename TSortPredicate>
template<typename TOtherType>
bool Map<TKey, TValue, TSortPredicate>::contains(const TOtherType& key) const {
  return find(key) != end();
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::iterator Map<TKey, TValue, TSortPredicate>::begin() {
  return iterator(this, 0);
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::iterator Map<TKey, TValue, TSortPredicate>::end() {
  return iterator(this, size());
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::const_iterator Map<TKey, TValue, TSortPredicate>::begin() const {
  return const_iterator(this, 0);
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::const_iterator Map<TKey, TValue, TSortPredicate>::end() const {
  return const_iterator(this, size());
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::const_iterator Map<TKey, TValue, TSortPredicate>::cbegin() const {
  return const_iterator(this, 0);
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::const_iterator Map<TKey, TValue, TSortPredicate>::cend() const {
  return const_iterator(this, size());
}

template<typename TKey, typename TValue, typename TSortPredicate>
void Map<TKey, TValue, TSortPredicate>::get_keys(Vector<TKey> &keys) const {
  keys = mKeys;
}

template<typename TKey, typename TValue, typename TSortPredicate>
void Map<TKey, TValue, TSortPredicate>::get_values(Vector<TValue> &values) const {
  values = mValues;
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::insert(const TKey &key, const TValue &value) {
  core::uint32 index = 0;
  if (!find_index(key, index)) {
    mKeys.insert_at(index, key);
    mValues.insert_at(index, value);

    return container_result::success(iterator(this, index));
  }
  return container_result::failure(iterator(this, index));
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::insert(const TKey &key, TValue &&value) {
  core::uint32 index = 0;
  if (!find_index(key, index)) {
    mKeys.insert_at(index, key);
    mValues.insert_at(index, core::forward<TValue>(value));

    return container_result::success(iterator(this, index));
  }
  return container_result::failure(iterator(this, index));
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::set(const TKey &key, const TValue &value) {
  core::uint32 index = 0;
  if (!find_index(key, index)) {
    mKeys.insert_at(index, key);
    mValues.insert_at(index, value);
    //mKeys.push_back(key);
    //mValues.push_back(value);
  } else {
    mValues[index] = value;
  }
  return container_result::success(iterator(this, index));
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::set(const TKey &key, TValue &&value) {
  core::uint32 index = 0;
  if (!find_index(key, index)) {
    mKeys.insert_at(index, key);
    mValues.insert_at(index, core::forward<TValue>(value));
  } else {
    mValues[index] = core::forward<TValue>(value);
  }
  return container_result::success(iterator(this, index));
}

template<typename TKey, typename TValue, typename TSortPredicate>
template<typename... TArgs>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::emplace(const TKey &key, TArgs &&... args) {
  core::uint32 index = 0;
  if (!find_index(key, index)) {
    mKeys.insert_at(index, key);
    mValues.emplace_at(index, core::forward<TValue>(args)...);

    return container_result::success(iterator(this, index));
  }
  return container_result::failure(iterator(this, index));
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::erase(const TKey &key) {
  core::uint32 index = 0;
  if (find_index(key, index)) {
    mKeys.erase_at(index);
    mValues.erase_at(index);

    return container_result::success(iterator(this, index));
  }
  return container_result::failure(iterator(this, index));
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::erase(Map::iterator iter) {
  // We can directly access the index position through the iterator
  core::uint32 index = iter.mIndex;

  mKeys.erase_at(index);
  mValues.erase_at(index);

  return container_result::success(iterator(this, index));
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::erase(Map::const_iterator iter) {
  // We can directly access the index position through the iterator
  core::uint32 index = iter.mIndex;

  mKeys.erase_at(index);
  mValues.erase_at(index);

  return container_result::success(iterator(this, index));
}

template<typename TKey, typename TValue, typename TSortPredicate>
typename Map<TKey, TValue, TSortPredicate>::container_result Map<TKey, TValue, TSortPredicate>::erase_value(const TValue &value) {
  typename Vector<TValue>::iterator iter = core::find(mValues.begin(), mValues.end(), value);
  if (iter != mValues.end()) {
    core::uint32 index = static_cast<core::uint32>(iter - mValues.begin());
    mKeys.erase_at(index);
    mValues.erase_at(index);
    return container_result::success(iterator(this, index));
  }

  return container_result::failure(iterator());
}


template<typename TKey, typename TValue, typename TSortPredicate>
template<typename TOtherType>
typename Map<TKey, TValue, TSortPredicate>::iterator Map<TKey, TValue, TSortPredicate>::find(const TOtherType &key) {
  core::uint32 index = 0;
  if (find_index(key, index)) {
    return iterator(this, index);
  }
  return end();
}

template<typename TKey, typename TValue, typename TSortPredicate>
template<typename TOtherType>
typename Map<TKey, TValue, TSortPredicate>::const_iterator Map<TKey, TValue, TSortPredicate>::find(const TOtherType &key) const {
  core::uint32 index = 0;
  if (find_index(key, index)) {
    return const_iterator(this, index);
  }
  return cend();
}

template<typename TKey, typename TValue, typename TSortPredicate>
template<typename TOtherType>
bool Map<TKey, TValue, TSortPredicate>::find(const TOtherType &key, TValue& value) const {
  core::uint32 index = 0;
  if (find_index(key, index)) {
    value = mValues[index];
    return true;
  }
  return false;
}


template<typename TKey, typename TValue, typename TSortPredicate>
bool Map<TKey, TValue, TSortPredicate>::find_index(const TKey& key, core::uint32& index) const {
  // Find the position of `key`
  sort_predicate predicate;
  typename core::Vector<TKey>::const_iterator iter = core::lower_bound(mKeys.cbegin(), mKeys.cend(), key, predicate);
  index = static_cast<core::uint32>(iter - mKeys.begin());
  return (iter != mKeys.end() && !predicate(key, *iter));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core