////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2023 RacoonStudios
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
#include "container/map.h"
#include <core/container/map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
MapTests::MapTests()
  : UnitTest("MapTests") {

}

MapTests::~MapTests() {

}

void MapTests::test() {
  // default constructor
  {
    core::Map<int, float> m;

    be_expect_true(m.empty())
    be_expect_eq(0, m.size())
  }

  // insert()
  {
    core::Map<int, float> m;

    be_expect_true(m.empty())
    be_expect_eq(0, m.size())
    be_expect_eq(0, m.data_size())

    m.insert(12, 12.24f);

    be_expect_false(m.empty())
    be_expect_eq(1, m.size())
    be_expect_eq(1, m.capacity())
    be_expect_eq(sizeof(int)+sizeof(float), m.data_size())
    be_expect_eq(sizeof(int)+sizeof(float), m.data_capacity())

    be_expect_eq(12.24f, m[12])
  }
  // emplace()
  {
    core::Map<int, float> m;

    be_expect_true(m.empty())
    be_expect_eq(0, m.size())

    m.emplace(12, 12.24f);

    be_expect_false(m.empty())
    be_expect_eq(1, m.size())

    be_expect_eq(12.24f, m[12])
  }
  // contains()
  {
    core::Map<int, float> m;

    be_expect_true(m.empty())
    be_expect_eq(0, m.size())

    m.emplace(12, 12.24f);

    be_expect_false(m.empty())
    be_expect_eq(1, m.size())

    be_expect_eq(12.24f, m[12])
    be_expect_true(m.contains(12))
    be_expect_false(m.contains(13))
  }
  // get_keys()
  {
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      m.emplace(12, 12.24f);
      core::Vector<int> v;
      m.get_keys(v);

      be_expect_false(m.empty())
      be_expect_eq(1, m.size())
      be_expect_false(v.empty())
      be_expect_eq(1, v.size())

      be_expect_eq(12, v[0])
    }
  }
  // get_values()
  {
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      m.emplace(12, 12.24f);
      core::Vector<float> v;
      m.get_values(v);

      be_expect_false(m.empty())
      be_expect_eq(1, m.size())
      be_expect_false(v.empty())
      be_expect_eq(1, v.size())

      be_expect_eq(12.24f, v[0])
      be_expect_eq(12.24f, m[12])
    }
  }
  // begin()/end()
  {
    core::Map<int, float> m;

    be_expect_true(m.empty())
    be_expect_eq(0, m.size())

    for (core::uint32 i = 0; i < 10; i++)
      m.emplace(i+1, 1.0f/(i+1));

    be_expect_false(m.empty())
    be_expect_eq(10, m.size())

    be_expect_eq(1, m[1])

    for (core::uint32 i = 0; i < 10; ++i) {
      be_expect_eq(1.0f/(i+1), m[i+1])
    }

    core::uint32 i = 0;
    for (auto iter: m) {
      be_expect_eq(i+1, iter.key())
      be_expect_eq(1.0f/(i+1), iter.value())
      ++i;
    }
  }
  // cbegin()/cend()
  {
    core::Map<int, float> m;

    be_expect_true(m.empty())
    be_expect_eq(0, m.size())

    for (core::uint32 i = 0; i < 10; i++)
      m.emplace(i+1, 1.0f/(i+1));

    be_expect_false(m.empty())
    be_expect_eq(10, m.size())

    be_expect_eq(1, m[1])

    for (core::uint32 i = 0; i < 10; ++i) {
      be_expect_eq(1.0f/(i+1), m[i+1])
    }

    core::uint32 i = 0;
    for (const auto& iter: m) {
      be_expect_eq(i+1, iter.key())
      be_expect_eq(1.0f/(i+1), iter.value())
      ++i;
    }
  }
  // find()
  {
    core::Map<int, float> m;

    be_expect_true(m.empty())
    be_expect_eq(0, m.size())

    for (core::uint32 i = 0; i < 10; i++)
      m.emplace(i+1, 1.0f/(i+1));

    be_expect_false(m.empty())
    be_expect_eq(10, m.size())

    be_expect_true(m.end() != m.find(5))
    be_expect_eq(1.0f/5.0f, m.find(5).value())
    be_expect_true(m.end() == m.find(15))


    be_expect_true(m.cend() != m.find(5))
    be_expect_eq(1.0f/5.0f, m.find(5).value())
    core::Map<int, float>::const_iterator temp_result = m.find(15);
    be_expect_true(m.cend() == temp_result)

    float v;
    be_expect_true(m.find(5, v))
    be_expect_near(1.0f/5.0f, v, 0.0001f)
  }
  // set()
  {
    core::Map<int, float> m;

    be_expect_true(m.empty())
    be_expect_eq(0, m.size())
    be_expect_eq(0, m.data_size())

    m.set(12, 12.24f);

    be_expect_false(m.empty())
    be_expect_eq(1, m.size())
    be_expect_eq(1, m.capacity())
    be_expect_eq(sizeof(int)+sizeof(float), m.data_size())
    be_expect_eq(sizeof(int)+sizeof(float), m.data_capacity())

    be_expect_eq(12.24f, m[12])

    m.set(11, 11.11f);
    be_expect_false(m.empty())
    be_expect_eq(2, m.size())
    be_expect_eq(12.24f, m[12])
    be_expect_eq(11.11f, m[11])

    m.set(11, 12.12f);
    be_expect_false(m.empty())
    be_expect_eq(2, m.size())
    be_expect_eq(12.24f, m[12])
    be_expect_eq(12.12f, m[11])
  }
  // erase()
  {
    // begin
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      for (core::uint32 i = 0; i < 10; i++)
        m.emplace(i + 1, 1.0f / (i + 1));

      be_expect_false(m.empty())
      be_expect_eq(10, m.size())

      m.erase(1);

      be_expect_false(m.empty())
      be_expect_eq(9, m.size())
    }
    // inner
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      for (core::uint32 i = 0; i < 10; i++)
        m.emplace(i + 1, 1.0f / (i + 1));

      be_expect_false(m.empty())
      be_expect_eq(10, m.size())

      m.erase(5);

      be_expect_false(m.empty())
      be_expect_eq(9, m.size())
    }
    // end
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      for (core::uint32 i = 0; i < 10; i++)
        m.emplace(i + 1, 1.0f / (i + 1));

      be_expect_false(m.empty())
      be_expect_eq(10, m.size())

      m.erase(10);

      be_expect_false(m.empty())
      be_expect_eq(9, m.size())
    }
  }
  // erase_value()
  {
    // begin
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      for (core::uint32 i = 0; i < 10; i++)
        m.emplace(i + 1, 1.0f / (i + 1));

      be_expect_false(m.empty())
      be_expect_eq(10, m.size())

      m.erase_value(1.0f);

      be_expect_false(m.empty())
      be_expect_eq(9, m.size())
    }
    // inner
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      for (core::uint32 i = 0; i < 10; i++)
        m.emplace(i + 1, 1.0f / (i + 1));

      be_expect_false(m.empty())
      be_expect_eq(10, m.size())

      m.erase_value(0.20f);

      be_expect_false(m.empty())
      be_expect_eq(9, m.size())
    }
    // invalid
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      for (core::uint32 i = 0; i < 10; i++)
        m.emplace(i + 1, 1.0f / (i + 1));

      be_expect_false(m.empty())
      be_expect_eq(10, m.size())

      m.erase_value(4.0f);

      be_expect_false(m.empty())
      be_expect_eq(10, m.size())
    }
    // end
    {
      core::Map<int, float> m;

      be_expect_true(m.empty())
      be_expect_eq(0, m.size())

      for (core::uint32 i = 0; i < 10; i++)
        m.emplace(i + 1, 1.0f / (i + 1));

      be_expect_false(m.empty())
      be_expect_eq(10, m.size())

      m.erase_value(0.10f);

      be_expect_false(m.empty())
      be_expect_eq(9, m.size())
    }
  }
}

be_unittest_autoregister(MapTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests