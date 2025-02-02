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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <typename TClass, bool PTP>
struct PointerHelper {

  static TClass *get(void* ptr) {
    return reinterpret_cast<TClass*>(ptr);
  }
};

template <typename TClass>
struct PointerHelper<TClass, true> {

  static TClass *get(void* ptr) {
    return *reinterpret_cast<TClass**>(ptr);
  }
};


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template <typename TClass, bool IS_POINTER_TO_POINTER>
Destructor Destructor::create() {
  Destructor d;
  d.mDestructorStub = &Destructor::destruct_stub<TClass, IS_POINTER_TO_POINTER>;

  return d;
}

template <typename TClass, bool IS_POINTER_TO_POINTER>
void Destructor::destruct_stub(void *ptr) {
  TClass* result = PointerHelper<TClass, IS_POINTER_TO_POINTER>::get(ptr);
  result->~TClass();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}