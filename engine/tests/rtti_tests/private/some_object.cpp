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
#include "some_object.h"

#include <core/log/log.h>
#include <core/rtti/rtti.h>
#include <core/rtti/rtti.h>


// Alternative approach, for testing
class Rtti_SomeObject {
public:
  typedef rtti_tests::SomeObject _This;
public:
  class _Class {
    friend class rtti_tests::SomeObject;
    friend class Rtti_SomeObject;
  public:
    _Class() {}
    ~_Class() {}
    static _Class* singleton(bool get = true) {
      static bool SShutdown = false;
      static _Class* SInstance = nullptr;
      if (get) {
        if (!SInstance && !SShutdown) {
          SInstance = new _Class();
        } else {
          SShutdown = true;
          if (SInstance) {
            delete SInstance;
            SInstance = nullptr;
          }
        }
      }
      return SInstance;
    }
  };

  struct Method_do_something_Params {

  };
  class Method_do_something {
  public:
    Method_do_something() {
    }
    ~Method_do_something() {
    }
    void call(void* ptr, Method_do_something_Params& params) {
      static_cast<_This*>(ptr)->do_something(); // <--- This is the actual call>
    }
  };

  class _Guard {
  public:
    _Guard() {
      _Class::singleton();
    }
    ~_Guard() {
      _Class::singleton(false);
    }
  };
};
Rtti_SomeObject::_Guard SGuard_Rtti_SomeObject;



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rtti_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Object::Object() {
}

Object::~Object() {
}

void Object::do_something() {
  BE_LOG(Info, "Object::do_something()")
}


SomeObject::SomeObject() {
}

SomeObject::~SomeObject() {
}

void SomeObject::do_something() {
  BE_LOG(Info, "SomeObject::do_something()")
}

void SomeObject::do_something2() {
  BE_LOG(Info, "SomeObject::do_something2()")
}

void SomeObject::do_something_with_arguments(int x, float y) {
  BE_LOG(Info, "SomeObject::do_something_with_arguments(" + core::to_string(x) + ", " + core::to_string(y) + ")")
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}