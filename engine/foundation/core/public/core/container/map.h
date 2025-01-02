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
#include "core/container/container_policies.h"
#include "core/container/container_result.h"
#include "core/container/vector.h"
#include "core/core/config.h"
#include "core/memory/std_allocator.h"
#include "core/std/functional.h"
#include "core/std/iterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TKey, typename TValue, typename TSortPredicate = core::less<TKey>>
class Map {
public:

  typedef TKey key_type;
  typedef TValue mapped_type;

public:

  struct ElementType {
  public:

    ElementType(const TKey& key, TValue& value)
    : mKey(key)
    , mValue(value) {}


    const TKey& key() const { return mKey; }
    const TValue& value() const { return mValue; }
    TValue& value() { return mValue; }

    const TKey& first() const { return mKey; }
    const TValue& second() const { return mValue; }
    TValue& second() { return mValue; }
  private:

    const key_type& mKey;
    mapped_type& mValue;
  };

  typedef ElementType value_type;

public:

  struct ConstIterator {
  public:

    typedef core::RandomAccessIteratorTag iterator_category;
    typedef ElementType value_type;
    typedef ElementType* pointer;
    typedef ElementType& reference;
    typedef core::uint32 difference_type;

  public:

    ConstIterator();

    ConstIterator(const ConstIterator& rhs);


    ConstIterator& operator=(const ConstIterator& rhs);

    bool operator==(const ConstIterator& rhs) const;

    bool operator!=(const ConstIterator& rhs) const;

    bool operator<=(const ConstIterator& rhs) const;

    bool operator<(const ConstIterator& rhs) const;

    bool operator>=(const ConstIterator& rhs) const;

    bool operator>(const ConstIterator& rhs) const;


    const TKey& key() const;

    const TValue& value() const;

    ElementType operator*() const;

    ConstIterator& operator++();

    ConstIterator operator+(difference_type count) const;

  protected:

    ConstIterator(const Map* map, core::uint32 index);

  protected:

    Map* mMap;
    core::uint32 mIndex;

  private:

    friend class Map;
  };

  struct Iterator : public ConstIterator {
  public:

    typedef core::RandomAccessIteratorTag iterator_category;
    typedef ElementType value_type;
    typedef ElementType* pointer;
    typedef ElementType& reference;
    typedef core::uint32 difference_type;

  public:

    Iterator();

    Iterator(const Iterator& rhs);


    Iterator& operator=(const Iterator& rhs);

    bool operator==(const Iterator& rhs) const;

    bool operator!=(const Iterator& rhs) const;

    bool operator<=(const Iterator& rhs) const;

    bool operator<(const Iterator& rhs) const;

    bool operator>=(const Iterator& rhs) const;

    bool operator>(const Iterator& rhs) const;


    TKey& key();

    TValue& value();

    ElementType operator*() const;

    Iterator& operator++();

    Iterator operator+(difference_type count) const;

  protected:

    Iterator(const Map* map, core::uint32 index);

  protected:

    Map* mMap;
    core::uint32 mIndex;

  private:

    friend class Map;
  };

public:

  typedef Iterator iterator;
  typedef ConstIterator const_iterator;
  typedef ContainerResult<iterator> container_result;
  typedef TSortPredicate sort_predicate;

public:

  Map();

  Map(core::uint32 initialCapacity);

  Map(const Map& rhs);

  ~Map();


  Map& operator=(const Map& rhs);

  Map& operator=(Map&& rhs);

  bool operator==(const Map& rhs) const;

  bool operator!=(const Map& rhs) const;

  TValue& operator[](const TKey& key);

  const TValue& operator[](const TKey& key) const;


  void swap(Map& rhs);


  core::uint32 size() const;

  core::uint32 data_size() const;

  core::uint32 capacity() const;

  core::uint32 data_capacity() const;

  bool empty() const;

  void clear();

  void reserve(core::uint32 newCapacity);

  void shrink();

  bool contains(const TKey& key) const;

  template<typename TOtherType>
  bool contains(const TOtherType& key) const;


  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator cbegin() const;

  const_iterator cend() const;


  void get_keys(Vector<TKey>& keys) const;

  void get_values(Vector<TValue>& values) const;


  container_result insert(const TKey& key, const TValue& value);

  container_result insert(const TKey& key, TValue&& value);

  container_result set(const TKey& key, const TValue& value);

  container_result set(const TKey& key, TValue&& value);

  template<typename... TArgs>
  container_result emplace(const TKey& key, TArgs&&... args);


  container_result erase(const TKey& key);

  container_result erase(iterator iter);

  container_result erase(const_iterator iter);

  container_result erase_value(const TValue& value);


  template<typename TOtherType>
  iterator find(const TOtherType& key);

  template<typename TOtherType>
  const_iterator find(const TOtherType& key) const;

  template<typename TOtherType>
  bool find(const TOtherType& key, TValue& value) const;

protected:

  bool find_index(const TKey& key, core::uint32& index) const;

private:

  core::Vector<TKey> mKeys;
  core::Vector<TValue> mValues;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/container/map.inl"