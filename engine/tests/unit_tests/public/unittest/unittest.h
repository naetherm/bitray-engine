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
#include "unittest/unit_tests.h"
#include <core/memory/memory.h>
#include <iostream>
#include <vector>


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
#define be_expect(CONDITION) \
  if (!(CONDITION)) {          \
    printf("[%s:%d] \"%s\" does not hold!\n", __FILE__, __LINE__, #CONDITION);                           \
  }

#define be_expect_true(CONDITION) \
  if (!(CONDITION)) {          \
    printf("[%s:%d] \"%s\" is not true!\n", __FILE__, __LINE__, #CONDITION);                           \
  }

#define be_expect_false(CONDITION) \
  if (CONDITION) {          \
    printf("[%s:%d] \"%s\" is not false!\n", __FILE__, __LINE__, #CONDITION);                           \
  }

#define be_expect_eq(A, B) \
  if (!((A) == (B))) {          \
    printf("[%s:%d] \"%s\" == \"%s\" does not hold!\n", __FILE__, __LINE__, std::to_string(A).c_str(), std::to_string(B).c_str());                           \
  }

#define be_expect_str_eq(A, B) \
  if (!(strcmp(A, B) == 0)) {          \
    printf("[%s:%d] \"%s\" == \"%s\" does not hold!\n", __FILE__, __LINE__, A, B);                           \
  }


// core::Log::instance().log(core::LogType::Error, "WRONG");                         \

#define be_expect_near(A, B, EPS) \
  if (!((A - B) <= EPS) || !((B - A) <= EPS)) { \
    printf("[%s:%d] %s != %s\n", __FILE__, __LINE__, std::to_string(A).c_str(), std::to_string(B).c_str());                                \
  }

//core::Log::instance().log(core::LogType::Error, "WRONG");                         \


#define be_unittest_autoregister(NAME) \
struct TestRegistrar##NAME { \
  TestRegistrar##NAME() { unittest::UnitTestRegistry::instance().createInstance<NAME>(); } \
};                                     \
TestRegistrar##NAME test__##NAME;


#define be_unittest_deactivate if (false)

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace unittest {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class UnitTest {
public:
  UnitTest(const char* name);

  virtual ~UnitTest();

  const char* name();

  virtual void test() = 0;

protected:

  const char* mName;
};


class UnitTestRegistry {
public:

  /**
   * @brief
   * Static instance of UnitTestRegistry.
   *
   * @return
   * Static instance of UnitTestRegistry.
   */
  static UnitTestRegistry& instance();

public:

  /**
   * @brief
   * Creates and adds an instance of test TUnitTest to list of all unit tests.
   *
   * @tparam TUnitTest
   * Unit test class to add.
   */
  template<typename TUnitTest>
  void createInstance() {
    mlstUnitTests.push_back(re_new<TUnitTest>());
  }

  /**
   * @brief
   * Runs through all registered tests.
   */
  void runAll() {
    printf("\033[32;1m= Starting Unit Tests =\033[0m\n");
    for(auto ut: mlstUnitTests) {
      printf("\033[32;1m=== Running '%s' ===\033[0m\n", ut->name());
      ut->test();
    }
  }

private:

  /**
   * @brief
   * Constructor
   */
  UnitTestRegistry() = default;

  /**
   * @brief
   * Destructor.
   */
  ~UnitTestRegistry();

private:

  std::vector<UnitTest*> mlstUnitTests;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // unittest