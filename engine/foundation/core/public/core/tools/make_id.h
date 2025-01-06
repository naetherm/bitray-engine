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
#include "core/std/limits.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * MakeId
 *
 * @tparam TType
 * The type of the ID values managed by this class.
 *
 * @brief
 * A generic ID maker implementation that provides mechanisms for creating, managing, and destroying unique IDs or ranges of IDs.
 *
 * This class maintains a range-based pool of IDs, allowing efficient allocation and deallocation of IDs or contiguous ranges of IDs.
 */
template<typename TType>
class MakeId {
//[-------------------------------------------------------]
//[ Public type definitions                               ]
//[-------------------------------------------------------]
public:

  /**
   * @typedef
   * ValueType
   *
   * @brief
   * Alias for the type of the IDs being managed.
   */
  typedef TType ValueType;

//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:

  /**
   * @brief
   * Constructor.
   *
   * Initializes the ID maker with an optional maximum ID limit.
   *
   * @param[in] nMaxID
   * The maximum ID that can be reached through the ID maker.
   * Defaults to the maximum value of `ValueType`.
   */
  MakeId(const ValueType nMaxID = core::numeric_limits<ValueType>::max());

  /**
   * @brief
   * Destructor.
   *
   * Releases resources used by the ID maker.
   */
  ~MakeId();

  /**
   * @brief
   * Creates a single ID.
   *
   * Allocates the next available ID from the pool.
   *
   * @param[out] nId
   * Reference to a variable where the newly allocated ID will be stored.
   *
   * @return
   * `true` if an ID was successfully created, `false` otherwise (e.g., if no IDs are available).
   */
  bool create_id(ValueType& nId);

  /**
   * @brief
   * Creates a range of IDs.
   *
   * Allocates a contiguous range of IDs from the pool.
   *
   * @param[out] nId
   * Reference to a variable where the starting ID of the allocated range will be stored.
   * @param[in] nCount
   * The number of contiguous IDs to allocate.
   *
   * @return
   * `true` if the range was successfully created, `false` otherwise (e.g., if not enough IDs are available).
   */
  bool create_range_id(ValueType& nId, const ValueType nCount);

  /**
   * @brief
   * Destroys a single ID.
   *
   * Returns a previously allocated ID back to the pool.
   *
   * @param[in] nId
   * The ID to be destroyed.
   *
   * @return
   * `true` if the ID was successfully destroyed, `false` otherwise (e.g., if the ID was not valid).
   */
  bool destroy_id(const ValueType nId);

  /**
   * @brief
   * Destroys a range of IDs.
   *
   * Returns a contiguous range of previously allocated IDs back to the pool.
   *
   * @param[in] nId
   * The starting ID of the range to be destroyed.
   * @param[in] nCount
   * The number of IDs in the range to be destroyed.
   *
   * @return
   * `true` if the range was successfully destroyed, `false` otherwise.
   */
  bool destroy_range_id(const ValueType nId, const ValueType nCount);

  /**
   * @brief
   * Checks if an ID is currently allocated.
   *
   * @param[in] nId
   * The ID to check.
   *
   * @return
   * `true` if the ID is currently allocated, `false` otherwise.
   */
  bool is_id(const ValueType nId) const;

  /**
   * @brief
   * Gets the number of available IDs.
   *
   * @return
   * The total number of IDs currently available in the pool.
   */
  ValueType get_available_ids() const;

  /**
   * @brief
   * Gets the size of the largest continuous range of available IDs.
   *
   * @return
   * The size of the largest contiguous range of IDs currently available in the pool.
   */
  ValueType get_largest_continuous_range() const;

//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:

  /**
   * @brief
   * Inserts a range of IDs into the pool.
   *
   * @param[in] nIndex
   * The index of the range to insert.
   */
  void insert_range(const ValueType nIndex);

  /**
   * @brief
   * Removes a range of IDs from the pool.
   *
   * @param[in] nIndex
   * The index of the range to remove.
   */
  void destroy_range(const ValueType nIndex);

//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:

  /**
   * @struct
   * Range
   *
   * @brief
   * Represents a contiguous range of IDs.
   */
  struct Range {
    /** The first ID in the range. */
    ValueType mFirst;
    /** The last ID in the range. */
    ValueType mLast;
  };

  /** Pointer to the array of ranges representing available ID ranges. */
  Range* mpRanges;
  /** The number of ranges currently in the pool. */
  ValueType mnNumRanges;
  /** The capacity of the ranges array. */
  ValueType mnCapacity;
};


using MakeIdInt8 = MakeId<int8>;
using MakeIdInt16 = MakeId<int16>;
using MakeIdInt32 = MakeId<int32>;
using MakeIdInt64 = MakeId<int64>;
using MakeIdUInt8 = MakeId<uint8>;
using MakeIdUInt16 = MakeId<uint16>;
using MakeIdUInt32 = MakeId<uint32>;
using MakeIdUInt64 = MakeId<uint64>;

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/tools/make_id.inl"