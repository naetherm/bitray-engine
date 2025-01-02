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
template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator()
  : mMap(nullptr) {

}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator(const ConstIterator &rhs)
  : mMap(rhs.mMap), mBucketIndex(rhs.mBucketIndex), mElement(rhs.mElement) {

}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator(const HashMap *map)
  : mMap(map), mBucketIndex(0), mElement(nullptr) {
  // Move to the first element
  if (!mMap->mSize) {
    mBucketIndex = mMap->mCapacity;
  } else
    while (mBucketIndex < mMap->mCapacity) {
      if (mMap->mBuckets[mBucketIndex] != std::numeric_limits<core::uint32>::max()) {
        mElement = static_cast< BucketElement * >( mMap->mBucketsPool.get_block(mMap->mBuckets[mBucketIndex]));
        break;
      }

      ++mBucketIndex;
    }
}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator(const HashMap *map, core::uint32 bucketIndex,
                                                           const BucketElement *element)
  : mMap(map), mBucketIndex(bucketIndex), mElement(element) {

}

template<typename TKey, typename TValue, typename THash>
typename HashMap<TKey, TValue, THash>::ConstIterator &
HashMap<TKey, TValue, THash>::ConstIterator::operator=(const ConstIterator &rhs) {
  mMap = rhs.mMap;
  mBucketIndex = rhs.mBucketIndex;
  mElement = rhs.mElement;

  return *this;
}

template<typename TKey, typename TValue, typename THash>
const TKey &HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator::key() const {
  return mElement->m_key;
}

template<typename TKey, typename TValue, typename THash>
const TValue &HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator::value() const {
  return mElement->m_value;
}

template<typename TKey, typename TValue, typename THash>
const typename HashMap<TKey, TValue, THash>::ElementType
HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator::operator*() const {
  return ElementType(mElement->m_key, mElement->m_value);
}

template<typename TKey, typename TValue, typename THash>
bool HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator::operator==(const ConstIterator &rhs) const {
  return mElement = rhs.mElement;
}

template<typename TKey, typename TValue, typename THash>
bool HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator::operator!=(const ConstIterator &rhs) const {
  return !this->operator==(rhs);
}

template<typename TKey, typename TValue, typename THash>
typename HashMap<TKey, TValue, THash>::ConstIterator &
HashMap<TKey, TValue, THash>::ConstIterator::ConstIterator::operator++() {
  // Did we reach the end?
  if (mBucketIndex == mMap->mCapacity) {
    return *this;
  }

  // Check next element on the list
  if (mElement->m_nextIndex != std::numeric_limits<core::uint32>::max()) {
    mElement = static_cast< BucketElement * >( mMap->m_bucketsPool.get_block(mElement->m_nextIndex));
    return *this;
  }

  // Check next non-empty bucket
  while (++mBucketIndex < mMap->mCapacity) {
    if (mMap->mBuckets[mBucketIndex] != std::numeric_limits<core::uint32>::max()) {
      mElement = static_cast< BucketElement * >( mMap->mBucketsPool.get_block(mMap->mBuckets[mBucketIndex]));
      return *this;
    }
  }

  // Reached the end
  mElement = nullptr;
  return *this;
}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::Iterator::Iterator()
  : mMap(nullptr) {

}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::Iterator::Iterator(const Iterator &rhs)
  : mMap(rhs.mMap), mBucketIndex(rhs.mBucketIndex), mElement(rhs.mElement) {

}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::Iterator::Iterator(HashMap *map)
  : mMap(map), mBucketIndex(0), mElement(nullptr) {
  // Move to the first element
  if (!mMap->mSize) {
    mBucketIndex = mMap->mCapacity;
  } else
    while (mBucketIndex < mMap->mCapacity) {
      if (mMap->mBuckets[mBucketIndex] != std::numeric_limits<core::uint32>::max()) {
        mElement = static_cast< BucketElement * >( mMap->mBucketsPool.get_block(mMap->mBuckets[mBucketIndex]));
        break;
      }

      ++mBucketIndex;
    }
}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::Iterator::Iterator(HashMap *map, core::uint32 bucketIndex, const BucketElement *element)
  : mMap(map), mBucketIndex(bucketIndex), mElement(element) {

}

template<typename TKey, typename TValue, typename THash>
typename HashMap<TKey, TValue, THash>::Iterator &
HashMap<TKey, TValue, THash>::Iterator::operator=(const Iterator &rhs) {
  mMap = rhs.mMap;
  mBucketIndex = rhs.mBucketIndex;
  mElement = rhs.mElement;

  return *this;
}

template<typename TKey, typename TValue, typename THash>
const TKey &HashMap<TKey, TValue, THash>::Iterator::Iterator::key() const {
  return mElement->m_key;
}

template<typename TKey, typename TValue, typename THash>
TValue &HashMap<TKey, TValue, THash>::Iterator::Iterator::value() const {
  return mElement->m_value;
}

template<typename TKey, typename TValue, typename THash>
const typename HashMap<TKey, TValue, THash>::ElementType
HashMap<TKey, TValue, THash>::Iterator::Iterator::operator*() const {
  return ElementType(mElement->m_key, mElement->m_value);
}

template<typename TKey, typename TValue, typename THash>
bool HashMap<TKey, TValue, THash>::Iterator::Iterator::operator==(const Iterator &rhs) const {
  return mElement = rhs.mElement;
}

template<typename TKey, typename TValue, typename THash>
bool HashMap<TKey, TValue, THash>::Iterator::Iterator::operator!=(const Iterator &rhs) const {
  return !this->operator==(rhs);
}

template<typename TKey, typename TValue, typename THash>
typename HashMap<TKey, TValue, THash>::Iterator &HashMap<TKey, TValue, THash>::Iterator::Iterator::operator++() {
  // Did we reach the end?
  if (mBucketIndex == mMap->mCapacity) {
    return *this;
  }

  // Check next element on the list
  if (mElement->m_nextIndex != std::numeric_limits<core::uint32>::max()) {
    mElement = static_cast< BucketElement * >( mMap->m_bucketsPool.get_block(mElement->m_nextIndex));
    return *this;
  }

  // Check next non-empty bucket
  while (++mBucketIndex < mMap->mCapacity) {
    if (mMap->mBuckets[mBucketIndex] != std::numeric_limits<core::uint32>::max()) {
      mElement = static_cast< BucketElement * >( mMap->mBucketsPool.get_block(mMap->mBuckets[mBucketIndex]));
      return *this;
    }
  }

  // Reached the end
  mElement = nullptr;
  return *this;
}


template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::HashMap()
: mBuckets(nullptr)
, mSize(0)
, mCapacity(0) {

}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::HashMap(core::uint32 capacity)
: mBuckets(nullptr)
, mSize(0)
, mCapacity(0) {
  rehash(capacity);
}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::HashMap(const HashMap &rhs) {

}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::HashMap(HashMap &&rhs) {

}

template<typename TKey, typename TValue, typename THash>
HashMap<TKey, TValue, THash>::~HashMap() {

}


template<typename TKey, typename TValue, typename THash>
void HashMap<TKey, TValue, THash>::rehash(core::uint32 newCapacity) {
  // Do nothing when capacities are same
  if (newCapacity == mCapacity) {
    return;
  }

  if (newCapacity == 0) {
    // Just clear everything and leave
    re_delete(mBucketsPool.data());
    mCapacity = 0;
    return;
  }

  // Allocate the whole memory block
  const core::uint32 newBucketsPoolSize = newCapacity * sizeof( BucketElement );
  const core::uint32 newBucketsSize = newCapacity * sizeof( core::uint32 );
  const core::uint32 newMemorySize = newBucketsPoolSize + newBucketsSize;

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core