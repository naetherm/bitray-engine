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
#include "math/mat33.h"
#include "math/rnd_type_provider.h"
#include <core/math/mat33.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Mat33Tests::Mat33Tests()
: UnitTest("Mat33Tests") {

}

Mat33Tests::~Mat33Tests() {

}

void Mat33Tests::test() {

  /*
    // Identify
    {
      glm::mat4 glm_m = glm::identity<glm::mat4>();
      core::Mat33f re_m = core::Mat33f::Identity;

      core::float32* glm_m_ptr = glm::value_ptr(glm_m);
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          be_expect_eq(be_m[i][j], glm_m_ptr[i*4+j]);
        }
      }
    }

    // look_at
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(3, 0, 10);
      glm::vec3 glm_v1 = glm::vec3(raw_a[0], raw_a[1], raw_a[2]);
      glm::vec3 glm_v2 = glm::vec3(0, 0, 0);
      glm::vec3 glm_v3 = glm::vec3(0, 1, 0);
      core::Vec3f re_v1(raw_a[0], raw_a[1], raw_a[2]);
      core::Vec3f re_v2(0, 0, 0);
      core::Vec3f re_v3(0, 1, 0);
      glm::mat4 glm_m = glm::lookAt(glm_v1, glm_v2, glm_v3);
      core::Mat33f re_m = core::Mat33f::look_at(re_v1, re_v2, re_v3);

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          be_expect_near(re_m[i][j], glm_m[i][j], core::Math::RE_EPSILON);
        }
      }

      // Cleanup
      delete[] raw_a;
    }

    // perspective(glm::radians(45.0f), 4.0f / 3.0f, 1000.0f, 0.001f);
    {
      glm::mat4 glm_m = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 1000.0f, 0.001f);
      core::Mat33f re_m = core::Mat33f::perspective(core::Math::radians(45.0f), 4.0f / 3.0f, 1000.0f, 0.001f);

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m[i][j], glm_m[i][j], core::Math::RE_EPSILON);
        }
      }
    }

    // glm::ortho(-(800.0f / 2.0f), 800.0f / 2.0f, 600.0f / 2.0f, -(600.0f / 2.0f), -1000.0f, 1000.0f)
    {
      glm::mat4 glm_m = glm::ortho(-(800.0f / 2.0f), 800.0f / 2.0f, 600.0f / 2.0f, -(600.0f / 2.0f), -1000.0f, 1000.0f);
      core::Mat33f re_m = core::Mat33f::ortho(-(800.0f / 2.0f), 800.0f / 2.0f, 600.0f / 2.0f, -(600.0f / 2.0f), -1000.0f, 1000.0f);

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m[i][j], glm_m[i][j], core::Math::RE_EPSILON);
        }
      }
    }

    // operator ==

    // operator !=

    // operator +=(mat44)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      glm_m_a += glm_m_b;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      re_m_a += re_m_b;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_a[i][j], glm_m_a[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
      delete[] raw_b;
    }

    // operator -=(mat44)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      glm_m_a -= glm_m_b;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      re_m_a -= re_m_b;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_a[i][j], glm_m_a[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
      delete[] raw_b;
    }

    // operator *=(mat44)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      glm_m_a *= glm_m_b;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      re_m_a *= re_m_b;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_a[i][j], glm_m_a[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
      delete[] raw_b;
    }

    // operator *=(scalar)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm_m_a *= 12.24f;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      re_m_a *= 12.24f;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_a[i][j], glm_m_a[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
    }

    // operator +(mat44)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      glm::mat4 glm_m_c = glm_m_a + glm_m_b;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      core::Mat33f re_m_c = re_m_a + re_m_b;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_c[i][j], glm_m_c[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
      delete[] raw_b;
    }

    // operator -(mat44)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      glm::mat4 glm_m_c = glm_m_a - glm_m_b;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      core::Mat33f re_m_c = re_m_a - re_m_b;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_c[i][j], glm_m_c[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
      delete[] raw_b;
    }

    // operator *(mat44)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      glm::mat4 glm_m_c = glm_m_a * glm_m_b;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      core::Mat33f re_m_c = re_m_a * re_m_b;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_c[i][j], glm_m_c[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
      delete[] raw_b;
    }

    // operator mat44*mat44*mat44
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_c = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      glm::mat4 glm_m_c(raw_c[0], raw_c[1], raw_c[2], raw_c[3], raw_c[4], raw_c[5], raw_c[6], raw_c[7], raw_c[8], raw_c[9], raw_c[10], raw_c[11], raw_c[12], raw_c[13], raw_c[14], raw_c[15]);
      glm::mat4 glm_m_d = glm_m_a * glm_m_b * glm_m_c;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15]);
      core::Mat33f re_m_c(raw_c[0], raw_c[1], raw_c[2], raw_c[3], raw_c[4], raw_c[5], raw_c[6], raw_c[7], raw_c[8], raw_c[9], raw_c[10], raw_c[11], raw_c[12], raw_c[13], raw_c[14], raw_c[15]);
      core::Mat33f re_m_d = re_m_a * re_m_b * re_m_c;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_d[i][j], glm_m_d[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
      delete[] raw_b;
      delete[] raw_c;
    }

    // operator *(vec4)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(4, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::vec4 glm_v_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3]);
      glm::vec4 glm_v_c = glm_m_a * glm_v_b;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Vec4f re_v_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3]);
      core::Vec4f re_v_c = re_m_a * re_v_b;

      for (int i = 0; i < 4; i++) {
        re_expect_near(re_v_c[i], glm_v_c[i], core::Math::RE_EPSILON);
      }

      delete[] raw_a;
      delete[] raw_b;
    }

    // operator *(scalar)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_c = glm_m_a * 12.24f;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_c = re_m_a * 12.24f;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_c[i][j], glm_m_c[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
    }

    // operator vec4*(mat44)
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      core::float32* raw_b = RndFloat32TypeProvider::generate_random_data(4, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::vec4 glm_v_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3]);
      glm::vec4 glm_v_c = glm_v_b * glm_m_a;
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Vec4f re_v_b(raw_b[0], raw_b[1], raw_b[2], raw_b[3]);
      core::Vec4f re_v_c = re_v_b * re_m_a;

      for (int i = 0; i < 4; i++) {
        re_expect_near(re_v_c[i], glm_v_c[i], core::Math::RE_EPSILON);
      }

      delete[] raw_a;
      delete[] raw_b;
    }

    // transpose
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b = glm::transpose(glm_m_a);
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b = re_m_a.get_transposed();

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_a[i][j], glm_m_a[i][j], core::Math::RE_EPSILON);
        }
      }

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_b[i][j], glm_m_b[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
    }

    // invert
    {
      core::float32* raw_a = RndFloat32TypeProvider::generate_random_data(16, 0, 10);
      glm::mat4 glm_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      glm::mat4 glm_m_b = glm::inverse(glm_m_a);
      core::Mat33f re_m_a(raw_a[0], raw_a[1], raw_a[2], raw_a[3], raw_a[4], raw_a[5], raw_a[6], raw_a[7], raw_a[8], raw_a[9], raw_a[10], raw_a[11], raw_a[12], raw_a[13], raw_a[14], raw_a[15]);
      core::Mat33f re_m_b = re_m_a.get_inverted();

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_a[i][j], glm_m_a[i][j], core::Math::RE_EPSILON);
        }
      }

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          re_expect_near(re_m_b[i][j], glm_m_b[i][j], core::Math::RE_EPSILON);
        }
      }

      delete[] raw_a;
    }
  */
}

be_unittest_autoregister(Mat33Tests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests