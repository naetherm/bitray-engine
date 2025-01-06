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
#include "container/vector.h"
#include "container/container_objects.h"
#include <core/container/vector.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
VectorTests::VectorTests()
: UnitTest("VectorTests") {

}

VectorTests::~VectorTests() {

}

void VectorTests::test() {
  {
    {
      core::Vector<int> a;

      be_expect_true(a.empty())
      be_expect_eq(0, a.size())
      be_expect_eq(0, a.capacity())
    }
    {
      core::Vector<TestObject> a;

      be_expect_true(a.empty())
      be_expect_eq(0, a.size())
      be_expect_eq(0, a.capacity())
    }
  }

  {
    {
      core::Vector<int> a;

      be_expect_true(a.empty())
      be_expect_eq(0, a.size())
      be_expect_eq(0, a.capacity())
      be_expect_eq(0, a.data_size())
      be_expect_eq(0, a.data_capacity())

      a.push_back(42);

      be_expect_false(a.empty())
      be_expect_eq(1, a.size())
      be_expect_eq(1, a.capacity())
      be_expect_eq(sizeof(int), a.data_size())
      be_expect_eq(sizeof(int), a.data_capacity())
      be_expect_eq(42, a[0]);

      a.push_back(12);

      be_expect_false(a.empty())
      be_expect_eq(2, a.size())
      be_expect_eq(2, a.capacity())
      be_expect_eq(2 * sizeof(int), a.data_size())
      be_expect_eq(2 * sizeof(int), a.data_capacity())
      be_expect_eq(42, a[0])
      be_expect_eq(12, a[1])
    }
    {

      core::Vector<TestObject> a;

      be_expect_true(a.empty())
      be_expect_eq(0, a.size())
      be_expect_eq(0, a.capacity())
      be_expect_eq(0, a.data_size())
      be_expect_eq(0, a.data_capacity())

      a.push_back(TestObject());

      be_expect_false(a.empty())
      be_expect_eq(1, a.size())
      be_expect_eq(1, a.capacity())
      be_expect_eq(sizeof(TestObject), a.data_size())
      be_expect_eq(sizeof(TestObject), a.data_capacity())
      be_expect_true(TestObject() == a[0]);
    }
  }

  {
    core::Vector<int> a;

    be_expect_true(a.empty())
    be_expect_eq(0, a.size())
    be_expect_eq(0, a.capacity())
    be_expect_eq(0, a.data_size())
    be_expect_eq(0, a.data_capacity())

    a.push_back(42);
    a.push_back(12);
    a.push_back(2);
    a.push_back(1);
    a.push_back(0);
    a.push_back(-2);

    be_expect_false(a.empty())
    be_expect_eq(6, a.size())
    be_expect_eq(8, a.capacity())
  }

  // emplace_back
  {
    core::Vector<int> a;

    be_expect_true(a.empty())
    be_expect_eq(0, a.size())
    be_expect_eq(0, a.capacity())

    a.emplace_back(42);

    be_expect_false(a.empty())
    be_expect_eq(1, a.size())
    be_expect_eq(42, a[0]);
  }
  {
    core::Vector<int> a;

    be_expect_true(a.empty())
    be_expect_eq(0, a.size())
    be_expect_eq(0, a.capacity())

    a.emplace_back(42);

    be_expect_false(a.empty())
    be_expect_eq(1, a.size())
    be_expect_eq(42, a[0]);

    a.emplace_back(32);
    a.emplace_back(22);
    a.emplace_back(12);

    be_expect_false(a.empty())
    be_expect_eq(4, a.size())
    be_expect_eq(42, a[0]);
    be_expect_eq(32, a[1]);
    be_expect_eq(22, a[2]);
    be_expect_eq(12, a[3]);
  }

  {
    core::Vector<int> a;

    be_expect_true(a.empty())
    be_expect_eq(0, a.size())
    be_expect_eq(0, a.capacity())

    for (core::uint32 i = 0; i < 100; ++i) {
      a.push_back(i);
    }

    be_expect_false(a.empty())
    be_expect_eq(100, a.size())

    core::uint32 i = 0;
    for(auto iter: a) {
      be_expect_eq(i, iter)
      i++;
    }
  }
  // Reserve
  {
    core::Vector<int> a;
    be_expect_true(a.empty())
    be_expect_eq(0, a.size())
    be_expect_eq(0, a.capacity())
    a.reserve(10);
    be_expect_true(a.empty())
    be_expect_eq(0, a.size())
    be_expect_eq(10, a.capacity())
  }
  {
    core::Vector<int> a(10);
    be_expect_false(a.empty())
    be_expect_eq(10, a.size())
    be_expect_eq(10, a.capacity())

    a[0] = 4;
    a[1] = 3;
    a[2] = 2;
    a[3] = 1;

    be_expect_eq(4, a[0]);
    be_expect_eq(3, a[1]);
    be_expect_eq(2, a[2]);
    be_expect_eq(1, a[3]);
  }
  // contains()
  {
    core::Vector<int> a;
    be_expect_true(a.empty())
    be_expect_eq(0, a.size())
    be_expect_eq(0, a.capacity())

    a.push_back(4);
    a.push_back(3);
    a.push_back(2);
    a.push_back(1);

    be_expect_eq(4, a[0]);
    be_expect_eq(3, a[1]);
    be_expect_eq(2, a[2]);
    be_expect_eq(1, a[3]);

    be_expect_true(a.contains(3))
    be_expect_false(a.contains(0))
    be_expect_false(a.contains(5))

  }
  // pop_back()
  {
    core::Vector<int> a;
    be_expect_true(a.empty())
    be_expect_eq(0, a.size())
    be_expect_eq(0, a.capacity())

    a.push_back(4);
    a.push_back(3);
    a.push_back(2);
    a.push_back(1);

    be_expect_eq(4, a[0])
    be_expect_eq(3, a[1])
    be_expect_eq(2, a[2])
    be_expect_eq(1, a[3])
    be_expect_eq(4, a.size())

    be_expect_true(a.contains(1))

    int v = a.pop_back();
    be_expect_eq(3, a.size())
    be_expect_eq(1, v)

    be_expect_false(a.contains(1))

  }
  // erase
  {
    // begin()
    {
      core::Vector<int> a(10);
      be_expect_false(a.empty())
      be_expect_eq(10, a.size())
      be_expect_eq(10, a.capacity())

      a[0] = 4;
      a[1] = 3;
      a[2] = 2;
      a[3] = 1;

      be_expect_eq(4, a[0]);
      be_expect_eq(3, a[1]);
      be_expect_eq(2, a[2]);
      be_expect_eq(1, a[3]);

      a.erase(a.begin());
      be_expect_false(a.empty())
      be_expect_eq(9, a.size())
      be_expect_eq(10, a.capacity())

      be_expect_eq(3, a[0]);
      be_expect_eq(2, a[1]);
      be_expect_eq(1, a[2]);
    }
    // inner
    {
      core::Vector<int> a(4);
      be_expect_false(a.empty())
      be_expect_eq(4, a.size())
      be_expect_eq(4, a.capacity())

      a[0] = 4;
      a[1] = 3;
      a[2] = 2;
      a[3] = 1;

      be_expect_eq(4, a[0]);
      be_expect_eq(3, a[1]);
      be_expect_eq(2, a[2]);
      be_expect_eq(1, a[3]);

      a.erase(a.begin() + 1);
      be_expect_false(a.empty())
      be_expect_eq(3, a.size())
      be_expect_eq(4, a.capacity())

      be_expect_eq(4, a[0]);
      be_expect_eq(2, a[1]);
      be_expect_eq(1, a[2]);

      a.erase(a.begin() + 1);
      be_expect_false(a.empty())
      be_expect_eq(2, a.size())
      be_expect_eq(4, a.capacity())

      be_expect_eq(4, a[0]);
      be_expect_eq(1, a[1]);
    }
    // end()
    {
      core::Vector<int> a(4);
      be_expect_false(a.empty())
      be_expect_eq(4, a.size())
      be_expect_eq(4, a.capacity())

      a[0] = 4;
      a[1] = 3;
      a[2] = 2;
      a[3] = 1;

      be_expect_eq(4, a[0]);
      be_expect_eq(3, a[1]);
      be_expect_eq(2, a[2]);
      be_expect_eq(1, a[3]);

      a.erase(a.end() - 1);
      be_expect_false(a.empty())
      be_expect_eq(3, a.size())
      be_expect_eq(4, a.capacity())

      be_expect_eq(4, a[0]);
      be_expect_eq(3, a[1]);
      be_expect_eq(2, a[2]);
    }
  }
}

be_unittest_autoregister(VectorTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests