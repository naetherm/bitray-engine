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

template<typename TType>
MakeId<TType>::MakeId(const MakeId<TType>::ValueType nMaxID)
  : mpRanges(static_cast<MakeId<TType>::Range *>(new MakeId<TType>::Range)), mnNumRanges(1), mnCapacity(1) {
  // start with a single range, from 0 to max allowed ID (specified)
  this->mpRanges[0].mFirst = 0;
  this->mpRanges[0].mLast = nMaxID;
}

template<typename TType>
MakeId<TType>::~MakeId() {

}

template<typename TType>
bool MakeId<TType>::create_id(MakeId<TType>::ValueType &nId) {
  if (this->mpRanges[0].mFirst <= this->mpRanges[0].mLast) {
    nId = this->mpRanges[0].mFirst;

    // If current range is full and there is another one, that will become the new current range
    if (this->mpRanges[0].mFirst == this->mpRanges[0].mLast && this->mnNumRanges > 1) {
      this->destroy_range(0);
    } else {
      ++this->mpRanges[0].mFirst;
    }
    return true;
  }

  // No available ID left
  return false;
}

template<typename TType>
bool MakeId<TType>::create_range_id(MakeId<TType>::ValueType &nId, const MakeId<TType>::ValueType nCount) {
  ValueType i = 0;
  do {
    const ValueType range_count = 1u + this->mpRanges[i].mLast - this->mpRanges[i].mFirst;
    if (nCount <= range_count) {
      nId = this->mpRanges[i].mFirst;

      // If current range is full and there is another one, that will become the new current range
      if (nCount == range_count && i + 1 < this->mnNumRanges) {
        this->destroy_range(i);
      } else {
        this->mpRanges[i].mFirst += nCount;
      }
      return true;
    }
    ++i;
  } while (i < this->mnNumRanges);

  // No range of free IDs was large enough to Create the requested continuous ID sequence
  return false;
}

template<typename TType>
bool MakeId<TType>::destroy_id(const MakeId<TType>::ValueType nId) {
  return this->destroy_range_id(nId, 1);
}

template<typename TType>
bool MakeId<TType>::destroy_range_id(const MakeId<TType>::ValueType nId, const MakeId<TType>::ValueType nCount) {
  const ValueType end_id = static_cast<ValueType>(nId + nCount);

  // Binary search of the range list
  ValueType i0 = 0u;
  ValueType i1 = this->mnNumRanges - 1u;

  for (;;) {
    const ValueType i = (i0 + i1) / 2u;

    if (nId < this->mpRanges[i].mFirst) {
      // Before current range, check if neighboring
      if (end_id >= this->mpRanges[i].mFirst) {
        if (end_id != this->mpRanges[i].mFirst)
          return false; // Overlaps a range of free IDs, thus (at least partially) invalid IDs

        // Neighbor id, check if neighboring previous range too
        if (i > i0 && nId - 1 == this->mpRanges[i - 1].mLast) {
          // Merge with previous range
          this->mpRanges[i - 1].mLast = this->mpRanges[i].mLast;
          this->destroy_range(i);
        } else {
          // Just grow range
          this->mpRanges[i].mFirst = nId;
        }
        return true;
      } else {
        // Non-neighbor id
        if (i != i0) {
          // Cull upper half of list
          i1 = i - 1u;
        } else {
          // Found our position in the list, insert the deleted range here
          this->insert_range(i);
          this->mpRanges[i].mFirst = nId;
          this->mpRanges[i].mLast = end_id - 1u;
          return true;
        }
      }
    } else if (nId > this->mpRanges[i].mLast) {
      // After current range, check if neighboring
      if (nId - 1 == this->mpRanges[i].mLast) {
        // Neighbor id, check if neighboring next range too
        if (i < i1 && end_id == this->mpRanges[i + 1].mFirst) {
          // Merge with next range
          this->mpRanges[i].mLast = this->mpRanges[i + 1].mLast;
          this->destroy_range(i + 1u);
        } else {
          // Just grow range
          this->mpRanges[i].mLast += nCount;
        }
        return true;
      } else {
        // Non-neighbor id
        if (i != i1) {
          // Cull bottom half of list
          i0 = i + 1u;
        } else {
          // Found our position in the list, insert the deleted range here
          this->insert_range(i + 1u);
          this->mpRanges[i + 1].mFirst = nId;
          this->mpRanges[i + 1].mLast = end_id - 1u;
          return true;
        }
      }
    } else {
      // Inside a free block, not a valid ID
      return false;
    }

  }
}

template<typename TType>
bool MakeId<TType>::is_id(const MakeId<TType>::ValueType nId) const {
  // Binary search of the range list
  ValueType i0 = 0u;
  ValueType i1 = this->mnNumRanges - 1u;

  for (;;) {
    const ValueType i = (i0 + i1) / 2u;

    if (nId < this->mpRanges[i].mFirst) {
      if (i == i0)
        return true;

      // Cull upper half of list
      i1 = i - 1u;
    } else if (nId > this->mpRanges[i].mLast) {
      if (i == i1)
        return true;

      // Cull bottom half of list
      i0 = i + 1u;
    } else {
      // Inside a free block, not a valid ID
      return false;
    }

  }
}

template<typename TType>
typename MakeId<TType>::ValueType MakeId<TType>::get_available_ids() const {
  ValueType count = this->mnNumRanges;
  ValueType i = 0;

  do {
    count += this->mpRanges[i].mLast - this->mpRanges[i].mFirst;
    ++i;
  } while (i < this->mnNumRanges);

  return count;
}

template<typename TType>
typename MakeId<TType>::ValueType MakeId<TType>::get_largest_continuous_range() const {
  ValueType max_count = 0;
  ValueType i = 0;

  do {
    ValueType count = this->mpRanges[i].mLast - this->mpRanges[i].mFirst + 1u;
    if (count > max_count)
      max_count = count;

    ++i;
  } while (i < this->mnNumRanges);

  return max_count;
}

template<typename TType>
void MakeId<TType>::insert_range(const MakeId<TType>::ValueType nIndex) {
  if (this->mnNumRanges >= mnCapacity) {

    re_delete_array(this->mpRanges, mnCapacity);
    this->mpRanges = re_new_array<MakeId<TType>::Range>(mnCapacity + mnCapacity);
    mnCapacity += mnCapacity;
  }

  memmove(this->mpRanges + nIndex + 1, this->mpRanges + nIndex, (this->mnNumRanges - nIndex) * sizeof(Range));
  ++this->mnNumRanges;
}

template<typename TType>
void MakeId<TType>::destroy_range(const MakeId<TType>::ValueType nIndex) {
  --this->mnNumRanges;
  memmove(this->mpRanges + nIndex, this->mpRanges + nIndex + 1, (this->mnNumRanges - nIndex) * sizeof(Range));
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core